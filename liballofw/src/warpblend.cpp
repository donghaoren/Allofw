#include "allofw/omnistereo.h"
#include "allofw/opengl.h"
#include "allofw/logger.h"

#include <yaml-cpp/yaml.h>
#include <lodepng.h>

#include <vector>
#include <cmath>

#ifdef _WIN32
#include <winsock.h>
#else
#include <unistd.h>
#endif


ALLOFW_NS_BEGIN

class WarpBlendImpl : public WarpBlend {
public:

    // How many viewports to draw.
    virtual int getViewportCount() {
        return data.size();
    }
    virtual ViewportInfo getViewport(int viewport) {
        return data[viewport].viewport;
    }

    // Get warp and blend data.
    virtual BlendData getBlendData(int viewport) {
        return data[viewport].blend;
    }
    virtual WarpData getWarpData(int viewport) {
        return data[viewport].warp;
    }

    // Get warp and blend textures.
    virtual GLTextureID getBlendTexture(int viewport) {
        ViewportData& vp = data[viewport];
        if(vp.blend_texture) return vp.blend_texture;
        glGenTextures(1, &vp.blend_texture);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, vp.blend_texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, vp.blend.size.w, vp.blend.size.h, 0, GL_RGBA, GL_FLOAT, vp.blend.data);
        glBindTexture(GL_TEXTURE_2D, 0);
        return vp.blend_texture;

    }
    virtual GLTextureID getWarpTexture(int viewport) {
        ViewportData& vp = data[viewport];
        if(vp.warp_texture) return vp.warp_texture;
        glGenTextures(1, &vp.warp_texture);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, vp.warp_texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, vp.warp.size.w, vp.warp.size.h, 0, GL_RGB, GL_FLOAT, vp.warp.data);
        glBindTexture(GL_TEXTURE_2D, 0);
        return vp.warp_texture;
    }

    void createEquirectangular(int width, int height) {
        data.resize(1);
        int W = width;
        int H = height;
        data[0].allocate(Size2i(W, H), Size2i(W, H));
        data[0].viewport.viewport = Rectangle2(0, 0, 1, 1);
        data[0].viewport.aspect_ratio = 2.0;
        data[0].viewport.enforce_aspect_ratio = true;
        data[0].viewport.screen_resolution = Size2i(width, height);
        for(int y = 0; y < H; y++) {
            for(int x = 0; x < W; x++) {
                int i = x + y * W;
                data[0].blend.data[i] = Vector4f(1, 1, 1, 1);
                float theta = (((float)x + 0.5f) / (float)W - 0.5f) * PI * 2.0f;
                float phi = (((float)y + 0.5f) / (float)H - 0.5f) * PI;
                float vx = std::sin(theta) * std::cos(phi);
                float vz = -std::cos(theta) * std::cos(phi);
                float vy = std::sin(phi);
                data[0].warp.data[i] = Vector3f(vx, vy, vz);
            }
        }
    }
    void createPerspective(int width, int height, float fov) {
        data.resize(1);
        int W = width;
        int H = height;
        data[0].allocate(Size2i(W, H), Size2i(W, H));
        data[0].viewport.viewport = Rectangle2(0, 0, 1, 1);
        data[0].viewport.aspect_ratio = 1.0;
        data[0].viewport.enforce_aspect_ratio = true;
        data[0].viewport.screen_resolution = Size2i(width, height);
        float scale = 2.0f * tan(fov / 2.0f);
        for(int y = 0; y < H; y++) {
            for(int x = 0; x < W; x++) {
                int i = x + y * W;
                float px = ((float)x / (float)W - 0.5) * scale;
                float py = ((float)y / (float)H - 0.5) * scale;
                float pz = -1.0f;
                data[0].blend.data[i] = Vector4f(1, 1, 1, 1);
                data[0].warp.data[i] = Vector3f(px, py, pz);
            }
        }
    }

    void loadAllosphereCalibration(const char* calibration_path, const char* hostname) {
        LoggerScope(Logger::kInfo, "WarpBlend::loadAllosphereCalibration");
        char hostname_cache[256];
        if(!hostname) {
            gethostname(hostname_cache, 256);
            hostname = hostname_cache;
        }
        YAML::Node conf = YAML::LoadFile(std::string(calibration_path) + "/" + hostname + ".json");
        data.resize(conf["projections"].size());

        Matrix3d calibration_to_user_matrix = Matrix3d::eye();
        Vector3d calibration_to_user_offset(0, 0, 0);

        try {
            YAML::Node phasespace_registration = YAML::LoadFile(std::string(calibration_path) + "/phasespace_registration.json");
            Logger::Default()->printf(Logger::kInfo, "Reading phasespace registration data.");
            calibration_to_user_matrix.a11 = phasespace_registration["calibration_to_user"]["matrix"][0][0].as<double>();
            calibration_to_user_matrix.a12 = phasespace_registration["calibration_to_user"]["matrix"][0][1].as<double>();
            calibration_to_user_matrix.a13 = phasespace_registration["calibration_to_user"]["matrix"][0][2].as<double>();
            calibration_to_user_matrix.a21 = phasespace_registration["calibration_to_user"]["matrix"][1][0].as<double>();
            calibration_to_user_matrix.a22 = phasespace_registration["calibration_to_user"]["matrix"][1][1].as<double>();
            calibration_to_user_matrix.a23 = phasespace_registration["calibration_to_user"]["matrix"][1][2].as<double>();
            calibration_to_user_matrix.a31 = phasespace_registration["calibration_to_user"]["matrix"][2][0].as<double>();
            calibration_to_user_matrix.a32 = phasespace_registration["calibration_to_user"]["matrix"][2][1].as<double>();
            calibration_to_user_matrix.a33 = phasespace_registration["calibration_to_user"]["matrix"][2][2].as<double>();
            calibration_to_user_offset.x = phasespace_registration["calibration_to_user"]["offset"][0].as<double>();
            calibration_to_user_offset.y = phasespace_registration["calibration_to_user"]["offset"][1].as<double>();
            calibration_to_user_offset.z = phasespace_registration["calibration_to_user"]["offset"][2].as<double>();
        } catch(...) {
        }

        for(size_t p = 0; p < data.size(); p++) {
            ViewportData& vp = data[p];
            YAML::Node proj = conf["projections"][p];
            vp.viewport.viewport.x = proj["viewport"]["l"].as<float>();
            vp.viewport.viewport.y = proj["viewport"]["b"].as<float>();
            vp.viewport.viewport.w = proj["viewport"]["w"].as<float>();
            vp.viewport.viewport.h = proj["viewport"]["h"].as<float>();
            std::string blend_file = proj["blend"]["file"].as<std::string>();
            std::vector<unsigned char> blend_image;
            unsigned int blend_image_width, blend_image_height;
            unsigned int error = lodepng::decode(blend_image, blend_image_width, blend_image_height, (std::string(calibration_path) + "/" + blend_file).c_str());
            vp.blend.size.w = blend_image_width;
            vp.blend.size.h = blend_image_height;
            vp.blend.data = new Vector4f[vp.blend.size.w * vp.blend.size.h];
            for(int y = 0; y < vp.blend.size.h; y++) {
                unsigned char* scanline = &blend_image[(vp.blend.size.h - 1 - y) * vp.blend.size.w * 4];
                Vector4f* out = vp.blend.data + y * vp.blend.size.w;
                for(int x = 0; x < vp.blend.size.w; x++) {
                    float value = scanline[x * 4 + 0] / 255.0;
                    out[x] = Vector4f(value, value, value, 1.0f);
                }
            }
            std::string warp_file = proj["warp"]["file"].as<std::string>();
            int32_t warp_size[2];
            FILE* fwarp = fopen((std::string(calibration_path) + "/" + warp_file).c_str(), "rb");
            fread(warp_size, sizeof(int32_t), 2, fwarp);
            vp.warp.size.w = warp_size[1];
            vp.warp.size.h = warp_size[0] / 3;
            vp.warp.data = new Vector3f[vp.warp.size.w * vp.warp.size.h];
            vp.viewport.aspect_ratio = (float)vp.warp.size.w / (float)vp.warp.size.h;
            vp.viewport.enforce_aspect_ratio = false;
            vp.viewport.screen_resolution = vp.warp.size;
            float* buf = new float[vp.warp.size.w * vp.warp.size.h];
            fread(buf, sizeof(float), vp.warp.size.w * vp.warp.size.h, fwarp); // x
            for(int j = 0; j < vp.warp.size.w * vp.warp.size.h; j++) {
                vp.warp.data[j].x = buf[j];
            }
            fread(buf, sizeof(float), vp.warp.size.w * vp.warp.size.h, fwarp); // y
            for(int j = 0; j < vp.warp.size.w * vp.warp.size.h; j++) {
                vp.warp.data[j].y = buf[j];
            }
            fread(buf, sizeof(float), vp.warp.size.w * vp.warp.size.h, fwarp); // z
            for(int j = 0; j < vp.warp.size.w * vp.warp.size.h; j++) {
                vp.warp.data[j].z = buf[j];
            }
            // Flip y axis.
            for(int y = 0; y < vp.warp.size.h / 2; y++) {
                Vector3f* row1 = vp.warp.data + y * vp.warp.size.w;
                Vector3f* row2 = vp.warp.data + (vp.warp.size.h - 1 - y) * vp.warp.size.w;
                for(int x = 0; x < vp.warp.size.w; x++) {
                    Vector3f tmp = row1[x];
                    row1[x] = row2[x];
                    row2[x] = tmp;
                }
            }
            for(int j = 0; j < vp.warp.size.w * vp.warp.size.h; j++) {
                vp.warp.data[j] = calibration_to_user_matrix * vp.warp.data[j] + calibration_to_user_offset;
            }
            delete [] buf;
            fclose(fwarp);
        }
    }

    ~WarpBlendImpl() {
        for(size_t i = 0; i < data.size(); i++) {
            data[i].deallocate();
        }
    }

    struct ViewportData {
        WarpData warp;
        BlendData blend;
        ViewportInfo viewport;
        GLuint warp_texture;
        GLuint blend_texture;

        ViewportData() {
            warp.size = Size2i(0, 0);
            warp.data = nullptr;
            blend.size = Size2i(0, 0);
            blend.data = nullptr;
            warp_texture = 0;
            blend_texture = 0;
        }

        void allocate(Size2i warp_size, Size2i blend_size) {
            warp.size = warp_size;
            warp.data = new Vector3f[warp.size.w * warp.size.h];
            blend.size = blend_size;
            blend.data = new Vector4f[blend.size.w * blend.size.h];
        }

        void deallocate() {
            if(warp.data) delete [] warp.data;
            warp.data = nullptr;
            warp.size = Size2i(0, 0);
            if(blend.data) delete [] blend.data;
            blend.data = nullptr;
            blend.size = Size2i(0, 0);
            if(warp_texture) glDeleteTextures(1, &warp_texture);
            warp_texture = 0;
            if(blend_texture) glDeleteTextures(1, &blend_texture);
            blend_texture = 0;
        }
    };
    std::vector<ViewportData> data;
};

WarpBlend* WarpBlend::CreateEquirectangular(int width, int height) {
    WarpBlendImpl* impl = new WarpBlendImpl();
    impl->createEquirectangular(width, height);
    return impl;
}

WarpBlend* WarpBlend::CreatePerspective(int width, int height, float fov) {
    WarpBlendImpl* impl = new WarpBlendImpl();
    impl->createPerspective(width, height, fov);
    return impl;
}

WarpBlend* WarpBlend::LoadAllosphereCalibration(const char* calibration_path, const char* hostname) {
    WarpBlendImpl* impl = new WarpBlendImpl();
    impl->loadAllosphereCalibration(calibration_path, hostname);
    return impl;
}

void WarpBlend::Destroy(WarpBlend* warpblend) {
    delete warpblend;
}

WarpBlend::~WarpBlend() { }

ALLOFW_NS_END
