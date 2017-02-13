#include <node.h>
#include <nan.h>

#include "node_omnistereo.h"

using namespace v8;

NAN_MODULE_INIT(NODE_init) {
    Nan::HandleScope();

    NODE_OpenVROmniStereo_init(target);
}

NODE_MODULE(allofw_openvr, NODE_init);
