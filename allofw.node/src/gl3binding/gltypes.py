import mustache

types = { }

class ConstPointerType:
    def __init__(self, name):
        self.name = name

    def define_c_variable(self, name):
        return ["const %s *%s;" % (self.name, name)]

    def assign_c_variable(self, js, c):
        return (map(lambda x: x.format(c = c, js = js, name = self.name, construct_name = self.name if self.name != "GLvoid" else "GLubyte"), [
            "{construct_name}* {c}_nonconst = NULL;",
            "if({js}->IsString()) {{",
            "    Nan::Utf8String {c}_utf8({js});",
            "    {c} = (const {name}*)(*{c}_utf8);",
            "}} else if({js}->IsArray()) {{",
            "    v8::Handle<v8::Array> {c}_array = v8::Handle<v8::Array>::Cast({js});",
            "    {c}_nonconst = new {construct_name}[{c}_array->Length()];",
            "    {c} = (const {name}*){c}_nonconst;",
            "}} else if({js}->IsObject()) {{",
            "    {c} = (const {name}*)glbind_get_buffer_data({js});",
            "}} else if({js}->IsNumber()) {{",
            "    {c} = (const {name}*)({js}->IntegerValue());",
            "}} else {{",
            "    {c} = NULL;",
            "}}"
        ]), map(lambda x: x.format(c = c, js = js, name = self.name), [
            "if({c}_nonconst) delete [] {c}_nonconst;"
        ]))

    def convert_to_js_variable(self, c, js):
        return []

    def dts_type(self):
        return "GLPointerType"

    def pointer(self):
        raise Exception("Invalid")

    def const_pointer(self):
        raise Exception("Invalid")

class PointerType:
    def __init__(self, name):
        self.name = name

    def define_c_variable(self, name):
        return ["%s *%s;" % (self.name, name)]

    def assign_c_variable(self, js, c):
        return (map(lambda x: x.format(c = c, js = js, name = self.name, construct_name = self.name if self.name != "GLvoid" else "GLubyte"), [
            "{construct_name}* {c}_nonconst = NULL;",
            "if({js}->IsString()) {{",
            "    Nan::Utf8String {c}_utf8({js});",
            "    {c} = ({name}*)(*{c}_utf8);",
            "}} else if({js}->IsArray()) {{",
            "    v8::Handle<v8::Array> {c}_array = v8::Handle<v8::Array>::Cast({js});",
            "    {c}_nonconst = new {construct_name}[{c}_array->Length()];",
            "    {c} = ({name}*){c}_nonconst;",
            "}} else if({js}->IsObject()) {{",
            "    {c} = ({name}*)glbind_get_buffer_data({js});",
            "}} else if({js}->IsNumber()) {{",
            "    {c} = ({name}*)({js}->IntegerValue());",
            "}} else {{",
            "    {c} = NULL;",
            "}}"
        ]), map(lambda x: x.format(c = c, js = js, name = self.name), [
            "if({c}_nonconst) delete [] {c}_nonconst;"
        ]))

    def dts_type(self):
        return "GLPointerType"

    def convert_to_js_variable(self, c, js):
        return ["v8::Handle<v8::Value> %s = Nan::NewBuffer((char*)%s, 0, do_nothing_release_callback, NULL).ToLocalChecked();" % (js, c)]

    def pointer(self):
        raise Exception("Invalid")

    def const_pointer(self):
        raise Exception("Invalid")

class Type:
    def __init__(self, name, c2js, js2c):
        self.name = name
        self.c2js = c2js
        self.js2c = js2c

    def define_c_variable(self, name):
        return ["%s %s;" % (self.name, name)]

    def assign_c_variable(self, js, c):
        return (["%s = %s;" % (c, self.js2c.replace("value", js)) ],
                [])

    def convert_to_js_variable(self, c, js):
        return ["v8::Handle<v8::Value> %s = " % js + self.c2js.replace("value", c) + ";"]

    def release_c_variable(self, name):
        return []

    def dts_type(self):
        return "number"

    def pointer(self):
        return PointerType(self.name)

    def const_pointer(self):
        return ConstPointerType(self.name)

