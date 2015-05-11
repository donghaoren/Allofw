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

#include "allofw/omnistereo.h"
#include "allofw/opengl_utils.h"
#include <string>

ALLOFW_NS_BEGIN

// Shader code.
namespace {
const char* gShader_omnistereo_include =
R"================(
// OmniStereo Shader Code:
// Pose information
uniform vec3 omni_position;
uniform vec4 omni_rotation;
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
    vec4 q1 = vec4(-q.xyz, q.w);
    return omni_quat_multiply(omni_quat_multiply(q, vec4(v, 0.0)), q1).xyz;
}
// Transform coordinates to camera space.
vec3 omni_transform(vec3 v) {
    return omni_quat_rotate(omni_rotation, v) + omni_position;
}
// Transform normal to camera space.
vec3 omni_transform_normal(vec3 v) {
    return omni_quat_rotate(omni_rotation, v);
}
// Perform Omnistereo displacement on vertex.
vec3 omni_displace(vec3 vertex) {
    vec3 EYE = vec3(-vertex.z, 0.0, vertex.x) * omni_eye / length(vertex);
    float a = dot(vertex - EYE, vertex - EYE);
    float b = 2.0 * dot(EYE, vertex - EYE);
    float c = dot(EYE, EYE) - omni_radius * omni_radius;
    float t = (-b + sqrt(b * b - 4 * a * c)) / a / 2.0;
    vec3 SP = EYE + t * (vertex - EYE);
    return normalize(SP) * length(EYE - vertex);
}
vec4 omni_project(vec3 v) {
    v = omni_quat_rotate(omni_viewport_rotation, v);
    vec4 r;
    r.xy = v.xy * omni_viewport_projection.xy;
    r.z = v.z * omni_viewport_projection.z + omni_viewport_projection.w;
    r.w = -v.z;
    return r;
}
vec4 omni_render(vec3 v) {
    return omni_project(omni_displace(v));
}
// END OmniStereo Shader Code.
)================";

const char* gShader_draw_vertex =
R"================(#version 330
layout(location = 0) in vec2 position;
out vec2 vp_coordinates;
void main() {
    vp_coordinates = position;
    gl_Position = vec4(position * 2.0 - 1.0, 0.0, 1.0);
}
)================";

const char* gShader_draw_fragment =
R"================(#version 330
uniform sampler2D texWarp;
uniform sampler2D texBlend;
uniform samplerCube texCubemap;
uniform sampler2D texBack;
uniform sampler2D texFront;
uniform sampler2D texPanorama;
uniform int drawMask = 0;

// These constants must be the same as in the header.
const int kDrawMask_Cubemap                   = 1 << 0;
const int kDrawMask_Back                      = 1 << 1;
const int kDrawMask_Front                     = 1 << 2;
const int kDrawMask_Panorama                  = 1 << 3;
const int kDrawMask_Panorama_Equirectangular  = 1 << 4;
const int kDrawMask_Panorama_Cubemap          = 1 << 5;

in vec2 vp_coordinates;
layout(location = 0) out vec4 fragment_output;

void main() {
    // Read warp and blend textures.
    vec3 warp = texture(texWarp, vp_coordinates).xyz;
    vec3 blend = texture(texBlend, vp_coordinates).rgb;

    // Read color from cubemap.
    vec4 cubemap_color = texture(texCubemap, normalize(warp));

    vec4 final_color = cubemap_color;

    fragment_output = vec4(final_color.rgb * blend, 1.0);
}
)================";

}

