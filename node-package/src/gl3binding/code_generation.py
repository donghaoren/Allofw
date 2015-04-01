from gltypes import types

constant_lines = []
constant_assert_lines = []
functions_definitions = []
function_codes = []

def DefineConstant(name, c_name, value, type = "Uint32"):
    constant_lines.append("""exports->Set(v8::String::NewSymbol("%s"), v8::%s::New(%s), v8::ReadOnly);""" % (name, type, c_name))
    if value == None: print name
    if type == "Uint32" and value > 0xffffffff:
        print name
    constant_assert_lines.append("""#if (%s != %d)\n #error "GL3 constant %s's value is incorrect."\n#endif""" % (c_name, value, name))

def DefineFunction(name, code):
    c_name = "EXPORT_%s" % name
    functions_definitions.append("""exports->Set(v8::String::NewSymbol("%s"), v8::FunctionTemplate::New(%s));""" % (name, c_name))
    function_codes.append("""v8::Handle<v8::Value> %s(const v8::Arguments& args) {\n%s\n}""" % (c_name, code))

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
"""

code_glbind_init = """
void gl3BindInit(v8::Handle<v8::Object> exports) {
    v8::HandleScope scope;
    v8::Handle<v8::ObjectTemplate> GL3 = v8::ObjectTemplate::New();
    GL3->SetInternalFieldCount(1);
    defineConstants(GL3);
    defineFunctions(GL3);
    defineObjects(GL3);
    exports->Set(v8::String::NewSymbol("GL3"), GL3->NewInstance());
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
