#include "allofw/omniapp_broadcasting.h"
#include "allofw/logger.h"
#include "allofw/opengl_utils.h"

#include <vector>

using namespace allofw;

// Shader Names
const char *vertex_code =
R"================(
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
out vec3 Color;
void main() {
    Color = color;
    gl_Position = omni_render(omni_transform(position));
}
)================";
const char *fragment_code =
R"================(
in vec3 Color;
layout(location = 0) out vec4 outputF;
void main() {
    outputF = vec4(Color, 1.0);
}
)================";

struct State {
    Pose pose;
    float size;
    float spacing;
};

class MyApp : public OmniApp<BroadcastingApp_Renderer<State> > {
public:
    struct VertexData {
        float x, y, z;
        float r, g, b;
    };

    MyApp() {
    }

    virtual void onInitialize() override {
        OmniApp::onInitialize();
        program_ = glutils::compileShaderProgram(
            (std::string("#version 330\n") + omni()->getShaderCode() + vertex_code).c_str(),
            (std::string("#version 330\n") + omni()->getShaderCode() + fragment_code).c_str()
        );
        setupBuffers();
    }

    virtual void onFrame(double dt) override {
        OmniApp::onFrame(dt);
        pose() = state().pose;
    }

    virtual void onCaptureViewport(const CaptureInfo& info) override {
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(program_);
        info.setUniforms(program_);

        glBindVertexArray(vertex_buffer_);
        glDrawArrays(GL_TRIANGLES, 0, vertices_count_);
    }
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

        GLuint buffer;

        glGenVertexArrays(1, &vertex_buffer_);
        glBindVertexArray(vertex_buffer_);
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), 0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)12);
        vertices_count_ = vertices.size();
    }

    GLuint vertex_buffer_;
    int vertices_count_;
    GLuint program_;
};

class MyAppSimulator : public BroadcastingApp_Simulator<State> {
public:
    virtual void onInitialize() override {
        BroadcastingApp_Simulator::onInitialize();
        state() = State();
    }
    virtual void onFrame(double dt) override {
        BroadcastingApp_Simulator::onFrame(dt);
        state().pose.rotation = state().pose.rotation * Quaternion::Rotation(Vector3(0, 0, 1), dt * 1);
        printf("fps: %lf\n", 1.0 / dt);
    }
};

int main(int argc, char* argv[]) {
    Configuration* config = Configuration::ParseMainArgs(argc, argv);
    try {
        if(config->getString("broadcasting.role", "renderer") == "simulator") {
            MyAppSimulator app;
            app.initialize(config);
            app.main();
        } else {
            MyApp app;
            app.initialize(config);
            app.main();
        }
    } catch(exception& e) {
        Logger::Default()->printf(Logger::kFatal, "Uncaught exception: %s", e.what());
        return -1;
    }
}
