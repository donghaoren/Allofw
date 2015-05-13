#include "allofw/omnistereo.h"
#include "allofw/opengl.h"
#include "allofw/logger.h"

#include <yaml-cpp/yaml.h>
#include <FreeImage.h>

#include <vector>
#include <cmath>

#include <unistd.h>

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

    void createEquirectangular() {
        data.resize(1);
        int W = 1000;
        int H = 500;
        data[0].allocate(Size2i(W, H), Size2i(W, H));
        data[0].viewport.viewport = Rectangle2(0, 0, 1, 1);
        data[0].viewport.aspect_ratio = 2.0;
        data[0].viewport.enforce_aspect_ratio = true;
        data[0].viewport.screen_resolution = Size2i(2000, 1000);
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
    void createPerspective(float fov) {
        data.resize(1);
        int W = 800;
        int H = 800;
        data[0].allocate(Size2i(W, H), Size2i(W, H));
        data[0].viewport.viewport = Rectangle2(0, 0, 1, 1);
        data[0].viewport.aspect_ratio = 1.0;
        data[0].viewport.enforce_aspect_ratio = true;
        data[0].viewport.screen_resolution = Size2i(1600, 1600);
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
        for(size_t p = 0; p < data.size(); p++) {
            ViewportData& vp = data[p];
            YAML::Node proj = conf["projections"][p];
            vp.viewport.viewport.x = proj["viewport"]["l"].as<float>();
            vp.viewport.viewport.y = proj["viewport"]["b"].as<float>();
            vp.viewport.viewport.w = proj["viewport"]["w"].as<float>();
            vp.viewport.viewport.h = proj["viewport"]["h"].as<float>();
            std::string blend_file = proj["blend"]["file"].as<std::string>();
            FIBITMAP *blend_image = FreeImage_Load(FIF_PNG, (std::string(calibration_path) + "/" + blend_file).c_str(), PNG_DEFAULT);
            blend_image = FreeImage_ConvertTo24Bits(blend_image);
            vp.blend.size.w = FreeImage_GetWidth(blend_image);
            vp.blend.size.h = FreeImage_GetHeight(blend_image);
            vp.blend.data = new Vector4f[vp.blend.size.w * vp.blend.size.h];
            for(int y = 0; y < vp.blend.size.h; y++) {
                BYTE* scanline = FreeImage_GetScanLine(blend_image, y);
                Vector4f* out = vp.blend.data + y * vp.blend.size.w;
                for(int x = 0; x < vp.blend.size.w; x++) {
                    float value = scanline[x * 3 + 0] / 255.0;
                    out[x] = Vector4f(value, value, value, 1.0f);
                }
            }
            FreeImage_Unload(blend_image);
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

WarpBlend* WarpBlend::CreateEquirectangular() {
    WarpBlendImpl* impl = new WarpBlendImpl();
    impl->createEquirectangular();
    return impl;
}

WarpBlend* WarpBlend::CreatePerspective(float fov) {
    WarpBlendImpl* impl = new WarpBlendImpl();
    impl->createPerspective(fov);
    return impl;
}

WarpBlend* WarpBlend::LoadAllosphereCalibration(const char* calibration_path, const char* hostname) {
    WarpBlendImpl* impl = new WarpBlendImpl();
    impl->loadAllosphereCalibration(calibration_path, hostname);
    return impl;
}

ALLOFW_NS_END