def typedef(name, c2js, js2c):
    types[name] = Type(name, c2js, js2c)

def typedef_s32(name):
    typedef(name, "Nan::New<v8::Int32>(value)", "value->Int32Value()")

def typedef_u32(name):
    typedef(name, "Nan::New<v8::Uint32>(value)", "value->Uint32Value()")

def typedef_s64(name):
    typedef(name, "Nan::New<v8::IntegerValue>(value)", "value->IntegerValue()")

def typedef_u64(name):
    typedef(name, "Nan::New<v8::IntegerValue>(value)", "(uint64_t)value->IntegerValue()")

def typedef_f32(name):
    typedef(name, "Nan::New<v8::NumberValue>((double)value)", "(float)value->NumberValue()")

def typedef_f64(name):
    typedef(name, "Nan::New<v8::NumberValue>(value)", "value->NumberValue()")

typedef("GLvoid", "", "")
typedef_u32("GLbitfield")
typedef_u32("GLboolean")
typedef_s32("GLbyte")
typedef_f32("GLclampf")
typedef_u32("GLenum")
typedef_f32("GLfloat")
typedef_s32("GLint")
typedef_s32("GLshort")
typedef_s32("GLsizei")
typedef_u32("GLubyte")
typedef_u32("GLuint")
typedef_u32("GLushort")
typedef_s32("GLchar")
typedef_f64("GLdouble")
typedef_f64("GLclampd")
typedef_s32("GLfixed")

typedef_s64("GLint64")
typedef_u64("GLuint64")

typedef_s64("GLintptr")
typedef_u64("GLsizeiptr")

class UStringType:
    def define_c_variable(self, name):
        return ["const GLubyte* %s;" % name]

    def assign_c_variable(self, js, c):
        return ([
            "Nan::Utf8String %s_utf8(%s);" % (c, js),
            "%s = (GLubyte*)*%s_utf8;" % (c, c)
        ], [])

    def convert_to_js_variable(self, c, js):
        return [
            "v8::Handle<v8::Value> %s;" % js,
            "if(%s) {" % c,
            "    %s = Nan::New<v8::String>((const char*)%s, strlen((const char*)%s)).ToLocalChecked();" % (js, c, c),
            "} else {",
            "    %s = Nan::Undefined();" % js,
            "}"
        ]

    def dts_type(self):
        return "string"

    def pointer(self):
        raise Exception("Invalid")

    def const_pointer(self):
        raise Exception("Invalid")

class CStringType:
    def define_c_variable(self, name):
        return ["const GLchar* %s;" % name]

    def assign_c_variable(self, js, c):
        return ([
            "Nan::Utf8String %s_utf8(%s);" % (c, js),
            "%s = (GLchar*)*%s_utf8;" % (c, c)
        ], [])

    def convert_to_js_variable(self, c, js):
        return ["v8::Handle<v8::Value> %s = Nan::New<v8::String>((const char*)%s, strlen((const char*)%s)).ToLocalChecked();" % (js, c, c)]

    def dts_type(self):
        return "string"

    def pointer(self):
        raise Exception("Invalid")

    def const_pointer(self):
        raise Exception("Invalid")

class ClassTypeOutputArray:
    def __init__(self, classtype):
        self.classtype = classtype
    def define_c_variable(self, name):
        return ["GLuint* %s;" % name]

    def assign_c_variable(self, js, c):
        jsr = js.replace("[", "_").replace("]", "")
        assign = [
            "v8::Handle<v8::Array> {jsr}_array = v8::Handle<v8::Array>::Cast({js});",
            "{c} = new GLuint[{jsr}_array->Length()];"
        ]
        free = [
            "for(uint32_t i = 0; i < {jsr}_array->Length(); i++) {{",
            "    {jsr}_array->Set(i, NODE_{classname}::fromGLHandle({c}[i]));",
            "}}",
            "delete [] {c};"
        ]
        return (map(lambda x: x.format(c = c, js = js, jsr = jsr, classname = self.classtype.name), assign),
                map(lambda x: x.format(c = c, js = js, jsr = jsr, classname = self.classtype.name), free))

    def convert_to_js_variable(self, c, js):
        return []

    def dts_type(self):
        return "number[]"

