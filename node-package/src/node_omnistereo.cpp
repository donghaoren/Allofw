#include "node_omnistereo.h"

using namespace v8;
using namespace allofw;

void NODE_OmniStereo::Init(v8::Handle<v8::Object> exports) {
    NanScope();
    // Prepare constructor template
    Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
    tpl->SetClassName(NanNew<String>("OmniStereo"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    NODE_SET_PROTOTYPE_METHOD(tpl, "setPose", NODE_setPose);
    NODE_SET_PROTOTYPE_METHOD(tpl, "setLens", NODE_setLens);
    NODE_SET_PROTOTYPE_METHOD(tpl, "setClipRange", NODE_setClipRange);
    NODE_SET_PROTOTYPE_METHOD(tpl, "getCubemapTexture", NODE_getCubemapTexture);
    NODE_SET_PROTOTYPE_METHOD(tpl, "getDepthCubemapTexture", NODE_getDepthCubemapTexture);
    NODE_SET_PROTOTYPE_METHOD(tpl, "capture", NODE_capture);
    NODE_SET_PROTOTYPE_METHOD(tpl, "composite", NODE_composite);
    NODE_SET_PROTOTYPE_METHOD(tpl, "setUniforms", NODE_setUniforms);
    NODE_SET_PROTOTYPE_METHOD(tpl, "getShaderCode", NODE_getShaderCode);
    NODE_SET_PROTOTYPE_METHOD(tpl, "onCaptureViewport", NODE_onCaptureViewport);

    NanAssignPersistent(constructor, tpl->GetFunction());

    // Export constructor.
    exports->Set(NanNew<String>("OmniStereo"), tpl->GetFunction());
}

NODE_OmniStereo::NODE_OmniStereo(Configuration* config) {
    omnistereo = OmniStereo::Create(config);
    omnistereo->setDelegate(this);
}

NODE_OmniStereo::~NODE_OmniStereo() {
    delete omnistereo;
}

NAN_METHOD(NODE_OmniStereo::New) {
    NanScope();

    if(args.IsConstructCall()) {
        try {
            NanUtf8String str(args[0]);
            char* argv[1] = { *str };
            Configuration* config = Configuration::ParseArgs(1, argv);
            NODE_OmniStereo* obj = new NODE_OmniStereo(config);
            obj->Wrap(args.This());
            NanReturnValue(args.This());
        } catch(const allofw::exception& e) {
            NanThrowError(e.what());
        } catch(...) {
            NanThrowError("unknown exception");
        }
    } else {
        // Invoked as plain function `MyObject(...)`, turn into construct call.
        const int argc = 2;
        Local<Value> argv[argc] = { args[0], args[1] };
        NanReturnValue(NanNew(constructor)->NewInstance(argc, argv));
    }
}

// setPose(x, y, z, qx, qy, qz, qw)
NAN_METHOD(NODE_OmniStereo::NODE_setPose) {
    NODE_OmniStereo* self = ObjectWrap::Unwrap<NODE_OmniStereo>(args.This());
    Pose pose;
    pose.position.x = args[0]->NumberValue();
    pose.position.y = args[1]->NumberValue();
    pose.position.z = args[2]->NumberValue();
    pose.rotation.x = args[3]->NumberValue();
    pose.rotation.y = args[4]->NumberValue();
    pose.rotation.z = args[5]->NumberValue();
    pose.rotation.w = args[6]->NumberValue();
    self->omnistereo->setPose(pose);
    NanReturnUndefined();
}
// setLens(eye_separation, sphere_radius)
NAN_METHOD(NODE_OmniStereo::NODE_setLens) {
    NODE_OmniStereo* self = ObjectWrap::Unwrap<NODE_OmniStereo>(args.This());
    self->omnistereo->setLens(args[0]->NumberValue(), args[1]->NumberValue());
    NanReturnUndefined();
}
// setClipRange(near, far)
NAN_METHOD(NODE_OmniStereo::NODE_setClipRange) {
    NODE_OmniStereo* self = ObjectWrap::Unwrap<NODE_OmniStereo>(args.This());
    self->omnistereo->setClipRange(args[0]->NumberValue(), args[1]->NumberValue());
    NanReturnUndefined();
}
// getCubemapTexture() -> [ left_id, right_id ]
NAN_METHOD(NODE_OmniStereo::NODE_getCubemapTexture) {
    NanScope();
    NODE_OmniStereo* self = ObjectWrap::Unwrap<NODE_OmniStereo>(args.This());
    OmniStereo::StereoTexture texture = self->omnistereo->getCubemapTexture();
    Local<Array> array = NanNew<Array>(2);
    array->Set(0, NanNew<Integer>(texture.L));
    array->Set(1, NanNew<Integer>(texture.R));
    NanReturnValue(array);
}
// getDepthCubemapTexture() -> [ left_id, right_id ]
NAN_METHOD(NODE_OmniStereo::NODE_getDepthCubemapTexture) {
    NanScope();
    NODE_OmniStereo* self = ObjectWrap::Unwrap<NODE_OmniStereo>(args.This());
    OmniStereo::StereoTexture texture = self->omnistereo->getDepthCubemapTexture();
    Local<Array> array = NanNew<Array>(2);
    array->Set(0, NanNew<Integer>(texture.L));
    array->Set(1, NanNew<Integer>(texture.R));
    NanReturnValue(array);
}
// capture()
NAN_METHOD(NODE_OmniStereo::NODE_capture) {
    NODE_OmniStereo* self = ObjectWrap::Unwrap<NODE_OmniStereo>(args.This());
    self->omnistereo->capture();
    NanReturnUndefined();
}
// composite(x, y, w, h)
NAN_METHOD(NODE_OmniStereo::NODE_composite) {
    NODE_OmniStereo* self = ObjectWrap::Unwrap<NODE_OmniStereo>(args.This());
    Rectangle2i viewport(
        args[0]->IntegerValue(),
        args[1]->IntegerValue(),
        args[2]->IntegerValue(),
        args[3]->IntegerValue()
    );
    OmniStereo::CompositeInfo composite_info;
    self->omnistereo->composite(viewport, composite_info);
    NanReturnUndefined();
}
// setUniforms(shader_id)
NAN_METHOD(NODE_OmniStereo::NODE_setUniforms) {
    NODE_OmniStereo* self = ObjectWrap::Unwrap<NODE_OmniStereo>(args.This());
    self->omnistereo->setUniforms(args[0]->IntegerValue(), *self->current_capture_info);
}
// getShaderCode() -> shader_code
NAN_METHOD(NODE_OmniStereo::NODE_getShaderCode) {
    NODE_OmniStereo* self = ObjectWrap::Unwrap<NODE_OmniStereo>(args.This());
    NanReturnValue(NanNew<String>(self->omnistereo->getShaderCode()));
}
// onCaptureViewport(callback)
//   callback(info)
NAN_METHOD(NODE_OmniStereo::NODE_onCaptureViewport) {
    NODE_OmniStereo* self = ObjectWrap::Unwrap<NODE_OmniStereo>(args.This());
    NanAssignPersistent(self->onCaptureViewport_callback, args[0].As<Function>());
    NanReturnUndefined();
}

void NODE_OmniStereo::onCaptureViewport(const CaptureInfo& info) {
    current_capture_info = &info;
    NanNew(onCaptureViewport_callback)->Call(NanObjectWrapHandle(this), 0, NULL);
}

v8::Persistent<v8::Function> NODE_OmniStereo::constructor;

void NODE_OmniStereo_init(v8::Handle<v8::Object> exports) {
    NODE_OmniStereo::Init(exports);
}
