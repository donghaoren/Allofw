#ifndef ALLOFW_OPENGL_H
#define ALLOFW_OPENGL_H

#include "common.h"
#include "config.h"

// OpenGL Window and Context Creation.

namespace ALLOFW_NS {

    class OpenGLWindow;
    typedef std::shared_ptr<OpenGLWindow> POpenGLWindow;

    class OpenGLWindow {
    public:

        class Delegate {
        public:
            virtual void onDraw() = 0;
            virtual void onClose() = 0;
            virtual ~Delegate() { }
        };

        void mainLoopTick();

        virtual ~OpenGLWindow();

        static OpenGLWindow* Create_(PConfiguration conf);
        static POpenGLWindow Create(PConfiguration conf) {
            return POpenGLWindow(Create_(conf));
        }
    };
}

#endif