class ClassTypeInputArray:
    def __init__(self, classtype):
        self.classtype = classtype
    def define_c_variable(self, name):
        return ["GLuint* %s;" % name]

    def assign_c_variable(self, js, c):
        jsr = js.replace("[", "_").replace("]", "")
        assign = [
            "v8::Handle<v8::Array> {jsr}_array = v8::Handle<v8::Array>::Cast({js});",
            "{c} = new GLuint[{jsr}_array->Length()];",
            "for(uint32_t i = 0; i < {jsr}_array->Length(); i++) {{",
            "    {c}[i] = Nan::ObjectWrap::Unwrap<NODE_{classname}>({jsr}_array->Get(i)->ToObject())->gl_handle;"
            "}}",
        ]
        free = [
            "delete [] {c};"
        ]
        return (map(lambda x: x.format(c = c, js = js, jsr = jsr, classname = self.classtype.name), assign),
                map(lambda x: x.format(c = c, js = js, jsr = jsr, classname = self.classtype.name), free))

    def convert_to_js_variable(self, c, js):
        return []

    def dts_type(self):
        return "number[]"

class ClassType:
    def __init__(self, name, release_line, construct_line = "%s = 0;"):
        self.name = name
        self.release_line = release_line
        self.construct_line = construct_line

    def define_c_variable(self, name):
        return ["GLuint %s;" % (name)]

    def assign_c_variable(self, js, c):
        return ([
            "if({js}->IsNumber()) {{".format(c = c, js = js, classname = self.name),
            "    {c} = {js}->IntegerValue();".format(c = c, js = js, classname = self.name),
            "} else {",
            "    {c} = Nan::ObjectWrap::Unwrap<NODE_{classname}>({js}->ToObject())->gl_handle;".format(c = c, js = js, classname = self.name),
            "}"
        ], [])

    def convert_to_js_variable(self, c, js):
        return ["v8::Handle<v8::Value> {js} = NODE_{classname}::fromGLHandle({c});".format(classname = self.name, c = c, js = js)]

    def class_define_code(self):
        return "NODE_{classname}::Init(exports);".format(classname = self.name)

    def class_code(self):
        c = """
        class NODE_{{classname}} : public Nan::ObjectWrap {
        public:
            static void Init(v8::Handle<v8::Object> exports);
            GLuint gl_handle;

            static v8::Handle<v8::Value> fromGLHandle(GLuint handle) {
                const int argc = 1;
                v8::Local<v8::Value> argv[argc] = { Nan::New<v8::Integer>(handle) };
                return Nan::New(constructor)->NewInstance(argc, argv);
            }

        private:
            explicit NODE_{{classname}}(GLuint gl_handle_) { gl_handle = gl_handle_; }
            explicit NODE_{{classname}}() { {{construct_gl_object}} }
            ~NODE_{{classname}}() { {{release_code}} }

            static NAN_METHOD(New);

            static NAN_METHOD(NODE_id);
            static NAN_METHOD(NODE_delete);
            static NAN_METHOD(NODE_toString);

            static Nan::Persistent<v8::Function> constructor;
        };

        Nan::Persistent<v8::Function> NODE_{{classname}}::constructor;

        void NODE_{{classname}}::Init(v8::Handle<v8::Object> exports) {
            v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
            tpl->SetClassName(Nan::New<v8::String>("{{classname}}").ToLocalChecked());
            tpl->InstanceTemplate()->SetInternalFieldCount(1);

            // Prototype
            Nan::SetPrototypeMethod(tpl, "id", NODE_id);
            Nan::SetPrototypeMethod(tpl, "toString", NODE_toString);
            Nan::SetPrototypeMethod(tpl, "inspect", NODE_toString);

            Nan::SetPrototypeMethod(tpl, "delete", NODE_delete);

            constructor.Reset(tpl->GetFunction());

            // Export constructor.
            Nan::Set(exports, Nan::New<v8::String>("{{classname}}").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
        }

        NAN_METHOD(NODE_{{classname}}::New) {
            Nan::HandleScope scope;

            if(info.IsConstructCall()) {
                if(info.Length() == 0) {
                    NODE_{{classname}}* obj = new NODE_{{classname}}();
                    obj->Wrap(info.This());
                    info.GetReturnValue().Set(info.This());
                } else {
                    NODE_{{classname}}* obj = new NODE_{{classname}}(info[0]->Uint32Value());
                    obj->Wrap(info.This());
                    info.GetReturnValue().Set(info.This());
                }
            } else {
                // Invoked as plain function `MyObject(...)`, turn into construct call.
                const int argc = 1;
                v8::Local<v8::Value> argv[argc] = { info[0] };
                info.GetReturnValue().Set(Nan::New(constructor)->NewInstance(argc, argv));
            }
        }

        NAN_METHOD(NODE_{{classname}}::NODE_id) {
            NODE_{{classname}}* obj = Nan::ObjectWrap::Unwrap<NODE_{{classname}}>(info.This());
            info.GetReturnValue().Set(Nan::New<v8::Uint32>(obj->gl_handle));
        }

        NAN_METHOD(NODE_{{classname}}::NODE_delete) {
            NODE_{{classname}}* obj = Nan::ObjectWrap::Unwrap<NODE_{{classname}}>(info.This());
            GLuint gl_handle = obj->gl_handle;
            {{release_code}}
            obj->gl_handle = 0;
        }

        NAN_METHOD(NODE_{{classname}}::NODE_toString) {
            NODE_{{classname}}* obj = Nan::ObjectWrap::Unwrap<NODE_{{classname}}>(info.This());
            char buf[64];
            sprintf(buf, "{{classname}}:%d", obj->gl_handle);
            info.GetReturnValue().Set(Nan::New<v8::String>(buf).ToLocalChecked());
        }
        """
        items = {
            "classname": self.name,
            "release_code": self.release_line % "gl_handle",
            "construct_gl_object": self.construct_line % "gl_handle"
        }
        for key, value in items.items():
            c = c.replace("{{%s}}" % key, value)
        return c.replace("\n        ", "\n")

    def pointer(self):
        return ClassTypeOutputArray(self)

    def const_pointer(self):
        return ClassTypeInputArray(self)

    def dts_type(self):
        return self.name + " | number"

