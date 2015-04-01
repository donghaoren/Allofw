#include <node.h>
#include <nan.h>

#include "node_sharedmemory.h"
#include "node_graphics.h"
#include "node_opengl.h"

#include <allofw/allofw.h>

using namespace v8;

void NODE_init(Handle<Object> exports) {
    NanScope();

    allofw::allofwInit();

    NODE_SharedMemory::Init(exports);
    NODE_OpenGL_Init(exports);

    Local<Object> graphics = Object::New();
    NODE_Graphics_init(graphics);
    exports->Set(String::NewSymbol("graphics"), graphics);
}

NODE_MODULE(allofw, NODE_init);
