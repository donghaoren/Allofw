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
    NanReturnUndefined();
}

NAN_METHOD(EXPORT_getMarkers) {
    NanScope();
    if(!phasespace) NanReturnUndefined();


    int start = args[0]->IntegerValue();
    int count = args[1]->IntegerValue();
    vector<allofw::Vector3> v(count);
    vector<int> s(count);
    phasespace->getMarkers(&v[0], &s[0], start, count);

    Local<Array> array = NanNew<Array>(count);
    for(int i = 0; i < count; i++) {
        Local<Object> obj = NanNew<Array>(4);
        obj->Set(0, NanNew<Number>(v[i].x));
        obj->Set(1, NanNew<Number>(v[i].y));
        obj->Set(2, NanNew<Number>(v[i].z));
        obj->Set(3, NanNew<Integer>(s[i]));
        array->Set(i, obj);
    }
    NanReturnValue(array);
}


void NODE_init(Handle<Object> exports) {
    exports->Set(NanNew<String>("start"), NanNew<FunctionTemplate>(EXPORT_start)->GetFunction());
    // getMarkers(start, count) -> [ [ x, y, z, last_seen ], ... ]
    exports->Set(NanNew<String>("getMarkers"), NanNew<FunctionTemplate>(EXPORT_getMarkers)->GetFunction());
}

NODE_MODULE(node_phasespace, NODE_init)

