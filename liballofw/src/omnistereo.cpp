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
#include <vector>

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
uniform vec2 positionScalers = vec2(1, 1);

layout(location = 0) in vec2 position;

out vec2 vp_coordinates;

void main() {
    vp_coordinates = position;
    gl_Position = vec4(positionScalers * (position * 2.0 - 1.0), 0.0, 1.0);
}
)================";

// For cubemap mode.
const char* gShader_cubemap_draw_fragment =
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
    vec4 scene_color = texture(texCubemap, warp);

    vec4 final_color = scene_color;

    fragment_output = vec4(final_color.rgb * blend, 1.0);
}
)================";

// For per projection mode.
const char* gShader_perprojection_draw_fragment =
R"================(#version 330
uniform sampler2D texWarp;
uniform sampler2D texBlend;
uniform sampler2D texPerProjection;
uniform sampler2D texBack;
uniform sampler2D texFront;
uniform sampler2D texPanorama;
uniform int drawMask = 0;

uniform vec4 viewportRotation;
uniform float tanFovDiv2;

// These constants must be the same as in the header.
const int kDrawMask_Cubemap                   = 1 << 0;
const int kDrawMask_Back                      = 1 << 1;
const int kDrawMask_Front                     = 1 << 2;
const int kDrawMask_Panorama                  = 1 << 3;
const int kDrawMask_Panorama_Equirectangular  = 1 << 4;
const int kDrawMask_Panorama_Cubemap          = 1 << 5;

in vec2 vp_coordinates;
layout(location = 0) out vec4 fragment_output;

vec3 omni_quat_rotate(vec4 q, vec3 v) {
    float d = dot(q.xyz, v);
    vec3 c = cross(q.xyz, v);
    return q.w * q.w * v + (q.w + q.w) * c + d * q.xyz - cross(c, q.xyz);
}

void main() {
    // Read warp and blend textures.
    vec3 warp = texture(texWarp, vp_coordinates).xyz;
    vec3 blend = texture(texBlend, vp_coordinates).rgb;

    vec3 p_coord = omni_quat_rotate(viewportRotation, warp);
    p_coord.xy /= -p_coord.z;
    p_coord.xy /= tanFovDiv2;

    // Read color from cubemap.
    vec4 scene_color = texture(texPerProjection, p_coord.xy / 2.0 + 0.5);

    vec4 final_color = scene_color;

    fragment_output = vec4(final_color.rgb * blend, 1.0);
}
)================";

const Quaternion kVPRotation_Positive_X = Quaternion::Rotation(Vector3(0, 0, 1), PI) * Quaternion::Rotation(Vector3(0, 1, 0), +PI / 2);
const Quaternion kVPRotation_Negative_X = Quaternion::Rotation(Vector3(0, 0, 1), PI) * Quaternion::Rotation(Vector3(0, 1, 0), -PI / 2);
const Quaternion kVPRotation_Positive_Y = Quaternion::Rotation(Vector3(1, 0, 0), -PI / 2);
const Quaternion kVPRotation_Negative_Y = Quaternion::Rotation(Vector3(1, 0, 0), +PI / 2);
const Quaternion kVPRotation_Positive_Z = Quaternion::Rotation(Vector3(1, 0, 0), PI);
const Quaternion kVPRotation_Negative_Z = Quaternion::Rotation(Vector3(0, 0, 1), PI);
}

