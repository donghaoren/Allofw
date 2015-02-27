#include <GLFW/glfw3.h>
#include "opengl.h"

using namespace allofw;

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
        int val = GL_GEOMETRY_SHADER;
        printf("%d\n", val);
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
