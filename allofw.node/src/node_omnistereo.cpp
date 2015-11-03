#include "node_omnistereo.h"

using namespace v8;
using namespace allofw;

void NODE_OmniStereo::Init(v8::Handle<v8::Object> exports) {
    Nan::HandleScope scope;
    // Prepare constructor template
    Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
    tpl->SetClassName(Nan::New<String>("OmniStereo").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    Nan::SetPrototypeMethod(tpl, "setPose", NODE_setPose);
    Nan::SetPrototypeMethod(tpl, "setLens", NODE_setLens);
    Nan::SetPrototypeMethod(tpl, "setClipRange", NODE_setClipRange);
    Nan::SetPrototypeMethod(tpl, "getCubemapTexture", NODE_getCubemapTexture);
    Nan::SetPrototypeMethod(tpl, "getDepthCubemapTexture", NODE_getDepthCubemapTexture);
    Nan::SetPrototypeMethod(tpl, "capture", NODE_capture);
    Nan::SetPrototypeMethod(tpl, "composite", NODE_composite);
    Nan::SetPrototypeMethod(tpl, "setUniforms", NODE_setUniforms);
    Nan::SetPrototypeMethod(tpl, "getShaderCode", NODE_getShaderCode);
    Nan::SetPrototypeMethod(tpl, "compositeCustomizeShader", NODE_compositeCustomizeShader);
    Nan::SetPrototypeMethod(tpl, "compositeRestoreShader", NODE_compositeRestoreShader);
    Nan::SetPrototypeMethod(tpl, "onCaptureViewport", NODE_onCaptureViewport);

    constructor.Reset(tpl->GetFunction());

    // Export constructor.
    Nan::Set(exports, Nan::New<String>("OmniStereo").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

NODE_OmniStereo::NODE_OmniStereo(Configuration* config) {
    omnistereo = OmniStereo::Create(config);
    omnistereo->setDelegate(this);
}

NODE_OmniStereo::~NODE_OmniStereo() {
    allofw::OmniStereo::Destroy(omnistereo);
}

NAN_METHOD(NODE_OmniStereo::New) {
    Nan::HandleScope scope;

    if(info.IsConstructCall()) {
        try {
            Configuration* config;
            if(info[0]->IsString()) {
                Nan::Utf8String str(info[0]);
                config = Configuration::CreateFromFile(*str);
            } else {
                config = Configuration::CreateFromFile(nullptr);
            }
            NODE_OmniStereo* obj = new NODE_OmniStereo(config);
            obj->Wrap(info.This());
            return info.GetReturnValue().Set(info.This());
        } catch(const allofw::exception& e) {
            return Nan::ThrowError(e.what());
        } catch(...) {
            return Nan::ThrowError("unknown exception");
        }
    } else {
        // Invoked as plain function `MyObject(...)`, turn into construct call.
        const int argc = 2;
        Local<Value> argv[argc] = { info[0], info[1] };
        return info.GetReturnValue().Set(Nan::New(constructor)->NewInstance(argc, argv));
    }
}

// setPose(x, y, z, qx, qy, qz, qw)
NAN_METHOD(NODE_OmniStereo::NODE_setPose) {
    NODE_OmniStereo* self = ObjectWrap::Unwrap<NODE_OmniStereo>(info.This());
    Pose pose;
    pose.position.x = info[0]->NumberValue();
    pose.position.y = info[1]->NumberValue();
    pose.position.z = info[2]->NumberValue();
    pose.rotation.x = info[3]->NumberValue();
    pose.rotation.y = info[4]->NumberValue();
    pose.rotation.z = info[5]->NumberValue();
    pose.rotation.w = info[6]->NumberValue();
    self->omnistereo->setPose(pose);
}
// setLens(eye_separation, sphere_radius)
NAN_METHOD(NODE_OmniStereo::NODE_setLens) {
    NODE_OmniStereo* self = ObjectWrap::Unwrap<NODE_OmniStereo>(info.This());
    self->omnistereo->setLens(info[0]->NumberValue(), info[1]->NumberValue());
}
// setClipRange(near, far)
NAN_METHOD(NODE_OmniStereo::NODE_setClipRange) {
    NODE_OmniStereo* self = ObjectWrap::Unwrap<NODE_OmniStereo>(info.This());
    self->omnistereo->setClipRange(info[0]->NumberValue(), info[1]->NumberValue());
}
// getCubemapTexture() -> [ left_id, right_id ]
NAN_METHOD(NODE_OmniStereo::NODE_getCubemapTexture) {
    Nan::HandleScope scope;
    NODE_OmniStereo* self = ObjectWrap::Unwrap<NODE_OmniStereo>(info.This());
    OmniStereo::StereoTexture texture = self->omnistereo->getCubemapTexture();
    Local<Array> array = Nan::New<Array>(2);
    array->Set(0, Nan::New<Integer>(texture.L));
    array->Set(1, Nan::New<Integer>(texture.R));
    info.GetReturnValue().Set(array);
}
// getDepthCubemapTexture() -> [ left_id, right_id ]
NAN_METHOD(NODE_OmniStereo::NODE_getDepthCubemapTexture) {
    Nan::HandleScope scope;
    NODE_OmniStereo* self = ObjectWrap::Unwrap<NODE_OmniStereo>(info.This());
    OmniStereo::StereoTexture texture = self->omnistereo->getDepthCubemapTexture();
    Local<Array> array = Nan::New<Array>(2);
    array->Set(0, Nan::New<Integer>(texture.L));
    array->Set(1, Nan::New<Integer>(texture.R));
    info.GetReturnValue().Set(array);
}
// capture()
NAN_METHOD(NODE_OmniStereo::NODE_capture) {
    NODE_OmniStereo* self = ObjectWrap::Unwrap<NODE_OmniStereo>(info.This());
    self->omnistereo->capture();
}
// composite(x, y, w, h)
NAN_METHOD(NODE_OmniStereo::NODE_composite) {
    NODE_OmniStereo* self = ObjectWrap::Unwrap<NODE_OmniStereo>(info.This());
    Rectangle2i viewport(
        info[0]->IntegerValue(),
        info[1]->IntegerValue(),
        info[2]->IntegerValue(),
        info[3]->IntegerValue()
    );
    OmniStereo::CompositeInfo composite_info;
    if(info[4]->IsObject()) {
        Handle<Object> obj = info[4]->ToObject();
        if(Nan::Has(obj, Nan::New<String>("panorama").ToLocalChecked()).FromMaybe(false)) {
            if(Nan::Get(obj, Nan::New<String>("panorama").ToLocalChecked()).ToLocalChecked()->IsArray()) {
                Handle<Object> val = Nan::Get(obj, Nan::New<String>("panorama").ToLocalChecked()).ToLocalChecked()->ToObject();
                if(val->Get(0)->IsObject()) {
                    int len = val->Get(0)->ToObject()->Get(Nan::New<String>("length").ToLocalChecked())->IntegerValue();
                    for(int i = 0; i < len && i < 3; i++) {
                        composite_info.panorama_planes[i].L = val->Get(0)->ToObject()->Get(i)->Uint32Value();
                    }
                } else {
                    composite_info.panorama.L = val->Get(0)->Uint32Value();
                }
                if(val->Get(1)->IsObject()) {
                    int len = val->Get(1)->ToObject()->Get(Nan::New<String>("length").ToLocalChecked())->IntegerValue();
                    for(int i = 0; i < len && i < 3; i++) {
                        composite_info.panorama_planes[i].R = val->Get(1)->ToObject()->Get(i)->Uint32Value();
                    }
                } else {
                    composite_info.panorama.R = val->Get(1)->Uint32Value();
                }
                Nan::Utf8String str(val->Get(2));
                std::string s = *str;
                if(s == "cubemap") {
                    composite_info.mask |= OmniStereo::kCompositeMask_Panorama | OmniStereo::kCompositeMask_Panorama_Cubemap;
                } else if(s == "cubemap-yuv420p") {
                    composite_info.mask |= OmniStereo::kCompositeMask_Panorama | OmniStereo::kCompositeMask_Panorama_Cubemap_YUV420P;
                } else {
                    composite_info.mask |= OmniStereo::kCompositeMask_Panorama | OmniStereo::kCompositeMask_Panorama_Equirectangular;
                }
            }
        }
    }
    self->omnistereo->composite(viewport, composite_info);
}
// setUniforms(shader_id)
NAN_METHOD(NODE_OmniStereo::NODE_setUniforms) {
    NODE_OmniStereo* self = ObjectWrap::Unwrap<NODE_OmniStereo>(info.This());
    self->omnistereo->setUniforms(info[0]->IntegerValue(), *self->current_capture_info);
}
// getShaderCode() -> shader_code
NAN_METHOD(NODE_OmniStereo::NODE_getShaderCode) {
    NODE_OmniStereo* self = ObjectWrap::Unwrap<NODE_OmniStereo>(info.This());
    info.GetReturnValue().Set(Nan::New<String>(self->omnistereo->getShaderCode()).ToLocalChecked());
}
NAN_METHOD(NODE_OmniStereo::NODE_compositeCustomizeShader) {
    NODE_OmniStereo* self = ObjectWrap::Unwrap<NODE_OmniStereo>(info.This());
    Nan::Utf8String str(info[0]);
    info.GetReturnValue().Set(Nan::New<Integer>(self->omnistereo->compositeCustomizeShader(*str)));
}
NAN_METHOD(NODE_OmniStereo::NODE_compositeRestoreShader) {
    NODE_OmniStereo* self = ObjectWrap::Unwrap<NODE_OmniStereo>(info.This());
    self->omnistereo->compositeRestoreShader();
}
// onCaptureViewport(callback)
//   callback(info)
NAN_METHOD(NODE_OmniStereo::NODE_onCaptureViewport) {
    NODE_OmniStereo* self = ObjectWrap::Unwrap<NODE_OmniStereo>(info.This());
    self->onCaptureViewport_callback.Reset(info[0].As<Function>());
}

void NODE_OmniStereo::onCaptureViewport(const CaptureInfo& info) {
    current_capture_info = &info;
    Nan::New(onCaptureViewport_callback)->Call(handle(), 0, NULL);
}

Nan::Persistent<v8::Function> NODE_OmniStereo::constructor;

void NODE_OmniStereo_init(v8::Handle<v8::Object> exports) {
    NODE_OmniStereo::Init(exports);
}
