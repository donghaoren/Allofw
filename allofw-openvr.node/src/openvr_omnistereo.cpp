// liballofw: A lightweight framework for graphics.
// See LICENSE.md for license information.
//
// omnistereo.cpp
//
// Implements Omnistereo rendering class.
//
// Author(s):
//   Donghao Ren (donghaoren@cs.ucsb.edu)
//


#include "openvr_omnistereo.h"
#include <allofw/utils.h>
#include <openvr.h>
#include <string>
#include <cmath>
#include <vector>

ALLOFW_NS_BEGIN

// Shader code.
namespace {
	const char* gShader_omnistereo_include =
		R"================(
// OmniStereo Shader Code:
// Pose information
uniform mat4 omni_mvp_matrix;
uniform vec3 omni_position;
uniform vec4 omni_rotation;
// This is half of the eye separation (distance between the origin and one eye).
uniform float omni_eye;
uniform float omni_radius;
uniform vec4 omni_viewport_rotation;
uniform vec4 omni_viewport_projection;
// Multiply quaternion with vector.
vec4 omni_quat_multiply(vec4 q1, vec4 q2) {
    return vec4(
        q1.w * q2.xyz + q2.w * q1.xyz + cross(q1.xyz, q2.xyz),
        q1.w * q2.w - dot(q1.xyz, q2.xyz)
    );
}
// Rotate vector v with quaternion q.
// Assumption: |q| = 1.
vec3 omni_quat_rotate(vec4 q, vec3 v) {
    float d = dot(q.xyz, v);
    vec3 c = cross(q.xyz, v);
    return q.w * q.w * v + (q.w + q.w) * c + d * q.xyz - cross(c, q.xyz);
}
vec3 omni_quat_inverse_rotate(vec4 q, vec3 v) {
    float d = dot(q.xyz, v);
    vec3 c = cross(q.xyz, v);
    return q.w * q.w * v - (q.w + q.w) * c + d * q.xyz - cross(c, q.xyz);
}
// Transform coordinates to camera space.
vec3 omni_transform(vec3 v) {
    return omni_quat_inverse_rotate(omni_rotation, v - omni_position);
}
// Transform normal to camera space.
vec3 omni_transform_normal(vec3 v) {
    return omni_quat_inverse_rotate(omni_rotation, v);
}
// Perform Omnistereo displacement on vertex.
vec3 omni_displace(vec3 vertex) {
    return vertex;
}
vec4 omni_project(vec3 v) {
    return omni_mvp_matrix * vec4(v, 1.0);
}
vec4 omni_render(vec3 v) {
    return omni_project(omni_displace(v));
}
// END OmniStereo Shader Code.
)================";


const Quaternion kVPRotation_Positive_X = Quaternion::Rotation(Vector3(0, 0, 1), PI) * Quaternion::Rotation(Vector3(0, 1, 0), +PI / 2);
const Quaternion kVPRotation_Negative_X = Quaternion::Rotation(Vector3(0, 0, 1), PI) * Quaternion::Rotation(Vector3(0, 1, 0), -PI / 2);
const Quaternion kVPRotation_Positive_Y = Quaternion::Rotation(Vector3(1, 0, 0), -PI / 2);
const Quaternion kVPRotation_Negative_Y = Quaternion::Rotation(Vector3(1, 0, 0), +PI / 2);
const Quaternion kVPRotation_Positive_Z = Quaternion::Rotation(Vector3(1, 0, 0), PI);
const Quaternion kVPRotation_Negative_Z = Quaternion::Rotation(Vector3(0, 0, 1), PI);


struct FrameBufferInfo {
	GLuint frameBuffer;
	GLuint renderTexture;
	GLuint depthBuffer;
	GLuint resolveFrameBuffer;
	GLuint resolveTexture;
};

}

