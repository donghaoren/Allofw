#ifndef ALLOFW_OMNISTEREO_H
#define ALLOFW_OMNISTEREO_H

// Omnistereo Rendering.
#include "common.h"
#include "config.h"
#include "math/math.h"
#include "opengl.h"

#undef near
#undef far

namespace allofw {

    //! OpenGL texture ID.
    typedef GLuint GLTextureID;

    //! Class for Omnistereo rendering.
    class OmniStereo {
    public:
        //@{
        //! Stereo mode.
        typedef int StereoMode;
        static const int kStereoMode_Mono               =   0;  //! Mono.
        static const int kStereoMode_Left               = 100;  //! Only show left eye.
        static const int kStereoMode_Right              = 101;  //! Only show right eye.
        static const int kStereoMode_Active             = 200;  //! Active stereo.
        static const int kStereoMode_Dual_LR            = 300;  //! Dual, left-right.
        static const int kStereoMode_Dual_RL            = 301;  //! Dual, right-left.
        static const int kStereoMode_Anaglyph_Red_Cyan  = 400;  //! Anaglyph, red/cyan.
        //@}

        //@{
        //! Panorama mode.
        typedef int PanoramaMode;
        static const int kPanoramaModeEquirectangular   = 0;    //! Equirectangular panorama mode.
        //@}

        //@{
        //! Eye index.
        typedef int Eye;
        static const int kEye        = 0;   //! Single eye.
        static const int kEye_Left   = 0;   //! Left eye.
        static const int kEye_Right  = 1;   //! Right eye.
        //@}

        //@{
        //! Composite mask.
        typedef int CompositeMask;
        static const int kCompositeMask_Scene                     = 1 << 0;     //! Show the scene.
        static const int kCompositeMask_Back                      = 1 << 1;     //! Show the back buffer.
        static const int kCompositeMask_Front                     = 1 << 2;     //! Show the front buffer.
        static const int kCompositeMask_Panorama                  = 1 << 3;     //! Show the panorama.
        static const int kCompositeMask_Panorama_Equirectangular  = 1 << 4;     //! Panorama is equirectangular.
        static const int kCompositeMask_Panorama_Cubemap          = 1 << 5;     //! Panorama is cubemap.
        static const int kCompositeMask_Panorama_Cubemap_YUV420P  = 1 << 6;     //! Panorama is cubemap.
        //@}

        // Stereo texture.
        struct StereoTexture {
            // In mono mode, we only use L.
            union {
                struct {
                    GLTextureID L;  //! Left eye texture.
                    GLTextureID R;  //! Right eye texture.
                };
                GLTextureID eyes[2]; //! Array-like access.
            };
            StereoTexture() : L(0), R(0) { }
        };

        //! OmniStereo delegate class.
        class Delegate {
        public:
            //! Capture info.
            struct CaptureInfo {
                //! Current pose.
                Pose pose;
                //! Which eye.
                Eye eye;
                //! Eye separation.
                float eye_separation;
                //! Sphere radius.
                float sphere_radius;

                //! Viewport pose.
                Quaternion viewport_rotation;
                //! Viewport fovy.
                float viewport_fovy;
                //! Viewport aspect ratio.
                float viewport_aspect;
                //! Near clipping distance.
                float near;
                //! Far clipping distance.
                float far;

                //! The OmniStereo object.
                OmniStereo* omnistereo;

                //! Set OmniStereo uniforms to shader program (shortcut).
                //! @param program: The program to set uniforms to.
                void setUniforms(GLuint program) const {
                    omnistereo->setUniforms(program, *this);
                }
            };

            //! Render the scene here.
            virtual void onCaptureViewport(const CaptureInfo& info);

            virtual ~Delegate() { }
        };

        //! Set cubemap resolution, allocate the cubemap.
        //! @param size: Width/height of the cubemap faces.
        virtual void setResolution(int size = 1024) = 0;

        //! Enable stereo or not.
        //! @param stereo_mode: The stereo mode.
        virtual void setStereoMode(StereoMode stereo_mode) = 0;

        //! Set pose.
        //! @param pose: The pose.
        virtual void setPose(const Pose& pose) = 0;
        //! Set lens.
        //! @param eye_separation: Distance between the two eyes in OpenGL units.
        //! @param sphere_radius: The radius of the spherical display in OpenGL units
        virtual void setLens(float eye_separation = 0.0f, float sphere_radius = 1.0f) = 0;
        //! Set clip range.
        //! @param near: The near clipping distance in OpenGL units.
        //! @param far: The far clipping distance in OpenGL units.
        virtual void setClipRange(float near, float far) = 0;

        //! Get the cubemap (only allowed in cubemap mode).
        //! @return A StereoTexture struct with left/right eye texture IDs.
        virtual StereoTexture getCubemapTexture() = 0;

        //! Get the depth cubemap (only allowed in cubemap mode).
        //! @return A StereoTexture struct with left/right eye texture IDs.
        virtual StereoTexture getDepthCubemapTexture() = 0;

        //! Capture the scene.
        virtual void capture() = 0;

        //! Composite info structure.
        struct CompositeInfo {
            CompositeMask mask;
            StereoTexture back, front;
            union {
                StereoTexture panorama;
                StereoTexture panorama_planes[3];
            };
            CompositeInfo() : mask(kCompositeMask_Scene) { }
        };

