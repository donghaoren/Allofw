#include <node.h>
#include <nan.h>

#include "node_sharedmemory.h"
#include "node_graphics.h"
#include "node_opengl.h"
#include "node_omnistereo.h"

#include <allofw/allofw.h>
#include <allofw/logger.h>

using namespace v8;

NAN_METHOD(NODE_log) {
    Nan::Utf8String str(info[1]);
    allofw::Logger::Default()->print(info[0]->IntegerValue(), *str);
}

NAN_MODULE_INIT(NODE_init) {
    Nan::HandleScope();

    allofw::allofwInit();

    Nan::Set(target, Nan::New<String>("kInfo").ToLocalChecked(), Nan::New<Uint32>(allofw::Logger::kInfo));
    Nan::Set(target, Nan::New<String>("kWarning").ToLocalChecked(), Nan::New<Uint32>(allofw::Logger::kWarning));
    Nan::Set(target, Nan::New<String>("kError").ToLocalChecked(), Nan::New<Uint32>(allofw::Logger::kError));
    Nan::Set(target, Nan::New<String>("kFatal").ToLocalChecked(), Nan::New<Uint32>(allofw::Logger::kFatal));
    Nan::Set(target, Nan::New<String>("log").ToLocalChecked(), Nan::GetFunction(Nan::New<FunctionTemplate>(NODE_log)).ToLocalChecked());

    // NODE_SharedMemory::Init(target);
    NODE_OpenGL_Init(target);

    NODE_OmniStereo_init(target);

    // Local<Object> graphics = Nan::New<Object>();
    // NODE_Graphics_init(graphics);
    // Nan::Set(target, Nan::New<String>("graphics").ToLocalChecked(), graphics);
}

NODE_MODULE(allofw, NODE_init);
