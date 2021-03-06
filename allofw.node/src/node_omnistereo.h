#include <node.h>
#include <v8.h>
#include <nan.h>

#include <allofw/omnistereo.h>

class NODE_OmniStereo : public Nan::ObjectWrap, public allofw::OmniStereo::Delegate {
public:

    static void Init(v8::Handle<v8::Object> exports);

    allofw::OmniStereo* omnistereo;
    Nan::Persistent<v8::Function> onCaptureViewport_callback;

private:
    explicit NODE_OmniStereo(allofw::Configuration* config);
    ~NODE_OmniStereo();

    static NAN_METHOD(New);

    // setPose(x, y, z, qx, qy, qz, qw)
    static NAN_METHOD(NODE_setPose);
    // setLens(eye_separation, sphere_radius)
    static NAN_METHOD(NODE_setLens);
    // setClipRange(near, far)
    static NAN_METHOD(NODE_setClipRange);
    // getCubemapTexture() -> [ left_id, right_id ]
    static NAN_METHOD(NODE_getCubemapTexture);
    // getDepthCubemapTexture() -> [ left_id, right_id ]
    static NAN_METHOD(NODE_getDepthCubemapTexture);
    // capture()
    static NAN_METHOD(NODE_capture);
    // composite(x, y, w, h)
    static NAN_METHOD(NODE_composite);
    // setUniforms(shader_id)
    static NAN_METHOD(NODE_setUniforms);
    // getShaderCode() -> shader_code
    static NAN_METHOD(NODE_getShaderCode);
    // compositeCustomizeShader(main_code)
    static NAN_METHOD(NODE_compositeCustomizeShader);
    // compositeRestoreShader()
    static NAN_METHOD(NODE_compositeRestoreShader);
    // onCaptureViewport(callback)
    //   callback(info)
    static NAN_METHOD(NODE_onCaptureViewport);

    static Nan::Persistent<v8::Function> constructor;

    virtual void onCaptureViewport(const CaptureInfo& info) override;
    const CaptureInfo* current_capture_info;
};

void NODE_OmniStereo_init(v8::Handle<v8::Object> exports);
