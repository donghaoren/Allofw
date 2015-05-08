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
        static const int kCubemapFacePositiveX = 0;
        static const int kCubemapFaceNegativeX = 1;
        static const int kCubemapFacePositiveY = 2;
        static const int kCubemapFaceNegativeY = 3;
        static const int kCubemapFacePositiveZ = 4;
        static const int kCubemapFaceNegativeZ = 5;

        // Panorama mode.
        typedef int PanoramaMode;
        static const int kPanoramaModeEquirectangular = 0;

        // Eye index.
        typedef int Eye;
        static const int kEye       = 0;
        static const int kLeftEye   = 0;
        static const int kRightEye  = 1;

        struct DrawInfo {
            Pose pose;
            Eye eye;
            CubemapFace cubemap_face;
            float eye_separation;
            float sphere_radius;
        };

        struct StereoTexture {
            // In mono mode, we only use L.
            GLTextureID L;
            GLTextureID R;
            StereoTexture() : L(0), R(0) { }
        };

        struct CompositeInfo {
            StereoTexture back, front;
            StereoTexture panorama;
            PanoramaMode panorama_mode;
        };

        class Delegate {
        public:
            // Capture a face of the cubemap.
            virtual void onCaptureFace(const DrawInfo& info);
            virtual ~Delegate() { }
        };

        // Set cubemap resolution, allocate the cubemap.
        virtual void setResolution(int size = 1024) = 0;

        // Enable stereo or not.
        virtual void setEnableStereo(bool enable) = 0;

        // Pose and eye separation.
        virtual void setPose(const Pose& pose) = 0;
        virtual void setLens(float eye_separation = 0.0f, float sphere_radius = 1.0f) = 0;

        // Get the cubemap.
        virtual StereoTexture getCubemapTexture() = 0;
        virtual StereoTexture getDepthCubemapTexture() = 0;

        // Capture.
        virtual void capture() = 0;

        // Draw the cubemap to the current viewport.
        virtual void draw(const Rectangle2i& viewport, const CompositeInfo& info) = 0;

        // Necessary functions and uniforms for use in shader programs.
        // vec4 omni_transform(vec4 v)          : Transform coordinates to camera space.
        // vec3 omni_transform_normal(vec3 v)   : Transform normal to camera space.
        // vec4 omni_displace(vec4 v)           : Perform Omnistereo displacement on vertex.
        // vec4 omni_project(vec4 v)            : Perform projection to cubemap face.
        // vec4 omni_render(vec4 v) : Short for 'omni_project(omni_displace(v))'
        // Uniforms:
        // omni_eye, omni_radius, omni_face
        // These uniforms are automatically set before calling capture().
        virtual char* getShaderCode() = 0;

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
            int width;
            int height;
        };

        struct BlendData {
            Vector4f* data;
            int width;
            int height;
        };

        // How many viewports to draw.
        virtual int getViewportCount() = 0;
        virtual Rectangle2 getViewport(int viewport) = 0;

        // Get warp and blend data.
        virtual BlendData getBlendData(int viewport) = 0;
        virtual WarpData getWarpData(int viewport) = 0;

        // Get warp and blend textures.
        virtual GLTextureID getBlendTexture(int viewport) = 0;
        virtual GLTextureID getWarpTexture(int viewport) = 0;

        static WarpBlend* LoadFisheye();
        static WarpBlend* LoadPerspective();
        static WarpBlend* LoadAllosphereCalibration(const char* calibration_path, const char* hostname);
    };
}

#endif