types["W_String"] = CStringType()
types["W_UString"] = UStringType()
types["W_Texture"] = ClassType("Texture", "glDeleteTextures(1, &%s);", "glGenTextures(1, &%s);")
types["W_Buffer"] = ClassType("Buffer", "glDeleteBuffers(1, &%s);", "glGenBuffers(1, &%s);")
types["W_Query"] = ClassType("Query", "glDeleteQueries(1, &%s);", "glGenQueries(1, &%s);")
types["W_Program"] = ClassType("Program", "glDeleteProgram(%s);")
types["W_Shader"] = ClassType("Shader", "glDeleteShader(%s);")
types["W_VertexArray"] = ClassType("VertexArray", "glDeleteVertexArrays(1, &%s);", "glGenVertexArrays(1, &%s);")
types["W_Renderbuffer"] = ClassType("Renderbuffer", "glDeleteRenderbuffers(1, &%s);", "glGenRenderbuffers(1, &%s);")
types["W_Framebuffer"] = ClassType("Framebuffer", "glDeleteFramebuffers(1, &%s);", "glGenFramebuffers(1, &%s);")
types["W_Sampler"] = ClassType("Sampler", "glDeleteSamplers(1, &%s);", "glGenSamplers(1, &%s);")
types["W_TransformFeedback"] = ClassType("TransformFeedback", "glDeleteTransformFeedbacks(1, &%s);", "glGenTransformFeedbacks(1, &%s);")
types["W_ProgramPipeline"] = ClassType("ProgramPipeline", "glDeleteProgramPipelines(1, &%s);", "glGenProgramPipelines(1, &%s);")