class OpenVROmniStereoImpl : public OpenVROmniStereo {
public:
    OpenVROmniStereoImpl() {
        LoggerScope logger(Logger::kInfo, "OpenVROmniStereo::Initialization");

		pose_ = Pose();

        vr::EVRInitError error;

		vr_system_ = vr::VR_Init(&error, vr::VRApplication_Scene);

		if(error != vr::VRInitError_None) {
			throw exception("could not init openvr runtime");
		}

		vr_render_models_ = (vr::IVRRenderModels*)vr::VR_GetGenericInterface(vr::IVRRenderModels_Version, &error);
		if(!vr_render_models_) {
			throw exception("could not get render model interface");
		}

		std::string tracking_system_name = getTrackedDeviceString(vr_system_, vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_TrackingSystemName_String);
		std::string serial_number = getTrackedDeviceString(vr_system_, vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_SerialNumber_String);

		logger.printf("VR Initialized: %s / %s", tracking_system_name.c_str(), serial_number.c_str());

        logger.printf("Creating framebuffers for rendering");

		vr_system_->GetRecommendedRenderTargetSize(&render_buffer_width_, &render_buffer_height_);
		logger.printf("Framebuffer size is: %dx%d", (int)render_buffer_width_, (int)render_buffer_height_);
		leftEye_ = createFrameBuffer(render_buffer_width_, render_buffer_height_);
		rightEye_ = createFrameBuffer(render_buffer_width_, render_buffer_height_);

		logger.printf("Setup cameras");
		setupCameras();
		setupFullscreenQuads();
    }

    // Set cubemap resolution, allocate the cubemap.
    virtual void setResolution(int size) override {
        // Use VR's default resolution
    }

    // Enable stereo or not.
    virtual void setStereoMode(StereoMode stereo_mode) override {
        // VR is always stereo
    }

    // Pose and eye separation.
    virtual void setPose(const Pose& pose) override {
        pose_ = pose;
    }
    virtual void setLens(float eye_separation = 0.0f, float sphere_radius = 1.0f) override {
        throw not_implemented_yet();
    }
    virtual void setClipRange(float near, float far) override {
        throw not_implemented_yet();
    }

    // Get the cubemap.
    virtual StereoTexture getCubemapTexture() override {
        throw not_implemented_yet();
    }
    virtual StereoTexture getDepthCubemapTexture() override {
        throw not_implemented_yet();
    }

