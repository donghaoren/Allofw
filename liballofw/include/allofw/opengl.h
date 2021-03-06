#ifndef ALLOFW_OPENGL_H
#define ALLOFW_OPENGL_H

#include "common.h"
#include "config.h"
#include "math/geometry.h"

#include <GL/glew.h>

// OpenGL Window and Context Creation.

namespace allofw {

    class OpenGLWindow {
    public:

        struct Hint {
            bool active_stereo;
            bool fullscreen;
            bool hide_cursor;
            int width;
            int height;

            Hint() {
                active_stereo = false;
                fullscreen = false;
                hide_cursor = false;
                width = 900;
                height = 600;
            }
        };

        class Delegate {
        public:
            // General window events.
            virtual void onMove(int x, int y);
            virtual void onResize(int width, int height);
            virtual void onClose();
            virtual void onRefresh();
            virtual void onFocus(int focused);
            virtual void onIconify(int iconified);
            virtual void onFramebufferSize(int width, int height);
            // Keyboard evnets.
            virtual void onKeyboard(const char* key, const char* action, const char* modifiers, int scancode);
            // Mouse events.
            virtual void onCursorPosition(double x, double y);
            virtual void onMouseButton(const char* button, const char* action, const char* modifiers);
            virtual void onCursorEnter(bool entered);
            virtual void onScroll(double xoffset, double yoffset);
            virtual ~Delegate() { }
        };

        // Set window delegate.
        virtual void setDelegate(Delegate* delegate) = 0;

        // Active the window's OpenGL context.
        virtual void makeContextCurrent() = 0;

        // Swap buffers.
        virtual void swapBuffers() = 0;

        // Set swap interval.
        virtual void setSwapInterval(int interval) = 0;

        // Process all pending events.
        virtual void pollEvents() = 0;

        // Wait for events.
        virtual void waitEvents() = 0;

        // Get framebuffer size. For high-resolution display, this might be different to the window size.
        virtual Size2i getFramebufferSize() = 0;

        // Input handling.
        virtual void enableKeyboardInput() = 0;
        virtual void enableMouseInput() = 0;

        // Should the window be closed?
        virtual bool shouldClose() = 0;
        // Flag for close, after this, shouldClose() return true.
        virtual void close() = 0;

        // Static methods to create a window.
        static OpenGLWindow* Create(Hint hint, const char* title);
        static OpenGLWindow* Create(Configuration* config);
        static void Destroy(OpenGLWindow* window);

    protected:
        virtual ~OpenGLWindow();
    };
}

#endif