class OmniStereoImpl : public OmniStereo {
public:
    OmniStereoImpl(Configuration* conf) {
        if(!conf) conf = Configuration::Create();
        LoggerScope logger(Logger::kInfo, "OmniStereo::Initialization");

        resolution_ = conf->getUInt32("omnistereo.resolution", 1024);
        std::string stereo_mode = conf->getSTLString("omnistereo.stereo_mode", "mono");
        /**/ if(stereo_mode == "mono") stereo_mode_ = kStereoMode_Mono;
        else if(stereo_mode == "left") stereo_mode_ = kStereoMode_Left;
        else if(stereo_mode == "right") stereo_mode_ = kStereoMode_Right;
        else if(stereo_mode == "dual_lr") stereo_mode_ = kStereoMode_DualLR;
        else if(stereo_mode == "dual_rl") stereo_mode_ = kStereoMode_DualRL;
        else if(stereo_mode == "active") stereo_mode_ = kStereoMode_Active;
        else if(stereo_mode == "anaglyph_red_cyan") stereo_mode_ = kStereoMode_AnaglyphRedCyan;
        else stereo_mode_ = kStereoMode_Mono;

        stereo_enabled_ = stereo_mode_ != kStereoMode_Mono;

        eye_separation_ = conf->getFloat("omnistereo.eye_separation", 0.065);
        sphere_radius_ = conf->getFloat("omnistereo.sphere_radius", 5.0);
        near_ = conf->getFloat("omnistereo.near", 0.01);
        far_ = conf->getFloat("omnistereo.far", 100);;

        initCubemaps();
        allocateCubemaps();
        initDraw();

        std::string warpblend = conf->getSTLString("omnistereo.warpblend.allosphere_calibration", "undefined");
        if(warpblend != "undefined") {
            std::string hostname = conf->getSTLString("omnistereo.warpblend.allosphere_calibration_hostname", "undefined");
            if(hostname == "undefined") {
                warpblend_ = WarpBlend::LoadAllosphereCalibration(warpblend.c_str(), nullptr);
            } else {
                warpblend_ = WarpBlend::LoadAllosphereCalibration(warpblend.c_str(), hostname.c_str());
            }
        } else {
            warpblend_ = WarpBlend::CreateEquirectangular();
        }
    }
    // Set cubemap resolution, allocate the cubemap.
    virtual void setResolution(int size) {
        if(resolution_ != size) {
            resolution_ = size;
            allocateCubemaps();
        }
    }

    // Enable stereo or not.
    virtual void setStereoMode(StereoMode stereo_mode) {
        if(stereo_mode_ != stereo_mode) {
            stereo_mode_ = stereo_mode;
            stereo_enabled_ = stereo_mode_ != kStereoMode_Mono;
            allocateCubemaps();
        }
    }

    // Pose and eye separation.
    virtual void setPose(const Pose& pose) {
        pose_ = pose;
    }
    virtual void setLens(float eye_separation = 0.0f, float sphere_radius = 1.0f) {
        eye_separation_ = eye_separation;
        sphere_radius_ = sphere_radius;
    }
    virtual void setClipRange(float near, float far) {
        near_ = near;
        far_ = far;
    }

    // Get the cubemap.
    virtual StereoTexture getCubemapTexture() {
        return tex_cubemap_;
    }
    virtual StereoTexture getDepthCubemapTexture() {
        return tex_cubemap_depth_;
    }

    // Find quaternion that rotates v1 -> v2, u1 -> u2.
    Quaternion quat_two_vectors_rotate(Vector3 v1, Vector3 u1, Vector3 v2, Vector3 u2) {
        Vector3 dv = (v2 - v1).unit();
        Vector3 du = (u2 - u2).unit();
        Vector3 axis = dv.cross(du).unit();
        double angle = std::acos(v1.dot(v2));
        if(v1.cross(v2).dot(axis) < 0) angle = -angle;
        return Quaternion::rotation(axis, angle);
    }

    Quaternion make_viewport_quaternion(Vector3 face, Vector3 up) {
        Quaternion r = quat_two_vectors_rotate(face, up, Vector3(0, 0, -1), Vector3(0, 1, 0));
        return r;
    }