class OmniStereoImpl : public OmniStereo {
public:
    OmniStereoImpl(Configuration* conf) {
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
        far_ = conf->getFloat("omnistereo.far", 100);

        std::string warpblend = conf->getSTLString("omnistereo.warpblend.allosphere_calibration", "undefined");
        if(warpblend != "undefined") {
            std::string hostname = conf->getSTLString("omnistereo.warpblend.allosphere_calibration_hostname", "undefined");
            if(hostname == "undefined") {
                warpblend_ = WarpBlend::LoadAllosphereCalibration(warpblend.c_str(), nullptr);
            } else {
                warpblend_ = WarpBlend::LoadAllosphereCalibration(warpblend.c_str(), hostname.c_str());
            }
        } else {
            std::string generate = conf->getSTLString("omnistereo.warpblend.generate", "equirectangular");
            if(generate == "equirectangular") {
                warpblend_ = WarpBlend::CreateEquirectangular();
            } else if(generate == "perspective") {
                float fov = conf->getFloat("omnistereo.warpblend.generate_fov", 90);
                warpblend_ = WarpBlend::CreatePerspective(fov / 180.0 * PI);
            }
        }

        std::string capture_method = conf->getSTLString("omnistereo.capture_method", "cubemap");
        capture_method_ = kCaptureMethod_Cubemap;

        if(capture_method == "cubemap") capture_method_ = kCaptureMethod_Cubemap;
        if(capture_method == "per_projection") capture_method_ = kCaptureMethod_PerProjection;

        preprojector_resolution_scale_ = conf->getFloat("omnistereo.resolution_scale", 1.0);

        switch(capture_method_) {
            case kCaptureMethod_Cubemap: {
                initCubemaps();
                allocateCubemaps();
            } break;
            case kCaptureMethod_PerProjection: {
                initPerProjectionTextures();
                allocatePerProjectionTextures();
            } break;
        }
    }
    // Set cubemap resolution, allocate the cubemap.
    virtual void setResolution(int size) {
        if(capture_method_ == kCaptureMethod_Cubemap) {
            if(resolution_ != size) {
                resolution_ = size;
                allocateCubemaps();
            }
        }
    }

    // Enable stereo or not.
    virtual void setStereoMode(StereoMode stereo_mode) {
        if(stereo_mode_ != stereo_mode) {
            stereo_mode_ = stereo_mode;
            stereo_enabled_ = stereo_mode_ != kStereoMode_Mono;
            switch(capture_method_) {
                case kCaptureMethod_Cubemap: {
                    allocateCubemaps();
                } break;
                case kCaptureMethod_PerProjection: {
                    allocatePerProjectionTextures();
                } break;
            }
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
        if(capture_method_ == kCaptureMethod_Cubemap) {
            return tex_cubemap_;
        } else {
            throw exception("cubemap not available in current capture method.");
        }
    }
    virtual StereoTexture getDepthCubemapTexture() {
        if(capture_method_ == kCaptureMethod_Cubemap) {
            return tex_cubemap_depth_;
        } else {
            throw exception("cubemap not available in current capture method.");
        }
    }

    virtual void capture() override {
        switch(capture_method_) {
            case kCaptureMethod_Cubemap: {
                captureCubemap();
            } break;
            case kCaptureMethod_PerProjection: {
                capturePerProjection();
            } break;
        }
    }

    virtual void composite(const Rectangle2i& viewport, const CompositeInfo& info) {
        switch(capture_method_) {
            case kCaptureMethod_Cubemap: {
                compositeCubemap(viewport, info);
            } break;
            case kCaptureMethod_PerProjection: {
                compositePerProjection(viewport, info);
            } break;
        }
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
        }
        if((l = glGetUniformLocation(program, "omni_radius")) >= 0) {
            glProgramUniform1f(program, l, info.sphere_radius);
        }
        if((l = glGetUniformLocation(program, "omni_position")) >= 0) {
            glProgramUniform3f(program, l, info.pose.position.x, info.pose.position.y, info.pose.position.z);
        }
        if((l = glGetUniformLocation(program, "omni_rotation")) >= 0) {
            glProgramUniform4f(program, l, info.pose.rotation.x, info.pose.rotation.y, info.pose.rotation.z, info.pose.rotation.w);
        }
        if((l = glGetUniformLocation(program, "omni_viewport_rotation")) >= 0) {
            glProgramUniform4f(program, l, info.viewport_rotation.x, info.viewport_rotation.y, info.viewport_rotation.z, info.viewport_rotation.w);
        }
        if((l = glGetUniformLocation(program, "omni_viewport_projection")) >= 0) {
            glProgramUniform4f(program, l,
                1.0f / std::tan(info.viewport_fovy / 2.0f) / info.viewport_aspect,
                1.0f / std::tan(info.viewport_fovy / 2.0f),
                (info.near + info.far) / (info.near - info.far),
                (2.0f * info.near * info.far) / (info.near - info.far)
            );
        }
    }

