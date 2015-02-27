#include <GLFW/glfw3.h>
#include "opengl.h"

using namespace allofw;

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

void showMonitors() {
    int count;
    GLFWmonitor** monitors = glfwGetMonitors(&count);
    for(int i = 0; i < count; i++) {
        int x, y;
        printf("Monitor %d\n", i);
        printf("  name: %s\n", glfwGetMonitorName(monitors[i]));
        glfwGetMonitorPos(monitors[i], &x, &y);
        printf("  glfwGetMonitorPos: %d %d\n", x, y);
        const GLFWvidmode* vm = glfwGetVideoMode(monitors[i]);
        printf("  glfwGetVideoMode: %d %d\n", vm->width, vm->height);
    }
}
POpenGLWindow w;

class MyDelegate : public OpenGLWindow::Delegate {
public:
    virtual void onMove(int x, int y) {
        printf("onMove: %d %d\n", x, y);
    }
    virtual void onResize(int width, int height) {
        printf("onResize: %d %d\n", width, height);
    }
    virtual void onClose() {
        printf("onClose\n");
    }
    virtual void onRefresh() {
        glClearColor(0.1, 0.3, 0.5, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        glFlush();
        w->swapBuffers();
        printf("onRefresh\n");
    }
    virtual void onFocus(int focused) {
        printf("onFocus: %d\n", focused);
    }
    virtual void onIconify(int iconified) {
        printf("onIconify: %d\n", iconified);
    }
    virtual void onFramebufferSize(int width, int height) {
        glViewport(0, 0, width, height);
        printf("onFramebufferSize: %d %d\n", width, height);
    }
};

int main(void)
{

    OpenGLWindow::Hint hint;
    w = OpenGLWindow::Create(hint, "Hello World");
    w->makeContextCurrent();

    // OpenGL Version.
    Size2i sz = w->getFramebufferSize();
    printf("Framebuffer size: %d %d\n", sz.x, sz.y);

    printf("OpenGL: %s\n", glGetString(GL_VERSION));

    MyDelegate d;
    w->setDelegate(&d);
    while(1) {
        w->pollEvents();
    }
}
