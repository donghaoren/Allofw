#ifndef ALLOFW_OMNIAPP_H
#define ALLOFW_OMNIAPP_H

// Basic application with OmniStereo.
// This is designed to be a header only class.

#include "opengl.h"
#include "omnistereo.h"

#include <vector>

namespace ALLOFW_NS {

    // OmniApp base class.
    class OmniAppBase
    : public OmniStereo::Delegate,
      public OpenGLWindow::Delegate,
      public non_copyable {
    public:
        //
        OmniAppBase();

        // Event handlers.

        // Called upon initialization.
        virtual void onInitialize();
        // Called before capturing the frame.
        virtual void onFrame(double dt);
        // Called before swapping buffers. (place for barrier).
        virtual void onPresent();
        // Omnistereo capture callback.
        virtual void onCaptureFace(const CaptureInfo& info) override;

        // Window event handlers.
        virtual void onMove(int x, int y) override;
        virtual void onResize(int width, int height) override;
        virtual void onClose() override;
        virtual void onRefresh() override;
        virtual void onFocus(int focused) override;
        virtual void onIconify(int iconified) override;
        virtual void onFramebufferSize(int width, int height) override;

        // Window input handlers.
        virtual void onKeyboard(const char* c_key, const char* c_action, const char* c_modifiers, int scancode) override;

        // Initialize.
        void initialize(const char* config_path = "allofw.yaml");

        // Render the scene.
        // Override if you want to change the default behaviour.
        virtual void render();

        // Start the main loop.
        void main(); // Run until window flagged for close.
        void tick(); // Run one tick.

        // Close this app?
        inline bool closing() { return closing_; }
        inline void setClosing() { closing_ = true; }


        virtual ~OmniAppBase();

        // Accessors.
        inline OpenGLWindow* window() { return window_; }
        inline OmniStereo* omni() { return omni_; }
        inline Configuration* config() { return config_; }

        // Mutable accessors.
        inline const Pose& pose() const { return pose_; }
        inline Pose& pose() { return pose_; }

    private:
        Pose pose_;
        OpenGLWindow* window_;
        OmniStereo* omni_;
        Configuration* config_;
        bool closing_;
        double t_previous_frame_;
    };

    template<typename... Mixins>
    class OmniApp : public virtual OmniAppBase {
    };

    template<typename Mixin, typename... Mixins>
    class OmniApp<Mixin, Mixins...> : public virtual OmniApp<Mixins...>, public virtual Mixin {
    public:
        virtual void onInitialize() override {
            Mixin::onInitialize();
            OmniApp<Mixins...>::onInitialize();
        }
        virtual void onFrame(double dt) override {
            Mixin::onFrame(dt);
            OmniApp<Mixins...>::onFrame(dt);
        }
        virtual void onPresent() override {
            Mixin::onPresent();
            OmniApp<Mixins...>::onPresent();
        }
        virtual void onKeyboard(const char* c_key, const char* c_action, const char* c_modifiers, int scancode) override {
            Mixin::onKeyboard(c_key, c_action, c_modifiers, scancode);
            OmniApp<Mixins...>::onKeyboard(c_key, c_action, c_modifiers, scancode);
        }
    };

    // Use mixins with the following syntax:
    // OmniApp<Mixin1, Mixin2, ...>
    class OmniAppMixin : public virtual OmniAppBase {
    public:
        virtual void onInitialize() override { }
        virtual void onFrame(double dt) override { }
        virtual void onPresent() override { }
        virtual void onKeyboard(const char* c_key, const char* c_action, const char* c_modifiers, int scancode) override { }
    };

    // Enable keyboard navigation.
    // W,X,A,D for rotation, Up,Down,Left,Right,Q,Z for translation.
    class OmniAppMixin_Navigation : public virtual OmniAppMixin {
    public:
        OmniAppMixin_Navigation();
        virtual void onKeyboard(const char* c_key, const char* c_action, const char* c_modifiers, int scancode) override;
        virtual void onFrame(double dt) override;

    private:
        std::vector<Vector4> translation_speeds_;
        Vector4 rotation_speed_;
    };

}

#endif