    // Set the delegate.
    virtual void setDelegate(Delegate* delegate) {
        delegate_ = delegate;
    }

    virtual ~OmniStereoImpl() {
        switch(capture_method_) {
            case kCaptureMethod_Cubemap: {
                freeCubemaps();
            } break;
            case kCaptureMethod_PerProjection: {

            } break;
        };
        freeCubemaps();
        delete warpblend_;
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

        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

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

        glutils::checkGLErrors("generate framebuffer");

        program_draw_ = glutils::compileShaderProgram(gShader_draw_vertex, gShader_cubemap_draw_fragment);
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
        program_draw_positionScalers_ = glGetUniformLocation(program_draw_, "positionScalers");
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
        glDeleteProgram(program_draw_);
    }
    // Capture into cubemap.
    void captureCubemap() {
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
        capture_info.sphere_radius = sphere_radius_;
        capture_info.near = near_;
        capture_info.far = far_;
        capture_info.viewport_fovy = PI / 2;
        capture_info.viewport_aspect = 1;
        for(int eye = 0; eye < eyes; eye++) {
            capture_info.eye = eye;
            capture_info.eye_separation = stereo_enabled_ ? (eye == kEye_Left ? -eye_separation_ : eye_separation_) : 0;
            for(int face = 0; face < 6; face++) {
                switch(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face) {
                    case GL_TEXTURE_CUBE_MAP_POSITIVE_X: capture_info.viewport_rotation = kVPRotation_Positive_X; break;
                    case GL_TEXTURE_CUBE_MAP_NEGATIVE_X: capture_info.viewport_rotation = kVPRotation_Negative_X; break;
                    case GL_TEXTURE_CUBE_MAP_POSITIVE_Y: capture_info.viewport_rotation = kVPRotation_Positive_Y; break;
                    case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y: capture_info.viewport_rotation = kVPRotation_Negative_Y; break;
                    case GL_TEXTURE_CUBE_MAP_POSITIVE_Z: capture_info.viewport_rotation = kVPRotation_Positive_Z; break;
                    case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z: capture_info.viewport_rotation = kVPRotation_Negative_Z; break;
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
                glEnable(GL_DEPTH_TEST);
                if(delegate_) delegate_->onCaptureViewport(capture_info);
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
    virtual void compositeCubemap(const Rectangle2i& viewport, const CompositeInfo& info) {
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
                WarpBlend::ViewportInfo vp_info = warpblend_->getViewport(vp);
                Rectangle2 viewport_bounds = vp_info.viewport;
                GLTextureID warp_texture = warpblend_->getWarpTexture(vp);
                GLTextureID blend_texture = warpblend_->getBlendTexture(vp);
                Rectangle2 vp_to_draw(
                    draw_viewport.x + viewport_bounds.x * draw_viewport.w,
                    draw_viewport.y + viewport_bounds.y * draw_viewport.h,
                    viewport_bounds.w * draw_viewport.w,
                    viewport_bounds.h * draw_viewport.h
                );
                glViewport(vp_to_draw.x, vp_to_draw.y, vp_to_draw.w, vp_to_draw.h);
                Vector2f position_scalers(1, 1);
                if(vp_info.enforce_aspect_ratio) {
                    if(vp_to_draw.w / vp_to_draw.h > vp_info.aspect_ratio) {
                        position_scalers.y = vp_to_draw.w / vp_to_draw.h / vp_info.aspect_ratio;
                    } else {
                        position_scalers.x = vp_to_draw.h * vp_info.aspect_ratio / vp_to_draw.w;
                    }
                }
                glUniform2f(program_draw_positionScalers_, position_scalers.x, position_scalers.y);

                glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, warp_texture);
                glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, blend_texture);
                glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_CUBE_MAP, tex_cubemap_.eyes[eye]);

                glDisable(GL_DEPTH_TEST);

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

    // PerProjection initialization.
    void initPerProjectionTextures() {
        LoggerScope logger(Logger::kInfo, "OmniStereo::initPerProjectionTextures");
        // Each viewport has a set of textures.
        tex_projections_.resize(warpblend_->getViewportCount());
        //First
        for(ProjectionTexture& p : tex_projections_) {
            GLuint textures[4];
            glGenTextures(4, textures);
            p.texture_color.L = textures[0];
            p.texture_color.R = textures[1];
            p.texture_depth.L = textures[2];
            p.texture_depth.R = textures[3];
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, p.texture_color.L);
            setupProjectionTextureParameters();
            glBindTexture(GL_TEXTURE_2D, p.texture_color.R);
            setupProjectionTextureParameters();
            glBindTexture(GL_TEXTURE_2D, p.texture_depth.L);
            setupProjectionTextureParameters(true);
            glBindTexture(GL_TEXTURE_2D, p.texture_depth.R);
            setupProjectionTextureParameters(true);
        }

        glutils::checkGLErrors("setup textures");

        glGenFramebuffers(1, &framebuffer_);

        glutils::checkGLErrors("generate framebuffer");

        program_draw_ = glutils::compileShaderProgram(gShader_draw_vertex, gShader_perprojection_draw_fragment);
        if(!program_draw_) {
            throw exception("Fatal: failed compile shader program.");
        }
        glProgramUniform1i(program_draw_, glGetUniformLocation(program_draw_, "texWarp"), 0);
        glProgramUniform1i(program_draw_, glGetUniformLocation(program_draw_, "texBlend"), 1);
        glProgramUniform1i(program_draw_, glGetUniformLocation(program_draw_, "texPerProjection"), 2);
        glProgramUniform1i(program_draw_, glGetUniformLocation(program_draw_, "texBack"), 3);
        glProgramUniform1i(program_draw_, glGetUniformLocation(program_draw_, "texFront"), 4);
        glProgramUniform1i(program_draw_, glGetUniformLocation(program_draw_, "texPanorama"), 5);
        program_draw_drawMask_ = glGetUniformLocation(program_draw_, "drawMask");
        program_draw_positionScalers_ = glGetUniformLocation(program_draw_, "positionScalers");
        program_draw_viewportRotation_ = glGetUniformLocation(program_draw_, "viewportRotation");
        program_draw_tanFovDiv2_ = glGetUniformLocation(program_draw_, "tanFovDiv2");
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

        // Determine projection dimensions.
        for(int vp = 0; vp < warpblend_->getViewportCount(); vp++) {
            WarpBlend::WarpData warp = warpblend_->getWarpData(vp);
            // First determine the central direction.
            Vector3f direction(0, 0, 0);
            for(int i = 0; i < warp.size.w * warp.size.h; i++) {
                Vector3f d = warp.data[i].unit();
                direction += d;
            }
            direction = direction.unit();
            // Determine the radius.
            float dot_max = 1;
            for(int i = 0; i < warp.size.w * warp.size.h; i++) {
                Vector3f d = warp.data[i].unit();
                float dot = d.dot(direction);
                if(dot_max > dot) dot_max = dot;
            }
            if(dot_max < 0.1) {
                // The angle is too wide, throw an error.
                throw exception("unable to use per-projection mode, viewport angle too large.");
            }
            float fov = std::acos(dot_max) * 2.0f;
            ProjectionTexture& pt = tex_projections_[vp];
            Vector3f rotation_axis = Vector3f(0, 0, -1).cross(direction);
            float rotation_angle = std::acos(Vector3f(0, 0, -1).dot(direction));
            pt.rotation = Quaternion::Rotation(rotation_axis, rotation_angle).inversion();
            pt.fov = fov;
            Size2i screen_resolution = warpblend_->getViewport(vp).screen_resolution;
            pt.resolution = resolution_; // std::max(screen_resolution.w, screen_resolution.h) * preprojector_resolution_scale_;
            logger.printf("Viewport %02d: d = (%.4f,%.4f,%.4f), fov = %.2f degs, res = %d\n", vp, direction.x, direction.y, direction.z, fov / PI * 180.0f, pt.resolution);
        }

    }
    void setupProjectionTextureParameters(bool is_depth = false) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        if(is_depth) {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        } else {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 8.0f);
        }
    }
    void allocatePerProjectionTextures() {
        glActiveTexture(GL_TEXTURE0);
        int eyes = stereo_enabled_ ? 2 : 1;
        for(ProjectionTexture& p : tex_projections_) {
            for(int eye = 0; eye < eyes; eye++) {
                glBindTexture(GL_TEXTURE_2D, p.texture_color.eyes[eye]);
                glTexImage2D(
                    GL_TEXTURE_2D,
                    0, GL_RGBA8,
                    p.resolution, p.resolution,
                    0, GL_BGRA, GL_UNSIGNED_BYTE,
                    NULL
                );
                glBindTexture(GL_TEXTURE_2D, p.texture_depth.eyes[eye]);
                glTexImage2D(
                    GL_TEXTURE_2D,
                    0, GL_DEPTH_COMPONENT32,
                    p.resolution, p.resolution,
                    0, GL_DEPTH_COMPONENT, GL_FLOAT,
                    NULL
                );
            }
        }
    }
    void freeProjectionTextures() {
        glDeleteFramebuffers(1, &framebuffer_);
        for(ProjectionTexture& p : tex_projections_) {
            GLuint textures[4];
            textures[0] = p.texture_color.L;
            textures[1] = p.texture_color.R;
            textures[2] = p.texture_depth.L;
            textures[3] = p.texture_depth.R;
            glDeleteTextures(4, textures);
        }
        glDeleteProgram(program_draw_);
    }
    // Capture into cubemap.
    void capturePerProjection() {
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);
        // Store viewport and depth range.
        GLint viewport[4];
        GLfloat depth_range[2];
        glGetIntegerv(GL_VIEWPORT, viewport);
        glGetFloatv(GL_DEPTH_RANGE, depth_range);

        Delegate::CaptureInfo capture_info;
        capture_info.omnistereo = this;
        capture_info.pose = pose_;
        capture_info.sphere_radius = sphere_radius_;
        capture_info.near = near_;
        capture_info.far = far_;
        capture_info.viewport_aspect = 1;

        int eyes = stereo_enabled_ ? 2 : 1;

        glDepthRange(0, 1);
        for(ProjectionTexture& p : tex_projections_) {
            for(int eye = 0; eye < eyes; eye++) {
                capture_info.eye = eye;
                capture_info.eye_separation = stereo_enabled_ ? (eye == kEye_Left ? -eye_separation_ : eye_separation_) : 0;
                capture_info.viewport_fovy = p.fov;
                capture_info.viewport_rotation = p.rotation;

                glViewport(0, 0, p.resolution, p.resolution);
                glDrawBuffer(GL_COLOR_ATTACHMENT0);
                glFramebufferTexture2D(
                    GL_FRAMEBUFFER,
                    GL_COLOR_ATTACHMENT0,
                    GL_TEXTURE_2D,
                    p.texture_color.eyes[eye], 0);
                glFramebufferTexture2D(
                    GL_FRAMEBUFFER,
                    GL_DEPTH_ATTACHMENT,
                    GL_TEXTURE_2D,
                    p.texture_depth.eyes[eye], 0);
                glDepthMask(GL_TRUE);
                glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
                glEnable(GL_DEPTH_TEST);
                if(delegate_) delegate_->onCaptureViewport(capture_info);
            }
        }
        // Set viewport and depth range back.
        glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
        glDepthRange(depth_range[0], depth_range[1]);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        for(ProjectionTexture& p : tex_projections_) {
            for(int eye = 0; eye < eyes; eye++) {
                glBindTexture(GL_TEXTURE_2D, p.texture_color.eyes[eye]);
                glGenerateMipmap(GL_TEXTURE_2D);
            }
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void compositePerProjection(const Rectangle2i& viewport, const CompositeInfo& info) {
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
                ProjectionTexture& projection_texture = tex_projections_[vp];
                WarpBlend::ViewportInfo vp_info = warpblend_->getViewport(vp);
                Rectangle2 viewport_bounds = vp_info.viewport;
                GLTextureID warp_texture = warpblend_->getWarpTexture(vp);
                GLTextureID blend_texture = warpblend_->getBlendTexture(vp);
                Rectangle2 vp_to_draw(
                    draw_viewport.x + viewport_bounds.x * draw_viewport.w,
                    draw_viewport.y + viewport_bounds.y * draw_viewport.h,
                    viewport_bounds.w * draw_viewport.w,
                    viewport_bounds.h * draw_viewport.h
                );
                glViewport(vp_to_draw.x, vp_to_draw.y, vp_to_draw.w, vp_to_draw.h);
                Vector2f position_scalers(1, 1);
                if(vp_info.enforce_aspect_ratio) {
                    if(vp_to_draw.w / vp_to_draw.h > vp_info.aspect_ratio) {
                        position_scalers.y = vp_to_draw.w / vp_to_draw.h / vp_info.aspect_ratio;
                    } else {
                        position_scalers.x = vp_to_draw.h * vp_info.aspect_ratio / vp_to_draw.w;
                    }
                }
                glUniform2f(program_draw_positionScalers_, position_scalers.x, position_scalers.y);
                glUniform4f(program_draw_viewportRotation_, projection_texture.rotation.x, projection_texture.rotation.y, projection_texture.rotation.z, projection_texture.rotation.w);
                glUniform1f(program_draw_tanFovDiv2_, std::tan(projection_texture.fov / 2.0));

                glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, warp_texture);
                glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, blend_texture);
                glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, projection_texture.texture_color.eyes[eye]);

                glDisable(GL_DEPTH_TEST);

                glBindVertexArray(vertex_array_quad_);
                glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

                glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, 0);
                glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, 0);
                glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, 0);

            }
        }
        glDrawBuffer(GL_BACK);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glUseProgram(0);
        glutils::checkGLErrors("OmniStereo::composite");
    }

    // Internal structs.
    struct ProjectionTexture {
        StereoTexture texture_color, texture_depth;
        int resolution;
        Quaternion rotation;
        float fov;
    };

    enum CaptureMethod {
        kCaptureMethod_Cubemap          = 0,
        kCaptureMethod_PerProjection    = 1
    };

    CaptureMethod capture_method_;

    // Cubemap capture method's data.
    StereoTexture tex_cubemap_, tex_cubemap_depth_;
    int resolution_;

    // PreProjection capture method's data.
    std::vector<ProjectionTexture> tex_projections_;
    float preprojector_resolution_scale_;

    // These are shared between the two methods.
    GLuint framebuffer_;
    GLuint program_draw_;
    GLuint program_draw_drawMask_, program_draw_positionScalers_;
    GLuint program_draw_viewportRotation_, program_draw_tanFovDiv2_;
    GLuint vertex_array_quad_, vertex_array_quad_buffer_;

    // Stereo mode.
    StereoMode stereo_mode_;
    bool stereo_enabled_;
    // Pose and lens information.
    Pose pose_;
    float eye_separation_, sphere_radius_;
    float near_, far_;
    // Delegate.
    Delegate* delegate_;
    // Warp and blend configurations.
    WarpBlend* warpblend_;
};

OmniStereo* OmniStereo::Create(Configuration* conf) {
    return new OmniStereoImpl(conf);
}

void OmniStereo::Delegate::onCaptureViewport(const CaptureInfo& info) { }

ALLOFW_NS_END