    // Capture.
    virtual void capture() {
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);
        // Store viewport and depth range.
        GLint viewport[4];
        GLfloat depth_range[2];
        glGetIntegerv(GL_VIEWPORT, viewport);
        glGetFloatv(GL_DEPTH_RANGE, depth_range);
        glViewport(0, 0, resolution_, resolution_);
        glDepthRange(0, 1);
        int eyes = stereo_enabled_ ? 2 : 1;
        Delegate::CaptureInfo capture_info;
        capture_info.omnistereo = this;
        capture_info.pose = pose_;
        capture_info.eye_separation = eye_separation_;
        capture_info.sphere_radius = sphere_radius_;
        capture_info.near = near_;
        capture_info.far = far_;
        capture_info.viewport_fovy = PI / 2;
        capture_info.viewport_aspect = 1;
        for(int eye = 0; eye < eyes; eye++) {
            capture_info.eye = eye;
            for(int face = 0; face < 6; face++) {
                capture_info.face = face;
                switch(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face) {
                    case GL_TEXTURE_CUBE_MAP_POSITIVE_X: {
                        capture_info.viewport_rotation = Quaternion::rotation(Vector3(0, 0, 1), PI) * Quaternion::rotation(Vector3(0, 1, 0), +PI / 2);
                    } break;
                    case GL_TEXTURE_CUBE_MAP_NEGATIVE_X: {
                        capture_info.viewport_rotation = Quaternion::rotation(Vector3(0, 0, 1), PI) * Quaternion::rotation(Vector3(0, 1, 0), -PI / 2);
                    } break;
                    case GL_TEXTURE_CUBE_MAP_POSITIVE_Y: {
                        capture_info.viewport_rotation = Quaternion::rotation(Vector3(1, 0, 0), -PI / 2);
                    } break;
                    case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y: {
                        capture_info.viewport_rotation = Quaternion::rotation(Vector3(1, 0, 0), +PI / 2);
                    } break;
                    case GL_TEXTURE_CUBE_MAP_POSITIVE_Z: {
                        capture_info.viewport_rotation = Quaternion::rotation(Vector3(1, 0, 0), PI);
                    } break;
                    case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z: {
                        capture_info.viewport_rotation = Quaternion::rotation(Vector3(0, 0, 1), PI);
                    } break;
                }
                glDrawBuffer(GL_COLOR_ATTACHMENT0);
                glFramebufferTexture2D(
                    GL_FRAMEBUFFER,
                    GL_COLOR_ATTACHMENT0,
                    GL_TEXTURE_CUBE_MAP_POSITIVE_X + face,
                    tex_cubemap_.eyes[eye], 0);
                glFramebufferTexture2D(
                    GL_FRAMEBUFFER,
                    GL_DEPTH_ATTACHMENT,
                    GL_TEXTURE_CUBE_MAP_POSITIVE_X + face,
                    tex_cubemap_depth_.eyes[eye], 0);
                glDepthMask(GL_TRUE);
                glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
                if(delegate_) delegate_->onCaptureFace(capture_info);
            }
        }
        // Set viewport and depth range back.
        glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
        glDepthRange(depth_range[0], depth_range[1]);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        for(int eye = 0; eye < eyes; eye++) {
            glBindTexture(GL_TEXTURE_CUBE_MAP, tex_cubemap_.eyes[eye]);
            glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
        }
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }

    // Draw the cubemap to the current viewport.
    virtual void composite(const Rectangle2i& viewport, const CompositeInfo& info) {
        int eyes = stereo_enabled_ ? 2 : 1;
        int mask = info.mask;
        for(int eye = 0; eye < eyes; eye++) {
            if(!info.front.eyes[eye]) mask &= ~kDrawMask_Front;
            if(!info.back.eyes[eye]) mask &= ~kDrawMask_Back;
            if(!info.panorama.eyes[eye]) mask &= ~kDrawMask_Panorama;
        }

        glUseProgram(program_draw_);
        glUniform1i(program_draw_drawMask_, mask);

        Rectangle2i draw_viewport = viewport;

        for(int eye = 0; eye < eyes; eye++) {
            switch(stereo_mode_) {
                case kStereoMode_Mono: break;
                case kStereoMode_Left: {
                    if(eye != kEye_Left) continue;
                } break;
                case kStereoMode_Right: {
                    if(eye != kEye_Right) continue;
                } break;
                case kStereoMode_Active: {
                    if(eye == kEye_Left) {
                        glDrawBuffer(GL_BACK_LEFT);
                    } else {
                        glDrawBuffer(GL_BACK_RIGHT);
                    }
                } break;
                case kStereoMode_DualLR: {
                    if(eye == kEye_Left) {
                        glDrawBuffer(GL_BACK_LEFT);
                    } else {
                        glDrawBuffer(GL_BACK_RIGHT);
                    }
                } break;
                case kStereoMode_DualRL: {
                    if(eye == kEye_Left) {
                        draw_viewport = Rectangle2i(viewport.x, viewport.y, viewport.w / 2, viewport.h);
                    } else {
                        draw_viewport = Rectangle2i(viewport.x + viewport.w / 2, viewport.y, viewport.w / 2, viewport.h);
                    }
                } break;
                case kStereoMode_AnaglyphRedCyan: {
                    if(eye == kEye_Left) {
                        glColorMask(GL_TRUE, GL_FALSE, GL_FALSE, GL_TRUE);
                    } else {
                        glColorMask(GL_FALSE, GL_TRUE, GL_TRUE, GL_TRUE);
                    }
                } break;
            }
            for(int vp = 0; vp < warpblend_->getViewportCount(); vp++) {
                Rectangle2 viewport_bounds = warpblend_->getViewport(vp);
                GLTextureID warp_texture = warpblend_->getWarpTexture(vp);
                GLTextureID blend_texture = warpblend_->getBlendTexture(vp);
                glViewport(
                    draw_viewport.x + viewport_bounds.x * draw_viewport.w,
                    draw_viewport.y + viewport_bounds.y * draw_viewport.h,
                    viewport_bounds.w * draw_viewport.w,
                    viewport_bounds.h * draw_viewport.h
                );

                glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, warp_texture);
                glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, blend_texture);
                glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_CUBE_MAP, tex_cubemap_.eyes[eye]);

                glBindVertexArray(vertex_array_quad_);
                glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

                glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
                glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, 0);
                glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, 0);

            }
        }
        glDrawBuffer(GL_BACK);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glUseProgram(0);
        glutils::checkGLErrors("OmniStereo::composite");
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
    virtual const char* getShaderCode() {
        return gShader_omnistereo_include;
    }

    virtual void setUniforms(GLuint program, const Delegate::CaptureInfo& info) {
        GLint l;
        if((l = glGetUniformLocation(program, "omni_eye")) >= 0) {
            glProgramUniform1f(program, l, info.eye_separation);
        } else fprintf(stderr, "omni_eye\n");

        if((l = glGetUniformLocation(program, "omni_radius")) >= 0) {
            glProgramUniform1f(program, l, info.sphere_radius);
        } else fprintf(stderr, "omni_radius\n");

        if((l = glGetUniformLocation(program, "omni_position")) >= 0) {
            glProgramUniform3f(program, l, info.pose.position.x, info.pose.position.y, info.pose.position.z);
        } else fprintf(stderr, "omni_position\n");

        if((l = glGetUniformLocation(program, "omni_rotation")) >= 0) {
            glProgramUniform4f(program, l, info.pose.rotation.x, info.pose.rotation.y, info.pose.rotation.z, info.pose.rotation.w);
        } else fprintf(stderr, "omni_rotation\n");

        if((l = glGetUniformLocation(program, "omni_viewport_rotation")) >= 0) {
            glProgramUniform4f(program, l, info.viewport_rotation.x, info.viewport_rotation.y, info.viewport_rotation.z, info.viewport_rotation.w);
        } else fprintf(stderr, "omni_viewport_rotation\n");

        if((l = glGetUniformLocation(program, "omni_viewport_projection")) >= 0) {
            glProgramUniform4f(program, l,
                1.0f / std::tan(info.viewport_fovy / 2.0f) / info.viewport_aspect,
                1.0f / std::tan(info.viewport_fovy / 2.0f),
                (info.near + info.far) / (info.near - info.far),
                (2.0f * info.near * info.far) / (info.near - info.far)
            );
        } else fprintf(stderr, "omni_viewport_projection\n");
    }

    // Set the delegate.
    virtual void setDelegate(Delegate* delegate) {
        delegate_ = delegate;
    }

    virtual ~OmniStereoImpl() {
        freeCubemaps();
    }