    virtual void capture() override {
		Delegate::CaptureInfo info;
		updateHMDPose();

		glEnable(GL_MULTISAMPLE);
        // Left eye capture.
		glBindFramebuffer(GL_FRAMEBUFFER, leftEye_.frameBuffer);
		glViewport(0, 0, render_buffer_width_, render_buffer_height_);


		info.eye_separation = 0.065;
		info.omnistereo = this;
		info.pose = pose_;

		info.eye = kEye_Left;
		delegate_->onCaptureViewport(info);

		glBindFramebuffer(GL_FRAMEBUFFER, rightEye_.frameBuffer);
		glViewport(0, 0, render_buffer_width_, render_buffer_height_);

		info.eye = kEye_Right;
		delegate_->onCaptureViewport(info);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glDisable(GL_MULTISAMPLE);

		glBindFramebuffer(GL_READ_FRAMEBUFFER, leftEye_.frameBuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, leftEye_.resolveFrameBuffer);
		glBlitFramebuffer(0, 0, render_buffer_width_, render_buffer_height_, 0, 0, render_buffer_width_, render_buffer_height_, GL_COLOR_BUFFER_BIT, GL_LINEAR);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, rightEye_.frameBuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, rightEye_.resolveFrameBuffer);
		glBlitFramebuffer(0, 0, render_buffer_width_, render_buffer_height_, 0, 0, render_buffer_width_, render_buffer_height_, GL_COLOR_BUFFER_BIT, GL_LINEAR);

		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    }

    virtual void composite(const Rectangle2i& viewport, const CompositeInfo& info) override {
		renderAuxWindow(viewport);
		submitFrame();
    }

    // Create a customized composite shader.
    virtual GLuint compositeCustomizeShader(const char* code) override {
        throw not_implemented_yet();
    }
    // Restore the composite shader to default.
    virtual void compositeRestoreShader() override {
        throw not_implemented_yet();
    }

    // Necessary functions and uniforms for use in shader programs.
    // vec4 omni_transform(vec4 v)          : Transform coordinates to camera space.
    // vec3 omni_transform_normal(vec3 v)   : Transform normal to camera space.
    // vec4 omni_displace(vec4 v)           : Perform Omnistereo displacement on vertex.
    // vec4 omni_project(vec4 v)            : Perform projection to cubemap face.
    // vec4 omni_render(vec4 v) : Short for 'omni_project(omni_displace(v))'
    // Uniforms:
    // omni_eye, omni_radius
    // omni_near, omni_far
    // omni_position, omni_rotation
    // These uniforms are automatically set before calling capture().
    virtual const char* getShaderCode() override {
        return gShader_omnistereo_include;
    }

    virtual void setUniforms(GLuint program, const Delegate::CaptureInfo& info) override {
		Matrix4f matrix_mvp;
		if(info.eye == kEye_Left) {
			matrix_mvp = projection_left_eye_ * pose_left_eye_ * pose_matrix_HMD_;
		}
		if(info.eye == kEye_Right) {
			matrix_mvp = projection_right_eye_ * pose_right_eye_ * pose_matrix_HMD_;
		}
		GLuint l;
		if ((l = glGetUniformLocation(program, "omni_mvp_matrix")) >= 0) {
			glProgramUniformMatrix4fv(program, l, 1, GL_FALSE, matrix_mvp.data());
		}
		if((l = glGetUniformLocation(program, "omni_position")) >= 0) {
			glProgramUniform3f(program, l, info.pose.position.x, info.pose.position.y, info.pose.position.z);
		}
		if((l = glGetUniformLocation(program, "omni_rotation")) >= 0) {
			glProgramUniform4f(program, l, info.pose.rotation.x, info.pose.rotation.y, info.pose.rotation.z, info.pose.rotation.w);
		}
    }

	virtual Pose getHeadPose() {
		Quaternion rotation;
		Vector3 position;

		position.x = pose_matrix_HMD_.a14;
		position.y = pose_matrix_HMD_.a24;
		position.z = pose_matrix_HMD_.a34;

		rotation.w = sqrt(max(0.0, 1.0 + pose_matrix_HMD_.a11 + pose_matrix_HMD_.a22 + pose_matrix_HMD_.a33 )) / 2.0;
		rotation.x = sqrt(max(0.0, 1.0 + pose_matrix_HMD_.a11 - pose_matrix_HMD_.a22 - pose_matrix_HMD_.a33 )) / 2.0;
		rotation.y = sqrt(max(0.0, 1.0 - pose_matrix_HMD_.a11 + pose_matrix_HMD_.a22 - pose_matrix_HMD_.a33 )) / 2.0;
		rotation.z = sqrt(max(0.0, 1.0 - pose_matrix_HMD_.a11 - pose_matrix_HMD_.a22 + pose_matrix_HMD_.a33 )) / 2.0;
		rotation.x = copysign(rotation.x, pose_matrix_HMD_.a32 - pose_matrix_HMD_.a23);
		rotation.y = copysign(rotation.y, pose_matrix_HMD_.a13 - pose_matrix_HMD_.a31);
		rotation.z = copysign(rotation.z, pose_matrix_HMD_.a21 - pose_matrix_HMD_.a12);

		return Pose(position, rotation);
	}

    // Set the delegate.
    virtual void setDelegate(Delegate* delegate) override {
        delegate_ = delegate;
    }

    virtual ~OpenVROmniStereoImpl() {
		vr::VR_Shutdown();
    }

