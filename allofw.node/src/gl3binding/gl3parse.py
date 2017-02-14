import yaml
import re

from code_generation import DefineConstant, GenerateCode, DefineFunction
from dts_generation import DTSDefineConstant, DTSGenerateCode, DTSDefineFunction
from gltypes import types

# Parse enums
with open("gl3prepared.h", "rb") as f:
    prepared = f.read().decode("utf-8").split("\n")
    prepared = map(lambda x: x.strip(), prepared)

def parse_arg(arg):
    arg = arg.strip()
    m = re.match(r"^const +([0-9a-zA-Z\_]+)( +([0-9a-zA-Z\_]+))?$", arg)
    if m != None:
        return (m.group(1), m.group(3), "const")
    m = re.match(r"^const +([0-9a-zA-Z\_]+) *\* *(([0-9a-zA-Z\_]+))?$", arg)
    if m != None:
        return (m.group(1), m.group(3), "const*")
    m = re.match(r"^([0-9a-zA-Z\_]+) *\* *(([0-9a-zA-Z\_]+))?$", arg)
    if m != None:
        return (m.group(1), m.group(3), "*")
    m = re.match(r"^([0-9a-zA-Z\_]+)( +([0-9a-zA-Z\_]+))?$", arg)
    if m != None:
        return (m.group(1), m.group(3), "")
    raise Exception(arg)

def parse_arg_type(arg):
    t, name, modifier = parse_arg(arg)
    if t == "void": return (None, name)
    t = types[t]
    if modifier == "const*": t = t.const_pointer()
    if modifier == "*": t = t.pointer()
    return (t, name)

for line in prepared:
    if line == "" or line[0] == '/': continue
    m = re.match(r"^#define (GL_[a-zA-Z0-9\_]+)[\ \t]+([0-9a-fA-Fx]+)$", line)
    if m != None:
        enum = m.group(1)
        value = int(m.group(2), 0)
        DefineConstant(name = enum[3:], c_name = enum, value = value, type = "Uint32")
        DTSDefineConstant(name = enum[3:], c_name = enum, value = value, type = "Uint32")
        continue

    if line[0] == '#': continue

    m = re.match(r"^ *((const *)?[0-9a-zA-Z\_]+( *\*)?) *([0-9a-zA-Z\_]+) *\((.*)\) *\;", line)
    if m != None:
        return_type, return_name = parse_arg_type(m.group(1))
        name = m.group(4)
        args = map(parse_arg_type, m.group(5).split(",")) if m.group(5) != "" else []

        code_lines = []

        code_lines.append("Nan::HandleScope scope;")

        code_lines.append("// " + repr(return_type))

        cleanup_lines = []

        for index, arg in zip(range(len(args)), args):
            argtype, argname = arg

            code_lines += argtype.define_c_variable("arg%d" % index)
            define, cleanup = argtype.assign_c_variable("info[%d]" % index, "arg%d" % index)
            code_lines += define
            cleanup_lines += cleanup

        if return_type == None:
            code_lines.append(name + "(" + ", ".join(map(lambda index: "arg%d" % index, range(len(args)))) + ");")
            code_lines += cleanup_lines
            code_lines.append("return;")
        else:
            code_lines += return_type.define_c_variable("result")
            code_lines.append("result = " + name + "(" + ", ".join(map(lambda index: "arg%d" % index, range(len(args)))) + ");")
            code_lines += return_type.convert_to_js_variable("result", "result_js")
            code_lines += cleanup_lines
            code_lines.append("return info.GetReturnValue().Set(result_js);")

        DefineFunction(name[2].lower() + name[3:], "\n".join(map(lambda x: "    " + x, code_lines)))
        DTSDefineFunction(name[2].lower() + name[3:], args, return_type)
        #print return_type, name, args
        continue

    print line

DefineFunction("shaderSource", """
    GLuint shader = Nan::ObjectWrap::Unwrap<NODE_Shader>(info[0]->ToObject())->gl_handle;
    v8::Handle<v8::Array> sources = v8::Handle<v8::Array>::Cast(info[1]);
    GLsizei count = sources->Length();
    const GLchar* *strings = new const GLchar*[count];
    GLint* lengths = new GLint[count];
    Nan::Utf8String **jstrings = new Nan::Utf8String*[count];
    for(GLsizei i = 0; i < count; i++) {
        jstrings[i] = new Nan::Utf8String(sources->Get(i));
        strings[i] = (const char*)*(*jstrings[i]);
        lengths[i] = jstrings[i]->length();
    }
    glShaderSource(shader, count, strings, lengths);
    delete [] lengths;
    delete [] strings;
    for(GLsizei i = 0; i < count; i++) {
        delete jstrings[i];
    }
    delete [] jstrings;
""")

code = GenerateCode()

with open("glbind.cpp", "wb") as f:
    f.write(code.encode("utf-8"))

code_dts = DTSGenerateCode()
code_dts = """
export type GLPointerType = number | number[] | string | ArrayBuffer;
export class Shader {
    public id(): number;
}
export class Program {
    public id(): number;
}
export class Texture {
    public id(): number;
}
export class Query {
    public id(): number;
}
export class Renderbuffer {
    public id(): number;
}
export class Framebuffer {
    public id(): number;
}
export class VertexArray {
    constructor();
}
export class Buffer {
    constructor();
}
export class Sampler {
    public id(): number;
}
export class TransformFeedback {
    public id(): number;
}
export class ProgramPipeline {
    public id(): number;
}

// Specially defined functions.
export function shaderSource(shader: Shader, source: string[]): void;
""".strip() + "\n" + code_dts
with open("glbind.d.ts", "wb") as f:
    f.write(code_dts.encode("utf-8"))
