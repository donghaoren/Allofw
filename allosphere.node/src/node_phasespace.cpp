#include <phasespace.h>
#include <node.h>
#include <nan.h>
#include <vector>

using namespace std;
using namespace node;
using namespace v8;

allofw::Phasespace* phasespace = NULL;

NAN_METHOD(EXPORT_start) {
    if(!phasespace) {
        phasespace = allofw::Phasespace::Create();
        phasespace->start();
    }
}

NAN_METHOD(EXPORT_getMarkers) {
    Nan::HandleScope scope;
    if(!phasespace) return;


    int start = info[0]->IntegerValue();
    int count = info[1]->IntegerValue();
    vector<allofw::Vector3> v(count);
    vector<int> s(count);
    phasespace->getMarkers(&v[0], &s[0], start, count);

    Local<Array> array = Nan::New<Array>(count);
    for(int i = 0; i < count; i++) {
        Local<Object> obj = Nan::New<Array>(4);
        obj->Set(0, Nan::New<Number>(v[i].x));
        obj->Set(1, Nan::New<Number>(v[i].y));
        obj->Set(2, Nan::New<Number>(v[i].z));
        obj->Set(3, Nan::New<Integer>(s[i]));
        array->Set(i, obj);
    }
    info.GetReturnValue().Set(array);
}


void NODE_init(Handle<Object> exports) {
    Nan::Set(exports, Nan::New<String>("start").ToLocalChecked(), Nan::New<FunctionTemplate>(EXPORT_start)->GetFunction());
    // getMarkers(start, count) -> [ [ x, y, z, last_seen ], ... ]
    Nan::Set(exports, Nan::New<String>("getMarkers").ToLocalChecked(), Nan::New<FunctionTemplate>(EXPORT_getMarkers)->GetFunction());
}

NODE_MODULE(node_phasespace, NODE_init)