private:

    std::string getTrackedDeviceString(vr::IVRSystem *pHmd, vr::TrackedDeviceIndex_t unDevice, vr::TrackedDeviceProperty prop, vr::TrackedPropertyError *peError = NULL)
	{
		uint32_t unRequiredBufferLen = pHmd->GetStringTrackedDeviceProperty(unDevice, prop, NULL, 0, peError);
		if (unRequiredBufferLen == 0)
			return "";

		char *pchBuffer = new char[unRequiredBufferLen];
		unRequiredBufferLen = pHmd->GetStringTrackedDeviceProperty(unDevice, prop, pchBuffer, unRequiredBufferLen, peError);
		std::string sResult = pchBuffer;
		delete[] pchBuffer;
		return sResult;
	}

	FrameBufferInfo createFrameBuffer(int width, int height) {
		FrameBufferInfo result;

		glGenFramebuffers(1, &result.frameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, result.frameBuffer);

		glGenRenderbuffers(1, &result.depthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, result.depthBuffer);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, result.depthBuffer);

		glGenTextures(1, &result.renderTexture);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, result.renderTexture);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA8, width, height, true);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, result.renderTexture, 0);

		glGenFramebuffers(1, &result.resolveFrameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, result.resolveFrameBuffer);

		glGenTextures(1, &result.resolveTexture);
		glBindTexture(GL_TEXTURE_2D, result.resolveTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, result.resolveTexture, 0);

		// check FBO status
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if(status != GL_FRAMEBUFFER_COMPLETE) {
			throw exception("error in createFrameBuffer()");
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		return result;
	}

    void submitFrame() {
		vr::Texture_t leftEyeTexture = { (void*)(uintptr_t)leftEye_.resolveTexture, vr::TextureType_OpenGL, vr::ColorSpace_Gamma };
		vr::VRCompositor()->Submit(vr::Eye_Left, &leftEyeTexture );

		vr::Texture_t rightEyeTexture = { (void*)(uintptr_t)rightEye_.resolveTexture, vr::TextureType_OpenGL, vr::ColorSpace_Gamma };
		vr::VRCompositor()->Submit(vr::Eye_Right, &rightEyeTexture );
    }

	GLuint quadProgramArray_, quadProgramArrayBuffer_;
	GLuint quadProgram_;

	void setupFullscreenQuads() {
		float vertices[] = {
			0, 0, 0, 1, 1, 0, 1, 1
		};
		glGenVertexArrays(1, &quadProgramArray_);
		glGenBuffers(1, &quadProgramArrayBuffer_);

		glBindBuffer(GL_ARRAY_BUFFER, quadProgramArrayBuffer_);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindVertexArray(quadProgramArray_);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8, 0);
		glBindVertexArray(0);

		quadProgram_ = allofw::glutils::compileShaderProgram(R"================(
		#version 330
		layout(location = 0) in vec2 position;
		out vec2 texCoord;
		void main() {
			gl_Position = vec4(position * 2.0 - vec2(1.0, 1.0), 0.0, 1.0);
			texCoord = position;
		}
		)================", R"================(
		#version 330
		layout(location = 0) out vec4 frag_color;
		in vec2 texCoord;
		uniform sampler2D inputTexture;
		void main() {
			frag_color = texture(inputTexture, texCoord);
		}
		)================");

		glProgramUniform1d(quadProgram_, glGetUniformLocation(quadProgram_, "inputTexture"), 0);
	}

	void renderAuxWindow(const Rectangle2i& viewport) {
		glDisable(GL_DEPTH_TEST);

		glUseProgram(quadProgram_);
		glBindVertexArray(quadProgramArray_);

		glBindTexture(GL_TEXTURE_2D, leftEye_.resolveTexture);
		glViewport(viewport.x, viewport.y, viewport.w / 2, viewport.h);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glBindTexture(GL_TEXTURE_2D, rightEye_.resolveTexture);
		glViewport(viewport.x + viewport.w / 2, viewport.y, viewport.w / 2, viewport.h);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);
		glUseProgram(0);
		glEnable(GL_DEPTH_TEST);
	}

	void updateHMDPose() {
		vr::VRCompositor()->WaitGetPoses(tracked_device_poses_, vr::k_unMaxTrackedDeviceCount, NULL, 0);
		for(int nDevice = 0; nDevice < vr::k_unMaxTrackedDeviceCount; ++nDevice) {
			if(tracked_device_poses_[nDevice].bPoseIsValid) {
				switch (vr_system_->GetTrackedDeviceClass(nDevice)) {
					case vr::TrackedDeviceClass_HMD: {
						pose_matrix_HMD_ = convertSteamVRMatrixToMatrix4(tracked_device_poses_[nDevice].mDeviceToAbsoluteTracking).inversion();
					} break;
				}
			}
		}
	}

	Matrix4f convertSteamVRMatrixToMatrix4(const vr::HmdMatrix34_t &matPose) {
		Matrix4f result;
		result.a11 = matPose.m[0][0]; result.a21 = matPose.m[1][0]; result.a31 = matPose.m[2][0]; result.a41 = 0.0f;
		result.a12 = matPose.m[0][1]; result.a22 = matPose.m[1][1]; result.a32 = matPose.m[2][1]; result.a42 = 0.0f;
		result.a13 = matPose.m[0][2]; result.a23 = matPose.m[1][2]; result.a33 = matPose.m[2][2]; result.a43 = 0.0f;
		result.a14 = matPose.m[0][3]; result.a24 = matPose.m[1][3]; result.a34 = matPose.m[2][3]; result.a44 = 1.0f;
		return result;
	}

	Matrix4f convertSteamVRMatrixToMatrix4(const vr::HmdMatrix44_t &matPose) {
		Matrix4f result;
		result.a11 = matPose.m[0][0]; result.a21 = matPose.m[1][0]; result.a31 = matPose.m[2][0]; result.a41 = matPose.m[3][0];
		result.a12 = matPose.m[0][1]; result.a22 = matPose.m[1][1]; result.a32 = matPose.m[2][1]; result.a42 = matPose.m[3][1];
		result.a13 = matPose.m[0][2]; result.a23 = matPose.m[1][2]; result.a33 = matPose.m[2][2]; result.a43 = matPose.m[3][2];
		result.a14 = matPose.m[0][3]; result.a24 = matPose.m[1][3]; result.a34 = matPose.m[2][3]; result.a44 = matPose.m[3][3];
		return result;
	}

	Matrix4f getHMDMatrixProjectionEye(vr::Hmd_Eye nEye) {
		vr::HmdMatrix44_t mat = vr_system_->GetProjectionMatrix(nEye, 0.1, 1000);
		return convertSteamVRMatrixToMatrix4(mat);
	}

	Matrix4f getHMDMatrixPoseEye(vr::Hmd_Eye nEye) {
		vr::HmdMatrix34_t mat = vr_system_->GetEyeToHeadTransform(nEye);
		return convertSteamVRMatrixToMatrix4(mat).inversion();
	}

	void setupCameras() {
		projection_left_eye_ = getHMDMatrixProjectionEye(vr::Eye_Left);
		pose_left_eye_ = getHMDMatrixPoseEye(vr::Eye_Left);
		projection_right_eye_ = getHMDMatrixProjectionEye(vr::Eye_Right);
		pose_right_eye_ = getHMDMatrixPoseEye(vr::Eye_Right);
	}

    Pose pose_;
    Delegate* delegate_;

	vr::IVRSystem* vr_system_;
	vr::IVRRenderModels* vr_render_models_;

	uint32_t render_buffer_width_, render_buffer_height_;
	FrameBufferInfo leftEye_, rightEye_;

	Matrix4f pose_matrix_HMD_;
	Matrix4f projection_left_eye_, projection_right_eye_;
	Matrix4f pose_left_eye_, pose_right_eye_;

	vr::TrackedDevicePose_t tracked_device_poses_[vr::k_unMaxTrackedDeviceCount];
};

OmniStereo* CreateOpenVROmniStereo() {
    return new OpenVROmniStereoImpl();
}

void DestroyOpenVROmniStereo(OmniStereo* omnistereo) {
    delete dynamic_cast<OpenVROmniStereoImpl*>(omnistereo);
}

ALLOFW_NS_END
