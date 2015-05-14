#include "allofw/opengl.h"
#include "allofw/logger.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>

#include <map>

ALLOFW_NS_BEGIN

namespace {
    bool glfw_initialized = false;

    void glfw_error_callback(int error, const char* description) {
        Logger::Default()->printf(Logger::kError, "glfw: %d, %s\n", error, description);
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
            glfwSetErrorCallback(glfw_error_callback);
            if(!glfwInit()) {
                throw glfw_runtime_error("glfwInit() failed.");
            }
            glfw_initialized = true;
        }
        glfwDefaultWindowHints();
        // We use OpenGL 3.3 Forward Compat Context, Core Profile.
        // Note: On Mac OS X, deprecated features are no longer supported.
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        if(hint.active_stereo) {
            glfwWindowHint(GLFW_STEREO, GL_TRUE);
        }
        int width = hint.width;
        int height = hint.height;
        if(hint.fullscreen) {
            glfwWindowHint(GLFW_DECORATED, GL_FALSE);
            glfw_get_screen_size(&width, &height);
        }
        window = glfwCreateWindow(width, height, title, NULL, NULL);
        if(!window) {
            if(hint.active_stereo) {
                glfwWindowHint(GLFW_STEREO, GL_FALSE);
                window = glfwCreateWindow(width, height, title, NULL, NULL);
            }
        }
        if(!window) {
            throw glfw_runtime_error("glfwCreateWindow() failed.");
        }
        if(hint.hide_cursor) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
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

    static void cb_keyboard(GLFWwindow* window, int key, int scancode, int action, int mods) {
        OpenGLWindowImpl* impl = (OpenGLWindowImpl*)glfwGetWindowUserPointer(window);
        if(!impl->delegate) return;
        const char* my_action = "UNKNOWN";
        switch(action) {
            case GLFW_PRESS: my_action = "PRESS"; break;
            case GLFW_REPEAT: my_action = "REPEAT"; break;
            case GLFW_RELEASE: my_action = "RELEASE"; break;
        };
        std::string modifiers;
        if(mods & GLFW_MOD_SHIFT) modifiers += "SHIFT;";
        if(mods & GLFW_MOD_CONTROL) modifiers += "CONTROL;";
        if(mods & GLFW_MOD_ALT) modifiers += "ALT;";
        if(mods & GLFW_MOD_SUPER) modifiers += "SUPER;";
        if(modifiers.size() > 0) modifiers = modifiers.substr(0, modifiers.size() - 1);
        impl->delegate->onKeyboard(impl->key_to_name[key].c_str(), my_action, modifiers.c_str(), scancode);
    }

    static void cb_cursor_position(GLFWwindow* window, double xpos, double ypos) {
        OpenGLWindowImpl* impl = (OpenGLWindowImpl*)glfwGetWindowUserPointer(window);
        if(!impl->delegate) return;
        impl->delegate->onCursorPosition(xpos, ypos);
    }

    static void cb_mouse_button(GLFWwindow* window, int button, int action, int mods) {
        OpenGLWindowImpl* impl = (OpenGLWindowImpl*)glfwGetWindowUserPointer(window);
        if(!impl->delegate) return;
        const char* my_action = "UNKNOWN";
        const char* my_button = "UNKNOWN";
        switch(action) {
            case GLFW_PRESS: my_action = "PRESS"; break;
            case GLFW_RELEASE: my_action = "RELEASE"; break;
        };
        switch(button) {
            case GLFW_MOUSE_BUTTON_LEFT: my_button = "LEFT"; break;
            case GLFW_MOUSE_BUTTON_RIGHT: my_button = "RIGHT"; break;
            case GLFW_MOUSE_BUTTON_MIDDLE: my_button = "MIDDLE"; break;
        };
        std::string my_mods;
        if(mods & GLFW_MOD_SHIFT) my_mods += "SHIFT;";
        if(mods & GLFW_MOD_CONTROL) my_mods += "CONTROL;";
        if(mods & GLFW_MOD_ALT) my_mods += "ALT;";
        if(mods & GLFW_MOD_SUPER) my_mods += "SUPER;";
        if(my_mods.size() > 0) my_mods = my_mods.substr(0, my_mods.size() - 1);
        impl->delegate->onMouseButton(my_button, my_action, my_mods.c_str());
    }
    static void cb_cursor_enter(GLFWwindow* window, int entered) {
        OpenGLWindowImpl* impl = (OpenGLWindowImpl*)glfwGetWindowUserPointer(window);
        if(!impl->delegate) return;
        impl->delegate->onCursorEnter(entered);
    }
    static void cb_scroll(GLFWwindow* window, double xoffset, double yoffset) {
        OpenGLWindowImpl* impl = (OpenGLWindowImpl*)glfwGetWindowUserPointer(window);
        if(!impl->delegate) return;
        impl->delegate->onScroll(xoffset, yoffset);
    }