private:
    // Cubemap initialization code.
    void initCubemaps() {
        LoggerScope logger(Logger::kInfo, "OmniStereo::initCubemaps");
        // Create cubemap textures.
        GLuint textures[4];
        glGenTextures(4, textures);
        // 2 for color/aloha and 2 for depth.
        tex_cubemap_.L = textures[0];
        tex_cubemap_.R = textures[1];
        tex_cubemap_depth_.L = textures[2];
        tex_cubemap_depth_.R = textures[3];
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, tex_cubemap_.L);
        setupCubemapParameters();
        glBindTexture(GL_TEXTURE_CUBE_MAP, tex_cubemap_.R);
        setupCubemapParameters();
        glBindTexture(GL_TEXTURE_CUBE_MAP, tex_cubemap_depth_.L);
        setupCubemapParameters(true);
        glBindTexture(GL_TEXTURE_CUBE_MAP, tex_cubemap_depth_.R);
        setupCubemapParameters(true);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
        glutils::checkGLErrors("setup textures");
        glGenFramebuffers(1, &framebuffer_);
    }
    void setupCubemapParameters(bool is_depth = false) {
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        if(is_depth) {
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_MODE, GL_NONE);
        } else {
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        }
    }
    void allocateCubemaps() {
        LoggerScope logger(Logger::kInfo, "OmniStereo::allocateCubemaps");
        GLuint colors[2], depths[2];
        int colors_count, depths_count;
        if(stereo_enabled_) {
            colors_count = depths_count = 2;
            colors[0] = tex_cubemap_.L;
            colors[1] = tex_cubemap_.R;
            depths[0] = tex_cubemap_depth_.L;
            depths[1] = tex_cubemap_depth_.R;
        } else {
            colors_count = depths_count = 1;
            colors[0] = tex_cubemap_.L;
            depths[0] = tex_cubemap_depth_.L;
        }
        glActiveTexture(GL_TEXTURE0);
        for(int i = 0; i < colors_count; i++) {
            glBindTexture(GL_TEXTURE_CUBE_MAP, colors[i]);
            for (int f = 0; f < 6; f++) {
                glTexImage2D(
                    GL_TEXTURE_CUBE_MAP_POSITIVE_X + f,
                    0, GL_RGBA8,
                    resolution_, resolution_,
                    0, GL_BGRA, GL_UNSIGNED_BYTE,
                    NULL
                );
            }
        }
        glutils::checkGLErrors("color cubemap");
        for(int i = 0; i < depths_count; i++) {
            glBindTexture(GL_TEXTURE_CUBE_MAP, depths[i]);
            for (int f = 0; f < 6; f++) {
                glTexImage2D(
                    GL_TEXTURE_CUBE_MAP_POSITIVE_X + f,
                    0, GL_DEPTH_COMPONENT32,
                    resolution_, resolution_,
                    0, GL_DEPTH_COMPONENT, GL_FLOAT,
                    NULL
                );
            }
        }
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
        glutils::checkGLErrors("depth cubemap");
    }
    void freeCubemaps() {
        glDeleteFramebuffers(1, &framebuffer_);
        GLuint textures[4];
        textures[0] = tex_cubemap_.L;
        textures[1] = tex_cubemap_.R;
        textures[2] = tex_cubemap_depth_.L;
        textures[3] = tex_cubemap_depth_.R;
        glDeleteTextures(4, textures);
    }

    void initDraw() {
        LoggerScope logger(Logger::kInfo, "OmniStereo::initDraw");
        program_draw_ = glutils::compileShaderProgram(gShader_draw_vertex, gShader_draw_fragment);
        if(!program_draw_) {
            throw exception("Fatal: failed compile shader program.");
        }
        glProgramUniform1i(program_draw_, glGetUniformLocation(program_draw_, "texWarp"), 0);
        glProgramUniform1i(program_draw_, glGetUniformLocation(program_draw_, "texBlend"), 1);
        glProgramUniform1i(program_draw_, glGetUniformLocation(program_draw_, "texCubemap"), 2);
        glProgramUniform1i(program_draw_, glGetUniformLocation(program_draw_, "texBack"), 3);
        glProgramUniform1i(program_draw_, glGetUniformLocation(program_draw_, "texFront"), 4);
        glProgramUniform1i(program_draw_, glGetUniformLocation(program_draw_, "texPanorama"), 5);
        program_draw_drawMask_ = glGetUniformLocation(program_draw_, "drawMask");
        glutils::checkGLErrors("compile program");

        glGenVertexArrays(1, &vertex_array_quad_);
        glBindVertexArray(vertex_array_quad_);
        glGenBuffers(1, &vertex_array_quad_buffer_);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_array_quad_buffer_);
        float vertex_array_quad_data[8] = {
            0, 0, 0, 1, 1, 0, 1, 1
        };
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_array_quad_data), vertex_array_quad_data, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glutils::checkGLErrors("vertex array");
    }

    StereoTexture tex_cubemap_, tex_cubemap_depth_;
    GLuint framebuffer_;
    GLuint program_draw_;
    GLuint program_draw_drawMask_;
    GLuint vertex_array_quad_, vertex_array_quad_buffer_;
    int resolution_;
    StereoMode stereo_mode_;
    bool stereo_enabled_;
    Pose pose_;
    float eye_separation_, sphere_radius_;
    float near_, far_;
    Delegate* delegate_;
    WarpBlend* warpblend_;
};

OmniStereo* OmniStereo::Create(Configuration* conf) {
    return new OmniStereoImpl(conf);
}

void OmniStereo::Delegate::onCaptureFace(const CaptureInfo& info) { }

ALLOFW_NS_END
