#include <node.h>
#include <v8.h>

#include "node_sharedmemory.h"
#include "node_graphics.h"

#include <allofw/allofw.h>

using namespace v8;

void NODE_init(Handle<Object> exports) {
    allofw::allofwInit();
    NODE_SharedMemory::Init(exports);
    NODE_Graphics_init(exports);
}

NODE_MODULE(allofw, NODE_init);