    virtual void enableKeyboardInput() {
        key_to_name[GLFW_KEY_UNKNOWN] = "UNKNOWN";
        key_to_name[GLFW_KEY_SPACE] = "SPACE";
        key_to_name[GLFW_KEY_APOSTROPHE] = "APOSTROPHE";
        key_to_name[GLFW_KEY_COMMA] = "COMMA";
        key_to_name[GLFW_KEY_MINUS] = "MINUS";
        key_to_name[GLFW_KEY_PERIOD] = "PERIOD";
        key_to_name[GLFW_KEY_SLASH] = "SLASH";
        key_to_name[GLFW_KEY_0] = "0";
        key_to_name[GLFW_KEY_1] = "1";
        key_to_name[GLFW_KEY_2] = "2";
        key_to_name[GLFW_KEY_3] = "3";
        key_to_name[GLFW_KEY_4] = "4";
        key_to_name[GLFW_KEY_5] = "5";
        key_to_name[GLFW_KEY_6] = "6";
        key_to_name[GLFW_KEY_7] = "7";
        key_to_name[GLFW_KEY_8] = "8";
        key_to_name[GLFW_KEY_9] = "9";
        key_to_name[GLFW_KEY_SEMICOLON] = "SEMICOLON";
        key_to_name[GLFW_KEY_EQUAL] = "EQUAL";
        key_to_name[GLFW_KEY_A] = "A";
        key_to_name[GLFW_KEY_B] = "B";
        key_to_name[GLFW_KEY_C] = "C";
        key_to_name[GLFW_KEY_D] = "D";
        key_to_name[GLFW_KEY_E] = "E";
        key_to_name[GLFW_KEY_F] = "F";
        key_to_name[GLFW_KEY_G] = "G";
        key_to_name[GLFW_KEY_H] = "H";
        key_to_name[GLFW_KEY_I] = "I";
        key_to_name[GLFW_KEY_J] = "J";
        key_to_name[GLFW_KEY_K] = "K";
        key_to_name[GLFW_KEY_L] = "L";
        key_to_name[GLFW_KEY_M] = "M";
        key_to_name[GLFW_KEY_N] = "N";
        key_to_name[GLFW_KEY_O] = "O";
        key_to_name[GLFW_KEY_P] = "P";
        key_to_name[GLFW_KEY_Q] = "Q";
        key_to_name[GLFW_KEY_R] = "R";
        key_to_name[GLFW_KEY_S] = "S";
        key_to_name[GLFW_KEY_T] = "T";
        key_to_name[GLFW_KEY_U] = "U";
        key_to_name[GLFW_KEY_V] = "V";
        key_to_name[GLFW_KEY_W] = "W";
        key_to_name[GLFW_KEY_X] = "X";
        key_to_name[GLFW_KEY_Y] = "Y";
        key_to_name[GLFW_KEY_Z] = "Z";
        key_to_name[GLFW_KEY_LEFT_BRACKET] = "LEFT_BRACKET";
        key_to_name[GLFW_KEY_BACKSLASH] = "BACKSLASH";
        key_to_name[GLFW_KEY_RIGHT_BRACKET] = "RIGHT_BRACKET";
        key_to_name[GLFW_KEY_GRAVE_ACCENT] = "GRAVE_ACCENT";
        key_to_name[GLFW_KEY_WORLD_1] = "WORLD_1";
        key_to_name[GLFW_KEY_WORLD_2] = "WORLD_2";
        key_to_name[GLFW_KEY_ESCAPE] = "ESCAPE";
        key_to_name[GLFW_KEY_ENTER] = "ENTER";
        key_to_name[GLFW_KEY_TAB] = "TAB";
        key_to_name[GLFW_KEY_BACKSPACE] = "BACKSPACE";
        key_to_name[GLFW_KEY_INSERT] = "INSERT";
        key_to_name[GLFW_KEY_DELETE] = "DELETE";
        key_to_name[GLFW_KEY_RIGHT] = "RIGHT";
        key_to_name[GLFW_KEY_LEFT] = "LEFT";
        key_to_name[GLFW_KEY_DOWN] = "DOWN";
        key_to_name[GLFW_KEY_UP] = "UP";
        key_to_name[GLFW_KEY_PAGE_UP] = "PAGE_UP";
        key_to_name[GLFW_KEY_PAGE_DOWN] = "PAGE_DOWN";
        key_to_name[GLFW_KEY_HOME] = "HOME";
        key_to_name[GLFW_KEY_END] = "END";
        key_to_name[GLFW_KEY_CAPS_LOCK] = "CAPS_LOCK";
        key_to_name[GLFW_KEY_SCROLL_LOCK] = "SCROLL_LOCK";
        key_to_name[GLFW_KEY_NUM_LOCK] = "NUM_LOCK";
        key_to_name[GLFW_KEY_PRINT_SCREEN] = "PRINT_SCREEN";
        key_to_name[GLFW_KEY_PAUSE] = "PAUSE";
        key_to_name[GLFW_KEY_F1] = "F1";
        key_to_name[GLFW_KEY_F2] = "F2";
        key_to_name[GLFW_KEY_F3] = "F3";
        key_to_name[GLFW_KEY_F4] = "F4";
        key_to_name[GLFW_KEY_F5] = "F5";
        key_to_name[GLFW_KEY_F6] = "F6";
        key_to_name[GLFW_KEY_F7] = "F7";
        key_to_name[GLFW_KEY_F8] = "F8";
        key_to_name[GLFW_KEY_F9] = "F9";
        key_to_name[GLFW_KEY_F10] = "F10";
        key_to_name[GLFW_KEY_F11] = "F11";
        key_to_name[GLFW_KEY_F12] = "F12";
        key_to_name[GLFW_KEY_F13] = "F13";
        key_to_name[GLFW_KEY_F14] = "F14";
        key_to_name[GLFW_KEY_F15] = "F15";
        key_to_name[GLFW_KEY_F16] = "F16";
        key_to_name[GLFW_KEY_F17] = "F17";
        key_to_name[GLFW_KEY_F18] = "F18";
        key_to_name[GLFW_KEY_F19] = "F19";
        key_to_name[GLFW_KEY_F20] = "F20";
        key_to_name[GLFW_KEY_F21] = "F21";
        key_to_name[GLFW_KEY_F22] = "F22";
        key_to_name[GLFW_KEY_F23] = "F23";
        key_to_name[GLFW_KEY_F24] = "F24";
        key_to_name[GLFW_KEY_F25] = "F25";
        key_to_name[GLFW_KEY_KP_0] = "KP_0";
        key_to_name[GLFW_KEY_KP_1] = "KP_1";
        key_to_name[GLFW_KEY_KP_2] = "KP_2";
        key_to_name[GLFW_KEY_KP_3] = "KP_3";
        key_to_name[GLFW_KEY_KP_4] = "KP_4";
        key_to_name[GLFW_KEY_KP_5] = "KP_5";
        key_to_name[GLFW_KEY_KP_6] = "KP_6";
        key_to_name[GLFW_KEY_KP_7] = "KP_7";
        key_to_name[GLFW_KEY_KP_8] = "KP_8";
        key_to_name[GLFW_KEY_KP_9] = "KP_9";
        key_to_name[GLFW_KEY_KP_DECIMAL] = "KP_DECIMAL";
        key_to_name[GLFW_KEY_KP_DIVIDE] = "KP_DIVIDE";
        key_to_name[GLFW_KEY_KP_MULTIPLY] = "KP_MULTIPLY";
        key_to_name[GLFW_KEY_KP_SUBTRACT] = "KP_SUBTRACT";
        key_to_name[GLFW_KEY_KP_ADD] = "KP_ADD";
        key_to_name[GLFW_KEY_KP_ENTER] = "KP_ENTER";
        key_to_name[GLFW_KEY_KP_EQUAL] = "KP_EQUAL";
        key_to_name[GLFW_KEY_LEFT_SHIFT] = "LEFT_SHIFT";
        key_to_name[GLFW_KEY_LEFT_CONTROL] = "LEFT_CONTROL";
        key_to_name[GLFW_KEY_LEFT_ALT] = "LEFT_ALT";
        key_to_name[GLFW_KEY_LEFT_SUPER] = "LEFT_SUPER";
        key_to_name[GLFW_KEY_RIGHT_SHIFT] = "RIGHT_SHIFT";
        key_to_name[GLFW_KEY_RIGHT_CONTROL] = "RIGHT_CONTROL";
        key_to_name[GLFW_KEY_RIGHT_ALT] = "RIGHT_ALT";
        key_to_name[GLFW_KEY_RIGHT_SUPER] = "RIGHT_SUPER";
        key_to_name[GLFW_KEY_MENU] = "MENU";
        key_to_name[GLFW_KEY_LAST] = "LAST";
        glfwSetKeyCallback(window, cb_keyboard);
    }