        //! Composite the scene: Draw internal buffers to current viewport.
        //! @param viewport: The viewport in pixels.
        //! @param info: The CompositeInfo.
        virtual void composite(const Rectangle2i& viewport, const CompositeInfo& info = CompositeInfo()) = 0;

        //! Create a customized composite shader.
        //! @param code: Custom shader code.
        //! @return The program ID of the compiled composite shader.
        virtual GLuint compositeCustomizeShader(const char* code) = 0;

        //! Restore the composite shader to default.
        virtual void compositeRestoreShader() = 0;

        //! Get OmniStereo shader prefix code.
        //!
        //! Necessary functions and uniforms for use in shader programs.
        //! - vec3 omni_transform(vec3 v)          : Transform coordinates to camera space.
        //! - vec3 omni_transform_normal(vec3 v)   : Transform normal to camera space.
        //! - vec4 omni_displace(vec4 v)           : Perform Omnistereo displacement on vertex.
        //! - vec4 omni_project(vec4 v)            : Perform projection to render target.
        //! - vec4 omni_render(vec4 v)             : Short for 'omni_project(omni_displace(v))'
        //!
        //! Uniforms:
        //! - omni_eye, omni_radius: Half eye separation, sphere radius.
        //! - omni_viewport_rotation, omni_viewport_projection: Viewport pose and projection information, handled internally.
        //!
        //! Call setUniforms() to set these uniforms to your shader.
        //! @return The shader code to be included.
        virtual const char* getShaderCode() = 0;

        //! Set uniforms to a shader program using getShaderCode().
        //! @param program: The OpenGL id of the program.
        //! @param info: The CaptureInfo given by onCaptureViewport.
        virtual void setUniforms(GLuint program, const Delegate::CaptureInfo& info) = 0;

        //! Set the delegate.
        //! @param delegate: Set the delegate object.
        virtual void setDelegate(Delegate* delegate) = 0;

        //! Create new OmniStereo with configuration information.
        //! @param conf: The Configuration object.
        static OmniStereo* Create(Configuration* conf);
        //! Destroy OmniStereo object.
        //! @param omnistereo: The object to destroy.
        static void Destroy(OmniStereo* omnistereo);

    protected:
        virtual ~OmniStereo();
    };

    //! Warp and blend information for a display configuration.
    class WarpBlend {
    public:

        //! Warp map data.
        struct WarpData {
            //! 3D positions for each pixel relative to the viewing center.
            Vector3f* data;
            //! Width and height of the warp map.
            Size2i size;
        };

        //! Blend map data.
        struct BlendData {
            //! Alpha map in RGBA.
            Vector4f* data;
            //! Width and height of the blend map.
            Size2i size;
        };

        //! Viewport information.
        struct ViewportInfo {
            //! The viewport coordinates in screen pixels.
            Rectangle2 viewport;
            //! Target screen resolution (OmniStereo will determine the size of internal framebuffers based on this value).
            Size2i screen_resolution;
            //! Should OmniStereo maintain the viewport aspect ratio.
            bool enforce_aspect_ratio;
            //! Target aspect ratio of the viewport.
            float aspect_ratio;
        };

        //! Get how many viewports to draw.
        virtual int getViewportCount() = 0;

        //! Get viewport information.
        //! @param viewport_index: The index of the viewport, 0 to getViewportCount() - 1.
        virtual ViewportInfo getViewport(int viewport_index) = 0;

        //! Get warp data for the viewport.
        //! @param viewport_index: The index of the viewport, 0 to getViewportCount() - 1.
        virtual BlendData getBlendData(int viewport_index) = 0;

        //! Get blend data for the viewport.
        //! @param viewport_index: The index of the viewport, 0 to getViewportCount() - 1.
        virtual WarpData getWarpData(int viewport_index) = 0;

        //! Get warp texture for the viewport.
        //! @param viewport_index: The index of the viewport, 0 to getViewportCount() - 1.
        virtual GLTextureID getBlendTexture(int viewport_index) = 0;

        //! Get blend texture for the viewport.
        //! @param viewport_index: The index of the viewport, 0 to getViewportCount() - 1.
        virtual GLTextureID getWarpTexture(int viewport_index) = 0;

        //! Create warpblend data with Equirectangular projection.
        //! @param width: The width of the warpblend info.
        //! @param height: The height of the warpblend info.
        static WarpBlend* CreateEquirectangular(int width, int height);

        //! Create warpblend data for Perspective projection.
        //! @param width: The width of the warpblend info.
        //! @param height: The height of the warpblend info.
        //! @param fov: The horizontal field of view.
        static WarpBlend* CreatePerspective(int width, int height, float fov);

        //! Load warpblend data from AlloSphere calibration directory.
        //! @param calibration_path: The path to the calibration directory.
        //! @param hostname: The hostname, if omitted, use gethostname().
        static WarpBlend* LoadAllosphereCalibration(const char* calibration_path, const char* hostname = nullptr);

        //! Destroy the warpblend object.
        //! @param warpblend: The object to be destroyed.
        static void Destroy(WarpBlend* warpblend);

    protected:
        virtual ~WarpBlend();
    };
}

#endif
