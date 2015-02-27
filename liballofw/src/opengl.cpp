#include "opengl.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>

ALLOFW_NS_BEGIN

namespace {
    bool glfw_initialized = false;

    void glfw_error_callback(int error, const char* description) {
        fprintf(stderr, "glfw: %s\n", description);
    }

    void glfw_get_screen_size(int* width, int* height) {
        int count;
        GLFWmonitor** monitors = glfwGetMonitors(&count);
        *width = 0;
        *height = 0;
        for(int i = 0; i < count; i++) {
            int x, y;
            glfwGetMonitorPos(monitors[i], &x, &y);
            const GLFWvidmode* vm = glfwGetVideoMode(monitors[i]);
            int xmax = x + vm->width;
            int ymax = y + vm->height;
            if(*width < xmax) *width = xmax;
            if(*height < ymax) *height = ymax;
        }
    }
}

class glfw_runtime_error : public exception {
public:
    glfw_runtime_error(const char* what_ = nullptr) : exception(what_) { }
};

class OpenGLWindowImpl : public OpenGLWindow {
public:

    OpenGLWindowImpl(Hint hint, const char* title) {
        delegate = nullptr;

        if(!glfw_initialized) {
            if(!glfwInit()) {
                throw glfw_runtime_error("glfwInit() failed.");
            }
            glfw_initialized = true;
        }
        #ifndef __APPLE__
            glewInit();
        #endif
        glfwDefaultWindowHints();
        // We use OpenGL 3.2 Forward Compat Context, Core Profile.
        // Note: On Mac OS X, deprecated features are no longer supported.
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        if(hint.stereo) {
            glfwWindowHint(GLFW_STEREO, GL_TRUE);
        }
        int width = hint.width;
        int height = hint.height;
        if(width == Hint::FULLSCREEN || height == Hint::FULLSCREEN) {
            glfwWindowHint(GLFW_DECORATED, GL_FALSE);
            glfw_get_screen_size(&width, &height);
        }
        window = glfwCreateWindow(width, height, title, NULL, NULL);
        if(!window) {
            if(hint.stereo) {
                glfwWindowHint(GLFW_STEREO, GL_FALSE);
                window = glfwCreateWindow(width, height, title, NULL, NULL);
            }
        }
        if(!window) {
            throw glfw_runtime_error("glfwCreateWindow() failed.");
        }
        glfwSetWindowUserPointer(window, this);
        glfwSetWindowPosCallback(window, cb_windowpos);
        glfwSetWindowSizeCallback(window, cb_windowsize);
        glfwSetWindowCloseCallback(window, cb_windowclose);
        glfwSetWindowRefreshCallback(window, cb_windowrefresh);
        glfwSetWindowFocusCallback(window, cb_windowfocus);
        glfwSetFramebufferSizeCallback(window, cb_framebuffersize);
    }

    // Callbacks.
    static void cb_windowpos(GLFWwindow* window, int x, int y) {
        OpenGLWindowImpl* impl = (OpenGLWindowImpl*)glfwGetWindowUserPointer(window);
        if(!impl->delegate) return;
        impl->delegate->onMove(x, y);
    }

    static void cb_windowsize(GLFWwindow* window, int width, int height) {
        OpenGLWindowImpl* impl = (OpenGLWindowImpl*)glfwGetWindowUserPointer(window);
        if(!impl->delegate) return;
        impl->delegate->onResize(width, height);
    }

    static void cb_windowclose(GLFWwindow* window) {
        OpenGLWindowImpl* impl = (OpenGLWindowImpl*)glfwGetWindowUserPointer(window);
        if(!impl->delegate) return;
        impl->delegate->onClose();
    }

    static void cb_windowrefresh(GLFWwindow* window) {
        OpenGLWindowImpl* impl = (OpenGLWindowImpl*)glfwGetWindowUserPointer(window);
        if(!impl->delegate) return;
        impl->delegate->onRefresh();
    }

    static void cb_windowfocus(GLFWwindow* window, int value) {
        OpenGLWindowImpl* impl = (OpenGLWindowImpl*)glfwGetWindowUserPointer(window);
        if(!impl->delegate) return;
        impl->delegate->onFocus(value);
    }

    static void cb_windowiconify(GLFWwindow* window, int value) {
        OpenGLWindowImpl* impl = (OpenGLWindowImpl*)glfwGetWindowUserPointer(window);
        if(!impl->delegate) return;
        impl->delegate->onIconify(value);
    }

    static void cb_framebuffersize(GLFWwindow* window, int width, int height) {
        OpenGLWindowImpl* impl = (OpenGLWindowImpl*)glfwGetWindowUserPointer(window);
        if(!impl->delegate) return;
        impl->delegate->onFramebufferSize(width, height);
    }

    virtual void setDelegate(Delegate* delegate_) {
        delegate = delegate_;
    }

    virtual void makeContextCurrent() {
        glfwMakeContextCurrent(window);
    }

    virtual void swapBuffers() {
        glfwSwapBuffers(window);
    }

    virtual void pollEvents() {
        glfwPollEvents();
    }

    virtual void waitEvents() {
        glfwWaitEvents();
    }

    virtual Size2i getFramebufferSize() {
        Size2i size;
        glfwGetFramebufferSize(window, &size.x, &size.y);
        return size;
    }

    virtual ~OpenGLWindowImpl() {
        glfwDestroyWindow(window);
    }

    Delegate* delegate;
    GLFWwindow* window;
};

OpenGLWindow* OpenGLWindow::Create_(OpenGLWindow::Hint hint, const char* title) {
    return new OpenGLWindowImpl(hint, title);
}

// Default callbacks.
void OpenGLWindow::Delegate::onMove(int x, int y) { }
void OpenGLWindow::Delegate::onResize(int width, int height) { }
void OpenGLWindow::Delegate::onClose() { }
void OpenGLWindow::Delegate::onRefresh() { }
void OpenGLWindow::Delegate::onFocus(int focused) { }
void OpenGLWindow::Delegate::onIconify(int iconified) { }
void OpenGLWindow::Delegate::onFramebufferSize(int width, int height) { }


ALLOFW_NS_END
