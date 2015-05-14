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
    NanUtf8String str(args[1]);
    allofw::Logger::Default()->print(args[0]->IntegerValue(), *str);
}

void NODE_init(Handle<Object> exports) {
    NanScope();

    allofw::allofwInit();

    exports->Set(NanNew<String>("kInfo"), NanNew<Uint32>(allofw::Logger::kInfo));
    exports->Set(NanNew<String>("kWarning"), NanNew<Uint32>(allofw::Logger::kWarning));
    exports->Set(NanNew<String>("kError"), NanNew<Uint32>(allofw::Logger::kError));
    exports->Set(NanNew<String>("kFatal"), NanNew<Uint32>(allofw::Logger::kFatal));
    exports->Set(NanNew<String>("log"), NanNew<FunctionTemplate>(NODE_log)->GetFunction());

    NODE_SharedMemory::Init(exports);
    NODE_OpenGL_Init(exports);

    NODE_OmniStereo_init(exports);

    Local<Object> graphics = NanNew<Object>();
    NODE_Graphics_init(graphics);
    exports->Set(NanNew<String>("graphics"), graphics);
}

NODE_MODULE(allofw, NODE_init);
