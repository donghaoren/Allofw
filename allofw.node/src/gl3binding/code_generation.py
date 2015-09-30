from gltypes import types

constant_lines = []
constant_assert_lines = []
functions_definitions = []
function_codes = []

def DefineConstant(name, c_name, value, type = "Uint32"):
    constant_lines.append("""Nan::SetTemplate(exports, Nan::New<v8::String>("%s").ToLocalChecked(), Nan::New<v8::%s>(%s), v8::ReadOnly);""" % (name, type, c_name))
    if value == None: print name
    if type == "Uint32" and value > 0xffffffff:
        print name
    constant_assert_lines.append("""#if (%s != %d)\n #error "GL3 constant %s's value is incorrect."\n#endif""" % (c_name, value, name))

def DefineFunction(name, code):
    c_name = "EXPORT_%s" % name
    functions_definitions.append("""Nan::SetTemplate(exports, "%s", Nan::New<v8::FunctionTemplate>(%s));""" % (name, c_name))
    function_codes.append("""NAN_METHOD(%s) {\n%s\n}""" % (c_name, code))

def indent(strings, spaces):
    return map(lambda x: spaces + x, strings)

code_define_constants = """
void defineConstants(v8::Handle<v8::ObjectTemplate> exports) {
%s
}
"""

code_define_functions = """
void defineFunctions(v8::Handle<v8::ObjectTemplate> exports) {
%s
}
"""

code_define_objects = """
void defineObjects(v8::Handle<v8::ObjectTemplate> exports) {
%s
}
"""

code_helper_functions = """
void do_nothing_release_callback(char*, void*) { }
void* glbind_get_buffer_data(v8::Handle<v8::Value> value) {
    if(value->IsArrayBufferView()) {
        v8::ArrayBufferView* view = v8::ArrayBufferView::Cast(*value);
        unsigned char* ptr = (unsigned char*)view->Buffer()->GetContents().Data();
        ptr += view->ByteOffset();
        // This is because ArrayBuffer::GetContents is not available yet (v8 version too low).
        // Here, we use Externalize, by not deallocating the memory here, the client side is still able to access it.
        // However, the GC won't collect this memory, so it's leaked.
        // Use this: new Buffer(new Uint8Array(fa.buffer))
        // fprintf(stderr, "Warning: memory leak occurred. This problem is currently unsolvable, please avoid using ArrayBuffers with GL3 binding functions.\\n");
        return ptr;
    } else if(node::Buffer::HasInstance(value)) {
        return node::Buffer::Data(value);
    } else {
        fprintf(stderr, "Invalid buffer.\\n");
        return NULL;
    }
}
"""

code_glbind_init = """
void gl3BindInit(v8::Handle<v8::Object> exports) {
    Nan::HandleScope scope;
    v8::Handle<v8::ObjectTemplate> GL3 = Nan::New<v8::ObjectTemplate>();
    GL3->SetInternalFieldCount(1);
    defineConstants(GL3);
    defineFunctions(GL3);
    defineObjects(GL3);
    Nan::Set(exports, Nan::New<v8::String>("GL3").ToLocalChecked(), GL3->NewInstance());
}
"""

def GenerateCode():
    lines = []
    lines.append('#include "glbind.h"')

    lines += [ code_helper_functions ]

    object_definitions = []
    for t in types.values():
        if getattr(t, "class_code", None) != None:
            lines.append(t.class_code())
            object_definitions.append(t.class_define_code())
    lines.append(code_define_objects % "\n".join(indent(object_definitions, "    ")))

    constants = code_define_constants % "\n".join(indent(constant_lines, "    "))
    lines.append(constants)

    lines += constant_assert_lines

    functions = "\n".join(function_codes)

    functions += code_define_functions % "\n".join(indent(functions_definitions, "    "))
    lines.append(functions)

    lines.append(code_glbind_init)

    return "\n".join(lines)