    virtual void enableMouseInput() {
        glfwSetCursorPosCallback(window, cb_cursor_position);
        glfwSetCursorEnterCallback(window, cb_cursor_enter);
        glfwSetMouseButtonCallback(window, cb_mouse_button);
        glfwSetScrollCallback(window, cb_scroll);
    }

    virtual ~OpenGLWindowImpl() {
        glfwDestroyWindow(window);
    }

    Delegate* delegate;
    GLFWwindow* window;
    std::map<int, std::string> key_to_name;
};

OpenGLWindow* OpenGLWindow::Create(OpenGLWindow::Hint hint, const char* title) {
    return new OpenGLWindowImpl(hint, title);
}

OpenGLWindow* OpenGLWindow::Create(Configuration* config) {
    OpenGLWindow::Hint hint;
    hint.width = config->getInt32("window.width", 900);
    hint.height = config->getInt32("window.height", 600);
    hint.fullscreen = config->getBoolean("window.fullscreen", false);
    hint.hide_cursor = config->getBoolean("window.hide_cursor", false);
    hint.active_stereo = config->getBoolean("window.active_stereo", false);
    std::string title = config->getSTLString("window.title", "Allofw Window");
    return Create(hint, title.c_str());
}

void OpenGLWindow::Destroy(OpenGLWindow* window) {
    delete window;
}

OpenGLWindow::~OpenGLWindow() { }

// Default callbacks.
void OpenGLWindow::Delegate::onMove(int x, int y) { }
void OpenGLWindow::Delegate::onResize(int width, int height) { }
void OpenGLWindow::Delegate::onClose() { }
void OpenGLWindow::Delegate::onRefresh() { }
void OpenGLWindow::Delegate::onFocus(int focused) { }
void OpenGLWindow::Delegate::onIconify(int iconified) { }
void OpenGLWindow::Delegate::onFramebufferSize(int width, int height) { }
void OpenGLWindow::Delegate::onKeyboard(char const*, const char*, char const*, int) { }
void OpenGLWindow::Delegate::onCursorPosition(double x, double y) { }
void OpenGLWindow::Delegate::onMouseButton(const char* button, const char* action, const char* modifiers) { }
void OpenGLWindow::Delegate::onCursorEnter(bool entered) { }
void OpenGLWindow::Delegate::onScroll(double xoffset, double yoffset) { }

ALLOFW_NS_END
