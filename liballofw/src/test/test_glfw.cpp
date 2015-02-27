#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
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

int main(void)
{

    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    glfwWindowHint(GLFW_STEREO, GL_TRUE);
    glfwWindowHint(GLFW_DECORATED, GL_FALSE);

    window = glfwCreateWindow(1920 * 2, 1200, "Simple example", NULL, NULL);
    showMonitors();
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetKeyCallback(window, key_callback);
    while (!glfwWindowShouldClose(window))
    {
        glDrawBuffer(GL_BACK_LEFT);
        float ratio;
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        printf("%d %d\n", width, height);
        ratio = width / (float) height;
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glRotatef((float) glfwGetTime() * 50.f, 0.f, 0.f, 1.f);
        glBegin(GL_TRIANGLES);
        glColor3f(1.f, 0.f, 0.f);
        glVertex3f(-1.6f, -1.4f, 1.f);
        glColor3f(0.f, 1.f, 0.f);
        glVertex3f(1.6f, -1.4f, 0.f);
        glColor3f(0.f, 0.f, 1.f);
        glVertex3f(0.f, 1.6f, 0.f);
        glEnd();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
