#ifndef ALLOFW_OMNISTEREO_H
#define ALLOFW_OMNISTEREO_H

// Omnistereo Rendering.
#include <memory>

#include "common.h"
#include "config.h"
#include "math/math.h"
#include "opengl.h"

namespace ALLOFW_NS {

    typedef GLuint GLTextureID;

    // The class for Omnistereo rendering.
    class OmniStereo {
    public:
        // Cubemap face.
        typedef int CubemapFace;
        static const int kCubemapFace_PositiveX = 0;
        static const int kCubemapFace_NegativeX = 1;
        static const int kCubemapFace_PositiveY = 2;
        static const int kCubemapFace_NegativeY = 3;
        static const int kCubemapFace_PositiveZ = 4;
        static const int kCubemapFace_NegativeZ = 5;

        typedef int StereoMode;
        static const int kStereoMode_Mono               =   0;
        static const int kStereoMode_Left               = 100;
        static const int kStereoMode_Right              = 101;
        static const int kStereoMode_Active             = 200;
        static const int kStereoMode_DualLR             = 300;
        static const int kStereoMode_DualRL             = 301;
        static const int kStereoMode_AnaglyphRedCyan    = 400;

        // Panorama mode.
        typedef int PanoramaMode;
        static const int kPanoramaModeEquirectangular = 0;

        // Eye index.
        typedef int Eye;
        static const int kEye        = 0;
        static const int kEye_Left   = 0;
        static const int kEye_Right  = 1;

        typedef int DrawMask;
        static const int kDrawMask_Cubemap                   = 1 << 0;
        static const int kDrawMask_Back                      = 1 << 1;
        static const int kDrawMask_Front                     = 1 << 2;
        static const int kDrawMask_Panorama                  = 1 << 3;
        static const int kDrawMask_Panorama_Equirectangular  = 1 << 4;
        static const int kDrawMask_Panorama_Cubemap          = 1 << 5;

        // Stereo texture.
        struct StereoTexture {
            // In mono mode, we only use L.
            union {
                struct {
                    GLTextureID L;
                    GLTextureID R;
                };
                GLTextureID eyes[2];
            };
            StereoTexture() : L(0), R(0) { }
        };

        class Delegate {
        public:
            // Capture info.
            struct CaptureInfo {
                // Current pose.
                Pose pose;
                // Which eye.
                Eye eye;
                // Stereo parameters.
                float eye_separation, sphere_radius;
                // Viewport and clipping information.
                Quaternion viewport_rotation;
                float viewport_fovy, viewport_aspect;
                float near, far;
                // The calling OmniStereo object.
                OmniStereo* omnistereo;

                // Set uniform to shader program.
                void setUniforms(GLuint program) const {
                    omnistereo->setUniforms(program, *this);
                }
            };
            // Capture a face of the cubemap.
            virtual void onCaptureFace(const CaptureInfo& info);
            virtual ~Delegate() { }
        };

        // Set cubemap resolution, allocate the cubemap.
        virtual void setResolution(int size = 1024) = 0;

        // Enable stereo or not.
        virtual void setStereoMode(StereoMode stereo_mode) = 0;

        // Pose and eye separation.
        virtual void setPose(const Pose& pose) = 0;
        virtual void setLens(float eye_separation = 0.0f, float sphere_radius = 1.0f) = 0;
        virtual void setClipRange(float near, float far) = 0;

        // Get the cubemap.
        virtual StereoTexture getCubemapTexture() = 0;
        virtual StereoTexture getDepthCubemapTexture() = 0;

        // Capture.
        virtual void capture() = 0;

        // Composite info.
        struct CompositeInfo {
            DrawMask mask;
            StereoTexture back, front;
            StereoTexture panorama;
            CompositeInfo() : mask(kDrawMask_Cubemap) { }
        };

        // Draw the cubemap to the current viewport.
        virtual void composite(const Rectangle2i& viewport, const CompositeInfo& info = CompositeInfo()) = 0;

        // Necessary functions and uniforms for use in shader programs.
        // vec3 omni_transform(vec3 v)          : Transform coordinates to camera space.
        // vec3 omni_transform_normal(vec3 v)   : Transform normal to camera space.
        // vec4 omni_displace(vec4 v)           : Perform Omnistereo displacement on vertex.
        // vec4 omni_project(vec4 v)            : Perform projection to cubemap face.
        // vec4 omni_render(vec4 v)             : Short for 'omni_project(omni_displace(v))'
        // Uniforms:
        // omni_eye, omni_radius, omni_face
        // These uniforms are automatically set before calling capture().
        virtual const char* getShaderCode() = 0;
        virtual void setUniforms(GLuint program, const Delegate::CaptureInfo& info) = 0;

        // Set the delegate.
        virtual void setDelegate(Delegate* delegate) = 0;

        virtual ~OmniStereo() { }

        // Create new OmniStereo with configuration information.
        static OmniStereo* Create(Configuration* conf);
    };

    class WarpBlend {
    public:

        struct WarpData {
            Vector3f* data;
            Size2i size;
        };

        struct BlendData {
            Vector4f* data;
            Size2i size;
        };

        struct ViewportInfo {
            Rectangle2 viewport;
            Size2i screen_resolution;
            bool enforce_aspect_ratio;
            float aspect_ratio;
        };

        // How many viewports to draw.
        virtual int getViewportCount() = 0;
        virtual ViewportInfo getViewport(int viewport) = 0;

        // Get warp and blend data.
        virtual BlendData getBlendData(int viewport) = 0;
        virtual WarpData getWarpData(int viewport) = 0;

        // Get warp and blend textures.
        virtual GLTextureID getBlendTexture(int viewport) = 0;
        virtual GLTextureID getWarpTexture(int viewport) = 0;

        virtual ~WarpBlend() { }

        static WarpBlend* CreateEquirectangular();
        static WarpBlend* CreatePerspective(float fov);
        static WarpBlend* LoadAllosphereCalibration(const char* calibration_path, const char* hostname = nullptr);
    };
}

#endif
