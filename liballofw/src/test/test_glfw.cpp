#include "allofw/opengl.h"
#include "allofw/opengl_utils.h"
#include "allofw/omnistereo.h"
#include "allofw/logger.h"

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <vector>

using namespace allofw;

OpenGLWindow* w;
OmniStereo* omni;

struct VertexData {
    float x, y, z;
    float r, g, b;
};

#define MULTI_LINE_STRING(...) #__VA_ARGS__
// Shader Names
const char *vertexFileName =
R"================(
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
out vec3 Color;
void main()
{
    Color = color;
    gl_Position = omni_render(omni_transform(position));
}
)================";
const char *fragmentFileName =
R"================(
in vec3 Color;
layout(location = 0) out vec4 outputF;

void main()
{
    outputF = vec4(Color, 1.0);
}
)================";

// Program and Shader Identifiers
GLuint p, v, f;

// Vertex Attribute Locations
GLuint vertexLoc, colorLoc;

// Vertex Array Objects Identifiers
GLuint vao[3];
int vertices_count = 0;

void addCube(std::vector<VertexData>& vs, float x, float y, float z, float size) {
    float c0 = 0.3, c1 = 0.7;
    VertexData v0 = { x - size / 2, y - size / 2, z - size / 2, c0, c0, c0 };
    VertexData v1 = { x - size / 2, y - size / 2, z + size / 2, c0, c0, c1 };
    VertexData v2 = { x - size / 2, y + size / 2, z - size / 2, c0, c1, c0 };
    VertexData v3 = { x - size / 2, y + size / 2, z + size / 2, c0, c1, c1 };
    VertexData v4 = { x + size / 2, y - size / 2, z - size / 2, c1, c0, c0 };
    VertexData v5 = { x + size / 2, y - size / 2, z + size / 2, c1, c0, c1 };
    VertexData v6 = { x + size / 2, y + size / 2, z - size / 2, c1, c1, c0 };
    VertexData v7 = { x + size / 2, y + size / 2, z + size / 2, c1, c1, c1 };
    vs.push_back(v0); vs.push_back(v1); vs.push_back(v3); vs.push_back(v0); vs.push_back(v3); vs.push_back(v2); // 0132;
    vs.push_back(v3); vs.push_back(v1); vs.push_back(v5); vs.push_back(v3); vs.push_back(v5); vs.push_back(v7); // 3157;
    vs.push_back(v3); vs.push_back(v7); vs.push_back(v6); vs.push_back(v3); vs.push_back(v6); vs.push_back(v2); // 3762;
    vs.push_back(v1); vs.push_back(v0); vs.push_back(v4); vs.push_back(v1); vs.push_back(v4); vs.push_back(v5); // 1045;
    vs.push_back(v5); vs.push_back(v4); vs.push_back(v6); vs.push_back(v5); vs.push_back(v6); vs.push_back(v7); // 5467;
    vs.push_back(v0); vs.push_back(v2); vs.push_back(v6); vs.push_back(v0); vs.push_back(v6); vs.push_back(v4); // 0264;
}

void setupBuffers() {

    // Generate cubes.
    std::vector<VertexData> vertices;
    for(int x = -5; x <= 5; x++)
        for(int y = -5; y <= 5; y++)
            for(int z = -5; z <= 5; z++) {
                if(x == 0 && y == 0 && z == 0) continue;
                float sz = 0.1;
                if(x == 1 && y == 0 && z == 0) sz = 0.3;
                if(x == 0 && y == 1 && z == 0) sz = 0.2;
                addCube(vertices, x, y, z, sz);
            }

    GLuint buffers[1];

    glGenVertexArrays(1, vao);
    //
    // VAO for first triangle
    //
    glBindVertexArray(vao[0]);
    // Generate two slots for the vertex and color buffers
    glGenBuffers(1, buffers);
    // bind buffer for vertices and copy data into buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)12);
    vertices_count = vertices.size();
}



GLuint setupShaders() {
    p = glutils::compileShaderProgram(
        (std::string("#version 330\n") + omni->getShaderCode() + vertexFileName).c_str(),
        (std::string("#version 330\n") + omni->getShaderCode() + fragmentFileName).c_str()
    );
    return(p);
}

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
        Size2i sz = w->getFramebufferSize();
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

class _Delegate : public OmniStereo::Delegate {
public:
    virtual void onCaptureFace(const CaptureInfo& info) {
        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(p);
        info.setUniforms(p);

        glBindVertexArray(vao[0]);
        glDrawArrays(GL_TRIANGLES, 0, vertices_count);
    }
};

_Delegate delegate;

int main(void)
{
    LoggerScope logger(Logger::kInfo, "Main");

    OpenGLWindow::Hint hint;
    Configuration* conf = Configuration::Create();
    conf->parseFile("allofw.yaml");
    w = OpenGLWindow::Create(conf);
    w->makeContextCurrent();

    omni = OmniStereo::Create(conf);
    omni->setDelegate(&delegate);

    // OpenGL Version.
    Size2i sz = w->getFramebufferSize();
    logger.printf("Framebuffer size: %d %d\n", sz.x, sz.y);
    logger.printf("OpenGL: %s\n", glGetString(GL_VERSION));

    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    p = setupShaders();
    setupBuffers();

    MyDelegate d;
    w->setDelegate(&d);
    int frame_index = 0;
    while(1) {
        Size2i sz = w->getFramebufferSize();
        Pose pose;
        //pose.rotation = Quaternion::rotation(Vector3(1, 0, 0), frame_index / 100.0).inversion();
        omni->setPose(pose);
        omni->capture();
        OmniStereo::CompositeInfo info;
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        omni->composite(Rectangle2i(0, 0, sz.x, sz.y), info);
        w->swapBuffers();
        int err = glGetError();
        if(err) printf("%d\n", err);
        w->pollEvents();
        frame_index += 1;
    }
}
