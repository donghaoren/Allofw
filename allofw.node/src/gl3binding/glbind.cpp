#include "glbind.h"

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
        // fprintf(stderr, "Warning: memory leak occurred. This problem is currently unsolvable, please avoid using ArrayBuffers with GL3 binding functions.\n");
        return ptr;
    } else if(node::Buffer::HasInstance(value)) {
        return node::Buffer::Data(value);
    } else {
        fprintf(stderr, "Invalid buffer.\n");
        return NULL;
    }
}


class NODE_Sampler : public Nan::ObjectWrap {
public:
    static void Init(v8::Handle<v8::ObjectTemplate> exports);
    GLuint gl_handle;

    static v8::Handle<v8::Value> fromGLHandle(GLuint handle) {
        const int argc = 1;
        v8::Local<v8::Value> argv[argc] = { Nan::New<v8::Integer>(handle) };
        return Nan::New(constructor)->NewInstance(argc, argv);
    }

private:
    explicit NODE_Sampler(GLuint gl_handle_) { gl_handle = gl_handle_; }
    explicit NODE_Sampler() { glGenSamplers(1, &gl_handle); }
    ~NODE_Sampler() { glDeleteSamplers(1, &gl_handle); }

    static NAN_METHOD(New);

    static NAN_METHOD(NODE_id);
    static NAN_METHOD(NODE_delete);
    static NAN_METHOD(NODE_toString);

    static Nan::Persistent<v8::Function> constructor;
};

Nan::Persistent<v8::Function> NODE_Sampler::constructor;

void NODE_Sampler::Init(v8::Handle<v8::ObjectTemplate> exports) {
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->SetClassName(Nan::New<v8::String>("Sampler").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    Nan::SetPrototypeMethod(tpl, "id", NODE_id);
    Nan::SetPrototypeMethod(tpl, "toString", NODE_toString);
    Nan::SetPrototypeMethod(tpl, "inspect", NODE_toString);

    Nan::SetPrototypeMethod(tpl, "delete", NODE_delete);

    constructor.Reset(tpl->GetFunction());

    // Export constructor.
    Nan::SetTemplate(exports, "Sampler", Nan::GetFunction(tpl).ToLocalChecked());
}

NAN_METHOD(NODE_Sampler::New) {
    Nan::HandleScope scope;

    if(info.IsConstructCall()) {
        if(info.Length() == 0) {
            NODE_Sampler* obj = new NODE_Sampler();
            obj->Wrap(info.This());
            info.GetReturnValue().Set(info.This());
        } else {
            NODE_Sampler* obj = new NODE_Sampler(info[0]->Uint32Value());
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

NAN_METHOD(NODE_Sampler::NODE_id) {
    NODE_Sampler* obj = Nan::ObjectWrap::Unwrap<NODE_Sampler>(info.This());
    info.GetReturnValue().Set(Nan::New<v8::Uint32>(obj->gl_handle));
}

NAN_METHOD(NODE_Sampler::NODE_delete) {
    NODE_Sampler* obj = Nan::ObjectWrap::Unwrap<NODE_Sampler>(info.This());
    GLuint gl_handle = obj->gl_handle;
    glDeleteSamplers(1, &gl_handle);
    obj->gl_handle = 0;
}

NAN_METHOD(NODE_Sampler::NODE_toString) {
    NODE_Sampler* obj = Nan::ObjectWrap::Unwrap<NODE_Sampler>(info.This());
    char buf[64];
    sprintf(buf, "Sampler:%d", obj->gl_handle);
    info.GetReturnValue().Set(Nan::New<v8::String>(buf).ToLocalChecked());
}


class NODE_Framebuffer : public Nan::ObjectWrap {
public:
    static void Init(v8::Handle<v8::ObjectTemplate> exports);
    GLuint gl_handle;

    static v8::Handle<v8::Value> fromGLHandle(GLuint handle) {
        const int argc = 1;
        v8::Local<v8::Value> argv[argc] = { Nan::New<v8::Integer>(handle) };
        return Nan::New(constructor)->NewInstance(argc, argv);
    }

private:
    explicit NODE_Framebuffer(GLuint gl_handle_) { gl_handle = gl_handle_; }
    explicit NODE_Framebuffer() { glGenFramebuffers(1, &gl_handle); }
    ~NODE_Framebuffer() { glDeleteFramebuffers(1, &gl_handle); }

    static NAN_METHOD(New);

    static NAN_METHOD(NODE_id);
    static NAN_METHOD(NODE_delete);
    static NAN_METHOD(NODE_toString);

    static Nan::Persistent<v8::Function> constructor;
};

Nan::Persistent<v8::Function> NODE_Framebuffer::constructor;

void NODE_Framebuffer::Init(v8::Handle<v8::ObjectTemplate> exports) {
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->SetClassName(Nan::New<v8::String>("Framebuffer").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    Nan::SetPrototypeMethod(tpl, "id", NODE_id);
    Nan::SetPrototypeMethod(tpl, "toString", NODE_toString);
    Nan::SetPrototypeMethod(tpl, "inspect", NODE_toString);

    Nan::SetPrototypeMethod(tpl, "delete", NODE_delete);

    constructor.Reset(tpl->GetFunction());

    // Export constructor.
    Nan::SetTemplate(exports, "Framebuffer", Nan::GetFunction(tpl).ToLocalChecked());
}

NAN_METHOD(NODE_Framebuffer::New) {
    Nan::HandleScope scope;

    if(info.IsConstructCall()) {
        if(info.Length() == 0) {
            NODE_Framebuffer* obj = new NODE_Framebuffer();
            obj->Wrap(info.This());
            info.GetReturnValue().Set(info.This());
        } else {
            NODE_Framebuffer* obj = new NODE_Framebuffer(info[0]->Uint32Value());
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

NAN_METHOD(NODE_Framebuffer::NODE_id) {
    NODE_Framebuffer* obj = Nan::ObjectWrap::Unwrap<NODE_Framebuffer>(info.This());
    info.GetReturnValue().Set(Nan::New<v8::Uint32>(obj->gl_handle));
}

NAN_METHOD(NODE_Framebuffer::NODE_delete) {
    NODE_Framebuffer* obj = Nan::ObjectWrap::Unwrap<NODE_Framebuffer>(info.This());
    GLuint gl_handle = obj->gl_handle;
    glDeleteFramebuffers(1, &gl_handle);
    obj->gl_handle = 0;
}

NAN_METHOD(NODE_Framebuffer::NODE_toString) {
    NODE_Framebuffer* obj = Nan::ObjectWrap::Unwrap<NODE_Framebuffer>(info.This());
    char buf[64];
    sprintf(buf, "Framebuffer:%d", obj->gl_handle);
    info.GetReturnValue().Set(Nan::New<v8::String>(buf).ToLocalChecked());
}


class NODE_Shader : public Nan::ObjectWrap {
public:
    static void Init(v8::Handle<v8::ObjectTemplate> exports);
    GLuint gl_handle;

    static v8::Handle<v8::Value> fromGLHandle(GLuint handle) {
        const int argc = 1;
        v8::Local<v8::Value> argv[argc] = { Nan::New<v8::Integer>(handle) };
        return Nan::New(constructor)->NewInstance(argc, argv);
    }

private:
    explicit NODE_Shader(GLuint gl_handle_) { gl_handle = gl_handle_; }
    explicit NODE_Shader() { gl_handle = 0; }
    ~NODE_Shader() { glDeleteShader(gl_handle); }

    static NAN_METHOD(New);

    static NAN_METHOD(NODE_id);
    static NAN_METHOD(NODE_delete);
    static NAN_METHOD(NODE_toString);

    static Nan::Persistent<v8::Function> constructor;
};

Nan::Persistent<v8::Function> NODE_Shader::constructor;

void NODE_Shader::Init(v8::Handle<v8::ObjectTemplate> exports) {
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->SetClassName(Nan::New<v8::String>("Shader").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    Nan::SetPrototypeMethod(tpl, "id", NODE_id);
    Nan::SetPrototypeMethod(tpl, "toString", NODE_toString);
    Nan::SetPrototypeMethod(tpl, "inspect", NODE_toString);

    Nan::SetPrototypeMethod(tpl, "delete", NODE_delete);

    constructor.Reset(tpl->GetFunction());

    // Export constructor.
    Nan::SetTemplate(exports, "Shader", Nan::GetFunction(tpl).ToLocalChecked());
}

NAN_METHOD(NODE_Shader::New) {
    Nan::HandleScope scope;

    if(info.IsConstructCall()) {
        if(info.Length() == 0) {
            NODE_Shader* obj = new NODE_Shader();
            obj->Wrap(info.This());
            info.GetReturnValue().Set(info.This());
        } else {
            NODE_Shader* obj = new NODE_Shader(info[0]->Uint32Value());
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

NAN_METHOD(NODE_Shader::NODE_id) {
    NODE_Shader* obj = Nan::ObjectWrap::Unwrap<NODE_Shader>(info.This());
    info.GetReturnValue().Set(Nan::New<v8::Uint32>(obj->gl_handle));
}

NAN_METHOD(NODE_Shader::NODE_delete) {
    NODE_Shader* obj = Nan::ObjectWrap::Unwrap<NODE_Shader>(info.This());
    GLuint gl_handle = obj->gl_handle;
    glDeleteShader(gl_handle);
    obj->gl_handle = 0;
}

NAN_METHOD(NODE_Shader::NODE_toString) {
    NODE_Shader* obj = Nan::ObjectWrap::Unwrap<NODE_Shader>(info.This());
    char buf[64];
    sprintf(buf, "Shader:%d", obj->gl_handle);
    info.GetReturnValue().Set(Nan::New<v8::String>(buf).ToLocalChecked());
}


class NODE_Texture : public Nan::ObjectWrap {
public:
    static void Init(v8::Handle<v8::ObjectTemplate> exports);
    GLuint gl_handle;

    static v8::Handle<v8::Value> fromGLHandle(GLuint handle) {
        const int argc = 1;
        v8::Local<v8::Value> argv[argc] = { Nan::New<v8::Integer>(handle) };
        return Nan::New(constructor)->NewInstance(argc, argv);
    }

private:
    explicit NODE_Texture(GLuint gl_handle_) { gl_handle = gl_handle_; }
    explicit NODE_Texture() { glGenTextures(1, &gl_handle); }
    ~NODE_Texture() { glDeleteTextures(1, &gl_handle); }

    static NAN_METHOD(New);

    static NAN_METHOD(NODE_id);
    static NAN_METHOD(NODE_delete);
    static NAN_METHOD(NODE_toString);

    static Nan::Persistent<v8::Function> constructor;
};

Nan::Persistent<v8::Function> NODE_Texture::constructor;

void NODE_Texture::Init(v8::Handle<v8::ObjectTemplate> exports) {
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->SetClassName(Nan::New<v8::String>("Texture").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    Nan::SetPrototypeMethod(tpl, "id", NODE_id);
    Nan::SetPrototypeMethod(tpl, "toString", NODE_toString);
    Nan::SetPrototypeMethod(tpl, "inspect", NODE_toString);

    Nan::SetPrototypeMethod(tpl, "delete", NODE_delete);

    constructor.Reset(tpl->GetFunction());

    // Export constructor.
    Nan::SetTemplate(exports, "Texture", Nan::GetFunction(tpl).ToLocalChecked());
}

NAN_METHOD(NODE_Texture::New) {
    Nan::HandleScope scope;

    if(info.IsConstructCall()) {
        if(info.Length() == 0) {
            NODE_Texture* obj = new NODE_Texture();
            obj->Wrap(info.This());
            info.GetReturnValue().Set(info.This());
        } else {
            NODE_Texture* obj = new NODE_Texture(info[0]->Uint32Value());
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

NAN_METHOD(NODE_Texture::NODE_id) {
    NODE_Texture* obj = Nan::ObjectWrap::Unwrap<NODE_Texture>(info.This());
    info.GetReturnValue().Set(Nan::New<v8::Uint32>(obj->gl_handle));
}

NAN_METHOD(NODE_Texture::NODE_delete) {
    NODE_Texture* obj = Nan::ObjectWrap::Unwrap<NODE_Texture>(info.This());
    GLuint gl_handle = obj->gl_handle;
    glDeleteTextures(1, &gl_handle);
    obj->gl_handle = 0;
}

NAN_METHOD(NODE_Texture::NODE_toString) {
    NODE_Texture* obj = Nan::ObjectWrap::Unwrap<NODE_Texture>(info.This());
    char buf[64];
    sprintf(buf, "Texture:%d", obj->gl_handle);
    info.GetReturnValue().Set(Nan::New<v8::String>(buf).ToLocalChecked());
}


class NODE_ProgramPipeline : public Nan::ObjectWrap {
public:
    static void Init(v8::Handle<v8::ObjectTemplate> exports);
    GLuint gl_handle;

    static v8::Handle<v8::Value> fromGLHandle(GLuint handle) {
        const int argc = 1;
        v8::Local<v8::Value> argv[argc] = { Nan::New<v8::Integer>(handle) };
        return Nan::New(constructor)->NewInstance(argc, argv);
    }

private:
    explicit NODE_ProgramPipeline(GLuint gl_handle_) { gl_handle = gl_handle_; }
    explicit NODE_ProgramPipeline() { glGenProgramPipelines(1, &gl_handle); }
    ~NODE_ProgramPipeline() { glDeleteProgramPipelines(1, &gl_handle); }

    static NAN_METHOD(New);

    static NAN_METHOD(NODE_id);
    static NAN_METHOD(NODE_delete);
    static NAN_METHOD(NODE_toString);

    static Nan::Persistent<v8::Function> constructor;
};

Nan::Persistent<v8::Function> NODE_ProgramPipeline::constructor;

void NODE_ProgramPipeline::Init(v8::Handle<v8::ObjectTemplate> exports) {
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->SetClassName(Nan::New<v8::String>("ProgramPipeline").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    Nan::SetPrototypeMethod(tpl, "id", NODE_id);
    Nan::SetPrototypeMethod(tpl, "toString", NODE_toString);
    Nan::SetPrototypeMethod(tpl, "inspect", NODE_toString);

    Nan::SetPrototypeMethod(tpl, "delete", NODE_delete);

    constructor.Reset(tpl->GetFunction());

    // Export constructor.
    Nan::SetTemplate(exports, "ProgramPipeline", Nan::GetFunction(tpl).ToLocalChecked());
}

NAN_METHOD(NODE_ProgramPipeline::New) {
    Nan::HandleScope scope;

    if(info.IsConstructCall()) {
        if(info.Length() == 0) {
            NODE_ProgramPipeline* obj = new NODE_ProgramPipeline();
            obj->Wrap(info.This());
            info.GetReturnValue().Set(info.This());
        } else {
            NODE_ProgramPipeline* obj = new NODE_ProgramPipeline(info[0]->Uint32Value());
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

NAN_METHOD(NODE_ProgramPipeline::NODE_id) {
    NODE_ProgramPipeline* obj = Nan::ObjectWrap::Unwrap<NODE_ProgramPipeline>(info.This());
    info.GetReturnValue().Set(Nan::New<v8::Uint32>(obj->gl_handle));
}

NAN_METHOD(NODE_ProgramPipeline::NODE_delete) {
    NODE_ProgramPipeline* obj = Nan::ObjectWrap::Unwrap<NODE_ProgramPipeline>(info.This());
    GLuint gl_handle = obj->gl_handle;
    glDeleteProgramPipelines(1, &gl_handle);
    obj->gl_handle = 0;
}

NAN_METHOD(NODE_ProgramPipeline::NODE_toString) {
    NODE_ProgramPipeline* obj = Nan::ObjectWrap::Unwrap<NODE_ProgramPipeline>(info.This());
    char buf[64];
    sprintf(buf, "ProgramPipeline:%d", obj->gl_handle);
    info.GetReturnValue().Set(Nan::New<v8::String>(buf).ToLocalChecked());
}


class NODE_Renderbuffer : public Nan::ObjectWrap {
public:
    static void Init(v8::Handle<v8::ObjectTemplate> exports);
    GLuint gl_handle;

    static v8::Handle<v8::Value> fromGLHandle(GLuint handle) {
        const int argc = 1;
        v8::Local<v8::Value> argv[argc] = { Nan::New<v8::Integer>(handle) };
        return Nan::New(constructor)->NewInstance(argc, argv);
    }

private:
    explicit NODE_Renderbuffer(GLuint gl_handle_) { gl_handle = gl_handle_; }
    explicit NODE_Renderbuffer() { glGenRenderbuffers(1, &gl_handle); }
    ~NODE_Renderbuffer() { glDeleteRenderbuffers(1, &gl_handle); }

    static NAN_METHOD(New);

    static NAN_METHOD(NODE_id);
    static NAN_METHOD(NODE_delete);
    static NAN_METHOD(NODE_toString);

    static Nan::Persistent<v8::Function> constructor;
};

Nan::Persistent<v8::Function> NODE_Renderbuffer::constructor;

void NODE_Renderbuffer::Init(v8::Handle<v8::ObjectTemplate> exports) {
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->SetClassName(Nan::New<v8::String>("Renderbuffer").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    Nan::SetPrototypeMethod(tpl, "id", NODE_id);
    Nan::SetPrototypeMethod(tpl, "toString", NODE_toString);
    Nan::SetPrototypeMethod(tpl, "inspect", NODE_toString);

    Nan::SetPrototypeMethod(tpl, "delete", NODE_delete);

    constructor.Reset(tpl->GetFunction());

    // Export constructor.
    Nan::SetTemplate(exports, "Renderbuffer", Nan::GetFunction(tpl).ToLocalChecked());
}

NAN_METHOD(NODE_Renderbuffer::New) {
    Nan::HandleScope scope;

    if(info.IsConstructCall()) {
        if(info.Length() == 0) {
            NODE_Renderbuffer* obj = new NODE_Renderbuffer();
            obj->Wrap(info.This());
            info.GetReturnValue().Set(info.This());
        } else {
            NODE_Renderbuffer* obj = new NODE_Renderbuffer(info[0]->Uint32Value());
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

NAN_METHOD(NODE_Renderbuffer::NODE_id) {
    NODE_Renderbuffer* obj = Nan::ObjectWrap::Unwrap<NODE_Renderbuffer>(info.This());
    info.GetReturnValue().Set(Nan::New<v8::Uint32>(obj->gl_handle));
}

NAN_METHOD(NODE_Renderbuffer::NODE_delete) {
    NODE_Renderbuffer* obj = Nan::ObjectWrap::Unwrap<NODE_Renderbuffer>(info.This());
    GLuint gl_handle = obj->gl_handle;
    glDeleteRenderbuffers(1, &gl_handle);
    obj->gl_handle = 0;
}

NAN_METHOD(NODE_Renderbuffer::NODE_toString) {
    NODE_Renderbuffer* obj = Nan::ObjectWrap::Unwrap<NODE_Renderbuffer>(info.This());
    char buf[64];
    sprintf(buf, "Renderbuffer:%d", obj->gl_handle);
    info.GetReturnValue().Set(Nan::New<v8::String>(buf).ToLocalChecked());
}


class NODE_VertexArray : public Nan::ObjectWrap {
public:
    static void Init(v8::Handle<v8::ObjectTemplate> exports);
    GLuint gl_handle;

    static v8::Handle<v8::Value> fromGLHandle(GLuint handle) {
        const int argc = 1;
        v8::Local<v8::Value> argv[argc] = { Nan::New<v8::Integer>(handle) };
        return Nan::New(constructor)->NewInstance(argc, argv);
    }

private:
    explicit NODE_VertexArray(GLuint gl_handle_) { gl_handle = gl_handle_; }
    explicit NODE_VertexArray() { glGenVertexArrays(1, &gl_handle); }
    ~NODE_VertexArray() { glDeleteVertexArrays(1, &gl_handle); }

    static NAN_METHOD(New);

    static NAN_METHOD(NODE_id);
    static NAN_METHOD(NODE_delete);
    static NAN_METHOD(NODE_toString);

    static Nan::Persistent<v8::Function> constructor;
};

Nan::Persistent<v8::Function> NODE_VertexArray::constructor;

void NODE_VertexArray::Init(v8::Handle<v8::ObjectTemplate> exports) {
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->SetClassName(Nan::New<v8::String>("VertexArray").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    Nan::SetPrototypeMethod(tpl, "id", NODE_id);
    Nan::SetPrototypeMethod(tpl, "toString", NODE_toString);
    Nan::SetPrototypeMethod(tpl, "inspect", NODE_toString);

    Nan::SetPrototypeMethod(tpl, "delete", NODE_delete);

    constructor.Reset(tpl->GetFunction());

    // Export constructor.
    Nan::SetTemplate(exports, "VertexArray", Nan::GetFunction(tpl).ToLocalChecked());
}

NAN_METHOD(NODE_VertexArray::New) {
    Nan::HandleScope scope;

    if(info.IsConstructCall()) {
        if(info.Length() == 0) {
            NODE_VertexArray* obj = new NODE_VertexArray();
            obj->Wrap(info.This());
            info.GetReturnValue().Set(info.This());
        } else {
            NODE_VertexArray* obj = new NODE_VertexArray(info[0]->Uint32Value());
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

NAN_METHOD(NODE_VertexArray::NODE_id) {
    NODE_VertexArray* obj = Nan::ObjectWrap::Unwrap<NODE_VertexArray>(info.This());
    info.GetReturnValue().Set(Nan::New<v8::Uint32>(obj->gl_handle));
}

NAN_METHOD(NODE_VertexArray::NODE_delete) {
    NODE_VertexArray* obj = Nan::ObjectWrap::Unwrap<NODE_VertexArray>(info.This());
    GLuint gl_handle = obj->gl_handle;
    glDeleteVertexArrays(1, &gl_handle);
    obj->gl_handle = 0;
}

NAN_METHOD(NODE_VertexArray::NODE_toString) {
    NODE_VertexArray* obj = Nan::ObjectWrap::Unwrap<NODE_VertexArray>(info.This());
    char buf[64];
    sprintf(buf, "VertexArray:%d", obj->gl_handle);
    info.GetReturnValue().Set(Nan::New<v8::String>(buf).ToLocalChecked());
}


class NODE_Query : public Nan::ObjectWrap {
public:
    static void Init(v8::Handle<v8::ObjectTemplate> exports);
    GLuint gl_handle;

    static v8::Handle<v8::Value> fromGLHandle(GLuint handle) {
        const int argc = 1;
        v8::Local<v8::Value> argv[argc] = { Nan::New<v8::Integer>(handle) };
        return Nan::New(constructor)->NewInstance(argc, argv);
    }

private:
    explicit NODE_Query(GLuint gl_handle_) { gl_handle = gl_handle_; }
    explicit NODE_Query() { glGenQueries(1, &gl_handle); }
    ~NODE_Query() { glDeleteQueries(1, &gl_handle); }

    static NAN_METHOD(New);

    static NAN_METHOD(NODE_id);
    static NAN_METHOD(NODE_delete);
    static NAN_METHOD(NODE_toString);

    static Nan::Persistent<v8::Function> constructor;
};

Nan::Persistent<v8::Function> NODE_Query::constructor;

void NODE_Query::Init(v8::Handle<v8::ObjectTemplate> exports) {
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->SetClassName(Nan::New<v8::String>("Query").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    Nan::SetPrototypeMethod(tpl, "id", NODE_id);
    Nan::SetPrototypeMethod(tpl, "toString", NODE_toString);
    Nan::SetPrototypeMethod(tpl, "inspect", NODE_toString);

    Nan::SetPrototypeMethod(tpl, "delete", NODE_delete);

    constructor.Reset(tpl->GetFunction());

    // Export constructor.
    Nan::SetTemplate(exports, "Query", Nan::GetFunction(tpl).ToLocalChecked());
}

NAN_METHOD(NODE_Query::New) {
    Nan::HandleScope scope;

    if(info.IsConstructCall()) {
        if(info.Length() == 0) {
            NODE_Query* obj = new NODE_Query();
            obj->Wrap(info.This());
            info.GetReturnValue().Set(info.This());
        } else {
            NODE_Query* obj = new NODE_Query(info[0]->Uint32Value());
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

NAN_METHOD(NODE_Query::NODE_id) {
    NODE_Query* obj = Nan::ObjectWrap::Unwrap<NODE_Query>(info.This());
    info.GetReturnValue().Set(Nan::New<v8::Uint32>(obj->gl_handle));
}

NAN_METHOD(NODE_Query::NODE_delete) {
    NODE_Query* obj = Nan::ObjectWrap::Unwrap<NODE_Query>(info.This());
    GLuint gl_handle = obj->gl_handle;
    glDeleteQueries(1, &gl_handle);
    obj->gl_handle = 0;
}

NAN_METHOD(NODE_Query::NODE_toString) {
    NODE_Query* obj = Nan::ObjectWrap::Unwrap<NODE_Query>(info.This());
    char buf[64];
    sprintf(buf, "Query:%d", obj->gl_handle);
    info.GetReturnValue().Set(Nan::New<v8::String>(buf).ToLocalChecked());
}


class NODE_Buffer : public Nan::ObjectWrap {
public:
    static void Init(v8::Handle<v8::ObjectTemplate> exports);
    GLuint gl_handle;

    static v8::Handle<v8::Value> fromGLHandle(GLuint handle) {
        const int argc = 1;
        v8::Local<v8::Value> argv[argc] = { Nan::New<v8::Integer>(handle) };
        return Nan::New(constructor)->NewInstance(argc, argv);
    }

private:
    explicit NODE_Buffer(GLuint gl_handle_) { gl_handle = gl_handle_; }
    explicit NODE_Buffer() { glGenBuffers(1, &gl_handle); }
    ~NODE_Buffer() { glDeleteBuffers(1, &gl_handle); }

    static NAN_METHOD(New);

    static NAN_METHOD(NODE_id);
    static NAN_METHOD(NODE_delete);
    static NAN_METHOD(NODE_toString);

    static Nan::Persistent<v8::Function> constructor;
};

Nan::Persistent<v8::Function> NODE_Buffer::constructor;

void NODE_Buffer::Init(v8::Handle<v8::ObjectTemplate> exports) {
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->SetClassName(Nan::New<v8::String>("Buffer").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    Nan::SetPrototypeMethod(tpl, "id", NODE_id);
    Nan::SetPrototypeMethod(tpl, "toString", NODE_toString);
    Nan::SetPrototypeMethod(tpl, "inspect", NODE_toString);

    Nan::SetPrototypeMethod(tpl, "delete", NODE_delete);

    constructor.Reset(tpl->GetFunction());

    // Export constructor.
    Nan::SetTemplate(exports, "Buffer", Nan::GetFunction(tpl).ToLocalChecked());
}

NAN_METHOD(NODE_Buffer::New) {
    Nan::HandleScope scope;

    if(info.IsConstructCall()) {
        if(info.Length() == 0) {
            NODE_Buffer* obj = new NODE_Buffer();
            obj->Wrap(info.This());
            info.GetReturnValue().Set(info.This());
        } else {
            NODE_Buffer* obj = new NODE_Buffer(info[0]->Uint32Value());
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

NAN_METHOD(NODE_Buffer::NODE_id) {
    NODE_Buffer* obj = Nan::ObjectWrap::Unwrap<NODE_Buffer>(info.This());
    info.GetReturnValue().Set(Nan::New<v8::Uint32>(obj->gl_handle));
}

NAN_METHOD(NODE_Buffer::NODE_delete) {
    NODE_Buffer* obj = Nan::ObjectWrap::Unwrap<NODE_Buffer>(info.This());
    GLuint gl_handle = obj->gl_handle;
    glDeleteBuffers(1, &gl_handle);
    obj->gl_handle = 0;
}

NAN_METHOD(NODE_Buffer::NODE_toString) {
    NODE_Buffer* obj = Nan::ObjectWrap::Unwrap<NODE_Buffer>(info.This());
    char buf[64];
    sprintf(buf, "Buffer:%d", obj->gl_handle);
    info.GetReturnValue().Set(Nan::New<v8::String>(buf).ToLocalChecked());
}


class NODE_Program : public Nan::ObjectWrap {
public:
    static void Init(v8::Handle<v8::ObjectTemplate> exports);
    GLuint gl_handle;

    static v8::Handle<v8::Value> fromGLHandle(GLuint handle) {
        const int argc = 1;
        v8::Local<v8::Value> argv[argc] = { Nan::New<v8::Integer>(handle) };
        return Nan::New(constructor)->NewInstance(argc, argv);
    }

private:
    explicit NODE_Program(GLuint gl_handle_) { gl_handle = gl_handle_; }
    explicit NODE_Program() { gl_handle = 0; }
    ~NODE_Program() { glDeleteProgram(gl_handle); }

    static NAN_METHOD(New);

    static NAN_METHOD(NODE_id);
    static NAN_METHOD(NODE_delete);
    static NAN_METHOD(NODE_toString);

    static Nan::Persistent<v8::Function> constructor;
};

Nan::Persistent<v8::Function> NODE_Program::constructor;

void NODE_Program::Init(v8::Handle<v8::ObjectTemplate> exports) {
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->SetClassName(Nan::New<v8::String>("Program").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    Nan::SetPrototypeMethod(tpl, "id", NODE_id);
    Nan::SetPrototypeMethod(tpl, "toString", NODE_toString);
    Nan::SetPrototypeMethod(tpl, "inspect", NODE_toString);

    Nan::SetPrototypeMethod(tpl, "delete", NODE_delete);

    constructor.Reset(tpl->GetFunction());

    // Export constructor.
    Nan::SetTemplate(exports, "Program", Nan::GetFunction(tpl).ToLocalChecked());
}

NAN_METHOD(NODE_Program::New) {
    Nan::HandleScope scope;

    if(info.IsConstructCall()) {
        if(info.Length() == 0) {
            NODE_Program* obj = new NODE_Program();
            obj->Wrap(info.This());
            info.GetReturnValue().Set(info.This());
        } else {
            NODE_Program* obj = new NODE_Program(info[0]->Uint32Value());
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

NAN_METHOD(NODE_Program::NODE_id) {
    NODE_Program* obj = Nan::ObjectWrap::Unwrap<NODE_Program>(info.This());
    info.GetReturnValue().Set(Nan::New<v8::Uint32>(obj->gl_handle));
}

NAN_METHOD(NODE_Program::NODE_delete) {
    NODE_Program* obj = Nan::ObjectWrap::Unwrap<NODE_Program>(info.This());
    GLuint gl_handle = obj->gl_handle;
    glDeleteProgram(gl_handle);
    obj->gl_handle = 0;
}

NAN_METHOD(NODE_Program::NODE_toString) {
    NODE_Program* obj = Nan::ObjectWrap::Unwrap<NODE_Program>(info.This());
    char buf[64];
    sprintf(buf, "Program:%d", obj->gl_handle);
    info.GetReturnValue().Set(Nan::New<v8::String>(buf).ToLocalChecked());
}


class NODE_TransformFeedback : public Nan::ObjectWrap {
public:
    static void Init(v8::Handle<v8::ObjectTemplate> exports);
    GLuint gl_handle;

    static v8::Handle<v8::Value> fromGLHandle(GLuint handle) {
        const int argc = 1;
        v8::Local<v8::Value> argv[argc] = { Nan::New<v8::Integer>(handle) };
        return Nan::New(constructor)->NewInstance(argc, argv);
    }

private:
    explicit NODE_TransformFeedback(GLuint gl_handle_) { gl_handle = gl_handle_; }
    explicit NODE_TransformFeedback() { glGenTransformFeedbacks(1, &gl_handle); }
    ~NODE_TransformFeedback() { glDeleteTransformFeedbacks(1, &gl_handle); }

    static NAN_METHOD(New);

    static NAN_METHOD(NODE_id);
    static NAN_METHOD(NODE_delete);
    static NAN_METHOD(NODE_toString);

    static Nan::Persistent<v8::Function> constructor;
};

Nan::Persistent<v8::Function> NODE_TransformFeedback::constructor;

void NODE_TransformFeedback::Init(v8::Handle<v8::ObjectTemplate> exports) {
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->SetClassName(Nan::New<v8::String>("TransformFeedback").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    Nan::SetPrototypeMethod(tpl, "id", NODE_id);
    Nan::SetPrototypeMethod(tpl, "toString", NODE_toString);
    Nan::SetPrototypeMethod(tpl, "inspect", NODE_toString);

    Nan::SetPrototypeMethod(tpl, "delete", NODE_delete);

    constructor.Reset(tpl->GetFunction());

    // Export constructor.
    Nan::SetTemplate(exports, "TransformFeedback", Nan::GetFunction(tpl).ToLocalChecked());
}

NAN_METHOD(NODE_TransformFeedback::New) {
    Nan::HandleScope scope;

    if(info.IsConstructCall()) {
        if(info.Length() == 0) {
            NODE_TransformFeedback* obj = new NODE_TransformFeedback();
            obj->Wrap(info.This());
            info.GetReturnValue().Set(info.This());
        } else {
            NODE_TransformFeedback* obj = new NODE_TransformFeedback(info[0]->Uint32Value());
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

NAN_METHOD(NODE_TransformFeedback::NODE_id) {
    NODE_TransformFeedback* obj = Nan::ObjectWrap::Unwrap<NODE_TransformFeedback>(info.This());
    info.GetReturnValue().Set(Nan::New<v8::Uint32>(obj->gl_handle));
}

NAN_METHOD(NODE_TransformFeedback::NODE_delete) {
    NODE_TransformFeedback* obj = Nan::ObjectWrap::Unwrap<NODE_TransformFeedback>(info.This());
    GLuint gl_handle = obj->gl_handle;
    glDeleteTransformFeedbacks(1, &gl_handle);
    obj->gl_handle = 0;
}

NAN_METHOD(NODE_TransformFeedback::NODE_toString) {
    NODE_TransformFeedback* obj = Nan::ObjectWrap::Unwrap<NODE_TransformFeedback>(info.This());
    char buf[64];
    sprintf(buf, "TransformFeedback:%d", obj->gl_handle);
    info.GetReturnValue().Set(Nan::New<v8::String>(buf).ToLocalChecked());
}


void defineObjects(v8::Handle<v8::ObjectTemplate> exports) {
    NODE_Sampler::Init(exports);
    NODE_Framebuffer::Init(exports);
    NODE_Shader::Init(exports);
    NODE_Texture::Init(exports);
    NODE_ProgramPipeline::Init(exports);
    NODE_Renderbuffer::Init(exports);
    NODE_VertexArray::Init(exports);
    NODE_Query::Init(exports);
    NODE_Buffer::Init(exports);
    NODE_Program::Init(exports);
    NODE_TransformFeedback::Init(exports);
}


void defineConstants(v8::Handle<v8::ObjectTemplate> exports) {
    Nan::SetTemplate(exports, Nan::New<v8::String>("DEPTH_BUFFER_BIT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DEPTH_BUFFER_BIT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("STENCIL_BUFFER_BIT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_STENCIL_BUFFER_BIT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("COLOR_BUFFER_BIT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_COLOR_BUFFER_BIT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("FALSE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_FALSE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TRUE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TRUE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("POINTS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_POINTS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("LINES").ToLocalChecked(), Nan::New<v8::Uint32>(GL_LINES), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("LINE_LOOP").ToLocalChecked(), Nan::New<v8::Uint32>(GL_LINE_LOOP), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("LINE_STRIP").ToLocalChecked(), Nan::New<v8::Uint32>(GL_LINE_STRIP), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TRIANGLES").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TRIANGLES), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TRIANGLE_STRIP").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TRIANGLE_STRIP), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TRIANGLE_FAN").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TRIANGLE_FAN), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("NEVER").ToLocalChecked(), Nan::New<v8::Uint32>(GL_NEVER), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("LESS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_LESS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("EQUAL").ToLocalChecked(), Nan::New<v8::Uint32>(GL_EQUAL), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("LEQUAL").ToLocalChecked(), Nan::New<v8::Uint32>(GL_LEQUAL), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("GREATER").ToLocalChecked(), Nan::New<v8::Uint32>(GL_GREATER), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("NOTEQUAL").ToLocalChecked(), Nan::New<v8::Uint32>(GL_NOTEQUAL), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("GEQUAL").ToLocalChecked(), Nan::New<v8::Uint32>(GL_GEQUAL), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("ALWAYS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_ALWAYS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("ZERO").ToLocalChecked(), Nan::New<v8::Uint32>(GL_ZERO), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("ONE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_ONE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SRC_COLOR").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SRC_COLOR), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("ONE_MINUS_SRC_COLOR").ToLocalChecked(), Nan::New<v8::Uint32>(GL_ONE_MINUS_SRC_COLOR), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SRC_ALPHA").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SRC_ALPHA), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("ONE_MINUS_SRC_ALPHA").ToLocalChecked(), Nan::New<v8::Uint32>(GL_ONE_MINUS_SRC_ALPHA), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DST_ALPHA").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DST_ALPHA), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("ONE_MINUS_DST_ALPHA").ToLocalChecked(), Nan::New<v8::Uint32>(GL_ONE_MINUS_DST_ALPHA), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DST_COLOR").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DST_COLOR), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("ONE_MINUS_DST_COLOR").ToLocalChecked(), Nan::New<v8::Uint32>(GL_ONE_MINUS_DST_COLOR), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SRC_ALPHA_SATURATE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SRC_ALPHA_SATURATE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("NONE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_NONE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("FRONT_LEFT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_FRONT_LEFT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("FRONT_RIGHT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_FRONT_RIGHT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("BACK_LEFT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_BACK_LEFT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("BACK_RIGHT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_BACK_RIGHT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("FRONT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_FRONT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("BACK").ToLocalChecked(), Nan::New<v8::Uint32>(GL_BACK), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("LEFT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_LEFT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RIGHT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RIGHT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("FRONT_AND_BACK").ToLocalChecked(), Nan::New<v8::Uint32>(GL_FRONT_AND_BACK), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("NO_ERROR").ToLocalChecked(), Nan::New<v8::Uint32>(GL_NO_ERROR), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("INVALID_ENUM").ToLocalChecked(), Nan::New<v8::Uint32>(GL_INVALID_ENUM), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("INVALID_VALUE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_INVALID_VALUE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("INVALID_OPERATION").ToLocalChecked(), Nan::New<v8::Uint32>(GL_INVALID_OPERATION), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("OUT_OF_MEMORY").ToLocalChecked(), Nan::New<v8::Uint32>(GL_OUT_OF_MEMORY), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("CW").ToLocalChecked(), Nan::New<v8::Uint32>(GL_CW), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("CCW").ToLocalChecked(), Nan::New<v8::Uint32>(GL_CCW), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("POINT_SIZE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_POINT_SIZE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("POINT_SIZE_RANGE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_POINT_SIZE_RANGE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("POINT_SIZE_GRANULARITY").ToLocalChecked(), Nan::New<v8::Uint32>(GL_POINT_SIZE_GRANULARITY), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("LINE_SMOOTH").ToLocalChecked(), Nan::New<v8::Uint32>(GL_LINE_SMOOTH), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("LINE_WIDTH").ToLocalChecked(), Nan::New<v8::Uint32>(GL_LINE_WIDTH), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("LINE_WIDTH_RANGE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_LINE_WIDTH_RANGE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("LINE_WIDTH_GRANULARITY").ToLocalChecked(), Nan::New<v8::Uint32>(GL_LINE_WIDTH_GRANULARITY), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("POLYGON_MODE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_POLYGON_MODE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("POLYGON_SMOOTH").ToLocalChecked(), Nan::New<v8::Uint32>(GL_POLYGON_SMOOTH), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("CULL_FACE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_CULL_FACE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("CULL_FACE_MODE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_CULL_FACE_MODE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("FRONT_FACE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_FRONT_FACE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DEPTH_RANGE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DEPTH_RANGE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DEPTH_TEST").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DEPTH_TEST), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DEPTH_WRITEMASK").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DEPTH_WRITEMASK), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DEPTH_CLEAR_VALUE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DEPTH_CLEAR_VALUE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DEPTH_FUNC").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DEPTH_FUNC), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("STENCIL_TEST").ToLocalChecked(), Nan::New<v8::Uint32>(GL_STENCIL_TEST), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("STENCIL_CLEAR_VALUE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_STENCIL_CLEAR_VALUE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("STENCIL_FUNC").ToLocalChecked(), Nan::New<v8::Uint32>(GL_STENCIL_FUNC), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("STENCIL_VALUE_MASK").ToLocalChecked(), Nan::New<v8::Uint32>(GL_STENCIL_VALUE_MASK), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("STENCIL_FAIL").ToLocalChecked(), Nan::New<v8::Uint32>(GL_STENCIL_FAIL), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("STENCIL_PASS_DEPTH_FAIL").ToLocalChecked(), Nan::New<v8::Uint32>(GL_STENCIL_PASS_DEPTH_FAIL), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("STENCIL_PASS_DEPTH_PASS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_STENCIL_PASS_DEPTH_PASS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("STENCIL_REF").ToLocalChecked(), Nan::New<v8::Uint32>(GL_STENCIL_REF), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("STENCIL_WRITEMASK").ToLocalChecked(), Nan::New<v8::Uint32>(GL_STENCIL_WRITEMASK), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("VIEWPORT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_VIEWPORT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DITHER").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DITHER), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("BLEND_DST").ToLocalChecked(), Nan::New<v8::Uint32>(GL_BLEND_DST), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("BLEND_SRC").ToLocalChecked(), Nan::New<v8::Uint32>(GL_BLEND_SRC), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("BLEND").ToLocalChecked(), Nan::New<v8::Uint32>(GL_BLEND), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("LOGIC_OP_MODE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_LOGIC_OP_MODE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("COLOR_LOGIC_OP").ToLocalChecked(), Nan::New<v8::Uint32>(GL_COLOR_LOGIC_OP), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DRAW_BUFFER").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DRAW_BUFFER), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("READ_BUFFER").ToLocalChecked(), Nan::New<v8::Uint32>(GL_READ_BUFFER), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SCISSOR_BOX").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SCISSOR_BOX), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SCISSOR_TEST").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SCISSOR_TEST), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("COLOR_CLEAR_VALUE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_COLOR_CLEAR_VALUE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("COLOR_WRITEMASK").ToLocalChecked(), Nan::New<v8::Uint32>(GL_COLOR_WRITEMASK), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DOUBLEBUFFER").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DOUBLEBUFFER), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("STEREO").ToLocalChecked(), Nan::New<v8::Uint32>(GL_STEREO), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("LINE_SMOOTH_HINT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_LINE_SMOOTH_HINT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("POLYGON_SMOOTH_HINT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_POLYGON_SMOOTH_HINT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNPACK_SWAP_BYTES").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNPACK_SWAP_BYTES), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNPACK_LSB_FIRST").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNPACK_LSB_FIRST), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNPACK_ROW_LENGTH").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNPACK_ROW_LENGTH), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNPACK_SKIP_ROWS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNPACK_SKIP_ROWS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNPACK_SKIP_PIXELS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNPACK_SKIP_PIXELS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNPACK_ALIGNMENT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNPACK_ALIGNMENT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("PACK_SWAP_BYTES").ToLocalChecked(), Nan::New<v8::Uint32>(GL_PACK_SWAP_BYTES), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("PACK_LSB_FIRST").ToLocalChecked(), Nan::New<v8::Uint32>(GL_PACK_LSB_FIRST), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("PACK_ROW_LENGTH").ToLocalChecked(), Nan::New<v8::Uint32>(GL_PACK_ROW_LENGTH), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("PACK_SKIP_ROWS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_PACK_SKIP_ROWS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("PACK_SKIP_PIXELS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_PACK_SKIP_PIXELS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("PACK_ALIGNMENT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_PACK_ALIGNMENT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_TEXTURE_SIZE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_TEXTURE_SIZE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_VIEWPORT_DIMS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_VIEWPORT_DIMS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SUBPIXEL_BITS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SUBPIXEL_BITS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_1D").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_1D), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_2D").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_2D), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("POLYGON_OFFSET_UNITS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_POLYGON_OFFSET_UNITS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("POLYGON_OFFSET_POINT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_POLYGON_OFFSET_POINT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("POLYGON_OFFSET_LINE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_POLYGON_OFFSET_LINE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("POLYGON_OFFSET_FILL").ToLocalChecked(), Nan::New<v8::Uint32>(GL_POLYGON_OFFSET_FILL), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("POLYGON_OFFSET_FACTOR").ToLocalChecked(), Nan::New<v8::Uint32>(GL_POLYGON_OFFSET_FACTOR), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_BINDING_1D").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_BINDING_1D), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_BINDING_2D").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_BINDING_2D), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_WIDTH").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_WIDTH), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_HEIGHT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_HEIGHT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_INTERNAL_FORMAT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_INTERNAL_FORMAT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_BORDER_COLOR").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_BORDER_COLOR), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_RED_SIZE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_RED_SIZE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_GREEN_SIZE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_GREEN_SIZE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_BLUE_SIZE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_BLUE_SIZE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_ALPHA_SIZE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_ALPHA_SIZE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DONT_CARE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DONT_CARE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("FASTEST").ToLocalChecked(), Nan::New<v8::Uint32>(GL_FASTEST), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("NICEST").ToLocalChecked(), Nan::New<v8::Uint32>(GL_NICEST), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("BYTE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_BYTE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNSIGNED_BYTE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNSIGNED_BYTE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SHORT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SHORT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNSIGNED_SHORT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNSIGNED_SHORT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("INT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_INT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNSIGNED_INT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNSIGNED_INT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("FLOAT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_FLOAT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DOUBLE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DOUBLE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("CLEAR").ToLocalChecked(), Nan::New<v8::Uint32>(GL_CLEAR), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("AND").ToLocalChecked(), Nan::New<v8::Uint32>(GL_AND), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("AND_REVERSE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_AND_REVERSE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("COPY").ToLocalChecked(), Nan::New<v8::Uint32>(GL_COPY), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("AND_INVERTED").ToLocalChecked(), Nan::New<v8::Uint32>(GL_AND_INVERTED), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("NOOP").ToLocalChecked(), Nan::New<v8::Uint32>(GL_NOOP), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("XOR").ToLocalChecked(), Nan::New<v8::Uint32>(GL_XOR), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("OR").ToLocalChecked(), Nan::New<v8::Uint32>(GL_OR), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("NOR").ToLocalChecked(), Nan::New<v8::Uint32>(GL_NOR), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("EQUIV").ToLocalChecked(), Nan::New<v8::Uint32>(GL_EQUIV), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("INVERT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_INVERT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("OR_REVERSE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_OR_REVERSE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("COPY_INVERTED").ToLocalChecked(), Nan::New<v8::Uint32>(GL_COPY_INVERTED), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("OR_INVERTED").ToLocalChecked(), Nan::New<v8::Uint32>(GL_OR_INVERTED), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("NAND").ToLocalChecked(), Nan::New<v8::Uint32>(GL_NAND), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SET").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SET), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("COLOR").ToLocalChecked(), Nan::New<v8::Uint32>(GL_COLOR), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DEPTH").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DEPTH), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("STENCIL").ToLocalChecked(), Nan::New<v8::Uint32>(GL_STENCIL), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("STENCIL_INDEX").ToLocalChecked(), Nan::New<v8::Uint32>(GL_STENCIL_INDEX), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DEPTH_COMPONENT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DEPTH_COMPONENT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RED").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RED), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("GREEN").ToLocalChecked(), Nan::New<v8::Uint32>(GL_GREEN), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("BLUE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_BLUE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("ALPHA").ToLocalChecked(), Nan::New<v8::Uint32>(GL_ALPHA), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RGB").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RGB), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RGBA").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RGBA), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("POINT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_POINT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("LINE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_LINE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("FILL").ToLocalChecked(), Nan::New<v8::Uint32>(GL_FILL), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("KEEP").ToLocalChecked(), Nan::New<v8::Uint32>(GL_KEEP), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("REPLACE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_REPLACE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("INCR").ToLocalChecked(), Nan::New<v8::Uint32>(GL_INCR), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DECR").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DECR), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("VENDOR").ToLocalChecked(), Nan::New<v8::Uint32>(GL_VENDOR), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RENDERER").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RENDERER), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("VERSION").ToLocalChecked(), Nan::New<v8::Uint32>(GL_VERSION), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("EXTENSIONS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_EXTENSIONS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("NEAREST").ToLocalChecked(), Nan::New<v8::Uint32>(GL_NEAREST), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("LINEAR").ToLocalChecked(), Nan::New<v8::Uint32>(GL_LINEAR), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("NEAREST_MIPMAP_NEAREST").ToLocalChecked(), Nan::New<v8::Uint32>(GL_NEAREST_MIPMAP_NEAREST), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("LINEAR_MIPMAP_NEAREST").ToLocalChecked(), Nan::New<v8::Uint32>(GL_LINEAR_MIPMAP_NEAREST), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("NEAREST_MIPMAP_LINEAR").ToLocalChecked(), Nan::New<v8::Uint32>(GL_NEAREST_MIPMAP_LINEAR), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("LINEAR_MIPMAP_LINEAR").ToLocalChecked(), Nan::New<v8::Uint32>(GL_LINEAR_MIPMAP_LINEAR), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_MAG_FILTER").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_MAG_FILTER), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_MIN_FILTER").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_MIN_FILTER), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_WRAP_S").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_WRAP_S), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_WRAP_T").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_WRAP_T), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("PROXY_TEXTURE_1D").ToLocalChecked(), Nan::New<v8::Uint32>(GL_PROXY_TEXTURE_1D), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("PROXY_TEXTURE_2D").ToLocalChecked(), Nan::New<v8::Uint32>(GL_PROXY_TEXTURE_2D), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("REPEAT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_REPEAT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("R3_G3_B2").ToLocalChecked(), Nan::New<v8::Uint32>(GL_R3_G3_B2), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RGB4").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RGB4), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RGB5").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RGB5), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RGB8").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RGB8), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RGB10").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RGB10), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RGB12").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RGB12), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RGB16").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RGB16), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RGBA2").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RGBA2), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RGBA4").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RGBA4), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RGB5_A1").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RGB5_A1), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RGBA8").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RGBA8), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RGB10_A2").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RGB10_A2), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RGBA12").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RGBA12), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RGBA16").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RGBA16), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNSIGNED_BYTE_3_3_2").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNSIGNED_BYTE_3_3_2), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNSIGNED_SHORT_4_4_4_4").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNSIGNED_SHORT_4_4_4_4), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNSIGNED_SHORT_5_5_5_1").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNSIGNED_SHORT_5_5_5_1), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNSIGNED_INT_8_8_8_8").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNSIGNED_INT_8_8_8_8), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNSIGNED_INT_10_10_10_2").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNSIGNED_INT_10_10_10_2), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_BINDING_3D").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_BINDING_3D), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("PACK_SKIP_IMAGES").ToLocalChecked(), Nan::New<v8::Uint32>(GL_PACK_SKIP_IMAGES), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("PACK_IMAGE_HEIGHT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_PACK_IMAGE_HEIGHT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNPACK_SKIP_IMAGES").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNPACK_SKIP_IMAGES), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNPACK_IMAGE_HEIGHT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNPACK_IMAGE_HEIGHT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_3D").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_3D), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("PROXY_TEXTURE_3D").ToLocalChecked(), Nan::New<v8::Uint32>(GL_PROXY_TEXTURE_3D), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_DEPTH").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_DEPTH), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_WRAP_R").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_WRAP_R), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_3D_TEXTURE_SIZE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_3D_TEXTURE_SIZE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNSIGNED_BYTE_2_3_3_REV").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNSIGNED_BYTE_2_3_3_REV), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNSIGNED_SHORT_5_6_5").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNSIGNED_SHORT_5_6_5), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNSIGNED_SHORT_5_6_5_REV").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNSIGNED_SHORT_5_6_5_REV), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNSIGNED_SHORT_4_4_4_4_REV").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNSIGNED_SHORT_4_4_4_4_REV), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNSIGNED_SHORT_1_5_5_5_REV").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNSIGNED_SHORT_1_5_5_5_REV), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNSIGNED_INT_8_8_8_8_REV").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNSIGNED_INT_8_8_8_8_REV), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNSIGNED_INT_2_10_10_10_REV").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNSIGNED_INT_2_10_10_10_REV), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("BGR").ToLocalChecked(), Nan::New<v8::Uint32>(GL_BGR), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("BGRA").ToLocalChecked(), Nan::New<v8::Uint32>(GL_BGRA), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_ELEMENTS_VERTICES").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_ELEMENTS_VERTICES), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_ELEMENTS_INDICES").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_ELEMENTS_INDICES), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("CLAMP_TO_EDGE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_CLAMP_TO_EDGE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_MIN_LOD").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_MIN_LOD), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_MAX_LOD").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_MAX_LOD), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_BASE_LEVEL").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_BASE_LEVEL), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_MAX_LEVEL").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_MAX_LEVEL), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SMOOTH_POINT_SIZE_RANGE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SMOOTH_POINT_SIZE_RANGE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SMOOTH_POINT_SIZE_GRANULARITY").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SMOOTH_POINT_SIZE_GRANULARITY), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SMOOTH_LINE_WIDTH_RANGE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SMOOTH_LINE_WIDTH_RANGE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SMOOTH_LINE_WIDTH_GRANULARITY").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SMOOTH_LINE_WIDTH_GRANULARITY), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("ALIASED_LINE_WIDTH_RANGE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_ALIASED_LINE_WIDTH_RANGE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("CONSTANT_COLOR").ToLocalChecked(), Nan::New<v8::Uint32>(GL_CONSTANT_COLOR), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("ONE_MINUS_CONSTANT_COLOR").ToLocalChecked(), Nan::New<v8::Uint32>(GL_ONE_MINUS_CONSTANT_COLOR), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("CONSTANT_ALPHA").ToLocalChecked(), Nan::New<v8::Uint32>(GL_CONSTANT_ALPHA), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("ONE_MINUS_CONSTANT_ALPHA").ToLocalChecked(), Nan::New<v8::Uint32>(GL_ONE_MINUS_CONSTANT_ALPHA), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("BLEND_COLOR").ToLocalChecked(), Nan::New<v8::Uint32>(GL_BLEND_COLOR), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("FUNC_ADD").ToLocalChecked(), Nan::New<v8::Uint32>(GL_FUNC_ADD), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MIN").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MIN), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("BLEND_EQUATION").ToLocalChecked(), Nan::New<v8::Uint32>(GL_BLEND_EQUATION), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("FUNC_SUBTRACT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_FUNC_SUBTRACT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("FUNC_REVERSE_SUBTRACT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_FUNC_REVERSE_SUBTRACT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE0").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE0), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE1").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE1), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE2").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE2), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE3").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE3), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE4").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE4), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE5").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE5), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE6").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE6), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE7").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE7), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE8").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE8), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE9").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE9), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE10").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE10), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE11").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE11), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE12").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE12), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE13").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE13), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE14").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE14), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE15").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE15), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE16").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE16), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE17").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE17), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE18").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE18), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE19").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE19), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE20").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE20), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE21").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE21), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE22").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE22), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE23").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE23), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE24").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE24), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE25").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE25), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE26").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE26), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE27").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE27), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE28").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE28), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE29").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE29), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE30").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE30), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE31").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE31), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("ACTIVE_TEXTURE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_ACTIVE_TEXTURE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MULTISAMPLE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MULTISAMPLE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SAMPLE_ALPHA_TO_COVERAGE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SAMPLE_ALPHA_TO_COVERAGE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SAMPLE_ALPHA_TO_ONE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SAMPLE_ALPHA_TO_ONE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SAMPLE_COVERAGE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SAMPLE_COVERAGE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SAMPLE_BUFFERS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SAMPLE_BUFFERS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SAMPLES").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SAMPLES), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SAMPLE_COVERAGE_VALUE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SAMPLE_COVERAGE_VALUE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SAMPLE_COVERAGE_INVERT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SAMPLE_COVERAGE_INVERT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_CUBE_MAP").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_CUBE_MAP), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_BINDING_CUBE_MAP").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_BINDING_CUBE_MAP), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_CUBE_MAP_POSITIVE_X").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_CUBE_MAP_POSITIVE_X), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_CUBE_MAP_NEGATIVE_X").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_CUBE_MAP_NEGATIVE_X), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_CUBE_MAP_POSITIVE_Y").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_CUBE_MAP_POSITIVE_Y), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_CUBE_MAP_NEGATIVE_Y").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_CUBE_MAP_POSITIVE_Z").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_CUBE_MAP_POSITIVE_Z), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_CUBE_MAP_NEGATIVE_Z").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("PROXY_TEXTURE_CUBE_MAP").ToLocalChecked(), Nan::New<v8::Uint32>(GL_PROXY_TEXTURE_CUBE_MAP), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_CUBE_MAP_TEXTURE_SIZE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_CUBE_MAP_TEXTURE_SIZE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("COMPRESSED_RGB").ToLocalChecked(), Nan::New<v8::Uint32>(GL_COMPRESSED_RGB), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("COMPRESSED_RGBA").ToLocalChecked(), Nan::New<v8::Uint32>(GL_COMPRESSED_RGBA), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_COMPRESSION_HINT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_COMPRESSION_HINT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_COMPRESSED_IMAGE_SIZE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_COMPRESSED_IMAGE_SIZE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_COMPRESSED").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_COMPRESSED), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("NUM_COMPRESSED_TEXTURE_FORMATS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_NUM_COMPRESSED_TEXTURE_FORMATS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("COMPRESSED_TEXTURE_FORMATS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_COMPRESSED_TEXTURE_FORMATS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("CLAMP_TO_BORDER").ToLocalChecked(), Nan::New<v8::Uint32>(GL_CLAMP_TO_BORDER), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("BLEND_DST_RGB").ToLocalChecked(), Nan::New<v8::Uint32>(GL_BLEND_DST_RGB), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("BLEND_SRC_RGB").ToLocalChecked(), Nan::New<v8::Uint32>(GL_BLEND_SRC_RGB), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("BLEND_DST_ALPHA").ToLocalChecked(), Nan::New<v8::Uint32>(GL_BLEND_DST_ALPHA), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("BLEND_SRC_ALPHA").ToLocalChecked(), Nan::New<v8::Uint32>(GL_BLEND_SRC_ALPHA), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("POINT_FADE_THRESHOLD_SIZE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_POINT_FADE_THRESHOLD_SIZE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DEPTH_COMPONENT16").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DEPTH_COMPONENT16), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DEPTH_COMPONENT24").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DEPTH_COMPONENT24), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DEPTH_COMPONENT32").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DEPTH_COMPONENT32), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MIRRORED_REPEAT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MIRRORED_REPEAT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_TEXTURE_LOD_BIAS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_TEXTURE_LOD_BIAS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_LOD_BIAS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_LOD_BIAS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("INCR_WRAP").ToLocalChecked(), Nan::New<v8::Uint32>(GL_INCR_WRAP), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DECR_WRAP").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DECR_WRAP), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_DEPTH_SIZE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_DEPTH_SIZE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_COMPARE_MODE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_COMPARE_MODE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_COMPARE_FUNC").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_COMPARE_FUNC), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("BUFFER_SIZE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_BUFFER_SIZE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("BUFFER_USAGE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_BUFFER_USAGE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("QUERY_COUNTER_BITS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_QUERY_COUNTER_BITS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("CURRENT_QUERY").ToLocalChecked(), Nan::New<v8::Uint32>(GL_CURRENT_QUERY), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("QUERY_RESULT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_QUERY_RESULT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("QUERY_RESULT_AVAILABLE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_QUERY_RESULT_AVAILABLE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("ARRAY_BUFFER").ToLocalChecked(), Nan::New<v8::Uint32>(GL_ARRAY_BUFFER), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("ELEMENT_ARRAY_BUFFER").ToLocalChecked(), Nan::New<v8::Uint32>(GL_ELEMENT_ARRAY_BUFFER), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("ARRAY_BUFFER_BINDING").ToLocalChecked(), Nan::New<v8::Uint32>(GL_ARRAY_BUFFER_BINDING), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("ELEMENT_ARRAY_BUFFER_BINDING").ToLocalChecked(), Nan::New<v8::Uint32>(GL_ELEMENT_ARRAY_BUFFER_BINDING), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("VERTEX_ATTRIB_ARRAY_BUFFER_BINDING").ToLocalChecked(), Nan::New<v8::Uint32>(GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("READ_ONLY").ToLocalChecked(), Nan::New<v8::Uint32>(GL_READ_ONLY), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("WRITE_ONLY").ToLocalChecked(), Nan::New<v8::Uint32>(GL_WRITE_ONLY), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("READ_WRITE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_READ_WRITE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("BUFFER_ACCESS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_BUFFER_ACCESS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("BUFFER_MAPPED").ToLocalChecked(), Nan::New<v8::Uint32>(GL_BUFFER_MAPPED), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("BUFFER_MAP_POINTER").ToLocalChecked(), Nan::New<v8::Uint32>(GL_BUFFER_MAP_POINTER), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("STREAM_DRAW").ToLocalChecked(), Nan::New<v8::Uint32>(GL_STREAM_DRAW), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("STREAM_READ").ToLocalChecked(), Nan::New<v8::Uint32>(GL_STREAM_READ), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("STREAM_COPY").ToLocalChecked(), Nan::New<v8::Uint32>(GL_STREAM_COPY), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("STATIC_DRAW").ToLocalChecked(), Nan::New<v8::Uint32>(GL_STATIC_DRAW), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("STATIC_READ").ToLocalChecked(), Nan::New<v8::Uint32>(GL_STATIC_READ), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("STATIC_COPY").ToLocalChecked(), Nan::New<v8::Uint32>(GL_STATIC_COPY), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DYNAMIC_DRAW").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DYNAMIC_DRAW), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DYNAMIC_READ").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DYNAMIC_READ), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DYNAMIC_COPY").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DYNAMIC_COPY), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SAMPLES_PASSED").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SAMPLES_PASSED), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("BLEND_EQUATION_RGB").ToLocalChecked(), Nan::New<v8::Uint32>(GL_BLEND_EQUATION_RGB), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("VERTEX_ATTRIB_ARRAY_ENABLED").ToLocalChecked(), Nan::New<v8::Uint32>(GL_VERTEX_ATTRIB_ARRAY_ENABLED), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("VERTEX_ATTRIB_ARRAY_SIZE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_VERTEX_ATTRIB_ARRAY_SIZE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("VERTEX_ATTRIB_ARRAY_STRIDE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_VERTEX_ATTRIB_ARRAY_STRIDE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("VERTEX_ATTRIB_ARRAY_TYPE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_VERTEX_ATTRIB_ARRAY_TYPE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("CURRENT_VERTEX_ATTRIB").ToLocalChecked(), Nan::New<v8::Uint32>(GL_CURRENT_VERTEX_ATTRIB), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("VERTEX_PROGRAM_POINT_SIZE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_VERTEX_PROGRAM_POINT_SIZE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("VERTEX_ATTRIB_ARRAY_POINTER").ToLocalChecked(), Nan::New<v8::Uint32>(GL_VERTEX_ATTRIB_ARRAY_POINTER), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("STENCIL_BACK_FUNC").ToLocalChecked(), Nan::New<v8::Uint32>(GL_STENCIL_BACK_FUNC), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("STENCIL_BACK_FAIL").ToLocalChecked(), Nan::New<v8::Uint32>(GL_STENCIL_BACK_FAIL), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("STENCIL_BACK_PASS_DEPTH_FAIL").ToLocalChecked(), Nan::New<v8::Uint32>(GL_STENCIL_BACK_PASS_DEPTH_FAIL), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("STENCIL_BACK_PASS_DEPTH_PASS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_STENCIL_BACK_PASS_DEPTH_PASS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_DRAW_BUFFERS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_DRAW_BUFFERS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DRAW_BUFFER0").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DRAW_BUFFER0), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DRAW_BUFFER1").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DRAW_BUFFER1), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DRAW_BUFFER2").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DRAW_BUFFER2), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DRAW_BUFFER3").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DRAW_BUFFER3), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DRAW_BUFFER4").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DRAW_BUFFER4), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DRAW_BUFFER5").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DRAW_BUFFER5), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DRAW_BUFFER6").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DRAW_BUFFER6), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DRAW_BUFFER7").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DRAW_BUFFER7), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DRAW_BUFFER8").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DRAW_BUFFER8), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DRAW_BUFFER9").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DRAW_BUFFER9), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DRAW_BUFFER10").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DRAW_BUFFER10), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DRAW_BUFFER11").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DRAW_BUFFER11), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DRAW_BUFFER12").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DRAW_BUFFER12), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DRAW_BUFFER13").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DRAW_BUFFER13), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DRAW_BUFFER14").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DRAW_BUFFER14), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DRAW_BUFFER15").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DRAW_BUFFER15), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("BLEND_EQUATION_ALPHA").ToLocalChecked(), Nan::New<v8::Uint32>(GL_BLEND_EQUATION_ALPHA), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_VERTEX_ATTRIBS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_VERTEX_ATTRIBS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("VERTEX_ATTRIB_ARRAY_NORMALIZED").ToLocalChecked(), Nan::New<v8::Uint32>(GL_VERTEX_ATTRIB_ARRAY_NORMALIZED), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_TEXTURE_IMAGE_UNITS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_TEXTURE_IMAGE_UNITS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("FRAGMENT_SHADER").ToLocalChecked(), Nan::New<v8::Uint32>(GL_FRAGMENT_SHADER), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("VERTEX_SHADER").ToLocalChecked(), Nan::New<v8::Uint32>(GL_VERTEX_SHADER), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_FRAGMENT_UNIFORM_COMPONENTS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_VERTEX_UNIFORM_COMPONENTS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_VERTEX_UNIFORM_COMPONENTS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_VARYING_FLOATS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_VARYING_FLOATS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_VERTEX_TEXTURE_IMAGE_UNITS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_COMBINED_TEXTURE_IMAGE_UNITS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SHADER_TYPE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SHADER_TYPE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("FLOAT_VEC2").ToLocalChecked(), Nan::New<v8::Uint32>(GL_FLOAT_VEC2), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("FLOAT_VEC3").ToLocalChecked(), Nan::New<v8::Uint32>(GL_FLOAT_VEC3), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("FLOAT_VEC4").ToLocalChecked(), Nan::New<v8::Uint32>(GL_FLOAT_VEC4), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("INT_VEC2").ToLocalChecked(), Nan::New<v8::Uint32>(GL_INT_VEC2), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("INT_VEC3").ToLocalChecked(), Nan::New<v8::Uint32>(GL_INT_VEC3), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("INT_VEC4").ToLocalChecked(), Nan::New<v8::Uint32>(GL_INT_VEC4), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("BOOL").ToLocalChecked(), Nan::New<v8::Uint32>(GL_BOOL), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("BOOL_VEC2").ToLocalChecked(), Nan::New<v8::Uint32>(GL_BOOL_VEC2), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("BOOL_VEC3").ToLocalChecked(), Nan::New<v8::Uint32>(GL_BOOL_VEC3), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("BOOL_VEC4").ToLocalChecked(), Nan::New<v8::Uint32>(GL_BOOL_VEC4), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("FLOAT_MAT2").ToLocalChecked(), Nan::New<v8::Uint32>(GL_FLOAT_MAT2), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("FLOAT_MAT3").ToLocalChecked(), Nan::New<v8::Uint32>(GL_FLOAT_MAT3), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("FLOAT_MAT4").ToLocalChecked(), Nan::New<v8::Uint32>(GL_FLOAT_MAT4), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SAMPLER_1D").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SAMPLER_1D), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SAMPLER_2D").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SAMPLER_2D), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SAMPLER_3D").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SAMPLER_3D), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SAMPLER_CUBE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SAMPLER_CUBE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SAMPLER_1D_SHADOW").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SAMPLER_1D_SHADOW), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SAMPLER_2D_SHADOW").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SAMPLER_2D_SHADOW), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DELETE_STATUS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DELETE_STATUS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("COMPILE_STATUS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_COMPILE_STATUS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("LINK_STATUS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_LINK_STATUS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("VALIDATE_STATUS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_VALIDATE_STATUS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("INFO_LOG_LENGTH").ToLocalChecked(), Nan::New<v8::Uint32>(GL_INFO_LOG_LENGTH), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("ATTACHED_SHADERS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_ATTACHED_SHADERS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("ACTIVE_UNIFORMS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_ACTIVE_UNIFORMS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("ACTIVE_UNIFORM_MAX_LENGTH").ToLocalChecked(), Nan::New<v8::Uint32>(GL_ACTIVE_UNIFORM_MAX_LENGTH), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SHADER_SOURCE_LENGTH").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SHADER_SOURCE_LENGTH), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("ACTIVE_ATTRIBUTES").ToLocalChecked(), Nan::New<v8::Uint32>(GL_ACTIVE_ATTRIBUTES), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("ACTIVE_ATTRIBUTE_MAX_LENGTH").ToLocalChecked(), Nan::New<v8::Uint32>(GL_ACTIVE_ATTRIBUTE_MAX_LENGTH), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("FRAGMENT_SHADER_DERIVATIVE_HINT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_FRAGMENT_SHADER_DERIVATIVE_HINT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SHADING_LANGUAGE_VERSION").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SHADING_LANGUAGE_VERSION), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("CURRENT_PROGRAM").ToLocalChecked(), Nan::New<v8::Uint32>(GL_CURRENT_PROGRAM), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("POINT_SPRITE_COORD_ORIGIN").ToLocalChecked(), Nan::New<v8::Uint32>(GL_POINT_SPRITE_COORD_ORIGIN), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("LOWER_LEFT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_LOWER_LEFT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UPPER_LEFT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UPPER_LEFT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("STENCIL_BACK_REF").ToLocalChecked(), Nan::New<v8::Uint32>(GL_STENCIL_BACK_REF), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("STENCIL_BACK_VALUE_MASK").ToLocalChecked(), Nan::New<v8::Uint32>(GL_STENCIL_BACK_VALUE_MASK), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("STENCIL_BACK_WRITEMASK").ToLocalChecked(), Nan::New<v8::Uint32>(GL_STENCIL_BACK_WRITEMASK), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("PIXEL_PACK_BUFFER").ToLocalChecked(), Nan::New<v8::Uint32>(GL_PIXEL_PACK_BUFFER), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("PIXEL_UNPACK_BUFFER").ToLocalChecked(), Nan::New<v8::Uint32>(GL_PIXEL_UNPACK_BUFFER), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("PIXEL_PACK_BUFFER_BINDING").ToLocalChecked(), Nan::New<v8::Uint32>(GL_PIXEL_PACK_BUFFER_BINDING), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("PIXEL_UNPACK_BUFFER_BINDING").ToLocalChecked(), Nan::New<v8::Uint32>(GL_PIXEL_UNPACK_BUFFER_BINDING), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("FLOAT_MAT2x3").ToLocalChecked(), Nan::New<v8::Uint32>(GL_FLOAT_MAT2x3), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("FLOAT_MAT2x4").ToLocalChecked(), Nan::New<v8::Uint32>(GL_FLOAT_MAT2x4), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("FLOAT_MAT3x2").ToLocalChecked(), Nan::New<v8::Uint32>(GL_FLOAT_MAT3x2), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("FLOAT_MAT3x4").ToLocalChecked(), Nan::New<v8::Uint32>(GL_FLOAT_MAT3x4), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("FLOAT_MAT4x2").ToLocalChecked(), Nan::New<v8::Uint32>(GL_FLOAT_MAT4x2), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("FLOAT_MAT4x3").ToLocalChecked(), Nan::New<v8::Uint32>(GL_FLOAT_MAT4x3), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SRGB").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SRGB), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SRGB8").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SRGB8), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SRGB_ALPHA").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SRGB_ALPHA), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SRGB8_ALPHA8").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SRGB8_ALPHA8), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("COMPRESSED_SRGB").ToLocalChecked(), Nan::New<v8::Uint32>(GL_COMPRESSED_SRGB), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("COMPRESSED_SRGB_ALPHA").ToLocalChecked(), Nan::New<v8::Uint32>(GL_COMPRESSED_SRGB_ALPHA), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("COMPARE_REF_TO_TEXTURE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_COMPARE_REF_TO_TEXTURE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("CLIP_DISTANCE0").ToLocalChecked(), Nan::New<v8::Uint32>(GL_CLIP_DISTANCE0), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("CLIP_DISTANCE1").ToLocalChecked(), Nan::New<v8::Uint32>(GL_CLIP_DISTANCE1), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("CLIP_DISTANCE2").ToLocalChecked(), Nan::New<v8::Uint32>(GL_CLIP_DISTANCE2), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("CLIP_DISTANCE3").ToLocalChecked(), Nan::New<v8::Uint32>(GL_CLIP_DISTANCE3), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("CLIP_DISTANCE4").ToLocalChecked(), Nan::New<v8::Uint32>(GL_CLIP_DISTANCE4), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("CLIP_DISTANCE5").ToLocalChecked(), Nan::New<v8::Uint32>(GL_CLIP_DISTANCE5), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_CLIP_DISTANCES").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_CLIP_DISTANCES), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAJOR_VERSION").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAJOR_VERSION), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MINOR_VERSION").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MINOR_VERSION), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("NUM_EXTENSIONS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_NUM_EXTENSIONS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("CONTEXT_FLAGS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_CONTEXT_FLAGS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("COMPRESSED_RED").ToLocalChecked(), Nan::New<v8::Uint32>(GL_COMPRESSED_RED), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("COMPRESSED_RG").ToLocalChecked(), Nan::New<v8::Uint32>(GL_COMPRESSED_RG), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RGBA32F").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RGBA32F), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RGB32F").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RGB32F), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RGBA16F").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RGBA16F), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RGB16F").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RGB16F), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("VERTEX_ATTRIB_ARRAY_INTEGER").ToLocalChecked(), Nan::New<v8::Uint32>(GL_VERTEX_ATTRIB_ARRAY_INTEGER), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_ARRAY_TEXTURE_LAYERS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_ARRAY_TEXTURE_LAYERS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MIN_PROGRAM_TEXEL_OFFSET").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MIN_PROGRAM_TEXEL_OFFSET), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_PROGRAM_TEXEL_OFFSET").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_PROGRAM_TEXEL_OFFSET), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("CLAMP_READ_COLOR").ToLocalChecked(), Nan::New<v8::Uint32>(GL_CLAMP_READ_COLOR), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("FIXED_ONLY").ToLocalChecked(), Nan::New<v8::Uint32>(GL_FIXED_ONLY), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_VARYING_COMPONENTS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_VARYING_COMPONENTS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_1D_ARRAY").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_1D_ARRAY), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("PROXY_TEXTURE_1D_ARRAY").ToLocalChecked(), Nan::New<v8::Uint32>(GL_PROXY_TEXTURE_1D_ARRAY), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_2D_ARRAY").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_2D_ARRAY), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("PROXY_TEXTURE_2D_ARRAY").ToLocalChecked(), Nan::New<v8::Uint32>(GL_PROXY_TEXTURE_2D_ARRAY), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_BINDING_1D_ARRAY").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_BINDING_1D_ARRAY), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_BINDING_2D_ARRAY").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_BINDING_2D_ARRAY), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("R11F_G11F_B10F").ToLocalChecked(), Nan::New<v8::Uint32>(GL_R11F_G11F_B10F), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNSIGNED_INT_10F_11F_11F_REV").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNSIGNED_INT_10F_11F_11F_REV), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RGB9_E5").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RGB9_E5), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNSIGNED_INT_5_9_9_9_REV").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNSIGNED_INT_5_9_9_9_REV), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_SHARED_SIZE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_SHARED_SIZE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TRANSFORM_FEEDBACK_BUFFER_MODE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TRANSFORM_FEEDBACK_BUFFER_MODE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TRANSFORM_FEEDBACK_VARYINGS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TRANSFORM_FEEDBACK_VARYINGS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TRANSFORM_FEEDBACK_BUFFER_START").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TRANSFORM_FEEDBACK_BUFFER_START), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TRANSFORM_FEEDBACK_BUFFER_SIZE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TRANSFORM_FEEDBACK_BUFFER_SIZE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("PRIMITIVES_GENERATED").ToLocalChecked(), Nan::New<v8::Uint32>(GL_PRIMITIVES_GENERATED), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RASTERIZER_DISCARD").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RASTERIZER_DISCARD), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("INTERLEAVED_ATTRIBS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_INTERLEAVED_ATTRIBS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SEPARATE_ATTRIBS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SEPARATE_ATTRIBS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TRANSFORM_FEEDBACK_BUFFER").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TRANSFORM_FEEDBACK_BUFFER), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TRANSFORM_FEEDBACK_BUFFER_BINDING").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TRANSFORM_FEEDBACK_BUFFER_BINDING), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RGBA32UI").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RGBA32UI), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RGB32UI").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RGB32UI), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RGBA16UI").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RGBA16UI), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RGB16UI").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RGB16UI), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RGBA8UI").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RGBA8UI), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RGB8UI").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RGB8UI), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RGBA32I").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RGBA32I), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RGB32I").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RGB32I), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RGBA16I").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RGBA16I), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RGB16I").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RGB16I), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RGBA8I").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RGBA8I), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RGB8I").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RGB8I), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RED_INTEGER").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RED_INTEGER), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("GREEN_INTEGER").ToLocalChecked(), Nan::New<v8::Uint32>(GL_GREEN_INTEGER), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("BLUE_INTEGER").ToLocalChecked(), Nan::New<v8::Uint32>(GL_BLUE_INTEGER), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RGB_INTEGER").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RGB_INTEGER), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RGBA_INTEGER").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RGBA_INTEGER), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("BGR_INTEGER").ToLocalChecked(), Nan::New<v8::Uint32>(GL_BGR_INTEGER), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("BGRA_INTEGER").ToLocalChecked(), Nan::New<v8::Uint32>(GL_BGRA_INTEGER), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SAMPLER_1D_ARRAY").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SAMPLER_1D_ARRAY), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SAMPLER_2D_ARRAY").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SAMPLER_2D_ARRAY), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SAMPLER_1D_ARRAY_SHADOW").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SAMPLER_1D_ARRAY_SHADOW), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SAMPLER_2D_ARRAY_SHADOW").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SAMPLER_2D_ARRAY_SHADOW), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SAMPLER_CUBE_SHADOW").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SAMPLER_CUBE_SHADOW), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNSIGNED_INT_VEC2").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNSIGNED_INT_VEC2), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNSIGNED_INT_VEC3").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNSIGNED_INT_VEC3), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNSIGNED_INT_VEC4").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNSIGNED_INT_VEC4), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("INT_SAMPLER_1D").ToLocalChecked(), Nan::New<v8::Uint32>(GL_INT_SAMPLER_1D), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("INT_SAMPLER_2D").ToLocalChecked(), Nan::New<v8::Uint32>(GL_INT_SAMPLER_2D), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("INT_SAMPLER_3D").ToLocalChecked(), Nan::New<v8::Uint32>(GL_INT_SAMPLER_3D), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("INT_SAMPLER_CUBE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_INT_SAMPLER_CUBE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("INT_SAMPLER_1D_ARRAY").ToLocalChecked(), Nan::New<v8::Uint32>(GL_INT_SAMPLER_1D_ARRAY), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("INT_SAMPLER_2D_ARRAY").ToLocalChecked(), Nan::New<v8::Uint32>(GL_INT_SAMPLER_2D_ARRAY), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNSIGNED_INT_SAMPLER_1D").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNSIGNED_INT_SAMPLER_1D), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNSIGNED_INT_SAMPLER_2D").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNSIGNED_INT_SAMPLER_2D), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNSIGNED_INT_SAMPLER_3D").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNSIGNED_INT_SAMPLER_3D), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNSIGNED_INT_SAMPLER_CUBE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNSIGNED_INT_SAMPLER_CUBE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNSIGNED_INT_SAMPLER_1D_ARRAY").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNSIGNED_INT_SAMPLER_1D_ARRAY), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNSIGNED_INT_SAMPLER_2D_ARRAY").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNSIGNED_INT_SAMPLER_2D_ARRAY), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("QUERY_WAIT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_QUERY_WAIT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("QUERY_NO_WAIT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_QUERY_NO_WAIT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("QUERY_BY_REGION_WAIT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_QUERY_BY_REGION_WAIT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("QUERY_BY_REGION_NO_WAIT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_QUERY_BY_REGION_NO_WAIT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("BUFFER_ACCESS_FLAGS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_BUFFER_ACCESS_FLAGS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("BUFFER_MAP_LENGTH").ToLocalChecked(), Nan::New<v8::Uint32>(GL_BUFFER_MAP_LENGTH), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("BUFFER_MAP_OFFSET").ToLocalChecked(), Nan::New<v8::Uint32>(GL_BUFFER_MAP_OFFSET), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SAMPLER_2D_RECT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SAMPLER_2D_RECT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SAMPLER_2D_RECT_SHADOW").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SAMPLER_2D_RECT_SHADOW), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SAMPLER_BUFFER").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SAMPLER_BUFFER), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("INT_SAMPLER_2D_RECT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_INT_SAMPLER_2D_RECT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("INT_SAMPLER_BUFFER").ToLocalChecked(), Nan::New<v8::Uint32>(GL_INT_SAMPLER_BUFFER), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNSIGNED_INT_SAMPLER_2D_RECT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNSIGNED_INT_SAMPLER_2D_RECT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNSIGNED_INT_SAMPLER_BUFFER").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNSIGNED_INT_SAMPLER_BUFFER), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_BUFFER").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_BUFFER), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_TEXTURE_BUFFER_SIZE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_TEXTURE_BUFFER_SIZE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_BINDING_BUFFER").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_BINDING_BUFFER), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_BUFFER_DATA_STORE_BINDING").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_BUFFER_DATA_STORE_BINDING), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_RECTANGLE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_RECTANGLE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_BINDING_RECTANGLE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_BINDING_RECTANGLE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("PROXY_TEXTURE_RECTANGLE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_PROXY_TEXTURE_RECTANGLE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_RECTANGLE_TEXTURE_SIZE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_RECTANGLE_TEXTURE_SIZE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("R8_SNORM").ToLocalChecked(), Nan::New<v8::Uint32>(GL_R8_SNORM), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RG8_SNORM").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RG8_SNORM), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RGB8_SNORM").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RGB8_SNORM), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RGBA8_SNORM").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RGBA8_SNORM), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("R16_SNORM").ToLocalChecked(), Nan::New<v8::Uint32>(GL_R16_SNORM), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RG16_SNORM").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RG16_SNORM), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RGB16_SNORM").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RGB16_SNORM), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RGBA16_SNORM").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RGBA16_SNORM), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SIGNED_NORMALIZED").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SIGNED_NORMALIZED), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("PRIMITIVE_RESTART").ToLocalChecked(), Nan::New<v8::Uint32>(GL_PRIMITIVE_RESTART), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("PRIMITIVE_RESTART_INDEX").ToLocalChecked(), Nan::New<v8::Uint32>(GL_PRIMITIVE_RESTART_INDEX), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("CONTEXT_CORE_PROFILE_BIT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_CONTEXT_CORE_PROFILE_BIT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("CONTEXT_COMPATIBILITY_PROFILE_BIT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_CONTEXT_COMPATIBILITY_PROFILE_BIT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("LINES_ADJACENCY").ToLocalChecked(), Nan::New<v8::Uint32>(GL_LINES_ADJACENCY), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("LINE_STRIP_ADJACENCY").ToLocalChecked(), Nan::New<v8::Uint32>(GL_LINE_STRIP_ADJACENCY), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TRIANGLES_ADJACENCY").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TRIANGLES_ADJACENCY), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TRIANGLE_STRIP_ADJACENCY").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TRIANGLE_STRIP_ADJACENCY), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("PROGRAM_POINT_SIZE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_PROGRAM_POINT_SIZE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_GEOMETRY_TEXTURE_IMAGE_UNITS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("FRAMEBUFFER_ATTACHMENT_LAYERED").ToLocalChecked(), Nan::New<v8::Uint32>(GL_FRAMEBUFFER_ATTACHMENT_LAYERED), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("GEOMETRY_SHADER").ToLocalChecked(), Nan::New<v8::Uint32>(GL_GEOMETRY_SHADER), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("GEOMETRY_VERTICES_OUT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_GEOMETRY_VERTICES_OUT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("GEOMETRY_INPUT_TYPE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_GEOMETRY_INPUT_TYPE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("GEOMETRY_OUTPUT_TYPE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_GEOMETRY_OUTPUT_TYPE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_GEOMETRY_UNIFORM_COMPONENTS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_GEOMETRY_UNIFORM_COMPONENTS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_GEOMETRY_OUTPUT_VERTICES").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_GEOMETRY_OUTPUT_VERTICES), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_VERTEX_OUTPUT_COMPONENTS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_VERTEX_OUTPUT_COMPONENTS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_GEOMETRY_INPUT_COMPONENTS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_GEOMETRY_INPUT_COMPONENTS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_GEOMETRY_OUTPUT_COMPONENTS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_GEOMETRY_OUTPUT_COMPONENTS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_FRAGMENT_INPUT_COMPONENTS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_FRAGMENT_INPUT_COMPONENTS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("CONTEXT_PROFILE_MASK").ToLocalChecked(), Nan::New<v8::Uint32>(GL_CONTEXT_PROFILE_MASK), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("VERTEX_ATTRIB_ARRAY_DIVISOR").ToLocalChecked(), Nan::New<v8::Uint32>(GL_VERTEX_ATTRIB_ARRAY_DIVISOR), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SAMPLE_SHADING").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SAMPLE_SHADING), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MIN_SAMPLE_SHADING_VALUE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MIN_SAMPLE_SHADING_VALUE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MIN_PROGRAM_TEXTURE_GATHER_OFFSET").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MIN_PROGRAM_TEXTURE_GATHER_OFFSET), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_PROGRAM_TEXTURE_GATHER_OFFSET").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_PROGRAM_TEXTURE_GATHER_OFFSET), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_CUBE_MAP_ARRAY").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_CUBE_MAP_ARRAY), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_BINDING_CUBE_MAP_ARRAY").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_BINDING_CUBE_MAP_ARRAY), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("PROXY_TEXTURE_CUBE_MAP_ARRAY").ToLocalChecked(), Nan::New<v8::Uint32>(GL_PROXY_TEXTURE_CUBE_MAP_ARRAY), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SAMPLER_CUBE_MAP_ARRAY").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SAMPLER_CUBE_MAP_ARRAY), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SAMPLER_CUBE_MAP_ARRAY_SHADOW").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SAMPLER_CUBE_MAP_ARRAY_SHADOW), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("INT_SAMPLER_CUBE_MAP_ARRAY").ToLocalChecked(), Nan::New<v8::Uint32>(GL_INT_SAMPLER_CUBE_MAP_ARRAY), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNSIGNED_INT_SAMPLER_CUBE_MAP_ARRAY").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNSIGNED_INT_SAMPLER_CUBE_MAP_ARRAY), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DEPTH_COMPONENT32F").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DEPTH_COMPONENT32F), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DEPTH32F_STENCIL8").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DEPTH32F_STENCIL8), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("FLOAT_32_UNSIGNED_INT_24_8_REV").ToLocalChecked(), Nan::New<v8::Uint32>(GL_FLOAT_32_UNSIGNED_INT_24_8_REV), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("INVALID_FRAMEBUFFER_OPERATION").ToLocalChecked(), Nan::New<v8::Uint32>(GL_INVALID_FRAMEBUFFER_OPERATION), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING").ToLocalChecked(), Nan::New<v8::Uint32>(GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("FRAMEBUFFER_ATTACHMENT_RED_SIZE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("FRAMEBUFFER_ATTACHMENT_GREEN_SIZE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("FRAMEBUFFER_ATTACHMENT_BLUE_SIZE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("FRAMEBUFFER_DEFAULT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_FRAMEBUFFER_DEFAULT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("FRAMEBUFFER_UNDEFINED").ToLocalChecked(), Nan::New<v8::Uint32>(GL_FRAMEBUFFER_UNDEFINED), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DEPTH_STENCIL_ATTACHMENT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DEPTH_STENCIL_ATTACHMENT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_RENDERBUFFER_SIZE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_RENDERBUFFER_SIZE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DEPTH_STENCIL").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DEPTH_STENCIL), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNSIGNED_INT_24_8").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNSIGNED_INT_24_8), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DEPTH24_STENCIL8").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DEPTH24_STENCIL8), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_STENCIL_SIZE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_STENCIL_SIZE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_RED_TYPE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_RED_TYPE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_GREEN_TYPE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_GREEN_TYPE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_BLUE_TYPE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_BLUE_TYPE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_ALPHA_TYPE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_ALPHA_TYPE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_DEPTH_TYPE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_DEPTH_TYPE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNSIGNED_NORMALIZED").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNSIGNED_NORMALIZED), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("FRAMEBUFFER_BINDING").ToLocalChecked(), Nan::New<v8::Uint32>(GL_FRAMEBUFFER_BINDING), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RENDERBUFFER_BINDING").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RENDERBUFFER_BINDING), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("READ_FRAMEBUFFER").ToLocalChecked(), Nan::New<v8::Uint32>(GL_READ_FRAMEBUFFER), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DRAW_FRAMEBUFFER").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DRAW_FRAMEBUFFER), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("READ_FRAMEBUFFER_BINDING").ToLocalChecked(), Nan::New<v8::Uint32>(GL_READ_FRAMEBUFFER_BINDING), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RENDERBUFFER_SAMPLES").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RENDERBUFFER_SAMPLES), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("FRAMEBUFFER_ATTACHMENT_OBJECT_NAME").ToLocalChecked(), Nan::New<v8::Uint32>(GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL").ToLocalChecked(), Nan::New<v8::Uint32>(GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER").ToLocalChecked(), Nan::New<v8::Uint32>(GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("FRAMEBUFFER_COMPLETE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_FRAMEBUFFER_COMPLETE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("FRAMEBUFFER_INCOMPLETE_ATTACHMENT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER").ToLocalChecked(), Nan::New<v8::Uint32>(GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("FRAMEBUFFER_INCOMPLETE_READ_BUFFER").ToLocalChecked(), Nan::New<v8::Uint32>(GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("FRAMEBUFFER_UNSUPPORTED").ToLocalChecked(), Nan::New<v8::Uint32>(GL_FRAMEBUFFER_UNSUPPORTED), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_COLOR_ATTACHMENTS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_COLOR_ATTACHMENTS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("COLOR_ATTACHMENT0").ToLocalChecked(), Nan::New<v8::Uint32>(GL_COLOR_ATTACHMENT0), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("COLOR_ATTACHMENT1").ToLocalChecked(), Nan::New<v8::Uint32>(GL_COLOR_ATTACHMENT1), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("COLOR_ATTACHMENT2").ToLocalChecked(), Nan::New<v8::Uint32>(GL_COLOR_ATTACHMENT2), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("COLOR_ATTACHMENT3").ToLocalChecked(), Nan::New<v8::Uint32>(GL_COLOR_ATTACHMENT3), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("COLOR_ATTACHMENT4").ToLocalChecked(), Nan::New<v8::Uint32>(GL_COLOR_ATTACHMENT4), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("COLOR_ATTACHMENT5").ToLocalChecked(), Nan::New<v8::Uint32>(GL_COLOR_ATTACHMENT5), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("COLOR_ATTACHMENT6").ToLocalChecked(), Nan::New<v8::Uint32>(GL_COLOR_ATTACHMENT6), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("COLOR_ATTACHMENT7").ToLocalChecked(), Nan::New<v8::Uint32>(GL_COLOR_ATTACHMENT7), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("COLOR_ATTACHMENT8").ToLocalChecked(), Nan::New<v8::Uint32>(GL_COLOR_ATTACHMENT8), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("COLOR_ATTACHMENT9").ToLocalChecked(), Nan::New<v8::Uint32>(GL_COLOR_ATTACHMENT9), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("COLOR_ATTACHMENT10").ToLocalChecked(), Nan::New<v8::Uint32>(GL_COLOR_ATTACHMENT10), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("COLOR_ATTACHMENT11").ToLocalChecked(), Nan::New<v8::Uint32>(GL_COLOR_ATTACHMENT11), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("COLOR_ATTACHMENT12").ToLocalChecked(), Nan::New<v8::Uint32>(GL_COLOR_ATTACHMENT12), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("COLOR_ATTACHMENT13").ToLocalChecked(), Nan::New<v8::Uint32>(GL_COLOR_ATTACHMENT13), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("COLOR_ATTACHMENT14").ToLocalChecked(), Nan::New<v8::Uint32>(GL_COLOR_ATTACHMENT14), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("COLOR_ATTACHMENT15").ToLocalChecked(), Nan::New<v8::Uint32>(GL_COLOR_ATTACHMENT15), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DEPTH_ATTACHMENT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DEPTH_ATTACHMENT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("STENCIL_ATTACHMENT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_STENCIL_ATTACHMENT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("FRAMEBUFFER").ToLocalChecked(), Nan::New<v8::Uint32>(GL_FRAMEBUFFER), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RENDERBUFFER").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RENDERBUFFER), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RENDERBUFFER_WIDTH").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RENDERBUFFER_WIDTH), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RENDERBUFFER_HEIGHT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RENDERBUFFER_HEIGHT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RENDERBUFFER_INTERNAL_FORMAT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RENDERBUFFER_INTERNAL_FORMAT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("STENCIL_INDEX1").ToLocalChecked(), Nan::New<v8::Uint32>(GL_STENCIL_INDEX1), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("STENCIL_INDEX4").ToLocalChecked(), Nan::New<v8::Uint32>(GL_STENCIL_INDEX4), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("STENCIL_INDEX8").ToLocalChecked(), Nan::New<v8::Uint32>(GL_STENCIL_INDEX8), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("STENCIL_INDEX16").ToLocalChecked(), Nan::New<v8::Uint32>(GL_STENCIL_INDEX16), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RENDERBUFFER_RED_SIZE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RENDERBUFFER_RED_SIZE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RENDERBUFFER_GREEN_SIZE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RENDERBUFFER_GREEN_SIZE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RENDERBUFFER_BLUE_SIZE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RENDERBUFFER_BLUE_SIZE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RENDERBUFFER_ALPHA_SIZE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RENDERBUFFER_ALPHA_SIZE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RENDERBUFFER_DEPTH_SIZE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RENDERBUFFER_DEPTH_SIZE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RENDERBUFFER_STENCIL_SIZE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RENDERBUFFER_STENCIL_SIZE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("FRAMEBUFFER_INCOMPLETE_MULTISAMPLE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_SAMPLES").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_SAMPLES), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("FRAMEBUFFER_SRGB").ToLocalChecked(), Nan::New<v8::Uint32>(GL_FRAMEBUFFER_SRGB), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("HALF_FLOAT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_HALF_FLOAT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAP_READ_BIT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAP_READ_BIT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAP_WRITE_BIT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAP_WRITE_BIT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAP_INVALIDATE_RANGE_BIT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAP_INVALIDATE_RANGE_BIT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAP_INVALIDATE_BUFFER_BIT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAP_INVALIDATE_BUFFER_BIT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAP_FLUSH_EXPLICIT_BIT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAP_FLUSH_EXPLICIT_BIT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAP_UNSYNCHRONIZED_BIT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAP_UNSYNCHRONIZED_BIT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("COMPRESSED_RED_RGTC1").ToLocalChecked(), Nan::New<v8::Uint32>(GL_COMPRESSED_RED_RGTC1), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("COMPRESSED_SIGNED_RED_RGTC1").ToLocalChecked(), Nan::New<v8::Uint32>(GL_COMPRESSED_SIGNED_RED_RGTC1), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("COMPRESSED_RG_RGTC2").ToLocalChecked(), Nan::New<v8::Uint32>(GL_COMPRESSED_RG_RGTC2), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("COMPRESSED_SIGNED_RG_RGTC2").ToLocalChecked(), Nan::New<v8::Uint32>(GL_COMPRESSED_SIGNED_RG_RGTC2), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RG").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RG), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RG_INTEGER").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RG_INTEGER), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("R8").ToLocalChecked(), Nan::New<v8::Uint32>(GL_R8), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("R16").ToLocalChecked(), Nan::New<v8::Uint32>(GL_R16), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RG8").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RG8), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RG16").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RG16), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("R16F").ToLocalChecked(), Nan::New<v8::Uint32>(GL_R16F), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("R32F").ToLocalChecked(), Nan::New<v8::Uint32>(GL_R32F), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RG16F").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RG16F), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RG32F").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RG32F), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("R8I").ToLocalChecked(), Nan::New<v8::Uint32>(GL_R8I), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("R8UI").ToLocalChecked(), Nan::New<v8::Uint32>(GL_R8UI), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("R16I").ToLocalChecked(), Nan::New<v8::Uint32>(GL_R16I), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("R16UI").ToLocalChecked(), Nan::New<v8::Uint32>(GL_R16UI), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("R32I").ToLocalChecked(), Nan::New<v8::Uint32>(GL_R32I), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("R32UI").ToLocalChecked(), Nan::New<v8::Uint32>(GL_R32UI), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RG8I").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RG8I), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RG8UI").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RG8UI), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RG16I").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RG16I), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RG16UI").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RG16UI), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RG32I").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RG32I), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RG32UI").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RG32UI), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("VERTEX_ARRAY_BINDING").ToLocalChecked(), Nan::New<v8::Uint32>(GL_VERTEX_ARRAY_BINDING), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNIFORM_BUFFER").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNIFORM_BUFFER), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNIFORM_BUFFER_BINDING").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNIFORM_BUFFER_BINDING), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNIFORM_BUFFER_START").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNIFORM_BUFFER_START), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNIFORM_BUFFER_SIZE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNIFORM_BUFFER_SIZE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_VERTEX_UNIFORM_BLOCKS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_VERTEX_UNIFORM_BLOCKS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_GEOMETRY_UNIFORM_BLOCKS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_GEOMETRY_UNIFORM_BLOCKS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_FRAGMENT_UNIFORM_BLOCKS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_FRAGMENT_UNIFORM_BLOCKS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_COMBINED_UNIFORM_BLOCKS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_COMBINED_UNIFORM_BLOCKS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_UNIFORM_BUFFER_BINDINGS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_UNIFORM_BUFFER_BINDINGS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_UNIFORM_BLOCK_SIZE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_UNIFORM_BLOCK_SIZE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNIFORM_BUFFER_OFFSET_ALIGNMENT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH").ToLocalChecked(), Nan::New<v8::Uint32>(GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("ACTIVE_UNIFORM_BLOCKS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_ACTIVE_UNIFORM_BLOCKS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNIFORM_TYPE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNIFORM_TYPE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNIFORM_SIZE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNIFORM_SIZE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNIFORM_NAME_LENGTH").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNIFORM_NAME_LENGTH), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNIFORM_BLOCK_INDEX").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNIFORM_BLOCK_INDEX), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNIFORM_OFFSET").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNIFORM_OFFSET), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNIFORM_ARRAY_STRIDE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNIFORM_ARRAY_STRIDE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNIFORM_MATRIX_STRIDE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNIFORM_MATRIX_STRIDE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNIFORM_IS_ROW_MAJOR").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNIFORM_IS_ROW_MAJOR), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNIFORM_BLOCK_BINDING").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNIFORM_BLOCK_BINDING), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNIFORM_BLOCK_DATA_SIZE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNIFORM_BLOCK_DATA_SIZE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNIFORM_BLOCK_NAME_LENGTH").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNIFORM_BLOCK_NAME_LENGTH), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNIFORM_BLOCK_ACTIVE_UNIFORMS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNIFORM_BLOCK_REFERENCED_BY_GEOMETRY_SHADER").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNIFORM_BLOCK_REFERENCED_BY_GEOMETRY_SHADER), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("COPY_READ_BUFFER").ToLocalChecked(), Nan::New<v8::Uint32>(GL_COPY_READ_BUFFER), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("COPY_WRITE_BUFFER").ToLocalChecked(), Nan::New<v8::Uint32>(GL_COPY_WRITE_BUFFER), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DEPTH_CLAMP").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DEPTH_CLAMP), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("FIRST_VERTEX_CONVENTION").ToLocalChecked(), Nan::New<v8::Uint32>(GL_FIRST_VERTEX_CONVENTION), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("LAST_VERTEX_CONVENTION").ToLocalChecked(), Nan::New<v8::Uint32>(GL_LAST_VERTEX_CONVENTION), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("PROVOKING_VERTEX").ToLocalChecked(), Nan::New<v8::Uint32>(GL_PROVOKING_VERTEX), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_CUBE_MAP_SEAMLESS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_CUBE_MAP_SEAMLESS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_SERVER_WAIT_TIMEOUT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_SERVER_WAIT_TIMEOUT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("OBJECT_TYPE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_OBJECT_TYPE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SYNC_CONDITION").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SYNC_CONDITION), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SYNC_STATUS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SYNC_STATUS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SYNC_FLAGS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SYNC_FLAGS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SYNC_FENCE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SYNC_FENCE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SYNC_GPU_COMMANDS_COMPLETE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SYNC_GPU_COMMANDS_COMPLETE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNSIGNALED").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNSIGNALED), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SIGNALED").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SIGNALED), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("ALREADY_SIGNALED").ToLocalChecked(), Nan::New<v8::Uint32>(GL_ALREADY_SIGNALED), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TIMEOUT_EXPIRED").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TIMEOUT_EXPIRED), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("CONDITION_SATISFIED").ToLocalChecked(), Nan::New<v8::Uint32>(GL_CONDITION_SATISFIED), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("WAIT_FAILED").ToLocalChecked(), Nan::New<v8::Uint32>(GL_WAIT_FAILED), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SYNC_FLUSH_COMMANDS_BIT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SYNC_FLUSH_COMMANDS_BIT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SAMPLE_POSITION").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SAMPLE_POSITION), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SAMPLE_MASK").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SAMPLE_MASK), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SAMPLE_MASK_VALUE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SAMPLE_MASK_VALUE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_SAMPLE_MASK_WORDS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_SAMPLE_MASK_WORDS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_2D_MULTISAMPLE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_2D_MULTISAMPLE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("PROXY_TEXTURE_2D_MULTISAMPLE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_PROXY_TEXTURE_2D_MULTISAMPLE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_2D_MULTISAMPLE_ARRAY").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_2D_MULTISAMPLE_ARRAY), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("PROXY_TEXTURE_2D_MULTISAMPLE_ARRAY").ToLocalChecked(), Nan::New<v8::Uint32>(GL_PROXY_TEXTURE_2D_MULTISAMPLE_ARRAY), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_BINDING_2D_MULTISAMPLE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_BINDING_2D_MULTISAMPLE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_SAMPLES").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_SAMPLES), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_FIXED_SAMPLE_LOCATIONS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_FIXED_SAMPLE_LOCATIONS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SAMPLER_2D_MULTISAMPLE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SAMPLER_2D_MULTISAMPLE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("INT_SAMPLER_2D_MULTISAMPLE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_INT_SAMPLER_2D_MULTISAMPLE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SAMPLER_2D_MULTISAMPLE_ARRAY").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SAMPLER_2D_MULTISAMPLE_ARRAY), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("INT_SAMPLER_2D_MULTISAMPLE_ARRAY").ToLocalChecked(), Nan::New<v8::Uint32>(GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_COLOR_TEXTURE_SAMPLES").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_COLOR_TEXTURE_SAMPLES), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_DEPTH_TEXTURE_SAMPLES").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_DEPTH_TEXTURE_SAMPLES), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_INTEGER_SAMPLES").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_INTEGER_SAMPLES), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SRC1_COLOR").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SRC1_COLOR), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SRC1_ALPHA").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SRC1_ALPHA), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("ONE_MINUS_SRC1_COLOR").ToLocalChecked(), Nan::New<v8::Uint32>(GL_ONE_MINUS_SRC1_COLOR), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("ONE_MINUS_SRC1_ALPHA").ToLocalChecked(), Nan::New<v8::Uint32>(GL_ONE_MINUS_SRC1_ALPHA), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_DUAL_SOURCE_DRAW_BUFFERS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_DUAL_SOURCE_DRAW_BUFFERS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("ANY_SAMPLES_PASSED").ToLocalChecked(), Nan::New<v8::Uint32>(GL_ANY_SAMPLES_PASSED), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SAMPLER_BINDING").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SAMPLER_BINDING), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RGB10_A2UI").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RGB10_A2UI), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_SWIZZLE_R").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_SWIZZLE_R), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_SWIZZLE_G").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_SWIZZLE_G), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_SWIZZLE_B").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_SWIZZLE_B), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_SWIZZLE_A").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_SWIZZLE_A), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_SWIZZLE_RGBA").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_SWIZZLE_RGBA), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TIME_ELAPSED").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TIME_ELAPSED), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TIMESTAMP").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TIMESTAMP), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("INT_2_10_10_10_REV").ToLocalChecked(), Nan::New<v8::Uint32>(GL_INT_2_10_10_10_REV), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DRAW_INDIRECT_BUFFER").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DRAW_INDIRECT_BUFFER), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DRAW_INDIRECT_BUFFER_BINDING").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DRAW_INDIRECT_BUFFER_BINDING), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("GEOMETRY_SHADER_INVOCATIONS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_GEOMETRY_SHADER_INVOCATIONS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_GEOMETRY_SHADER_INVOCATIONS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_GEOMETRY_SHADER_INVOCATIONS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MIN_FRAGMENT_INTERPOLATION_OFFSET").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MIN_FRAGMENT_INTERPOLATION_OFFSET), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_FRAGMENT_INTERPOLATION_OFFSET").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_FRAGMENT_INTERPOLATION_OFFSET), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("FRAGMENT_INTERPOLATION_OFFSET_BITS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_FRAGMENT_INTERPOLATION_OFFSET_BITS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DOUBLE_VEC2").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DOUBLE_VEC2), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DOUBLE_VEC3").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DOUBLE_VEC3), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DOUBLE_VEC4").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DOUBLE_VEC4), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DOUBLE_MAT2").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DOUBLE_MAT2), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DOUBLE_MAT3").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DOUBLE_MAT3), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DOUBLE_MAT4").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DOUBLE_MAT4), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DOUBLE_MAT2x3").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DOUBLE_MAT2x3), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DOUBLE_MAT2x4").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DOUBLE_MAT2x4), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DOUBLE_MAT3x2").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DOUBLE_MAT3x2), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DOUBLE_MAT3x4").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DOUBLE_MAT3x4), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DOUBLE_MAT4x2").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DOUBLE_MAT4x2), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DOUBLE_MAT4x3").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DOUBLE_MAT4x3), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("ACTIVE_SUBROUTINES").ToLocalChecked(), Nan::New<v8::Uint32>(GL_ACTIVE_SUBROUTINES), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("ACTIVE_SUBROUTINE_UNIFORMS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_ACTIVE_SUBROUTINE_UNIFORMS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("ACTIVE_SUBROUTINE_UNIFORM_LOCATIONS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_ACTIVE_SUBROUTINE_UNIFORM_LOCATIONS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("ACTIVE_SUBROUTINE_MAX_LENGTH").ToLocalChecked(), Nan::New<v8::Uint32>(GL_ACTIVE_SUBROUTINE_MAX_LENGTH), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("ACTIVE_SUBROUTINE_UNIFORM_MAX_LENGTH").ToLocalChecked(), Nan::New<v8::Uint32>(GL_ACTIVE_SUBROUTINE_UNIFORM_MAX_LENGTH), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_SUBROUTINES").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_SUBROUTINES), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_SUBROUTINE_UNIFORM_LOCATIONS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_SUBROUTINE_UNIFORM_LOCATIONS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("NUM_COMPATIBLE_SUBROUTINES").ToLocalChecked(), Nan::New<v8::Uint32>(GL_NUM_COMPATIBLE_SUBROUTINES), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("COMPATIBLE_SUBROUTINES").ToLocalChecked(), Nan::New<v8::Uint32>(GL_COMPATIBLE_SUBROUTINES), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("PATCHES").ToLocalChecked(), Nan::New<v8::Uint32>(GL_PATCHES), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("PATCH_VERTICES").ToLocalChecked(), Nan::New<v8::Uint32>(GL_PATCH_VERTICES), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("PATCH_DEFAULT_INNER_LEVEL").ToLocalChecked(), Nan::New<v8::Uint32>(GL_PATCH_DEFAULT_INNER_LEVEL), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("PATCH_DEFAULT_OUTER_LEVEL").ToLocalChecked(), Nan::New<v8::Uint32>(GL_PATCH_DEFAULT_OUTER_LEVEL), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TESS_CONTROL_OUTPUT_VERTICES").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TESS_CONTROL_OUTPUT_VERTICES), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TESS_GEN_MODE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TESS_GEN_MODE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TESS_GEN_SPACING").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TESS_GEN_SPACING), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TESS_GEN_VERTEX_ORDER").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TESS_GEN_VERTEX_ORDER), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TESS_GEN_POINT_MODE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TESS_GEN_POINT_MODE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("QUADS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_QUADS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("ISOLINES").ToLocalChecked(), Nan::New<v8::Uint32>(GL_ISOLINES), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("FRACTIONAL_ODD").ToLocalChecked(), Nan::New<v8::Uint32>(GL_FRACTIONAL_ODD), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("FRACTIONAL_EVEN").ToLocalChecked(), Nan::New<v8::Uint32>(GL_FRACTIONAL_EVEN), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_PATCH_VERTICES").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_PATCH_VERTICES), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_TESS_GEN_LEVEL").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_TESS_GEN_LEVEL), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_TESS_CONTROL_UNIFORM_COMPONENTS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_TESS_CONTROL_UNIFORM_COMPONENTS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_TESS_EVALUATION_UNIFORM_COMPONENTS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_TESS_EVALUATION_UNIFORM_COMPONENTS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_TESS_CONTROL_TEXTURE_IMAGE_UNITS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_TESS_CONTROL_TEXTURE_IMAGE_UNITS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_TESS_EVALUATION_TEXTURE_IMAGE_UNITS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_TESS_EVALUATION_TEXTURE_IMAGE_UNITS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_TESS_CONTROL_OUTPUT_COMPONENTS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_TESS_CONTROL_OUTPUT_COMPONENTS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_TESS_PATCH_COMPONENTS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_TESS_PATCH_COMPONENTS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_TESS_CONTROL_TOTAL_OUTPUT_COMPONENTS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_TESS_CONTROL_TOTAL_OUTPUT_COMPONENTS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_TESS_EVALUATION_OUTPUT_COMPONENTS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_TESS_EVALUATION_OUTPUT_COMPONENTS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_TESS_CONTROL_UNIFORM_BLOCKS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_TESS_CONTROL_UNIFORM_BLOCKS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_TESS_EVALUATION_UNIFORM_BLOCKS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_TESS_EVALUATION_UNIFORM_BLOCKS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_TESS_CONTROL_INPUT_COMPONENTS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_TESS_CONTROL_INPUT_COMPONENTS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_TESS_EVALUATION_INPUT_COMPONENTS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_TESS_EVALUATION_INPUT_COMPONENTS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_COMBINED_TESS_CONTROL_UNIFORM_COMPONENTS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_COMBINED_TESS_CONTROL_UNIFORM_COMPONENTS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_COMBINED_TESS_EVALUATION_UNIFORM_COMPONENTS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_COMBINED_TESS_EVALUATION_UNIFORM_COMPONENTS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNIFORM_BLOCK_REFERENCED_BY_TESS_CONTROL_SHADER").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNIFORM_BLOCK_REFERENCED_BY_TESS_CONTROL_SHADER), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNIFORM_BLOCK_REFERENCED_BY_TESS_EVALUATION_SHADER").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNIFORM_BLOCK_REFERENCED_BY_TESS_EVALUATION_SHADER), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TESS_EVALUATION_SHADER").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TESS_EVALUATION_SHADER), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TESS_CONTROL_SHADER").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TESS_CONTROL_SHADER), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TRANSFORM_FEEDBACK").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TRANSFORM_FEEDBACK), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TRANSFORM_FEEDBACK_BUFFER_PAUSED").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TRANSFORM_FEEDBACK_BUFFER_PAUSED), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TRANSFORM_FEEDBACK_BUFFER_ACTIVE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TRANSFORM_FEEDBACK_BUFFER_ACTIVE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TRANSFORM_FEEDBACK_BINDING").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TRANSFORM_FEEDBACK_BINDING), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_TRANSFORM_FEEDBACK_BUFFERS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_TRANSFORM_FEEDBACK_BUFFERS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_VERTEX_STREAMS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_VERTEX_STREAMS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("FIXED").ToLocalChecked(), Nan::New<v8::Uint32>(GL_FIXED), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("IMPLEMENTATION_COLOR_READ_TYPE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_IMPLEMENTATION_COLOR_READ_TYPE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("IMPLEMENTATION_COLOR_READ_FORMAT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_IMPLEMENTATION_COLOR_READ_FORMAT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("LOW_FLOAT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_LOW_FLOAT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MEDIUM_FLOAT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MEDIUM_FLOAT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("HIGH_FLOAT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_HIGH_FLOAT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("LOW_INT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_LOW_INT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MEDIUM_INT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MEDIUM_INT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("HIGH_INT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_HIGH_INT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SHADER_COMPILER").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SHADER_COMPILER), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SHADER_BINARY_FORMATS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SHADER_BINARY_FORMATS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("NUM_SHADER_BINARY_FORMATS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_NUM_SHADER_BINARY_FORMATS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_VERTEX_UNIFORM_VECTORS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_VERTEX_UNIFORM_VECTORS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_VARYING_VECTORS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_VARYING_VECTORS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_FRAGMENT_UNIFORM_VECTORS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_FRAGMENT_UNIFORM_VECTORS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("RGB565").ToLocalChecked(), Nan::New<v8::Uint32>(GL_RGB565), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("PROGRAM_BINARY_RETRIEVABLE_HINT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_PROGRAM_BINARY_RETRIEVABLE_HINT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("PROGRAM_BINARY_LENGTH").ToLocalChecked(), Nan::New<v8::Uint32>(GL_PROGRAM_BINARY_LENGTH), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("NUM_PROGRAM_BINARY_FORMATS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_NUM_PROGRAM_BINARY_FORMATS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("PROGRAM_BINARY_FORMATS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_PROGRAM_BINARY_FORMATS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("VERTEX_SHADER_BIT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_VERTEX_SHADER_BIT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("FRAGMENT_SHADER_BIT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_FRAGMENT_SHADER_BIT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("GEOMETRY_SHADER_BIT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_GEOMETRY_SHADER_BIT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TESS_CONTROL_SHADER_BIT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TESS_CONTROL_SHADER_BIT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TESS_EVALUATION_SHADER_BIT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TESS_EVALUATION_SHADER_BIT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("ALL_SHADER_BITS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_ALL_SHADER_BITS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("PROGRAM_SEPARABLE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_PROGRAM_SEPARABLE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("ACTIVE_PROGRAM").ToLocalChecked(), Nan::New<v8::Uint32>(GL_ACTIVE_PROGRAM), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("PROGRAM_PIPELINE_BINDING").ToLocalChecked(), Nan::New<v8::Uint32>(GL_PROGRAM_PIPELINE_BINDING), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_VIEWPORTS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_VIEWPORTS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("VIEWPORT_SUBPIXEL_BITS").ToLocalChecked(), Nan::New<v8::Uint32>(GL_VIEWPORT_SUBPIXEL_BITS), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("VIEWPORT_BOUNDS_RANGE").ToLocalChecked(), Nan::New<v8::Uint32>(GL_VIEWPORT_BOUNDS_RANGE), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("LAYER_PROVOKING_VERTEX").ToLocalChecked(), Nan::New<v8::Uint32>(GL_LAYER_PROVOKING_VERTEX), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("VIEWPORT_INDEX_PROVOKING_VERTEX").ToLocalChecked(), Nan::New<v8::Uint32>(GL_VIEWPORT_INDEX_PROVOKING_VERTEX), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("UNDEFINED_VERTEX").ToLocalChecked(), Nan::New<v8::Uint32>(GL_UNDEFINED_VERTEX), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DEPTH_BOUNDS_TEST_EXT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DEPTH_BOUNDS_TEST_EXT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DEPTH_BOUNDS_EXT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DEPTH_BOUNDS_EXT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SCALED_RESOLVE_FASTEST_EXT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SCALED_RESOLVE_FASTEST_EXT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SCALED_RESOLVE_NICEST_EXT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SCALED_RESOLVE_NICEST_EXT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("COMPRESSED_RGB_S3TC_DXT1_EXT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_COMPRESSED_RGB_S3TC_DXT1_EXT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("COMPRESSED_RGBA_S3TC_DXT1_EXT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_COMPRESSED_RGBA_S3TC_DXT1_EXT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("COMPRESSED_RGBA_S3TC_DXT3_EXT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_COMPRESSED_RGBA_S3TC_DXT3_EXT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("COMPRESSED_RGBA_S3TC_DXT5_EXT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_COMPRESSED_RGBA_S3TC_DXT5_EXT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("COMPRESSED_SRGB_S3TC_DXT1_EXT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_COMPRESSED_SRGB_S3TC_DXT1_EXT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_MAX_ANISOTROPY_EXT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_MAX_ANISOTROPY_EXT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MAX_TEXTURE_MAX_ANISOTROPY_EXT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MIRROR_CLAMP_EXT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MIRROR_CLAMP_EXT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MIRROR_CLAMP_TO_EDGE_EXT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MIRROR_CLAMP_TO_EDGE_EXT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("MIRROR_CLAMP_TO_BORDER_EXT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_MIRROR_CLAMP_TO_BORDER_EXT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("TEXTURE_SRGB_DECODE_EXT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_TEXTURE_SRGB_DECODE_EXT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("DECODE_EXT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_DECODE_EXT), v8::ReadOnly);
    Nan::SetTemplate(exports, Nan::New<v8::String>("SKIP_DECODE_EXT").ToLocalChecked(), Nan::New<v8::Uint32>(GL_SKIP_DECODE_EXT), v8::ReadOnly);
}

#if (GL_DEPTH_BUFFER_BIT != 256)
 #error "GL3 constant DEPTH_BUFFER_BIT's value is incorrect."
#endif
#if (GL_STENCIL_BUFFER_BIT != 1024)
 #error "GL3 constant STENCIL_BUFFER_BIT's value is incorrect."
#endif
#if (GL_COLOR_BUFFER_BIT != 16384)
 #error "GL3 constant COLOR_BUFFER_BIT's value is incorrect."
#endif
#if (GL_FALSE != 0)
 #error "GL3 constant FALSE's value is incorrect."
#endif
#if (GL_TRUE != 1)
 #error "GL3 constant TRUE's value is incorrect."
#endif
#if (GL_POINTS != 0)
 #error "GL3 constant POINTS's value is incorrect."
#endif
#if (GL_LINES != 1)
 #error "GL3 constant LINES's value is incorrect."
#endif
#if (GL_LINE_LOOP != 2)
 #error "GL3 constant LINE_LOOP's value is incorrect."
#endif
#if (GL_LINE_STRIP != 3)
 #error "GL3 constant LINE_STRIP's value is incorrect."
#endif
#if (GL_TRIANGLES != 4)
 #error "GL3 constant TRIANGLES's value is incorrect."
#endif
#if (GL_TRIANGLE_STRIP != 5)
 #error "GL3 constant TRIANGLE_STRIP's value is incorrect."
#endif
#if (GL_TRIANGLE_FAN != 6)
 #error "GL3 constant TRIANGLE_FAN's value is incorrect."
#endif
#if (GL_NEVER != 512)
 #error "GL3 constant NEVER's value is incorrect."
#endif
#if (GL_LESS != 513)
 #error "GL3 constant LESS's value is incorrect."
#endif
#if (GL_EQUAL != 514)
 #error "GL3 constant EQUAL's value is incorrect."
#endif
#if (GL_LEQUAL != 515)
 #error "GL3 constant LEQUAL's value is incorrect."
#endif
#if (GL_GREATER != 516)
 #error "GL3 constant GREATER's value is incorrect."
#endif
#if (GL_NOTEQUAL != 517)
 #error "GL3 constant NOTEQUAL's value is incorrect."
#endif
#if (GL_GEQUAL != 518)
 #error "GL3 constant GEQUAL's value is incorrect."
#endif
#if (GL_ALWAYS != 519)
 #error "GL3 constant ALWAYS's value is incorrect."
#endif
#if (GL_ZERO != 0)
 #error "GL3 constant ZERO's value is incorrect."
#endif
#if (GL_ONE != 1)
 #error "GL3 constant ONE's value is incorrect."
#endif
#if (GL_SRC_COLOR != 768)
 #error "GL3 constant SRC_COLOR's value is incorrect."
#endif
#if (GL_ONE_MINUS_SRC_COLOR != 769)
 #error "GL3 constant ONE_MINUS_SRC_COLOR's value is incorrect."
#endif
#if (GL_SRC_ALPHA != 770)
 #error "GL3 constant SRC_ALPHA's value is incorrect."
#endif
#if (GL_ONE_MINUS_SRC_ALPHA != 771)
 #error "GL3 constant ONE_MINUS_SRC_ALPHA's value is incorrect."
#endif
#if (GL_DST_ALPHA != 772)
 #error "GL3 constant DST_ALPHA's value is incorrect."
#endif
#if (GL_ONE_MINUS_DST_ALPHA != 773)
 #error "GL3 constant ONE_MINUS_DST_ALPHA's value is incorrect."
#endif
#if (GL_DST_COLOR != 774)
 #error "GL3 constant DST_COLOR's value is incorrect."
#endif
#if (GL_ONE_MINUS_DST_COLOR != 775)
 #error "GL3 constant ONE_MINUS_DST_COLOR's value is incorrect."
#endif
#if (GL_SRC_ALPHA_SATURATE != 776)
 #error "GL3 constant SRC_ALPHA_SATURATE's value is incorrect."
#endif
#if (GL_NONE != 0)
 #error "GL3 constant NONE's value is incorrect."
#endif
#if (GL_FRONT_LEFT != 1024)
 #error "GL3 constant FRONT_LEFT's value is incorrect."
#endif
#if (GL_FRONT_RIGHT != 1025)
 #error "GL3 constant FRONT_RIGHT's value is incorrect."
#endif
#if (GL_BACK_LEFT != 1026)
 #error "GL3 constant BACK_LEFT's value is incorrect."
#endif
#if (GL_BACK_RIGHT != 1027)
 #error "GL3 constant BACK_RIGHT's value is incorrect."
#endif
#if (GL_FRONT != 1028)
 #error "GL3 constant FRONT's value is incorrect."
#endif
#if (GL_BACK != 1029)
 #error "GL3 constant BACK's value is incorrect."
#endif
#if (GL_LEFT != 1030)
 #error "GL3 constant LEFT's value is incorrect."
#endif
#if (GL_RIGHT != 1031)
 #error "GL3 constant RIGHT's value is incorrect."
#endif
#if (GL_FRONT_AND_BACK != 1032)
 #error "GL3 constant FRONT_AND_BACK's value is incorrect."
#endif
#if (GL_NO_ERROR != 0)
 #error "GL3 constant NO_ERROR's value is incorrect."
#endif
#if (GL_INVALID_ENUM != 1280)
 #error "GL3 constant INVALID_ENUM's value is incorrect."
#endif
#if (GL_INVALID_VALUE != 1281)
 #error "GL3 constant INVALID_VALUE's value is incorrect."
#endif
#if (GL_INVALID_OPERATION != 1282)
 #error "GL3 constant INVALID_OPERATION's value is incorrect."
#endif
#if (GL_OUT_OF_MEMORY != 1285)
 #error "GL3 constant OUT_OF_MEMORY's value is incorrect."
#endif
#if (GL_CW != 2304)
 #error "GL3 constant CW's value is incorrect."
#endif
#if (GL_CCW != 2305)
 #error "GL3 constant CCW's value is incorrect."
#endif
#if (GL_POINT_SIZE != 2833)
 #error "GL3 constant POINT_SIZE's value is incorrect."
#endif
#if (GL_POINT_SIZE_RANGE != 2834)
 #error "GL3 constant POINT_SIZE_RANGE's value is incorrect."
#endif
#if (GL_POINT_SIZE_GRANULARITY != 2835)
 #error "GL3 constant POINT_SIZE_GRANULARITY's value is incorrect."
#endif
#if (GL_LINE_SMOOTH != 2848)
 #error "GL3 constant LINE_SMOOTH's value is incorrect."
#endif
#if (GL_LINE_WIDTH != 2849)
 #error "GL3 constant LINE_WIDTH's value is incorrect."
#endif
#if (GL_LINE_WIDTH_RANGE != 2850)
 #error "GL3 constant LINE_WIDTH_RANGE's value is incorrect."
#endif
#if (GL_LINE_WIDTH_GRANULARITY != 2851)
 #error "GL3 constant LINE_WIDTH_GRANULARITY's value is incorrect."
#endif
#if (GL_POLYGON_MODE != 2880)
 #error "GL3 constant POLYGON_MODE's value is incorrect."
#endif
#if (GL_POLYGON_SMOOTH != 2881)
 #error "GL3 constant POLYGON_SMOOTH's value is incorrect."
#endif
#if (GL_CULL_FACE != 2884)
 #error "GL3 constant CULL_FACE's value is incorrect."
#endif
#if (GL_CULL_FACE_MODE != 2885)
 #error "GL3 constant CULL_FACE_MODE's value is incorrect."
#endif
#if (GL_FRONT_FACE != 2886)
 #error "GL3 constant FRONT_FACE's value is incorrect."
#endif
#if (GL_DEPTH_RANGE != 2928)
 #error "GL3 constant DEPTH_RANGE's value is incorrect."
#endif
#if (GL_DEPTH_TEST != 2929)
 #error "GL3 constant DEPTH_TEST's value is incorrect."
#endif
#if (GL_DEPTH_WRITEMASK != 2930)
 #error "GL3 constant DEPTH_WRITEMASK's value is incorrect."
#endif
#if (GL_DEPTH_CLEAR_VALUE != 2931)
 #error "GL3 constant DEPTH_CLEAR_VALUE's value is incorrect."
#endif
#if (GL_DEPTH_FUNC != 2932)
 #error "GL3 constant DEPTH_FUNC's value is incorrect."
#endif
#if (GL_STENCIL_TEST != 2960)
 #error "GL3 constant STENCIL_TEST's value is incorrect."
#endif
#if (GL_STENCIL_CLEAR_VALUE != 2961)
 #error "GL3 constant STENCIL_CLEAR_VALUE's value is incorrect."
#endif
#if (GL_STENCIL_FUNC != 2962)
 #error "GL3 constant STENCIL_FUNC's value is incorrect."
#endif
#if (GL_STENCIL_VALUE_MASK != 2963)
 #error "GL3 constant STENCIL_VALUE_MASK's value is incorrect."
#endif
#if (GL_STENCIL_FAIL != 2964)
 #error "GL3 constant STENCIL_FAIL's value is incorrect."
#endif
#if (GL_STENCIL_PASS_DEPTH_FAIL != 2965)
 #error "GL3 constant STENCIL_PASS_DEPTH_FAIL's value is incorrect."
#endif
#if (GL_STENCIL_PASS_DEPTH_PASS != 2966)
 #error "GL3 constant STENCIL_PASS_DEPTH_PASS's value is incorrect."
#endif
#if (GL_STENCIL_REF != 2967)
 #error "GL3 constant STENCIL_REF's value is incorrect."
#endif
#if (GL_STENCIL_WRITEMASK != 2968)
 #error "GL3 constant STENCIL_WRITEMASK's value is incorrect."
#endif
#if (GL_VIEWPORT != 2978)
 #error "GL3 constant VIEWPORT's value is incorrect."
#endif
#if (GL_DITHER != 3024)
 #error "GL3 constant DITHER's value is incorrect."
#endif
#if (GL_BLEND_DST != 3040)
 #error "GL3 constant BLEND_DST's value is incorrect."
#endif
#if (GL_BLEND_SRC != 3041)
 #error "GL3 constant BLEND_SRC's value is incorrect."
#endif
#if (GL_BLEND != 3042)
 #error "GL3 constant BLEND's value is incorrect."
#endif
#if (GL_LOGIC_OP_MODE != 3056)
 #error "GL3 constant LOGIC_OP_MODE's value is incorrect."
#endif
#if (GL_COLOR_LOGIC_OP != 3058)
 #error "GL3 constant COLOR_LOGIC_OP's value is incorrect."
#endif
#if (GL_DRAW_BUFFER != 3073)
 #error "GL3 constant DRAW_BUFFER's value is incorrect."
#endif
#if (GL_READ_BUFFER != 3074)
 #error "GL3 constant READ_BUFFER's value is incorrect."
#endif
#if (GL_SCISSOR_BOX != 3088)
 #error "GL3 constant SCISSOR_BOX's value is incorrect."
#endif
#if (GL_SCISSOR_TEST != 3089)
 #error "GL3 constant SCISSOR_TEST's value is incorrect."
#endif
#if (GL_COLOR_CLEAR_VALUE != 3106)
 #error "GL3 constant COLOR_CLEAR_VALUE's value is incorrect."
#endif
#if (GL_COLOR_WRITEMASK != 3107)
 #error "GL3 constant COLOR_WRITEMASK's value is incorrect."
#endif
#if (GL_DOUBLEBUFFER != 3122)
 #error "GL3 constant DOUBLEBUFFER's value is incorrect."
#endif
#if (GL_STEREO != 3123)
 #error "GL3 constant STEREO's value is incorrect."
#endif
#if (GL_LINE_SMOOTH_HINT != 3154)
 #error "GL3 constant LINE_SMOOTH_HINT's value is incorrect."
#endif
#if (GL_POLYGON_SMOOTH_HINT != 3155)
 #error "GL3 constant POLYGON_SMOOTH_HINT's value is incorrect."
#endif
#if (GL_UNPACK_SWAP_BYTES != 3312)
 #error "GL3 constant UNPACK_SWAP_BYTES's value is incorrect."
#endif
#if (GL_UNPACK_LSB_FIRST != 3313)
 #error "GL3 constant UNPACK_LSB_FIRST's value is incorrect."
#endif
#if (GL_UNPACK_ROW_LENGTH != 3314)
 #error "GL3 constant UNPACK_ROW_LENGTH's value is incorrect."
#endif
#if (GL_UNPACK_SKIP_ROWS != 3315)
 #error "GL3 constant UNPACK_SKIP_ROWS's value is incorrect."
#endif
#if (GL_UNPACK_SKIP_PIXELS != 3316)
 #error "GL3 constant UNPACK_SKIP_PIXELS's value is incorrect."
#endif
#if (GL_UNPACK_ALIGNMENT != 3317)
 #error "GL3 constant UNPACK_ALIGNMENT's value is incorrect."
#endif
#if (GL_PACK_SWAP_BYTES != 3328)
 #error "GL3 constant PACK_SWAP_BYTES's value is incorrect."
#endif
#if (GL_PACK_LSB_FIRST != 3329)
 #error "GL3 constant PACK_LSB_FIRST's value is incorrect."
#endif
#if (GL_PACK_ROW_LENGTH != 3330)
 #error "GL3 constant PACK_ROW_LENGTH's value is incorrect."
#endif
#if (GL_PACK_SKIP_ROWS != 3331)
 #error "GL3 constant PACK_SKIP_ROWS's value is incorrect."
#endif
#if (GL_PACK_SKIP_PIXELS != 3332)
 #error "GL3 constant PACK_SKIP_PIXELS's value is incorrect."
#endif
#if (GL_PACK_ALIGNMENT != 3333)
 #error "GL3 constant PACK_ALIGNMENT's value is incorrect."
#endif
#if (GL_MAX_TEXTURE_SIZE != 3379)
 #error "GL3 constant MAX_TEXTURE_SIZE's value is incorrect."
#endif
#if (GL_MAX_VIEWPORT_DIMS != 3386)
 #error "GL3 constant MAX_VIEWPORT_DIMS's value is incorrect."
#endif
#if (GL_SUBPIXEL_BITS != 3408)
 #error "GL3 constant SUBPIXEL_BITS's value is incorrect."
#endif
#if (GL_TEXTURE_1D != 3552)
 #error "GL3 constant TEXTURE_1D's value is incorrect."
#endif
#if (GL_TEXTURE_2D != 3553)
 #error "GL3 constant TEXTURE_2D's value is incorrect."
#endif
#if (GL_POLYGON_OFFSET_UNITS != 10752)
 #error "GL3 constant POLYGON_OFFSET_UNITS's value is incorrect."
#endif
#if (GL_POLYGON_OFFSET_POINT != 10753)
 #error "GL3 constant POLYGON_OFFSET_POINT's value is incorrect."
#endif
#if (GL_POLYGON_OFFSET_LINE != 10754)
 #error "GL3 constant POLYGON_OFFSET_LINE's value is incorrect."
#endif
#if (GL_POLYGON_OFFSET_FILL != 32823)
 #error "GL3 constant POLYGON_OFFSET_FILL's value is incorrect."
#endif
#if (GL_POLYGON_OFFSET_FACTOR != 32824)
 #error "GL3 constant POLYGON_OFFSET_FACTOR's value is incorrect."
#endif
#if (GL_TEXTURE_BINDING_1D != 32872)
 #error "GL3 constant TEXTURE_BINDING_1D's value is incorrect."
#endif
#if (GL_TEXTURE_BINDING_2D != 32873)
 #error "GL3 constant TEXTURE_BINDING_2D's value is incorrect."
#endif
#if (GL_TEXTURE_WIDTH != 4096)
 #error "GL3 constant TEXTURE_WIDTH's value is incorrect."
#endif
#if (GL_TEXTURE_HEIGHT != 4097)
 #error "GL3 constant TEXTURE_HEIGHT's value is incorrect."
#endif
#if (GL_TEXTURE_INTERNAL_FORMAT != 4099)
 #error "GL3 constant TEXTURE_INTERNAL_FORMAT's value is incorrect."
#endif
#if (GL_TEXTURE_BORDER_COLOR != 4100)
 #error "GL3 constant TEXTURE_BORDER_COLOR's value is incorrect."
#endif
#if (GL_TEXTURE_RED_SIZE != 32860)
 #error "GL3 constant TEXTURE_RED_SIZE's value is incorrect."
#endif
#if (GL_TEXTURE_GREEN_SIZE != 32861)
 #error "GL3 constant TEXTURE_GREEN_SIZE's value is incorrect."
#endif
#if (GL_TEXTURE_BLUE_SIZE != 32862)
 #error "GL3 constant TEXTURE_BLUE_SIZE's value is incorrect."
#endif
#if (GL_TEXTURE_ALPHA_SIZE != 32863)
 #error "GL3 constant TEXTURE_ALPHA_SIZE's value is incorrect."
#endif
#if (GL_DONT_CARE != 4352)
 #error "GL3 constant DONT_CARE's value is incorrect."
#endif
#if (GL_FASTEST != 4353)
 #error "GL3 constant FASTEST's value is incorrect."
#endif
#if (GL_NICEST != 4354)
 #error "GL3 constant NICEST's value is incorrect."
#endif
#if (GL_BYTE != 5120)
 #error "GL3 constant BYTE's value is incorrect."
#endif
#if (GL_UNSIGNED_BYTE != 5121)
 #error "GL3 constant UNSIGNED_BYTE's value is incorrect."
#endif
#if (GL_SHORT != 5122)
 #error "GL3 constant SHORT's value is incorrect."
#endif
#if (GL_UNSIGNED_SHORT != 5123)
 #error "GL3 constant UNSIGNED_SHORT's value is incorrect."
#endif
#if (GL_INT != 5124)
 #error "GL3 constant INT's value is incorrect."
#endif
#if (GL_UNSIGNED_INT != 5125)
 #error "GL3 constant UNSIGNED_INT's value is incorrect."
#endif
#if (GL_FLOAT != 5126)
 #error "GL3 constant FLOAT's value is incorrect."
#endif
#if (GL_DOUBLE != 5130)
 #error "GL3 constant DOUBLE's value is incorrect."
#endif
#if (GL_CLEAR != 5376)
 #error "GL3 constant CLEAR's value is incorrect."
#endif
#if (GL_AND != 5377)
 #error "GL3 constant AND's value is incorrect."
#endif
#if (GL_AND_REVERSE != 5378)
 #error "GL3 constant AND_REVERSE's value is incorrect."
#endif
#if (GL_COPY != 5379)
 #error "GL3 constant COPY's value is incorrect."
#endif
#if (GL_AND_INVERTED != 5380)
 #error "GL3 constant AND_INVERTED's value is incorrect."
#endif
#if (GL_NOOP != 5381)
 #error "GL3 constant NOOP's value is incorrect."
#endif
#if (GL_XOR != 5382)
 #error "GL3 constant XOR's value is incorrect."
#endif
#if (GL_OR != 5383)
 #error "GL3 constant OR's value is incorrect."
#endif
#if (GL_NOR != 5384)
 #error "GL3 constant NOR's value is incorrect."
#endif
#if (GL_EQUIV != 5385)
 #error "GL3 constant EQUIV's value is incorrect."
#endif
#if (GL_INVERT != 5386)
 #error "GL3 constant INVERT's value is incorrect."
#endif
#if (GL_OR_REVERSE != 5387)
 #error "GL3 constant OR_REVERSE's value is incorrect."
#endif
#if (GL_COPY_INVERTED != 5388)
 #error "GL3 constant COPY_INVERTED's value is incorrect."
#endif
#if (GL_OR_INVERTED != 5389)
 #error "GL3 constant OR_INVERTED's value is incorrect."
#endif
#if (GL_NAND != 5390)
 #error "GL3 constant NAND's value is incorrect."
#endif
#if (GL_SET != 5391)
 #error "GL3 constant SET's value is incorrect."
#endif
#if (GL_TEXTURE != 5890)
 #error "GL3 constant TEXTURE's value is incorrect."
#endif
#if (GL_COLOR != 6144)
 #error "GL3 constant COLOR's value is incorrect."
#endif
#if (GL_DEPTH != 6145)
 #error "GL3 constant DEPTH's value is incorrect."
#endif
#if (GL_STENCIL != 6146)
 #error "GL3 constant STENCIL's value is incorrect."
#endif
#if (GL_STENCIL_INDEX != 6401)
 #error "GL3 constant STENCIL_INDEX's value is incorrect."
#endif
#if (GL_DEPTH_COMPONENT != 6402)
 #error "GL3 constant DEPTH_COMPONENT's value is incorrect."
#endif
#if (GL_RED != 6403)
 #error "GL3 constant RED's value is incorrect."
#endif
#if (GL_GREEN != 6404)
 #error "GL3 constant GREEN's value is incorrect."
#endif
#if (GL_BLUE != 6405)
 #error "GL3 constant BLUE's value is incorrect."
#endif
#if (GL_ALPHA != 6406)
 #error "GL3 constant ALPHA's value is incorrect."
#endif
#if (GL_RGB != 6407)
 #error "GL3 constant RGB's value is incorrect."
#endif
#if (GL_RGBA != 6408)
 #error "GL3 constant RGBA's value is incorrect."
#endif
#if (GL_POINT != 6912)
 #error "GL3 constant POINT's value is incorrect."
#endif
#if (GL_LINE != 6913)
 #error "GL3 constant LINE's value is incorrect."
#endif
#if (GL_FILL != 6914)
 #error "GL3 constant FILL's value is incorrect."
#endif
#if (GL_KEEP != 7680)
 #error "GL3 constant KEEP's value is incorrect."
#endif
#if (GL_REPLACE != 7681)
 #error "GL3 constant REPLACE's value is incorrect."
#endif
#if (GL_INCR != 7682)
 #error "GL3 constant INCR's value is incorrect."
#endif
#if (GL_DECR != 7683)
 #error "GL3 constant DECR's value is incorrect."
#endif
#if (GL_VENDOR != 7936)
 #error "GL3 constant VENDOR's value is incorrect."
#endif
#if (GL_RENDERER != 7937)
 #error "GL3 constant RENDERER's value is incorrect."
#endif
#if (GL_VERSION != 7938)
 #error "GL3 constant VERSION's value is incorrect."
#endif
#if (GL_EXTENSIONS != 7939)
 #error "GL3 constant EXTENSIONS's value is incorrect."
#endif
#if (GL_NEAREST != 9728)
 #error "GL3 constant NEAREST's value is incorrect."
#endif
#if (GL_LINEAR != 9729)
 #error "GL3 constant LINEAR's value is incorrect."
#endif
#if (GL_NEAREST_MIPMAP_NEAREST != 9984)
 #error "GL3 constant NEAREST_MIPMAP_NEAREST's value is incorrect."
#endif
#if (GL_LINEAR_MIPMAP_NEAREST != 9985)
 #error "GL3 constant LINEAR_MIPMAP_NEAREST's value is incorrect."
#endif
#if (GL_NEAREST_MIPMAP_LINEAR != 9986)
 #error "GL3 constant NEAREST_MIPMAP_LINEAR's value is incorrect."
#endif
#if (GL_LINEAR_MIPMAP_LINEAR != 9987)
 #error "GL3 constant LINEAR_MIPMAP_LINEAR's value is incorrect."
#endif
#if (GL_TEXTURE_MAG_FILTER != 10240)
 #error "GL3 constant TEXTURE_MAG_FILTER's value is incorrect."
#endif
#if (GL_TEXTURE_MIN_FILTER != 10241)
 #error "GL3 constant TEXTURE_MIN_FILTER's value is incorrect."
#endif
#if (GL_TEXTURE_WRAP_S != 10242)
 #error "GL3 constant TEXTURE_WRAP_S's value is incorrect."
#endif
#if (GL_TEXTURE_WRAP_T != 10243)
 #error "GL3 constant TEXTURE_WRAP_T's value is incorrect."
#endif
#if (GL_PROXY_TEXTURE_1D != 32867)
 #error "GL3 constant PROXY_TEXTURE_1D's value is incorrect."
#endif
#if (GL_PROXY_TEXTURE_2D != 32868)
 #error "GL3 constant PROXY_TEXTURE_2D's value is incorrect."
#endif
#if (GL_REPEAT != 10497)
 #error "GL3 constant REPEAT's value is incorrect."
#endif
#if (GL_R3_G3_B2 != 10768)
 #error "GL3 constant R3_G3_B2's value is incorrect."
#endif
#if (GL_RGB4 != 32847)
 #error "GL3 constant RGB4's value is incorrect."
#endif
#if (GL_RGB5 != 32848)
 #error "GL3 constant RGB5's value is incorrect."
#endif
#if (GL_RGB8 != 32849)
 #error "GL3 constant RGB8's value is incorrect."
#endif
#if (GL_RGB10 != 32850)
 #error "GL3 constant RGB10's value is incorrect."
#endif
#if (GL_RGB12 != 32851)
 #error "GL3 constant RGB12's value is incorrect."
#endif
#if (GL_RGB16 != 32852)
 #error "GL3 constant RGB16's value is incorrect."
#endif
#if (GL_RGBA2 != 32853)
 #error "GL3 constant RGBA2's value is incorrect."
#endif
#if (GL_RGBA4 != 32854)
 #error "GL3 constant RGBA4's value is incorrect."
#endif
#if (GL_RGB5_A1 != 32855)
 #error "GL3 constant RGB5_A1's value is incorrect."
#endif
#if (GL_RGBA8 != 32856)
 #error "GL3 constant RGBA8's value is incorrect."
#endif
#if (GL_RGB10_A2 != 32857)
 #error "GL3 constant RGB10_A2's value is incorrect."
#endif
#if (GL_RGBA12 != 32858)
 #error "GL3 constant RGBA12's value is incorrect."
#endif
#if (GL_RGBA16 != 32859)
 #error "GL3 constant RGBA16's value is incorrect."
#endif
#if (GL_UNSIGNED_BYTE_3_3_2 != 32818)
 #error "GL3 constant UNSIGNED_BYTE_3_3_2's value is incorrect."
#endif
#if (GL_UNSIGNED_SHORT_4_4_4_4 != 32819)
 #error "GL3 constant UNSIGNED_SHORT_4_4_4_4's value is incorrect."
#endif
#if (GL_UNSIGNED_SHORT_5_5_5_1 != 32820)
 #error "GL3 constant UNSIGNED_SHORT_5_5_5_1's value is incorrect."
#endif
#if (GL_UNSIGNED_INT_8_8_8_8 != 32821)
 #error "GL3 constant UNSIGNED_INT_8_8_8_8's value is incorrect."
#endif
#if (GL_UNSIGNED_INT_10_10_10_2 != 32822)
 #error "GL3 constant UNSIGNED_INT_10_10_10_2's value is incorrect."
#endif
#if (GL_TEXTURE_BINDING_3D != 32874)
 #error "GL3 constant TEXTURE_BINDING_3D's value is incorrect."
#endif
#if (GL_PACK_SKIP_IMAGES != 32875)
 #error "GL3 constant PACK_SKIP_IMAGES's value is incorrect."
#endif
#if (GL_PACK_IMAGE_HEIGHT != 32876)
 #error "GL3 constant PACK_IMAGE_HEIGHT's value is incorrect."
#endif
#if (GL_UNPACK_SKIP_IMAGES != 32877)
 #error "GL3 constant UNPACK_SKIP_IMAGES's value is incorrect."
#endif
#if (GL_UNPACK_IMAGE_HEIGHT != 32878)
 #error "GL3 constant UNPACK_IMAGE_HEIGHT's value is incorrect."
#endif
#if (GL_TEXTURE_3D != 32879)
 #error "GL3 constant TEXTURE_3D's value is incorrect."
#endif
#if (GL_PROXY_TEXTURE_3D != 32880)
 #error "GL3 constant PROXY_TEXTURE_3D's value is incorrect."
#endif
#if (GL_TEXTURE_DEPTH != 32881)
 #error "GL3 constant TEXTURE_DEPTH's value is incorrect."
#endif
#if (GL_TEXTURE_WRAP_R != 32882)
 #error "GL3 constant TEXTURE_WRAP_R's value is incorrect."
#endif
#if (GL_MAX_3D_TEXTURE_SIZE != 32883)
 #error "GL3 constant MAX_3D_TEXTURE_SIZE's value is incorrect."
#endif
#if (GL_UNSIGNED_BYTE_2_3_3_REV != 33634)
 #error "GL3 constant UNSIGNED_BYTE_2_3_3_REV's value is incorrect."
#endif
#if (GL_UNSIGNED_SHORT_5_6_5 != 33635)
 #error "GL3 constant UNSIGNED_SHORT_5_6_5's value is incorrect."
#endif
#if (GL_UNSIGNED_SHORT_5_6_5_REV != 33636)
 #error "GL3 constant UNSIGNED_SHORT_5_6_5_REV's value is incorrect."
#endif
#if (GL_UNSIGNED_SHORT_4_4_4_4_REV != 33637)
 #error "GL3 constant UNSIGNED_SHORT_4_4_4_4_REV's value is incorrect."
#endif
#if (GL_UNSIGNED_SHORT_1_5_5_5_REV != 33638)
 #error "GL3 constant UNSIGNED_SHORT_1_5_5_5_REV's value is incorrect."
#endif
#if (GL_UNSIGNED_INT_8_8_8_8_REV != 33639)
 #error "GL3 constant UNSIGNED_INT_8_8_8_8_REV's value is incorrect."
#endif
#if (GL_UNSIGNED_INT_2_10_10_10_REV != 33640)
 #error "GL3 constant UNSIGNED_INT_2_10_10_10_REV's value is incorrect."
#endif
#if (GL_BGR != 32992)
 #error "GL3 constant BGR's value is incorrect."
#endif
#if (GL_BGRA != 32993)
 #error "GL3 constant BGRA's value is incorrect."
#endif
#if (GL_MAX_ELEMENTS_VERTICES != 33000)
 #error "GL3 constant MAX_ELEMENTS_VERTICES's value is incorrect."
#endif
#if (GL_MAX_ELEMENTS_INDICES != 33001)
 #error "GL3 constant MAX_ELEMENTS_INDICES's value is incorrect."
#endif
#if (GL_CLAMP_TO_EDGE != 33071)
 #error "GL3 constant CLAMP_TO_EDGE's value is incorrect."
#endif
#if (GL_TEXTURE_MIN_LOD != 33082)
 #error "GL3 constant TEXTURE_MIN_LOD's value is incorrect."
#endif
#if (GL_TEXTURE_MAX_LOD != 33083)
 #error "GL3 constant TEXTURE_MAX_LOD's value is incorrect."
#endif
#if (GL_TEXTURE_BASE_LEVEL != 33084)
 #error "GL3 constant TEXTURE_BASE_LEVEL's value is incorrect."
#endif
#if (GL_TEXTURE_MAX_LEVEL != 33085)
 #error "GL3 constant TEXTURE_MAX_LEVEL's value is incorrect."
#endif
#if (GL_SMOOTH_POINT_SIZE_RANGE != 2834)
 #error "GL3 constant SMOOTH_POINT_SIZE_RANGE's value is incorrect."
#endif
#if (GL_SMOOTH_POINT_SIZE_GRANULARITY != 2835)
 #error "GL3 constant SMOOTH_POINT_SIZE_GRANULARITY's value is incorrect."
#endif
#if (GL_SMOOTH_LINE_WIDTH_RANGE != 2850)
 #error "GL3 constant SMOOTH_LINE_WIDTH_RANGE's value is incorrect."
#endif
#if (GL_SMOOTH_LINE_WIDTH_GRANULARITY != 2851)
 #error "GL3 constant SMOOTH_LINE_WIDTH_GRANULARITY's value is incorrect."
#endif
#if (GL_ALIASED_LINE_WIDTH_RANGE != 33902)
 #error "GL3 constant ALIASED_LINE_WIDTH_RANGE's value is incorrect."
#endif
#if (GL_CONSTANT_COLOR != 32769)
 #error "GL3 constant CONSTANT_COLOR's value is incorrect."
#endif
#if (GL_ONE_MINUS_CONSTANT_COLOR != 32770)
 #error "GL3 constant ONE_MINUS_CONSTANT_COLOR's value is incorrect."
#endif
#if (GL_CONSTANT_ALPHA != 32771)
 #error "GL3 constant CONSTANT_ALPHA's value is incorrect."
#endif
#if (GL_ONE_MINUS_CONSTANT_ALPHA != 32772)
 #error "GL3 constant ONE_MINUS_CONSTANT_ALPHA's value is incorrect."
#endif
#if (GL_BLEND_COLOR != 32773)
 #error "GL3 constant BLEND_COLOR's value is incorrect."
#endif
#if (GL_FUNC_ADD != 32774)
 #error "GL3 constant FUNC_ADD's value is incorrect."
#endif
#if (GL_MIN != 32775)
 #error "GL3 constant MIN's value is incorrect."
#endif
#if (GL_MAX != 32776)
 #error "GL3 constant MAX's value is incorrect."
#endif
#if (GL_BLEND_EQUATION != 32777)
 #error "GL3 constant BLEND_EQUATION's value is incorrect."
#endif
#if (GL_FUNC_SUBTRACT != 32778)
 #error "GL3 constant FUNC_SUBTRACT's value is incorrect."
#endif
#if (GL_FUNC_REVERSE_SUBTRACT != 32779)
 #error "GL3 constant FUNC_REVERSE_SUBTRACT's value is incorrect."
#endif
#if (GL_TEXTURE0 != 33984)
 #error "GL3 constant TEXTURE0's value is incorrect."
#endif
#if (GL_TEXTURE1 != 33985)
 #error "GL3 constant TEXTURE1's value is incorrect."
#endif
#if (GL_TEXTURE2 != 33986)
 #error "GL3 constant TEXTURE2's value is incorrect."
#endif
#if (GL_TEXTURE3 != 33987)
 #error "GL3 constant TEXTURE3's value is incorrect."
#endif
#if (GL_TEXTURE4 != 33988)
 #error "GL3 constant TEXTURE4's value is incorrect."
#endif
#if (GL_TEXTURE5 != 33989)
 #error "GL3 constant TEXTURE5's value is incorrect."
#endif
#if (GL_TEXTURE6 != 33990)
 #error "GL3 constant TEXTURE6's value is incorrect."
#endif
#if (GL_TEXTURE7 != 33991)
 #error "GL3 constant TEXTURE7's value is incorrect."
#endif
#if (GL_TEXTURE8 != 33992)
 #error "GL3 constant TEXTURE8's value is incorrect."
#endif
#if (GL_TEXTURE9 != 33993)
 #error "GL3 constant TEXTURE9's value is incorrect."
#endif
#if (GL_TEXTURE10 != 33994)
 #error "GL3 constant TEXTURE10's value is incorrect."
#endif
#if (GL_TEXTURE11 != 33995)
 #error "GL3 constant TEXTURE11's value is incorrect."
#endif
#if (GL_TEXTURE12 != 33996)
 #error "GL3 constant TEXTURE12's value is incorrect."
#endif
#if (GL_TEXTURE13 != 33997)
 #error "GL3 constant TEXTURE13's value is incorrect."
#endif
#if (GL_TEXTURE14 != 33998)
 #error "GL3 constant TEXTURE14's value is incorrect."
#endif
#if (GL_TEXTURE15 != 33999)
 #error "GL3 constant TEXTURE15's value is incorrect."
#endif
#if (GL_TEXTURE16 != 34000)
 #error "GL3 constant TEXTURE16's value is incorrect."
#endif
#if (GL_TEXTURE17 != 34001)
 #error "GL3 constant TEXTURE17's value is incorrect."
#endif
#if (GL_TEXTURE18 != 34002)
 #error "GL3 constant TEXTURE18's value is incorrect."
#endif
#if (GL_TEXTURE19 != 34003)
 #error "GL3 constant TEXTURE19's value is incorrect."
#endif
#if (GL_TEXTURE20 != 34004)
 #error "GL3 constant TEXTURE20's value is incorrect."
#endif
#if (GL_TEXTURE21 != 34005)
 #error "GL3 constant TEXTURE21's value is incorrect."
#endif
#if (GL_TEXTURE22 != 34006)
 #error "GL3 constant TEXTURE22's value is incorrect."
#endif
#if (GL_TEXTURE23 != 34007)
 #error "GL3 constant TEXTURE23's value is incorrect."
#endif
#if (GL_TEXTURE24 != 34008)
 #error "GL3 constant TEXTURE24's value is incorrect."
#endif
#if (GL_TEXTURE25 != 34009)
 #error "GL3 constant TEXTURE25's value is incorrect."
#endif
#if (GL_TEXTURE26 != 34010)
 #error "GL3 constant TEXTURE26's value is incorrect."
#endif
#if (GL_TEXTURE27 != 34011)
 #error "GL3 constant TEXTURE27's value is incorrect."
#endif
#if (GL_TEXTURE28 != 34012)
 #error "GL3 constant TEXTURE28's value is incorrect."
#endif
#if (GL_TEXTURE29 != 34013)
 #error "GL3 constant TEXTURE29's value is incorrect."
#endif
#if (GL_TEXTURE30 != 34014)
 #error "GL3 constant TEXTURE30's value is incorrect."
#endif
#if (GL_TEXTURE31 != 34015)
 #error "GL3 constant TEXTURE31's value is incorrect."
#endif
#if (GL_ACTIVE_TEXTURE != 34016)
 #error "GL3 constant ACTIVE_TEXTURE's value is incorrect."
#endif
#if (GL_MULTISAMPLE != 32925)
 #error "GL3 constant MULTISAMPLE's value is incorrect."
#endif
#if (GL_SAMPLE_ALPHA_TO_COVERAGE != 32926)
 #error "GL3 constant SAMPLE_ALPHA_TO_COVERAGE's value is incorrect."
#endif
#if (GL_SAMPLE_ALPHA_TO_ONE != 32927)
 #error "GL3 constant SAMPLE_ALPHA_TO_ONE's value is incorrect."
#endif
#if (GL_SAMPLE_COVERAGE != 32928)
 #error "GL3 constant SAMPLE_COVERAGE's value is incorrect."
#endif
#if (GL_SAMPLE_BUFFERS != 32936)
 #error "GL3 constant SAMPLE_BUFFERS's value is incorrect."
#endif
#if (GL_SAMPLES != 32937)
 #error "GL3 constant SAMPLES's value is incorrect."
#endif
#if (GL_SAMPLE_COVERAGE_VALUE != 32938)
 #error "GL3 constant SAMPLE_COVERAGE_VALUE's value is incorrect."
#endif
#if (GL_SAMPLE_COVERAGE_INVERT != 32939)
 #error "GL3 constant SAMPLE_COVERAGE_INVERT's value is incorrect."
#endif
#if (GL_TEXTURE_CUBE_MAP != 34067)
 #error "GL3 constant TEXTURE_CUBE_MAP's value is incorrect."
#endif
#if (GL_TEXTURE_BINDING_CUBE_MAP != 34068)
 #error "GL3 constant TEXTURE_BINDING_CUBE_MAP's value is incorrect."
#endif
#if (GL_TEXTURE_CUBE_MAP_POSITIVE_X != 34069)
 #error "GL3 constant TEXTURE_CUBE_MAP_POSITIVE_X's value is incorrect."
#endif
#if (GL_TEXTURE_CUBE_MAP_NEGATIVE_X != 34070)
 #error "GL3 constant TEXTURE_CUBE_MAP_NEGATIVE_X's value is incorrect."
#endif
#if (GL_TEXTURE_CUBE_MAP_POSITIVE_Y != 34071)
 #error "GL3 constant TEXTURE_CUBE_MAP_POSITIVE_Y's value is incorrect."
#endif
#if (GL_TEXTURE_CUBE_MAP_NEGATIVE_Y != 34072)
 #error "GL3 constant TEXTURE_CUBE_MAP_NEGATIVE_Y's value is incorrect."
#endif
#if (GL_TEXTURE_CUBE_MAP_POSITIVE_Z != 34073)
 #error "GL3 constant TEXTURE_CUBE_MAP_POSITIVE_Z's value is incorrect."
#endif
#if (GL_TEXTURE_CUBE_MAP_NEGATIVE_Z != 34074)
 #error "GL3 constant TEXTURE_CUBE_MAP_NEGATIVE_Z's value is incorrect."
#endif
#if (GL_PROXY_TEXTURE_CUBE_MAP != 34075)
 #error "GL3 constant PROXY_TEXTURE_CUBE_MAP's value is incorrect."
#endif
#if (GL_MAX_CUBE_MAP_TEXTURE_SIZE != 34076)
 #error "GL3 constant MAX_CUBE_MAP_TEXTURE_SIZE's value is incorrect."
#endif
#if (GL_COMPRESSED_RGB != 34029)
 #error "GL3 constant COMPRESSED_RGB's value is incorrect."
#endif
#if (GL_COMPRESSED_RGBA != 34030)
 #error "GL3 constant COMPRESSED_RGBA's value is incorrect."
#endif
#if (GL_TEXTURE_COMPRESSION_HINT != 34031)
 #error "GL3 constant TEXTURE_COMPRESSION_HINT's value is incorrect."
#endif
#if (GL_TEXTURE_COMPRESSED_IMAGE_SIZE != 34464)
 #error "GL3 constant TEXTURE_COMPRESSED_IMAGE_SIZE's value is incorrect."
#endif
#if (GL_TEXTURE_COMPRESSED != 34465)
 #error "GL3 constant TEXTURE_COMPRESSED's value is incorrect."
#endif
#if (GL_NUM_COMPRESSED_TEXTURE_FORMATS != 34466)
 #error "GL3 constant NUM_COMPRESSED_TEXTURE_FORMATS's value is incorrect."
#endif
#if (GL_COMPRESSED_TEXTURE_FORMATS != 34467)
 #error "GL3 constant COMPRESSED_TEXTURE_FORMATS's value is incorrect."
#endif
#if (GL_CLAMP_TO_BORDER != 33069)
 #error "GL3 constant CLAMP_TO_BORDER's value is incorrect."
#endif
#if (GL_BLEND_DST_RGB != 32968)
 #error "GL3 constant BLEND_DST_RGB's value is incorrect."
#endif
#if (GL_BLEND_SRC_RGB != 32969)
 #error "GL3 constant BLEND_SRC_RGB's value is incorrect."
#endif
#if (GL_BLEND_DST_ALPHA != 32970)
 #error "GL3 constant BLEND_DST_ALPHA's value is incorrect."
#endif
#if (GL_BLEND_SRC_ALPHA != 32971)
 #error "GL3 constant BLEND_SRC_ALPHA's value is incorrect."
#endif
#if (GL_POINT_FADE_THRESHOLD_SIZE != 33064)
 #error "GL3 constant POINT_FADE_THRESHOLD_SIZE's value is incorrect."
#endif
#if (GL_DEPTH_COMPONENT16 != 33189)
 #error "GL3 constant DEPTH_COMPONENT16's value is incorrect."
#endif
#if (GL_DEPTH_COMPONENT24 != 33190)
 #error "GL3 constant DEPTH_COMPONENT24's value is incorrect."
#endif
#if (GL_DEPTH_COMPONENT32 != 33191)
 #error "GL3 constant DEPTH_COMPONENT32's value is incorrect."
#endif
#if (GL_MIRRORED_REPEAT != 33648)
 #error "GL3 constant MIRRORED_REPEAT's value is incorrect."
#endif
#if (GL_MAX_TEXTURE_LOD_BIAS != 34045)
 #error "GL3 constant MAX_TEXTURE_LOD_BIAS's value is incorrect."
#endif
#if (GL_TEXTURE_LOD_BIAS != 34049)
 #error "GL3 constant TEXTURE_LOD_BIAS's value is incorrect."
#endif
#if (GL_INCR_WRAP != 34055)
 #error "GL3 constant INCR_WRAP's value is incorrect."
#endif
#if (GL_DECR_WRAP != 34056)
 #error "GL3 constant DECR_WRAP's value is incorrect."
#endif
#if (GL_TEXTURE_DEPTH_SIZE != 34890)
 #error "GL3 constant TEXTURE_DEPTH_SIZE's value is incorrect."
#endif
#if (GL_TEXTURE_COMPARE_MODE != 34892)
 #error "GL3 constant TEXTURE_COMPARE_MODE's value is incorrect."
#endif
#if (GL_TEXTURE_COMPARE_FUNC != 34893)
 #error "GL3 constant TEXTURE_COMPARE_FUNC's value is incorrect."
#endif
#if (GL_BUFFER_SIZE != 34660)
 #error "GL3 constant BUFFER_SIZE's value is incorrect."
#endif
#if (GL_BUFFER_USAGE != 34661)
 #error "GL3 constant BUFFER_USAGE's value is incorrect."
#endif
#if (GL_QUERY_COUNTER_BITS != 34916)
 #error "GL3 constant QUERY_COUNTER_BITS's value is incorrect."
#endif
#if (GL_CURRENT_QUERY != 34917)
 #error "GL3 constant CURRENT_QUERY's value is incorrect."
#endif
#if (GL_QUERY_RESULT != 34918)
 #error "GL3 constant QUERY_RESULT's value is incorrect."
#endif
#if (GL_QUERY_RESULT_AVAILABLE != 34919)
 #error "GL3 constant QUERY_RESULT_AVAILABLE's value is incorrect."
#endif
#if (GL_ARRAY_BUFFER != 34962)
 #error "GL3 constant ARRAY_BUFFER's value is incorrect."
#endif
#if (GL_ELEMENT_ARRAY_BUFFER != 34963)
 #error "GL3 constant ELEMENT_ARRAY_BUFFER's value is incorrect."
#endif
#if (GL_ARRAY_BUFFER_BINDING != 34964)
 #error "GL3 constant ARRAY_BUFFER_BINDING's value is incorrect."
#endif
#if (GL_ELEMENT_ARRAY_BUFFER_BINDING != 34965)
 #error "GL3 constant ELEMENT_ARRAY_BUFFER_BINDING's value is incorrect."
#endif
#if (GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING != 34975)
 #error "GL3 constant VERTEX_ATTRIB_ARRAY_BUFFER_BINDING's value is incorrect."
#endif
#if (GL_READ_ONLY != 35000)
 #error "GL3 constant READ_ONLY's value is incorrect."
#endif
#if (GL_WRITE_ONLY != 35001)
 #error "GL3 constant WRITE_ONLY's value is incorrect."
#endif
#if (GL_READ_WRITE != 35002)
 #error "GL3 constant READ_WRITE's value is incorrect."
#endif
#if (GL_BUFFER_ACCESS != 35003)
 #error "GL3 constant BUFFER_ACCESS's value is incorrect."
#endif
#if (GL_BUFFER_MAPPED != 35004)
 #error "GL3 constant BUFFER_MAPPED's value is incorrect."
#endif
#if (GL_BUFFER_MAP_POINTER != 35005)
 #error "GL3 constant BUFFER_MAP_POINTER's value is incorrect."
#endif
#if (GL_STREAM_DRAW != 35040)
 #error "GL3 constant STREAM_DRAW's value is incorrect."
#endif
#if (GL_STREAM_READ != 35041)
 #error "GL3 constant STREAM_READ's value is incorrect."
#endif
#if (GL_STREAM_COPY != 35042)
 #error "GL3 constant STREAM_COPY's value is incorrect."
#endif
#if (GL_STATIC_DRAW != 35044)
 #error "GL3 constant STATIC_DRAW's value is incorrect."
#endif
#if (GL_STATIC_READ != 35045)
 #error "GL3 constant STATIC_READ's value is incorrect."
#endif
#if (GL_STATIC_COPY != 35046)
 #error "GL3 constant STATIC_COPY's value is incorrect."
#endif
#if (GL_DYNAMIC_DRAW != 35048)
 #error "GL3 constant DYNAMIC_DRAW's value is incorrect."
#endif
#if (GL_DYNAMIC_READ != 35049)
 #error "GL3 constant DYNAMIC_READ's value is incorrect."
#endif
#if (GL_DYNAMIC_COPY != 35050)
 #error "GL3 constant DYNAMIC_COPY's value is incorrect."
#endif
#if (GL_SAMPLES_PASSED != 35092)
 #error "GL3 constant SAMPLES_PASSED's value is incorrect."
#endif
#if (GL_BLEND_EQUATION_RGB != 32777)
 #error "GL3 constant BLEND_EQUATION_RGB's value is incorrect."
#endif
#if (GL_VERTEX_ATTRIB_ARRAY_ENABLED != 34338)
 #error "GL3 constant VERTEX_ATTRIB_ARRAY_ENABLED's value is incorrect."
#endif
#if (GL_VERTEX_ATTRIB_ARRAY_SIZE != 34339)
 #error "GL3 constant VERTEX_ATTRIB_ARRAY_SIZE's value is incorrect."
#endif
#if (GL_VERTEX_ATTRIB_ARRAY_STRIDE != 34340)
 #error "GL3 constant VERTEX_ATTRIB_ARRAY_STRIDE's value is incorrect."
#endif
#if (GL_VERTEX_ATTRIB_ARRAY_TYPE != 34341)
 #error "GL3 constant VERTEX_ATTRIB_ARRAY_TYPE's value is incorrect."
#endif
#if (GL_CURRENT_VERTEX_ATTRIB != 34342)
 #error "GL3 constant CURRENT_VERTEX_ATTRIB's value is incorrect."
#endif
#if (GL_VERTEX_PROGRAM_POINT_SIZE != 34370)
 #error "GL3 constant VERTEX_PROGRAM_POINT_SIZE's value is incorrect."
#endif
#if (GL_VERTEX_ATTRIB_ARRAY_POINTER != 34373)
 #error "GL3 constant VERTEX_ATTRIB_ARRAY_POINTER's value is incorrect."
#endif
#if (GL_STENCIL_BACK_FUNC != 34816)
 #error "GL3 constant STENCIL_BACK_FUNC's value is incorrect."
#endif
#if (GL_STENCIL_BACK_FAIL != 34817)
 #error "GL3 constant STENCIL_BACK_FAIL's value is incorrect."
#endif
#if (GL_STENCIL_BACK_PASS_DEPTH_FAIL != 34818)
 #error "GL3 constant STENCIL_BACK_PASS_DEPTH_FAIL's value is incorrect."
#endif
#if (GL_STENCIL_BACK_PASS_DEPTH_PASS != 34819)
 #error "GL3 constant STENCIL_BACK_PASS_DEPTH_PASS's value is incorrect."
#endif
#if (GL_MAX_DRAW_BUFFERS != 34852)
 #error "GL3 constant MAX_DRAW_BUFFERS's value is incorrect."
#endif
#if (GL_DRAW_BUFFER0 != 34853)
 #error "GL3 constant DRAW_BUFFER0's value is incorrect."
#endif
#if (GL_DRAW_BUFFER1 != 34854)
 #error "GL3 constant DRAW_BUFFER1's value is incorrect."
#endif
#if (GL_DRAW_BUFFER2 != 34855)
 #error "GL3 constant DRAW_BUFFER2's value is incorrect."
#endif
#if (GL_DRAW_BUFFER3 != 34856)
 #error "GL3 constant DRAW_BUFFER3's value is incorrect."
#endif
#if (GL_DRAW_BUFFER4 != 34857)
 #error "GL3 constant DRAW_BUFFER4's value is incorrect."
#endif
#if (GL_DRAW_BUFFER5 != 34858)
 #error "GL3 constant DRAW_BUFFER5's value is incorrect."
#endif
#if (GL_DRAW_BUFFER6 != 34859)
 #error "GL3 constant DRAW_BUFFER6's value is incorrect."
#endif
#if (GL_DRAW_BUFFER7 != 34860)
 #error "GL3 constant DRAW_BUFFER7's value is incorrect."
#endif
#if (GL_DRAW_BUFFER8 != 34861)
 #error "GL3 constant DRAW_BUFFER8's value is incorrect."
#endif
#if (GL_DRAW_BUFFER9 != 34862)
 #error "GL3 constant DRAW_BUFFER9's value is incorrect."
#endif
#if (GL_DRAW_BUFFER10 != 34863)
 #error "GL3 constant DRAW_BUFFER10's value is incorrect."
#endif
#if (GL_DRAW_BUFFER11 != 34864)
 #error "GL3 constant DRAW_BUFFER11's value is incorrect."
#endif
#if (GL_DRAW_BUFFER12 != 34865)
 #error "GL3 constant DRAW_BUFFER12's value is incorrect."
#endif
#if (GL_DRAW_BUFFER13 != 34866)
 #error "GL3 constant DRAW_BUFFER13's value is incorrect."
#endif
#if (GL_DRAW_BUFFER14 != 34867)
 #error "GL3 constant DRAW_BUFFER14's value is incorrect."
#endif
#if (GL_DRAW_BUFFER15 != 34868)
 #error "GL3 constant DRAW_BUFFER15's value is incorrect."
#endif
#if (GL_BLEND_EQUATION_ALPHA != 34877)
 #error "GL3 constant BLEND_EQUATION_ALPHA's value is incorrect."
#endif
#if (GL_MAX_VERTEX_ATTRIBS != 34921)
 #error "GL3 constant MAX_VERTEX_ATTRIBS's value is incorrect."
#endif
#if (GL_VERTEX_ATTRIB_ARRAY_NORMALIZED != 34922)
 #error "GL3 constant VERTEX_ATTRIB_ARRAY_NORMALIZED's value is incorrect."
#endif
#if (GL_MAX_TEXTURE_IMAGE_UNITS != 34930)
 #error "GL3 constant MAX_TEXTURE_IMAGE_UNITS's value is incorrect."
#endif
#if (GL_FRAGMENT_SHADER != 35632)
 #error "GL3 constant FRAGMENT_SHADER's value is incorrect."
#endif
#if (GL_VERTEX_SHADER != 35633)
 #error "GL3 constant VERTEX_SHADER's value is incorrect."
#endif
#if (GL_MAX_FRAGMENT_UNIFORM_COMPONENTS != 35657)
 #error "GL3 constant MAX_FRAGMENT_UNIFORM_COMPONENTS's value is incorrect."
#endif
#if (GL_MAX_VERTEX_UNIFORM_COMPONENTS != 35658)
 #error "GL3 constant MAX_VERTEX_UNIFORM_COMPONENTS's value is incorrect."
#endif
#if (GL_MAX_VARYING_FLOATS != 35659)
 #error "GL3 constant MAX_VARYING_FLOATS's value is incorrect."
#endif
#if (GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS != 35660)
 #error "GL3 constant MAX_VERTEX_TEXTURE_IMAGE_UNITS's value is incorrect."
#endif
#if (GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS != 35661)
 #error "GL3 constant MAX_COMBINED_TEXTURE_IMAGE_UNITS's value is incorrect."
#endif
#if (GL_SHADER_TYPE != 35663)
 #error "GL3 constant SHADER_TYPE's value is incorrect."
#endif
#if (GL_FLOAT_VEC2 != 35664)
 #error "GL3 constant FLOAT_VEC2's value is incorrect."
#endif
#if (GL_FLOAT_VEC3 != 35665)
 #error "GL3 constant FLOAT_VEC3's value is incorrect."
#endif
#if (GL_FLOAT_VEC4 != 35666)
 #error "GL3 constant FLOAT_VEC4's value is incorrect."
#endif
#if (GL_INT_VEC2 != 35667)
 #error "GL3 constant INT_VEC2's value is incorrect."
#endif
#if (GL_INT_VEC3 != 35668)
 #error "GL3 constant INT_VEC3's value is incorrect."
#endif
#if (GL_INT_VEC4 != 35669)
 #error "GL3 constant INT_VEC4's value is incorrect."
#endif
#if (GL_BOOL != 35670)
 #error "GL3 constant BOOL's value is incorrect."
#endif
#if (GL_BOOL_VEC2 != 35671)
 #error "GL3 constant BOOL_VEC2's value is incorrect."
#endif
#if (GL_BOOL_VEC3 != 35672)
 #error "GL3 constant BOOL_VEC3's value is incorrect."
#endif
#if (GL_BOOL_VEC4 != 35673)
 #error "GL3 constant BOOL_VEC4's value is incorrect."
#endif
#if (GL_FLOAT_MAT2 != 35674)
 #error "GL3 constant FLOAT_MAT2's value is incorrect."
#endif
#if (GL_FLOAT_MAT3 != 35675)
 #error "GL3 constant FLOAT_MAT3's value is incorrect."
#endif
#if (GL_FLOAT_MAT4 != 35676)
 #error "GL3 constant FLOAT_MAT4's value is incorrect."
#endif
#if (GL_SAMPLER_1D != 35677)
 #error "GL3 constant SAMPLER_1D's value is incorrect."
#endif
#if (GL_SAMPLER_2D != 35678)
 #error "GL3 constant SAMPLER_2D's value is incorrect."
#endif
#if (GL_SAMPLER_3D != 35679)
 #error "GL3 constant SAMPLER_3D's value is incorrect."
#endif
#if (GL_SAMPLER_CUBE != 35680)
 #error "GL3 constant SAMPLER_CUBE's value is incorrect."
#endif
#if (GL_SAMPLER_1D_SHADOW != 35681)
 #error "GL3 constant SAMPLER_1D_SHADOW's value is incorrect."
#endif
#if (GL_SAMPLER_2D_SHADOW != 35682)
 #error "GL3 constant SAMPLER_2D_SHADOW's value is incorrect."
#endif
#if (GL_DELETE_STATUS != 35712)
 #error "GL3 constant DELETE_STATUS's value is incorrect."
#endif
#if (GL_COMPILE_STATUS != 35713)
 #error "GL3 constant COMPILE_STATUS's value is incorrect."
#endif
#if (GL_LINK_STATUS != 35714)
 #error "GL3 constant LINK_STATUS's value is incorrect."
#endif
#if (GL_VALIDATE_STATUS != 35715)
 #error "GL3 constant VALIDATE_STATUS's value is incorrect."
#endif
#if (GL_INFO_LOG_LENGTH != 35716)
 #error "GL3 constant INFO_LOG_LENGTH's value is incorrect."
#endif
#if (GL_ATTACHED_SHADERS != 35717)
 #error "GL3 constant ATTACHED_SHADERS's value is incorrect."
#endif
#if (GL_ACTIVE_UNIFORMS != 35718)
 #error "GL3 constant ACTIVE_UNIFORMS's value is incorrect."
#endif
#if (GL_ACTIVE_UNIFORM_MAX_LENGTH != 35719)
 #error "GL3 constant ACTIVE_UNIFORM_MAX_LENGTH's value is incorrect."
#endif
#if (GL_SHADER_SOURCE_LENGTH != 35720)
 #error "GL3 constant SHADER_SOURCE_LENGTH's value is incorrect."
#endif
#if (GL_ACTIVE_ATTRIBUTES != 35721)
 #error "GL3 constant ACTIVE_ATTRIBUTES's value is incorrect."
#endif
#if (GL_ACTIVE_ATTRIBUTE_MAX_LENGTH != 35722)
 #error "GL3 constant ACTIVE_ATTRIBUTE_MAX_LENGTH's value is incorrect."
#endif
#if (GL_FRAGMENT_SHADER_DERIVATIVE_HINT != 35723)
 #error "GL3 constant FRAGMENT_SHADER_DERIVATIVE_HINT's value is incorrect."
#endif
#if (GL_SHADING_LANGUAGE_VERSION != 35724)
 #error "GL3 constant SHADING_LANGUAGE_VERSION's value is incorrect."
#endif
#if (GL_CURRENT_PROGRAM != 35725)
 #error "GL3 constant CURRENT_PROGRAM's value is incorrect."
#endif
#if (GL_POINT_SPRITE_COORD_ORIGIN != 36000)
 #error "GL3 constant POINT_SPRITE_COORD_ORIGIN's value is incorrect."
#endif
#if (GL_LOWER_LEFT != 36001)
 #error "GL3 constant LOWER_LEFT's value is incorrect."
#endif
#if (GL_UPPER_LEFT != 36002)
 #error "GL3 constant UPPER_LEFT's value is incorrect."
#endif
#if (GL_STENCIL_BACK_REF != 36003)
 #error "GL3 constant STENCIL_BACK_REF's value is incorrect."
#endif
#if (GL_STENCIL_BACK_VALUE_MASK != 36004)
 #error "GL3 constant STENCIL_BACK_VALUE_MASK's value is incorrect."
#endif
#if (GL_STENCIL_BACK_WRITEMASK != 36005)
 #error "GL3 constant STENCIL_BACK_WRITEMASK's value is incorrect."
#endif
#if (GL_PIXEL_PACK_BUFFER != 35051)
 #error "GL3 constant PIXEL_PACK_BUFFER's value is incorrect."
#endif
#if (GL_PIXEL_UNPACK_BUFFER != 35052)
 #error "GL3 constant PIXEL_UNPACK_BUFFER's value is incorrect."
#endif
#if (GL_PIXEL_PACK_BUFFER_BINDING != 35053)
 #error "GL3 constant PIXEL_PACK_BUFFER_BINDING's value is incorrect."
#endif
#if (GL_PIXEL_UNPACK_BUFFER_BINDING != 35055)
 #error "GL3 constant PIXEL_UNPACK_BUFFER_BINDING's value is incorrect."
#endif
#if (GL_FLOAT_MAT2x3 != 35685)
 #error "GL3 constant FLOAT_MAT2x3's value is incorrect."
#endif
#if (GL_FLOAT_MAT2x4 != 35686)
 #error "GL3 constant FLOAT_MAT2x4's value is incorrect."
#endif
#if (GL_FLOAT_MAT3x2 != 35687)
 #error "GL3 constant FLOAT_MAT3x2's value is incorrect."
#endif
#if (GL_FLOAT_MAT3x4 != 35688)
 #error "GL3 constant FLOAT_MAT3x4's value is incorrect."
#endif
#if (GL_FLOAT_MAT4x2 != 35689)
 #error "GL3 constant FLOAT_MAT4x2's value is incorrect."
#endif
#if (GL_FLOAT_MAT4x3 != 35690)
 #error "GL3 constant FLOAT_MAT4x3's value is incorrect."
#endif
#if (GL_SRGB != 35904)
 #error "GL3 constant SRGB's value is incorrect."
#endif
#if (GL_SRGB8 != 35905)
 #error "GL3 constant SRGB8's value is incorrect."
#endif
#if (GL_SRGB_ALPHA != 35906)
 #error "GL3 constant SRGB_ALPHA's value is incorrect."
#endif
#if (GL_SRGB8_ALPHA8 != 35907)
 #error "GL3 constant SRGB8_ALPHA8's value is incorrect."
#endif
#if (GL_COMPRESSED_SRGB != 35912)
 #error "GL3 constant COMPRESSED_SRGB's value is incorrect."
#endif
#if (GL_COMPRESSED_SRGB_ALPHA != 35913)
 #error "GL3 constant COMPRESSED_SRGB_ALPHA's value is incorrect."
#endif
#if (GL_COMPARE_REF_TO_TEXTURE != 34894)
 #error "GL3 constant COMPARE_REF_TO_TEXTURE's value is incorrect."
#endif
#if (GL_CLIP_DISTANCE0 != 12288)
 #error "GL3 constant CLIP_DISTANCE0's value is incorrect."
#endif
#if (GL_CLIP_DISTANCE1 != 12289)
 #error "GL3 constant CLIP_DISTANCE1's value is incorrect."
#endif
#if (GL_CLIP_DISTANCE2 != 12290)
 #error "GL3 constant CLIP_DISTANCE2's value is incorrect."
#endif
#if (GL_CLIP_DISTANCE3 != 12291)
 #error "GL3 constant CLIP_DISTANCE3's value is incorrect."
#endif
#if (GL_CLIP_DISTANCE4 != 12292)
 #error "GL3 constant CLIP_DISTANCE4's value is incorrect."
#endif
#if (GL_CLIP_DISTANCE5 != 12293)
 #error "GL3 constant CLIP_DISTANCE5's value is incorrect."
#endif
#if (GL_MAX_CLIP_DISTANCES != 3378)
 #error "GL3 constant MAX_CLIP_DISTANCES's value is incorrect."
#endif
#if (GL_MAJOR_VERSION != 33307)
 #error "GL3 constant MAJOR_VERSION's value is incorrect."
#endif
#if (GL_MINOR_VERSION != 33308)
 #error "GL3 constant MINOR_VERSION's value is incorrect."
#endif
#if (GL_NUM_EXTENSIONS != 33309)
 #error "GL3 constant NUM_EXTENSIONS's value is incorrect."
#endif
#if (GL_CONTEXT_FLAGS != 33310)
 #error "GL3 constant CONTEXT_FLAGS's value is incorrect."
#endif
#if (GL_COMPRESSED_RED != 33317)
 #error "GL3 constant COMPRESSED_RED's value is incorrect."
#endif
#if (GL_COMPRESSED_RG != 33318)
 #error "GL3 constant COMPRESSED_RG's value is incorrect."
#endif
#if (GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT != 1)
 #error "GL3 constant CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT's value is incorrect."
#endif
#if (GL_RGBA32F != 34836)
 #error "GL3 constant RGBA32F's value is incorrect."
#endif
#if (GL_RGB32F != 34837)
 #error "GL3 constant RGB32F's value is incorrect."
#endif
#if (GL_RGBA16F != 34842)
 #error "GL3 constant RGBA16F's value is incorrect."
#endif
#if (GL_RGB16F != 34843)
 #error "GL3 constant RGB16F's value is incorrect."
#endif
#if (GL_VERTEX_ATTRIB_ARRAY_INTEGER != 35069)
 #error "GL3 constant VERTEX_ATTRIB_ARRAY_INTEGER's value is incorrect."
#endif
#if (GL_MAX_ARRAY_TEXTURE_LAYERS != 35071)
 #error "GL3 constant MAX_ARRAY_TEXTURE_LAYERS's value is incorrect."
#endif
#if (GL_MIN_PROGRAM_TEXEL_OFFSET != 35076)
 #error "GL3 constant MIN_PROGRAM_TEXEL_OFFSET's value is incorrect."
#endif
#if (GL_MAX_PROGRAM_TEXEL_OFFSET != 35077)
 #error "GL3 constant MAX_PROGRAM_TEXEL_OFFSET's value is incorrect."
#endif
#if (GL_CLAMP_READ_COLOR != 35100)
 #error "GL3 constant CLAMP_READ_COLOR's value is incorrect."
#endif
#if (GL_FIXED_ONLY != 35101)
 #error "GL3 constant FIXED_ONLY's value is incorrect."
#endif
#if (GL_MAX_VARYING_COMPONENTS != 35659)
 #error "GL3 constant MAX_VARYING_COMPONENTS's value is incorrect."
#endif
#if (GL_TEXTURE_1D_ARRAY != 35864)
 #error "GL3 constant TEXTURE_1D_ARRAY's value is incorrect."
#endif
#if (GL_PROXY_TEXTURE_1D_ARRAY != 35865)
 #error "GL3 constant PROXY_TEXTURE_1D_ARRAY's value is incorrect."
#endif
#if (GL_TEXTURE_2D_ARRAY != 35866)
 #error "GL3 constant TEXTURE_2D_ARRAY's value is incorrect."
#endif
#if (GL_PROXY_TEXTURE_2D_ARRAY != 35867)
 #error "GL3 constant PROXY_TEXTURE_2D_ARRAY's value is incorrect."
#endif
#if (GL_TEXTURE_BINDING_1D_ARRAY != 35868)
 #error "GL3 constant TEXTURE_BINDING_1D_ARRAY's value is incorrect."
#endif
#if (GL_TEXTURE_BINDING_2D_ARRAY != 35869)
 #error "GL3 constant TEXTURE_BINDING_2D_ARRAY's value is incorrect."
#endif
#if (GL_R11F_G11F_B10F != 35898)
 #error "GL3 constant R11F_G11F_B10F's value is incorrect."
#endif
#if (GL_UNSIGNED_INT_10F_11F_11F_REV != 35899)
 #error "GL3 constant UNSIGNED_INT_10F_11F_11F_REV's value is incorrect."
#endif
#if (GL_RGB9_E5 != 35901)
 #error "GL3 constant RGB9_E5's value is incorrect."
#endif
#if (GL_UNSIGNED_INT_5_9_9_9_REV != 35902)
 #error "GL3 constant UNSIGNED_INT_5_9_9_9_REV's value is incorrect."
#endif
#if (GL_TEXTURE_SHARED_SIZE != 35903)
 #error "GL3 constant TEXTURE_SHARED_SIZE's value is incorrect."
#endif
#if (GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH != 35958)
 #error "GL3 constant TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH's value is incorrect."
#endif
#if (GL_TRANSFORM_FEEDBACK_BUFFER_MODE != 35967)
 #error "GL3 constant TRANSFORM_FEEDBACK_BUFFER_MODE's value is incorrect."
#endif
#if (GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS != 35968)
 #error "GL3 constant MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS's value is incorrect."
#endif
#if (GL_TRANSFORM_FEEDBACK_VARYINGS != 35971)
 #error "GL3 constant TRANSFORM_FEEDBACK_VARYINGS's value is incorrect."
#endif
#if (GL_TRANSFORM_FEEDBACK_BUFFER_START != 35972)
 #error "GL3 constant TRANSFORM_FEEDBACK_BUFFER_START's value is incorrect."
#endif
#if (GL_TRANSFORM_FEEDBACK_BUFFER_SIZE != 35973)
 #error "GL3 constant TRANSFORM_FEEDBACK_BUFFER_SIZE's value is incorrect."
#endif
#if (GL_PRIMITIVES_GENERATED != 35975)
 #error "GL3 constant PRIMITIVES_GENERATED's value is incorrect."
#endif
#if (GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN != 35976)
 #error "GL3 constant TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN's value is incorrect."
#endif
#if (GL_RASTERIZER_DISCARD != 35977)
 #error "GL3 constant RASTERIZER_DISCARD's value is incorrect."
#endif
#if (GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS != 35978)
 #error "GL3 constant MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS's value is incorrect."
#endif
#if (GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS != 35979)
 #error "GL3 constant MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS's value is incorrect."
#endif
#if (GL_INTERLEAVED_ATTRIBS != 35980)
 #error "GL3 constant INTERLEAVED_ATTRIBS's value is incorrect."
#endif
#if (GL_SEPARATE_ATTRIBS != 35981)
 #error "GL3 constant SEPARATE_ATTRIBS's value is incorrect."
#endif
#if (GL_TRANSFORM_FEEDBACK_BUFFER != 35982)
 #error "GL3 constant TRANSFORM_FEEDBACK_BUFFER's value is incorrect."
#endif
#if (GL_TRANSFORM_FEEDBACK_BUFFER_BINDING != 35983)
 #error "GL3 constant TRANSFORM_FEEDBACK_BUFFER_BINDING's value is incorrect."
#endif
#if (GL_RGBA32UI != 36208)
 #error "GL3 constant RGBA32UI's value is incorrect."
#endif
#if (GL_RGB32UI != 36209)
 #error "GL3 constant RGB32UI's value is incorrect."
#endif
#if (GL_RGBA16UI != 36214)
 #error "GL3 constant RGBA16UI's value is incorrect."
#endif
#if (GL_RGB16UI != 36215)
 #error "GL3 constant RGB16UI's value is incorrect."
#endif
#if (GL_RGBA8UI != 36220)
 #error "GL3 constant RGBA8UI's value is incorrect."
#endif
#if (GL_RGB8UI != 36221)
 #error "GL3 constant RGB8UI's value is incorrect."
#endif
#if (GL_RGBA32I != 36226)
 #error "GL3 constant RGBA32I's value is incorrect."
#endif
#if (GL_RGB32I != 36227)
 #error "GL3 constant RGB32I's value is incorrect."
#endif
#if (GL_RGBA16I != 36232)
 #error "GL3 constant RGBA16I's value is incorrect."
#endif
#if (GL_RGB16I != 36233)
 #error "GL3 constant RGB16I's value is incorrect."
#endif
#if (GL_RGBA8I != 36238)
 #error "GL3 constant RGBA8I's value is incorrect."
#endif
#if (GL_RGB8I != 36239)
 #error "GL3 constant RGB8I's value is incorrect."
#endif
#if (GL_RED_INTEGER != 36244)
 #error "GL3 constant RED_INTEGER's value is incorrect."
#endif
#if (GL_GREEN_INTEGER != 36245)
 #error "GL3 constant GREEN_INTEGER's value is incorrect."
#endif
#if (GL_BLUE_INTEGER != 36246)
 #error "GL3 constant BLUE_INTEGER's value is incorrect."
#endif
#if (GL_RGB_INTEGER != 36248)
 #error "GL3 constant RGB_INTEGER's value is incorrect."
#endif
#if (GL_RGBA_INTEGER != 36249)
 #error "GL3 constant RGBA_INTEGER's value is incorrect."
#endif
#if (GL_BGR_INTEGER != 36250)
 #error "GL3 constant BGR_INTEGER's value is incorrect."
#endif
#if (GL_BGRA_INTEGER != 36251)
 #error "GL3 constant BGRA_INTEGER's value is incorrect."
#endif
#if (GL_SAMPLER_1D_ARRAY != 36288)
 #error "GL3 constant SAMPLER_1D_ARRAY's value is incorrect."
#endif
#if (GL_SAMPLER_2D_ARRAY != 36289)
 #error "GL3 constant SAMPLER_2D_ARRAY's value is incorrect."
#endif
#if (GL_SAMPLER_1D_ARRAY_SHADOW != 36291)
 #error "GL3 constant SAMPLER_1D_ARRAY_SHADOW's value is incorrect."
#endif
#if (GL_SAMPLER_2D_ARRAY_SHADOW != 36292)
 #error "GL3 constant SAMPLER_2D_ARRAY_SHADOW's value is incorrect."
#endif
#if (GL_SAMPLER_CUBE_SHADOW != 36293)
 #error "GL3 constant SAMPLER_CUBE_SHADOW's value is incorrect."
#endif
#if (GL_UNSIGNED_INT_VEC2 != 36294)
 #error "GL3 constant UNSIGNED_INT_VEC2's value is incorrect."
#endif
#if (GL_UNSIGNED_INT_VEC3 != 36295)
 #error "GL3 constant UNSIGNED_INT_VEC3's value is incorrect."
#endif
#if (GL_UNSIGNED_INT_VEC4 != 36296)
 #error "GL3 constant UNSIGNED_INT_VEC4's value is incorrect."
#endif
#if (GL_INT_SAMPLER_1D != 36297)
 #error "GL3 constant INT_SAMPLER_1D's value is incorrect."
#endif
#if (GL_INT_SAMPLER_2D != 36298)
 #error "GL3 constant INT_SAMPLER_2D's value is incorrect."
#endif
#if (GL_INT_SAMPLER_3D != 36299)
 #error "GL3 constant INT_SAMPLER_3D's value is incorrect."
#endif
#if (GL_INT_SAMPLER_CUBE != 36300)
 #error "GL3 constant INT_SAMPLER_CUBE's value is incorrect."
#endif
#if (GL_INT_SAMPLER_1D_ARRAY != 36302)
 #error "GL3 constant INT_SAMPLER_1D_ARRAY's value is incorrect."
#endif
#if (GL_INT_SAMPLER_2D_ARRAY != 36303)
 #error "GL3 constant INT_SAMPLER_2D_ARRAY's value is incorrect."
#endif
#if (GL_UNSIGNED_INT_SAMPLER_1D != 36305)
 #error "GL3 constant UNSIGNED_INT_SAMPLER_1D's value is incorrect."
#endif
#if (GL_UNSIGNED_INT_SAMPLER_2D != 36306)
 #error "GL3 constant UNSIGNED_INT_SAMPLER_2D's value is incorrect."
#endif
#if (GL_UNSIGNED_INT_SAMPLER_3D != 36307)
 #error "GL3 constant UNSIGNED_INT_SAMPLER_3D's value is incorrect."
#endif
#if (GL_UNSIGNED_INT_SAMPLER_CUBE != 36308)
 #error "GL3 constant UNSIGNED_INT_SAMPLER_CUBE's value is incorrect."
#endif
#if (GL_UNSIGNED_INT_SAMPLER_1D_ARRAY != 36310)
 #error "GL3 constant UNSIGNED_INT_SAMPLER_1D_ARRAY's value is incorrect."
#endif
#if (GL_UNSIGNED_INT_SAMPLER_2D_ARRAY != 36311)
 #error "GL3 constant UNSIGNED_INT_SAMPLER_2D_ARRAY's value is incorrect."
#endif
#if (GL_QUERY_WAIT != 36371)
 #error "GL3 constant QUERY_WAIT's value is incorrect."
#endif
#if (GL_QUERY_NO_WAIT != 36372)
 #error "GL3 constant QUERY_NO_WAIT's value is incorrect."
#endif
#if (GL_QUERY_BY_REGION_WAIT != 36373)
 #error "GL3 constant QUERY_BY_REGION_WAIT's value is incorrect."
#endif
#if (GL_QUERY_BY_REGION_NO_WAIT != 36374)
 #error "GL3 constant QUERY_BY_REGION_NO_WAIT's value is incorrect."
#endif
#if (GL_BUFFER_ACCESS_FLAGS != 37151)
 #error "GL3 constant BUFFER_ACCESS_FLAGS's value is incorrect."
#endif
#if (GL_BUFFER_MAP_LENGTH != 37152)
 #error "GL3 constant BUFFER_MAP_LENGTH's value is incorrect."
#endif
#if (GL_BUFFER_MAP_OFFSET != 37153)
 #error "GL3 constant BUFFER_MAP_OFFSET's value is incorrect."
#endif
#if (GL_SAMPLER_2D_RECT != 35683)
 #error "GL3 constant SAMPLER_2D_RECT's value is incorrect."
#endif
#if (GL_SAMPLER_2D_RECT_SHADOW != 35684)
 #error "GL3 constant SAMPLER_2D_RECT_SHADOW's value is incorrect."
#endif
#if (GL_SAMPLER_BUFFER != 36290)
 #error "GL3 constant SAMPLER_BUFFER's value is incorrect."
#endif
#if (GL_INT_SAMPLER_2D_RECT != 36301)
 #error "GL3 constant INT_SAMPLER_2D_RECT's value is incorrect."
#endif
#if (GL_INT_SAMPLER_BUFFER != 36304)
 #error "GL3 constant INT_SAMPLER_BUFFER's value is incorrect."
#endif
#if (GL_UNSIGNED_INT_SAMPLER_2D_RECT != 36309)
 #error "GL3 constant UNSIGNED_INT_SAMPLER_2D_RECT's value is incorrect."
#endif
#if (GL_UNSIGNED_INT_SAMPLER_BUFFER != 36312)
 #error "GL3 constant UNSIGNED_INT_SAMPLER_BUFFER's value is incorrect."
#endif
#if (GL_TEXTURE_BUFFER != 35882)
 #error "GL3 constant TEXTURE_BUFFER's value is incorrect."
#endif
#if (GL_MAX_TEXTURE_BUFFER_SIZE != 35883)
 #error "GL3 constant MAX_TEXTURE_BUFFER_SIZE's value is incorrect."
#endif
#if (GL_TEXTURE_BINDING_BUFFER != 35884)
 #error "GL3 constant TEXTURE_BINDING_BUFFER's value is incorrect."
#endif
#if (GL_TEXTURE_BUFFER_DATA_STORE_BINDING != 35885)
 #error "GL3 constant TEXTURE_BUFFER_DATA_STORE_BINDING's value is incorrect."
#endif
#if (GL_TEXTURE_RECTANGLE != 34037)
 #error "GL3 constant TEXTURE_RECTANGLE's value is incorrect."
#endif
#if (GL_TEXTURE_BINDING_RECTANGLE != 34038)
 #error "GL3 constant TEXTURE_BINDING_RECTANGLE's value is incorrect."
#endif
#if (GL_PROXY_TEXTURE_RECTANGLE != 34039)
 #error "GL3 constant PROXY_TEXTURE_RECTANGLE's value is incorrect."
#endif
#if (GL_MAX_RECTANGLE_TEXTURE_SIZE != 34040)
 #error "GL3 constant MAX_RECTANGLE_TEXTURE_SIZE's value is incorrect."
#endif
#if (GL_R8_SNORM != 36756)
 #error "GL3 constant R8_SNORM's value is incorrect."
#endif
#if (GL_RG8_SNORM != 36757)
 #error "GL3 constant RG8_SNORM's value is incorrect."
#endif
#if (GL_RGB8_SNORM != 36758)
 #error "GL3 constant RGB8_SNORM's value is incorrect."
#endif
#if (GL_RGBA8_SNORM != 36759)
 #error "GL3 constant RGBA8_SNORM's value is incorrect."
#endif
#if (GL_R16_SNORM != 36760)
 #error "GL3 constant R16_SNORM's value is incorrect."
#endif
#if (GL_RG16_SNORM != 36761)
 #error "GL3 constant RG16_SNORM's value is incorrect."
#endif
#if (GL_RGB16_SNORM != 36762)
 #error "GL3 constant RGB16_SNORM's value is incorrect."
#endif
#if (GL_RGBA16_SNORM != 36763)
 #error "GL3 constant RGBA16_SNORM's value is incorrect."
#endif
#if (GL_SIGNED_NORMALIZED != 36764)
 #error "GL3 constant SIGNED_NORMALIZED's value is incorrect."
#endif
#if (GL_PRIMITIVE_RESTART != 36765)
 #error "GL3 constant PRIMITIVE_RESTART's value is incorrect."
#endif
#if (GL_PRIMITIVE_RESTART_INDEX != 36766)
 #error "GL3 constant PRIMITIVE_RESTART_INDEX's value is incorrect."
#endif
#if (GL_CONTEXT_CORE_PROFILE_BIT != 1)
 #error "GL3 constant CONTEXT_CORE_PROFILE_BIT's value is incorrect."
#endif
#if (GL_CONTEXT_COMPATIBILITY_PROFILE_BIT != 2)
 #error "GL3 constant CONTEXT_COMPATIBILITY_PROFILE_BIT's value is incorrect."
#endif
#if (GL_LINES_ADJACENCY != 10)
 #error "GL3 constant LINES_ADJACENCY's value is incorrect."
#endif
#if (GL_LINE_STRIP_ADJACENCY != 11)
 #error "GL3 constant LINE_STRIP_ADJACENCY's value is incorrect."
#endif
#if (GL_TRIANGLES_ADJACENCY != 12)
 #error "GL3 constant TRIANGLES_ADJACENCY's value is incorrect."
#endif
#if (GL_TRIANGLE_STRIP_ADJACENCY != 13)
 #error "GL3 constant TRIANGLE_STRIP_ADJACENCY's value is incorrect."
#endif
#if (GL_PROGRAM_POINT_SIZE != 34370)
 #error "GL3 constant PROGRAM_POINT_SIZE's value is incorrect."
#endif
#if (GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS != 35881)
 #error "GL3 constant MAX_GEOMETRY_TEXTURE_IMAGE_UNITS's value is incorrect."
#endif
#if (GL_FRAMEBUFFER_ATTACHMENT_LAYERED != 36263)
 #error "GL3 constant FRAMEBUFFER_ATTACHMENT_LAYERED's value is incorrect."
#endif
#if (GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS != 36264)
 #error "GL3 constant FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS's value is incorrect."
#endif
#if (GL_GEOMETRY_SHADER != 36313)
 #error "GL3 constant GEOMETRY_SHADER's value is incorrect."
#endif
#if (GL_GEOMETRY_VERTICES_OUT != 35094)
 #error "GL3 constant GEOMETRY_VERTICES_OUT's value is incorrect."
#endif
#if (GL_GEOMETRY_INPUT_TYPE != 35095)
 #error "GL3 constant GEOMETRY_INPUT_TYPE's value is incorrect."
#endif
#if (GL_GEOMETRY_OUTPUT_TYPE != 35096)
 #error "GL3 constant GEOMETRY_OUTPUT_TYPE's value is incorrect."
#endif
#if (GL_MAX_GEOMETRY_UNIFORM_COMPONENTS != 36319)
 #error "GL3 constant MAX_GEOMETRY_UNIFORM_COMPONENTS's value is incorrect."
#endif
#if (GL_MAX_GEOMETRY_OUTPUT_VERTICES != 36320)
 #error "GL3 constant MAX_GEOMETRY_OUTPUT_VERTICES's value is incorrect."
#endif
#if (GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS != 36321)
 #error "GL3 constant MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS's value is incorrect."
#endif
#if (GL_MAX_VERTEX_OUTPUT_COMPONENTS != 37154)
 #error "GL3 constant MAX_VERTEX_OUTPUT_COMPONENTS's value is incorrect."
#endif
#if (GL_MAX_GEOMETRY_INPUT_COMPONENTS != 37155)
 #error "GL3 constant MAX_GEOMETRY_INPUT_COMPONENTS's value is incorrect."
#endif
#if (GL_MAX_GEOMETRY_OUTPUT_COMPONENTS != 37156)
 #error "GL3 constant MAX_GEOMETRY_OUTPUT_COMPONENTS's value is incorrect."
#endif
#if (GL_MAX_FRAGMENT_INPUT_COMPONENTS != 37157)
 #error "GL3 constant MAX_FRAGMENT_INPUT_COMPONENTS's value is incorrect."
#endif
#if (GL_CONTEXT_PROFILE_MASK != 37158)
 #error "GL3 constant CONTEXT_PROFILE_MASK's value is incorrect."
#endif
#if (GL_VERTEX_ATTRIB_ARRAY_DIVISOR != 35070)
 #error "GL3 constant VERTEX_ATTRIB_ARRAY_DIVISOR's value is incorrect."
#endif
#if (GL_SAMPLE_SHADING != 35894)
 #error "GL3 constant SAMPLE_SHADING's value is incorrect."
#endif
#if (GL_MIN_SAMPLE_SHADING_VALUE != 35895)
 #error "GL3 constant MIN_SAMPLE_SHADING_VALUE's value is incorrect."
#endif
#if (GL_MIN_PROGRAM_TEXTURE_GATHER_OFFSET != 36446)
 #error "GL3 constant MIN_PROGRAM_TEXTURE_GATHER_OFFSET's value is incorrect."
#endif
#if (GL_MAX_PROGRAM_TEXTURE_GATHER_OFFSET != 36447)
 #error "GL3 constant MAX_PROGRAM_TEXTURE_GATHER_OFFSET's value is incorrect."
#endif
#if (GL_TEXTURE_CUBE_MAP_ARRAY != 36873)
 #error "GL3 constant TEXTURE_CUBE_MAP_ARRAY's value is incorrect."
#endif
#if (GL_TEXTURE_BINDING_CUBE_MAP_ARRAY != 36874)
 #error "GL3 constant TEXTURE_BINDING_CUBE_MAP_ARRAY's value is incorrect."
#endif
#if (GL_PROXY_TEXTURE_CUBE_MAP_ARRAY != 36875)
 #error "GL3 constant PROXY_TEXTURE_CUBE_MAP_ARRAY's value is incorrect."
#endif
#if (GL_SAMPLER_CUBE_MAP_ARRAY != 36876)
 #error "GL3 constant SAMPLER_CUBE_MAP_ARRAY's value is incorrect."
#endif
#if (GL_SAMPLER_CUBE_MAP_ARRAY_SHADOW != 36877)
 #error "GL3 constant SAMPLER_CUBE_MAP_ARRAY_SHADOW's value is incorrect."
#endif
#if (GL_INT_SAMPLER_CUBE_MAP_ARRAY != 36878)
 #error "GL3 constant INT_SAMPLER_CUBE_MAP_ARRAY's value is incorrect."
#endif
#if (GL_UNSIGNED_INT_SAMPLER_CUBE_MAP_ARRAY != 36879)
 #error "GL3 constant UNSIGNED_INT_SAMPLER_CUBE_MAP_ARRAY's value is incorrect."
#endif
#if (GL_DEPTH_COMPONENT32F != 36012)
 #error "GL3 constant DEPTH_COMPONENT32F's value is incorrect."
#endif
#if (GL_DEPTH32F_STENCIL8 != 36013)
 #error "GL3 constant DEPTH32F_STENCIL8's value is incorrect."
#endif
#if (GL_FLOAT_32_UNSIGNED_INT_24_8_REV != 36269)
 #error "GL3 constant FLOAT_32_UNSIGNED_INT_24_8_REV's value is incorrect."
#endif
#if (GL_INVALID_FRAMEBUFFER_OPERATION != 1286)
 #error "GL3 constant INVALID_FRAMEBUFFER_OPERATION's value is incorrect."
#endif
#if (GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING != 33296)
 #error "GL3 constant FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING's value is incorrect."
#endif
#if (GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE != 33297)
 #error "GL3 constant FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE's value is incorrect."
#endif
#if (GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE != 33298)
 #error "GL3 constant FRAMEBUFFER_ATTACHMENT_RED_SIZE's value is incorrect."
#endif
#if (GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE != 33299)
 #error "GL3 constant FRAMEBUFFER_ATTACHMENT_GREEN_SIZE's value is incorrect."
#endif
#if (GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE != 33300)
 #error "GL3 constant FRAMEBUFFER_ATTACHMENT_BLUE_SIZE's value is incorrect."
#endif
#if (GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE != 33301)
 #error "GL3 constant FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE's value is incorrect."
#endif
#if (GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE != 33302)
 #error "GL3 constant FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE's value is incorrect."
#endif
#if (GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE != 33303)
 #error "GL3 constant FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE's value is incorrect."
#endif
#if (GL_FRAMEBUFFER_DEFAULT != 33304)
 #error "GL3 constant FRAMEBUFFER_DEFAULT's value is incorrect."
#endif
#if (GL_FRAMEBUFFER_UNDEFINED != 33305)
 #error "GL3 constant FRAMEBUFFER_UNDEFINED's value is incorrect."
#endif
#if (GL_DEPTH_STENCIL_ATTACHMENT != 33306)
 #error "GL3 constant DEPTH_STENCIL_ATTACHMENT's value is incorrect."
#endif
#if (GL_MAX_RENDERBUFFER_SIZE != 34024)
 #error "GL3 constant MAX_RENDERBUFFER_SIZE's value is incorrect."
#endif
#if (GL_DEPTH_STENCIL != 34041)
 #error "GL3 constant DEPTH_STENCIL's value is incorrect."
#endif
#if (GL_UNSIGNED_INT_24_8 != 34042)
 #error "GL3 constant UNSIGNED_INT_24_8's value is incorrect."
#endif
#if (GL_DEPTH24_STENCIL8 != 35056)
 #error "GL3 constant DEPTH24_STENCIL8's value is incorrect."
#endif
#if (GL_TEXTURE_STENCIL_SIZE != 35057)
 #error "GL3 constant TEXTURE_STENCIL_SIZE's value is incorrect."
#endif
#if (GL_TEXTURE_RED_TYPE != 35856)
 #error "GL3 constant TEXTURE_RED_TYPE's value is incorrect."
#endif
#if (GL_TEXTURE_GREEN_TYPE != 35857)
 #error "GL3 constant TEXTURE_GREEN_TYPE's value is incorrect."
#endif
#if (GL_TEXTURE_BLUE_TYPE != 35858)
 #error "GL3 constant TEXTURE_BLUE_TYPE's value is incorrect."
#endif
#if (GL_TEXTURE_ALPHA_TYPE != 35859)
 #error "GL3 constant TEXTURE_ALPHA_TYPE's value is incorrect."
#endif
#if (GL_TEXTURE_DEPTH_TYPE != 35862)
 #error "GL3 constant TEXTURE_DEPTH_TYPE's value is incorrect."
#endif
#if (GL_UNSIGNED_NORMALIZED != 35863)
 #error "GL3 constant UNSIGNED_NORMALIZED's value is incorrect."
#endif
#if (GL_FRAMEBUFFER_BINDING != 36006)
 #error "GL3 constant FRAMEBUFFER_BINDING's value is incorrect."
#endif
#if (GL_RENDERBUFFER_BINDING != 36007)
 #error "GL3 constant RENDERBUFFER_BINDING's value is incorrect."
#endif
#if (GL_READ_FRAMEBUFFER != 36008)
 #error "GL3 constant READ_FRAMEBUFFER's value is incorrect."
#endif
#if (GL_DRAW_FRAMEBUFFER != 36009)
 #error "GL3 constant DRAW_FRAMEBUFFER's value is incorrect."
#endif
#if (GL_READ_FRAMEBUFFER_BINDING != 36010)
 #error "GL3 constant READ_FRAMEBUFFER_BINDING's value is incorrect."
#endif
#if (GL_RENDERBUFFER_SAMPLES != 36011)
 #error "GL3 constant RENDERBUFFER_SAMPLES's value is incorrect."
#endif
#if (GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE != 36048)
 #error "GL3 constant FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE's value is incorrect."
#endif
#if (GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME != 36049)
 #error "GL3 constant FRAMEBUFFER_ATTACHMENT_OBJECT_NAME's value is incorrect."
#endif
#if (GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL != 36050)
 #error "GL3 constant FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL's value is incorrect."
#endif
#if (GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE != 36051)
 #error "GL3 constant FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE's value is incorrect."
#endif
#if (GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER != 36052)
 #error "GL3 constant FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER's value is incorrect."
#endif
#if (GL_FRAMEBUFFER_COMPLETE != 36053)
 #error "GL3 constant FRAMEBUFFER_COMPLETE's value is incorrect."
#endif
#if (GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT != 36054)
 #error "GL3 constant FRAMEBUFFER_INCOMPLETE_ATTACHMENT's value is incorrect."
#endif
#if (GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT != 36055)
 #error "GL3 constant FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT's value is incorrect."
#endif
#if (GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER != 36059)
 #error "GL3 constant FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER's value is incorrect."
#endif
#if (GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER != 36060)
 #error "GL3 constant FRAMEBUFFER_INCOMPLETE_READ_BUFFER's value is incorrect."
#endif
#if (GL_FRAMEBUFFER_UNSUPPORTED != 36061)
 #error "GL3 constant FRAMEBUFFER_UNSUPPORTED's value is incorrect."
#endif
#if (GL_MAX_COLOR_ATTACHMENTS != 36063)
 #error "GL3 constant MAX_COLOR_ATTACHMENTS's value is incorrect."
#endif
#if (GL_COLOR_ATTACHMENT0 != 36064)
 #error "GL3 constant COLOR_ATTACHMENT0's value is incorrect."
#endif
#if (GL_COLOR_ATTACHMENT1 != 36065)
 #error "GL3 constant COLOR_ATTACHMENT1's value is incorrect."
#endif
#if (GL_COLOR_ATTACHMENT2 != 36066)
 #error "GL3 constant COLOR_ATTACHMENT2's value is incorrect."
#endif
#if (GL_COLOR_ATTACHMENT3 != 36067)
 #error "GL3 constant COLOR_ATTACHMENT3's value is incorrect."
#endif
#if (GL_COLOR_ATTACHMENT4 != 36068)
 #error "GL3 constant COLOR_ATTACHMENT4's value is incorrect."
#endif
#if (GL_COLOR_ATTACHMENT5 != 36069)
 #error "GL3 constant COLOR_ATTACHMENT5's value is incorrect."
#endif
#if (GL_COLOR_ATTACHMENT6 != 36070)
 #error "GL3 constant COLOR_ATTACHMENT6's value is incorrect."
#endif
#if (GL_COLOR_ATTACHMENT7 != 36071)
 #error "GL3 constant COLOR_ATTACHMENT7's value is incorrect."
#endif
#if (GL_COLOR_ATTACHMENT8 != 36072)
 #error "GL3 constant COLOR_ATTACHMENT8's value is incorrect."
#endif
#if (GL_COLOR_ATTACHMENT9 != 36073)
 #error "GL3 constant COLOR_ATTACHMENT9's value is incorrect."
#endif
#if (GL_COLOR_ATTACHMENT10 != 36074)
 #error "GL3 constant COLOR_ATTACHMENT10's value is incorrect."
#endif
#if (GL_COLOR_ATTACHMENT11 != 36075)
 #error "GL3 constant COLOR_ATTACHMENT11's value is incorrect."
#endif
#if (GL_COLOR_ATTACHMENT12 != 36076)
 #error "GL3 constant COLOR_ATTACHMENT12's value is incorrect."
#endif
#if (GL_COLOR_ATTACHMENT13 != 36077)
 #error "GL3 constant COLOR_ATTACHMENT13's value is incorrect."
#endif
#if (GL_COLOR_ATTACHMENT14 != 36078)
 #error "GL3 constant COLOR_ATTACHMENT14's value is incorrect."
#endif
#if (GL_COLOR_ATTACHMENT15 != 36079)
 #error "GL3 constant COLOR_ATTACHMENT15's value is incorrect."
#endif
#if (GL_DEPTH_ATTACHMENT != 36096)
 #error "GL3 constant DEPTH_ATTACHMENT's value is incorrect."
#endif
#if (GL_STENCIL_ATTACHMENT != 36128)
 #error "GL3 constant STENCIL_ATTACHMENT's value is incorrect."
#endif
#if (GL_FRAMEBUFFER != 36160)
 #error "GL3 constant FRAMEBUFFER's value is incorrect."
#endif
#if (GL_RENDERBUFFER != 36161)
 #error "GL3 constant RENDERBUFFER's value is incorrect."
#endif
#if (GL_RENDERBUFFER_WIDTH != 36162)
 #error "GL3 constant RENDERBUFFER_WIDTH's value is incorrect."
#endif
#if (GL_RENDERBUFFER_HEIGHT != 36163)
 #error "GL3 constant RENDERBUFFER_HEIGHT's value is incorrect."
#endif
#if (GL_RENDERBUFFER_INTERNAL_FORMAT != 36164)
 #error "GL3 constant RENDERBUFFER_INTERNAL_FORMAT's value is incorrect."
#endif
#if (GL_STENCIL_INDEX1 != 36166)
 #error "GL3 constant STENCIL_INDEX1's value is incorrect."
#endif
#if (GL_STENCIL_INDEX4 != 36167)
 #error "GL3 constant STENCIL_INDEX4's value is incorrect."
#endif
#if (GL_STENCIL_INDEX8 != 36168)
 #error "GL3 constant STENCIL_INDEX8's value is incorrect."
#endif
#if (GL_STENCIL_INDEX16 != 36169)
 #error "GL3 constant STENCIL_INDEX16's value is incorrect."
#endif
#if (GL_RENDERBUFFER_RED_SIZE != 36176)
 #error "GL3 constant RENDERBUFFER_RED_SIZE's value is incorrect."
#endif
#if (GL_RENDERBUFFER_GREEN_SIZE != 36177)
 #error "GL3 constant RENDERBUFFER_GREEN_SIZE's value is incorrect."
#endif
#if (GL_RENDERBUFFER_BLUE_SIZE != 36178)
 #error "GL3 constant RENDERBUFFER_BLUE_SIZE's value is incorrect."
#endif
#if (GL_RENDERBUFFER_ALPHA_SIZE != 36179)
 #error "GL3 constant RENDERBUFFER_ALPHA_SIZE's value is incorrect."
#endif
#if (GL_RENDERBUFFER_DEPTH_SIZE != 36180)
 #error "GL3 constant RENDERBUFFER_DEPTH_SIZE's value is incorrect."
#endif
#if (GL_RENDERBUFFER_STENCIL_SIZE != 36181)
 #error "GL3 constant RENDERBUFFER_STENCIL_SIZE's value is incorrect."
#endif
#if (GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE != 36182)
 #error "GL3 constant FRAMEBUFFER_INCOMPLETE_MULTISAMPLE's value is incorrect."
#endif
#if (GL_MAX_SAMPLES != 36183)
 #error "GL3 constant MAX_SAMPLES's value is incorrect."
#endif
#if (GL_FRAMEBUFFER_SRGB != 36281)
 #error "GL3 constant FRAMEBUFFER_SRGB's value is incorrect."
#endif
#if (GL_HALF_FLOAT != 5131)
 #error "GL3 constant HALF_FLOAT's value is incorrect."
#endif
#if (GL_MAP_READ_BIT != 1)
 #error "GL3 constant MAP_READ_BIT's value is incorrect."
#endif
#if (GL_MAP_WRITE_BIT != 2)
 #error "GL3 constant MAP_WRITE_BIT's value is incorrect."
#endif
#if (GL_MAP_INVALIDATE_RANGE_BIT != 4)
 #error "GL3 constant MAP_INVALIDATE_RANGE_BIT's value is incorrect."
#endif
#if (GL_MAP_INVALIDATE_BUFFER_BIT != 8)
 #error "GL3 constant MAP_INVALIDATE_BUFFER_BIT's value is incorrect."
#endif
#if (GL_MAP_FLUSH_EXPLICIT_BIT != 16)
 #error "GL3 constant MAP_FLUSH_EXPLICIT_BIT's value is incorrect."
#endif
#if (GL_MAP_UNSYNCHRONIZED_BIT != 32)
 #error "GL3 constant MAP_UNSYNCHRONIZED_BIT's value is incorrect."
#endif
#if (GL_COMPRESSED_RED_RGTC1 != 36283)
 #error "GL3 constant COMPRESSED_RED_RGTC1's value is incorrect."
#endif
#if (GL_COMPRESSED_SIGNED_RED_RGTC1 != 36284)
 #error "GL3 constant COMPRESSED_SIGNED_RED_RGTC1's value is incorrect."
#endif
#if (GL_COMPRESSED_RG_RGTC2 != 36285)
 #error "GL3 constant COMPRESSED_RG_RGTC2's value is incorrect."
#endif
#if (GL_COMPRESSED_SIGNED_RG_RGTC2 != 36286)
 #error "GL3 constant COMPRESSED_SIGNED_RG_RGTC2's value is incorrect."
#endif
#if (GL_RG != 33319)
 #error "GL3 constant RG's value is incorrect."
#endif
#if (GL_RG_INTEGER != 33320)
 #error "GL3 constant RG_INTEGER's value is incorrect."
#endif
#if (GL_R8 != 33321)
 #error "GL3 constant R8's value is incorrect."
#endif
#if (GL_R16 != 33322)
 #error "GL3 constant R16's value is incorrect."
#endif
#if (GL_RG8 != 33323)
 #error "GL3 constant RG8's value is incorrect."
#endif
#if (GL_RG16 != 33324)
 #error "GL3 constant RG16's value is incorrect."
#endif
#if (GL_R16F != 33325)
 #error "GL3 constant R16F's value is incorrect."
#endif
#if (GL_R32F != 33326)
 #error "GL3 constant R32F's value is incorrect."
#endif
#if (GL_RG16F != 33327)
 #error "GL3 constant RG16F's value is incorrect."
#endif
#if (GL_RG32F != 33328)
 #error "GL3 constant RG32F's value is incorrect."
#endif
#if (GL_R8I != 33329)
 #error "GL3 constant R8I's value is incorrect."
#endif
#if (GL_R8UI != 33330)
 #error "GL3 constant R8UI's value is incorrect."
#endif
#if (GL_R16I != 33331)
 #error "GL3 constant R16I's value is incorrect."
#endif
#if (GL_R16UI != 33332)
 #error "GL3 constant R16UI's value is incorrect."
#endif
#if (GL_R32I != 33333)
 #error "GL3 constant R32I's value is incorrect."
#endif
#if (GL_R32UI != 33334)
 #error "GL3 constant R32UI's value is incorrect."
#endif
#if (GL_RG8I != 33335)
 #error "GL3 constant RG8I's value is incorrect."
#endif
#if (GL_RG8UI != 33336)
 #error "GL3 constant RG8UI's value is incorrect."
#endif
#if (GL_RG16I != 33337)
 #error "GL3 constant RG16I's value is incorrect."
#endif
#if (GL_RG16UI != 33338)
 #error "GL3 constant RG16UI's value is incorrect."
#endif
#if (GL_RG32I != 33339)
 #error "GL3 constant RG32I's value is incorrect."
#endif
#if (GL_RG32UI != 33340)
 #error "GL3 constant RG32UI's value is incorrect."
#endif
#if (GL_VERTEX_ARRAY_BINDING != 34229)
 #error "GL3 constant VERTEX_ARRAY_BINDING's value is incorrect."
#endif
#if (GL_UNIFORM_BUFFER != 35345)
 #error "GL3 constant UNIFORM_BUFFER's value is incorrect."
#endif
#if (GL_UNIFORM_BUFFER_BINDING != 35368)
 #error "GL3 constant UNIFORM_BUFFER_BINDING's value is incorrect."
#endif
#if (GL_UNIFORM_BUFFER_START != 35369)
 #error "GL3 constant UNIFORM_BUFFER_START's value is incorrect."
#endif
#if (GL_UNIFORM_BUFFER_SIZE != 35370)
 #error "GL3 constant UNIFORM_BUFFER_SIZE's value is incorrect."
#endif
#if (GL_MAX_VERTEX_UNIFORM_BLOCKS != 35371)
 #error "GL3 constant MAX_VERTEX_UNIFORM_BLOCKS's value is incorrect."
#endif
#if (GL_MAX_GEOMETRY_UNIFORM_BLOCKS != 35372)
 #error "GL3 constant MAX_GEOMETRY_UNIFORM_BLOCKS's value is incorrect."
#endif
#if (GL_MAX_FRAGMENT_UNIFORM_BLOCKS != 35373)
 #error "GL3 constant MAX_FRAGMENT_UNIFORM_BLOCKS's value is incorrect."
#endif
#if (GL_MAX_COMBINED_UNIFORM_BLOCKS != 35374)
 #error "GL3 constant MAX_COMBINED_UNIFORM_BLOCKS's value is incorrect."
#endif
#if (GL_MAX_UNIFORM_BUFFER_BINDINGS != 35375)
 #error "GL3 constant MAX_UNIFORM_BUFFER_BINDINGS's value is incorrect."
#endif
#if (GL_MAX_UNIFORM_BLOCK_SIZE != 35376)
 #error "GL3 constant MAX_UNIFORM_BLOCK_SIZE's value is incorrect."
#endif
#if (GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS != 35377)
 #error "GL3 constant MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS's value is incorrect."
#endif
#if (GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS != 35378)
 #error "GL3 constant MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS's value is incorrect."
#endif
#if (GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS != 35379)
 #error "GL3 constant MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS's value is incorrect."
#endif
#if (GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT != 35380)
 #error "GL3 constant UNIFORM_BUFFER_OFFSET_ALIGNMENT's value is incorrect."
#endif
#if (GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH != 35381)
 #error "GL3 constant ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH's value is incorrect."
#endif
#if (GL_ACTIVE_UNIFORM_BLOCKS != 35382)
 #error "GL3 constant ACTIVE_UNIFORM_BLOCKS's value is incorrect."
#endif
#if (GL_UNIFORM_TYPE != 35383)
 #error "GL3 constant UNIFORM_TYPE's value is incorrect."
#endif
#if (GL_UNIFORM_SIZE != 35384)
 #error "GL3 constant UNIFORM_SIZE's value is incorrect."
#endif
#if (GL_UNIFORM_NAME_LENGTH != 35385)
 #error "GL3 constant UNIFORM_NAME_LENGTH's value is incorrect."
#endif
#if (GL_UNIFORM_BLOCK_INDEX != 35386)
 #error "GL3 constant UNIFORM_BLOCK_INDEX's value is incorrect."
#endif
#if (GL_UNIFORM_OFFSET != 35387)
 #error "GL3 constant UNIFORM_OFFSET's value is incorrect."
#endif
#if (GL_UNIFORM_ARRAY_STRIDE != 35388)
 #error "GL3 constant UNIFORM_ARRAY_STRIDE's value is incorrect."
#endif
#if (GL_UNIFORM_MATRIX_STRIDE != 35389)
 #error "GL3 constant UNIFORM_MATRIX_STRIDE's value is incorrect."
#endif
#if (GL_UNIFORM_IS_ROW_MAJOR != 35390)
 #error "GL3 constant UNIFORM_IS_ROW_MAJOR's value is incorrect."
#endif
#if (GL_UNIFORM_BLOCK_BINDING != 35391)
 #error "GL3 constant UNIFORM_BLOCK_BINDING's value is incorrect."
#endif
#if (GL_UNIFORM_BLOCK_DATA_SIZE != 35392)
 #error "GL3 constant UNIFORM_BLOCK_DATA_SIZE's value is incorrect."
#endif
#if (GL_UNIFORM_BLOCK_NAME_LENGTH != 35393)
 #error "GL3 constant UNIFORM_BLOCK_NAME_LENGTH's value is incorrect."
#endif
#if (GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS != 35394)
 #error "GL3 constant UNIFORM_BLOCK_ACTIVE_UNIFORMS's value is incorrect."
#endif
#if (GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES != 35395)
 #error "GL3 constant UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES's value is incorrect."
#endif
#if (GL_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER != 35396)
 #error "GL3 constant UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER's value is incorrect."
#endif
#if (GL_UNIFORM_BLOCK_REFERENCED_BY_GEOMETRY_SHADER != 35397)
 #error "GL3 constant UNIFORM_BLOCK_REFERENCED_BY_GEOMETRY_SHADER's value is incorrect."
#endif
#if (GL_UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER != 35398)
 #error "GL3 constant UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER's value is incorrect."
#endif
#if (GL_COPY_READ_BUFFER != 36662)
 #error "GL3 constant COPY_READ_BUFFER's value is incorrect."
#endif
#if (GL_COPY_WRITE_BUFFER != 36663)
 #error "GL3 constant COPY_WRITE_BUFFER's value is incorrect."
#endif
#if (GL_DEPTH_CLAMP != 34383)
 #error "GL3 constant DEPTH_CLAMP's value is incorrect."
#endif
#if (GL_FIRST_VERTEX_CONVENTION != 36429)
 #error "GL3 constant FIRST_VERTEX_CONVENTION's value is incorrect."
#endif
#if (GL_LAST_VERTEX_CONVENTION != 36430)
 #error "GL3 constant LAST_VERTEX_CONVENTION's value is incorrect."
#endif
#if (GL_PROVOKING_VERTEX != 36431)
 #error "GL3 constant PROVOKING_VERTEX's value is incorrect."
#endif
#if (GL_TEXTURE_CUBE_MAP_SEAMLESS != 34895)
 #error "GL3 constant TEXTURE_CUBE_MAP_SEAMLESS's value is incorrect."
#endif
#if (GL_MAX_SERVER_WAIT_TIMEOUT != 37137)
 #error "GL3 constant MAX_SERVER_WAIT_TIMEOUT's value is incorrect."
#endif
#if (GL_OBJECT_TYPE != 37138)
 #error "GL3 constant OBJECT_TYPE's value is incorrect."
#endif
#if (GL_SYNC_CONDITION != 37139)
 #error "GL3 constant SYNC_CONDITION's value is incorrect."
#endif
#if (GL_SYNC_STATUS != 37140)
 #error "GL3 constant SYNC_STATUS's value is incorrect."
#endif
#if (GL_SYNC_FLAGS != 37141)
 #error "GL3 constant SYNC_FLAGS's value is incorrect."
#endif
#if (GL_SYNC_FENCE != 37142)
 #error "GL3 constant SYNC_FENCE's value is incorrect."
#endif
#if (GL_SYNC_GPU_COMMANDS_COMPLETE != 37143)
 #error "GL3 constant SYNC_GPU_COMMANDS_COMPLETE's value is incorrect."
#endif
#if (GL_UNSIGNALED != 37144)
 #error "GL3 constant UNSIGNALED's value is incorrect."
#endif
#if (GL_SIGNALED != 37145)
 #error "GL3 constant SIGNALED's value is incorrect."
#endif
#if (GL_ALREADY_SIGNALED != 37146)
 #error "GL3 constant ALREADY_SIGNALED's value is incorrect."
#endif
#if (GL_TIMEOUT_EXPIRED != 37147)
 #error "GL3 constant TIMEOUT_EXPIRED's value is incorrect."
#endif
#if (GL_CONDITION_SATISFIED != 37148)
 #error "GL3 constant CONDITION_SATISFIED's value is incorrect."
#endif
#if (GL_WAIT_FAILED != 37149)
 #error "GL3 constant WAIT_FAILED's value is incorrect."
#endif
#if (GL_SYNC_FLUSH_COMMANDS_BIT != 1)
 #error "GL3 constant SYNC_FLUSH_COMMANDS_BIT's value is incorrect."
#endif
#if (GL_SAMPLE_POSITION != 36432)
 #error "GL3 constant SAMPLE_POSITION's value is incorrect."
#endif
#if (GL_SAMPLE_MASK != 36433)
 #error "GL3 constant SAMPLE_MASK's value is incorrect."
#endif
#if (GL_SAMPLE_MASK_VALUE != 36434)
 #error "GL3 constant SAMPLE_MASK_VALUE's value is incorrect."
#endif
#if (GL_MAX_SAMPLE_MASK_WORDS != 36441)
 #error "GL3 constant MAX_SAMPLE_MASK_WORDS's value is incorrect."
#endif
#if (GL_TEXTURE_2D_MULTISAMPLE != 37120)
 #error "GL3 constant TEXTURE_2D_MULTISAMPLE's value is incorrect."
#endif
#if (GL_PROXY_TEXTURE_2D_MULTISAMPLE != 37121)
 #error "GL3 constant PROXY_TEXTURE_2D_MULTISAMPLE's value is incorrect."
#endif
#if (GL_TEXTURE_2D_MULTISAMPLE_ARRAY != 37122)
 #error "GL3 constant TEXTURE_2D_MULTISAMPLE_ARRAY's value is incorrect."
#endif
#if (GL_PROXY_TEXTURE_2D_MULTISAMPLE_ARRAY != 37123)
 #error "GL3 constant PROXY_TEXTURE_2D_MULTISAMPLE_ARRAY's value is incorrect."
#endif
#if (GL_TEXTURE_BINDING_2D_MULTISAMPLE != 37124)
 #error "GL3 constant TEXTURE_BINDING_2D_MULTISAMPLE's value is incorrect."
#endif
#if (GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY != 37125)
 #error "GL3 constant TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY's value is incorrect."
#endif
#if (GL_TEXTURE_SAMPLES != 37126)
 #error "GL3 constant TEXTURE_SAMPLES's value is incorrect."
#endif
#if (GL_TEXTURE_FIXED_SAMPLE_LOCATIONS != 37127)
 #error "GL3 constant TEXTURE_FIXED_SAMPLE_LOCATIONS's value is incorrect."
#endif
#if (GL_SAMPLER_2D_MULTISAMPLE != 37128)
 #error "GL3 constant SAMPLER_2D_MULTISAMPLE's value is incorrect."
#endif
#if (GL_INT_SAMPLER_2D_MULTISAMPLE != 37129)
 #error "GL3 constant INT_SAMPLER_2D_MULTISAMPLE's value is incorrect."
#endif
#if (GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE != 37130)
 #error "GL3 constant UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE's value is incorrect."
#endif
#if (GL_SAMPLER_2D_MULTISAMPLE_ARRAY != 37131)
 #error "GL3 constant SAMPLER_2D_MULTISAMPLE_ARRAY's value is incorrect."
#endif
#if (GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY != 37132)
 #error "GL3 constant INT_SAMPLER_2D_MULTISAMPLE_ARRAY's value is incorrect."
#endif
#if (GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY != 37133)
 #error "GL3 constant UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY's value is incorrect."
#endif
#if (GL_MAX_COLOR_TEXTURE_SAMPLES != 37134)
 #error "GL3 constant MAX_COLOR_TEXTURE_SAMPLES's value is incorrect."
#endif
#if (GL_MAX_DEPTH_TEXTURE_SAMPLES != 37135)
 #error "GL3 constant MAX_DEPTH_TEXTURE_SAMPLES's value is incorrect."
#endif
#if (GL_MAX_INTEGER_SAMPLES != 37136)
 #error "GL3 constant MAX_INTEGER_SAMPLES's value is incorrect."
#endif
#if (GL_SRC1_COLOR != 35065)
 #error "GL3 constant SRC1_COLOR's value is incorrect."
#endif
#if (GL_SRC1_ALPHA != 34185)
 #error "GL3 constant SRC1_ALPHA's value is incorrect."
#endif
#if (GL_ONE_MINUS_SRC1_COLOR != 35066)
 #error "GL3 constant ONE_MINUS_SRC1_COLOR's value is incorrect."
#endif
#if (GL_ONE_MINUS_SRC1_ALPHA != 35067)
 #error "GL3 constant ONE_MINUS_SRC1_ALPHA's value is incorrect."
#endif
#if (GL_MAX_DUAL_SOURCE_DRAW_BUFFERS != 35068)
 #error "GL3 constant MAX_DUAL_SOURCE_DRAW_BUFFERS's value is incorrect."
#endif
#if (GL_ANY_SAMPLES_PASSED != 35887)
 #error "GL3 constant ANY_SAMPLES_PASSED's value is incorrect."
#endif
#if (GL_SAMPLER_BINDING != 35097)
 #error "GL3 constant SAMPLER_BINDING's value is incorrect."
#endif
#if (GL_RGB10_A2UI != 36975)
 #error "GL3 constant RGB10_A2UI's value is incorrect."
#endif
#if (GL_TEXTURE_SWIZZLE_R != 36418)
 #error "GL3 constant TEXTURE_SWIZZLE_R's value is incorrect."
#endif
#if (GL_TEXTURE_SWIZZLE_G != 36419)
 #error "GL3 constant TEXTURE_SWIZZLE_G's value is incorrect."
#endif
#if (GL_TEXTURE_SWIZZLE_B != 36420)
 #error "GL3 constant TEXTURE_SWIZZLE_B's value is incorrect."
#endif
#if (GL_TEXTURE_SWIZZLE_A != 36421)
 #error "GL3 constant TEXTURE_SWIZZLE_A's value is incorrect."
#endif
#if (GL_TEXTURE_SWIZZLE_RGBA != 36422)
 #error "GL3 constant TEXTURE_SWIZZLE_RGBA's value is incorrect."
#endif
#if (GL_TIME_ELAPSED != 35007)
 #error "GL3 constant TIME_ELAPSED's value is incorrect."
#endif
#if (GL_TIMESTAMP != 36392)
 #error "GL3 constant TIMESTAMP's value is incorrect."
#endif
#if (GL_INT_2_10_10_10_REV != 36255)
 #error "GL3 constant INT_2_10_10_10_REV's value is incorrect."
#endif
#if (GL_DRAW_INDIRECT_BUFFER != 36671)
 #error "GL3 constant DRAW_INDIRECT_BUFFER's value is incorrect."
#endif
#if (GL_DRAW_INDIRECT_BUFFER_BINDING != 36675)
 #error "GL3 constant DRAW_INDIRECT_BUFFER_BINDING's value is incorrect."
#endif
#if (GL_GEOMETRY_SHADER_INVOCATIONS != 34943)
 #error "GL3 constant GEOMETRY_SHADER_INVOCATIONS's value is incorrect."
#endif
#if (GL_MAX_GEOMETRY_SHADER_INVOCATIONS != 36442)
 #error "GL3 constant MAX_GEOMETRY_SHADER_INVOCATIONS's value is incorrect."
#endif
#if (GL_MIN_FRAGMENT_INTERPOLATION_OFFSET != 36443)
 #error "GL3 constant MIN_FRAGMENT_INTERPOLATION_OFFSET's value is incorrect."
#endif
#if (GL_MAX_FRAGMENT_INTERPOLATION_OFFSET != 36444)
 #error "GL3 constant MAX_FRAGMENT_INTERPOLATION_OFFSET's value is incorrect."
#endif
#if (GL_FRAGMENT_INTERPOLATION_OFFSET_BITS != 36445)
 #error "GL3 constant FRAGMENT_INTERPOLATION_OFFSET_BITS's value is incorrect."
#endif
#if (GL_DOUBLE_VEC2 != 36860)
 #error "GL3 constant DOUBLE_VEC2's value is incorrect."
#endif
#if (GL_DOUBLE_VEC3 != 36861)
 #error "GL3 constant DOUBLE_VEC3's value is incorrect."
#endif
#if (GL_DOUBLE_VEC4 != 36862)
 #error "GL3 constant DOUBLE_VEC4's value is incorrect."
#endif
#if (GL_DOUBLE_MAT2 != 36678)
 #error "GL3 constant DOUBLE_MAT2's value is incorrect."
#endif
#if (GL_DOUBLE_MAT3 != 36679)
 #error "GL3 constant DOUBLE_MAT3's value is incorrect."
#endif
#if (GL_DOUBLE_MAT4 != 36680)
 #error "GL3 constant DOUBLE_MAT4's value is incorrect."
#endif
#if (GL_DOUBLE_MAT2x3 != 36681)
 #error "GL3 constant DOUBLE_MAT2x3's value is incorrect."
#endif
#if (GL_DOUBLE_MAT2x4 != 36682)
 #error "GL3 constant DOUBLE_MAT2x4's value is incorrect."
#endif
#if (GL_DOUBLE_MAT3x2 != 36683)
 #error "GL3 constant DOUBLE_MAT3x2's value is incorrect."
#endif
#if (GL_DOUBLE_MAT3x4 != 36684)
 #error "GL3 constant DOUBLE_MAT3x4's value is incorrect."
#endif
#if (GL_DOUBLE_MAT4x2 != 36685)
 #error "GL3 constant DOUBLE_MAT4x2's value is incorrect."
#endif
#if (GL_DOUBLE_MAT4x3 != 36686)
 #error "GL3 constant DOUBLE_MAT4x3's value is incorrect."
#endif
#if (GL_ACTIVE_SUBROUTINES != 36325)
 #error "GL3 constant ACTIVE_SUBROUTINES's value is incorrect."
#endif
#if (GL_ACTIVE_SUBROUTINE_UNIFORMS != 36326)
 #error "GL3 constant ACTIVE_SUBROUTINE_UNIFORMS's value is incorrect."
#endif
#if (GL_ACTIVE_SUBROUTINE_UNIFORM_LOCATIONS != 36423)
 #error "GL3 constant ACTIVE_SUBROUTINE_UNIFORM_LOCATIONS's value is incorrect."
#endif
#if (GL_ACTIVE_SUBROUTINE_MAX_LENGTH != 36424)
 #error "GL3 constant ACTIVE_SUBROUTINE_MAX_LENGTH's value is incorrect."
#endif
#if (GL_ACTIVE_SUBROUTINE_UNIFORM_MAX_LENGTH != 36425)
 #error "GL3 constant ACTIVE_SUBROUTINE_UNIFORM_MAX_LENGTH's value is incorrect."
#endif
#if (GL_MAX_SUBROUTINES != 36327)
 #error "GL3 constant MAX_SUBROUTINES's value is incorrect."
#endif
#if (GL_MAX_SUBROUTINE_UNIFORM_LOCATIONS != 36328)
 #error "GL3 constant MAX_SUBROUTINE_UNIFORM_LOCATIONS's value is incorrect."
#endif
#if (GL_NUM_COMPATIBLE_SUBROUTINES != 36426)
 #error "GL3 constant NUM_COMPATIBLE_SUBROUTINES's value is incorrect."
#endif
#if (GL_COMPATIBLE_SUBROUTINES != 36427)
 #error "GL3 constant COMPATIBLE_SUBROUTINES's value is incorrect."
#endif
#if (GL_PATCHES != 14)
 #error "GL3 constant PATCHES's value is incorrect."
#endif
#if (GL_PATCH_VERTICES != 36466)
 #error "GL3 constant PATCH_VERTICES's value is incorrect."
#endif
#if (GL_PATCH_DEFAULT_INNER_LEVEL != 36467)
 #error "GL3 constant PATCH_DEFAULT_INNER_LEVEL's value is incorrect."
#endif
#if (GL_PATCH_DEFAULT_OUTER_LEVEL != 36468)
 #error "GL3 constant PATCH_DEFAULT_OUTER_LEVEL's value is incorrect."
#endif
#if (GL_TESS_CONTROL_OUTPUT_VERTICES != 36469)
 #error "GL3 constant TESS_CONTROL_OUTPUT_VERTICES's value is incorrect."
#endif
#if (GL_TESS_GEN_MODE != 36470)
 #error "GL3 constant TESS_GEN_MODE's value is incorrect."
#endif
#if (GL_TESS_GEN_SPACING != 36471)
 #error "GL3 constant TESS_GEN_SPACING's value is incorrect."
#endif
#if (GL_TESS_GEN_VERTEX_ORDER != 36472)
 #error "GL3 constant TESS_GEN_VERTEX_ORDER's value is incorrect."
#endif
#if (GL_TESS_GEN_POINT_MODE != 36473)
 #error "GL3 constant TESS_GEN_POINT_MODE's value is incorrect."
#endif
#if (GL_QUADS != 7)
 #error "GL3 constant QUADS's value is incorrect."
#endif
#if (GL_ISOLINES != 36474)
 #error "GL3 constant ISOLINES's value is incorrect."
#endif
#if (GL_FRACTIONAL_ODD != 36475)
 #error "GL3 constant FRACTIONAL_ODD's value is incorrect."
#endif
#if (GL_FRACTIONAL_EVEN != 36476)
 #error "GL3 constant FRACTIONAL_EVEN's value is incorrect."
#endif
#if (GL_MAX_PATCH_VERTICES != 36477)
 #error "GL3 constant MAX_PATCH_VERTICES's value is incorrect."
#endif
#if (GL_MAX_TESS_GEN_LEVEL != 36478)
 #error "GL3 constant MAX_TESS_GEN_LEVEL's value is incorrect."
#endif
#if (GL_MAX_TESS_CONTROL_UNIFORM_COMPONENTS != 36479)
 #error "GL3 constant MAX_TESS_CONTROL_UNIFORM_COMPONENTS's value is incorrect."
#endif
#if (GL_MAX_TESS_EVALUATION_UNIFORM_COMPONENTS != 36480)
 #error "GL3 constant MAX_TESS_EVALUATION_UNIFORM_COMPONENTS's value is incorrect."
#endif
#if (GL_MAX_TESS_CONTROL_TEXTURE_IMAGE_UNITS != 36481)
 #error "GL3 constant MAX_TESS_CONTROL_TEXTURE_IMAGE_UNITS's value is incorrect."
#endif
#if (GL_MAX_TESS_EVALUATION_TEXTURE_IMAGE_UNITS != 36482)
 #error "GL3 constant MAX_TESS_EVALUATION_TEXTURE_IMAGE_UNITS's value is incorrect."
#endif
#if (GL_MAX_TESS_CONTROL_OUTPUT_COMPONENTS != 36483)
 #error "GL3 constant MAX_TESS_CONTROL_OUTPUT_COMPONENTS's value is incorrect."
#endif
#if (GL_MAX_TESS_PATCH_COMPONENTS != 36484)
 #error "GL3 constant MAX_TESS_PATCH_COMPONENTS's value is incorrect."
#endif
#if (GL_MAX_TESS_CONTROL_TOTAL_OUTPUT_COMPONENTS != 36485)
 #error "GL3 constant MAX_TESS_CONTROL_TOTAL_OUTPUT_COMPONENTS's value is incorrect."
#endif
#if (GL_MAX_TESS_EVALUATION_OUTPUT_COMPONENTS != 36486)
 #error "GL3 constant MAX_TESS_EVALUATION_OUTPUT_COMPONENTS's value is incorrect."
#endif
#if (GL_MAX_TESS_CONTROL_UNIFORM_BLOCKS != 36489)
 #error "GL3 constant MAX_TESS_CONTROL_UNIFORM_BLOCKS's value is incorrect."
#endif
#if (GL_MAX_TESS_EVALUATION_UNIFORM_BLOCKS != 36490)
 #error "GL3 constant MAX_TESS_EVALUATION_UNIFORM_BLOCKS's value is incorrect."
#endif
#if (GL_MAX_TESS_CONTROL_INPUT_COMPONENTS != 34924)
 #error "GL3 constant MAX_TESS_CONTROL_INPUT_COMPONENTS's value is incorrect."
#endif
#if (GL_MAX_TESS_EVALUATION_INPUT_COMPONENTS != 34925)
 #error "GL3 constant MAX_TESS_EVALUATION_INPUT_COMPONENTS's value is incorrect."
#endif
#if (GL_MAX_COMBINED_TESS_CONTROL_UNIFORM_COMPONENTS != 36382)
 #error "GL3 constant MAX_COMBINED_TESS_CONTROL_UNIFORM_COMPONENTS's value is incorrect."
#endif
#if (GL_MAX_COMBINED_TESS_EVALUATION_UNIFORM_COMPONENTS != 36383)
 #error "GL3 constant MAX_COMBINED_TESS_EVALUATION_UNIFORM_COMPONENTS's value is incorrect."
#endif
#if (GL_UNIFORM_BLOCK_REFERENCED_BY_TESS_CONTROL_SHADER != 34032)
 #error "GL3 constant UNIFORM_BLOCK_REFERENCED_BY_TESS_CONTROL_SHADER's value is incorrect."
#endif
#if (GL_UNIFORM_BLOCK_REFERENCED_BY_TESS_EVALUATION_SHADER != 34033)
 #error "GL3 constant UNIFORM_BLOCK_REFERENCED_BY_TESS_EVALUATION_SHADER's value is incorrect."
#endif
#if (GL_TESS_EVALUATION_SHADER != 36487)
 #error "GL3 constant TESS_EVALUATION_SHADER's value is incorrect."
#endif
#if (GL_TESS_CONTROL_SHADER != 36488)
 #error "GL3 constant TESS_CONTROL_SHADER's value is incorrect."
#endif
#if (GL_TRANSFORM_FEEDBACK != 36386)
 #error "GL3 constant TRANSFORM_FEEDBACK's value is incorrect."
#endif
#if (GL_TRANSFORM_FEEDBACK_BUFFER_PAUSED != 36387)
 #error "GL3 constant TRANSFORM_FEEDBACK_BUFFER_PAUSED's value is incorrect."
#endif
#if (GL_TRANSFORM_FEEDBACK_BUFFER_ACTIVE != 36388)
 #error "GL3 constant TRANSFORM_FEEDBACK_BUFFER_ACTIVE's value is incorrect."
#endif
#if (GL_TRANSFORM_FEEDBACK_BINDING != 36389)
 #error "GL3 constant TRANSFORM_FEEDBACK_BINDING's value is incorrect."
#endif
#if (GL_MAX_TRANSFORM_FEEDBACK_BUFFERS != 36464)
 #error "GL3 constant MAX_TRANSFORM_FEEDBACK_BUFFERS's value is incorrect."
#endif
#if (GL_MAX_VERTEX_STREAMS != 36465)
 #error "GL3 constant MAX_VERTEX_STREAMS's value is incorrect."
#endif
#if (GL_FIXED != 5132)
 #error "GL3 constant FIXED's value is incorrect."
#endif
#if (GL_IMPLEMENTATION_COLOR_READ_TYPE != 35738)
 #error "GL3 constant IMPLEMENTATION_COLOR_READ_TYPE's value is incorrect."
#endif
#if (GL_IMPLEMENTATION_COLOR_READ_FORMAT != 35739)
 #error "GL3 constant IMPLEMENTATION_COLOR_READ_FORMAT's value is incorrect."
#endif
#if (GL_LOW_FLOAT != 36336)
 #error "GL3 constant LOW_FLOAT's value is incorrect."
#endif
#if (GL_MEDIUM_FLOAT != 36337)
 #error "GL3 constant MEDIUM_FLOAT's value is incorrect."
#endif
#if (GL_HIGH_FLOAT != 36338)
 #error "GL3 constant HIGH_FLOAT's value is incorrect."
#endif
#if (GL_LOW_INT != 36339)
 #error "GL3 constant LOW_INT's value is incorrect."
#endif
#if (GL_MEDIUM_INT != 36340)
 #error "GL3 constant MEDIUM_INT's value is incorrect."
#endif
#if (GL_HIGH_INT != 36341)
 #error "GL3 constant HIGH_INT's value is incorrect."
#endif
#if (GL_SHADER_COMPILER != 36346)
 #error "GL3 constant SHADER_COMPILER's value is incorrect."
#endif
#if (GL_SHADER_BINARY_FORMATS != 36344)
 #error "GL3 constant SHADER_BINARY_FORMATS's value is incorrect."
#endif
#if (GL_NUM_SHADER_BINARY_FORMATS != 36345)
 #error "GL3 constant NUM_SHADER_BINARY_FORMATS's value is incorrect."
#endif
#if (GL_MAX_VERTEX_UNIFORM_VECTORS != 36347)
 #error "GL3 constant MAX_VERTEX_UNIFORM_VECTORS's value is incorrect."
#endif
#if (GL_MAX_VARYING_VECTORS != 36348)
 #error "GL3 constant MAX_VARYING_VECTORS's value is incorrect."
#endif
#if (GL_MAX_FRAGMENT_UNIFORM_VECTORS != 36349)
 #error "GL3 constant MAX_FRAGMENT_UNIFORM_VECTORS's value is incorrect."
#endif
#if (GL_RGB565 != 36194)
 #error "GL3 constant RGB565's value is incorrect."
#endif
#if (GL_PROGRAM_BINARY_RETRIEVABLE_HINT != 33367)
 #error "GL3 constant PROGRAM_BINARY_RETRIEVABLE_HINT's value is incorrect."
#endif
#if (GL_PROGRAM_BINARY_LENGTH != 34625)
 #error "GL3 constant PROGRAM_BINARY_LENGTH's value is incorrect."
#endif
#if (GL_NUM_PROGRAM_BINARY_FORMATS != 34814)
 #error "GL3 constant NUM_PROGRAM_BINARY_FORMATS's value is incorrect."
#endif
#if (GL_PROGRAM_BINARY_FORMATS != 34815)
 #error "GL3 constant PROGRAM_BINARY_FORMATS's value is incorrect."
#endif
#if (GL_VERTEX_SHADER_BIT != 1)
 #error "GL3 constant VERTEX_SHADER_BIT's value is incorrect."
#endif
#if (GL_FRAGMENT_SHADER_BIT != 2)
 #error "GL3 constant FRAGMENT_SHADER_BIT's value is incorrect."
#endif
#if (GL_GEOMETRY_SHADER_BIT != 4)
 #error "GL3 constant GEOMETRY_SHADER_BIT's value is incorrect."
#endif
#if (GL_TESS_CONTROL_SHADER_BIT != 8)
 #error "GL3 constant TESS_CONTROL_SHADER_BIT's value is incorrect."
#endif
#if (GL_TESS_EVALUATION_SHADER_BIT != 16)
 #error "GL3 constant TESS_EVALUATION_SHADER_BIT's value is incorrect."
#endif
#if (GL_ALL_SHADER_BITS != 4294967295)
 #error "GL3 constant ALL_SHADER_BITS's value is incorrect."
#endif
#if (GL_PROGRAM_SEPARABLE != 33368)
 #error "GL3 constant PROGRAM_SEPARABLE's value is incorrect."
#endif
#if (GL_ACTIVE_PROGRAM != 33369)
 #error "GL3 constant ACTIVE_PROGRAM's value is incorrect."
#endif
#if (GL_PROGRAM_PIPELINE_BINDING != 33370)
 #error "GL3 constant PROGRAM_PIPELINE_BINDING's value is incorrect."
#endif
#if (GL_MAX_VIEWPORTS != 33371)
 #error "GL3 constant MAX_VIEWPORTS's value is incorrect."
#endif
#if (GL_VIEWPORT_SUBPIXEL_BITS != 33372)
 #error "GL3 constant VIEWPORT_SUBPIXEL_BITS's value is incorrect."
#endif
#if (GL_VIEWPORT_BOUNDS_RANGE != 33373)
 #error "GL3 constant VIEWPORT_BOUNDS_RANGE's value is incorrect."
#endif
#if (GL_LAYER_PROVOKING_VERTEX != 33374)
 #error "GL3 constant LAYER_PROVOKING_VERTEX's value is incorrect."
#endif
#if (GL_VIEWPORT_INDEX_PROVOKING_VERTEX != 33375)
 #error "GL3 constant VIEWPORT_INDEX_PROVOKING_VERTEX's value is incorrect."
#endif
#if (GL_UNDEFINED_VERTEX != 33376)
 #error "GL3 constant UNDEFINED_VERTEX's value is incorrect."
#endif
#if (GL_DEPTH_BOUNDS_TEST_EXT != 34960)
 #error "GL3 constant DEPTH_BOUNDS_TEST_EXT's value is incorrect."
#endif
#if (GL_DEPTH_BOUNDS_EXT != 34961)
 #error "GL3 constant DEPTH_BOUNDS_EXT's value is incorrect."
#endif
#if (GL_SCALED_RESOLVE_FASTEST_EXT != 37050)
 #error "GL3 constant SCALED_RESOLVE_FASTEST_EXT's value is incorrect."
#endif
#if (GL_SCALED_RESOLVE_NICEST_EXT != 37051)
 #error "GL3 constant SCALED_RESOLVE_NICEST_EXT's value is incorrect."
#endif
#if (GL_COMPRESSED_RGB_S3TC_DXT1_EXT != 33776)
 #error "GL3 constant COMPRESSED_RGB_S3TC_DXT1_EXT's value is incorrect."
#endif
#if (GL_COMPRESSED_RGBA_S3TC_DXT1_EXT != 33777)
 #error "GL3 constant COMPRESSED_RGBA_S3TC_DXT1_EXT's value is incorrect."
#endif
#if (GL_COMPRESSED_RGBA_S3TC_DXT3_EXT != 33778)
 #error "GL3 constant COMPRESSED_RGBA_S3TC_DXT3_EXT's value is incorrect."
#endif
#if (GL_COMPRESSED_RGBA_S3TC_DXT5_EXT != 33779)
 #error "GL3 constant COMPRESSED_RGBA_S3TC_DXT5_EXT's value is incorrect."
#endif
#if (GL_COMPRESSED_SRGB_S3TC_DXT1_EXT != 35916)
 #error "GL3 constant COMPRESSED_SRGB_S3TC_DXT1_EXT's value is incorrect."
#endif
#if (GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT != 35917)
 #error "GL3 constant COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT's value is incorrect."
#endif
#if (GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT != 35918)
 #error "GL3 constant COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT's value is incorrect."
#endif
#if (GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT != 35919)
 #error "GL3 constant COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT's value is incorrect."
#endif
#if (GL_TEXTURE_MAX_ANISOTROPY_EXT != 34046)
 #error "GL3 constant TEXTURE_MAX_ANISOTROPY_EXT's value is incorrect."
#endif
#if (GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT != 34047)
 #error "GL3 constant MAX_TEXTURE_MAX_ANISOTROPY_EXT's value is incorrect."
#endif
#if (GL_MIRROR_CLAMP_EXT != 34626)
 #error "GL3 constant MIRROR_CLAMP_EXT's value is incorrect."
#endif
#if (GL_MIRROR_CLAMP_TO_EDGE_EXT != 34627)
 #error "GL3 constant MIRROR_CLAMP_TO_EDGE_EXT's value is incorrect."
#endif
#if (GL_MIRROR_CLAMP_TO_BORDER_EXT != 35090)
 #error "GL3 constant MIRROR_CLAMP_TO_BORDER_EXT's value is incorrect."
#endif
#if (GL_TEXTURE_SRGB_DECODE_EXT != 35400)
 #error "GL3 constant TEXTURE_SRGB_DECODE_EXT's value is incorrect."
#endif
#if (GL_DECODE_EXT != 35401)
 #error "GL3 constant DECODE_EXT's value is incorrect."
#endif
#if (GL_SKIP_DECODE_EXT != 35402)
 #error "GL3 constant SKIP_DECODE_EXT's value is incorrect."
#endif
NAN_METHOD(EXPORT_cullFace) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    glCullFace(arg0);
    return;
}
NAN_METHOD(EXPORT_frontFace) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    glFrontFace(arg0);
    return;
}
NAN_METHOD(EXPORT_hint) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    glHint(arg0, arg1);
    return;
}
NAN_METHOD(EXPORT_lineWidth) {
    Nan::HandleScope scope;
    // None
    GLfloat arg0;
    arg0 = (float)info[0]->NumberValue();
    glLineWidth(arg0);
    return;
}
NAN_METHOD(EXPORT_pointSize) {
    Nan::HandleScope scope;
    // None
    GLfloat arg0;
    arg0 = (float)info[0]->NumberValue();
    glPointSize(arg0);
    return;
}
NAN_METHOD(EXPORT_polygonMode) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    glPolygonMode(arg0, arg1);
    return;
}
NAN_METHOD(EXPORT_scissor) {
    Nan::HandleScope scope;
    // None
    GLint arg0;
    arg0 = info[0]->Int32Value();
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLsizei arg2;
    arg2 = info[2]->Int32Value();
    GLsizei arg3;
    arg3 = info[3]->Int32Value();
    glScissor(arg0, arg1, arg2, arg3);
    return;
}
NAN_METHOD(EXPORT_texParameterf) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    GLfloat arg2;
    arg2 = (float)info[2]->NumberValue();
    glTexParameterf(arg0, arg1, arg2);
    return;
}
NAN_METHOD(EXPORT_texParameterfv) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    const GLfloat *arg2;
    GLfloat* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (const GLfloat*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLfloat[arg2_array->Length()];
        arg2 = (const GLfloat*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (const GLfloat*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (const GLfloat*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glTexParameterfv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_texParameteri) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    GLint arg2;
    arg2 = info[2]->Int32Value();
    glTexParameteri(arg0, arg1, arg2);
    return;
}
NAN_METHOD(EXPORT_texParameteriv) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    const GLint *arg2;
    GLint* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (const GLint*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLint[arg2_array->Length()];
        arg2 = (const GLint*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (const GLint*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (const GLint*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glTexParameteriv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_texImage1D) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLint arg2;
    arg2 = info[2]->Int32Value();
    GLsizei arg3;
    arg3 = info[3]->Int32Value();
    GLint arg4;
    arg4 = info[4]->Int32Value();
    GLenum arg5;
    arg5 = info[5]->Uint32Value();
    GLenum arg6;
    arg6 = info[6]->Uint32Value();
    const GLvoid *arg7;
    GLubyte* arg7_nonconst = NULL;
    if(info[7]->IsString()) {
        Nan::Utf8String arg7_utf8(info[7]);
        arg7 = (const GLvoid*)(*arg7_utf8);
    } else if(info[7]->IsArray()) {
        v8::Handle<v8::Array> arg7_array = v8::Handle<v8::Array>::Cast(info[7]);
        arg7_nonconst = new GLubyte[arg7_array->Length()];
        arg7 = (const GLvoid*)arg7_nonconst;
    } else if(info[7]->IsObject()) {
        arg7 = (const GLvoid*)glbind_get_buffer_data(info[7]);
    } else if(info[7]->IsNumber()) {
        arg7 = (const GLvoid*)(info[7]->IntegerValue());
    } else {
        arg7 = NULL;
    }
    glTexImage1D(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
    if(arg7_nonconst) delete [] arg7_nonconst;
    return;
}
NAN_METHOD(EXPORT_texImage2D) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLint arg2;
    arg2 = info[2]->Int32Value();
    GLsizei arg3;
    arg3 = info[3]->Int32Value();
    GLsizei arg4;
    arg4 = info[4]->Int32Value();
    GLint arg5;
    arg5 = info[5]->Int32Value();
    GLenum arg6;
    arg6 = info[6]->Uint32Value();
    GLenum arg7;
    arg7 = info[7]->Uint32Value();
    const GLvoid *arg8;
    GLubyte* arg8_nonconst = NULL;
    if(info[8]->IsString()) {
        Nan::Utf8String arg8_utf8(info[8]);
        arg8 = (const GLvoid*)(*arg8_utf8);
    } else if(info[8]->IsArray()) {
        v8::Handle<v8::Array> arg8_array = v8::Handle<v8::Array>::Cast(info[8]);
        arg8_nonconst = new GLubyte[arg8_array->Length()];
        arg8 = (const GLvoid*)arg8_nonconst;
    } else if(info[8]->IsObject()) {
        arg8 = (const GLvoid*)glbind_get_buffer_data(info[8]);
    } else if(info[8]->IsNumber()) {
        arg8 = (const GLvoid*)(info[8]->IntegerValue());
    } else {
        arg8 = NULL;
    }
    glTexImage2D(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
    if(arg8_nonconst) delete [] arg8_nonconst;
    return;
}
NAN_METHOD(EXPORT_drawBuffer) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    glDrawBuffer(arg0);
    return;
}
NAN_METHOD(EXPORT_clear) {
    Nan::HandleScope scope;
    // None
    GLbitfield arg0;
    arg0 = info[0]->Uint32Value();
    glClear(arg0);
    return;
}
NAN_METHOD(EXPORT_clearColor) {
    Nan::HandleScope scope;
    // None
    GLfloat arg0;
    arg0 = (float)info[0]->NumberValue();
    GLfloat arg1;
    arg1 = (float)info[1]->NumberValue();
    GLfloat arg2;
    arg2 = (float)info[2]->NumberValue();
    GLfloat arg3;
    arg3 = (float)info[3]->NumberValue();
    glClearColor(arg0, arg1, arg2, arg3);
    return;
}
NAN_METHOD(EXPORT_clearStencil) {
    Nan::HandleScope scope;
    // None
    GLint arg0;
    arg0 = info[0]->Int32Value();
    glClearStencil(arg0);
    return;
}
NAN_METHOD(EXPORT_clearDepth) {
    Nan::HandleScope scope;
    // None
    GLclampd arg0;
    arg0 = info[0]->NumberValue();
    glClearDepth(arg0);
    return;
}
NAN_METHOD(EXPORT_stencilMask) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    glStencilMask(arg0);
    return;
}
NAN_METHOD(EXPORT_colorMask) {
    Nan::HandleScope scope;
    // None
    GLboolean arg0;
    arg0 = info[0]->Uint32Value();
    GLboolean arg1;
    arg1 = info[1]->Uint32Value();
    GLboolean arg2;
    arg2 = info[2]->Uint32Value();
    GLboolean arg3;
    arg3 = info[3]->Uint32Value();
    glColorMask(arg0, arg1, arg2, arg3);
    return;
}
NAN_METHOD(EXPORT_depthMask) {
    Nan::HandleScope scope;
    // None
    GLboolean arg0;
    arg0 = info[0]->Uint32Value();
    glDepthMask(arg0);
    return;
}
NAN_METHOD(EXPORT_disable) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    glDisable(arg0);
    return;
}
NAN_METHOD(EXPORT_enable) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    glEnable(arg0);
    return;
}
NAN_METHOD(EXPORT_finish) {
    Nan::HandleScope scope;
    // None
    glFinish();
    return;
}
NAN_METHOD(EXPORT_flush) {
    Nan::HandleScope scope;
    // None
    glFlush();
    return;
}
NAN_METHOD(EXPORT_blendFunc) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    glBlendFunc(arg0, arg1);
    return;
}
NAN_METHOD(EXPORT_logicOp) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    glLogicOp(arg0);
    return;
}
NAN_METHOD(EXPORT_stencilFunc) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLuint arg2;
    arg2 = info[2]->Uint32Value();
    glStencilFunc(arg0, arg1, arg2);
    return;
}
NAN_METHOD(EXPORT_stencilOp) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    GLenum arg2;
    arg2 = info[2]->Uint32Value();
    glStencilOp(arg0, arg1, arg2);
    return;
}
NAN_METHOD(EXPORT_depthFunc) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    glDepthFunc(arg0);
    return;
}
NAN_METHOD(EXPORT_pixelStoref) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLfloat arg1;
    arg1 = (float)info[1]->NumberValue();
    glPixelStoref(arg0, arg1);
    return;
}
NAN_METHOD(EXPORT_pixelStorei) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLint arg1;
    arg1 = info[1]->Int32Value();
    glPixelStorei(arg0, arg1);
    return;
}
NAN_METHOD(EXPORT_readBuffer) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    glReadBuffer(arg0);
    return;
}
NAN_METHOD(EXPORT_readPixels) {
    Nan::HandleScope scope;
    // None
    GLint arg0;
    arg0 = info[0]->Int32Value();
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLsizei arg2;
    arg2 = info[2]->Int32Value();
    GLsizei arg3;
    arg3 = info[3]->Int32Value();
    GLenum arg4;
    arg4 = info[4]->Uint32Value();
    GLenum arg5;
    arg5 = info[5]->Uint32Value();
    GLvoid *arg6;
    GLubyte* arg6_nonconst = NULL;
    if(info[6]->IsString()) {
        Nan::Utf8String arg6_utf8(info[6]);
        arg6 = (GLvoid*)(*arg6_utf8);
    } else if(info[6]->IsArray()) {
        v8::Handle<v8::Array> arg6_array = v8::Handle<v8::Array>::Cast(info[6]);
        arg6_nonconst = new GLubyte[arg6_array->Length()];
        arg6 = (GLvoid*)arg6_nonconst;
    } else if(info[6]->IsObject()) {
        arg6 = (GLvoid*)glbind_get_buffer_data(info[6]);
    } else if(info[6]->IsNumber()) {
        arg6 = (GLvoid*)(info[6]->IntegerValue());
    } else {
        arg6 = NULL;
    }
    glReadPixels(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
    if(arg6_nonconst) delete [] arg6_nonconst;
    return;
}
NAN_METHOD(EXPORT_getBooleanv) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLboolean *arg1;
    GLboolean* arg1_nonconst = NULL;
    if(info[1]->IsString()) {
        Nan::Utf8String arg1_utf8(info[1]);
        arg1 = (GLboolean*)(*arg1_utf8);
    } else if(info[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(info[1]);
        arg1_nonconst = new GLboolean[arg1_array->Length()];
        arg1 = (GLboolean*)arg1_nonconst;
    } else if(info[1]->IsObject()) {
        arg1 = (GLboolean*)glbind_get_buffer_data(info[1]);
    } else if(info[1]->IsNumber()) {
        arg1 = (GLboolean*)(info[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glGetBooleanv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    return;
}
NAN_METHOD(EXPORT_getDoublev) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLdouble *arg1;
    GLdouble* arg1_nonconst = NULL;
    if(info[1]->IsString()) {
        Nan::Utf8String arg1_utf8(info[1]);
        arg1 = (GLdouble*)(*arg1_utf8);
    } else if(info[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(info[1]);
        arg1_nonconst = new GLdouble[arg1_array->Length()];
        arg1 = (GLdouble*)arg1_nonconst;
    } else if(info[1]->IsObject()) {
        arg1 = (GLdouble*)glbind_get_buffer_data(info[1]);
    } else if(info[1]->IsNumber()) {
        arg1 = (GLdouble*)(info[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glGetDoublev(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    return;
}
NAN_METHOD(EXPORT_getError) {
    Nan::HandleScope scope;
    // <gltypes.Type instance at 0x102dd85a8>
    GLenum result;
    result = glGetError();
    v8::Handle<v8::Value> result_js = Nan::New<v8::Uint32>(result);
    return info.GetReturnValue().Set(result_js);
}
NAN_METHOD(EXPORT_getFloatv) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLfloat *arg1;
    GLfloat* arg1_nonconst = NULL;
    if(info[1]->IsString()) {
        Nan::Utf8String arg1_utf8(info[1]);
        arg1 = (GLfloat*)(*arg1_utf8);
    } else if(info[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(info[1]);
        arg1_nonconst = new GLfloat[arg1_array->Length()];
        arg1 = (GLfloat*)arg1_nonconst;
    } else if(info[1]->IsObject()) {
        arg1 = (GLfloat*)glbind_get_buffer_data(info[1]);
    } else if(info[1]->IsNumber()) {
        arg1 = (GLfloat*)(info[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glGetFloatv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    return;
}
NAN_METHOD(EXPORT_getIntegerv) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLint *arg1;
    GLint* arg1_nonconst = NULL;
    if(info[1]->IsString()) {
        Nan::Utf8String arg1_utf8(info[1]);
        arg1 = (GLint*)(*arg1_utf8);
    } else if(info[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(info[1]);
        arg1_nonconst = new GLint[arg1_array->Length()];
        arg1 = (GLint*)arg1_nonconst;
    } else if(info[1]->IsObject()) {
        arg1 = (GLint*)glbind_get_buffer_data(info[1]);
    } else if(info[1]->IsNumber()) {
        arg1 = (GLint*)(info[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glGetIntegerv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    return;
}
NAN_METHOD(EXPORT_getString) {
    Nan::HandleScope scope;
    // <gltypes.UStringType instance at 0x102dd8a70>
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    const GLubyte* result;
    result = glGetString(arg0);
    v8::Handle<v8::Value> result_js;
    if(result) {
        result_js = Nan::New<v8::String>((const char*)result, strlen((const char*)result)).ToLocalChecked();
    } else {
        result_js = Nan::Undefined();
    }
    return info.GetReturnValue().Set(result_js);
}
NAN_METHOD(EXPORT_getTexImage) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLenum arg2;
    arg2 = info[2]->Uint32Value();
    GLenum arg3;
    arg3 = info[3]->Uint32Value();
    GLvoid *arg4;
    GLubyte* arg4_nonconst = NULL;
    if(info[4]->IsString()) {
        Nan::Utf8String arg4_utf8(info[4]);
        arg4 = (GLvoid*)(*arg4_utf8);
    } else if(info[4]->IsArray()) {
        v8::Handle<v8::Array> arg4_array = v8::Handle<v8::Array>::Cast(info[4]);
        arg4_nonconst = new GLubyte[arg4_array->Length()];
        arg4 = (GLvoid*)arg4_nonconst;
    } else if(info[4]->IsObject()) {
        arg4 = (GLvoid*)glbind_get_buffer_data(info[4]);
    } else if(info[4]->IsNumber()) {
        arg4 = (GLvoid*)(info[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glGetTexImage(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    return;
}
NAN_METHOD(EXPORT_getTexParameterfv) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    GLfloat *arg2;
    GLfloat* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (GLfloat*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLfloat[arg2_array->Length()];
        arg2 = (GLfloat*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (GLfloat*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (GLfloat*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetTexParameterfv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_getTexParameteriv) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    GLint *arg2;
    GLint* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (GLint*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLint[arg2_array->Length()];
        arg2 = (GLint*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (GLint*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (GLint*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetTexParameteriv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_getTexLevelParameterfv) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLenum arg2;
    arg2 = info[2]->Uint32Value();
    GLfloat *arg3;
    GLfloat* arg3_nonconst = NULL;
    if(info[3]->IsString()) {
        Nan::Utf8String arg3_utf8(info[3]);
        arg3 = (GLfloat*)(*arg3_utf8);
    } else if(info[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(info[3]);
        arg3_nonconst = new GLfloat[arg3_array->Length()];
        arg3 = (GLfloat*)arg3_nonconst;
    } else if(info[3]->IsObject()) {
        arg3 = (GLfloat*)glbind_get_buffer_data(info[3]);
    } else if(info[3]->IsNumber()) {
        arg3 = (GLfloat*)(info[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glGetTexLevelParameterfv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    return;
}
NAN_METHOD(EXPORT_getTexLevelParameteriv) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLenum arg2;
    arg2 = info[2]->Uint32Value();
    GLint *arg3;
    GLint* arg3_nonconst = NULL;
    if(info[3]->IsString()) {
        Nan::Utf8String arg3_utf8(info[3]);
        arg3 = (GLint*)(*arg3_utf8);
    } else if(info[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(info[3]);
        arg3_nonconst = new GLint[arg3_array->Length()];
        arg3 = (GLint*)arg3_nonconst;
    } else if(info[3]->IsObject()) {
        arg3 = (GLint*)glbind_get_buffer_data(info[3]);
    } else if(info[3]->IsNumber()) {
        arg3 = (GLint*)(info[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glGetTexLevelParameteriv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    return;
}
NAN_METHOD(EXPORT_isEnabled) {
    Nan::HandleScope scope;
    // <gltypes.Type instance at 0x102dd84d0>
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLboolean result;
    result = glIsEnabled(arg0);
    v8::Handle<v8::Value> result_js = Nan::New<v8::Uint32>(result);
    return info.GetReturnValue().Set(result_js);
}
NAN_METHOD(EXPORT_depthRange) {
    Nan::HandleScope scope;
    // None
    GLclampd arg0;
    arg0 = info[0]->NumberValue();
    GLclampd arg1;
    arg1 = info[1]->NumberValue();
    glDepthRange(arg0, arg1);
    return;
}
NAN_METHOD(EXPORT_viewport) {
    Nan::HandleScope scope;
    // None
    GLint arg0;
    arg0 = info[0]->Int32Value();
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLsizei arg2;
    arg2 = info[2]->Int32Value();
    GLsizei arg3;
    arg3 = info[3]->Int32Value();
    glViewport(arg0, arg1, arg2, arg3);
    return;
}
NAN_METHOD(EXPORT_drawArrays) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLsizei arg2;
    arg2 = info[2]->Int32Value();
    glDrawArrays(arg0, arg1, arg2);
    return;
}
NAN_METHOD(EXPORT_drawElements) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLsizei arg1;
    arg1 = info[1]->Int32Value();
    GLenum arg2;
    arg2 = info[2]->Uint32Value();
    const GLvoid *arg3;
    GLubyte* arg3_nonconst = NULL;
    if(info[3]->IsString()) {
        Nan::Utf8String arg3_utf8(info[3]);
        arg3 = (const GLvoid*)(*arg3_utf8);
    } else if(info[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(info[3]);
        arg3_nonconst = new GLubyte[arg3_array->Length()];
        arg3 = (const GLvoid*)arg3_nonconst;
    } else if(info[3]->IsObject()) {
        arg3 = (const GLvoid*)glbind_get_buffer_data(info[3]);
    } else if(info[3]->IsNumber()) {
        arg3 = (const GLvoid*)(info[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glDrawElements(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    return;
}
NAN_METHOD(EXPORT_polygonOffset) {
    Nan::HandleScope scope;
    // None
    GLfloat arg0;
    arg0 = (float)info[0]->NumberValue();
    GLfloat arg1;
    arg1 = (float)info[1]->NumberValue();
    glPolygonOffset(arg0, arg1);
    return;
}
NAN_METHOD(EXPORT_copyTexImage1D) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLenum arg2;
    arg2 = info[2]->Uint32Value();
    GLint arg3;
    arg3 = info[3]->Int32Value();
    GLint arg4;
    arg4 = info[4]->Int32Value();
    GLsizei arg5;
    arg5 = info[5]->Int32Value();
    GLint arg6;
    arg6 = info[6]->Int32Value();
    glCopyTexImage1D(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
    return;
}
NAN_METHOD(EXPORT_copyTexImage2D) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLenum arg2;
    arg2 = info[2]->Uint32Value();
    GLint arg3;
    arg3 = info[3]->Int32Value();
    GLint arg4;
    arg4 = info[4]->Int32Value();
    GLsizei arg5;
    arg5 = info[5]->Int32Value();
    GLsizei arg6;
    arg6 = info[6]->Int32Value();
    GLint arg7;
    arg7 = info[7]->Int32Value();
    glCopyTexImage2D(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
    return;
}
NAN_METHOD(EXPORT_copyTexSubImage1D) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLint arg2;
    arg2 = info[2]->Int32Value();
    GLint arg3;
    arg3 = info[3]->Int32Value();
    GLint arg4;
    arg4 = info[4]->Int32Value();
    GLsizei arg5;
    arg5 = info[5]->Int32Value();
    glCopyTexSubImage1D(arg0, arg1, arg2, arg3, arg4, arg5);
    return;
}
NAN_METHOD(EXPORT_copyTexSubImage2D) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLint arg2;
    arg2 = info[2]->Int32Value();
    GLint arg3;
    arg3 = info[3]->Int32Value();
    GLint arg4;
    arg4 = info[4]->Int32Value();
    GLint arg5;
    arg5 = info[5]->Int32Value();
    GLsizei arg6;
    arg6 = info[6]->Int32Value();
    GLsizei arg7;
    arg7 = info[7]->Int32Value();
    glCopyTexSubImage2D(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
    return;
}
NAN_METHOD(EXPORT_texSubImage1D) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLint arg2;
    arg2 = info[2]->Int32Value();
    GLsizei arg3;
    arg3 = info[3]->Int32Value();
    GLenum arg4;
    arg4 = info[4]->Uint32Value();
    GLenum arg5;
    arg5 = info[5]->Uint32Value();
    const GLvoid *arg6;
    GLubyte* arg6_nonconst = NULL;
    if(info[6]->IsString()) {
        Nan::Utf8String arg6_utf8(info[6]);
        arg6 = (const GLvoid*)(*arg6_utf8);
    } else if(info[6]->IsArray()) {
        v8::Handle<v8::Array> arg6_array = v8::Handle<v8::Array>::Cast(info[6]);
        arg6_nonconst = new GLubyte[arg6_array->Length()];
        arg6 = (const GLvoid*)arg6_nonconst;
    } else if(info[6]->IsObject()) {
        arg6 = (const GLvoid*)glbind_get_buffer_data(info[6]);
    } else if(info[6]->IsNumber()) {
        arg6 = (const GLvoid*)(info[6]->IntegerValue());
    } else {
        arg6 = NULL;
    }
    glTexSubImage1D(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
    if(arg6_nonconst) delete [] arg6_nonconst;
    return;
}
NAN_METHOD(EXPORT_texSubImage2D) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLint arg2;
    arg2 = info[2]->Int32Value();
    GLint arg3;
    arg3 = info[3]->Int32Value();
    GLsizei arg4;
    arg4 = info[4]->Int32Value();
    GLsizei arg5;
    arg5 = info[5]->Int32Value();
    GLenum arg6;
    arg6 = info[6]->Uint32Value();
    GLenum arg7;
    arg7 = info[7]->Uint32Value();
    const GLvoid *arg8;
    GLubyte* arg8_nonconst = NULL;
    if(info[8]->IsString()) {
        Nan::Utf8String arg8_utf8(info[8]);
        arg8 = (const GLvoid*)(*arg8_utf8);
    } else if(info[8]->IsArray()) {
        v8::Handle<v8::Array> arg8_array = v8::Handle<v8::Array>::Cast(info[8]);
        arg8_nonconst = new GLubyte[arg8_array->Length()];
        arg8 = (const GLvoid*)arg8_nonconst;
    } else if(info[8]->IsObject()) {
        arg8 = (const GLvoid*)glbind_get_buffer_data(info[8]);
    } else if(info[8]->IsNumber()) {
        arg8 = (const GLvoid*)(info[8]->IntegerValue());
    } else {
        arg8 = NULL;
    }
    glTexSubImage2D(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
    if(arg8_nonconst) delete [] arg8_nonconst;
    return;
}
NAN_METHOD(EXPORT_bindTexture) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLuint arg1;
    if(info[1]->IsNumber()) {
        arg1 = info[1]->IntegerValue();
    } else {
        arg1 = Nan::ObjectWrap::Unwrap<NODE_Texture>(info[1]->ToObject())->gl_handle;
    }
    glBindTexture(arg0, arg1);
    return;
}
NAN_METHOD(EXPORT_genTextures) {
    Nan::HandleScope scope;
    // None
    GLsizei arg0;
    arg0 = info[0]->Int32Value();
    GLuint* arg1;
    v8::Handle<v8::Array> info_1_array = v8::Handle<v8::Array>::Cast(info[1]);
    arg1 = new GLuint[info_1_array->Length()];
    glGenTextures(arg0, arg1);
    for(uint32_t i = 0; i < info_1_array->Length(); i++) {
        info_1_array->Set(i, NODE_Texture::fromGLHandle(arg1[i]));
    }
    delete [] arg1;
    return;
}
NAN_METHOD(EXPORT_isTexture) {
    Nan::HandleScope scope;
    // <gltypes.Type instance at 0x102dd84d0>
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Texture>(info[0]->ToObject())->gl_handle;
    }
    GLboolean result;
    result = glIsTexture(arg0);
    v8::Handle<v8::Value> result_js = Nan::New<v8::Uint32>(result);
    return info.GetReturnValue().Set(result_js);
}
NAN_METHOD(EXPORT_blendColor) {
    Nan::HandleScope scope;
    // None
    GLfloat arg0;
    arg0 = (float)info[0]->NumberValue();
    GLfloat arg1;
    arg1 = (float)info[1]->NumberValue();
    GLfloat arg2;
    arg2 = (float)info[2]->NumberValue();
    GLfloat arg3;
    arg3 = (float)info[3]->NumberValue();
    glBlendColor(arg0, arg1, arg2, arg3);
    return;
}
NAN_METHOD(EXPORT_blendEquation) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    glBlendEquation(arg0);
    return;
}
NAN_METHOD(EXPORT_drawRangeElements) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLuint arg1;
    arg1 = info[1]->Uint32Value();
    GLuint arg2;
    arg2 = info[2]->Uint32Value();
    GLsizei arg3;
    arg3 = info[3]->Int32Value();
    GLenum arg4;
    arg4 = info[4]->Uint32Value();
    const GLvoid *arg5;
    GLubyte* arg5_nonconst = NULL;
    if(info[5]->IsString()) {
        Nan::Utf8String arg5_utf8(info[5]);
        arg5 = (const GLvoid*)(*arg5_utf8);
    } else if(info[5]->IsArray()) {
        v8::Handle<v8::Array> arg5_array = v8::Handle<v8::Array>::Cast(info[5]);
        arg5_nonconst = new GLubyte[arg5_array->Length()];
        arg5 = (const GLvoid*)arg5_nonconst;
    } else if(info[5]->IsObject()) {
        arg5 = (const GLvoid*)glbind_get_buffer_data(info[5]);
    } else if(info[5]->IsNumber()) {
        arg5 = (const GLvoid*)(info[5]->IntegerValue());
    } else {
        arg5 = NULL;
    }
    glDrawRangeElements(arg0, arg1, arg2, arg3, arg4, arg5);
    if(arg5_nonconst) delete [] arg5_nonconst;
    return;
}
NAN_METHOD(EXPORT_texImage3D) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLint arg2;
    arg2 = info[2]->Int32Value();
    GLsizei arg3;
    arg3 = info[3]->Int32Value();
    GLsizei arg4;
    arg4 = info[4]->Int32Value();
    GLsizei arg5;
    arg5 = info[5]->Int32Value();
    GLint arg6;
    arg6 = info[6]->Int32Value();
    GLenum arg7;
    arg7 = info[7]->Uint32Value();
    GLenum arg8;
    arg8 = info[8]->Uint32Value();
    const GLvoid *arg9;
    GLubyte* arg9_nonconst = NULL;
    if(info[9]->IsString()) {
        Nan::Utf8String arg9_utf8(info[9]);
        arg9 = (const GLvoid*)(*arg9_utf8);
    } else if(info[9]->IsArray()) {
        v8::Handle<v8::Array> arg9_array = v8::Handle<v8::Array>::Cast(info[9]);
        arg9_nonconst = new GLubyte[arg9_array->Length()];
        arg9 = (const GLvoid*)arg9_nonconst;
    } else if(info[9]->IsObject()) {
        arg9 = (const GLvoid*)glbind_get_buffer_data(info[9]);
    } else if(info[9]->IsNumber()) {
        arg9 = (const GLvoid*)(info[9]->IntegerValue());
    } else {
        arg9 = NULL;
    }
    glTexImage3D(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
    if(arg9_nonconst) delete [] arg9_nonconst;
    return;
}
NAN_METHOD(EXPORT_texSubImage3D) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLint arg2;
    arg2 = info[2]->Int32Value();
    GLint arg3;
    arg3 = info[3]->Int32Value();
    GLint arg4;
    arg4 = info[4]->Int32Value();
    GLsizei arg5;
    arg5 = info[5]->Int32Value();
    GLsizei arg6;
    arg6 = info[6]->Int32Value();
    GLsizei arg7;
    arg7 = info[7]->Int32Value();
    GLenum arg8;
    arg8 = info[8]->Uint32Value();
    GLenum arg9;
    arg9 = info[9]->Uint32Value();
    const GLvoid *arg10;
    GLubyte* arg10_nonconst = NULL;
    if(info[10]->IsString()) {
        Nan::Utf8String arg10_utf8(info[10]);
        arg10 = (const GLvoid*)(*arg10_utf8);
    } else if(info[10]->IsArray()) {
        v8::Handle<v8::Array> arg10_array = v8::Handle<v8::Array>::Cast(info[10]);
        arg10_nonconst = new GLubyte[arg10_array->Length()];
        arg10 = (const GLvoid*)arg10_nonconst;
    } else if(info[10]->IsObject()) {
        arg10 = (const GLvoid*)glbind_get_buffer_data(info[10]);
    } else if(info[10]->IsNumber()) {
        arg10 = (const GLvoid*)(info[10]->IntegerValue());
    } else {
        arg10 = NULL;
    }
    glTexSubImage3D(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10);
    if(arg10_nonconst) delete [] arg10_nonconst;
    return;
}
NAN_METHOD(EXPORT_copyTexSubImage3D) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLint arg2;
    arg2 = info[2]->Int32Value();
    GLint arg3;
    arg3 = info[3]->Int32Value();
    GLint arg4;
    arg4 = info[4]->Int32Value();
    GLint arg5;
    arg5 = info[5]->Int32Value();
    GLint arg6;
    arg6 = info[6]->Int32Value();
    GLsizei arg7;
    arg7 = info[7]->Int32Value();
    GLsizei arg8;
    arg8 = info[8]->Int32Value();
    glCopyTexSubImage3D(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
    return;
}
NAN_METHOD(EXPORT_activeTexture) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    glActiveTexture(arg0);
    return;
}
NAN_METHOD(EXPORT_sampleCoverage) {
    Nan::HandleScope scope;
    // None
    GLclampf arg0;
    arg0 = (float)info[0]->NumberValue();
    GLboolean arg1;
    arg1 = info[1]->Uint32Value();
    glSampleCoverage(arg0, arg1);
    return;
}
NAN_METHOD(EXPORT_compressedTexImage3D) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLenum arg2;
    arg2 = info[2]->Uint32Value();
    GLsizei arg3;
    arg3 = info[3]->Int32Value();
    GLsizei arg4;
    arg4 = info[4]->Int32Value();
    GLsizei arg5;
    arg5 = info[5]->Int32Value();
    GLint arg6;
    arg6 = info[6]->Int32Value();
    GLsizei arg7;
    arg7 = info[7]->Int32Value();
    const GLvoid *arg8;
    GLubyte* arg8_nonconst = NULL;
    if(info[8]->IsString()) {
        Nan::Utf8String arg8_utf8(info[8]);
        arg8 = (const GLvoid*)(*arg8_utf8);
    } else if(info[8]->IsArray()) {
        v8::Handle<v8::Array> arg8_array = v8::Handle<v8::Array>::Cast(info[8]);
        arg8_nonconst = new GLubyte[arg8_array->Length()];
        arg8 = (const GLvoid*)arg8_nonconst;
    } else if(info[8]->IsObject()) {
        arg8 = (const GLvoid*)glbind_get_buffer_data(info[8]);
    } else if(info[8]->IsNumber()) {
        arg8 = (const GLvoid*)(info[8]->IntegerValue());
    } else {
        arg8 = NULL;
    }
    glCompressedTexImage3D(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
    if(arg8_nonconst) delete [] arg8_nonconst;
    return;
}
NAN_METHOD(EXPORT_compressedTexImage2D) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLenum arg2;
    arg2 = info[2]->Uint32Value();
    GLsizei arg3;
    arg3 = info[3]->Int32Value();
    GLsizei arg4;
    arg4 = info[4]->Int32Value();
    GLint arg5;
    arg5 = info[5]->Int32Value();
    GLsizei arg6;
    arg6 = info[6]->Int32Value();
    const GLvoid *arg7;
    GLubyte* arg7_nonconst = NULL;
    if(info[7]->IsString()) {
        Nan::Utf8String arg7_utf8(info[7]);
        arg7 = (const GLvoid*)(*arg7_utf8);
    } else if(info[7]->IsArray()) {
        v8::Handle<v8::Array> arg7_array = v8::Handle<v8::Array>::Cast(info[7]);
        arg7_nonconst = new GLubyte[arg7_array->Length()];
        arg7 = (const GLvoid*)arg7_nonconst;
    } else if(info[7]->IsObject()) {
        arg7 = (const GLvoid*)glbind_get_buffer_data(info[7]);
    } else if(info[7]->IsNumber()) {
        arg7 = (const GLvoid*)(info[7]->IntegerValue());
    } else {
        arg7 = NULL;
    }
    glCompressedTexImage2D(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
    if(arg7_nonconst) delete [] arg7_nonconst;
    return;
}
NAN_METHOD(EXPORT_compressedTexImage1D) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLenum arg2;
    arg2 = info[2]->Uint32Value();
    GLsizei arg3;
    arg3 = info[3]->Int32Value();
    GLint arg4;
    arg4 = info[4]->Int32Value();
    GLsizei arg5;
    arg5 = info[5]->Int32Value();
    const GLvoid *arg6;
    GLubyte* arg6_nonconst = NULL;
    if(info[6]->IsString()) {
        Nan::Utf8String arg6_utf8(info[6]);
        arg6 = (const GLvoid*)(*arg6_utf8);
    } else if(info[6]->IsArray()) {
        v8::Handle<v8::Array> arg6_array = v8::Handle<v8::Array>::Cast(info[6]);
        arg6_nonconst = new GLubyte[arg6_array->Length()];
        arg6 = (const GLvoid*)arg6_nonconst;
    } else if(info[6]->IsObject()) {
        arg6 = (const GLvoid*)glbind_get_buffer_data(info[6]);
    } else if(info[6]->IsNumber()) {
        arg6 = (const GLvoid*)(info[6]->IntegerValue());
    } else {
        arg6 = NULL;
    }
    glCompressedTexImage1D(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
    if(arg6_nonconst) delete [] arg6_nonconst;
    return;
}
NAN_METHOD(EXPORT_compressedTexSubImage3D) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLint arg2;
    arg2 = info[2]->Int32Value();
    GLint arg3;
    arg3 = info[3]->Int32Value();
    GLint arg4;
    arg4 = info[4]->Int32Value();
    GLsizei arg5;
    arg5 = info[5]->Int32Value();
    GLsizei arg6;
    arg6 = info[6]->Int32Value();
    GLsizei arg7;
    arg7 = info[7]->Int32Value();
    GLenum arg8;
    arg8 = info[8]->Uint32Value();
    GLsizei arg9;
    arg9 = info[9]->Int32Value();
    const GLvoid *arg10;
    GLubyte* arg10_nonconst = NULL;
    if(info[10]->IsString()) {
        Nan::Utf8String arg10_utf8(info[10]);
        arg10 = (const GLvoid*)(*arg10_utf8);
    } else if(info[10]->IsArray()) {
        v8::Handle<v8::Array> arg10_array = v8::Handle<v8::Array>::Cast(info[10]);
        arg10_nonconst = new GLubyte[arg10_array->Length()];
        arg10 = (const GLvoid*)arg10_nonconst;
    } else if(info[10]->IsObject()) {
        arg10 = (const GLvoid*)glbind_get_buffer_data(info[10]);
    } else if(info[10]->IsNumber()) {
        arg10 = (const GLvoid*)(info[10]->IntegerValue());
    } else {
        arg10 = NULL;
    }
    glCompressedTexSubImage3D(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10);
    if(arg10_nonconst) delete [] arg10_nonconst;
    return;
}
NAN_METHOD(EXPORT_compressedTexSubImage2D) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLint arg2;
    arg2 = info[2]->Int32Value();
    GLint arg3;
    arg3 = info[3]->Int32Value();
    GLsizei arg4;
    arg4 = info[4]->Int32Value();
    GLsizei arg5;
    arg5 = info[5]->Int32Value();
    GLenum arg6;
    arg6 = info[6]->Uint32Value();
    GLsizei arg7;
    arg7 = info[7]->Int32Value();
    const GLvoid *arg8;
    GLubyte* arg8_nonconst = NULL;
    if(info[8]->IsString()) {
        Nan::Utf8String arg8_utf8(info[8]);
        arg8 = (const GLvoid*)(*arg8_utf8);
    } else if(info[8]->IsArray()) {
        v8::Handle<v8::Array> arg8_array = v8::Handle<v8::Array>::Cast(info[8]);
        arg8_nonconst = new GLubyte[arg8_array->Length()];
        arg8 = (const GLvoid*)arg8_nonconst;
    } else if(info[8]->IsObject()) {
        arg8 = (const GLvoid*)glbind_get_buffer_data(info[8]);
    } else if(info[8]->IsNumber()) {
        arg8 = (const GLvoid*)(info[8]->IntegerValue());
    } else {
        arg8 = NULL;
    }
    glCompressedTexSubImage2D(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
    if(arg8_nonconst) delete [] arg8_nonconst;
    return;
}
NAN_METHOD(EXPORT_compressedTexSubImage1D) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLint arg2;
    arg2 = info[2]->Int32Value();
    GLsizei arg3;
    arg3 = info[3]->Int32Value();
    GLenum arg4;
    arg4 = info[4]->Uint32Value();
    GLsizei arg5;
    arg5 = info[5]->Int32Value();
    const GLvoid *arg6;
    GLubyte* arg6_nonconst = NULL;
    if(info[6]->IsString()) {
        Nan::Utf8String arg6_utf8(info[6]);
        arg6 = (const GLvoid*)(*arg6_utf8);
    } else if(info[6]->IsArray()) {
        v8::Handle<v8::Array> arg6_array = v8::Handle<v8::Array>::Cast(info[6]);
        arg6_nonconst = new GLubyte[arg6_array->Length()];
        arg6 = (const GLvoid*)arg6_nonconst;
    } else if(info[6]->IsObject()) {
        arg6 = (const GLvoid*)glbind_get_buffer_data(info[6]);
    } else if(info[6]->IsNumber()) {
        arg6 = (const GLvoid*)(info[6]->IntegerValue());
    } else {
        arg6 = NULL;
    }
    glCompressedTexSubImage1D(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
    if(arg6_nonconst) delete [] arg6_nonconst;
    return;
}
NAN_METHOD(EXPORT_getCompressedTexImage) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLvoid *arg2;
    GLubyte* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (GLvoid*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLubyte[arg2_array->Length()];
        arg2 = (GLvoid*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (GLvoid*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (GLvoid*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetCompressedTexImage(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_blendFuncSeparate) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    GLenum arg2;
    arg2 = info[2]->Uint32Value();
    GLenum arg3;
    arg3 = info[3]->Uint32Value();
    glBlendFuncSeparate(arg0, arg1, arg2, arg3);
    return;
}
NAN_METHOD(EXPORT_multiDrawArrays) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    const GLint *arg1;
    GLint* arg1_nonconst = NULL;
    if(info[1]->IsString()) {
        Nan::Utf8String arg1_utf8(info[1]);
        arg1 = (const GLint*)(*arg1_utf8);
    } else if(info[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(info[1]);
        arg1_nonconst = new GLint[arg1_array->Length()];
        arg1 = (const GLint*)arg1_nonconst;
    } else if(info[1]->IsObject()) {
        arg1 = (const GLint*)glbind_get_buffer_data(info[1]);
    } else if(info[1]->IsNumber()) {
        arg1 = (const GLint*)(info[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    const GLsizei *arg2;
    GLsizei* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (const GLsizei*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLsizei[arg2_array->Length()];
        arg2 = (const GLsizei*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (const GLsizei*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (const GLsizei*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    GLsizei arg3;
    arg3 = info[3]->Int32Value();
    glMultiDrawArrays(arg0, arg1, arg2, arg3);
    if(arg1_nonconst) delete [] arg1_nonconst;
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_pointParameterf) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLfloat arg1;
    arg1 = (float)info[1]->NumberValue();
    glPointParameterf(arg0, arg1);
    return;
}
NAN_METHOD(EXPORT_pointParameterfv) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    const GLfloat *arg1;
    GLfloat* arg1_nonconst = NULL;
    if(info[1]->IsString()) {
        Nan::Utf8String arg1_utf8(info[1]);
        arg1 = (const GLfloat*)(*arg1_utf8);
    } else if(info[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(info[1]);
        arg1_nonconst = new GLfloat[arg1_array->Length()];
        arg1 = (const GLfloat*)arg1_nonconst;
    } else if(info[1]->IsObject()) {
        arg1 = (const GLfloat*)glbind_get_buffer_data(info[1]);
    } else if(info[1]->IsNumber()) {
        arg1 = (const GLfloat*)(info[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glPointParameterfv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    return;
}
NAN_METHOD(EXPORT_pointParameteri) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLint arg1;
    arg1 = info[1]->Int32Value();
    glPointParameteri(arg0, arg1);
    return;
}
NAN_METHOD(EXPORT_pointParameteriv) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    const GLint *arg1;
    GLint* arg1_nonconst = NULL;
    if(info[1]->IsString()) {
        Nan::Utf8String arg1_utf8(info[1]);
        arg1 = (const GLint*)(*arg1_utf8);
    } else if(info[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(info[1]);
        arg1_nonconst = new GLint[arg1_array->Length()];
        arg1 = (const GLint*)arg1_nonconst;
    } else if(info[1]->IsObject()) {
        arg1 = (const GLint*)glbind_get_buffer_data(info[1]);
    } else if(info[1]->IsNumber()) {
        arg1 = (const GLint*)(info[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glPointParameteriv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    return;
}
NAN_METHOD(EXPORT_genQueries) {
    Nan::HandleScope scope;
    // None
    GLsizei arg0;
    arg0 = info[0]->Int32Value();
    GLuint* arg1;
    v8::Handle<v8::Array> info_1_array = v8::Handle<v8::Array>::Cast(info[1]);
    arg1 = new GLuint[info_1_array->Length()];
    glGenQueries(arg0, arg1);
    for(uint32_t i = 0; i < info_1_array->Length(); i++) {
        info_1_array->Set(i, NODE_Query::fromGLHandle(arg1[i]));
    }
    delete [] arg1;
    return;
}
NAN_METHOD(EXPORT_isQuery) {
    Nan::HandleScope scope;
    // <gltypes.Type instance at 0x102dd84d0>
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Query>(info[0]->ToObject())->gl_handle;
    }
    GLboolean result;
    result = glIsQuery(arg0);
    v8::Handle<v8::Value> result_js = Nan::New<v8::Uint32>(result);
    return info.GetReturnValue().Set(result_js);
}
NAN_METHOD(EXPORT_beginQuery) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLuint arg1;
    if(info[1]->IsNumber()) {
        arg1 = info[1]->IntegerValue();
    } else {
        arg1 = Nan::ObjectWrap::Unwrap<NODE_Query>(info[1]->ToObject())->gl_handle;
    }
    glBeginQuery(arg0, arg1);
    return;
}
NAN_METHOD(EXPORT_endQuery) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    glEndQuery(arg0);
    return;
}
NAN_METHOD(EXPORT_getQueryiv) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    GLint *arg2;
    GLint* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (GLint*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLint[arg2_array->Length()];
        arg2 = (GLint*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (GLint*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (GLint*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetQueryiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_getQueryObjectiv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Query>(info[0]->ToObject())->gl_handle;
    }
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    GLint *arg2;
    GLint* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (GLint*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLint[arg2_array->Length()];
        arg2 = (GLint*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (GLint*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (GLint*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetQueryObjectiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_getQueryObjectuiv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Query>(info[0]->ToObject())->gl_handle;
    }
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    GLuint *arg2;
    GLuint* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (GLuint*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLuint[arg2_array->Length()];
        arg2 = (GLuint*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (GLuint*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (GLuint*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetQueryObjectuiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_bindBuffer) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLuint arg1;
    if(info[1]->IsNumber()) {
        arg1 = info[1]->IntegerValue();
    } else {
        arg1 = Nan::ObjectWrap::Unwrap<NODE_Buffer>(info[1]->ToObject())->gl_handle;
    }
    glBindBuffer(arg0, arg1);
    return;
}
NAN_METHOD(EXPORT_genBuffers) {
    Nan::HandleScope scope;
    // None
    GLsizei arg0;
    arg0 = info[0]->Int32Value();
    GLuint* arg1;
    v8::Handle<v8::Array> info_1_array = v8::Handle<v8::Array>::Cast(info[1]);
    arg1 = new GLuint[info_1_array->Length()];
    glGenBuffers(arg0, arg1);
    for(uint32_t i = 0; i < info_1_array->Length(); i++) {
        info_1_array->Set(i, NODE_Buffer::fromGLHandle(arg1[i]));
    }
    delete [] arg1;
    return;
}
NAN_METHOD(EXPORT_isBuffer) {
    Nan::HandleScope scope;
    // <gltypes.Type instance at 0x102dd84d0>
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Buffer>(info[0]->ToObject())->gl_handle;
    }
    GLboolean result;
    result = glIsBuffer(arg0);
    v8::Handle<v8::Value> result_js = Nan::New<v8::Uint32>(result);
    return info.GetReturnValue().Set(result_js);
}
NAN_METHOD(EXPORT_bufferData) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLsizeiptr arg1;
    arg1 = (uint64_t)info[1]->IntegerValue();
    const GLvoid *arg2;
    GLubyte* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (const GLvoid*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLubyte[arg2_array->Length()];
        arg2 = (const GLvoid*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (const GLvoid*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (const GLvoid*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    GLenum arg3;
    arg3 = info[3]->Uint32Value();
    glBufferData(arg0, arg1, arg2, arg3);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_bufferSubData) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLintptr arg1;
    arg1 = info[1]->IntegerValue();
    GLsizeiptr arg2;
    arg2 = (uint64_t)info[2]->IntegerValue();
    const GLvoid *arg3;
    GLubyte* arg3_nonconst = NULL;
    if(info[3]->IsString()) {
        Nan::Utf8String arg3_utf8(info[3]);
        arg3 = (const GLvoid*)(*arg3_utf8);
    } else if(info[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(info[3]);
        arg3_nonconst = new GLubyte[arg3_array->Length()];
        arg3 = (const GLvoid*)arg3_nonconst;
    } else if(info[3]->IsObject()) {
        arg3 = (const GLvoid*)glbind_get_buffer_data(info[3]);
    } else if(info[3]->IsNumber()) {
        arg3 = (const GLvoid*)(info[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glBufferSubData(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    return;
}
NAN_METHOD(EXPORT_getBufferSubData) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLintptr arg1;
    arg1 = info[1]->IntegerValue();
    GLsizeiptr arg2;
    arg2 = (uint64_t)info[2]->IntegerValue();
    GLvoid *arg3;
    GLubyte* arg3_nonconst = NULL;
    if(info[3]->IsString()) {
        Nan::Utf8String arg3_utf8(info[3]);
        arg3 = (GLvoid*)(*arg3_utf8);
    } else if(info[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(info[3]);
        arg3_nonconst = new GLubyte[arg3_array->Length()];
        arg3 = (GLvoid*)arg3_nonconst;
    } else if(info[3]->IsObject()) {
        arg3 = (GLvoid*)glbind_get_buffer_data(info[3]);
    } else if(info[3]->IsNumber()) {
        arg3 = (GLvoid*)(info[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glGetBufferSubData(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    return;
}
NAN_METHOD(EXPORT_mapBuffer) {
    Nan::HandleScope scope;
    // <gltypes.PointerType instance at 0x102e71758>
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    GLvoid *result;
    result = glMapBuffer(arg0, arg1);
    v8::Handle<v8::Value> result_js = Nan::NewBuffer((char*)result, 0, do_nothing_release_callback, NULL).ToLocalChecked();
    return info.GetReturnValue().Set(result_js);
}
NAN_METHOD(EXPORT_unmapBuffer) {
    Nan::HandleScope scope;
    // <gltypes.Type instance at 0x102dd84d0>
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLboolean result;
    result = glUnmapBuffer(arg0);
    v8::Handle<v8::Value> result_js = Nan::New<v8::Uint32>(result);
    return info.GetReturnValue().Set(result_js);
}
NAN_METHOD(EXPORT_getBufferParameteriv) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    GLint *arg2;
    GLint* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (GLint*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLint[arg2_array->Length()];
        arg2 = (GLint*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (GLint*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (GLint*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetBufferParameteriv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_blendEquationSeparate) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    glBlendEquationSeparate(arg0, arg1);
    return;
}
NAN_METHOD(EXPORT_drawBuffers) {
    Nan::HandleScope scope;
    // None
    GLsizei arg0;
    arg0 = info[0]->Int32Value();
    const GLenum *arg1;
    GLenum* arg1_nonconst = NULL;
    if(info[1]->IsString()) {
        Nan::Utf8String arg1_utf8(info[1]);
        arg1 = (const GLenum*)(*arg1_utf8);
    } else if(info[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(info[1]);
        arg1_nonconst = new GLenum[arg1_array->Length()];
        arg1 = (const GLenum*)arg1_nonconst;
    } else if(info[1]->IsObject()) {
        arg1 = (const GLenum*)glbind_get_buffer_data(info[1]);
    } else if(info[1]->IsNumber()) {
        arg1 = (const GLenum*)(info[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glDrawBuffers(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    return;
}
NAN_METHOD(EXPORT_stencilOpSeparate) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    GLenum arg2;
    arg2 = info[2]->Uint32Value();
    GLenum arg3;
    arg3 = info[3]->Uint32Value();
    glStencilOpSeparate(arg0, arg1, arg2, arg3);
    return;
}
NAN_METHOD(EXPORT_stencilFuncSeparate) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    GLint arg2;
    arg2 = info[2]->Int32Value();
    GLuint arg3;
    arg3 = info[3]->Uint32Value();
    glStencilFuncSeparate(arg0, arg1, arg2, arg3);
    return;
}
NAN_METHOD(EXPORT_stencilMaskSeparate) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLuint arg1;
    arg1 = info[1]->Uint32Value();
    glStencilMaskSeparate(arg0, arg1);
    return;
}
NAN_METHOD(EXPORT_attachShader) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLuint arg1;
    if(info[1]->IsNumber()) {
        arg1 = info[1]->IntegerValue();
    } else {
        arg1 = Nan::ObjectWrap::Unwrap<NODE_Shader>(info[1]->ToObject())->gl_handle;
    }
    glAttachShader(arg0, arg1);
    return;
}
NAN_METHOD(EXPORT_bindAttribLocation) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLuint arg1;
    arg1 = info[1]->Uint32Value();
    const GLchar* arg2;
    Nan::Utf8String arg2_utf8(info[2]);
    arg2 = (GLchar*)*arg2_utf8;
    glBindAttribLocation(arg0, arg1, arg2);
    return;
}
NAN_METHOD(EXPORT_compileShader) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Shader>(info[0]->ToObject())->gl_handle;
    }
    glCompileShader(arg0);
    return;
}
NAN_METHOD(EXPORT_createProgram) {
    Nan::HandleScope scope;
    // <gltypes.ClassType instance at 0x102dd8b90>
    GLuint result;
    result = glCreateProgram();
    v8::Handle<v8::Value> result_js = NODE_Program::fromGLHandle(result);
    return info.GetReturnValue().Set(result_js);
}
NAN_METHOD(EXPORT_createShader) {
    Nan::HandleScope scope;
    // <gltypes.ClassType instance at 0x102dd8bd8>
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLuint result;
    result = glCreateShader(arg0);
    v8::Handle<v8::Value> result_js = NODE_Shader::fromGLHandle(result);
    return info.GetReturnValue().Set(result_js);
}
NAN_METHOD(EXPORT_detachShader) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLuint arg1;
    if(info[1]->IsNumber()) {
        arg1 = info[1]->IntegerValue();
    } else {
        arg1 = Nan::ObjectWrap::Unwrap<NODE_Shader>(info[1]->ToObject())->gl_handle;
    }
    glDetachShader(arg0, arg1);
    return;
}
NAN_METHOD(EXPORT_disableVertexAttribArray) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    glDisableVertexAttribArray(arg0);
    return;
}
NAN_METHOD(EXPORT_enableVertexAttribArray) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    glEnableVertexAttribArray(arg0);
    return;
}
NAN_METHOD(EXPORT_getActiveAttrib) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLuint arg1;
    arg1 = info[1]->Uint32Value();
    GLsizei arg2;
    arg2 = info[2]->Int32Value();
    GLsizei *arg3;
    GLsizei* arg3_nonconst = NULL;
    if(info[3]->IsString()) {
        Nan::Utf8String arg3_utf8(info[3]);
        arg3 = (GLsizei*)(*arg3_utf8);
    } else if(info[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(info[3]);
        arg3_nonconst = new GLsizei[arg3_array->Length()];
        arg3 = (GLsizei*)arg3_nonconst;
    } else if(info[3]->IsObject()) {
        arg3 = (GLsizei*)glbind_get_buffer_data(info[3]);
    } else if(info[3]->IsNumber()) {
        arg3 = (GLsizei*)(info[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    GLint *arg4;
    GLint* arg4_nonconst = NULL;
    if(info[4]->IsString()) {
        Nan::Utf8String arg4_utf8(info[4]);
        arg4 = (GLint*)(*arg4_utf8);
    } else if(info[4]->IsArray()) {
        v8::Handle<v8::Array> arg4_array = v8::Handle<v8::Array>::Cast(info[4]);
        arg4_nonconst = new GLint[arg4_array->Length()];
        arg4 = (GLint*)arg4_nonconst;
    } else if(info[4]->IsObject()) {
        arg4 = (GLint*)glbind_get_buffer_data(info[4]);
    } else if(info[4]->IsNumber()) {
        arg4 = (GLint*)(info[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    GLenum *arg5;
    GLenum* arg5_nonconst = NULL;
    if(info[5]->IsString()) {
        Nan::Utf8String arg5_utf8(info[5]);
        arg5 = (GLenum*)(*arg5_utf8);
    } else if(info[5]->IsArray()) {
        v8::Handle<v8::Array> arg5_array = v8::Handle<v8::Array>::Cast(info[5]);
        arg5_nonconst = new GLenum[arg5_array->Length()];
        arg5 = (GLenum*)arg5_nonconst;
    } else if(info[5]->IsObject()) {
        arg5 = (GLenum*)glbind_get_buffer_data(info[5]);
    } else if(info[5]->IsNumber()) {
        arg5 = (GLenum*)(info[5]->IntegerValue());
    } else {
        arg5 = NULL;
    }
    GLchar *arg6;
    GLchar* arg6_nonconst = NULL;
    if(info[6]->IsString()) {
        Nan::Utf8String arg6_utf8(info[6]);
        arg6 = (GLchar*)(*arg6_utf8);
    } else if(info[6]->IsArray()) {
        v8::Handle<v8::Array> arg6_array = v8::Handle<v8::Array>::Cast(info[6]);
        arg6_nonconst = new GLchar[arg6_array->Length()];
        arg6 = (GLchar*)arg6_nonconst;
    } else if(info[6]->IsObject()) {
        arg6 = (GLchar*)glbind_get_buffer_data(info[6]);
    } else if(info[6]->IsNumber()) {
        arg6 = (GLchar*)(info[6]->IntegerValue());
    } else {
        arg6 = NULL;
    }
    glGetActiveAttrib(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
    if(arg3_nonconst) delete [] arg3_nonconst;
    if(arg4_nonconst) delete [] arg4_nonconst;
    if(arg5_nonconst) delete [] arg5_nonconst;
    if(arg6_nonconst) delete [] arg6_nonconst;
    return;
}
NAN_METHOD(EXPORT_getActiveUniform) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLuint arg1;
    arg1 = info[1]->Uint32Value();
    GLsizei arg2;
    arg2 = info[2]->Int32Value();
    GLsizei *arg3;
    GLsizei* arg3_nonconst = NULL;
    if(info[3]->IsString()) {
        Nan::Utf8String arg3_utf8(info[3]);
        arg3 = (GLsizei*)(*arg3_utf8);
    } else if(info[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(info[3]);
        arg3_nonconst = new GLsizei[arg3_array->Length()];
        arg3 = (GLsizei*)arg3_nonconst;
    } else if(info[3]->IsObject()) {
        arg3 = (GLsizei*)glbind_get_buffer_data(info[3]);
    } else if(info[3]->IsNumber()) {
        arg3 = (GLsizei*)(info[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    GLint *arg4;
    GLint* arg4_nonconst = NULL;
    if(info[4]->IsString()) {
        Nan::Utf8String arg4_utf8(info[4]);
        arg4 = (GLint*)(*arg4_utf8);
    } else if(info[4]->IsArray()) {
        v8::Handle<v8::Array> arg4_array = v8::Handle<v8::Array>::Cast(info[4]);
        arg4_nonconst = new GLint[arg4_array->Length()];
        arg4 = (GLint*)arg4_nonconst;
    } else if(info[4]->IsObject()) {
        arg4 = (GLint*)glbind_get_buffer_data(info[4]);
    } else if(info[4]->IsNumber()) {
        arg4 = (GLint*)(info[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    GLenum *arg5;
    GLenum* arg5_nonconst = NULL;
    if(info[5]->IsString()) {
        Nan::Utf8String arg5_utf8(info[5]);
        arg5 = (GLenum*)(*arg5_utf8);
    } else if(info[5]->IsArray()) {
        v8::Handle<v8::Array> arg5_array = v8::Handle<v8::Array>::Cast(info[5]);
        arg5_nonconst = new GLenum[arg5_array->Length()];
        arg5 = (GLenum*)arg5_nonconst;
    } else if(info[5]->IsObject()) {
        arg5 = (GLenum*)glbind_get_buffer_data(info[5]);
    } else if(info[5]->IsNumber()) {
        arg5 = (GLenum*)(info[5]->IntegerValue());
    } else {
        arg5 = NULL;
    }
    GLchar *arg6;
    GLchar* arg6_nonconst = NULL;
    if(info[6]->IsString()) {
        Nan::Utf8String arg6_utf8(info[6]);
        arg6 = (GLchar*)(*arg6_utf8);
    } else if(info[6]->IsArray()) {
        v8::Handle<v8::Array> arg6_array = v8::Handle<v8::Array>::Cast(info[6]);
        arg6_nonconst = new GLchar[arg6_array->Length()];
        arg6 = (GLchar*)arg6_nonconst;
    } else if(info[6]->IsObject()) {
        arg6 = (GLchar*)glbind_get_buffer_data(info[6]);
    } else if(info[6]->IsNumber()) {
        arg6 = (GLchar*)(info[6]->IntegerValue());
    } else {
        arg6 = NULL;
    }
    glGetActiveUniform(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
    if(arg3_nonconst) delete [] arg3_nonconst;
    if(arg4_nonconst) delete [] arg4_nonconst;
    if(arg5_nonconst) delete [] arg5_nonconst;
    if(arg6_nonconst) delete [] arg6_nonconst;
    return;
}
NAN_METHOD(EXPORT_getAttachedShaders) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLsizei arg1;
    arg1 = info[1]->Int32Value();
    GLsizei *arg2;
    GLsizei* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (GLsizei*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLsizei[arg2_array->Length()];
        arg2 = (GLsizei*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (GLsizei*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (GLsizei*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    GLuint* arg3;
    v8::Handle<v8::Array> info_3_array = v8::Handle<v8::Array>::Cast(info[3]);
    arg3 = new GLuint[info_3_array->Length()];
    glGetAttachedShaders(arg0, arg1, arg2, arg3);
    if(arg2_nonconst) delete [] arg2_nonconst;
    for(uint32_t i = 0; i < info_3_array->Length(); i++) {
        info_3_array->Set(i, NODE_Shader::fromGLHandle(arg3[i]));
    }
    delete [] arg3;
    return;
}
NAN_METHOD(EXPORT_getAttribLocation) {
    Nan::HandleScope scope;
    // <gltypes.Type instance at 0x102dd8638>
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    const GLchar* arg1;
    Nan::Utf8String arg1_utf8(info[1]);
    arg1 = (GLchar*)*arg1_utf8;
    GLint result;
    result = glGetAttribLocation(arg0, arg1);
    v8::Handle<v8::Value> result_js = Nan::New<v8::Int32>(result);
    return info.GetReturnValue().Set(result_js);
}
NAN_METHOD(EXPORT_getProgramiv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    GLint *arg2;
    GLint* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (GLint*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLint[arg2_array->Length()];
        arg2 = (GLint*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (GLint*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (GLint*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetProgramiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_getProgramInfoLog) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLsizei arg1;
    arg1 = info[1]->Int32Value();
    GLsizei *arg2;
    GLsizei* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (GLsizei*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLsizei[arg2_array->Length()];
        arg2 = (GLsizei*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (GLsizei*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (GLsizei*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    GLchar *arg3;
    GLchar* arg3_nonconst = NULL;
    if(info[3]->IsString()) {
        Nan::Utf8String arg3_utf8(info[3]);
        arg3 = (GLchar*)(*arg3_utf8);
    } else if(info[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(info[3]);
        arg3_nonconst = new GLchar[arg3_array->Length()];
        arg3 = (GLchar*)arg3_nonconst;
    } else if(info[3]->IsObject()) {
        arg3 = (GLchar*)glbind_get_buffer_data(info[3]);
    } else if(info[3]->IsNumber()) {
        arg3 = (GLchar*)(info[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glGetProgramInfoLog(arg0, arg1, arg2, arg3);
    if(arg2_nonconst) delete [] arg2_nonconst;
    if(arg3_nonconst) delete [] arg3_nonconst;
    return;
}
NAN_METHOD(EXPORT_getShaderiv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Shader>(info[0]->ToObject())->gl_handle;
    }
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    GLint *arg2;
    GLint* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (GLint*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLint[arg2_array->Length()];
        arg2 = (GLint*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (GLint*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (GLint*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetShaderiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_getShaderInfoLog) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Shader>(info[0]->ToObject())->gl_handle;
    }
    GLsizei arg1;
    arg1 = info[1]->Int32Value();
    GLsizei *arg2;
    GLsizei* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (GLsizei*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLsizei[arg2_array->Length()];
        arg2 = (GLsizei*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (GLsizei*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (GLsizei*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    GLchar *arg3;
    GLchar* arg3_nonconst = NULL;
    if(info[3]->IsString()) {
        Nan::Utf8String arg3_utf8(info[3]);
        arg3 = (GLchar*)(*arg3_utf8);
    } else if(info[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(info[3]);
        arg3_nonconst = new GLchar[arg3_array->Length()];
        arg3 = (GLchar*)arg3_nonconst;
    } else if(info[3]->IsObject()) {
        arg3 = (GLchar*)glbind_get_buffer_data(info[3]);
    } else if(info[3]->IsNumber()) {
        arg3 = (GLchar*)(info[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glGetShaderInfoLog(arg0, arg1, arg2, arg3);
    if(arg2_nonconst) delete [] arg2_nonconst;
    if(arg3_nonconst) delete [] arg3_nonconst;
    return;
}
NAN_METHOD(EXPORT_getShaderSource) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Shader>(info[0]->ToObject())->gl_handle;
    }
    GLsizei arg1;
    arg1 = info[1]->Int32Value();
    GLsizei *arg2;
    GLsizei* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (GLsizei*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLsizei[arg2_array->Length()];
        arg2 = (GLsizei*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (GLsizei*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (GLsizei*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    GLchar *arg3;
    GLchar* arg3_nonconst = NULL;
    if(info[3]->IsString()) {
        Nan::Utf8String arg3_utf8(info[3]);
        arg3 = (GLchar*)(*arg3_utf8);
    } else if(info[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(info[3]);
        arg3_nonconst = new GLchar[arg3_array->Length()];
        arg3 = (GLchar*)arg3_nonconst;
    } else if(info[3]->IsObject()) {
        arg3 = (GLchar*)glbind_get_buffer_data(info[3]);
    } else if(info[3]->IsNumber()) {
        arg3 = (GLchar*)(info[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glGetShaderSource(arg0, arg1, arg2, arg3);
    if(arg2_nonconst) delete [] arg2_nonconst;
    if(arg3_nonconst) delete [] arg3_nonconst;
    return;
}
NAN_METHOD(EXPORT_getUniformLocation) {
    Nan::HandleScope scope;
    // <gltypes.Type instance at 0x102dd8638>
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    const GLchar* arg1;
    Nan::Utf8String arg1_utf8(info[1]);
    arg1 = (GLchar*)*arg1_utf8;
    GLint result;
    result = glGetUniformLocation(arg0, arg1);
    v8::Handle<v8::Value> result_js = Nan::New<v8::Int32>(result);
    return info.GetReturnValue().Set(result_js);
}
NAN_METHOD(EXPORT_getUniformfv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLfloat *arg2;
    GLfloat* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (GLfloat*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLfloat[arg2_array->Length()];
        arg2 = (GLfloat*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (GLfloat*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (GLfloat*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetUniformfv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_getUniformiv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLint *arg2;
    GLint* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (GLint*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLint[arg2_array->Length()];
        arg2 = (GLint*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (GLint*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (GLint*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetUniformiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_getVertexAttribdv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    GLdouble *arg2;
    GLdouble* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (GLdouble*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLdouble[arg2_array->Length()];
        arg2 = (GLdouble*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (GLdouble*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (GLdouble*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetVertexAttribdv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_getVertexAttribfv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    GLfloat *arg2;
    GLfloat* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (GLfloat*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLfloat[arg2_array->Length()];
        arg2 = (GLfloat*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (GLfloat*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (GLfloat*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetVertexAttribfv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_getVertexAttribiv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    GLint *arg2;
    GLint* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (GLint*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLint[arg2_array->Length()];
        arg2 = (GLint*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (GLint*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (GLint*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetVertexAttribiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_isProgram) {
    Nan::HandleScope scope;
    // <gltypes.Type instance at 0x102dd84d0>
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLboolean result;
    result = glIsProgram(arg0);
    v8::Handle<v8::Value> result_js = Nan::New<v8::Uint32>(result);
    return info.GetReturnValue().Set(result_js);
}
NAN_METHOD(EXPORT_isShader) {
    Nan::HandleScope scope;
    // <gltypes.Type instance at 0x102dd84d0>
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Shader>(info[0]->ToObject())->gl_handle;
    }
    GLboolean result;
    result = glIsShader(arg0);
    v8::Handle<v8::Value> result_js = Nan::New<v8::Uint32>(result);
    return info.GetReturnValue().Set(result_js);
}
NAN_METHOD(EXPORT_linkProgram) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    glLinkProgram(arg0);
    return;
}
NAN_METHOD(EXPORT_useProgram) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    glUseProgram(arg0);
    return;
}
NAN_METHOD(EXPORT_uniform1f) {
    Nan::HandleScope scope;
    // None
    GLint arg0;
    arg0 = info[0]->Int32Value();
    GLfloat arg1;
    arg1 = (float)info[1]->NumberValue();
    glUniform1f(arg0, arg1);
    return;
}
NAN_METHOD(EXPORT_uniform2f) {
    Nan::HandleScope scope;
    // None
    GLint arg0;
    arg0 = info[0]->Int32Value();
    GLfloat arg1;
    arg1 = (float)info[1]->NumberValue();
    GLfloat arg2;
    arg2 = (float)info[2]->NumberValue();
    glUniform2f(arg0, arg1, arg2);
    return;
}
NAN_METHOD(EXPORT_uniform3f) {
    Nan::HandleScope scope;
    // None
    GLint arg0;
    arg0 = info[0]->Int32Value();
    GLfloat arg1;
    arg1 = (float)info[1]->NumberValue();
    GLfloat arg2;
    arg2 = (float)info[2]->NumberValue();
    GLfloat arg3;
    arg3 = (float)info[3]->NumberValue();
    glUniform3f(arg0, arg1, arg2, arg3);
    return;
}
NAN_METHOD(EXPORT_uniform4f) {
    Nan::HandleScope scope;
    // None
    GLint arg0;
    arg0 = info[0]->Int32Value();
    GLfloat arg1;
    arg1 = (float)info[1]->NumberValue();
    GLfloat arg2;
    arg2 = (float)info[2]->NumberValue();
    GLfloat arg3;
    arg3 = (float)info[3]->NumberValue();
    GLfloat arg4;
    arg4 = (float)info[4]->NumberValue();
    glUniform4f(arg0, arg1, arg2, arg3, arg4);
    return;
}
NAN_METHOD(EXPORT_uniform1i) {
    Nan::HandleScope scope;
    // None
    GLint arg0;
    arg0 = info[0]->Int32Value();
    GLint arg1;
    arg1 = info[1]->Int32Value();
    glUniform1i(arg0, arg1);
    return;
}
NAN_METHOD(EXPORT_uniform2i) {
    Nan::HandleScope scope;
    // None
    GLint arg0;
    arg0 = info[0]->Int32Value();
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLint arg2;
    arg2 = info[2]->Int32Value();
    glUniform2i(arg0, arg1, arg2);
    return;
}
NAN_METHOD(EXPORT_uniform3i) {
    Nan::HandleScope scope;
    // None
    GLint arg0;
    arg0 = info[0]->Int32Value();
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLint arg2;
    arg2 = info[2]->Int32Value();
    GLint arg3;
    arg3 = info[3]->Int32Value();
    glUniform3i(arg0, arg1, arg2, arg3);
    return;
}
NAN_METHOD(EXPORT_uniform4i) {
    Nan::HandleScope scope;
    // None
    GLint arg0;
    arg0 = info[0]->Int32Value();
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLint arg2;
    arg2 = info[2]->Int32Value();
    GLint arg3;
    arg3 = info[3]->Int32Value();
    GLint arg4;
    arg4 = info[4]->Int32Value();
    glUniform4i(arg0, arg1, arg2, arg3, arg4);
    return;
}
NAN_METHOD(EXPORT_uniform1fv) {
    Nan::HandleScope scope;
    // None
    GLint arg0;
    arg0 = info[0]->Int32Value();
    GLsizei arg1;
    arg1 = info[1]->Int32Value();
    const GLfloat *arg2;
    GLfloat* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (const GLfloat*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLfloat[arg2_array->Length()];
        arg2 = (const GLfloat*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (const GLfloat*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (const GLfloat*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glUniform1fv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_uniform2fv) {
    Nan::HandleScope scope;
    // None
    GLint arg0;
    arg0 = info[0]->Int32Value();
    GLsizei arg1;
    arg1 = info[1]->Int32Value();
    const GLfloat *arg2;
    GLfloat* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (const GLfloat*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLfloat[arg2_array->Length()];
        arg2 = (const GLfloat*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (const GLfloat*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (const GLfloat*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glUniform2fv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_uniform3fv) {
    Nan::HandleScope scope;
    // None
    GLint arg0;
    arg0 = info[0]->Int32Value();
    GLsizei arg1;
    arg1 = info[1]->Int32Value();
    const GLfloat *arg2;
    GLfloat* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (const GLfloat*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLfloat[arg2_array->Length()];
        arg2 = (const GLfloat*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (const GLfloat*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (const GLfloat*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glUniform3fv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_uniform4fv) {
    Nan::HandleScope scope;
    // None
    GLint arg0;
    arg0 = info[0]->Int32Value();
    GLsizei arg1;
    arg1 = info[1]->Int32Value();
    const GLfloat *arg2;
    GLfloat* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (const GLfloat*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLfloat[arg2_array->Length()];
        arg2 = (const GLfloat*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (const GLfloat*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (const GLfloat*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glUniform4fv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_uniform1iv) {
    Nan::HandleScope scope;
    // None
    GLint arg0;
    arg0 = info[0]->Int32Value();
    GLsizei arg1;
    arg1 = info[1]->Int32Value();
    const GLint *arg2;
    GLint* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (const GLint*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLint[arg2_array->Length()];
        arg2 = (const GLint*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (const GLint*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (const GLint*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glUniform1iv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_uniform2iv) {
    Nan::HandleScope scope;
    // None
    GLint arg0;
    arg0 = info[0]->Int32Value();
    GLsizei arg1;
    arg1 = info[1]->Int32Value();
    const GLint *arg2;
    GLint* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (const GLint*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLint[arg2_array->Length()];
        arg2 = (const GLint*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (const GLint*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (const GLint*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glUniform2iv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_uniform3iv) {
    Nan::HandleScope scope;
    // None
    GLint arg0;
    arg0 = info[0]->Int32Value();
    GLsizei arg1;
    arg1 = info[1]->Int32Value();
    const GLint *arg2;
    GLint* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (const GLint*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLint[arg2_array->Length()];
        arg2 = (const GLint*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (const GLint*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (const GLint*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glUniform3iv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_uniform4iv) {
    Nan::HandleScope scope;
    // None
    GLint arg0;
    arg0 = info[0]->Int32Value();
    GLsizei arg1;
    arg1 = info[1]->Int32Value();
    const GLint *arg2;
    GLint* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (const GLint*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLint[arg2_array->Length()];
        arg2 = (const GLint*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (const GLint*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (const GLint*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glUniform4iv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_uniformMatrix2fv) {
    Nan::HandleScope scope;
    // None
    GLint arg0;
    arg0 = info[0]->Int32Value();
    GLsizei arg1;
    arg1 = info[1]->Int32Value();
    GLboolean arg2;
    arg2 = info[2]->Uint32Value();
    const GLfloat *arg3;
    GLfloat* arg3_nonconst = NULL;
    if(info[3]->IsString()) {
        Nan::Utf8String arg3_utf8(info[3]);
        arg3 = (const GLfloat*)(*arg3_utf8);
    } else if(info[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(info[3]);
        arg3_nonconst = new GLfloat[arg3_array->Length()];
        arg3 = (const GLfloat*)arg3_nonconst;
    } else if(info[3]->IsObject()) {
        arg3 = (const GLfloat*)glbind_get_buffer_data(info[3]);
    } else if(info[3]->IsNumber()) {
        arg3 = (const GLfloat*)(info[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glUniformMatrix2fv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    return;
}
NAN_METHOD(EXPORT_uniformMatrix3fv) {
    Nan::HandleScope scope;
    // None
    GLint arg0;
    arg0 = info[0]->Int32Value();
    GLsizei arg1;
    arg1 = info[1]->Int32Value();
    GLboolean arg2;
    arg2 = info[2]->Uint32Value();
    const GLfloat *arg3;
    GLfloat* arg3_nonconst = NULL;
    if(info[3]->IsString()) {
        Nan::Utf8String arg3_utf8(info[3]);
        arg3 = (const GLfloat*)(*arg3_utf8);
    } else if(info[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(info[3]);
        arg3_nonconst = new GLfloat[arg3_array->Length()];
        arg3 = (const GLfloat*)arg3_nonconst;
    } else if(info[3]->IsObject()) {
        arg3 = (const GLfloat*)glbind_get_buffer_data(info[3]);
    } else if(info[3]->IsNumber()) {
        arg3 = (const GLfloat*)(info[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glUniformMatrix3fv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    return;
}
NAN_METHOD(EXPORT_uniformMatrix4fv) {
    Nan::HandleScope scope;
    // None
    GLint arg0;
    arg0 = info[0]->Int32Value();
    GLsizei arg1;
    arg1 = info[1]->Int32Value();
    GLboolean arg2;
    arg2 = info[2]->Uint32Value();
    const GLfloat *arg3;
    GLfloat* arg3_nonconst = NULL;
    if(info[3]->IsString()) {
        Nan::Utf8String arg3_utf8(info[3]);
        arg3 = (const GLfloat*)(*arg3_utf8);
    } else if(info[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(info[3]);
        arg3_nonconst = new GLfloat[arg3_array->Length()];
        arg3 = (const GLfloat*)arg3_nonconst;
    } else if(info[3]->IsObject()) {
        arg3 = (const GLfloat*)glbind_get_buffer_data(info[3]);
    } else if(info[3]->IsNumber()) {
        arg3 = (const GLfloat*)(info[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glUniformMatrix4fv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    return;
}
NAN_METHOD(EXPORT_validateProgram) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    glValidateProgram(arg0);
    return;
}
NAN_METHOD(EXPORT_vertexAttrib1d) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    GLdouble arg1;
    arg1 = info[1]->NumberValue();
    glVertexAttrib1d(arg0, arg1);
    return;
}
NAN_METHOD(EXPORT_vertexAttrib1dv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    const GLdouble *arg1;
    GLdouble* arg1_nonconst = NULL;
    if(info[1]->IsString()) {
        Nan::Utf8String arg1_utf8(info[1]);
        arg1 = (const GLdouble*)(*arg1_utf8);
    } else if(info[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(info[1]);
        arg1_nonconst = new GLdouble[arg1_array->Length()];
        arg1 = (const GLdouble*)arg1_nonconst;
    } else if(info[1]->IsObject()) {
        arg1 = (const GLdouble*)glbind_get_buffer_data(info[1]);
    } else if(info[1]->IsNumber()) {
        arg1 = (const GLdouble*)(info[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib1dv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    return;
}
NAN_METHOD(EXPORT_vertexAttrib1f) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    GLfloat arg1;
    arg1 = (float)info[1]->NumberValue();
    glVertexAttrib1f(arg0, arg1);
    return;
}
NAN_METHOD(EXPORT_vertexAttrib1fv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    const GLfloat *arg1;
    GLfloat* arg1_nonconst = NULL;
    if(info[1]->IsString()) {
        Nan::Utf8String arg1_utf8(info[1]);
        arg1 = (const GLfloat*)(*arg1_utf8);
    } else if(info[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(info[1]);
        arg1_nonconst = new GLfloat[arg1_array->Length()];
        arg1 = (const GLfloat*)arg1_nonconst;
    } else if(info[1]->IsObject()) {
        arg1 = (const GLfloat*)glbind_get_buffer_data(info[1]);
    } else if(info[1]->IsNumber()) {
        arg1 = (const GLfloat*)(info[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib1fv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    return;
}
NAN_METHOD(EXPORT_vertexAttrib1s) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    GLshort arg1;
    arg1 = info[1]->Int32Value();
    glVertexAttrib1s(arg0, arg1);
    return;
}
NAN_METHOD(EXPORT_vertexAttrib1sv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    const GLshort *arg1;
    GLshort* arg1_nonconst = NULL;
    if(info[1]->IsString()) {
        Nan::Utf8String arg1_utf8(info[1]);
        arg1 = (const GLshort*)(*arg1_utf8);
    } else if(info[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(info[1]);
        arg1_nonconst = new GLshort[arg1_array->Length()];
        arg1 = (const GLshort*)arg1_nonconst;
    } else if(info[1]->IsObject()) {
        arg1 = (const GLshort*)glbind_get_buffer_data(info[1]);
    } else if(info[1]->IsNumber()) {
        arg1 = (const GLshort*)(info[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib1sv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    return;
}
NAN_METHOD(EXPORT_vertexAttrib2d) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    GLdouble arg1;
    arg1 = info[1]->NumberValue();
    GLdouble arg2;
    arg2 = info[2]->NumberValue();
    glVertexAttrib2d(arg0, arg1, arg2);
    return;
}
NAN_METHOD(EXPORT_vertexAttrib2dv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    const GLdouble *arg1;
    GLdouble* arg1_nonconst = NULL;
    if(info[1]->IsString()) {
        Nan::Utf8String arg1_utf8(info[1]);
        arg1 = (const GLdouble*)(*arg1_utf8);
    } else if(info[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(info[1]);
        arg1_nonconst = new GLdouble[arg1_array->Length()];
        arg1 = (const GLdouble*)arg1_nonconst;
    } else if(info[1]->IsObject()) {
        arg1 = (const GLdouble*)glbind_get_buffer_data(info[1]);
    } else if(info[1]->IsNumber()) {
        arg1 = (const GLdouble*)(info[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib2dv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    return;
}
NAN_METHOD(EXPORT_vertexAttrib2f) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    GLfloat arg1;
    arg1 = (float)info[1]->NumberValue();
    GLfloat arg2;
    arg2 = (float)info[2]->NumberValue();
    glVertexAttrib2f(arg0, arg1, arg2);
    return;
}
NAN_METHOD(EXPORT_vertexAttrib2fv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    const GLfloat *arg1;
    GLfloat* arg1_nonconst = NULL;
    if(info[1]->IsString()) {
        Nan::Utf8String arg1_utf8(info[1]);
        arg1 = (const GLfloat*)(*arg1_utf8);
    } else if(info[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(info[1]);
        arg1_nonconst = new GLfloat[arg1_array->Length()];
        arg1 = (const GLfloat*)arg1_nonconst;
    } else if(info[1]->IsObject()) {
        arg1 = (const GLfloat*)glbind_get_buffer_data(info[1]);
    } else if(info[1]->IsNumber()) {
        arg1 = (const GLfloat*)(info[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib2fv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    return;
}
NAN_METHOD(EXPORT_vertexAttrib2s) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    GLshort arg1;
    arg1 = info[1]->Int32Value();
    GLshort arg2;
    arg2 = info[2]->Int32Value();
    glVertexAttrib2s(arg0, arg1, arg2);
    return;
}
NAN_METHOD(EXPORT_vertexAttrib2sv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    const GLshort *arg1;
    GLshort* arg1_nonconst = NULL;
    if(info[1]->IsString()) {
        Nan::Utf8String arg1_utf8(info[1]);
        arg1 = (const GLshort*)(*arg1_utf8);
    } else if(info[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(info[1]);
        arg1_nonconst = new GLshort[arg1_array->Length()];
        arg1 = (const GLshort*)arg1_nonconst;
    } else if(info[1]->IsObject()) {
        arg1 = (const GLshort*)glbind_get_buffer_data(info[1]);
    } else if(info[1]->IsNumber()) {
        arg1 = (const GLshort*)(info[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib2sv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    return;
}
NAN_METHOD(EXPORT_vertexAttrib3d) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    GLdouble arg1;
    arg1 = info[1]->NumberValue();
    GLdouble arg2;
    arg2 = info[2]->NumberValue();
    GLdouble arg3;
    arg3 = info[3]->NumberValue();
    glVertexAttrib3d(arg0, arg1, arg2, arg3);
    return;
}
NAN_METHOD(EXPORT_vertexAttrib3dv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    const GLdouble *arg1;
    GLdouble* arg1_nonconst = NULL;
    if(info[1]->IsString()) {
        Nan::Utf8String arg1_utf8(info[1]);
        arg1 = (const GLdouble*)(*arg1_utf8);
    } else if(info[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(info[1]);
        arg1_nonconst = new GLdouble[arg1_array->Length()];
        arg1 = (const GLdouble*)arg1_nonconst;
    } else if(info[1]->IsObject()) {
        arg1 = (const GLdouble*)glbind_get_buffer_data(info[1]);
    } else if(info[1]->IsNumber()) {
        arg1 = (const GLdouble*)(info[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib3dv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    return;
}
NAN_METHOD(EXPORT_vertexAttrib3f) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    GLfloat arg1;
    arg1 = (float)info[1]->NumberValue();
    GLfloat arg2;
    arg2 = (float)info[2]->NumberValue();
    GLfloat arg3;
    arg3 = (float)info[3]->NumberValue();
    glVertexAttrib3f(arg0, arg1, arg2, arg3);
    return;
}
NAN_METHOD(EXPORT_vertexAttrib3fv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    const GLfloat *arg1;
    GLfloat* arg1_nonconst = NULL;
    if(info[1]->IsString()) {
        Nan::Utf8String arg1_utf8(info[1]);
        arg1 = (const GLfloat*)(*arg1_utf8);
    } else if(info[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(info[1]);
        arg1_nonconst = new GLfloat[arg1_array->Length()];
        arg1 = (const GLfloat*)arg1_nonconst;
    } else if(info[1]->IsObject()) {
        arg1 = (const GLfloat*)glbind_get_buffer_data(info[1]);
    } else if(info[1]->IsNumber()) {
        arg1 = (const GLfloat*)(info[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib3fv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    return;
}
NAN_METHOD(EXPORT_vertexAttrib3s) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    GLshort arg1;
    arg1 = info[1]->Int32Value();
    GLshort arg2;
    arg2 = info[2]->Int32Value();
    GLshort arg3;
    arg3 = info[3]->Int32Value();
    glVertexAttrib3s(arg0, arg1, arg2, arg3);
    return;
}
NAN_METHOD(EXPORT_vertexAttrib3sv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    const GLshort *arg1;
    GLshort* arg1_nonconst = NULL;
    if(info[1]->IsString()) {
        Nan::Utf8String arg1_utf8(info[1]);
        arg1 = (const GLshort*)(*arg1_utf8);
    } else if(info[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(info[1]);
        arg1_nonconst = new GLshort[arg1_array->Length()];
        arg1 = (const GLshort*)arg1_nonconst;
    } else if(info[1]->IsObject()) {
        arg1 = (const GLshort*)glbind_get_buffer_data(info[1]);
    } else if(info[1]->IsNumber()) {
        arg1 = (const GLshort*)(info[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib3sv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    return;
}
NAN_METHOD(EXPORT_vertexAttrib4Nbv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    const GLbyte *arg1;
    GLbyte* arg1_nonconst = NULL;
    if(info[1]->IsString()) {
        Nan::Utf8String arg1_utf8(info[1]);
        arg1 = (const GLbyte*)(*arg1_utf8);
    } else if(info[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(info[1]);
        arg1_nonconst = new GLbyte[arg1_array->Length()];
        arg1 = (const GLbyte*)arg1_nonconst;
    } else if(info[1]->IsObject()) {
        arg1 = (const GLbyte*)glbind_get_buffer_data(info[1]);
    } else if(info[1]->IsNumber()) {
        arg1 = (const GLbyte*)(info[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib4Nbv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    return;
}
NAN_METHOD(EXPORT_vertexAttrib4Niv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    const GLint *arg1;
    GLint* arg1_nonconst = NULL;
    if(info[1]->IsString()) {
        Nan::Utf8String arg1_utf8(info[1]);
        arg1 = (const GLint*)(*arg1_utf8);
    } else if(info[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(info[1]);
        arg1_nonconst = new GLint[arg1_array->Length()];
        arg1 = (const GLint*)arg1_nonconst;
    } else if(info[1]->IsObject()) {
        arg1 = (const GLint*)glbind_get_buffer_data(info[1]);
    } else if(info[1]->IsNumber()) {
        arg1 = (const GLint*)(info[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib4Niv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    return;
}
NAN_METHOD(EXPORT_vertexAttrib4Nsv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    const GLshort *arg1;
    GLshort* arg1_nonconst = NULL;
    if(info[1]->IsString()) {
        Nan::Utf8String arg1_utf8(info[1]);
        arg1 = (const GLshort*)(*arg1_utf8);
    } else if(info[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(info[1]);
        arg1_nonconst = new GLshort[arg1_array->Length()];
        arg1 = (const GLshort*)arg1_nonconst;
    } else if(info[1]->IsObject()) {
        arg1 = (const GLshort*)glbind_get_buffer_data(info[1]);
    } else if(info[1]->IsNumber()) {
        arg1 = (const GLshort*)(info[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib4Nsv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    return;
}
NAN_METHOD(EXPORT_vertexAttrib4Nub) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    GLubyte arg1;
    arg1 = info[1]->Uint32Value();
    GLubyte arg2;
    arg2 = info[2]->Uint32Value();
    GLubyte arg3;
    arg3 = info[3]->Uint32Value();
    GLubyte arg4;
    arg4 = info[4]->Uint32Value();
    glVertexAttrib4Nub(arg0, arg1, arg2, arg3, arg4);
    return;
}
NAN_METHOD(EXPORT_vertexAttrib4Nubv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    const GLubyte *arg1;
    GLubyte* arg1_nonconst = NULL;
    if(info[1]->IsString()) {
        Nan::Utf8String arg1_utf8(info[1]);
        arg1 = (const GLubyte*)(*arg1_utf8);
    } else if(info[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(info[1]);
        arg1_nonconst = new GLubyte[arg1_array->Length()];
        arg1 = (const GLubyte*)arg1_nonconst;
    } else if(info[1]->IsObject()) {
        arg1 = (const GLubyte*)glbind_get_buffer_data(info[1]);
    } else if(info[1]->IsNumber()) {
        arg1 = (const GLubyte*)(info[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib4Nubv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    return;
}
NAN_METHOD(EXPORT_vertexAttrib4Nuiv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    const GLuint *arg1;
    GLuint* arg1_nonconst = NULL;
    if(info[1]->IsString()) {
        Nan::Utf8String arg1_utf8(info[1]);
        arg1 = (const GLuint*)(*arg1_utf8);
    } else if(info[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(info[1]);
        arg1_nonconst = new GLuint[arg1_array->Length()];
        arg1 = (const GLuint*)arg1_nonconst;
    } else if(info[1]->IsObject()) {
        arg1 = (const GLuint*)glbind_get_buffer_data(info[1]);
    } else if(info[1]->IsNumber()) {
        arg1 = (const GLuint*)(info[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib4Nuiv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    return;
}
NAN_METHOD(EXPORT_vertexAttrib4Nusv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    const GLushort *arg1;
    GLushort* arg1_nonconst = NULL;
    if(info[1]->IsString()) {
        Nan::Utf8String arg1_utf8(info[1]);
        arg1 = (const GLushort*)(*arg1_utf8);
    } else if(info[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(info[1]);
        arg1_nonconst = new GLushort[arg1_array->Length()];
        arg1 = (const GLushort*)arg1_nonconst;
    } else if(info[1]->IsObject()) {
        arg1 = (const GLushort*)glbind_get_buffer_data(info[1]);
    } else if(info[1]->IsNumber()) {
        arg1 = (const GLushort*)(info[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib4Nusv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    return;
}
NAN_METHOD(EXPORT_vertexAttrib4bv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    const GLbyte *arg1;
    GLbyte* arg1_nonconst = NULL;
    if(info[1]->IsString()) {
        Nan::Utf8String arg1_utf8(info[1]);
        arg1 = (const GLbyte*)(*arg1_utf8);
    } else if(info[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(info[1]);
        arg1_nonconst = new GLbyte[arg1_array->Length()];
        arg1 = (const GLbyte*)arg1_nonconst;
    } else if(info[1]->IsObject()) {
        arg1 = (const GLbyte*)glbind_get_buffer_data(info[1]);
    } else if(info[1]->IsNumber()) {
        arg1 = (const GLbyte*)(info[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib4bv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    return;
}
NAN_METHOD(EXPORT_vertexAttrib4d) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    GLdouble arg1;
    arg1 = info[1]->NumberValue();
    GLdouble arg2;
    arg2 = info[2]->NumberValue();
    GLdouble arg3;
    arg3 = info[3]->NumberValue();
    GLdouble arg4;
    arg4 = info[4]->NumberValue();
    glVertexAttrib4d(arg0, arg1, arg2, arg3, arg4);
    return;
}
NAN_METHOD(EXPORT_vertexAttrib4dv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    const GLdouble *arg1;
    GLdouble* arg1_nonconst = NULL;
    if(info[1]->IsString()) {
        Nan::Utf8String arg1_utf8(info[1]);
        arg1 = (const GLdouble*)(*arg1_utf8);
    } else if(info[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(info[1]);
        arg1_nonconst = new GLdouble[arg1_array->Length()];
        arg1 = (const GLdouble*)arg1_nonconst;
    } else if(info[1]->IsObject()) {
        arg1 = (const GLdouble*)glbind_get_buffer_data(info[1]);
    } else if(info[1]->IsNumber()) {
        arg1 = (const GLdouble*)(info[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib4dv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    return;
}
NAN_METHOD(EXPORT_vertexAttrib4f) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    GLfloat arg1;
    arg1 = (float)info[1]->NumberValue();
    GLfloat arg2;
    arg2 = (float)info[2]->NumberValue();
    GLfloat arg3;
    arg3 = (float)info[3]->NumberValue();
    GLfloat arg4;
    arg4 = (float)info[4]->NumberValue();
    glVertexAttrib4f(arg0, arg1, arg2, arg3, arg4);
    return;
}
NAN_METHOD(EXPORT_vertexAttrib4fv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    const GLfloat *arg1;
    GLfloat* arg1_nonconst = NULL;
    if(info[1]->IsString()) {
        Nan::Utf8String arg1_utf8(info[1]);
        arg1 = (const GLfloat*)(*arg1_utf8);
    } else if(info[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(info[1]);
        arg1_nonconst = new GLfloat[arg1_array->Length()];
        arg1 = (const GLfloat*)arg1_nonconst;
    } else if(info[1]->IsObject()) {
        arg1 = (const GLfloat*)glbind_get_buffer_data(info[1]);
    } else if(info[1]->IsNumber()) {
        arg1 = (const GLfloat*)(info[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib4fv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    return;
}
NAN_METHOD(EXPORT_vertexAttrib4iv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    const GLint *arg1;
    GLint* arg1_nonconst = NULL;
    if(info[1]->IsString()) {
        Nan::Utf8String arg1_utf8(info[1]);
        arg1 = (const GLint*)(*arg1_utf8);
    } else if(info[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(info[1]);
        arg1_nonconst = new GLint[arg1_array->Length()];
        arg1 = (const GLint*)arg1_nonconst;
    } else if(info[1]->IsObject()) {
        arg1 = (const GLint*)glbind_get_buffer_data(info[1]);
    } else if(info[1]->IsNumber()) {
        arg1 = (const GLint*)(info[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib4iv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    return;
}
NAN_METHOD(EXPORT_vertexAttrib4s) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    GLshort arg1;
    arg1 = info[1]->Int32Value();
    GLshort arg2;
    arg2 = info[2]->Int32Value();
    GLshort arg3;
    arg3 = info[3]->Int32Value();
    GLshort arg4;
    arg4 = info[4]->Int32Value();
    glVertexAttrib4s(arg0, arg1, arg2, arg3, arg4);
    return;
}
NAN_METHOD(EXPORT_vertexAttrib4sv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    const GLshort *arg1;
    GLshort* arg1_nonconst = NULL;
    if(info[1]->IsString()) {
        Nan::Utf8String arg1_utf8(info[1]);
        arg1 = (const GLshort*)(*arg1_utf8);
    } else if(info[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(info[1]);
        arg1_nonconst = new GLshort[arg1_array->Length()];
        arg1 = (const GLshort*)arg1_nonconst;
    } else if(info[1]->IsObject()) {
        arg1 = (const GLshort*)glbind_get_buffer_data(info[1]);
    } else if(info[1]->IsNumber()) {
        arg1 = (const GLshort*)(info[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib4sv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    return;
}
NAN_METHOD(EXPORT_vertexAttrib4ubv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    const GLubyte *arg1;
    GLubyte* arg1_nonconst = NULL;
    if(info[1]->IsString()) {
        Nan::Utf8String arg1_utf8(info[1]);
        arg1 = (const GLubyte*)(*arg1_utf8);
    } else if(info[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(info[1]);
        arg1_nonconst = new GLubyte[arg1_array->Length()];
        arg1 = (const GLubyte*)arg1_nonconst;
    } else if(info[1]->IsObject()) {
        arg1 = (const GLubyte*)glbind_get_buffer_data(info[1]);
    } else if(info[1]->IsNumber()) {
        arg1 = (const GLubyte*)(info[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib4ubv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    return;
}
NAN_METHOD(EXPORT_vertexAttrib4uiv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    const GLuint *arg1;
    GLuint* arg1_nonconst = NULL;
    if(info[1]->IsString()) {
        Nan::Utf8String arg1_utf8(info[1]);
        arg1 = (const GLuint*)(*arg1_utf8);
    } else if(info[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(info[1]);
        arg1_nonconst = new GLuint[arg1_array->Length()];
        arg1 = (const GLuint*)arg1_nonconst;
    } else if(info[1]->IsObject()) {
        arg1 = (const GLuint*)glbind_get_buffer_data(info[1]);
    } else if(info[1]->IsNumber()) {
        arg1 = (const GLuint*)(info[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib4uiv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    return;
}
NAN_METHOD(EXPORT_vertexAttrib4usv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    const GLushort *arg1;
    GLushort* arg1_nonconst = NULL;
    if(info[1]->IsString()) {
        Nan::Utf8String arg1_utf8(info[1]);
        arg1 = (const GLushort*)(*arg1_utf8);
    } else if(info[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(info[1]);
        arg1_nonconst = new GLushort[arg1_array->Length()];
        arg1 = (const GLushort*)arg1_nonconst;
    } else if(info[1]->IsObject()) {
        arg1 = (const GLushort*)glbind_get_buffer_data(info[1]);
    } else if(info[1]->IsNumber()) {
        arg1 = (const GLushort*)(info[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib4usv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    return;
}
NAN_METHOD(EXPORT_vertexAttribPointer) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLenum arg2;
    arg2 = info[2]->Uint32Value();
    GLboolean arg3;
    arg3 = info[3]->Uint32Value();
    GLsizei arg4;
    arg4 = info[4]->Int32Value();
    const GLvoid *arg5;
    GLubyte* arg5_nonconst = NULL;
    if(info[5]->IsString()) {
        Nan::Utf8String arg5_utf8(info[5]);
        arg5 = (const GLvoid*)(*arg5_utf8);
    } else if(info[5]->IsArray()) {
        v8::Handle<v8::Array> arg5_array = v8::Handle<v8::Array>::Cast(info[5]);
        arg5_nonconst = new GLubyte[arg5_array->Length()];
        arg5 = (const GLvoid*)arg5_nonconst;
    } else if(info[5]->IsObject()) {
        arg5 = (const GLvoid*)glbind_get_buffer_data(info[5]);
    } else if(info[5]->IsNumber()) {
        arg5 = (const GLvoid*)(info[5]->IntegerValue());
    } else {
        arg5 = NULL;
    }
    glVertexAttribPointer(arg0, arg1, arg2, arg3, arg4, arg5);
    if(arg5_nonconst) delete [] arg5_nonconst;
    return;
}
NAN_METHOD(EXPORT_uniformMatrix2x3fv) {
    Nan::HandleScope scope;
    // None
    GLint arg0;
    arg0 = info[0]->Int32Value();
    GLsizei arg1;
    arg1 = info[1]->Int32Value();
    GLboolean arg2;
    arg2 = info[2]->Uint32Value();
    const GLfloat *arg3;
    GLfloat* arg3_nonconst = NULL;
    if(info[3]->IsString()) {
        Nan::Utf8String arg3_utf8(info[3]);
        arg3 = (const GLfloat*)(*arg3_utf8);
    } else if(info[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(info[3]);
        arg3_nonconst = new GLfloat[arg3_array->Length()];
        arg3 = (const GLfloat*)arg3_nonconst;
    } else if(info[3]->IsObject()) {
        arg3 = (const GLfloat*)glbind_get_buffer_data(info[3]);
    } else if(info[3]->IsNumber()) {
        arg3 = (const GLfloat*)(info[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glUniformMatrix2x3fv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    return;
}
NAN_METHOD(EXPORT_uniformMatrix3x2fv) {
    Nan::HandleScope scope;
    // None
    GLint arg0;
    arg0 = info[0]->Int32Value();
    GLsizei arg1;
    arg1 = info[1]->Int32Value();
    GLboolean arg2;
    arg2 = info[2]->Uint32Value();
    const GLfloat *arg3;
    GLfloat* arg3_nonconst = NULL;
    if(info[3]->IsString()) {
        Nan::Utf8String arg3_utf8(info[3]);
        arg3 = (const GLfloat*)(*arg3_utf8);
    } else if(info[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(info[3]);
        arg3_nonconst = new GLfloat[arg3_array->Length()];
        arg3 = (const GLfloat*)arg3_nonconst;
    } else if(info[3]->IsObject()) {
        arg3 = (const GLfloat*)glbind_get_buffer_data(info[3]);
    } else if(info[3]->IsNumber()) {
        arg3 = (const GLfloat*)(info[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glUniformMatrix3x2fv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    return;
}
NAN_METHOD(EXPORT_uniformMatrix2x4fv) {
    Nan::HandleScope scope;
    // None
    GLint arg0;
    arg0 = info[0]->Int32Value();
    GLsizei arg1;
    arg1 = info[1]->Int32Value();
    GLboolean arg2;
    arg2 = info[2]->Uint32Value();
    const GLfloat *arg3;
    GLfloat* arg3_nonconst = NULL;
    if(info[3]->IsString()) {
        Nan::Utf8String arg3_utf8(info[3]);
        arg3 = (const GLfloat*)(*arg3_utf8);
    } else if(info[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(info[3]);
        arg3_nonconst = new GLfloat[arg3_array->Length()];
        arg3 = (const GLfloat*)arg3_nonconst;
    } else if(info[3]->IsObject()) {
        arg3 = (const GLfloat*)glbind_get_buffer_data(info[3]);
    } else if(info[3]->IsNumber()) {
        arg3 = (const GLfloat*)(info[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glUniformMatrix2x4fv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    return;
}
NAN_METHOD(EXPORT_uniformMatrix4x2fv) {
    Nan::HandleScope scope;
    // None
    GLint arg0;
    arg0 = info[0]->Int32Value();
    GLsizei arg1;
    arg1 = info[1]->Int32Value();
    GLboolean arg2;
    arg2 = info[2]->Uint32Value();
    const GLfloat *arg3;
    GLfloat* arg3_nonconst = NULL;
    if(info[3]->IsString()) {
        Nan::Utf8String arg3_utf8(info[3]);
        arg3 = (const GLfloat*)(*arg3_utf8);
    } else if(info[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(info[3]);
        arg3_nonconst = new GLfloat[arg3_array->Length()];
        arg3 = (const GLfloat*)arg3_nonconst;
    } else if(info[3]->IsObject()) {
        arg3 = (const GLfloat*)glbind_get_buffer_data(info[3]);
    } else if(info[3]->IsNumber()) {
        arg3 = (const GLfloat*)(info[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glUniformMatrix4x2fv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    return;
}
NAN_METHOD(EXPORT_uniformMatrix3x4fv) {
    Nan::HandleScope scope;
    // None
    GLint arg0;
    arg0 = info[0]->Int32Value();
    GLsizei arg1;
    arg1 = info[1]->Int32Value();
    GLboolean arg2;
    arg2 = info[2]->Uint32Value();
    const GLfloat *arg3;
    GLfloat* arg3_nonconst = NULL;
    if(info[3]->IsString()) {
        Nan::Utf8String arg3_utf8(info[3]);
        arg3 = (const GLfloat*)(*arg3_utf8);
    } else if(info[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(info[3]);
        arg3_nonconst = new GLfloat[arg3_array->Length()];
        arg3 = (const GLfloat*)arg3_nonconst;
    } else if(info[3]->IsObject()) {
        arg3 = (const GLfloat*)glbind_get_buffer_data(info[3]);
    } else if(info[3]->IsNumber()) {
        arg3 = (const GLfloat*)(info[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glUniformMatrix3x4fv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    return;
}
NAN_METHOD(EXPORT_uniformMatrix4x3fv) {
    Nan::HandleScope scope;
    // None
    GLint arg0;
    arg0 = info[0]->Int32Value();
    GLsizei arg1;
    arg1 = info[1]->Int32Value();
    GLboolean arg2;
    arg2 = info[2]->Uint32Value();
    const GLfloat *arg3;
    GLfloat* arg3_nonconst = NULL;
    if(info[3]->IsString()) {
        Nan::Utf8String arg3_utf8(info[3]);
        arg3 = (const GLfloat*)(*arg3_utf8);
    } else if(info[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(info[3]);
        arg3_nonconst = new GLfloat[arg3_array->Length()];
        arg3 = (const GLfloat*)arg3_nonconst;
    } else if(info[3]->IsObject()) {
        arg3 = (const GLfloat*)glbind_get_buffer_data(info[3]);
    } else if(info[3]->IsNumber()) {
        arg3 = (const GLfloat*)(info[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glUniformMatrix4x3fv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    return;
}
NAN_METHOD(EXPORT_colorMaski) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    GLboolean arg1;
    arg1 = info[1]->Uint32Value();
    GLboolean arg2;
    arg2 = info[2]->Uint32Value();
    GLboolean arg3;
    arg3 = info[3]->Uint32Value();
    GLboolean arg4;
    arg4 = info[4]->Uint32Value();
    glColorMaski(arg0, arg1, arg2, arg3, arg4);
    return;
}
NAN_METHOD(EXPORT_getBooleani_v) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLuint arg1;
    arg1 = info[1]->Uint32Value();
    GLboolean *arg2;
    GLboolean* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (GLboolean*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLboolean[arg2_array->Length()];
        arg2 = (GLboolean*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (GLboolean*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (GLboolean*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetBooleani_v(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_getIntegeri_v) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLuint arg1;
    arg1 = info[1]->Uint32Value();
    GLint *arg2;
    GLint* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (GLint*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLint[arg2_array->Length()];
        arg2 = (GLint*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (GLint*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (GLint*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetIntegeri_v(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_enablei) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLuint arg1;
    arg1 = info[1]->Uint32Value();
    glEnablei(arg0, arg1);
    return;
}
NAN_METHOD(EXPORT_disablei) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLuint arg1;
    arg1 = info[1]->Uint32Value();
    glDisablei(arg0, arg1);
    return;
}
NAN_METHOD(EXPORT_isEnabledi) {
    Nan::HandleScope scope;
    // <gltypes.Type instance at 0x102dd84d0>
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLuint arg1;
    arg1 = info[1]->Uint32Value();
    GLboolean result;
    result = glIsEnabledi(arg0, arg1);
    v8::Handle<v8::Value> result_js = Nan::New<v8::Uint32>(result);
    return info.GetReturnValue().Set(result_js);
}
NAN_METHOD(EXPORT_beginTransformFeedback) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    glBeginTransformFeedback(arg0);
    return;
}
NAN_METHOD(EXPORT_endTransformFeedback) {
    Nan::HandleScope scope;
    // None
    glEndTransformFeedback();
    return;
}
NAN_METHOD(EXPORT_bindBufferRange) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLuint arg1;
    arg1 = info[1]->Uint32Value();
    GLuint arg2;
    arg2 = info[2]->Uint32Value();
    GLintptr arg3;
    arg3 = info[3]->IntegerValue();
    GLsizeiptr arg4;
    arg4 = (uint64_t)info[4]->IntegerValue();
    glBindBufferRange(arg0, arg1, arg2, arg3, arg4);
    return;
}
NAN_METHOD(EXPORT_bindBufferBase) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLuint arg1;
    arg1 = info[1]->Uint32Value();
    GLuint arg2;
    arg2 = info[2]->Uint32Value();
    glBindBufferBase(arg0, arg1, arg2);
    return;
}
NAN_METHOD(EXPORT_getTransformFeedbackVarying) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    GLuint arg1;
    arg1 = info[1]->Uint32Value();
    GLsizei arg2;
    arg2 = info[2]->Int32Value();
    GLsizei *arg3;
    GLsizei* arg3_nonconst = NULL;
    if(info[3]->IsString()) {
        Nan::Utf8String arg3_utf8(info[3]);
        arg3 = (GLsizei*)(*arg3_utf8);
    } else if(info[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(info[3]);
        arg3_nonconst = new GLsizei[arg3_array->Length()];
        arg3 = (GLsizei*)arg3_nonconst;
    } else if(info[3]->IsObject()) {
        arg3 = (GLsizei*)glbind_get_buffer_data(info[3]);
    } else if(info[3]->IsNumber()) {
        arg3 = (GLsizei*)(info[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    GLsizei *arg4;
    GLsizei* arg4_nonconst = NULL;
    if(info[4]->IsString()) {
        Nan::Utf8String arg4_utf8(info[4]);
        arg4 = (GLsizei*)(*arg4_utf8);
    } else if(info[4]->IsArray()) {
        v8::Handle<v8::Array> arg4_array = v8::Handle<v8::Array>::Cast(info[4]);
        arg4_nonconst = new GLsizei[arg4_array->Length()];
        arg4 = (GLsizei*)arg4_nonconst;
    } else if(info[4]->IsObject()) {
        arg4 = (GLsizei*)glbind_get_buffer_data(info[4]);
    } else if(info[4]->IsNumber()) {
        arg4 = (GLsizei*)(info[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    GLenum *arg5;
    GLenum* arg5_nonconst = NULL;
    if(info[5]->IsString()) {
        Nan::Utf8String arg5_utf8(info[5]);
        arg5 = (GLenum*)(*arg5_utf8);
    } else if(info[5]->IsArray()) {
        v8::Handle<v8::Array> arg5_array = v8::Handle<v8::Array>::Cast(info[5]);
        arg5_nonconst = new GLenum[arg5_array->Length()];
        arg5 = (GLenum*)arg5_nonconst;
    } else if(info[5]->IsObject()) {
        arg5 = (GLenum*)glbind_get_buffer_data(info[5]);
    } else if(info[5]->IsNumber()) {
        arg5 = (GLenum*)(info[5]->IntegerValue());
    } else {
        arg5 = NULL;
    }
    GLchar *arg6;
    GLchar* arg6_nonconst = NULL;
    if(info[6]->IsString()) {
        Nan::Utf8String arg6_utf8(info[6]);
        arg6 = (GLchar*)(*arg6_utf8);
    } else if(info[6]->IsArray()) {
        v8::Handle<v8::Array> arg6_array = v8::Handle<v8::Array>::Cast(info[6]);
        arg6_nonconst = new GLchar[arg6_array->Length()];
        arg6 = (GLchar*)arg6_nonconst;
    } else if(info[6]->IsObject()) {
        arg6 = (GLchar*)glbind_get_buffer_data(info[6]);
    } else if(info[6]->IsNumber()) {
        arg6 = (GLchar*)(info[6]->IntegerValue());
    } else {
        arg6 = NULL;
    }
    glGetTransformFeedbackVarying(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
    if(arg3_nonconst) delete [] arg3_nonconst;
    if(arg4_nonconst) delete [] arg4_nonconst;
    if(arg5_nonconst) delete [] arg5_nonconst;
    if(arg6_nonconst) delete [] arg6_nonconst;
    return;
}
NAN_METHOD(EXPORT_clampColor) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    glClampColor(arg0, arg1);
    return;
}
NAN_METHOD(EXPORT_beginConditionalRender) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    glBeginConditionalRender(arg0, arg1);
    return;
}
NAN_METHOD(EXPORT_endConditionalRender) {
    Nan::HandleScope scope;
    // None
    glEndConditionalRender();
    return;
}
NAN_METHOD(EXPORT_vertexAttribIPointer) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLenum arg2;
    arg2 = info[2]->Uint32Value();
    GLsizei arg3;
    arg3 = info[3]->Int32Value();
    const GLvoid *arg4;
    GLubyte* arg4_nonconst = NULL;
    if(info[4]->IsString()) {
        Nan::Utf8String arg4_utf8(info[4]);
        arg4 = (const GLvoid*)(*arg4_utf8);
    } else if(info[4]->IsArray()) {
        v8::Handle<v8::Array> arg4_array = v8::Handle<v8::Array>::Cast(info[4]);
        arg4_nonconst = new GLubyte[arg4_array->Length()];
        arg4 = (const GLvoid*)arg4_nonconst;
    } else if(info[4]->IsObject()) {
        arg4 = (const GLvoid*)glbind_get_buffer_data(info[4]);
    } else if(info[4]->IsNumber()) {
        arg4 = (const GLvoid*)(info[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glVertexAttribIPointer(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    return;
}
NAN_METHOD(EXPORT_getVertexAttribIiv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    GLint *arg2;
    GLint* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (GLint*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLint[arg2_array->Length()];
        arg2 = (GLint*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (GLint*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (GLint*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetVertexAttribIiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_getVertexAttribIuiv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    GLuint *arg2;
    GLuint* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (GLuint*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLuint[arg2_array->Length()];
        arg2 = (GLuint*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (GLuint*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (GLuint*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetVertexAttribIuiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_vertexAttribI1i) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    GLint arg1;
    arg1 = info[1]->Int32Value();
    glVertexAttribI1i(arg0, arg1);
    return;
}
NAN_METHOD(EXPORT_vertexAttribI2i) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLint arg2;
    arg2 = info[2]->Int32Value();
    glVertexAttribI2i(arg0, arg1, arg2);
    return;
}
NAN_METHOD(EXPORT_vertexAttribI3i) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLint arg2;
    arg2 = info[2]->Int32Value();
    GLint arg3;
    arg3 = info[3]->Int32Value();
    glVertexAttribI3i(arg0, arg1, arg2, arg3);
    return;
}
NAN_METHOD(EXPORT_vertexAttribI4i) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLint arg2;
    arg2 = info[2]->Int32Value();
    GLint arg3;
    arg3 = info[3]->Int32Value();
    GLint arg4;
    arg4 = info[4]->Int32Value();
    glVertexAttribI4i(arg0, arg1, arg2, arg3, arg4);
    return;
}
NAN_METHOD(EXPORT_vertexAttribI1ui) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    GLuint arg1;
    arg1 = info[1]->Uint32Value();
    glVertexAttribI1ui(arg0, arg1);
    return;
}
NAN_METHOD(EXPORT_vertexAttribI2ui) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    GLuint arg1;
    arg1 = info[1]->Uint32Value();
    GLuint arg2;
    arg2 = info[2]->Uint32Value();
    glVertexAttribI2ui(arg0, arg1, arg2);
    return;
}
NAN_METHOD(EXPORT_vertexAttribI3ui) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    GLuint arg1;
    arg1 = info[1]->Uint32Value();
    GLuint arg2;
    arg2 = info[2]->Uint32Value();
    GLuint arg3;
    arg3 = info[3]->Uint32Value();
    glVertexAttribI3ui(arg0, arg1, arg2, arg3);
    return;
}
NAN_METHOD(EXPORT_vertexAttribI4ui) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    GLuint arg1;
    arg1 = info[1]->Uint32Value();
    GLuint arg2;
    arg2 = info[2]->Uint32Value();
    GLuint arg3;
    arg3 = info[3]->Uint32Value();
    GLuint arg4;
    arg4 = info[4]->Uint32Value();
    glVertexAttribI4ui(arg0, arg1, arg2, arg3, arg4);
    return;
}
NAN_METHOD(EXPORT_vertexAttribI1iv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    const GLint *arg1;
    GLint* arg1_nonconst = NULL;
    if(info[1]->IsString()) {
        Nan::Utf8String arg1_utf8(info[1]);
        arg1 = (const GLint*)(*arg1_utf8);
    } else if(info[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(info[1]);
        arg1_nonconst = new GLint[arg1_array->Length()];
        arg1 = (const GLint*)arg1_nonconst;
    } else if(info[1]->IsObject()) {
        arg1 = (const GLint*)glbind_get_buffer_data(info[1]);
    } else if(info[1]->IsNumber()) {
        arg1 = (const GLint*)(info[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttribI1iv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    return;
}
NAN_METHOD(EXPORT_vertexAttribI2iv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    const GLint *arg1;
    GLint* arg1_nonconst = NULL;
    if(info[1]->IsString()) {
        Nan::Utf8String arg1_utf8(info[1]);
        arg1 = (const GLint*)(*arg1_utf8);
    } else if(info[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(info[1]);
        arg1_nonconst = new GLint[arg1_array->Length()];
        arg1 = (const GLint*)arg1_nonconst;
    } else if(info[1]->IsObject()) {
        arg1 = (const GLint*)glbind_get_buffer_data(info[1]);
    } else if(info[1]->IsNumber()) {
        arg1 = (const GLint*)(info[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttribI2iv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    return;
}
NAN_METHOD(EXPORT_vertexAttribI3iv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    const GLint *arg1;
    GLint* arg1_nonconst = NULL;
    if(info[1]->IsString()) {
        Nan::Utf8String arg1_utf8(info[1]);
        arg1 = (const GLint*)(*arg1_utf8);
    } else if(info[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(info[1]);
        arg1_nonconst = new GLint[arg1_array->Length()];
        arg1 = (const GLint*)arg1_nonconst;
    } else if(info[1]->IsObject()) {
        arg1 = (const GLint*)glbind_get_buffer_data(info[1]);
    } else if(info[1]->IsNumber()) {
        arg1 = (const GLint*)(info[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttribI3iv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    return;
}
NAN_METHOD(EXPORT_vertexAttribI4iv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    const GLint *arg1;
    GLint* arg1_nonconst = NULL;
    if(info[1]->IsString()) {
        Nan::Utf8String arg1_utf8(info[1]);
        arg1 = (const GLint*)(*arg1_utf8);
    } else if(info[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(info[1]);
        arg1_nonconst = new GLint[arg1_array->Length()];
        arg1 = (const GLint*)arg1_nonconst;
    } else if(info[1]->IsObject()) {
        arg1 = (const GLint*)glbind_get_buffer_data(info[1]);
    } else if(info[1]->IsNumber()) {
        arg1 = (const GLint*)(info[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttribI4iv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    return;
}
NAN_METHOD(EXPORT_vertexAttribI1uiv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    const GLuint *arg1;
    GLuint* arg1_nonconst = NULL;
    if(info[1]->IsString()) {
        Nan::Utf8String arg1_utf8(info[1]);
        arg1 = (const GLuint*)(*arg1_utf8);
    } else if(info[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(info[1]);
        arg1_nonconst = new GLuint[arg1_array->Length()];
        arg1 = (const GLuint*)arg1_nonconst;
    } else if(info[1]->IsObject()) {
        arg1 = (const GLuint*)glbind_get_buffer_data(info[1]);
    } else if(info[1]->IsNumber()) {
        arg1 = (const GLuint*)(info[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttribI1uiv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    return;
}
NAN_METHOD(EXPORT_vertexAttribI2uiv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    const GLuint *arg1;
    GLuint* arg1_nonconst = NULL;
    if(info[1]->IsString()) {
        Nan::Utf8String arg1_utf8(info[1]);
        arg1 = (const GLuint*)(*arg1_utf8);
    } else if(info[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(info[1]);
        arg1_nonconst = new GLuint[arg1_array->Length()];
        arg1 = (const GLuint*)arg1_nonconst;
    } else if(info[1]->IsObject()) {
        arg1 = (const GLuint*)glbind_get_buffer_data(info[1]);
    } else if(info[1]->IsNumber()) {
        arg1 = (const GLuint*)(info[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttribI2uiv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    return;
}
NAN_METHOD(EXPORT_vertexAttribI3uiv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    const GLuint *arg1;
    GLuint* arg1_nonconst = NULL;
    if(info[1]->IsString()) {
        Nan::Utf8String arg1_utf8(info[1]);
        arg1 = (const GLuint*)(*arg1_utf8);
    } else if(info[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(info[1]);
        arg1_nonconst = new GLuint[arg1_array->Length()];
        arg1 = (const GLuint*)arg1_nonconst;
    } else if(info[1]->IsObject()) {
        arg1 = (const GLuint*)glbind_get_buffer_data(info[1]);
    } else if(info[1]->IsNumber()) {
        arg1 = (const GLuint*)(info[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttribI3uiv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    return;
}
NAN_METHOD(EXPORT_vertexAttribI4uiv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    const GLuint *arg1;
    GLuint* arg1_nonconst = NULL;
    if(info[1]->IsString()) {
        Nan::Utf8String arg1_utf8(info[1]);
        arg1 = (const GLuint*)(*arg1_utf8);
    } else if(info[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(info[1]);
        arg1_nonconst = new GLuint[arg1_array->Length()];
        arg1 = (const GLuint*)arg1_nonconst;
    } else if(info[1]->IsObject()) {
        arg1 = (const GLuint*)glbind_get_buffer_data(info[1]);
    } else if(info[1]->IsNumber()) {
        arg1 = (const GLuint*)(info[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttribI4uiv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    return;
}
NAN_METHOD(EXPORT_vertexAttribI4bv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    const GLbyte *arg1;
    GLbyte* arg1_nonconst = NULL;
    if(info[1]->IsString()) {
        Nan::Utf8String arg1_utf8(info[1]);
        arg1 = (const GLbyte*)(*arg1_utf8);
    } else if(info[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(info[1]);
        arg1_nonconst = new GLbyte[arg1_array->Length()];
        arg1 = (const GLbyte*)arg1_nonconst;
    } else if(info[1]->IsObject()) {
        arg1 = (const GLbyte*)glbind_get_buffer_data(info[1]);
    } else if(info[1]->IsNumber()) {
        arg1 = (const GLbyte*)(info[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttribI4bv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    return;
}
NAN_METHOD(EXPORT_vertexAttribI4sv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    const GLshort *arg1;
    GLshort* arg1_nonconst = NULL;
    if(info[1]->IsString()) {
        Nan::Utf8String arg1_utf8(info[1]);
        arg1 = (const GLshort*)(*arg1_utf8);
    } else if(info[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(info[1]);
        arg1_nonconst = new GLshort[arg1_array->Length()];
        arg1 = (const GLshort*)arg1_nonconst;
    } else if(info[1]->IsObject()) {
        arg1 = (const GLshort*)glbind_get_buffer_data(info[1]);
    } else if(info[1]->IsNumber()) {
        arg1 = (const GLshort*)(info[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttribI4sv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    return;
}
NAN_METHOD(EXPORT_vertexAttribI4ubv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    const GLubyte *arg1;
    GLubyte* arg1_nonconst = NULL;
    if(info[1]->IsString()) {
        Nan::Utf8String arg1_utf8(info[1]);
        arg1 = (const GLubyte*)(*arg1_utf8);
    } else if(info[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(info[1]);
        arg1_nonconst = new GLubyte[arg1_array->Length()];
        arg1 = (const GLubyte*)arg1_nonconst;
    } else if(info[1]->IsObject()) {
        arg1 = (const GLubyte*)glbind_get_buffer_data(info[1]);
    } else if(info[1]->IsNumber()) {
        arg1 = (const GLubyte*)(info[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttribI4ubv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    return;
}
NAN_METHOD(EXPORT_vertexAttribI4usv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    const GLushort *arg1;
    GLushort* arg1_nonconst = NULL;
    if(info[1]->IsString()) {
        Nan::Utf8String arg1_utf8(info[1]);
        arg1 = (const GLushort*)(*arg1_utf8);
    } else if(info[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(info[1]);
        arg1_nonconst = new GLushort[arg1_array->Length()];
        arg1 = (const GLushort*)arg1_nonconst;
    } else if(info[1]->IsObject()) {
        arg1 = (const GLushort*)glbind_get_buffer_data(info[1]);
    } else if(info[1]->IsNumber()) {
        arg1 = (const GLushort*)(info[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttribI4usv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    return;
}
NAN_METHOD(EXPORT_getUniformuiv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLuint *arg2;
    GLuint* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (GLuint*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLuint[arg2_array->Length()];
        arg2 = (GLuint*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (GLuint*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (GLuint*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetUniformuiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_bindFragDataLocation) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLuint arg1;
    arg1 = info[1]->Uint32Value();
    const GLchar* arg2;
    Nan::Utf8String arg2_utf8(info[2]);
    arg2 = (GLchar*)*arg2_utf8;
    glBindFragDataLocation(arg0, arg1, arg2);
    return;
}
NAN_METHOD(EXPORT_getFragDataLocation) {
    Nan::HandleScope scope;
    // <gltypes.Type instance at 0x102dd8638>
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    const GLchar* arg1;
    Nan::Utf8String arg1_utf8(info[1]);
    arg1 = (GLchar*)*arg1_utf8;
    GLint result;
    result = glGetFragDataLocation(arg0, arg1);
    v8::Handle<v8::Value> result_js = Nan::New<v8::Int32>(result);
    return info.GetReturnValue().Set(result_js);
}
NAN_METHOD(EXPORT_uniform1ui) {
    Nan::HandleScope scope;
    // None
    GLint arg0;
    arg0 = info[0]->Int32Value();
    GLuint arg1;
    arg1 = info[1]->Uint32Value();
    glUniform1ui(arg0, arg1);
    return;
}
NAN_METHOD(EXPORT_uniform2ui) {
    Nan::HandleScope scope;
    // None
    GLint arg0;
    arg0 = info[0]->Int32Value();
    GLuint arg1;
    arg1 = info[1]->Uint32Value();
    GLuint arg2;
    arg2 = info[2]->Uint32Value();
    glUniform2ui(arg0, arg1, arg2);
    return;
}
NAN_METHOD(EXPORT_uniform3ui) {
    Nan::HandleScope scope;
    // None
    GLint arg0;
    arg0 = info[0]->Int32Value();
    GLuint arg1;
    arg1 = info[1]->Uint32Value();
    GLuint arg2;
    arg2 = info[2]->Uint32Value();
    GLuint arg3;
    arg3 = info[3]->Uint32Value();
    glUniform3ui(arg0, arg1, arg2, arg3);
    return;
}
NAN_METHOD(EXPORT_uniform4ui) {
    Nan::HandleScope scope;
    // None
    GLint arg0;
    arg0 = info[0]->Int32Value();
    GLuint arg1;
    arg1 = info[1]->Uint32Value();
    GLuint arg2;
    arg2 = info[2]->Uint32Value();
    GLuint arg3;
    arg3 = info[3]->Uint32Value();
    GLuint arg4;
    arg4 = info[4]->Uint32Value();
    glUniform4ui(arg0, arg1, arg2, arg3, arg4);
    return;
}
NAN_METHOD(EXPORT_uniform1uiv) {
    Nan::HandleScope scope;
    // None
    GLint arg0;
    arg0 = info[0]->Int32Value();
    GLsizei arg1;
    arg1 = info[1]->Int32Value();
    const GLuint *arg2;
    GLuint* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (const GLuint*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLuint[arg2_array->Length()];
        arg2 = (const GLuint*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (const GLuint*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (const GLuint*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glUniform1uiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_uniform2uiv) {
    Nan::HandleScope scope;
    // None
    GLint arg0;
    arg0 = info[0]->Int32Value();
    GLsizei arg1;
    arg1 = info[1]->Int32Value();
    const GLuint *arg2;
    GLuint* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (const GLuint*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLuint[arg2_array->Length()];
        arg2 = (const GLuint*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (const GLuint*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (const GLuint*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glUniform2uiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_uniform3uiv) {
    Nan::HandleScope scope;
    // None
    GLint arg0;
    arg0 = info[0]->Int32Value();
    GLsizei arg1;
    arg1 = info[1]->Int32Value();
    const GLuint *arg2;
    GLuint* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (const GLuint*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLuint[arg2_array->Length()];
        arg2 = (const GLuint*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (const GLuint*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (const GLuint*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glUniform3uiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_uniform4uiv) {
    Nan::HandleScope scope;
    // None
    GLint arg0;
    arg0 = info[0]->Int32Value();
    GLsizei arg1;
    arg1 = info[1]->Int32Value();
    const GLuint *arg2;
    GLuint* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (const GLuint*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLuint[arg2_array->Length()];
        arg2 = (const GLuint*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (const GLuint*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (const GLuint*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glUniform4uiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_texParameterIiv) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    const GLint *arg2;
    GLint* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (const GLint*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLint[arg2_array->Length()];
        arg2 = (const GLint*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (const GLint*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (const GLint*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glTexParameterIiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_texParameterIuiv) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    const GLuint *arg2;
    GLuint* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (const GLuint*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLuint[arg2_array->Length()];
        arg2 = (const GLuint*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (const GLuint*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (const GLuint*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glTexParameterIuiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_getTexParameterIiv) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    GLint *arg2;
    GLint* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (GLint*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLint[arg2_array->Length()];
        arg2 = (GLint*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (GLint*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (GLint*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetTexParameterIiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_getTexParameterIuiv) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    GLuint *arg2;
    GLuint* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (GLuint*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLuint[arg2_array->Length()];
        arg2 = (GLuint*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (GLuint*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (GLuint*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetTexParameterIuiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_clearBufferiv) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLint arg1;
    arg1 = info[1]->Int32Value();
    const GLint *arg2;
    GLint* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (const GLint*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLint[arg2_array->Length()];
        arg2 = (const GLint*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (const GLint*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (const GLint*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glClearBufferiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_clearBufferuiv) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLint arg1;
    arg1 = info[1]->Int32Value();
    const GLuint *arg2;
    GLuint* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (const GLuint*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLuint[arg2_array->Length()];
        arg2 = (const GLuint*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (const GLuint*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (const GLuint*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glClearBufferuiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_clearBufferfv) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLint arg1;
    arg1 = info[1]->Int32Value();
    const GLfloat *arg2;
    GLfloat* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (const GLfloat*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLfloat[arg2_array->Length()];
        arg2 = (const GLfloat*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (const GLfloat*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (const GLfloat*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glClearBufferfv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_clearBufferfi) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLfloat arg2;
    arg2 = (float)info[2]->NumberValue();
    GLint arg3;
    arg3 = info[3]->Int32Value();
    glClearBufferfi(arg0, arg1, arg2, arg3);
    return;
}
NAN_METHOD(EXPORT_getStringi) {
    Nan::HandleScope scope;
    // <gltypes.UStringType instance at 0x102dd8a70>
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLuint arg1;
    arg1 = info[1]->Uint32Value();
    const GLubyte* result;
    result = glGetStringi(arg0, arg1);
    v8::Handle<v8::Value> result_js;
    if(result) {
        result_js = Nan::New<v8::String>((const char*)result, strlen((const char*)result)).ToLocalChecked();
    } else {
        result_js = Nan::Undefined();
    }
    return info.GetReturnValue().Set(result_js);
}
NAN_METHOD(EXPORT_drawArraysInstanced) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLsizei arg2;
    arg2 = info[2]->Int32Value();
    GLsizei arg3;
    arg3 = info[3]->Int32Value();
    glDrawArraysInstanced(arg0, arg1, arg2, arg3);
    return;
}
NAN_METHOD(EXPORT_drawElementsInstanced) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLsizei arg1;
    arg1 = info[1]->Int32Value();
    GLenum arg2;
    arg2 = info[2]->Uint32Value();
    const GLvoid *arg3;
    GLubyte* arg3_nonconst = NULL;
    if(info[3]->IsString()) {
        Nan::Utf8String arg3_utf8(info[3]);
        arg3 = (const GLvoid*)(*arg3_utf8);
    } else if(info[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(info[3]);
        arg3_nonconst = new GLubyte[arg3_array->Length()];
        arg3 = (const GLvoid*)arg3_nonconst;
    } else if(info[3]->IsObject()) {
        arg3 = (const GLvoid*)glbind_get_buffer_data(info[3]);
    } else if(info[3]->IsNumber()) {
        arg3 = (const GLvoid*)(info[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    GLsizei arg4;
    arg4 = info[4]->Int32Value();
    glDrawElementsInstanced(arg0, arg1, arg2, arg3, arg4);
    if(arg3_nonconst) delete [] arg3_nonconst;
    return;
}
NAN_METHOD(EXPORT_texBuffer) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    GLuint arg2;
    if(info[2]->IsNumber()) {
        arg2 = info[2]->IntegerValue();
    } else {
        arg2 = Nan::ObjectWrap::Unwrap<NODE_Buffer>(info[2]->ToObject())->gl_handle;
    }
    glTexBuffer(arg0, arg1, arg2);
    return;
}
NAN_METHOD(EXPORT_primitiveRestartIndex) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    glPrimitiveRestartIndex(arg0);
    return;
}
NAN_METHOD(EXPORT_getInteger64i_v) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLuint arg1;
    arg1 = info[1]->Uint32Value();
    GLint64 *arg2;
    GLint64* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (GLint64*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLint64[arg2_array->Length()];
        arg2 = (GLint64*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (GLint64*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (GLint64*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetInteger64i_v(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_getBufferParameteri64v) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    GLint64 *arg2;
    GLint64* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (GLint64*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLint64[arg2_array->Length()];
        arg2 = (GLint64*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (GLint64*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (GLint64*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetBufferParameteri64v(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_framebufferTexture) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    GLuint arg2;
    if(info[2]->IsNumber()) {
        arg2 = info[2]->IntegerValue();
    } else {
        arg2 = Nan::ObjectWrap::Unwrap<NODE_Texture>(info[2]->ToObject())->gl_handle;
    }
    GLint arg3;
    arg3 = info[3]->Int32Value();
    glFramebufferTexture(arg0, arg1, arg2, arg3);
    return;
}
NAN_METHOD(EXPORT_vertexAttribDivisor) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    GLuint arg1;
    arg1 = info[1]->Uint32Value();
    glVertexAttribDivisor(arg0, arg1);
    return;
}
NAN_METHOD(EXPORT_minSampleShading) {
    Nan::HandleScope scope;
    // None
    GLfloat arg0;
    arg0 = (float)info[0]->NumberValue();
    glMinSampleShading(arg0);
    return;
}
NAN_METHOD(EXPORT_blendEquationi) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    glBlendEquationi(arg0, arg1);
    return;
}
NAN_METHOD(EXPORT_blendEquationSeparatei) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    GLenum arg2;
    arg2 = info[2]->Uint32Value();
    glBlendEquationSeparatei(arg0, arg1, arg2);
    return;
}
NAN_METHOD(EXPORT_blendFunci) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    GLenum arg2;
    arg2 = info[2]->Uint32Value();
    glBlendFunci(arg0, arg1, arg2);
    return;
}
NAN_METHOD(EXPORT_blendFuncSeparatei) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    GLenum arg2;
    arg2 = info[2]->Uint32Value();
    GLenum arg3;
    arg3 = info[3]->Uint32Value();
    GLenum arg4;
    arg4 = info[4]->Uint32Value();
    glBlendFuncSeparatei(arg0, arg1, arg2, arg3, arg4);
    return;
}
NAN_METHOD(EXPORT_isRenderbuffer) {
    Nan::HandleScope scope;
    // <gltypes.Type instance at 0x102dd84d0>
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Renderbuffer>(info[0]->ToObject())->gl_handle;
    }
    GLboolean result;
    result = glIsRenderbuffer(arg0);
    v8::Handle<v8::Value> result_js = Nan::New<v8::Uint32>(result);
    return info.GetReturnValue().Set(result_js);
}
NAN_METHOD(EXPORT_bindRenderbuffer) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLuint arg1;
    if(info[1]->IsNumber()) {
        arg1 = info[1]->IntegerValue();
    } else {
        arg1 = Nan::ObjectWrap::Unwrap<NODE_Renderbuffer>(info[1]->ToObject())->gl_handle;
    }
    glBindRenderbuffer(arg0, arg1);
    return;
}
NAN_METHOD(EXPORT_genRenderbuffers) {
    Nan::HandleScope scope;
    // None
    GLsizei arg0;
    arg0 = info[0]->Int32Value();
    GLuint* arg1;
    v8::Handle<v8::Array> info_1_array = v8::Handle<v8::Array>::Cast(info[1]);
    arg1 = new GLuint[info_1_array->Length()];
    glGenRenderbuffers(arg0, arg1);
    for(uint32_t i = 0; i < info_1_array->Length(); i++) {
        info_1_array->Set(i, NODE_Renderbuffer::fromGLHandle(arg1[i]));
    }
    delete [] arg1;
    return;
}
NAN_METHOD(EXPORT_renderbufferStorage) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    GLsizei arg2;
    arg2 = info[2]->Int32Value();
    GLsizei arg3;
    arg3 = info[3]->Int32Value();
    glRenderbufferStorage(arg0, arg1, arg2, arg3);
    return;
}
NAN_METHOD(EXPORT_getRenderbufferParameteriv) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    GLint *arg2;
    GLint* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (GLint*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLint[arg2_array->Length()];
        arg2 = (GLint*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (GLint*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (GLint*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetRenderbufferParameteriv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_isFramebuffer) {
    Nan::HandleScope scope;
    // <gltypes.Type instance at 0x102dd84d0>
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Framebuffer>(info[0]->ToObject())->gl_handle;
    }
    GLboolean result;
    result = glIsFramebuffer(arg0);
    v8::Handle<v8::Value> result_js = Nan::New<v8::Uint32>(result);
    return info.GetReturnValue().Set(result_js);
}
NAN_METHOD(EXPORT_bindFramebuffer) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLuint arg1;
    if(info[1]->IsNumber()) {
        arg1 = info[1]->IntegerValue();
    } else {
        arg1 = Nan::ObjectWrap::Unwrap<NODE_Framebuffer>(info[1]->ToObject())->gl_handle;
    }
    glBindFramebuffer(arg0, arg1);
    return;
}
NAN_METHOD(EXPORT_genFramebuffers) {
    Nan::HandleScope scope;
    // None
    GLsizei arg0;
    arg0 = info[0]->Int32Value();
    GLuint* arg1;
    v8::Handle<v8::Array> info_1_array = v8::Handle<v8::Array>::Cast(info[1]);
    arg1 = new GLuint[info_1_array->Length()];
    glGenFramebuffers(arg0, arg1);
    for(uint32_t i = 0; i < info_1_array->Length(); i++) {
        info_1_array->Set(i, NODE_Framebuffer::fromGLHandle(arg1[i]));
    }
    delete [] arg1;
    return;
}
NAN_METHOD(EXPORT_checkFramebufferStatus) {
    Nan::HandleScope scope;
    // <gltypes.Type instance at 0x102dd85a8>
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLenum result;
    result = glCheckFramebufferStatus(arg0);
    v8::Handle<v8::Value> result_js = Nan::New<v8::Uint32>(result);
    return info.GetReturnValue().Set(result_js);
}
NAN_METHOD(EXPORT_framebufferTexture1D) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    GLenum arg2;
    arg2 = info[2]->Uint32Value();
    GLuint arg3;
    if(info[3]->IsNumber()) {
        arg3 = info[3]->IntegerValue();
    } else {
        arg3 = Nan::ObjectWrap::Unwrap<NODE_Texture>(info[3]->ToObject())->gl_handle;
    }
    GLint arg4;
    arg4 = info[4]->Int32Value();
    glFramebufferTexture1D(arg0, arg1, arg2, arg3, arg4);
    return;
}
NAN_METHOD(EXPORT_framebufferTexture2D) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    GLenum arg2;
    arg2 = info[2]->Uint32Value();
    GLuint arg3;
    if(info[3]->IsNumber()) {
        arg3 = info[3]->IntegerValue();
    } else {
        arg3 = Nan::ObjectWrap::Unwrap<NODE_Texture>(info[3]->ToObject())->gl_handle;
    }
    GLint arg4;
    arg4 = info[4]->Int32Value();
    glFramebufferTexture2D(arg0, arg1, arg2, arg3, arg4);
    return;
}
NAN_METHOD(EXPORT_framebufferTexture3D) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    GLenum arg2;
    arg2 = info[2]->Uint32Value();
    GLuint arg3;
    if(info[3]->IsNumber()) {
        arg3 = info[3]->IntegerValue();
    } else {
        arg3 = Nan::ObjectWrap::Unwrap<NODE_Texture>(info[3]->ToObject())->gl_handle;
    }
    GLint arg4;
    arg4 = info[4]->Int32Value();
    GLint arg5;
    arg5 = info[5]->Int32Value();
    glFramebufferTexture3D(arg0, arg1, arg2, arg3, arg4, arg5);
    return;
}
NAN_METHOD(EXPORT_framebufferRenderbuffer) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    GLenum arg2;
    arg2 = info[2]->Uint32Value();
    GLuint arg3;
    if(info[3]->IsNumber()) {
        arg3 = info[3]->IntegerValue();
    } else {
        arg3 = Nan::ObjectWrap::Unwrap<NODE_Renderbuffer>(info[3]->ToObject())->gl_handle;
    }
    glFramebufferRenderbuffer(arg0, arg1, arg2, arg3);
    return;
}
NAN_METHOD(EXPORT_getFramebufferAttachmentParameteriv) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    GLenum arg2;
    arg2 = info[2]->Uint32Value();
    GLint *arg3;
    GLint* arg3_nonconst = NULL;
    if(info[3]->IsString()) {
        Nan::Utf8String arg3_utf8(info[3]);
        arg3 = (GLint*)(*arg3_utf8);
    } else if(info[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(info[3]);
        arg3_nonconst = new GLint[arg3_array->Length()];
        arg3 = (GLint*)arg3_nonconst;
    } else if(info[3]->IsObject()) {
        arg3 = (GLint*)glbind_get_buffer_data(info[3]);
    } else if(info[3]->IsNumber()) {
        arg3 = (GLint*)(info[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glGetFramebufferAttachmentParameteriv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    return;
}
NAN_METHOD(EXPORT_generateMipmap) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    glGenerateMipmap(arg0);
    return;
}
NAN_METHOD(EXPORT_blitFramebuffer) {
    Nan::HandleScope scope;
    // None
    GLint arg0;
    arg0 = info[0]->Int32Value();
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLint arg2;
    arg2 = info[2]->Int32Value();
    GLint arg3;
    arg3 = info[3]->Int32Value();
    GLint arg4;
    arg4 = info[4]->Int32Value();
    GLint arg5;
    arg5 = info[5]->Int32Value();
    GLint arg6;
    arg6 = info[6]->Int32Value();
    GLint arg7;
    arg7 = info[7]->Int32Value();
    GLbitfield arg8;
    arg8 = info[8]->Uint32Value();
    GLenum arg9;
    arg9 = info[9]->Uint32Value();
    glBlitFramebuffer(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
    return;
}
NAN_METHOD(EXPORT_renderbufferStorageMultisample) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLsizei arg1;
    arg1 = info[1]->Int32Value();
    GLenum arg2;
    arg2 = info[2]->Uint32Value();
    GLsizei arg3;
    arg3 = info[3]->Int32Value();
    GLsizei arg4;
    arg4 = info[4]->Int32Value();
    glRenderbufferStorageMultisample(arg0, arg1, arg2, arg3, arg4);
    return;
}
NAN_METHOD(EXPORT_framebufferTextureLayer) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    GLuint arg2;
    if(info[2]->IsNumber()) {
        arg2 = info[2]->IntegerValue();
    } else {
        arg2 = Nan::ObjectWrap::Unwrap<NODE_Texture>(info[2]->ToObject())->gl_handle;
    }
    GLint arg3;
    arg3 = info[3]->Int32Value();
    GLint arg4;
    arg4 = info[4]->Int32Value();
    glFramebufferTextureLayer(arg0, arg1, arg2, arg3, arg4);
    return;
}
NAN_METHOD(EXPORT_mapBufferRange) {
    Nan::HandleScope scope;
    // <gltypes.PointerType instance at 0x102e6e098>
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLintptr arg1;
    arg1 = info[1]->IntegerValue();
    GLsizeiptr arg2;
    arg2 = (uint64_t)info[2]->IntegerValue();
    GLbitfield arg3;
    arg3 = info[3]->Uint32Value();
    GLvoid *result;
    result = glMapBufferRange(arg0, arg1, arg2, arg3);
    v8::Handle<v8::Value> result_js = Nan::NewBuffer((char*)result, 0, do_nothing_release_callback, NULL).ToLocalChecked();
    return info.GetReturnValue().Set(result_js);
}
NAN_METHOD(EXPORT_flushMappedBufferRange) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLintptr arg1;
    arg1 = info[1]->IntegerValue();
    GLsizeiptr arg2;
    arg2 = (uint64_t)info[2]->IntegerValue();
    glFlushMappedBufferRange(arg0, arg1, arg2);
    return;
}
NAN_METHOD(EXPORT_bindVertexArray) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_VertexArray>(info[0]->ToObject())->gl_handle;
    }
    glBindVertexArray(arg0);
    return;
}
NAN_METHOD(EXPORT_genVertexArrays) {
    Nan::HandleScope scope;
    // None
    GLsizei arg0;
    arg0 = info[0]->Int32Value();
    GLuint* arg1;
    v8::Handle<v8::Array> info_1_array = v8::Handle<v8::Array>::Cast(info[1]);
    arg1 = new GLuint[info_1_array->Length()];
    glGenVertexArrays(arg0, arg1);
    for(uint32_t i = 0; i < info_1_array->Length(); i++) {
        info_1_array->Set(i, NODE_VertexArray::fromGLHandle(arg1[i]));
    }
    delete [] arg1;
    return;
}
NAN_METHOD(EXPORT_isVertexArray) {
    Nan::HandleScope scope;
    // <gltypes.Type instance at 0x102dd84d0>
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_VertexArray>(info[0]->ToObject())->gl_handle;
    }
    GLboolean result;
    result = glIsVertexArray(arg0);
    v8::Handle<v8::Value> result_js = Nan::New<v8::Uint32>(result);
    return info.GetReturnValue().Set(result_js);
}
NAN_METHOD(EXPORT_getActiveUniformsiv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLsizei arg1;
    arg1 = info[1]->Int32Value();
    const GLuint *arg2;
    GLuint* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (const GLuint*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLuint[arg2_array->Length()];
        arg2 = (const GLuint*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (const GLuint*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (const GLuint*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    GLenum arg3;
    arg3 = info[3]->Uint32Value();
    GLint *arg4;
    GLint* arg4_nonconst = NULL;
    if(info[4]->IsString()) {
        Nan::Utf8String arg4_utf8(info[4]);
        arg4 = (GLint*)(*arg4_utf8);
    } else if(info[4]->IsArray()) {
        v8::Handle<v8::Array> arg4_array = v8::Handle<v8::Array>::Cast(info[4]);
        arg4_nonconst = new GLint[arg4_array->Length()];
        arg4 = (GLint*)arg4_nonconst;
    } else if(info[4]->IsObject()) {
        arg4 = (GLint*)glbind_get_buffer_data(info[4]);
    } else if(info[4]->IsNumber()) {
        arg4 = (GLint*)(info[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glGetActiveUniformsiv(arg0, arg1, arg2, arg3, arg4);
    if(arg2_nonconst) delete [] arg2_nonconst;
    if(arg4_nonconst) delete [] arg4_nonconst;
    return;
}
NAN_METHOD(EXPORT_getActiveUniformName) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLuint arg1;
    arg1 = info[1]->Uint32Value();
    GLsizei arg2;
    arg2 = info[2]->Int32Value();
    GLsizei *arg3;
    GLsizei* arg3_nonconst = NULL;
    if(info[3]->IsString()) {
        Nan::Utf8String arg3_utf8(info[3]);
        arg3 = (GLsizei*)(*arg3_utf8);
    } else if(info[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(info[3]);
        arg3_nonconst = new GLsizei[arg3_array->Length()];
        arg3 = (GLsizei*)arg3_nonconst;
    } else if(info[3]->IsObject()) {
        arg3 = (GLsizei*)glbind_get_buffer_data(info[3]);
    } else if(info[3]->IsNumber()) {
        arg3 = (GLsizei*)(info[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    GLchar *arg4;
    GLchar* arg4_nonconst = NULL;
    if(info[4]->IsString()) {
        Nan::Utf8String arg4_utf8(info[4]);
        arg4 = (GLchar*)(*arg4_utf8);
    } else if(info[4]->IsArray()) {
        v8::Handle<v8::Array> arg4_array = v8::Handle<v8::Array>::Cast(info[4]);
        arg4_nonconst = new GLchar[arg4_array->Length()];
        arg4 = (GLchar*)arg4_nonconst;
    } else if(info[4]->IsObject()) {
        arg4 = (GLchar*)glbind_get_buffer_data(info[4]);
    } else if(info[4]->IsNumber()) {
        arg4 = (GLchar*)(info[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glGetActiveUniformName(arg0, arg1, arg2, arg3, arg4);
    if(arg3_nonconst) delete [] arg3_nonconst;
    if(arg4_nonconst) delete [] arg4_nonconst;
    return;
}
NAN_METHOD(EXPORT_getUniformBlockIndex) {
    Nan::HandleScope scope;
    // <gltypes.Type instance at 0x102dd8758>
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    const GLchar *arg1;
    GLchar* arg1_nonconst = NULL;
    if(info[1]->IsString()) {
        Nan::Utf8String arg1_utf8(info[1]);
        arg1 = (const GLchar*)(*arg1_utf8);
    } else if(info[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(info[1]);
        arg1_nonconst = new GLchar[arg1_array->Length()];
        arg1 = (const GLchar*)arg1_nonconst;
    } else if(info[1]->IsObject()) {
        arg1 = (const GLchar*)glbind_get_buffer_data(info[1]);
    } else if(info[1]->IsNumber()) {
        arg1 = (const GLchar*)(info[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    GLuint result;
    result = glGetUniformBlockIndex(arg0, arg1);
    v8::Handle<v8::Value> result_js = Nan::New<v8::Uint32>(result);
    if(arg1_nonconst) delete [] arg1_nonconst;
    return info.GetReturnValue().Set(result_js);
}
NAN_METHOD(EXPORT_getActiveUniformBlockiv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLuint arg1;
    arg1 = info[1]->Uint32Value();
    GLenum arg2;
    arg2 = info[2]->Uint32Value();
    GLint *arg3;
    GLint* arg3_nonconst = NULL;
    if(info[3]->IsString()) {
        Nan::Utf8String arg3_utf8(info[3]);
        arg3 = (GLint*)(*arg3_utf8);
    } else if(info[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(info[3]);
        arg3_nonconst = new GLint[arg3_array->Length()];
        arg3 = (GLint*)arg3_nonconst;
    } else if(info[3]->IsObject()) {
        arg3 = (GLint*)glbind_get_buffer_data(info[3]);
    } else if(info[3]->IsNumber()) {
        arg3 = (GLint*)(info[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glGetActiveUniformBlockiv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    return;
}
NAN_METHOD(EXPORT_getActiveUniformBlockName) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLuint arg1;
    arg1 = info[1]->Uint32Value();
    GLsizei arg2;
    arg2 = info[2]->Int32Value();
    GLsizei *arg3;
    GLsizei* arg3_nonconst = NULL;
    if(info[3]->IsString()) {
        Nan::Utf8String arg3_utf8(info[3]);
        arg3 = (GLsizei*)(*arg3_utf8);
    } else if(info[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(info[3]);
        arg3_nonconst = new GLsizei[arg3_array->Length()];
        arg3 = (GLsizei*)arg3_nonconst;
    } else if(info[3]->IsObject()) {
        arg3 = (GLsizei*)glbind_get_buffer_data(info[3]);
    } else if(info[3]->IsNumber()) {
        arg3 = (GLsizei*)(info[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    GLchar *arg4;
    GLchar* arg4_nonconst = NULL;
    if(info[4]->IsString()) {
        Nan::Utf8String arg4_utf8(info[4]);
        arg4 = (GLchar*)(*arg4_utf8);
    } else if(info[4]->IsArray()) {
        v8::Handle<v8::Array> arg4_array = v8::Handle<v8::Array>::Cast(info[4]);
        arg4_nonconst = new GLchar[arg4_array->Length()];
        arg4 = (GLchar*)arg4_nonconst;
    } else if(info[4]->IsObject()) {
        arg4 = (GLchar*)glbind_get_buffer_data(info[4]);
    } else if(info[4]->IsNumber()) {
        arg4 = (GLchar*)(info[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glGetActiveUniformBlockName(arg0, arg1, arg2, arg3, arg4);
    if(arg3_nonconst) delete [] arg3_nonconst;
    if(arg4_nonconst) delete [] arg4_nonconst;
    return;
}
NAN_METHOD(EXPORT_uniformBlockBinding) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLuint arg1;
    arg1 = info[1]->Uint32Value();
    GLuint arg2;
    arg2 = info[2]->Uint32Value();
    glUniformBlockBinding(arg0, arg1, arg2);
    return;
}
NAN_METHOD(EXPORT_copyBufferSubData) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    GLintptr arg2;
    arg2 = info[2]->IntegerValue();
    GLintptr arg3;
    arg3 = info[3]->IntegerValue();
    GLsizeiptr arg4;
    arg4 = (uint64_t)info[4]->IntegerValue();
    glCopyBufferSubData(arg0, arg1, arg2, arg3, arg4);
    return;
}
NAN_METHOD(EXPORT_drawElementsBaseVertex) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLsizei arg1;
    arg1 = info[1]->Int32Value();
    GLenum arg2;
    arg2 = info[2]->Uint32Value();
    const GLvoid *arg3;
    GLubyte* arg3_nonconst = NULL;
    if(info[3]->IsString()) {
        Nan::Utf8String arg3_utf8(info[3]);
        arg3 = (const GLvoid*)(*arg3_utf8);
    } else if(info[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(info[3]);
        arg3_nonconst = new GLubyte[arg3_array->Length()];
        arg3 = (const GLvoid*)arg3_nonconst;
    } else if(info[3]->IsObject()) {
        arg3 = (const GLvoid*)glbind_get_buffer_data(info[3]);
    } else if(info[3]->IsNumber()) {
        arg3 = (const GLvoid*)(info[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    GLint arg4;
    arg4 = info[4]->Int32Value();
    glDrawElementsBaseVertex(arg0, arg1, arg2, arg3, arg4);
    if(arg3_nonconst) delete [] arg3_nonconst;
    return;
}
NAN_METHOD(EXPORT_drawRangeElementsBaseVertex) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLuint arg1;
    arg1 = info[1]->Uint32Value();
    GLuint arg2;
    arg2 = info[2]->Uint32Value();
    GLsizei arg3;
    arg3 = info[3]->Int32Value();
    GLenum arg4;
    arg4 = info[4]->Uint32Value();
    const GLvoid *arg5;
    GLubyte* arg5_nonconst = NULL;
    if(info[5]->IsString()) {
        Nan::Utf8String arg5_utf8(info[5]);
        arg5 = (const GLvoid*)(*arg5_utf8);
    } else if(info[5]->IsArray()) {
        v8::Handle<v8::Array> arg5_array = v8::Handle<v8::Array>::Cast(info[5]);
        arg5_nonconst = new GLubyte[arg5_array->Length()];
        arg5 = (const GLvoid*)arg5_nonconst;
    } else if(info[5]->IsObject()) {
        arg5 = (const GLvoid*)glbind_get_buffer_data(info[5]);
    } else if(info[5]->IsNumber()) {
        arg5 = (const GLvoid*)(info[5]->IntegerValue());
    } else {
        arg5 = NULL;
    }
    GLint arg6;
    arg6 = info[6]->Int32Value();
    glDrawRangeElementsBaseVertex(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
    if(arg5_nonconst) delete [] arg5_nonconst;
    return;
}
NAN_METHOD(EXPORT_drawElementsInstancedBaseVertex) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLsizei arg1;
    arg1 = info[1]->Int32Value();
    GLenum arg2;
    arg2 = info[2]->Uint32Value();
    const GLvoid *arg3;
    GLubyte* arg3_nonconst = NULL;
    if(info[3]->IsString()) {
        Nan::Utf8String arg3_utf8(info[3]);
        arg3 = (const GLvoid*)(*arg3_utf8);
    } else if(info[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(info[3]);
        arg3_nonconst = new GLubyte[arg3_array->Length()];
        arg3 = (const GLvoid*)arg3_nonconst;
    } else if(info[3]->IsObject()) {
        arg3 = (const GLvoid*)glbind_get_buffer_data(info[3]);
    } else if(info[3]->IsNumber()) {
        arg3 = (const GLvoid*)(info[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    GLsizei arg4;
    arg4 = info[4]->Int32Value();
    GLint arg5;
    arg5 = info[5]->Int32Value();
    glDrawElementsInstancedBaseVertex(arg0, arg1, arg2, arg3, arg4, arg5);
    if(arg3_nonconst) delete [] arg3_nonconst;
    return;
}
NAN_METHOD(EXPORT_provokingVertex) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    glProvokingVertex(arg0);
    return;
}
NAN_METHOD(EXPORT_texImage2DMultisample) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLsizei arg1;
    arg1 = info[1]->Int32Value();
    GLint arg2;
    arg2 = info[2]->Int32Value();
    GLsizei arg3;
    arg3 = info[3]->Int32Value();
    GLsizei arg4;
    arg4 = info[4]->Int32Value();
    GLboolean arg5;
    arg5 = info[5]->Uint32Value();
    glTexImage2DMultisample(arg0, arg1, arg2, arg3, arg4, arg5);
    return;
}
NAN_METHOD(EXPORT_texImage3DMultisample) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLsizei arg1;
    arg1 = info[1]->Int32Value();
    GLint arg2;
    arg2 = info[2]->Int32Value();
    GLsizei arg3;
    arg3 = info[3]->Int32Value();
    GLsizei arg4;
    arg4 = info[4]->Int32Value();
    GLsizei arg5;
    arg5 = info[5]->Int32Value();
    GLboolean arg6;
    arg6 = info[6]->Uint32Value();
    glTexImage3DMultisample(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
    return;
}
NAN_METHOD(EXPORT_getMultisamplefv) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLuint arg1;
    arg1 = info[1]->Uint32Value();
    GLfloat *arg2;
    GLfloat* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (GLfloat*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLfloat[arg2_array->Length()];
        arg2 = (GLfloat*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (GLfloat*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (GLfloat*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetMultisamplefv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_sampleMaski) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    GLbitfield arg1;
    arg1 = info[1]->Uint32Value();
    glSampleMaski(arg0, arg1);
    return;
}
NAN_METHOD(EXPORT_bindFragDataLocationIndexed) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLuint arg1;
    arg1 = info[1]->Uint32Value();
    GLuint arg2;
    arg2 = info[2]->Uint32Value();
    const GLchar *arg3;
    GLchar* arg3_nonconst = NULL;
    if(info[3]->IsString()) {
        Nan::Utf8String arg3_utf8(info[3]);
        arg3 = (const GLchar*)(*arg3_utf8);
    } else if(info[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(info[3]);
        arg3_nonconst = new GLchar[arg3_array->Length()];
        arg3 = (const GLchar*)arg3_nonconst;
    } else if(info[3]->IsObject()) {
        arg3 = (const GLchar*)glbind_get_buffer_data(info[3]);
    } else if(info[3]->IsNumber()) {
        arg3 = (const GLchar*)(info[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glBindFragDataLocationIndexed(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    return;
}
NAN_METHOD(EXPORT_getFragDataIndex) {
    Nan::HandleScope scope;
    // <gltypes.Type instance at 0x102dd8638>
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    const GLchar *arg1;
    GLchar* arg1_nonconst = NULL;
    if(info[1]->IsString()) {
        Nan::Utf8String arg1_utf8(info[1]);
        arg1 = (const GLchar*)(*arg1_utf8);
    } else if(info[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(info[1]);
        arg1_nonconst = new GLchar[arg1_array->Length()];
        arg1 = (const GLchar*)arg1_nonconst;
    } else if(info[1]->IsObject()) {
        arg1 = (const GLchar*)glbind_get_buffer_data(info[1]);
    } else if(info[1]->IsNumber()) {
        arg1 = (const GLchar*)(info[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    GLint result;
    result = glGetFragDataIndex(arg0, arg1);
    v8::Handle<v8::Value> result_js = Nan::New<v8::Int32>(result);
    if(arg1_nonconst) delete [] arg1_nonconst;
    return info.GetReturnValue().Set(result_js);
}
NAN_METHOD(EXPORT_genSamplers) {
    Nan::HandleScope scope;
    // None
    GLsizei arg0;
    arg0 = info[0]->Int32Value();
    GLuint* arg1;
    v8::Handle<v8::Array> info_1_array = v8::Handle<v8::Array>::Cast(info[1]);
    arg1 = new GLuint[info_1_array->Length()];
    glGenSamplers(arg0, arg1);
    for(uint32_t i = 0; i < info_1_array->Length(); i++) {
        info_1_array->Set(i, NODE_Sampler::fromGLHandle(arg1[i]));
    }
    delete [] arg1;
    return;
}
NAN_METHOD(EXPORT_isSampler) {
    Nan::HandleScope scope;
    // <gltypes.Type instance at 0x102dd84d0>
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Sampler>(info[0]->ToObject())->gl_handle;
    }
    GLboolean result;
    result = glIsSampler(arg0);
    v8::Handle<v8::Value> result_js = Nan::New<v8::Uint32>(result);
    return info.GetReturnValue().Set(result_js);
}
NAN_METHOD(EXPORT_bindSampler) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    GLuint arg1;
    if(info[1]->IsNumber()) {
        arg1 = info[1]->IntegerValue();
    } else {
        arg1 = Nan::ObjectWrap::Unwrap<NODE_Sampler>(info[1]->ToObject())->gl_handle;
    }
    glBindSampler(arg0, arg1);
    return;
}
NAN_METHOD(EXPORT_samplerParameteri) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Sampler>(info[0]->ToObject())->gl_handle;
    }
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    GLint arg2;
    arg2 = info[2]->Int32Value();
    glSamplerParameteri(arg0, arg1, arg2);
    return;
}
NAN_METHOD(EXPORT_samplerParameteriv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Sampler>(info[0]->ToObject())->gl_handle;
    }
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    const GLint *arg2;
    GLint* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (const GLint*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLint[arg2_array->Length()];
        arg2 = (const GLint*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (const GLint*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (const GLint*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glSamplerParameteriv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_samplerParameterf) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Sampler>(info[0]->ToObject())->gl_handle;
    }
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    GLfloat arg2;
    arg2 = (float)info[2]->NumberValue();
    glSamplerParameterf(arg0, arg1, arg2);
    return;
}
NAN_METHOD(EXPORT_samplerParameterfv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Sampler>(info[0]->ToObject())->gl_handle;
    }
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    const GLfloat *arg2;
    GLfloat* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (const GLfloat*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLfloat[arg2_array->Length()];
        arg2 = (const GLfloat*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (const GLfloat*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (const GLfloat*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glSamplerParameterfv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_samplerParameterIiv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Sampler>(info[0]->ToObject())->gl_handle;
    }
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    const GLint *arg2;
    GLint* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (const GLint*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLint[arg2_array->Length()];
        arg2 = (const GLint*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (const GLint*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (const GLint*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glSamplerParameterIiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_samplerParameterIuiv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Sampler>(info[0]->ToObject())->gl_handle;
    }
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    const GLuint *arg2;
    GLuint* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (const GLuint*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLuint[arg2_array->Length()];
        arg2 = (const GLuint*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (const GLuint*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (const GLuint*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glSamplerParameterIuiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_getSamplerParameteriv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Sampler>(info[0]->ToObject())->gl_handle;
    }
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    GLint *arg2;
    GLint* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (GLint*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLint[arg2_array->Length()];
        arg2 = (GLint*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (GLint*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (GLint*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetSamplerParameteriv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_getSamplerParameterIiv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Sampler>(info[0]->ToObject())->gl_handle;
    }
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    GLint *arg2;
    GLint* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (GLint*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLint[arg2_array->Length()];
        arg2 = (GLint*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (GLint*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (GLint*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetSamplerParameterIiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_getSamplerParameterfv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Sampler>(info[0]->ToObject())->gl_handle;
    }
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    GLfloat *arg2;
    GLfloat* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (GLfloat*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLfloat[arg2_array->Length()];
        arg2 = (GLfloat*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (GLfloat*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (GLfloat*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetSamplerParameterfv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_getSamplerParameterIuiv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Sampler>(info[0]->ToObject())->gl_handle;
    }
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    GLuint *arg2;
    GLuint* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (GLuint*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLuint[arg2_array->Length()];
        arg2 = (GLuint*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (GLuint*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (GLuint*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetSamplerParameterIuiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_queryCounter) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    glQueryCounter(arg0, arg1);
    return;
}
NAN_METHOD(EXPORT_getQueryObjecti64v) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    GLint64 *arg2;
    GLint64* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (GLint64*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLint64[arg2_array->Length()];
        arg2 = (GLint64*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (GLint64*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (GLint64*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetQueryObjecti64v(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_getQueryObjectui64v) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    GLuint64 *arg2;
    GLuint64* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (GLuint64*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLuint64[arg2_array->Length()];
        arg2 = (GLuint64*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (GLuint64*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (GLuint64*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetQueryObjectui64v(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_vertexAttribP1ui) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    GLboolean arg2;
    arg2 = info[2]->Uint32Value();
    GLuint arg3;
    arg3 = info[3]->Uint32Value();
    glVertexAttribP1ui(arg0, arg1, arg2, arg3);
    return;
}
NAN_METHOD(EXPORT_vertexAttribP1uiv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    GLboolean arg2;
    arg2 = info[2]->Uint32Value();
    const GLuint *arg3;
    GLuint* arg3_nonconst = NULL;
    if(info[3]->IsString()) {
        Nan::Utf8String arg3_utf8(info[3]);
        arg3 = (const GLuint*)(*arg3_utf8);
    } else if(info[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(info[3]);
        arg3_nonconst = new GLuint[arg3_array->Length()];
        arg3 = (const GLuint*)arg3_nonconst;
    } else if(info[3]->IsObject()) {
        arg3 = (const GLuint*)glbind_get_buffer_data(info[3]);
    } else if(info[3]->IsNumber()) {
        arg3 = (const GLuint*)(info[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glVertexAttribP1uiv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    return;
}
NAN_METHOD(EXPORT_vertexAttribP2ui) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    GLboolean arg2;
    arg2 = info[2]->Uint32Value();
    GLuint arg3;
    arg3 = info[3]->Uint32Value();
    glVertexAttribP2ui(arg0, arg1, arg2, arg3);
    return;
}
NAN_METHOD(EXPORT_vertexAttribP2uiv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    GLboolean arg2;
    arg2 = info[2]->Uint32Value();
    const GLuint *arg3;
    GLuint* arg3_nonconst = NULL;
    if(info[3]->IsString()) {
        Nan::Utf8String arg3_utf8(info[3]);
        arg3 = (const GLuint*)(*arg3_utf8);
    } else if(info[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(info[3]);
        arg3_nonconst = new GLuint[arg3_array->Length()];
        arg3 = (const GLuint*)arg3_nonconst;
    } else if(info[3]->IsObject()) {
        arg3 = (const GLuint*)glbind_get_buffer_data(info[3]);
    } else if(info[3]->IsNumber()) {
        arg3 = (const GLuint*)(info[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glVertexAttribP2uiv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    return;
}
NAN_METHOD(EXPORT_vertexAttribP3ui) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    GLboolean arg2;
    arg2 = info[2]->Uint32Value();
    GLuint arg3;
    arg3 = info[3]->Uint32Value();
    glVertexAttribP3ui(arg0, arg1, arg2, arg3);
    return;
}
NAN_METHOD(EXPORT_vertexAttribP3uiv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    GLboolean arg2;
    arg2 = info[2]->Uint32Value();
    const GLuint *arg3;
    GLuint* arg3_nonconst = NULL;
    if(info[3]->IsString()) {
        Nan::Utf8String arg3_utf8(info[3]);
        arg3 = (const GLuint*)(*arg3_utf8);
    } else if(info[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(info[3]);
        arg3_nonconst = new GLuint[arg3_array->Length()];
        arg3 = (const GLuint*)arg3_nonconst;
    } else if(info[3]->IsObject()) {
        arg3 = (const GLuint*)glbind_get_buffer_data(info[3]);
    } else if(info[3]->IsNumber()) {
        arg3 = (const GLuint*)(info[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glVertexAttribP3uiv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    return;
}
NAN_METHOD(EXPORT_vertexAttribP4ui) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    GLboolean arg2;
    arg2 = info[2]->Uint32Value();
    GLuint arg3;
    arg3 = info[3]->Uint32Value();
    glVertexAttribP4ui(arg0, arg1, arg2, arg3);
    return;
}
NAN_METHOD(EXPORT_vertexAttribP4uiv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    GLboolean arg2;
    arg2 = info[2]->Uint32Value();
    const GLuint *arg3;
    GLuint* arg3_nonconst = NULL;
    if(info[3]->IsString()) {
        Nan::Utf8String arg3_utf8(info[3]);
        arg3 = (const GLuint*)(*arg3_utf8);
    } else if(info[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(info[3]);
        arg3_nonconst = new GLuint[arg3_array->Length()];
        arg3 = (const GLuint*)arg3_nonconst;
    } else if(info[3]->IsObject()) {
        arg3 = (const GLuint*)glbind_get_buffer_data(info[3]);
    } else if(info[3]->IsNumber()) {
        arg3 = (const GLuint*)(info[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glVertexAttribP4uiv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    return;
}
NAN_METHOD(EXPORT_drawArraysIndirect) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    const GLvoid *arg1;
    GLubyte* arg1_nonconst = NULL;
    if(info[1]->IsString()) {
        Nan::Utf8String arg1_utf8(info[1]);
        arg1 = (const GLvoid*)(*arg1_utf8);
    } else if(info[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(info[1]);
        arg1_nonconst = new GLubyte[arg1_array->Length()];
        arg1 = (const GLvoid*)arg1_nonconst;
    } else if(info[1]->IsObject()) {
        arg1 = (const GLvoid*)glbind_get_buffer_data(info[1]);
    } else if(info[1]->IsNumber()) {
        arg1 = (const GLvoid*)(info[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glDrawArraysIndirect(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    return;
}
NAN_METHOD(EXPORT_drawElementsIndirect) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    const GLvoid *arg2;
    GLubyte* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (const GLvoid*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLubyte[arg2_array->Length()];
        arg2 = (const GLvoid*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (const GLvoid*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (const GLvoid*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glDrawElementsIndirect(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_uniform1d) {
    Nan::HandleScope scope;
    // None
    GLint arg0;
    arg0 = info[0]->Int32Value();
    GLdouble arg1;
    arg1 = info[1]->NumberValue();
    glUniform1d(arg0, arg1);
    return;
}
NAN_METHOD(EXPORT_uniform2d) {
    Nan::HandleScope scope;
    // None
    GLint arg0;
    arg0 = info[0]->Int32Value();
    GLdouble arg1;
    arg1 = info[1]->NumberValue();
    GLdouble arg2;
    arg2 = info[2]->NumberValue();
    glUniform2d(arg0, arg1, arg2);
    return;
}
NAN_METHOD(EXPORT_uniform3d) {
    Nan::HandleScope scope;
    // None
    GLint arg0;
    arg0 = info[0]->Int32Value();
    GLdouble arg1;
    arg1 = info[1]->NumberValue();
    GLdouble arg2;
    arg2 = info[2]->NumberValue();
    GLdouble arg3;
    arg3 = info[3]->NumberValue();
    glUniform3d(arg0, arg1, arg2, arg3);
    return;
}
NAN_METHOD(EXPORT_uniform4d) {
    Nan::HandleScope scope;
    // None
    GLint arg0;
    arg0 = info[0]->Int32Value();
    GLdouble arg1;
    arg1 = info[1]->NumberValue();
    GLdouble arg2;
    arg2 = info[2]->NumberValue();
    GLdouble arg3;
    arg3 = info[3]->NumberValue();
    GLdouble arg4;
    arg4 = info[4]->NumberValue();
    glUniform4d(arg0, arg1, arg2, arg3, arg4);
    return;
}
NAN_METHOD(EXPORT_uniform1dv) {
    Nan::HandleScope scope;
    // None
    GLint arg0;
    arg0 = info[0]->Int32Value();
    GLsizei arg1;
    arg1 = info[1]->Int32Value();
    const GLdouble *arg2;
    GLdouble* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (const GLdouble*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLdouble[arg2_array->Length()];
        arg2 = (const GLdouble*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (const GLdouble*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (const GLdouble*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glUniform1dv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_uniform2dv) {
    Nan::HandleScope scope;
    // None
    GLint arg0;
    arg0 = info[0]->Int32Value();
    GLsizei arg1;
    arg1 = info[1]->Int32Value();
    const GLdouble *arg2;
    GLdouble* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (const GLdouble*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLdouble[arg2_array->Length()];
        arg2 = (const GLdouble*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (const GLdouble*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (const GLdouble*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glUniform2dv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_uniform3dv) {
    Nan::HandleScope scope;
    // None
    GLint arg0;
    arg0 = info[0]->Int32Value();
    GLsizei arg1;
    arg1 = info[1]->Int32Value();
    const GLdouble *arg2;
    GLdouble* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (const GLdouble*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLdouble[arg2_array->Length()];
        arg2 = (const GLdouble*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (const GLdouble*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (const GLdouble*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glUniform3dv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_uniform4dv) {
    Nan::HandleScope scope;
    // None
    GLint arg0;
    arg0 = info[0]->Int32Value();
    GLsizei arg1;
    arg1 = info[1]->Int32Value();
    const GLdouble *arg2;
    GLdouble* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (const GLdouble*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLdouble[arg2_array->Length()];
        arg2 = (const GLdouble*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (const GLdouble*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (const GLdouble*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glUniform4dv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_uniformMatrix2dv) {
    Nan::HandleScope scope;
    // None
    GLint arg0;
    arg0 = info[0]->Int32Value();
    GLsizei arg1;
    arg1 = info[1]->Int32Value();
    GLboolean arg2;
    arg2 = info[2]->Uint32Value();
    const GLdouble *arg3;
    GLdouble* arg3_nonconst = NULL;
    if(info[3]->IsString()) {
        Nan::Utf8String arg3_utf8(info[3]);
        arg3 = (const GLdouble*)(*arg3_utf8);
    } else if(info[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(info[3]);
        arg3_nonconst = new GLdouble[arg3_array->Length()];
        arg3 = (const GLdouble*)arg3_nonconst;
    } else if(info[3]->IsObject()) {
        arg3 = (const GLdouble*)glbind_get_buffer_data(info[3]);
    } else if(info[3]->IsNumber()) {
        arg3 = (const GLdouble*)(info[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glUniformMatrix2dv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    return;
}
NAN_METHOD(EXPORT_uniformMatrix3dv) {
    Nan::HandleScope scope;
    // None
    GLint arg0;
    arg0 = info[0]->Int32Value();
    GLsizei arg1;
    arg1 = info[1]->Int32Value();
    GLboolean arg2;
    arg2 = info[2]->Uint32Value();
    const GLdouble *arg3;
    GLdouble* arg3_nonconst = NULL;
    if(info[3]->IsString()) {
        Nan::Utf8String arg3_utf8(info[3]);
        arg3 = (const GLdouble*)(*arg3_utf8);
    } else if(info[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(info[3]);
        arg3_nonconst = new GLdouble[arg3_array->Length()];
        arg3 = (const GLdouble*)arg3_nonconst;
    } else if(info[3]->IsObject()) {
        arg3 = (const GLdouble*)glbind_get_buffer_data(info[3]);
    } else if(info[3]->IsNumber()) {
        arg3 = (const GLdouble*)(info[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glUniformMatrix3dv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    return;
}
NAN_METHOD(EXPORT_uniformMatrix4dv) {
    Nan::HandleScope scope;
    // None
    GLint arg0;
    arg0 = info[0]->Int32Value();
    GLsizei arg1;
    arg1 = info[1]->Int32Value();
    GLboolean arg2;
    arg2 = info[2]->Uint32Value();
    const GLdouble *arg3;
    GLdouble* arg3_nonconst = NULL;
    if(info[3]->IsString()) {
        Nan::Utf8String arg3_utf8(info[3]);
        arg3 = (const GLdouble*)(*arg3_utf8);
    } else if(info[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(info[3]);
        arg3_nonconst = new GLdouble[arg3_array->Length()];
        arg3 = (const GLdouble*)arg3_nonconst;
    } else if(info[3]->IsObject()) {
        arg3 = (const GLdouble*)glbind_get_buffer_data(info[3]);
    } else if(info[3]->IsNumber()) {
        arg3 = (const GLdouble*)(info[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glUniformMatrix4dv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    return;
}
NAN_METHOD(EXPORT_uniformMatrix2x3dv) {
    Nan::HandleScope scope;
    // None
    GLint arg0;
    arg0 = info[0]->Int32Value();
    GLsizei arg1;
    arg1 = info[1]->Int32Value();
    GLboolean arg2;
    arg2 = info[2]->Uint32Value();
    const GLdouble *arg3;
    GLdouble* arg3_nonconst = NULL;
    if(info[3]->IsString()) {
        Nan::Utf8String arg3_utf8(info[3]);
        arg3 = (const GLdouble*)(*arg3_utf8);
    } else if(info[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(info[3]);
        arg3_nonconst = new GLdouble[arg3_array->Length()];
        arg3 = (const GLdouble*)arg3_nonconst;
    } else if(info[3]->IsObject()) {
        arg3 = (const GLdouble*)glbind_get_buffer_data(info[3]);
    } else if(info[3]->IsNumber()) {
        arg3 = (const GLdouble*)(info[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glUniformMatrix2x3dv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    return;
}
NAN_METHOD(EXPORT_uniformMatrix2x4dv) {
    Nan::HandleScope scope;
    // None
    GLint arg0;
    arg0 = info[0]->Int32Value();
    GLsizei arg1;
    arg1 = info[1]->Int32Value();
    GLboolean arg2;
    arg2 = info[2]->Uint32Value();
    const GLdouble *arg3;
    GLdouble* arg3_nonconst = NULL;
    if(info[3]->IsString()) {
        Nan::Utf8String arg3_utf8(info[3]);
        arg3 = (const GLdouble*)(*arg3_utf8);
    } else if(info[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(info[3]);
        arg3_nonconst = new GLdouble[arg3_array->Length()];
        arg3 = (const GLdouble*)arg3_nonconst;
    } else if(info[3]->IsObject()) {
        arg3 = (const GLdouble*)glbind_get_buffer_data(info[3]);
    } else if(info[3]->IsNumber()) {
        arg3 = (const GLdouble*)(info[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glUniformMatrix2x4dv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    return;
}
NAN_METHOD(EXPORT_uniformMatrix3x2dv) {
    Nan::HandleScope scope;
    // None
    GLint arg0;
    arg0 = info[0]->Int32Value();
    GLsizei arg1;
    arg1 = info[1]->Int32Value();
    GLboolean arg2;
    arg2 = info[2]->Uint32Value();
    const GLdouble *arg3;
    GLdouble* arg3_nonconst = NULL;
    if(info[3]->IsString()) {
        Nan::Utf8String arg3_utf8(info[3]);
        arg3 = (const GLdouble*)(*arg3_utf8);
    } else if(info[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(info[3]);
        arg3_nonconst = new GLdouble[arg3_array->Length()];
        arg3 = (const GLdouble*)arg3_nonconst;
    } else if(info[3]->IsObject()) {
        arg3 = (const GLdouble*)glbind_get_buffer_data(info[3]);
    } else if(info[3]->IsNumber()) {
        arg3 = (const GLdouble*)(info[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glUniformMatrix3x2dv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    return;
}
NAN_METHOD(EXPORT_uniformMatrix3x4dv) {
    Nan::HandleScope scope;
    // None
    GLint arg0;
    arg0 = info[0]->Int32Value();
    GLsizei arg1;
    arg1 = info[1]->Int32Value();
    GLboolean arg2;
    arg2 = info[2]->Uint32Value();
    const GLdouble *arg3;
    GLdouble* arg3_nonconst = NULL;
    if(info[3]->IsString()) {
        Nan::Utf8String arg3_utf8(info[3]);
        arg3 = (const GLdouble*)(*arg3_utf8);
    } else if(info[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(info[3]);
        arg3_nonconst = new GLdouble[arg3_array->Length()];
        arg3 = (const GLdouble*)arg3_nonconst;
    } else if(info[3]->IsObject()) {
        arg3 = (const GLdouble*)glbind_get_buffer_data(info[3]);
    } else if(info[3]->IsNumber()) {
        arg3 = (const GLdouble*)(info[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glUniformMatrix3x4dv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    return;
}
NAN_METHOD(EXPORT_uniformMatrix4x2dv) {
    Nan::HandleScope scope;
    // None
    GLint arg0;
    arg0 = info[0]->Int32Value();
    GLsizei arg1;
    arg1 = info[1]->Int32Value();
    GLboolean arg2;
    arg2 = info[2]->Uint32Value();
    const GLdouble *arg3;
    GLdouble* arg3_nonconst = NULL;
    if(info[3]->IsString()) {
        Nan::Utf8String arg3_utf8(info[3]);
        arg3 = (const GLdouble*)(*arg3_utf8);
    } else if(info[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(info[3]);
        arg3_nonconst = new GLdouble[arg3_array->Length()];
        arg3 = (const GLdouble*)arg3_nonconst;
    } else if(info[3]->IsObject()) {
        arg3 = (const GLdouble*)glbind_get_buffer_data(info[3]);
    } else if(info[3]->IsNumber()) {
        arg3 = (const GLdouble*)(info[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glUniformMatrix4x2dv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    return;
}
NAN_METHOD(EXPORT_uniformMatrix4x3dv) {
    Nan::HandleScope scope;
    // None
    GLint arg0;
    arg0 = info[0]->Int32Value();
    GLsizei arg1;
    arg1 = info[1]->Int32Value();
    GLboolean arg2;
    arg2 = info[2]->Uint32Value();
    const GLdouble *arg3;
    GLdouble* arg3_nonconst = NULL;
    if(info[3]->IsString()) {
        Nan::Utf8String arg3_utf8(info[3]);
        arg3 = (const GLdouble*)(*arg3_utf8);
    } else if(info[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(info[3]);
        arg3_nonconst = new GLdouble[arg3_array->Length()];
        arg3 = (const GLdouble*)arg3_nonconst;
    } else if(info[3]->IsObject()) {
        arg3 = (const GLdouble*)glbind_get_buffer_data(info[3]);
    } else if(info[3]->IsNumber()) {
        arg3 = (const GLdouble*)(info[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glUniformMatrix4x3dv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    return;
}
NAN_METHOD(EXPORT_getUniformdv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLdouble *arg2;
    GLdouble* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (GLdouble*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLdouble[arg2_array->Length()];
        arg2 = (GLdouble*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (GLdouble*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (GLdouble*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetUniformdv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_getSubroutineUniformLocation) {
    Nan::HandleScope scope;
    // <gltypes.Type instance at 0x102dd8638>
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    const GLchar *arg2;
    GLchar* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (const GLchar*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLchar[arg2_array->Length()];
        arg2 = (const GLchar*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (const GLchar*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (const GLchar*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    GLint result;
    result = glGetSubroutineUniformLocation(arg0, arg1, arg2);
    v8::Handle<v8::Value> result_js = Nan::New<v8::Int32>(result);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return info.GetReturnValue().Set(result_js);
}
NAN_METHOD(EXPORT_getSubroutineIndex) {
    Nan::HandleScope scope;
    // <gltypes.Type instance at 0x102dd8758>
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    const GLchar *arg2;
    GLchar* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (const GLchar*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLchar[arg2_array->Length()];
        arg2 = (const GLchar*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (const GLchar*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (const GLchar*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    GLuint result;
    result = glGetSubroutineIndex(arg0, arg1, arg2);
    v8::Handle<v8::Value> result_js = Nan::New<v8::Uint32>(result);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return info.GetReturnValue().Set(result_js);
}
NAN_METHOD(EXPORT_getActiveSubroutineUniformiv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    GLuint arg2;
    arg2 = info[2]->Uint32Value();
    GLenum arg3;
    arg3 = info[3]->Uint32Value();
    GLint *arg4;
    GLint* arg4_nonconst = NULL;
    if(info[4]->IsString()) {
        Nan::Utf8String arg4_utf8(info[4]);
        arg4 = (GLint*)(*arg4_utf8);
    } else if(info[4]->IsArray()) {
        v8::Handle<v8::Array> arg4_array = v8::Handle<v8::Array>::Cast(info[4]);
        arg4_nonconst = new GLint[arg4_array->Length()];
        arg4 = (GLint*)arg4_nonconst;
    } else if(info[4]->IsObject()) {
        arg4 = (GLint*)glbind_get_buffer_data(info[4]);
    } else if(info[4]->IsNumber()) {
        arg4 = (GLint*)(info[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glGetActiveSubroutineUniformiv(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    return;
}
NAN_METHOD(EXPORT_getActiveSubroutineUniformName) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    GLuint arg2;
    arg2 = info[2]->Uint32Value();
    GLsizei arg3;
    arg3 = info[3]->Int32Value();
    GLsizei *arg4;
    GLsizei* arg4_nonconst = NULL;
    if(info[4]->IsString()) {
        Nan::Utf8String arg4_utf8(info[4]);
        arg4 = (GLsizei*)(*arg4_utf8);
    } else if(info[4]->IsArray()) {
        v8::Handle<v8::Array> arg4_array = v8::Handle<v8::Array>::Cast(info[4]);
        arg4_nonconst = new GLsizei[arg4_array->Length()];
        arg4 = (GLsizei*)arg4_nonconst;
    } else if(info[4]->IsObject()) {
        arg4 = (GLsizei*)glbind_get_buffer_data(info[4]);
    } else if(info[4]->IsNumber()) {
        arg4 = (GLsizei*)(info[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    GLchar *arg5;
    GLchar* arg5_nonconst = NULL;
    if(info[5]->IsString()) {
        Nan::Utf8String arg5_utf8(info[5]);
        arg5 = (GLchar*)(*arg5_utf8);
    } else if(info[5]->IsArray()) {
        v8::Handle<v8::Array> arg5_array = v8::Handle<v8::Array>::Cast(info[5]);
        arg5_nonconst = new GLchar[arg5_array->Length()];
        arg5 = (GLchar*)arg5_nonconst;
    } else if(info[5]->IsObject()) {
        arg5 = (GLchar*)glbind_get_buffer_data(info[5]);
    } else if(info[5]->IsNumber()) {
        arg5 = (GLchar*)(info[5]->IntegerValue());
    } else {
        arg5 = NULL;
    }
    glGetActiveSubroutineUniformName(arg0, arg1, arg2, arg3, arg4, arg5);
    if(arg4_nonconst) delete [] arg4_nonconst;
    if(arg5_nonconst) delete [] arg5_nonconst;
    return;
}
NAN_METHOD(EXPORT_getActiveSubroutineName) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    GLuint arg2;
    arg2 = info[2]->Uint32Value();
    GLsizei arg3;
    arg3 = info[3]->Int32Value();
    GLsizei *arg4;
    GLsizei* arg4_nonconst = NULL;
    if(info[4]->IsString()) {
        Nan::Utf8String arg4_utf8(info[4]);
        arg4 = (GLsizei*)(*arg4_utf8);
    } else if(info[4]->IsArray()) {
        v8::Handle<v8::Array> arg4_array = v8::Handle<v8::Array>::Cast(info[4]);
        arg4_nonconst = new GLsizei[arg4_array->Length()];
        arg4 = (GLsizei*)arg4_nonconst;
    } else if(info[4]->IsObject()) {
        arg4 = (GLsizei*)glbind_get_buffer_data(info[4]);
    } else if(info[4]->IsNumber()) {
        arg4 = (GLsizei*)(info[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    GLchar *arg5;
    GLchar* arg5_nonconst = NULL;
    if(info[5]->IsString()) {
        Nan::Utf8String arg5_utf8(info[5]);
        arg5 = (GLchar*)(*arg5_utf8);
    } else if(info[5]->IsArray()) {
        v8::Handle<v8::Array> arg5_array = v8::Handle<v8::Array>::Cast(info[5]);
        arg5_nonconst = new GLchar[arg5_array->Length()];
        arg5 = (GLchar*)arg5_nonconst;
    } else if(info[5]->IsObject()) {
        arg5 = (GLchar*)glbind_get_buffer_data(info[5]);
    } else if(info[5]->IsNumber()) {
        arg5 = (GLchar*)(info[5]->IntegerValue());
    } else {
        arg5 = NULL;
    }
    glGetActiveSubroutineName(arg0, arg1, arg2, arg3, arg4, arg5);
    if(arg4_nonconst) delete [] arg4_nonconst;
    if(arg5_nonconst) delete [] arg5_nonconst;
    return;
}
NAN_METHOD(EXPORT_uniformSubroutinesuiv) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLsizei arg1;
    arg1 = info[1]->Int32Value();
    const GLuint *arg2;
    GLuint* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (const GLuint*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLuint[arg2_array->Length()];
        arg2 = (const GLuint*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (const GLuint*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (const GLuint*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glUniformSubroutinesuiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_getUniformSubroutineuiv) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLuint *arg2;
    GLuint* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (GLuint*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLuint[arg2_array->Length()];
        arg2 = (GLuint*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (GLuint*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (GLuint*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetUniformSubroutineuiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_getProgramStageiv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    GLenum arg2;
    arg2 = info[2]->Uint32Value();
    GLint *arg3;
    GLint* arg3_nonconst = NULL;
    if(info[3]->IsString()) {
        Nan::Utf8String arg3_utf8(info[3]);
        arg3 = (GLint*)(*arg3_utf8);
    } else if(info[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(info[3]);
        arg3_nonconst = new GLint[arg3_array->Length()];
        arg3 = (GLint*)arg3_nonconst;
    } else if(info[3]->IsObject()) {
        arg3 = (GLint*)glbind_get_buffer_data(info[3]);
    } else if(info[3]->IsNumber()) {
        arg3 = (GLint*)(info[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glGetProgramStageiv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    return;
}
NAN_METHOD(EXPORT_patchParameteri) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLint arg1;
    arg1 = info[1]->Int32Value();
    glPatchParameteri(arg0, arg1);
    return;
}
NAN_METHOD(EXPORT_patchParameterfv) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    const GLfloat *arg1;
    GLfloat* arg1_nonconst = NULL;
    if(info[1]->IsString()) {
        Nan::Utf8String arg1_utf8(info[1]);
        arg1 = (const GLfloat*)(*arg1_utf8);
    } else if(info[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(info[1]);
        arg1_nonconst = new GLfloat[arg1_array->Length()];
        arg1 = (const GLfloat*)arg1_nonconst;
    } else if(info[1]->IsObject()) {
        arg1 = (const GLfloat*)glbind_get_buffer_data(info[1]);
    } else if(info[1]->IsNumber()) {
        arg1 = (const GLfloat*)(info[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glPatchParameterfv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    return;
}
NAN_METHOD(EXPORT_bindTransformFeedback) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLuint arg1;
    if(info[1]->IsNumber()) {
        arg1 = info[1]->IntegerValue();
    } else {
        arg1 = Nan::ObjectWrap::Unwrap<NODE_TransformFeedback>(info[1]->ToObject())->gl_handle;
    }
    glBindTransformFeedback(arg0, arg1);
    return;
}
NAN_METHOD(EXPORT_genTransformFeedbacks) {
    Nan::HandleScope scope;
    // None
    GLsizei arg0;
    arg0 = info[0]->Int32Value();
    GLuint* arg1;
    v8::Handle<v8::Array> info_1_array = v8::Handle<v8::Array>::Cast(info[1]);
    arg1 = new GLuint[info_1_array->Length()];
    glGenTransformFeedbacks(arg0, arg1);
    for(uint32_t i = 0; i < info_1_array->Length(); i++) {
        info_1_array->Set(i, NODE_TransformFeedback::fromGLHandle(arg1[i]));
    }
    delete [] arg1;
    return;
}
NAN_METHOD(EXPORT_isTransformFeedback) {
    Nan::HandleScope scope;
    // <gltypes.Type instance at 0x102dd84d0>
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_TransformFeedback>(info[0]->ToObject())->gl_handle;
    }
    GLboolean result;
    result = glIsTransformFeedback(arg0);
    v8::Handle<v8::Value> result_js = Nan::New<v8::Uint32>(result);
    return info.GetReturnValue().Set(result_js);
}
NAN_METHOD(EXPORT_pauseTransformFeedback) {
    Nan::HandleScope scope;
    // None
    glPauseTransformFeedback();
    return;
}
NAN_METHOD(EXPORT_resumeTransformFeedback) {
    Nan::HandleScope scope;
    // None
    glResumeTransformFeedback();
    return;
}
NAN_METHOD(EXPORT_drawTransformFeedback) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLuint arg1;
    if(info[1]->IsNumber()) {
        arg1 = info[1]->IntegerValue();
    } else {
        arg1 = Nan::ObjectWrap::Unwrap<NODE_TransformFeedback>(info[1]->ToObject())->gl_handle;
    }
    glDrawTransformFeedback(arg0, arg1);
    return;
}
NAN_METHOD(EXPORT_drawTransformFeedbackStream) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLuint arg1;
    arg1 = info[1]->Uint32Value();
    GLuint arg2;
    arg2 = info[2]->Uint32Value();
    glDrawTransformFeedbackStream(arg0, arg1, arg2);
    return;
}
NAN_METHOD(EXPORT_beginQueryIndexed) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLuint arg1;
    arg1 = info[1]->Uint32Value();
    GLuint arg2;
    arg2 = info[2]->Uint32Value();
    glBeginQueryIndexed(arg0, arg1, arg2);
    return;
}
NAN_METHOD(EXPORT_endQueryIndexed) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLuint arg1;
    arg1 = info[1]->Uint32Value();
    glEndQueryIndexed(arg0, arg1);
    return;
}
NAN_METHOD(EXPORT_getQueryIndexediv) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLuint arg1;
    arg1 = info[1]->Uint32Value();
    GLenum arg2;
    arg2 = info[2]->Uint32Value();
    GLint *arg3;
    GLint* arg3_nonconst = NULL;
    if(info[3]->IsString()) {
        Nan::Utf8String arg3_utf8(info[3]);
        arg3 = (GLint*)(*arg3_utf8);
    } else if(info[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(info[3]);
        arg3_nonconst = new GLint[arg3_array->Length()];
        arg3 = (GLint*)arg3_nonconst;
    } else if(info[3]->IsObject()) {
        arg3 = (GLint*)glbind_get_buffer_data(info[3]);
    } else if(info[3]->IsNumber()) {
        arg3 = (GLint*)(info[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glGetQueryIndexediv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    return;
}
NAN_METHOD(EXPORT_releaseShaderCompiler) {
    Nan::HandleScope scope;
    // None
    glReleaseShaderCompiler();
    return;
}
NAN_METHOD(EXPORT_shaderBinary) {
    Nan::HandleScope scope;
    // None
    GLsizei arg0;
    arg0 = info[0]->Int32Value();
    GLuint* arg1;
    v8::Handle<v8::Array> info_1_array = v8::Handle<v8::Array>::Cast(info[1]);
    arg1 = new GLuint[info_1_array->Length()];
    for(uint32_t i = 0; i < info_1_array->Length(); i++) {
        arg1[i] = Nan::ObjectWrap::Unwrap<NODE_Shader>(info_1_array->Get(i)->ToObject())->gl_handle;}
    GLenum arg2;
    arg2 = info[2]->Uint32Value();
    const GLvoid *arg3;
    GLubyte* arg3_nonconst = NULL;
    if(info[3]->IsString()) {
        Nan::Utf8String arg3_utf8(info[3]);
        arg3 = (const GLvoid*)(*arg3_utf8);
    } else if(info[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(info[3]);
        arg3_nonconst = new GLubyte[arg3_array->Length()];
        arg3 = (const GLvoid*)arg3_nonconst;
    } else if(info[3]->IsObject()) {
        arg3 = (const GLvoid*)glbind_get_buffer_data(info[3]);
    } else if(info[3]->IsNumber()) {
        arg3 = (const GLvoid*)(info[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    GLsizei arg4;
    arg4 = info[4]->Int32Value();
    glShaderBinary(arg0, arg1, arg2, arg3, arg4);
    delete [] arg1;
    if(arg3_nonconst) delete [] arg3_nonconst;
    return;
}
NAN_METHOD(EXPORT_getShaderPrecisionFormat) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    GLint *arg2;
    GLint* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (GLint*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLint[arg2_array->Length()];
        arg2 = (GLint*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (GLint*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (GLint*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    GLint *arg3;
    GLint* arg3_nonconst = NULL;
    if(info[3]->IsString()) {
        Nan::Utf8String arg3_utf8(info[3]);
        arg3 = (GLint*)(*arg3_utf8);
    } else if(info[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(info[3]);
        arg3_nonconst = new GLint[arg3_array->Length()];
        arg3 = (GLint*)arg3_nonconst;
    } else if(info[3]->IsObject()) {
        arg3 = (GLint*)glbind_get_buffer_data(info[3]);
    } else if(info[3]->IsNumber()) {
        arg3 = (GLint*)(info[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glGetShaderPrecisionFormat(arg0, arg1, arg2, arg3);
    if(arg2_nonconst) delete [] arg2_nonconst;
    if(arg3_nonconst) delete [] arg3_nonconst;
    return;
}
NAN_METHOD(EXPORT_depthRangef) {
    Nan::HandleScope scope;
    // None
    GLclampf arg0;
    arg0 = (float)info[0]->NumberValue();
    GLclampf arg1;
    arg1 = (float)info[1]->NumberValue();
    glDepthRangef(arg0, arg1);
    return;
}
NAN_METHOD(EXPORT_clearDepthf) {
    Nan::HandleScope scope;
    // None
    GLclampf arg0;
    arg0 = (float)info[0]->NumberValue();
    glClearDepthf(arg0);
    return;
}
NAN_METHOD(EXPORT_getProgramBinary) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLsizei arg1;
    arg1 = info[1]->Int32Value();
    GLsizei *arg2;
    GLsizei* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (GLsizei*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLsizei[arg2_array->Length()];
        arg2 = (GLsizei*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (GLsizei*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (GLsizei*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    GLenum *arg3;
    GLenum* arg3_nonconst = NULL;
    if(info[3]->IsString()) {
        Nan::Utf8String arg3_utf8(info[3]);
        arg3 = (GLenum*)(*arg3_utf8);
    } else if(info[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(info[3]);
        arg3_nonconst = new GLenum[arg3_array->Length()];
        arg3 = (GLenum*)arg3_nonconst;
    } else if(info[3]->IsObject()) {
        arg3 = (GLenum*)glbind_get_buffer_data(info[3]);
    } else if(info[3]->IsNumber()) {
        arg3 = (GLenum*)(info[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    GLvoid *arg4;
    GLubyte* arg4_nonconst = NULL;
    if(info[4]->IsString()) {
        Nan::Utf8String arg4_utf8(info[4]);
        arg4 = (GLvoid*)(*arg4_utf8);
    } else if(info[4]->IsArray()) {
        v8::Handle<v8::Array> arg4_array = v8::Handle<v8::Array>::Cast(info[4]);
        arg4_nonconst = new GLubyte[arg4_array->Length()];
        arg4 = (GLvoid*)arg4_nonconst;
    } else if(info[4]->IsObject()) {
        arg4 = (GLvoid*)glbind_get_buffer_data(info[4]);
    } else if(info[4]->IsNumber()) {
        arg4 = (GLvoid*)(info[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glGetProgramBinary(arg0, arg1, arg2, arg3, arg4);
    if(arg2_nonconst) delete [] arg2_nonconst;
    if(arg3_nonconst) delete [] arg3_nonconst;
    if(arg4_nonconst) delete [] arg4_nonconst;
    return;
}
NAN_METHOD(EXPORT_programBinary) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    const GLvoid *arg2;
    GLubyte* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (const GLvoid*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLubyte[arg2_array->Length()];
        arg2 = (const GLvoid*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (const GLvoid*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (const GLvoid*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    GLsizei arg3;
    arg3 = info[3]->Int32Value();
    glProgramBinary(arg0, arg1, arg2, arg3);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_programParameteri) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    GLint arg2;
    arg2 = info[2]->Int32Value();
    glProgramParameteri(arg0, arg1, arg2);
    return;
}
NAN_METHOD(EXPORT_useProgramStages) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_ProgramPipeline>(info[0]->ToObject())->gl_handle;
    }
    GLbitfield arg1;
    arg1 = info[1]->Uint32Value();
    GLuint arg2;
    if(info[2]->IsNumber()) {
        arg2 = info[2]->IntegerValue();
    } else {
        arg2 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[2]->ToObject())->gl_handle;
    }
    glUseProgramStages(arg0, arg1, arg2);
    return;
}
NAN_METHOD(EXPORT_activeShaderProgram) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_ProgramPipeline>(info[0]->ToObject())->gl_handle;
    }
    GLuint arg1;
    if(info[1]->IsNumber()) {
        arg1 = info[1]->IntegerValue();
    } else {
        arg1 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[1]->ToObject())->gl_handle;
    }
    glActiveShaderProgram(arg0, arg1);
    return;
}
NAN_METHOD(EXPORT_bindProgramPipeline) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_ProgramPipeline>(info[0]->ToObject())->gl_handle;
    }
    glBindProgramPipeline(arg0);
    return;
}
NAN_METHOD(EXPORT_genProgramPipelines) {
    Nan::HandleScope scope;
    // None
    GLsizei arg0;
    arg0 = info[0]->Int32Value();
    GLuint* arg1;
    v8::Handle<v8::Array> info_1_array = v8::Handle<v8::Array>::Cast(info[1]);
    arg1 = new GLuint[info_1_array->Length()];
    glGenProgramPipelines(arg0, arg1);
    for(uint32_t i = 0; i < info_1_array->Length(); i++) {
        info_1_array->Set(i, NODE_ProgramPipeline::fromGLHandle(arg1[i]));
    }
    delete [] arg1;
    return;
}
NAN_METHOD(EXPORT_isProgramPipeline) {
    Nan::HandleScope scope;
    // <gltypes.Type instance at 0x102dd84d0>
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_ProgramPipeline>(info[0]->ToObject())->gl_handle;
    }
    GLboolean result;
    result = glIsProgramPipeline(arg0);
    v8::Handle<v8::Value> result_js = Nan::New<v8::Uint32>(result);
    return info.GetReturnValue().Set(result_js);
}
NAN_METHOD(EXPORT_getProgramPipelineiv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_ProgramPipeline>(info[0]->ToObject())->gl_handle;
    }
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    GLint *arg2;
    GLint* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (GLint*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLint[arg2_array->Length()];
        arg2 = (GLint*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (GLint*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (GLint*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetProgramPipelineiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_programUniform1i) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLint arg2;
    arg2 = info[2]->Int32Value();
    glProgramUniform1i(arg0, arg1, arg2);
    return;
}
NAN_METHOD(EXPORT_programUniform1iv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLsizei arg2;
    arg2 = info[2]->Int32Value();
    const GLint *arg3;
    GLint* arg3_nonconst = NULL;
    if(info[3]->IsString()) {
        Nan::Utf8String arg3_utf8(info[3]);
        arg3 = (const GLint*)(*arg3_utf8);
    } else if(info[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(info[3]);
        arg3_nonconst = new GLint[arg3_array->Length()];
        arg3 = (const GLint*)arg3_nonconst;
    } else if(info[3]->IsObject()) {
        arg3 = (const GLint*)glbind_get_buffer_data(info[3]);
    } else if(info[3]->IsNumber()) {
        arg3 = (const GLint*)(info[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glProgramUniform1iv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    return;
}
NAN_METHOD(EXPORT_programUniform1f) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLfloat arg2;
    arg2 = (float)info[2]->NumberValue();
    glProgramUniform1f(arg0, arg1, arg2);
    return;
}
NAN_METHOD(EXPORT_programUniform1fv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLsizei arg2;
    arg2 = info[2]->Int32Value();
    const GLfloat *arg3;
    GLfloat* arg3_nonconst = NULL;
    if(info[3]->IsString()) {
        Nan::Utf8String arg3_utf8(info[3]);
        arg3 = (const GLfloat*)(*arg3_utf8);
    } else if(info[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(info[3]);
        arg3_nonconst = new GLfloat[arg3_array->Length()];
        arg3 = (const GLfloat*)arg3_nonconst;
    } else if(info[3]->IsObject()) {
        arg3 = (const GLfloat*)glbind_get_buffer_data(info[3]);
    } else if(info[3]->IsNumber()) {
        arg3 = (const GLfloat*)(info[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glProgramUniform1fv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    return;
}
NAN_METHOD(EXPORT_programUniform1d) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLdouble arg2;
    arg2 = info[2]->NumberValue();
    glProgramUniform1d(arg0, arg1, arg2);
    return;
}
NAN_METHOD(EXPORT_programUniform1dv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLsizei arg2;
    arg2 = info[2]->Int32Value();
    const GLdouble *arg3;
    GLdouble* arg3_nonconst = NULL;
    if(info[3]->IsString()) {
        Nan::Utf8String arg3_utf8(info[3]);
        arg3 = (const GLdouble*)(*arg3_utf8);
    } else if(info[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(info[3]);
        arg3_nonconst = new GLdouble[arg3_array->Length()];
        arg3 = (const GLdouble*)arg3_nonconst;
    } else if(info[3]->IsObject()) {
        arg3 = (const GLdouble*)glbind_get_buffer_data(info[3]);
    } else if(info[3]->IsNumber()) {
        arg3 = (const GLdouble*)(info[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glProgramUniform1dv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    return;
}
NAN_METHOD(EXPORT_programUniform1ui) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLuint arg2;
    arg2 = info[2]->Uint32Value();
    glProgramUniform1ui(arg0, arg1, arg2);
    return;
}
NAN_METHOD(EXPORT_programUniform1uiv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLsizei arg2;
    arg2 = info[2]->Int32Value();
    const GLuint *arg3;
    GLuint* arg3_nonconst = NULL;
    if(info[3]->IsString()) {
        Nan::Utf8String arg3_utf8(info[3]);
        arg3 = (const GLuint*)(*arg3_utf8);
    } else if(info[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(info[3]);
        arg3_nonconst = new GLuint[arg3_array->Length()];
        arg3 = (const GLuint*)arg3_nonconst;
    } else if(info[3]->IsObject()) {
        arg3 = (const GLuint*)glbind_get_buffer_data(info[3]);
    } else if(info[3]->IsNumber()) {
        arg3 = (const GLuint*)(info[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glProgramUniform1uiv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    return;
}
NAN_METHOD(EXPORT_programUniform2i) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLint arg2;
    arg2 = info[2]->Int32Value();
    GLint arg3;
    arg3 = info[3]->Int32Value();
    glProgramUniform2i(arg0, arg1, arg2, arg3);
    return;
}
NAN_METHOD(EXPORT_programUniform2iv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLsizei arg2;
    arg2 = info[2]->Int32Value();
    const GLint *arg3;
    GLint* arg3_nonconst = NULL;
    if(info[3]->IsString()) {
        Nan::Utf8String arg3_utf8(info[3]);
        arg3 = (const GLint*)(*arg3_utf8);
    } else if(info[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(info[3]);
        arg3_nonconst = new GLint[arg3_array->Length()];
        arg3 = (const GLint*)arg3_nonconst;
    } else if(info[3]->IsObject()) {
        arg3 = (const GLint*)glbind_get_buffer_data(info[3]);
    } else if(info[3]->IsNumber()) {
        arg3 = (const GLint*)(info[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glProgramUniform2iv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    return;
}
NAN_METHOD(EXPORT_programUniform2f) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLfloat arg2;
    arg2 = (float)info[2]->NumberValue();
    GLfloat arg3;
    arg3 = (float)info[3]->NumberValue();
    glProgramUniform2f(arg0, arg1, arg2, arg3);
    return;
}
NAN_METHOD(EXPORT_programUniform2fv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLsizei arg2;
    arg2 = info[2]->Int32Value();
    const GLfloat *arg3;
    GLfloat* arg3_nonconst = NULL;
    if(info[3]->IsString()) {
        Nan::Utf8String arg3_utf8(info[3]);
        arg3 = (const GLfloat*)(*arg3_utf8);
    } else if(info[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(info[3]);
        arg3_nonconst = new GLfloat[arg3_array->Length()];
        arg3 = (const GLfloat*)arg3_nonconst;
    } else if(info[3]->IsObject()) {
        arg3 = (const GLfloat*)glbind_get_buffer_data(info[3]);
    } else if(info[3]->IsNumber()) {
        arg3 = (const GLfloat*)(info[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glProgramUniform2fv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    return;
}
NAN_METHOD(EXPORT_programUniform2d) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLdouble arg2;
    arg2 = info[2]->NumberValue();
    GLdouble arg3;
    arg3 = info[3]->NumberValue();
    glProgramUniform2d(arg0, arg1, arg2, arg3);
    return;
}
NAN_METHOD(EXPORT_programUniform2dv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLsizei arg2;
    arg2 = info[2]->Int32Value();
    const GLdouble *arg3;
    GLdouble* arg3_nonconst = NULL;
    if(info[3]->IsString()) {
        Nan::Utf8String arg3_utf8(info[3]);
        arg3 = (const GLdouble*)(*arg3_utf8);
    } else if(info[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(info[3]);
        arg3_nonconst = new GLdouble[arg3_array->Length()];
        arg3 = (const GLdouble*)arg3_nonconst;
    } else if(info[3]->IsObject()) {
        arg3 = (const GLdouble*)glbind_get_buffer_data(info[3]);
    } else if(info[3]->IsNumber()) {
        arg3 = (const GLdouble*)(info[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glProgramUniform2dv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    return;
}
NAN_METHOD(EXPORT_programUniform2ui) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLuint arg2;
    arg2 = info[2]->Uint32Value();
    GLuint arg3;
    arg3 = info[3]->Uint32Value();
    glProgramUniform2ui(arg0, arg1, arg2, arg3);
    return;
}
NAN_METHOD(EXPORT_programUniform2uiv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLsizei arg2;
    arg2 = info[2]->Int32Value();
    const GLuint *arg3;
    GLuint* arg3_nonconst = NULL;
    if(info[3]->IsString()) {
        Nan::Utf8String arg3_utf8(info[3]);
        arg3 = (const GLuint*)(*arg3_utf8);
    } else if(info[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(info[3]);
        arg3_nonconst = new GLuint[arg3_array->Length()];
        arg3 = (const GLuint*)arg3_nonconst;
    } else if(info[3]->IsObject()) {
        arg3 = (const GLuint*)glbind_get_buffer_data(info[3]);
    } else if(info[3]->IsNumber()) {
        arg3 = (const GLuint*)(info[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glProgramUniform2uiv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    return;
}
NAN_METHOD(EXPORT_programUniform3i) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLint arg2;
    arg2 = info[2]->Int32Value();
    GLint arg3;
    arg3 = info[3]->Int32Value();
    GLint arg4;
    arg4 = info[4]->Int32Value();
    glProgramUniform3i(arg0, arg1, arg2, arg3, arg4);
    return;
}
NAN_METHOD(EXPORT_programUniform3iv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLsizei arg2;
    arg2 = info[2]->Int32Value();
    const GLint *arg3;
    GLint* arg3_nonconst = NULL;
    if(info[3]->IsString()) {
        Nan::Utf8String arg3_utf8(info[3]);
        arg3 = (const GLint*)(*arg3_utf8);
    } else if(info[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(info[3]);
        arg3_nonconst = new GLint[arg3_array->Length()];
        arg3 = (const GLint*)arg3_nonconst;
    } else if(info[3]->IsObject()) {
        arg3 = (const GLint*)glbind_get_buffer_data(info[3]);
    } else if(info[3]->IsNumber()) {
        arg3 = (const GLint*)(info[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glProgramUniform3iv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    return;
}
NAN_METHOD(EXPORT_programUniform3f) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLfloat arg2;
    arg2 = (float)info[2]->NumberValue();
    GLfloat arg3;
    arg3 = (float)info[3]->NumberValue();
    GLfloat arg4;
    arg4 = (float)info[4]->NumberValue();
    glProgramUniform3f(arg0, arg1, arg2, arg3, arg4);
    return;
}
NAN_METHOD(EXPORT_programUniform3fv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLsizei arg2;
    arg2 = info[2]->Int32Value();
    const GLfloat *arg3;
    GLfloat* arg3_nonconst = NULL;
    if(info[3]->IsString()) {
        Nan::Utf8String arg3_utf8(info[3]);
        arg3 = (const GLfloat*)(*arg3_utf8);
    } else if(info[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(info[3]);
        arg3_nonconst = new GLfloat[arg3_array->Length()];
        arg3 = (const GLfloat*)arg3_nonconst;
    } else if(info[3]->IsObject()) {
        arg3 = (const GLfloat*)glbind_get_buffer_data(info[3]);
    } else if(info[3]->IsNumber()) {
        arg3 = (const GLfloat*)(info[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glProgramUniform3fv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    return;
}
NAN_METHOD(EXPORT_programUniform3d) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLdouble arg2;
    arg2 = info[2]->NumberValue();
    GLdouble arg3;
    arg3 = info[3]->NumberValue();
    GLdouble arg4;
    arg4 = info[4]->NumberValue();
    glProgramUniform3d(arg0, arg1, arg2, arg3, arg4);
    return;
}
NAN_METHOD(EXPORT_programUniform3dv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLsizei arg2;
    arg2 = info[2]->Int32Value();
    const GLdouble *arg3;
    GLdouble* arg3_nonconst = NULL;
    if(info[3]->IsString()) {
        Nan::Utf8String arg3_utf8(info[3]);
        arg3 = (const GLdouble*)(*arg3_utf8);
    } else if(info[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(info[3]);
        arg3_nonconst = new GLdouble[arg3_array->Length()];
        arg3 = (const GLdouble*)arg3_nonconst;
    } else if(info[3]->IsObject()) {
        arg3 = (const GLdouble*)glbind_get_buffer_data(info[3]);
    } else if(info[3]->IsNumber()) {
        arg3 = (const GLdouble*)(info[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glProgramUniform3dv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    return;
}
NAN_METHOD(EXPORT_programUniform3ui) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLuint arg2;
    arg2 = info[2]->Uint32Value();
    GLuint arg3;
    arg3 = info[3]->Uint32Value();
    GLuint arg4;
    arg4 = info[4]->Uint32Value();
    glProgramUniform3ui(arg0, arg1, arg2, arg3, arg4);
    return;
}
NAN_METHOD(EXPORT_programUniform3uiv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLsizei arg2;
    arg2 = info[2]->Int32Value();
    const GLuint *arg3;
    GLuint* arg3_nonconst = NULL;
    if(info[3]->IsString()) {
        Nan::Utf8String arg3_utf8(info[3]);
        arg3 = (const GLuint*)(*arg3_utf8);
    } else if(info[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(info[3]);
        arg3_nonconst = new GLuint[arg3_array->Length()];
        arg3 = (const GLuint*)arg3_nonconst;
    } else if(info[3]->IsObject()) {
        arg3 = (const GLuint*)glbind_get_buffer_data(info[3]);
    } else if(info[3]->IsNumber()) {
        arg3 = (const GLuint*)(info[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glProgramUniform3uiv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    return;
}
NAN_METHOD(EXPORT_programUniform4i) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLint arg2;
    arg2 = info[2]->Int32Value();
    GLint arg3;
    arg3 = info[3]->Int32Value();
    GLint arg4;
    arg4 = info[4]->Int32Value();
    GLint arg5;
    arg5 = info[5]->Int32Value();
    glProgramUniform4i(arg0, arg1, arg2, arg3, arg4, arg5);
    return;
}
NAN_METHOD(EXPORT_programUniform4iv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLsizei arg2;
    arg2 = info[2]->Int32Value();
    const GLint *arg3;
    GLint* arg3_nonconst = NULL;
    if(info[3]->IsString()) {
        Nan::Utf8String arg3_utf8(info[3]);
        arg3 = (const GLint*)(*arg3_utf8);
    } else if(info[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(info[3]);
        arg3_nonconst = new GLint[arg3_array->Length()];
        arg3 = (const GLint*)arg3_nonconst;
    } else if(info[3]->IsObject()) {
        arg3 = (const GLint*)glbind_get_buffer_data(info[3]);
    } else if(info[3]->IsNumber()) {
        arg3 = (const GLint*)(info[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glProgramUniform4iv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    return;
}
NAN_METHOD(EXPORT_programUniform4f) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLfloat arg2;
    arg2 = (float)info[2]->NumberValue();
    GLfloat arg3;
    arg3 = (float)info[3]->NumberValue();
    GLfloat arg4;
    arg4 = (float)info[4]->NumberValue();
    GLfloat arg5;
    arg5 = (float)info[5]->NumberValue();
    glProgramUniform4f(arg0, arg1, arg2, arg3, arg4, arg5);
    return;
}
NAN_METHOD(EXPORT_programUniform4fv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLsizei arg2;
    arg2 = info[2]->Int32Value();
    const GLfloat *arg3;
    GLfloat* arg3_nonconst = NULL;
    if(info[3]->IsString()) {
        Nan::Utf8String arg3_utf8(info[3]);
        arg3 = (const GLfloat*)(*arg3_utf8);
    } else if(info[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(info[3]);
        arg3_nonconst = new GLfloat[arg3_array->Length()];
        arg3 = (const GLfloat*)arg3_nonconst;
    } else if(info[3]->IsObject()) {
        arg3 = (const GLfloat*)glbind_get_buffer_data(info[3]);
    } else if(info[3]->IsNumber()) {
        arg3 = (const GLfloat*)(info[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glProgramUniform4fv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    return;
}
NAN_METHOD(EXPORT_programUniform4d) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLdouble arg2;
    arg2 = info[2]->NumberValue();
    GLdouble arg3;
    arg3 = info[3]->NumberValue();
    GLdouble arg4;
    arg4 = info[4]->NumberValue();
    GLdouble arg5;
    arg5 = info[5]->NumberValue();
    glProgramUniform4d(arg0, arg1, arg2, arg3, arg4, arg5);
    return;
}
NAN_METHOD(EXPORT_programUniform4dv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLsizei arg2;
    arg2 = info[2]->Int32Value();
    const GLdouble *arg3;
    GLdouble* arg3_nonconst = NULL;
    if(info[3]->IsString()) {
        Nan::Utf8String arg3_utf8(info[3]);
        arg3 = (const GLdouble*)(*arg3_utf8);
    } else if(info[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(info[3]);
        arg3_nonconst = new GLdouble[arg3_array->Length()];
        arg3 = (const GLdouble*)arg3_nonconst;
    } else if(info[3]->IsObject()) {
        arg3 = (const GLdouble*)glbind_get_buffer_data(info[3]);
    } else if(info[3]->IsNumber()) {
        arg3 = (const GLdouble*)(info[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glProgramUniform4dv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    return;
}
NAN_METHOD(EXPORT_programUniform4ui) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLuint arg2;
    arg2 = info[2]->Uint32Value();
    GLuint arg3;
    arg3 = info[3]->Uint32Value();
    GLuint arg4;
    arg4 = info[4]->Uint32Value();
    GLuint arg5;
    arg5 = info[5]->Uint32Value();
    glProgramUniform4ui(arg0, arg1, arg2, arg3, arg4, arg5);
    return;
}
NAN_METHOD(EXPORT_programUniform4uiv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLsizei arg2;
    arg2 = info[2]->Int32Value();
    const GLuint *arg3;
    GLuint* arg3_nonconst = NULL;
    if(info[3]->IsString()) {
        Nan::Utf8String arg3_utf8(info[3]);
        arg3 = (const GLuint*)(*arg3_utf8);
    } else if(info[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(info[3]);
        arg3_nonconst = new GLuint[arg3_array->Length()];
        arg3 = (const GLuint*)arg3_nonconst;
    } else if(info[3]->IsObject()) {
        arg3 = (const GLuint*)glbind_get_buffer_data(info[3]);
    } else if(info[3]->IsNumber()) {
        arg3 = (const GLuint*)(info[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glProgramUniform4uiv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    return;
}
NAN_METHOD(EXPORT_programUniformMatrix2fv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLsizei arg2;
    arg2 = info[2]->Int32Value();
    GLboolean arg3;
    arg3 = info[3]->Uint32Value();
    const GLfloat *arg4;
    GLfloat* arg4_nonconst = NULL;
    if(info[4]->IsString()) {
        Nan::Utf8String arg4_utf8(info[4]);
        arg4 = (const GLfloat*)(*arg4_utf8);
    } else if(info[4]->IsArray()) {
        v8::Handle<v8::Array> arg4_array = v8::Handle<v8::Array>::Cast(info[4]);
        arg4_nonconst = new GLfloat[arg4_array->Length()];
        arg4 = (const GLfloat*)arg4_nonconst;
    } else if(info[4]->IsObject()) {
        arg4 = (const GLfloat*)glbind_get_buffer_data(info[4]);
    } else if(info[4]->IsNumber()) {
        arg4 = (const GLfloat*)(info[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glProgramUniformMatrix2fv(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    return;
}
NAN_METHOD(EXPORT_programUniformMatrix3fv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLsizei arg2;
    arg2 = info[2]->Int32Value();
    GLboolean arg3;
    arg3 = info[3]->Uint32Value();
    const GLfloat *arg4;
    GLfloat* arg4_nonconst = NULL;
    if(info[4]->IsString()) {
        Nan::Utf8String arg4_utf8(info[4]);
        arg4 = (const GLfloat*)(*arg4_utf8);
    } else if(info[4]->IsArray()) {
        v8::Handle<v8::Array> arg4_array = v8::Handle<v8::Array>::Cast(info[4]);
        arg4_nonconst = new GLfloat[arg4_array->Length()];
        arg4 = (const GLfloat*)arg4_nonconst;
    } else if(info[4]->IsObject()) {
        arg4 = (const GLfloat*)glbind_get_buffer_data(info[4]);
    } else if(info[4]->IsNumber()) {
        arg4 = (const GLfloat*)(info[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glProgramUniformMatrix3fv(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    return;
}
NAN_METHOD(EXPORT_programUniformMatrix4fv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLsizei arg2;
    arg2 = info[2]->Int32Value();
    GLboolean arg3;
    arg3 = info[3]->Uint32Value();
    const GLfloat *arg4;
    GLfloat* arg4_nonconst = NULL;
    if(info[4]->IsString()) {
        Nan::Utf8String arg4_utf8(info[4]);
        arg4 = (const GLfloat*)(*arg4_utf8);
    } else if(info[4]->IsArray()) {
        v8::Handle<v8::Array> arg4_array = v8::Handle<v8::Array>::Cast(info[4]);
        arg4_nonconst = new GLfloat[arg4_array->Length()];
        arg4 = (const GLfloat*)arg4_nonconst;
    } else if(info[4]->IsObject()) {
        arg4 = (const GLfloat*)glbind_get_buffer_data(info[4]);
    } else if(info[4]->IsNumber()) {
        arg4 = (const GLfloat*)(info[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glProgramUniformMatrix4fv(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    return;
}
NAN_METHOD(EXPORT_programUniformMatrix2dv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLsizei arg2;
    arg2 = info[2]->Int32Value();
    GLboolean arg3;
    arg3 = info[3]->Uint32Value();
    const GLdouble *arg4;
    GLdouble* arg4_nonconst = NULL;
    if(info[4]->IsString()) {
        Nan::Utf8String arg4_utf8(info[4]);
        arg4 = (const GLdouble*)(*arg4_utf8);
    } else if(info[4]->IsArray()) {
        v8::Handle<v8::Array> arg4_array = v8::Handle<v8::Array>::Cast(info[4]);
        arg4_nonconst = new GLdouble[arg4_array->Length()];
        arg4 = (const GLdouble*)arg4_nonconst;
    } else if(info[4]->IsObject()) {
        arg4 = (const GLdouble*)glbind_get_buffer_data(info[4]);
    } else if(info[4]->IsNumber()) {
        arg4 = (const GLdouble*)(info[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glProgramUniformMatrix2dv(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    return;
}
NAN_METHOD(EXPORT_programUniformMatrix3dv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLsizei arg2;
    arg2 = info[2]->Int32Value();
    GLboolean arg3;
    arg3 = info[3]->Uint32Value();
    const GLdouble *arg4;
    GLdouble* arg4_nonconst = NULL;
    if(info[4]->IsString()) {
        Nan::Utf8String arg4_utf8(info[4]);
        arg4 = (const GLdouble*)(*arg4_utf8);
    } else if(info[4]->IsArray()) {
        v8::Handle<v8::Array> arg4_array = v8::Handle<v8::Array>::Cast(info[4]);
        arg4_nonconst = new GLdouble[arg4_array->Length()];
        arg4 = (const GLdouble*)arg4_nonconst;
    } else if(info[4]->IsObject()) {
        arg4 = (const GLdouble*)glbind_get_buffer_data(info[4]);
    } else if(info[4]->IsNumber()) {
        arg4 = (const GLdouble*)(info[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glProgramUniformMatrix3dv(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    return;
}
NAN_METHOD(EXPORT_programUniformMatrix4dv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLsizei arg2;
    arg2 = info[2]->Int32Value();
    GLboolean arg3;
    arg3 = info[3]->Uint32Value();
    const GLdouble *arg4;
    GLdouble* arg4_nonconst = NULL;
    if(info[4]->IsString()) {
        Nan::Utf8String arg4_utf8(info[4]);
        arg4 = (const GLdouble*)(*arg4_utf8);
    } else if(info[4]->IsArray()) {
        v8::Handle<v8::Array> arg4_array = v8::Handle<v8::Array>::Cast(info[4]);
        arg4_nonconst = new GLdouble[arg4_array->Length()];
        arg4 = (const GLdouble*)arg4_nonconst;
    } else if(info[4]->IsObject()) {
        arg4 = (const GLdouble*)glbind_get_buffer_data(info[4]);
    } else if(info[4]->IsNumber()) {
        arg4 = (const GLdouble*)(info[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glProgramUniformMatrix4dv(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    return;
}
NAN_METHOD(EXPORT_programUniformMatrix2x3fv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLsizei arg2;
    arg2 = info[2]->Int32Value();
    GLboolean arg3;
    arg3 = info[3]->Uint32Value();
    const GLfloat *arg4;
    GLfloat* arg4_nonconst = NULL;
    if(info[4]->IsString()) {
        Nan::Utf8String arg4_utf8(info[4]);
        arg4 = (const GLfloat*)(*arg4_utf8);
    } else if(info[4]->IsArray()) {
        v8::Handle<v8::Array> arg4_array = v8::Handle<v8::Array>::Cast(info[4]);
        arg4_nonconst = new GLfloat[arg4_array->Length()];
        arg4 = (const GLfloat*)arg4_nonconst;
    } else if(info[4]->IsObject()) {
        arg4 = (const GLfloat*)glbind_get_buffer_data(info[4]);
    } else if(info[4]->IsNumber()) {
        arg4 = (const GLfloat*)(info[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glProgramUniformMatrix2x3fv(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    return;
}
NAN_METHOD(EXPORT_programUniformMatrix3x2fv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLsizei arg2;
    arg2 = info[2]->Int32Value();
    GLboolean arg3;
    arg3 = info[3]->Uint32Value();
    const GLfloat *arg4;
    GLfloat* arg4_nonconst = NULL;
    if(info[4]->IsString()) {
        Nan::Utf8String arg4_utf8(info[4]);
        arg4 = (const GLfloat*)(*arg4_utf8);
    } else if(info[4]->IsArray()) {
        v8::Handle<v8::Array> arg4_array = v8::Handle<v8::Array>::Cast(info[4]);
        arg4_nonconst = new GLfloat[arg4_array->Length()];
        arg4 = (const GLfloat*)arg4_nonconst;
    } else if(info[4]->IsObject()) {
        arg4 = (const GLfloat*)glbind_get_buffer_data(info[4]);
    } else if(info[4]->IsNumber()) {
        arg4 = (const GLfloat*)(info[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glProgramUniformMatrix3x2fv(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    return;
}
NAN_METHOD(EXPORT_programUniformMatrix2x4fv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLsizei arg2;
    arg2 = info[2]->Int32Value();
    GLboolean arg3;
    arg3 = info[3]->Uint32Value();
    const GLfloat *arg4;
    GLfloat* arg4_nonconst = NULL;
    if(info[4]->IsString()) {
        Nan::Utf8String arg4_utf8(info[4]);
        arg4 = (const GLfloat*)(*arg4_utf8);
    } else if(info[4]->IsArray()) {
        v8::Handle<v8::Array> arg4_array = v8::Handle<v8::Array>::Cast(info[4]);
        arg4_nonconst = new GLfloat[arg4_array->Length()];
        arg4 = (const GLfloat*)arg4_nonconst;
    } else if(info[4]->IsObject()) {
        arg4 = (const GLfloat*)glbind_get_buffer_data(info[4]);
    } else if(info[4]->IsNumber()) {
        arg4 = (const GLfloat*)(info[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glProgramUniformMatrix2x4fv(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    return;
}
NAN_METHOD(EXPORT_programUniformMatrix4x2fv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLsizei arg2;
    arg2 = info[2]->Int32Value();
    GLboolean arg3;
    arg3 = info[3]->Uint32Value();
    const GLfloat *arg4;
    GLfloat* arg4_nonconst = NULL;
    if(info[4]->IsString()) {
        Nan::Utf8String arg4_utf8(info[4]);
        arg4 = (const GLfloat*)(*arg4_utf8);
    } else if(info[4]->IsArray()) {
        v8::Handle<v8::Array> arg4_array = v8::Handle<v8::Array>::Cast(info[4]);
        arg4_nonconst = new GLfloat[arg4_array->Length()];
        arg4 = (const GLfloat*)arg4_nonconst;
    } else if(info[4]->IsObject()) {
        arg4 = (const GLfloat*)glbind_get_buffer_data(info[4]);
    } else if(info[4]->IsNumber()) {
        arg4 = (const GLfloat*)(info[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glProgramUniformMatrix4x2fv(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    return;
}
NAN_METHOD(EXPORT_programUniformMatrix3x4fv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLsizei arg2;
    arg2 = info[2]->Int32Value();
    GLboolean arg3;
    arg3 = info[3]->Uint32Value();
    const GLfloat *arg4;
    GLfloat* arg4_nonconst = NULL;
    if(info[4]->IsString()) {
        Nan::Utf8String arg4_utf8(info[4]);
        arg4 = (const GLfloat*)(*arg4_utf8);
    } else if(info[4]->IsArray()) {
        v8::Handle<v8::Array> arg4_array = v8::Handle<v8::Array>::Cast(info[4]);
        arg4_nonconst = new GLfloat[arg4_array->Length()];
        arg4 = (const GLfloat*)arg4_nonconst;
    } else if(info[4]->IsObject()) {
        arg4 = (const GLfloat*)glbind_get_buffer_data(info[4]);
    } else if(info[4]->IsNumber()) {
        arg4 = (const GLfloat*)(info[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glProgramUniformMatrix3x4fv(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    return;
}
NAN_METHOD(EXPORT_programUniformMatrix4x3fv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLsizei arg2;
    arg2 = info[2]->Int32Value();
    GLboolean arg3;
    arg3 = info[3]->Uint32Value();
    const GLfloat *arg4;
    GLfloat* arg4_nonconst = NULL;
    if(info[4]->IsString()) {
        Nan::Utf8String arg4_utf8(info[4]);
        arg4 = (const GLfloat*)(*arg4_utf8);
    } else if(info[4]->IsArray()) {
        v8::Handle<v8::Array> arg4_array = v8::Handle<v8::Array>::Cast(info[4]);
        arg4_nonconst = new GLfloat[arg4_array->Length()];
        arg4 = (const GLfloat*)arg4_nonconst;
    } else if(info[4]->IsObject()) {
        arg4 = (const GLfloat*)glbind_get_buffer_data(info[4]);
    } else if(info[4]->IsNumber()) {
        arg4 = (const GLfloat*)(info[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glProgramUniformMatrix4x3fv(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    return;
}
NAN_METHOD(EXPORT_programUniformMatrix2x3dv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLsizei arg2;
    arg2 = info[2]->Int32Value();
    GLboolean arg3;
    arg3 = info[3]->Uint32Value();
    const GLdouble *arg4;
    GLdouble* arg4_nonconst = NULL;
    if(info[4]->IsString()) {
        Nan::Utf8String arg4_utf8(info[4]);
        arg4 = (const GLdouble*)(*arg4_utf8);
    } else if(info[4]->IsArray()) {
        v8::Handle<v8::Array> arg4_array = v8::Handle<v8::Array>::Cast(info[4]);
        arg4_nonconst = new GLdouble[arg4_array->Length()];
        arg4 = (const GLdouble*)arg4_nonconst;
    } else if(info[4]->IsObject()) {
        arg4 = (const GLdouble*)glbind_get_buffer_data(info[4]);
    } else if(info[4]->IsNumber()) {
        arg4 = (const GLdouble*)(info[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glProgramUniformMatrix2x3dv(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    return;
}
NAN_METHOD(EXPORT_programUniformMatrix3x2dv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLsizei arg2;
    arg2 = info[2]->Int32Value();
    GLboolean arg3;
    arg3 = info[3]->Uint32Value();
    const GLdouble *arg4;
    GLdouble* arg4_nonconst = NULL;
    if(info[4]->IsString()) {
        Nan::Utf8String arg4_utf8(info[4]);
        arg4 = (const GLdouble*)(*arg4_utf8);
    } else if(info[4]->IsArray()) {
        v8::Handle<v8::Array> arg4_array = v8::Handle<v8::Array>::Cast(info[4]);
        arg4_nonconst = new GLdouble[arg4_array->Length()];
        arg4 = (const GLdouble*)arg4_nonconst;
    } else if(info[4]->IsObject()) {
        arg4 = (const GLdouble*)glbind_get_buffer_data(info[4]);
    } else if(info[4]->IsNumber()) {
        arg4 = (const GLdouble*)(info[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glProgramUniformMatrix3x2dv(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    return;
}
NAN_METHOD(EXPORT_programUniformMatrix2x4dv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLsizei arg2;
    arg2 = info[2]->Int32Value();
    GLboolean arg3;
    arg3 = info[3]->Uint32Value();
    const GLdouble *arg4;
    GLdouble* arg4_nonconst = NULL;
    if(info[4]->IsString()) {
        Nan::Utf8String arg4_utf8(info[4]);
        arg4 = (const GLdouble*)(*arg4_utf8);
    } else if(info[4]->IsArray()) {
        v8::Handle<v8::Array> arg4_array = v8::Handle<v8::Array>::Cast(info[4]);
        arg4_nonconst = new GLdouble[arg4_array->Length()];
        arg4 = (const GLdouble*)arg4_nonconst;
    } else if(info[4]->IsObject()) {
        arg4 = (const GLdouble*)glbind_get_buffer_data(info[4]);
    } else if(info[4]->IsNumber()) {
        arg4 = (const GLdouble*)(info[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glProgramUniformMatrix2x4dv(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    return;
}
NAN_METHOD(EXPORT_programUniformMatrix4x2dv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLsizei arg2;
    arg2 = info[2]->Int32Value();
    GLboolean arg3;
    arg3 = info[3]->Uint32Value();
    const GLdouble *arg4;
    GLdouble* arg4_nonconst = NULL;
    if(info[4]->IsString()) {
        Nan::Utf8String arg4_utf8(info[4]);
        arg4 = (const GLdouble*)(*arg4_utf8);
    } else if(info[4]->IsArray()) {
        v8::Handle<v8::Array> arg4_array = v8::Handle<v8::Array>::Cast(info[4]);
        arg4_nonconst = new GLdouble[arg4_array->Length()];
        arg4 = (const GLdouble*)arg4_nonconst;
    } else if(info[4]->IsObject()) {
        arg4 = (const GLdouble*)glbind_get_buffer_data(info[4]);
    } else if(info[4]->IsNumber()) {
        arg4 = (const GLdouble*)(info[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glProgramUniformMatrix4x2dv(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    return;
}
NAN_METHOD(EXPORT_programUniformMatrix3x4dv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLsizei arg2;
    arg2 = info[2]->Int32Value();
    GLboolean arg3;
    arg3 = info[3]->Uint32Value();
    const GLdouble *arg4;
    GLdouble* arg4_nonconst = NULL;
    if(info[4]->IsString()) {
        Nan::Utf8String arg4_utf8(info[4]);
        arg4 = (const GLdouble*)(*arg4_utf8);
    } else if(info[4]->IsArray()) {
        v8::Handle<v8::Array> arg4_array = v8::Handle<v8::Array>::Cast(info[4]);
        arg4_nonconst = new GLdouble[arg4_array->Length()];
        arg4 = (const GLdouble*)arg4_nonconst;
    } else if(info[4]->IsObject()) {
        arg4 = (const GLdouble*)glbind_get_buffer_data(info[4]);
    } else if(info[4]->IsNumber()) {
        arg4 = (const GLdouble*)(info[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glProgramUniformMatrix3x4dv(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    return;
}
NAN_METHOD(EXPORT_programUniformMatrix4x3dv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_Program>(info[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLsizei arg2;
    arg2 = info[2]->Int32Value();
    GLboolean arg3;
    arg3 = info[3]->Uint32Value();
    const GLdouble *arg4;
    GLdouble* arg4_nonconst = NULL;
    if(info[4]->IsString()) {
        Nan::Utf8String arg4_utf8(info[4]);
        arg4 = (const GLdouble*)(*arg4_utf8);
    } else if(info[4]->IsArray()) {
        v8::Handle<v8::Array> arg4_array = v8::Handle<v8::Array>::Cast(info[4]);
        arg4_nonconst = new GLdouble[arg4_array->Length()];
        arg4 = (const GLdouble*)arg4_nonconst;
    } else if(info[4]->IsObject()) {
        arg4 = (const GLdouble*)glbind_get_buffer_data(info[4]);
    } else if(info[4]->IsNumber()) {
        arg4 = (const GLdouble*)(info[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glProgramUniformMatrix4x3dv(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    return;
}
NAN_METHOD(EXPORT_validateProgramPipeline) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_ProgramPipeline>(info[0]->ToObject())->gl_handle;
    }
    glValidateProgramPipeline(arg0);
    return;
}
NAN_METHOD(EXPORT_getProgramPipelineInfoLog) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    if(info[0]->IsNumber()) {
        arg0 = info[0]->IntegerValue();
    } else {
        arg0 = Nan::ObjectWrap::Unwrap<NODE_ProgramPipeline>(info[0]->ToObject())->gl_handle;
    }
    GLsizei arg1;
    arg1 = info[1]->Int32Value();
    GLsizei *arg2;
    GLsizei* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (GLsizei*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLsizei[arg2_array->Length()];
        arg2 = (GLsizei*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (GLsizei*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (GLsizei*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    GLchar *arg3;
    GLchar* arg3_nonconst = NULL;
    if(info[3]->IsString()) {
        Nan::Utf8String arg3_utf8(info[3]);
        arg3 = (GLchar*)(*arg3_utf8);
    } else if(info[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(info[3]);
        arg3_nonconst = new GLchar[arg3_array->Length()];
        arg3 = (GLchar*)arg3_nonconst;
    } else if(info[3]->IsObject()) {
        arg3 = (GLchar*)glbind_get_buffer_data(info[3]);
    } else if(info[3]->IsNumber()) {
        arg3 = (GLchar*)(info[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glGetProgramPipelineInfoLog(arg0, arg1, arg2, arg3);
    if(arg2_nonconst) delete [] arg2_nonconst;
    if(arg3_nonconst) delete [] arg3_nonconst;
    return;
}
NAN_METHOD(EXPORT_vertexAttribL1d) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    GLdouble arg1;
    arg1 = info[1]->NumberValue();
    glVertexAttribL1d(arg0, arg1);
    return;
}
NAN_METHOD(EXPORT_vertexAttribL2d) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    GLdouble arg1;
    arg1 = info[1]->NumberValue();
    GLdouble arg2;
    arg2 = info[2]->NumberValue();
    glVertexAttribL2d(arg0, arg1, arg2);
    return;
}
NAN_METHOD(EXPORT_vertexAttribL3d) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    GLdouble arg1;
    arg1 = info[1]->NumberValue();
    GLdouble arg2;
    arg2 = info[2]->NumberValue();
    GLdouble arg3;
    arg3 = info[3]->NumberValue();
    glVertexAttribL3d(arg0, arg1, arg2, arg3);
    return;
}
NAN_METHOD(EXPORT_vertexAttribL4d) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    GLdouble arg1;
    arg1 = info[1]->NumberValue();
    GLdouble arg2;
    arg2 = info[2]->NumberValue();
    GLdouble arg3;
    arg3 = info[3]->NumberValue();
    GLdouble arg4;
    arg4 = info[4]->NumberValue();
    glVertexAttribL4d(arg0, arg1, arg2, arg3, arg4);
    return;
}
NAN_METHOD(EXPORT_vertexAttribL1dv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    const GLdouble *arg1;
    GLdouble* arg1_nonconst = NULL;
    if(info[1]->IsString()) {
        Nan::Utf8String arg1_utf8(info[1]);
        arg1 = (const GLdouble*)(*arg1_utf8);
    } else if(info[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(info[1]);
        arg1_nonconst = new GLdouble[arg1_array->Length()];
        arg1 = (const GLdouble*)arg1_nonconst;
    } else if(info[1]->IsObject()) {
        arg1 = (const GLdouble*)glbind_get_buffer_data(info[1]);
    } else if(info[1]->IsNumber()) {
        arg1 = (const GLdouble*)(info[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttribL1dv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    return;
}
NAN_METHOD(EXPORT_vertexAttribL2dv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    const GLdouble *arg1;
    GLdouble* arg1_nonconst = NULL;
    if(info[1]->IsString()) {
        Nan::Utf8String arg1_utf8(info[1]);
        arg1 = (const GLdouble*)(*arg1_utf8);
    } else if(info[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(info[1]);
        arg1_nonconst = new GLdouble[arg1_array->Length()];
        arg1 = (const GLdouble*)arg1_nonconst;
    } else if(info[1]->IsObject()) {
        arg1 = (const GLdouble*)glbind_get_buffer_data(info[1]);
    } else if(info[1]->IsNumber()) {
        arg1 = (const GLdouble*)(info[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttribL2dv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    return;
}
NAN_METHOD(EXPORT_vertexAttribL3dv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    const GLdouble *arg1;
    GLdouble* arg1_nonconst = NULL;
    if(info[1]->IsString()) {
        Nan::Utf8String arg1_utf8(info[1]);
        arg1 = (const GLdouble*)(*arg1_utf8);
    } else if(info[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(info[1]);
        arg1_nonconst = new GLdouble[arg1_array->Length()];
        arg1 = (const GLdouble*)arg1_nonconst;
    } else if(info[1]->IsObject()) {
        arg1 = (const GLdouble*)glbind_get_buffer_data(info[1]);
    } else if(info[1]->IsNumber()) {
        arg1 = (const GLdouble*)(info[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttribL3dv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    return;
}
NAN_METHOD(EXPORT_vertexAttribL4dv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    const GLdouble *arg1;
    GLdouble* arg1_nonconst = NULL;
    if(info[1]->IsString()) {
        Nan::Utf8String arg1_utf8(info[1]);
        arg1 = (const GLdouble*)(*arg1_utf8);
    } else if(info[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(info[1]);
        arg1_nonconst = new GLdouble[arg1_array->Length()];
        arg1 = (const GLdouble*)arg1_nonconst;
    } else if(info[1]->IsObject()) {
        arg1 = (const GLdouble*)glbind_get_buffer_data(info[1]);
    } else if(info[1]->IsNumber()) {
        arg1 = (const GLdouble*)(info[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttribL4dv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    return;
}
NAN_METHOD(EXPORT_vertexAttribLPointer) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLenum arg2;
    arg2 = info[2]->Uint32Value();
    GLsizei arg3;
    arg3 = info[3]->Int32Value();
    const GLvoid *arg4;
    GLubyte* arg4_nonconst = NULL;
    if(info[4]->IsString()) {
        Nan::Utf8String arg4_utf8(info[4]);
        arg4 = (const GLvoid*)(*arg4_utf8);
    } else if(info[4]->IsArray()) {
        v8::Handle<v8::Array> arg4_array = v8::Handle<v8::Array>::Cast(info[4]);
        arg4_nonconst = new GLubyte[arg4_array->Length()];
        arg4 = (const GLvoid*)arg4_nonconst;
    } else if(info[4]->IsObject()) {
        arg4 = (const GLvoid*)glbind_get_buffer_data(info[4]);
    } else if(info[4]->IsNumber()) {
        arg4 = (const GLvoid*)(info[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glVertexAttribLPointer(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    return;
}
NAN_METHOD(EXPORT_getVertexAttribLdv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    GLenum arg1;
    arg1 = info[1]->Uint32Value();
    GLdouble *arg2;
    GLdouble* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (GLdouble*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLdouble[arg2_array->Length()];
        arg2 = (GLdouble*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (GLdouble*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (GLdouble*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetVertexAttribLdv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_viewportArrayv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    GLsizei arg1;
    arg1 = info[1]->Int32Value();
    const GLfloat *arg2;
    GLfloat* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (const GLfloat*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLfloat[arg2_array->Length()];
        arg2 = (const GLfloat*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (const GLfloat*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (const GLfloat*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glViewportArrayv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_viewportIndexedf) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    GLfloat arg1;
    arg1 = (float)info[1]->NumberValue();
    GLfloat arg2;
    arg2 = (float)info[2]->NumberValue();
    GLfloat arg3;
    arg3 = (float)info[3]->NumberValue();
    GLfloat arg4;
    arg4 = (float)info[4]->NumberValue();
    glViewportIndexedf(arg0, arg1, arg2, arg3, arg4);
    return;
}
NAN_METHOD(EXPORT_viewportIndexedfv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    const GLfloat *arg1;
    GLfloat* arg1_nonconst = NULL;
    if(info[1]->IsString()) {
        Nan::Utf8String arg1_utf8(info[1]);
        arg1 = (const GLfloat*)(*arg1_utf8);
    } else if(info[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(info[1]);
        arg1_nonconst = new GLfloat[arg1_array->Length()];
        arg1 = (const GLfloat*)arg1_nonconst;
    } else if(info[1]->IsObject()) {
        arg1 = (const GLfloat*)glbind_get_buffer_data(info[1]);
    } else if(info[1]->IsNumber()) {
        arg1 = (const GLfloat*)(info[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glViewportIndexedfv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    return;
}
NAN_METHOD(EXPORT_scissorArrayv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    GLsizei arg1;
    arg1 = info[1]->Int32Value();
    const GLint *arg2;
    GLint* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (const GLint*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLint[arg2_array->Length()];
        arg2 = (const GLint*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (const GLint*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (const GLint*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glScissorArrayv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_scissorIndexed) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    GLint arg1;
    arg1 = info[1]->Int32Value();
    GLint arg2;
    arg2 = info[2]->Int32Value();
    GLsizei arg3;
    arg3 = info[3]->Int32Value();
    GLsizei arg4;
    arg4 = info[4]->Int32Value();
    glScissorIndexed(arg0, arg1, arg2, arg3, arg4);
    return;
}
NAN_METHOD(EXPORT_scissorIndexedv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    const GLint *arg1;
    GLint* arg1_nonconst = NULL;
    if(info[1]->IsString()) {
        Nan::Utf8String arg1_utf8(info[1]);
        arg1 = (const GLint*)(*arg1_utf8);
    } else if(info[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(info[1]);
        arg1_nonconst = new GLint[arg1_array->Length()];
        arg1 = (const GLint*)arg1_nonconst;
    } else if(info[1]->IsObject()) {
        arg1 = (const GLint*)glbind_get_buffer_data(info[1]);
    } else if(info[1]->IsNumber()) {
        arg1 = (const GLint*)(info[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glScissorIndexedv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    return;
}
NAN_METHOD(EXPORT_depthRangeArrayv) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    GLsizei arg1;
    arg1 = info[1]->Int32Value();
    const GLclampd *arg2;
    GLclampd* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (const GLclampd*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLclampd[arg2_array->Length()];
        arg2 = (const GLclampd*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (const GLclampd*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (const GLclampd*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glDepthRangeArrayv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_depthRangeIndexed) {
    Nan::HandleScope scope;
    // None
    GLuint arg0;
    arg0 = info[0]->Uint32Value();
    GLclampd arg1;
    arg1 = info[1]->NumberValue();
    GLclampd arg2;
    arg2 = info[2]->NumberValue();
    glDepthRangeIndexed(arg0, arg1, arg2);
    return;
}
NAN_METHOD(EXPORT_getFloati_v) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLuint arg1;
    arg1 = info[1]->Uint32Value();
    GLfloat *arg2;
    GLfloat* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (GLfloat*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLfloat[arg2_array->Length()];
        arg2 = (GLfloat*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (GLfloat*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (GLfloat*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetFloati_v(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_getDoublei_v) {
    Nan::HandleScope scope;
    // None
    GLenum arg0;
    arg0 = info[0]->Uint32Value();
    GLuint arg1;
    arg1 = info[1]->Uint32Value();
    GLdouble *arg2;
    GLdouble* arg2_nonconst = NULL;
    if(info[2]->IsString()) {
        Nan::Utf8String arg2_utf8(info[2]);
        arg2 = (GLdouble*)(*arg2_utf8);
    } else if(info[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(info[2]);
        arg2_nonconst = new GLdouble[arg2_array->Length()];
        arg2 = (GLdouble*)arg2_nonconst;
    } else if(info[2]->IsObject()) {
        arg2 = (GLdouble*)glbind_get_buffer_data(info[2]);
    } else if(info[2]->IsNumber()) {
        arg2 = (GLdouble*)(info[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetDoublei_v(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    return;
}
NAN_METHOD(EXPORT_shaderSource) {

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

}
void defineFunctions(v8::Handle<v8::ObjectTemplate> exports) {
    Nan::SetTemplate(exports, "cullFace", Nan::New<v8::FunctionTemplate>(EXPORT_cullFace));
    Nan::SetTemplate(exports, "frontFace", Nan::New<v8::FunctionTemplate>(EXPORT_frontFace));
    Nan::SetTemplate(exports, "hint", Nan::New<v8::FunctionTemplate>(EXPORT_hint));
    Nan::SetTemplate(exports, "lineWidth", Nan::New<v8::FunctionTemplate>(EXPORT_lineWidth));
    Nan::SetTemplate(exports, "pointSize", Nan::New<v8::FunctionTemplate>(EXPORT_pointSize));
    Nan::SetTemplate(exports, "polygonMode", Nan::New<v8::FunctionTemplate>(EXPORT_polygonMode));
    Nan::SetTemplate(exports, "scissor", Nan::New<v8::FunctionTemplate>(EXPORT_scissor));
    Nan::SetTemplate(exports, "texParameterf", Nan::New<v8::FunctionTemplate>(EXPORT_texParameterf));
    Nan::SetTemplate(exports, "texParameterfv", Nan::New<v8::FunctionTemplate>(EXPORT_texParameterfv));
    Nan::SetTemplate(exports, "texParameteri", Nan::New<v8::FunctionTemplate>(EXPORT_texParameteri));
    Nan::SetTemplate(exports, "texParameteriv", Nan::New<v8::FunctionTemplate>(EXPORT_texParameteriv));
    Nan::SetTemplate(exports, "texImage1D", Nan::New<v8::FunctionTemplate>(EXPORT_texImage1D));
    Nan::SetTemplate(exports, "texImage2D", Nan::New<v8::FunctionTemplate>(EXPORT_texImage2D));
    Nan::SetTemplate(exports, "drawBuffer", Nan::New<v8::FunctionTemplate>(EXPORT_drawBuffer));
    Nan::SetTemplate(exports, "clear", Nan::New<v8::FunctionTemplate>(EXPORT_clear));
    Nan::SetTemplate(exports, "clearColor", Nan::New<v8::FunctionTemplate>(EXPORT_clearColor));
    Nan::SetTemplate(exports, "clearStencil", Nan::New<v8::FunctionTemplate>(EXPORT_clearStencil));
    Nan::SetTemplate(exports, "clearDepth", Nan::New<v8::FunctionTemplate>(EXPORT_clearDepth));
    Nan::SetTemplate(exports, "stencilMask", Nan::New<v8::FunctionTemplate>(EXPORT_stencilMask));
    Nan::SetTemplate(exports, "colorMask", Nan::New<v8::FunctionTemplate>(EXPORT_colorMask));
    Nan::SetTemplate(exports, "depthMask", Nan::New<v8::FunctionTemplate>(EXPORT_depthMask));
    Nan::SetTemplate(exports, "disable", Nan::New<v8::FunctionTemplate>(EXPORT_disable));
    Nan::SetTemplate(exports, "enable", Nan::New<v8::FunctionTemplate>(EXPORT_enable));
    Nan::SetTemplate(exports, "finish", Nan::New<v8::FunctionTemplate>(EXPORT_finish));
    Nan::SetTemplate(exports, "flush", Nan::New<v8::FunctionTemplate>(EXPORT_flush));
    Nan::SetTemplate(exports, "blendFunc", Nan::New<v8::FunctionTemplate>(EXPORT_blendFunc));
    Nan::SetTemplate(exports, "logicOp", Nan::New<v8::FunctionTemplate>(EXPORT_logicOp));
    Nan::SetTemplate(exports, "stencilFunc", Nan::New<v8::FunctionTemplate>(EXPORT_stencilFunc));
    Nan::SetTemplate(exports, "stencilOp", Nan::New<v8::FunctionTemplate>(EXPORT_stencilOp));
    Nan::SetTemplate(exports, "depthFunc", Nan::New<v8::FunctionTemplate>(EXPORT_depthFunc));
    Nan::SetTemplate(exports, "pixelStoref", Nan::New<v8::FunctionTemplate>(EXPORT_pixelStoref));
    Nan::SetTemplate(exports, "pixelStorei", Nan::New<v8::FunctionTemplate>(EXPORT_pixelStorei));
    Nan::SetTemplate(exports, "readBuffer", Nan::New<v8::FunctionTemplate>(EXPORT_readBuffer));
    Nan::SetTemplate(exports, "readPixels", Nan::New<v8::FunctionTemplate>(EXPORT_readPixels));
    Nan::SetTemplate(exports, "getBooleanv", Nan::New<v8::FunctionTemplate>(EXPORT_getBooleanv));
    Nan::SetTemplate(exports, "getDoublev", Nan::New<v8::FunctionTemplate>(EXPORT_getDoublev));
    Nan::SetTemplate(exports, "getError", Nan::New<v8::FunctionTemplate>(EXPORT_getError));
    Nan::SetTemplate(exports, "getFloatv", Nan::New<v8::FunctionTemplate>(EXPORT_getFloatv));
    Nan::SetTemplate(exports, "getIntegerv", Nan::New<v8::FunctionTemplate>(EXPORT_getIntegerv));
    Nan::SetTemplate(exports, "getString", Nan::New<v8::FunctionTemplate>(EXPORT_getString));
    Nan::SetTemplate(exports, "getTexImage", Nan::New<v8::FunctionTemplate>(EXPORT_getTexImage));
    Nan::SetTemplate(exports, "getTexParameterfv", Nan::New<v8::FunctionTemplate>(EXPORT_getTexParameterfv));
    Nan::SetTemplate(exports, "getTexParameteriv", Nan::New<v8::FunctionTemplate>(EXPORT_getTexParameteriv));
    Nan::SetTemplate(exports, "getTexLevelParameterfv", Nan::New<v8::FunctionTemplate>(EXPORT_getTexLevelParameterfv));
    Nan::SetTemplate(exports, "getTexLevelParameteriv", Nan::New<v8::FunctionTemplate>(EXPORT_getTexLevelParameteriv));
    Nan::SetTemplate(exports, "isEnabled", Nan::New<v8::FunctionTemplate>(EXPORT_isEnabled));
    Nan::SetTemplate(exports, "depthRange", Nan::New<v8::FunctionTemplate>(EXPORT_depthRange));
    Nan::SetTemplate(exports, "viewport", Nan::New<v8::FunctionTemplate>(EXPORT_viewport));
    Nan::SetTemplate(exports, "drawArrays", Nan::New<v8::FunctionTemplate>(EXPORT_drawArrays));
    Nan::SetTemplate(exports, "drawElements", Nan::New<v8::FunctionTemplate>(EXPORT_drawElements));
    Nan::SetTemplate(exports, "polygonOffset", Nan::New<v8::FunctionTemplate>(EXPORT_polygonOffset));
    Nan::SetTemplate(exports, "copyTexImage1D", Nan::New<v8::FunctionTemplate>(EXPORT_copyTexImage1D));
    Nan::SetTemplate(exports, "copyTexImage2D", Nan::New<v8::FunctionTemplate>(EXPORT_copyTexImage2D));
    Nan::SetTemplate(exports, "copyTexSubImage1D", Nan::New<v8::FunctionTemplate>(EXPORT_copyTexSubImage1D));
    Nan::SetTemplate(exports, "copyTexSubImage2D", Nan::New<v8::FunctionTemplate>(EXPORT_copyTexSubImage2D));
    Nan::SetTemplate(exports, "texSubImage1D", Nan::New<v8::FunctionTemplate>(EXPORT_texSubImage1D));
    Nan::SetTemplate(exports, "texSubImage2D", Nan::New<v8::FunctionTemplate>(EXPORT_texSubImage2D));
    Nan::SetTemplate(exports, "bindTexture", Nan::New<v8::FunctionTemplate>(EXPORT_bindTexture));
    Nan::SetTemplate(exports, "genTextures", Nan::New<v8::FunctionTemplate>(EXPORT_genTextures));
    Nan::SetTemplate(exports, "isTexture", Nan::New<v8::FunctionTemplate>(EXPORT_isTexture));
    Nan::SetTemplate(exports, "blendColor", Nan::New<v8::FunctionTemplate>(EXPORT_blendColor));
    Nan::SetTemplate(exports, "blendEquation", Nan::New<v8::FunctionTemplate>(EXPORT_blendEquation));
    Nan::SetTemplate(exports, "drawRangeElements", Nan::New<v8::FunctionTemplate>(EXPORT_drawRangeElements));
    Nan::SetTemplate(exports, "texImage3D", Nan::New<v8::FunctionTemplate>(EXPORT_texImage3D));
    Nan::SetTemplate(exports, "texSubImage3D", Nan::New<v8::FunctionTemplate>(EXPORT_texSubImage3D));
    Nan::SetTemplate(exports, "copyTexSubImage3D", Nan::New<v8::FunctionTemplate>(EXPORT_copyTexSubImage3D));
    Nan::SetTemplate(exports, "activeTexture", Nan::New<v8::FunctionTemplate>(EXPORT_activeTexture));
    Nan::SetTemplate(exports, "sampleCoverage", Nan::New<v8::FunctionTemplate>(EXPORT_sampleCoverage));
    Nan::SetTemplate(exports, "compressedTexImage3D", Nan::New<v8::FunctionTemplate>(EXPORT_compressedTexImage3D));
    Nan::SetTemplate(exports, "compressedTexImage2D", Nan::New<v8::FunctionTemplate>(EXPORT_compressedTexImage2D));
    Nan::SetTemplate(exports, "compressedTexImage1D", Nan::New<v8::FunctionTemplate>(EXPORT_compressedTexImage1D));
    Nan::SetTemplate(exports, "compressedTexSubImage3D", Nan::New<v8::FunctionTemplate>(EXPORT_compressedTexSubImage3D));
    Nan::SetTemplate(exports, "compressedTexSubImage2D", Nan::New<v8::FunctionTemplate>(EXPORT_compressedTexSubImage2D));
    Nan::SetTemplate(exports, "compressedTexSubImage1D", Nan::New<v8::FunctionTemplate>(EXPORT_compressedTexSubImage1D));
    Nan::SetTemplate(exports, "getCompressedTexImage", Nan::New<v8::FunctionTemplate>(EXPORT_getCompressedTexImage));
    Nan::SetTemplate(exports, "blendFuncSeparate", Nan::New<v8::FunctionTemplate>(EXPORT_blendFuncSeparate));
    Nan::SetTemplate(exports, "multiDrawArrays", Nan::New<v8::FunctionTemplate>(EXPORT_multiDrawArrays));
    Nan::SetTemplate(exports, "pointParameterf", Nan::New<v8::FunctionTemplate>(EXPORT_pointParameterf));
    Nan::SetTemplate(exports, "pointParameterfv", Nan::New<v8::FunctionTemplate>(EXPORT_pointParameterfv));
    Nan::SetTemplate(exports, "pointParameteri", Nan::New<v8::FunctionTemplate>(EXPORT_pointParameteri));
    Nan::SetTemplate(exports, "pointParameteriv", Nan::New<v8::FunctionTemplate>(EXPORT_pointParameteriv));
    Nan::SetTemplate(exports, "genQueries", Nan::New<v8::FunctionTemplate>(EXPORT_genQueries));
    Nan::SetTemplate(exports, "isQuery", Nan::New<v8::FunctionTemplate>(EXPORT_isQuery));
    Nan::SetTemplate(exports, "beginQuery", Nan::New<v8::FunctionTemplate>(EXPORT_beginQuery));
    Nan::SetTemplate(exports, "endQuery", Nan::New<v8::FunctionTemplate>(EXPORT_endQuery));
    Nan::SetTemplate(exports, "getQueryiv", Nan::New<v8::FunctionTemplate>(EXPORT_getQueryiv));
    Nan::SetTemplate(exports, "getQueryObjectiv", Nan::New<v8::FunctionTemplate>(EXPORT_getQueryObjectiv));
    Nan::SetTemplate(exports, "getQueryObjectuiv", Nan::New<v8::FunctionTemplate>(EXPORT_getQueryObjectuiv));
    Nan::SetTemplate(exports, "bindBuffer", Nan::New<v8::FunctionTemplate>(EXPORT_bindBuffer));
    Nan::SetTemplate(exports, "genBuffers", Nan::New<v8::FunctionTemplate>(EXPORT_genBuffers));
    Nan::SetTemplate(exports, "isBuffer", Nan::New<v8::FunctionTemplate>(EXPORT_isBuffer));
    Nan::SetTemplate(exports, "bufferData", Nan::New<v8::FunctionTemplate>(EXPORT_bufferData));
    Nan::SetTemplate(exports, "bufferSubData", Nan::New<v8::FunctionTemplate>(EXPORT_bufferSubData));
    Nan::SetTemplate(exports, "getBufferSubData", Nan::New<v8::FunctionTemplate>(EXPORT_getBufferSubData));
    Nan::SetTemplate(exports, "mapBuffer", Nan::New<v8::FunctionTemplate>(EXPORT_mapBuffer));
    Nan::SetTemplate(exports, "unmapBuffer", Nan::New<v8::FunctionTemplate>(EXPORT_unmapBuffer));
    Nan::SetTemplate(exports, "getBufferParameteriv", Nan::New<v8::FunctionTemplate>(EXPORT_getBufferParameteriv));
    Nan::SetTemplate(exports, "blendEquationSeparate", Nan::New<v8::FunctionTemplate>(EXPORT_blendEquationSeparate));
    Nan::SetTemplate(exports, "drawBuffers", Nan::New<v8::FunctionTemplate>(EXPORT_drawBuffers));
    Nan::SetTemplate(exports, "stencilOpSeparate", Nan::New<v8::FunctionTemplate>(EXPORT_stencilOpSeparate));
    Nan::SetTemplate(exports, "stencilFuncSeparate", Nan::New<v8::FunctionTemplate>(EXPORT_stencilFuncSeparate));
    Nan::SetTemplate(exports, "stencilMaskSeparate", Nan::New<v8::FunctionTemplate>(EXPORT_stencilMaskSeparate));
    Nan::SetTemplate(exports, "attachShader", Nan::New<v8::FunctionTemplate>(EXPORT_attachShader));
    Nan::SetTemplate(exports, "bindAttribLocation", Nan::New<v8::FunctionTemplate>(EXPORT_bindAttribLocation));
    Nan::SetTemplate(exports, "compileShader", Nan::New<v8::FunctionTemplate>(EXPORT_compileShader));
    Nan::SetTemplate(exports, "createProgram", Nan::New<v8::FunctionTemplate>(EXPORT_createProgram));
    Nan::SetTemplate(exports, "createShader", Nan::New<v8::FunctionTemplate>(EXPORT_createShader));
    Nan::SetTemplate(exports, "detachShader", Nan::New<v8::FunctionTemplate>(EXPORT_detachShader));
    Nan::SetTemplate(exports, "disableVertexAttribArray", Nan::New<v8::FunctionTemplate>(EXPORT_disableVertexAttribArray));
    Nan::SetTemplate(exports, "enableVertexAttribArray", Nan::New<v8::FunctionTemplate>(EXPORT_enableVertexAttribArray));
    Nan::SetTemplate(exports, "getActiveAttrib", Nan::New<v8::FunctionTemplate>(EXPORT_getActiveAttrib));
    Nan::SetTemplate(exports, "getActiveUniform", Nan::New<v8::FunctionTemplate>(EXPORT_getActiveUniform));
    Nan::SetTemplate(exports, "getAttachedShaders", Nan::New<v8::FunctionTemplate>(EXPORT_getAttachedShaders));
    Nan::SetTemplate(exports, "getAttribLocation", Nan::New<v8::FunctionTemplate>(EXPORT_getAttribLocation));
    Nan::SetTemplate(exports, "getProgramiv", Nan::New<v8::FunctionTemplate>(EXPORT_getProgramiv));
    Nan::SetTemplate(exports, "getProgramInfoLog", Nan::New<v8::FunctionTemplate>(EXPORT_getProgramInfoLog));
    Nan::SetTemplate(exports, "getShaderiv", Nan::New<v8::FunctionTemplate>(EXPORT_getShaderiv));
    Nan::SetTemplate(exports, "getShaderInfoLog", Nan::New<v8::FunctionTemplate>(EXPORT_getShaderInfoLog));
    Nan::SetTemplate(exports, "getShaderSource", Nan::New<v8::FunctionTemplate>(EXPORT_getShaderSource));
    Nan::SetTemplate(exports, "getUniformLocation", Nan::New<v8::FunctionTemplate>(EXPORT_getUniformLocation));
    Nan::SetTemplate(exports, "getUniformfv", Nan::New<v8::FunctionTemplate>(EXPORT_getUniformfv));
    Nan::SetTemplate(exports, "getUniformiv", Nan::New<v8::FunctionTemplate>(EXPORT_getUniformiv));
    Nan::SetTemplate(exports, "getVertexAttribdv", Nan::New<v8::FunctionTemplate>(EXPORT_getVertexAttribdv));
    Nan::SetTemplate(exports, "getVertexAttribfv", Nan::New<v8::FunctionTemplate>(EXPORT_getVertexAttribfv));
    Nan::SetTemplate(exports, "getVertexAttribiv", Nan::New<v8::FunctionTemplate>(EXPORT_getVertexAttribiv));
    Nan::SetTemplate(exports, "isProgram", Nan::New<v8::FunctionTemplate>(EXPORT_isProgram));
    Nan::SetTemplate(exports, "isShader", Nan::New<v8::FunctionTemplate>(EXPORT_isShader));
    Nan::SetTemplate(exports, "linkProgram", Nan::New<v8::FunctionTemplate>(EXPORT_linkProgram));
    Nan::SetTemplate(exports, "useProgram", Nan::New<v8::FunctionTemplate>(EXPORT_useProgram));
    Nan::SetTemplate(exports, "uniform1f", Nan::New<v8::FunctionTemplate>(EXPORT_uniform1f));
    Nan::SetTemplate(exports, "uniform2f", Nan::New<v8::FunctionTemplate>(EXPORT_uniform2f));
    Nan::SetTemplate(exports, "uniform3f", Nan::New<v8::FunctionTemplate>(EXPORT_uniform3f));
    Nan::SetTemplate(exports, "uniform4f", Nan::New<v8::FunctionTemplate>(EXPORT_uniform4f));
    Nan::SetTemplate(exports, "uniform1i", Nan::New<v8::FunctionTemplate>(EXPORT_uniform1i));
    Nan::SetTemplate(exports, "uniform2i", Nan::New<v8::FunctionTemplate>(EXPORT_uniform2i));
    Nan::SetTemplate(exports, "uniform3i", Nan::New<v8::FunctionTemplate>(EXPORT_uniform3i));
    Nan::SetTemplate(exports, "uniform4i", Nan::New<v8::FunctionTemplate>(EXPORT_uniform4i));
    Nan::SetTemplate(exports, "uniform1fv", Nan::New<v8::FunctionTemplate>(EXPORT_uniform1fv));
    Nan::SetTemplate(exports, "uniform2fv", Nan::New<v8::FunctionTemplate>(EXPORT_uniform2fv));
    Nan::SetTemplate(exports, "uniform3fv", Nan::New<v8::FunctionTemplate>(EXPORT_uniform3fv));
    Nan::SetTemplate(exports, "uniform4fv", Nan::New<v8::FunctionTemplate>(EXPORT_uniform4fv));
    Nan::SetTemplate(exports, "uniform1iv", Nan::New<v8::FunctionTemplate>(EXPORT_uniform1iv));
    Nan::SetTemplate(exports, "uniform2iv", Nan::New<v8::FunctionTemplate>(EXPORT_uniform2iv));
    Nan::SetTemplate(exports, "uniform3iv", Nan::New<v8::FunctionTemplate>(EXPORT_uniform3iv));
    Nan::SetTemplate(exports, "uniform4iv", Nan::New<v8::FunctionTemplate>(EXPORT_uniform4iv));
    Nan::SetTemplate(exports, "uniformMatrix2fv", Nan::New<v8::FunctionTemplate>(EXPORT_uniformMatrix2fv));
    Nan::SetTemplate(exports, "uniformMatrix3fv", Nan::New<v8::FunctionTemplate>(EXPORT_uniformMatrix3fv));
    Nan::SetTemplate(exports, "uniformMatrix4fv", Nan::New<v8::FunctionTemplate>(EXPORT_uniformMatrix4fv));
    Nan::SetTemplate(exports, "validateProgram", Nan::New<v8::FunctionTemplate>(EXPORT_validateProgram));
    Nan::SetTemplate(exports, "vertexAttrib1d", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttrib1d));
    Nan::SetTemplate(exports, "vertexAttrib1dv", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttrib1dv));
    Nan::SetTemplate(exports, "vertexAttrib1f", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttrib1f));
    Nan::SetTemplate(exports, "vertexAttrib1fv", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttrib1fv));
    Nan::SetTemplate(exports, "vertexAttrib1s", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttrib1s));
    Nan::SetTemplate(exports, "vertexAttrib1sv", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttrib1sv));
    Nan::SetTemplate(exports, "vertexAttrib2d", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttrib2d));
    Nan::SetTemplate(exports, "vertexAttrib2dv", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttrib2dv));
    Nan::SetTemplate(exports, "vertexAttrib2f", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttrib2f));
    Nan::SetTemplate(exports, "vertexAttrib2fv", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttrib2fv));
    Nan::SetTemplate(exports, "vertexAttrib2s", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttrib2s));
    Nan::SetTemplate(exports, "vertexAttrib2sv", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttrib2sv));
    Nan::SetTemplate(exports, "vertexAttrib3d", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttrib3d));
    Nan::SetTemplate(exports, "vertexAttrib3dv", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttrib3dv));
    Nan::SetTemplate(exports, "vertexAttrib3f", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttrib3f));
    Nan::SetTemplate(exports, "vertexAttrib3fv", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttrib3fv));
    Nan::SetTemplate(exports, "vertexAttrib3s", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttrib3s));
    Nan::SetTemplate(exports, "vertexAttrib3sv", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttrib3sv));
    Nan::SetTemplate(exports, "vertexAttrib4Nbv", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttrib4Nbv));
    Nan::SetTemplate(exports, "vertexAttrib4Niv", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttrib4Niv));
    Nan::SetTemplate(exports, "vertexAttrib4Nsv", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttrib4Nsv));
    Nan::SetTemplate(exports, "vertexAttrib4Nub", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttrib4Nub));
    Nan::SetTemplate(exports, "vertexAttrib4Nubv", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttrib4Nubv));
    Nan::SetTemplate(exports, "vertexAttrib4Nuiv", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttrib4Nuiv));
    Nan::SetTemplate(exports, "vertexAttrib4Nusv", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttrib4Nusv));
    Nan::SetTemplate(exports, "vertexAttrib4bv", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttrib4bv));
    Nan::SetTemplate(exports, "vertexAttrib4d", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttrib4d));
    Nan::SetTemplate(exports, "vertexAttrib4dv", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttrib4dv));
    Nan::SetTemplate(exports, "vertexAttrib4f", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttrib4f));
    Nan::SetTemplate(exports, "vertexAttrib4fv", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttrib4fv));
    Nan::SetTemplate(exports, "vertexAttrib4iv", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttrib4iv));
    Nan::SetTemplate(exports, "vertexAttrib4s", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttrib4s));
    Nan::SetTemplate(exports, "vertexAttrib4sv", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttrib4sv));
    Nan::SetTemplate(exports, "vertexAttrib4ubv", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttrib4ubv));
    Nan::SetTemplate(exports, "vertexAttrib4uiv", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttrib4uiv));
    Nan::SetTemplate(exports, "vertexAttrib4usv", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttrib4usv));
    Nan::SetTemplate(exports, "vertexAttribPointer", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttribPointer));
    Nan::SetTemplate(exports, "uniformMatrix2x3fv", Nan::New<v8::FunctionTemplate>(EXPORT_uniformMatrix2x3fv));
    Nan::SetTemplate(exports, "uniformMatrix3x2fv", Nan::New<v8::FunctionTemplate>(EXPORT_uniformMatrix3x2fv));
    Nan::SetTemplate(exports, "uniformMatrix2x4fv", Nan::New<v8::FunctionTemplate>(EXPORT_uniformMatrix2x4fv));
    Nan::SetTemplate(exports, "uniformMatrix4x2fv", Nan::New<v8::FunctionTemplate>(EXPORT_uniformMatrix4x2fv));
    Nan::SetTemplate(exports, "uniformMatrix3x4fv", Nan::New<v8::FunctionTemplate>(EXPORT_uniformMatrix3x4fv));
    Nan::SetTemplate(exports, "uniformMatrix4x3fv", Nan::New<v8::FunctionTemplate>(EXPORT_uniformMatrix4x3fv));
    Nan::SetTemplate(exports, "colorMaski", Nan::New<v8::FunctionTemplate>(EXPORT_colorMaski));
    Nan::SetTemplate(exports, "getBooleani_v", Nan::New<v8::FunctionTemplate>(EXPORT_getBooleani_v));
    Nan::SetTemplate(exports, "getIntegeri_v", Nan::New<v8::FunctionTemplate>(EXPORT_getIntegeri_v));
    Nan::SetTemplate(exports, "enablei", Nan::New<v8::FunctionTemplate>(EXPORT_enablei));
    Nan::SetTemplate(exports, "disablei", Nan::New<v8::FunctionTemplate>(EXPORT_disablei));
    Nan::SetTemplate(exports, "isEnabledi", Nan::New<v8::FunctionTemplate>(EXPORT_isEnabledi));
    Nan::SetTemplate(exports, "beginTransformFeedback", Nan::New<v8::FunctionTemplate>(EXPORT_beginTransformFeedback));
    Nan::SetTemplate(exports, "endTransformFeedback", Nan::New<v8::FunctionTemplate>(EXPORT_endTransformFeedback));
    Nan::SetTemplate(exports, "bindBufferRange", Nan::New<v8::FunctionTemplate>(EXPORT_bindBufferRange));
    Nan::SetTemplate(exports, "bindBufferBase", Nan::New<v8::FunctionTemplate>(EXPORT_bindBufferBase));
    Nan::SetTemplate(exports, "getTransformFeedbackVarying", Nan::New<v8::FunctionTemplate>(EXPORT_getTransformFeedbackVarying));
    Nan::SetTemplate(exports, "clampColor", Nan::New<v8::FunctionTemplate>(EXPORT_clampColor));
    Nan::SetTemplate(exports, "beginConditionalRender", Nan::New<v8::FunctionTemplate>(EXPORT_beginConditionalRender));
    Nan::SetTemplate(exports, "endConditionalRender", Nan::New<v8::FunctionTemplate>(EXPORT_endConditionalRender));
    Nan::SetTemplate(exports, "vertexAttribIPointer", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttribIPointer));
    Nan::SetTemplate(exports, "getVertexAttribIiv", Nan::New<v8::FunctionTemplate>(EXPORT_getVertexAttribIiv));
    Nan::SetTemplate(exports, "getVertexAttribIuiv", Nan::New<v8::FunctionTemplate>(EXPORT_getVertexAttribIuiv));
    Nan::SetTemplate(exports, "vertexAttribI1i", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttribI1i));
    Nan::SetTemplate(exports, "vertexAttribI2i", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttribI2i));
    Nan::SetTemplate(exports, "vertexAttribI3i", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttribI3i));
    Nan::SetTemplate(exports, "vertexAttribI4i", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttribI4i));
    Nan::SetTemplate(exports, "vertexAttribI1ui", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttribI1ui));
    Nan::SetTemplate(exports, "vertexAttribI2ui", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttribI2ui));
    Nan::SetTemplate(exports, "vertexAttribI3ui", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttribI3ui));
    Nan::SetTemplate(exports, "vertexAttribI4ui", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttribI4ui));
    Nan::SetTemplate(exports, "vertexAttribI1iv", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttribI1iv));
    Nan::SetTemplate(exports, "vertexAttribI2iv", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttribI2iv));
    Nan::SetTemplate(exports, "vertexAttribI3iv", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttribI3iv));
    Nan::SetTemplate(exports, "vertexAttribI4iv", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttribI4iv));
    Nan::SetTemplate(exports, "vertexAttribI1uiv", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttribI1uiv));
    Nan::SetTemplate(exports, "vertexAttribI2uiv", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttribI2uiv));
    Nan::SetTemplate(exports, "vertexAttribI3uiv", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttribI3uiv));
    Nan::SetTemplate(exports, "vertexAttribI4uiv", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttribI4uiv));
    Nan::SetTemplate(exports, "vertexAttribI4bv", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttribI4bv));
    Nan::SetTemplate(exports, "vertexAttribI4sv", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttribI4sv));
    Nan::SetTemplate(exports, "vertexAttribI4ubv", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttribI4ubv));
    Nan::SetTemplate(exports, "vertexAttribI4usv", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttribI4usv));
    Nan::SetTemplate(exports, "getUniformuiv", Nan::New<v8::FunctionTemplate>(EXPORT_getUniformuiv));
    Nan::SetTemplate(exports, "bindFragDataLocation", Nan::New<v8::FunctionTemplate>(EXPORT_bindFragDataLocation));
    Nan::SetTemplate(exports, "getFragDataLocation", Nan::New<v8::FunctionTemplate>(EXPORT_getFragDataLocation));
    Nan::SetTemplate(exports, "uniform1ui", Nan::New<v8::FunctionTemplate>(EXPORT_uniform1ui));
    Nan::SetTemplate(exports, "uniform2ui", Nan::New<v8::FunctionTemplate>(EXPORT_uniform2ui));
    Nan::SetTemplate(exports, "uniform3ui", Nan::New<v8::FunctionTemplate>(EXPORT_uniform3ui));
    Nan::SetTemplate(exports, "uniform4ui", Nan::New<v8::FunctionTemplate>(EXPORT_uniform4ui));
    Nan::SetTemplate(exports, "uniform1uiv", Nan::New<v8::FunctionTemplate>(EXPORT_uniform1uiv));
    Nan::SetTemplate(exports, "uniform2uiv", Nan::New<v8::FunctionTemplate>(EXPORT_uniform2uiv));
    Nan::SetTemplate(exports, "uniform3uiv", Nan::New<v8::FunctionTemplate>(EXPORT_uniform3uiv));
    Nan::SetTemplate(exports, "uniform4uiv", Nan::New<v8::FunctionTemplate>(EXPORT_uniform4uiv));
    Nan::SetTemplate(exports, "texParameterIiv", Nan::New<v8::FunctionTemplate>(EXPORT_texParameterIiv));
    Nan::SetTemplate(exports, "texParameterIuiv", Nan::New<v8::FunctionTemplate>(EXPORT_texParameterIuiv));
    Nan::SetTemplate(exports, "getTexParameterIiv", Nan::New<v8::FunctionTemplate>(EXPORT_getTexParameterIiv));
    Nan::SetTemplate(exports, "getTexParameterIuiv", Nan::New<v8::FunctionTemplate>(EXPORT_getTexParameterIuiv));
    Nan::SetTemplate(exports, "clearBufferiv", Nan::New<v8::FunctionTemplate>(EXPORT_clearBufferiv));
    Nan::SetTemplate(exports, "clearBufferuiv", Nan::New<v8::FunctionTemplate>(EXPORT_clearBufferuiv));
    Nan::SetTemplate(exports, "clearBufferfv", Nan::New<v8::FunctionTemplate>(EXPORT_clearBufferfv));
    Nan::SetTemplate(exports, "clearBufferfi", Nan::New<v8::FunctionTemplate>(EXPORT_clearBufferfi));
    Nan::SetTemplate(exports, "getStringi", Nan::New<v8::FunctionTemplate>(EXPORT_getStringi));
    Nan::SetTemplate(exports, "drawArraysInstanced", Nan::New<v8::FunctionTemplate>(EXPORT_drawArraysInstanced));
    Nan::SetTemplate(exports, "drawElementsInstanced", Nan::New<v8::FunctionTemplate>(EXPORT_drawElementsInstanced));
    Nan::SetTemplate(exports, "texBuffer", Nan::New<v8::FunctionTemplate>(EXPORT_texBuffer));
    Nan::SetTemplate(exports, "primitiveRestartIndex", Nan::New<v8::FunctionTemplate>(EXPORT_primitiveRestartIndex));
    Nan::SetTemplate(exports, "getInteger64i_v", Nan::New<v8::FunctionTemplate>(EXPORT_getInteger64i_v));
    Nan::SetTemplate(exports, "getBufferParameteri64v", Nan::New<v8::FunctionTemplate>(EXPORT_getBufferParameteri64v));
    Nan::SetTemplate(exports, "framebufferTexture", Nan::New<v8::FunctionTemplate>(EXPORT_framebufferTexture));
    Nan::SetTemplate(exports, "vertexAttribDivisor", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttribDivisor));
    Nan::SetTemplate(exports, "minSampleShading", Nan::New<v8::FunctionTemplate>(EXPORT_minSampleShading));
    Nan::SetTemplate(exports, "blendEquationi", Nan::New<v8::FunctionTemplate>(EXPORT_blendEquationi));
    Nan::SetTemplate(exports, "blendEquationSeparatei", Nan::New<v8::FunctionTemplate>(EXPORT_blendEquationSeparatei));
    Nan::SetTemplate(exports, "blendFunci", Nan::New<v8::FunctionTemplate>(EXPORT_blendFunci));
    Nan::SetTemplate(exports, "blendFuncSeparatei", Nan::New<v8::FunctionTemplate>(EXPORT_blendFuncSeparatei));
    Nan::SetTemplate(exports, "isRenderbuffer", Nan::New<v8::FunctionTemplate>(EXPORT_isRenderbuffer));
    Nan::SetTemplate(exports, "bindRenderbuffer", Nan::New<v8::FunctionTemplate>(EXPORT_bindRenderbuffer));
    Nan::SetTemplate(exports, "genRenderbuffers", Nan::New<v8::FunctionTemplate>(EXPORT_genRenderbuffers));
    Nan::SetTemplate(exports, "renderbufferStorage", Nan::New<v8::FunctionTemplate>(EXPORT_renderbufferStorage));
    Nan::SetTemplate(exports, "getRenderbufferParameteriv", Nan::New<v8::FunctionTemplate>(EXPORT_getRenderbufferParameteriv));
    Nan::SetTemplate(exports, "isFramebuffer", Nan::New<v8::FunctionTemplate>(EXPORT_isFramebuffer));
    Nan::SetTemplate(exports, "bindFramebuffer", Nan::New<v8::FunctionTemplate>(EXPORT_bindFramebuffer));
    Nan::SetTemplate(exports, "genFramebuffers", Nan::New<v8::FunctionTemplate>(EXPORT_genFramebuffers));
    Nan::SetTemplate(exports, "checkFramebufferStatus", Nan::New<v8::FunctionTemplate>(EXPORT_checkFramebufferStatus));
    Nan::SetTemplate(exports, "framebufferTexture1D", Nan::New<v8::FunctionTemplate>(EXPORT_framebufferTexture1D));
    Nan::SetTemplate(exports, "framebufferTexture2D", Nan::New<v8::FunctionTemplate>(EXPORT_framebufferTexture2D));
    Nan::SetTemplate(exports, "framebufferTexture3D", Nan::New<v8::FunctionTemplate>(EXPORT_framebufferTexture3D));
    Nan::SetTemplate(exports, "framebufferRenderbuffer", Nan::New<v8::FunctionTemplate>(EXPORT_framebufferRenderbuffer));
    Nan::SetTemplate(exports, "getFramebufferAttachmentParameteriv", Nan::New<v8::FunctionTemplate>(EXPORT_getFramebufferAttachmentParameteriv));
    Nan::SetTemplate(exports, "generateMipmap", Nan::New<v8::FunctionTemplate>(EXPORT_generateMipmap));
    Nan::SetTemplate(exports, "blitFramebuffer", Nan::New<v8::FunctionTemplate>(EXPORT_blitFramebuffer));
    Nan::SetTemplate(exports, "renderbufferStorageMultisample", Nan::New<v8::FunctionTemplate>(EXPORT_renderbufferStorageMultisample));
    Nan::SetTemplate(exports, "framebufferTextureLayer", Nan::New<v8::FunctionTemplate>(EXPORT_framebufferTextureLayer));
    Nan::SetTemplate(exports, "mapBufferRange", Nan::New<v8::FunctionTemplate>(EXPORT_mapBufferRange));
    Nan::SetTemplate(exports, "flushMappedBufferRange", Nan::New<v8::FunctionTemplate>(EXPORT_flushMappedBufferRange));
    Nan::SetTemplate(exports, "bindVertexArray", Nan::New<v8::FunctionTemplate>(EXPORT_bindVertexArray));
    Nan::SetTemplate(exports, "genVertexArrays", Nan::New<v8::FunctionTemplate>(EXPORT_genVertexArrays));
    Nan::SetTemplate(exports, "isVertexArray", Nan::New<v8::FunctionTemplate>(EXPORT_isVertexArray));
    Nan::SetTemplate(exports, "getActiveUniformsiv", Nan::New<v8::FunctionTemplate>(EXPORT_getActiveUniformsiv));
    Nan::SetTemplate(exports, "getActiveUniformName", Nan::New<v8::FunctionTemplate>(EXPORT_getActiveUniformName));
    Nan::SetTemplate(exports, "getUniformBlockIndex", Nan::New<v8::FunctionTemplate>(EXPORT_getUniformBlockIndex));
    Nan::SetTemplate(exports, "getActiveUniformBlockiv", Nan::New<v8::FunctionTemplate>(EXPORT_getActiveUniformBlockiv));
    Nan::SetTemplate(exports, "getActiveUniformBlockName", Nan::New<v8::FunctionTemplate>(EXPORT_getActiveUniformBlockName));
    Nan::SetTemplate(exports, "uniformBlockBinding", Nan::New<v8::FunctionTemplate>(EXPORT_uniformBlockBinding));
    Nan::SetTemplate(exports, "copyBufferSubData", Nan::New<v8::FunctionTemplate>(EXPORT_copyBufferSubData));
    Nan::SetTemplate(exports, "drawElementsBaseVertex", Nan::New<v8::FunctionTemplate>(EXPORT_drawElementsBaseVertex));
    Nan::SetTemplate(exports, "drawRangeElementsBaseVertex", Nan::New<v8::FunctionTemplate>(EXPORT_drawRangeElementsBaseVertex));
    Nan::SetTemplate(exports, "drawElementsInstancedBaseVertex", Nan::New<v8::FunctionTemplate>(EXPORT_drawElementsInstancedBaseVertex));
    Nan::SetTemplate(exports, "provokingVertex", Nan::New<v8::FunctionTemplate>(EXPORT_provokingVertex));
    Nan::SetTemplate(exports, "texImage2DMultisample", Nan::New<v8::FunctionTemplate>(EXPORT_texImage2DMultisample));
    Nan::SetTemplate(exports, "texImage3DMultisample", Nan::New<v8::FunctionTemplate>(EXPORT_texImage3DMultisample));
    Nan::SetTemplate(exports, "getMultisamplefv", Nan::New<v8::FunctionTemplate>(EXPORT_getMultisamplefv));
    Nan::SetTemplate(exports, "sampleMaski", Nan::New<v8::FunctionTemplate>(EXPORT_sampleMaski));
    Nan::SetTemplate(exports, "bindFragDataLocationIndexed", Nan::New<v8::FunctionTemplate>(EXPORT_bindFragDataLocationIndexed));
    Nan::SetTemplate(exports, "getFragDataIndex", Nan::New<v8::FunctionTemplate>(EXPORT_getFragDataIndex));
    Nan::SetTemplate(exports, "genSamplers", Nan::New<v8::FunctionTemplate>(EXPORT_genSamplers));
    Nan::SetTemplate(exports, "isSampler", Nan::New<v8::FunctionTemplate>(EXPORT_isSampler));
    Nan::SetTemplate(exports, "bindSampler", Nan::New<v8::FunctionTemplate>(EXPORT_bindSampler));
    Nan::SetTemplate(exports, "samplerParameteri", Nan::New<v8::FunctionTemplate>(EXPORT_samplerParameteri));
    Nan::SetTemplate(exports, "samplerParameteriv", Nan::New<v8::FunctionTemplate>(EXPORT_samplerParameteriv));
    Nan::SetTemplate(exports, "samplerParameterf", Nan::New<v8::FunctionTemplate>(EXPORT_samplerParameterf));
    Nan::SetTemplate(exports, "samplerParameterfv", Nan::New<v8::FunctionTemplate>(EXPORT_samplerParameterfv));
    Nan::SetTemplate(exports, "samplerParameterIiv", Nan::New<v8::FunctionTemplate>(EXPORT_samplerParameterIiv));
    Nan::SetTemplate(exports, "samplerParameterIuiv", Nan::New<v8::FunctionTemplate>(EXPORT_samplerParameterIuiv));
    Nan::SetTemplate(exports, "getSamplerParameteriv", Nan::New<v8::FunctionTemplate>(EXPORT_getSamplerParameteriv));
    Nan::SetTemplate(exports, "getSamplerParameterIiv", Nan::New<v8::FunctionTemplate>(EXPORT_getSamplerParameterIiv));
    Nan::SetTemplate(exports, "getSamplerParameterfv", Nan::New<v8::FunctionTemplate>(EXPORT_getSamplerParameterfv));
    Nan::SetTemplate(exports, "getSamplerParameterIuiv", Nan::New<v8::FunctionTemplate>(EXPORT_getSamplerParameterIuiv));
    Nan::SetTemplate(exports, "queryCounter", Nan::New<v8::FunctionTemplate>(EXPORT_queryCounter));
    Nan::SetTemplate(exports, "getQueryObjecti64v", Nan::New<v8::FunctionTemplate>(EXPORT_getQueryObjecti64v));
    Nan::SetTemplate(exports, "getQueryObjectui64v", Nan::New<v8::FunctionTemplate>(EXPORT_getQueryObjectui64v));
    Nan::SetTemplate(exports, "vertexAttribP1ui", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttribP1ui));
    Nan::SetTemplate(exports, "vertexAttribP1uiv", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttribP1uiv));
    Nan::SetTemplate(exports, "vertexAttribP2ui", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttribP2ui));
    Nan::SetTemplate(exports, "vertexAttribP2uiv", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttribP2uiv));
    Nan::SetTemplate(exports, "vertexAttribP3ui", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttribP3ui));
    Nan::SetTemplate(exports, "vertexAttribP3uiv", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttribP3uiv));
    Nan::SetTemplate(exports, "vertexAttribP4ui", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttribP4ui));
    Nan::SetTemplate(exports, "vertexAttribP4uiv", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttribP4uiv));
    Nan::SetTemplate(exports, "drawArraysIndirect", Nan::New<v8::FunctionTemplate>(EXPORT_drawArraysIndirect));
    Nan::SetTemplate(exports, "drawElementsIndirect", Nan::New<v8::FunctionTemplate>(EXPORT_drawElementsIndirect));
    Nan::SetTemplate(exports, "uniform1d", Nan::New<v8::FunctionTemplate>(EXPORT_uniform1d));
    Nan::SetTemplate(exports, "uniform2d", Nan::New<v8::FunctionTemplate>(EXPORT_uniform2d));
    Nan::SetTemplate(exports, "uniform3d", Nan::New<v8::FunctionTemplate>(EXPORT_uniform3d));
    Nan::SetTemplate(exports, "uniform4d", Nan::New<v8::FunctionTemplate>(EXPORT_uniform4d));
    Nan::SetTemplate(exports, "uniform1dv", Nan::New<v8::FunctionTemplate>(EXPORT_uniform1dv));
    Nan::SetTemplate(exports, "uniform2dv", Nan::New<v8::FunctionTemplate>(EXPORT_uniform2dv));
    Nan::SetTemplate(exports, "uniform3dv", Nan::New<v8::FunctionTemplate>(EXPORT_uniform3dv));
    Nan::SetTemplate(exports, "uniform4dv", Nan::New<v8::FunctionTemplate>(EXPORT_uniform4dv));
    Nan::SetTemplate(exports, "uniformMatrix2dv", Nan::New<v8::FunctionTemplate>(EXPORT_uniformMatrix2dv));
    Nan::SetTemplate(exports, "uniformMatrix3dv", Nan::New<v8::FunctionTemplate>(EXPORT_uniformMatrix3dv));
    Nan::SetTemplate(exports, "uniformMatrix4dv", Nan::New<v8::FunctionTemplate>(EXPORT_uniformMatrix4dv));
    Nan::SetTemplate(exports, "uniformMatrix2x3dv", Nan::New<v8::FunctionTemplate>(EXPORT_uniformMatrix2x3dv));
    Nan::SetTemplate(exports, "uniformMatrix2x4dv", Nan::New<v8::FunctionTemplate>(EXPORT_uniformMatrix2x4dv));
    Nan::SetTemplate(exports, "uniformMatrix3x2dv", Nan::New<v8::FunctionTemplate>(EXPORT_uniformMatrix3x2dv));
    Nan::SetTemplate(exports, "uniformMatrix3x4dv", Nan::New<v8::FunctionTemplate>(EXPORT_uniformMatrix3x4dv));
    Nan::SetTemplate(exports, "uniformMatrix4x2dv", Nan::New<v8::FunctionTemplate>(EXPORT_uniformMatrix4x2dv));
    Nan::SetTemplate(exports, "uniformMatrix4x3dv", Nan::New<v8::FunctionTemplate>(EXPORT_uniformMatrix4x3dv));
    Nan::SetTemplate(exports, "getUniformdv", Nan::New<v8::FunctionTemplate>(EXPORT_getUniformdv));
    Nan::SetTemplate(exports, "getSubroutineUniformLocation", Nan::New<v8::FunctionTemplate>(EXPORT_getSubroutineUniformLocation));
    Nan::SetTemplate(exports, "getSubroutineIndex", Nan::New<v8::FunctionTemplate>(EXPORT_getSubroutineIndex));
    Nan::SetTemplate(exports, "getActiveSubroutineUniformiv", Nan::New<v8::FunctionTemplate>(EXPORT_getActiveSubroutineUniformiv));
    Nan::SetTemplate(exports, "getActiveSubroutineUniformName", Nan::New<v8::FunctionTemplate>(EXPORT_getActiveSubroutineUniformName));
    Nan::SetTemplate(exports, "getActiveSubroutineName", Nan::New<v8::FunctionTemplate>(EXPORT_getActiveSubroutineName));
    Nan::SetTemplate(exports, "uniformSubroutinesuiv", Nan::New<v8::FunctionTemplate>(EXPORT_uniformSubroutinesuiv));
    Nan::SetTemplate(exports, "getUniformSubroutineuiv", Nan::New<v8::FunctionTemplate>(EXPORT_getUniformSubroutineuiv));
    Nan::SetTemplate(exports, "getProgramStageiv", Nan::New<v8::FunctionTemplate>(EXPORT_getProgramStageiv));
    Nan::SetTemplate(exports, "patchParameteri", Nan::New<v8::FunctionTemplate>(EXPORT_patchParameteri));
    Nan::SetTemplate(exports, "patchParameterfv", Nan::New<v8::FunctionTemplate>(EXPORT_patchParameterfv));
    Nan::SetTemplate(exports, "bindTransformFeedback", Nan::New<v8::FunctionTemplate>(EXPORT_bindTransformFeedback));
    Nan::SetTemplate(exports, "genTransformFeedbacks", Nan::New<v8::FunctionTemplate>(EXPORT_genTransformFeedbacks));
    Nan::SetTemplate(exports, "isTransformFeedback", Nan::New<v8::FunctionTemplate>(EXPORT_isTransformFeedback));
    Nan::SetTemplate(exports, "pauseTransformFeedback", Nan::New<v8::FunctionTemplate>(EXPORT_pauseTransformFeedback));
    Nan::SetTemplate(exports, "resumeTransformFeedback", Nan::New<v8::FunctionTemplate>(EXPORT_resumeTransformFeedback));
    Nan::SetTemplate(exports, "drawTransformFeedback", Nan::New<v8::FunctionTemplate>(EXPORT_drawTransformFeedback));
    Nan::SetTemplate(exports, "drawTransformFeedbackStream", Nan::New<v8::FunctionTemplate>(EXPORT_drawTransformFeedbackStream));
    Nan::SetTemplate(exports, "beginQueryIndexed", Nan::New<v8::FunctionTemplate>(EXPORT_beginQueryIndexed));
    Nan::SetTemplate(exports, "endQueryIndexed", Nan::New<v8::FunctionTemplate>(EXPORT_endQueryIndexed));
    Nan::SetTemplate(exports, "getQueryIndexediv", Nan::New<v8::FunctionTemplate>(EXPORT_getQueryIndexediv));
    Nan::SetTemplate(exports, "releaseShaderCompiler", Nan::New<v8::FunctionTemplate>(EXPORT_releaseShaderCompiler));
    Nan::SetTemplate(exports, "shaderBinary", Nan::New<v8::FunctionTemplate>(EXPORT_shaderBinary));
    Nan::SetTemplate(exports, "getShaderPrecisionFormat", Nan::New<v8::FunctionTemplate>(EXPORT_getShaderPrecisionFormat));
    Nan::SetTemplate(exports, "depthRangef", Nan::New<v8::FunctionTemplate>(EXPORT_depthRangef));
    Nan::SetTemplate(exports, "clearDepthf", Nan::New<v8::FunctionTemplate>(EXPORT_clearDepthf));
    Nan::SetTemplate(exports, "getProgramBinary", Nan::New<v8::FunctionTemplate>(EXPORT_getProgramBinary));
    Nan::SetTemplate(exports, "programBinary", Nan::New<v8::FunctionTemplate>(EXPORT_programBinary));
    Nan::SetTemplate(exports, "programParameteri", Nan::New<v8::FunctionTemplate>(EXPORT_programParameteri));
    Nan::SetTemplate(exports, "useProgramStages", Nan::New<v8::FunctionTemplate>(EXPORT_useProgramStages));
    Nan::SetTemplate(exports, "activeShaderProgram", Nan::New<v8::FunctionTemplate>(EXPORT_activeShaderProgram));
    Nan::SetTemplate(exports, "bindProgramPipeline", Nan::New<v8::FunctionTemplate>(EXPORT_bindProgramPipeline));
    Nan::SetTemplate(exports, "genProgramPipelines", Nan::New<v8::FunctionTemplate>(EXPORT_genProgramPipelines));
    Nan::SetTemplate(exports, "isProgramPipeline", Nan::New<v8::FunctionTemplate>(EXPORT_isProgramPipeline));
    Nan::SetTemplate(exports, "getProgramPipelineiv", Nan::New<v8::FunctionTemplate>(EXPORT_getProgramPipelineiv));
    Nan::SetTemplate(exports, "programUniform1i", Nan::New<v8::FunctionTemplate>(EXPORT_programUniform1i));
    Nan::SetTemplate(exports, "programUniform1iv", Nan::New<v8::FunctionTemplate>(EXPORT_programUniform1iv));
    Nan::SetTemplate(exports, "programUniform1f", Nan::New<v8::FunctionTemplate>(EXPORT_programUniform1f));
    Nan::SetTemplate(exports, "programUniform1fv", Nan::New<v8::FunctionTemplate>(EXPORT_programUniform1fv));
    Nan::SetTemplate(exports, "programUniform1d", Nan::New<v8::FunctionTemplate>(EXPORT_programUniform1d));
    Nan::SetTemplate(exports, "programUniform1dv", Nan::New<v8::FunctionTemplate>(EXPORT_programUniform1dv));
    Nan::SetTemplate(exports, "programUniform1ui", Nan::New<v8::FunctionTemplate>(EXPORT_programUniform1ui));
    Nan::SetTemplate(exports, "programUniform1uiv", Nan::New<v8::FunctionTemplate>(EXPORT_programUniform1uiv));
    Nan::SetTemplate(exports, "programUniform2i", Nan::New<v8::FunctionTemplate>(EXPORT_programUniform2i));
    Nan::SetTemplate(exports, "programUniform2iv", Nan::New<v8::FunctionTemplate>(EXPORT_programUniform2iv));
    Nan::SetTemplate(exports, "programUniform2f", Nan::New<v8::FunctionTemplate>(EXPORT_programUniform2f));
    Nan::SetTemplate(exports, "programUniform2fv", Nan::New<v8::FunctionTemplate>(EXPORT_programUniform2fv));
    Nan::SetTemplate(exports, "programUniform2d", Nan::New<v8::FunctionTemplate>(EXPORT_programUniform2d));
    Nan::SetTemplate(exports, "programUniform2dv", Nan::New<v8::FunctionTemplate>(EXPORT_programUniform2dv));
    Nan::SetTemplate(exports, "programUniform2ui", Nan::New<v8::FunctionTemplate>(EXPORT_programUniform2ui));
    Nan::SetTemplate(exports, "programUniform2uiv", Nan::New<v8::FunctionTemplate>(EXPORT_programUniform2uiv));
    Nan::SetTemplate(exports, "programUniform3i", Nan::New<v8::FunctionTemplate>(EXPORT_programUniform3i));
    Nan::SetTemplate(exports, "programUniform3iv", Nan::New<v8::FunctionTemplate>(EXPORT_programUniform3iv));
    Nan::SetTemplate(exports, "programUniform3f", Nan::New<v8::FunctionTemplate>(EXPORT_programUniform3f));
    Nan::SetTemplate(exports, "programUniform3fv", Nan::New<v8::FunctionTemplate>(EXPORT_programUniform3fv));
    Nan::SetTemplate(exports, "programUniform3d", Nan::New<v8::FunctionTemplate>(EXPORT_programUniform3d));
    Nan::SetTemplate(exports, "programUniform3dv", Nan::New<v8::FunctionTemplate>(EXPORT_programUniform3dv));
    Nan::SetTemplate(exports, "programUniform3ui", Nan::New<v8::FunctionTemplate>(EXPORT_programUniform3ui));
    Nan::SetTemplate(exports, "programUniform3uiv", Nan::New<v8::FunctionTemplate>(EXPORT_programUniform3uiv));
    Nan::SetTemplate(exports, "programUniform4i", Nan::New<v8::FunctionTemplate>(EXPORT_programUniform4i));
    Nan::SetTemplate(exports, "programUniform4iv", Nan::New<v8::FunctionTemplate>(EXPORT_programUniform4iv));
    Nan::SetTemplate(exports, "programUniform4f", Nan::New<v8::FunctionTemplate>(EXPORT_programUniform4f));
    Nan::SetTemplate(exports, "programUniform4fv", Nan::New<v8::FunctionTemplate>(EXPORT_programUniform4fv));
    Nan::SetTemplate(exports, "programUniform4d", Nan::New<v8::FunctionTemplate>(EXPORT_programUniform4d));
    Nan::SetTemplate(exports, "programUniform4dv", Nan::New<v8::FunctionTemplate>(EXPORT_programUniform4dv));
    Nan::SetTemplate(exports, "programUniform4ui", Nan::New<v8::FunctionTemplate>(EXPORT_programUniform4ui));
    Nan::SetTemplate(exports, "programUniform4uiv", Nan::New<v8::FunctionTemplate>(EXPORT_programUniform4uiv));
    Nan::SetTemplate(exports, "programUniformMatrix2fv", Nan::New<v8::FunctionTemplate>(EXPORT_programUniformMatrix2fv));
    Nan::SetTemplate(exports, "programUniformMatrix3fv", Nan::New<v8::FunctionTemplate>(EXPORT_programUniformMatrix3fv));
    Nan::SetTemplate(exports, "programUniformMatrix4fv", Nan::New<v8::FunctionTemplate>(EXPORT_programUniformMatrix4fv));
    Nan::SetTemplate(exports, "programUniformMatrix2dv", Nan::New<v8::FunctionTemplate>(EXPORT_programUniformMatrix2dv));
    Nan::SetTemplate(exports, "programUniformMatrix3dv", Nan::New<v8::FunctionTemplate>(EXPORT_programUniformMatrix3dv));
    Nan::SetTemplate(exports, "programUniformMatrix4dv", Nan::New<v8::FunctionTemplate>(EXPORT_programUniformMatrix4dv));
    Nan::SetTemplate(exports, "programUniformMatrix2x3fv", Nan::New<v8::FunctionTemplate>(EXPORT_programUniformMatrix2x3fv));
    Nan::SetTemplate(exports, "programUniformMatrix3x2fv", Nan::New<v8::FunctionTemplate>(EXPORT_programUniformMatrix3x2fv));
    Nan::SetTemplate(exports, "programUniformMatrix2x4fv", Nan::New<v8::FunctionTemplate>(EXPORT_programUniformMatrix2x4fv));
    Nan::SetTemplate(exports, "programUniformMatrix4x2fv", Nan::New<v8::FunctionTemplate>(EXPORT_programUniformMatrix4x2fv));
    Nan::SetTemplate(exports, "programUniformMatrix3x4fv", Nan::New<v8::FunctionTemplate>(EXPORT_programUniformMatrix3x4fv));
    Nan::SetTemplate(exports, "programUniformMatrix4x3fv", Nan::New<v8::FunctionTemplate>(EXPORT_programUniformMatrix4x3fv));
    Nan::SetTemplate(exports, "programUniformMatrix2x3dv", Nan::New<v8::FunctionTemplate>(EXPORT_programUniformMatrix2x3dv));
    Nan::SetTemplate(exports, "programUniformMatrix3x2dv", Nan::New<v8::FunctionTemplate>(EXPORT_programUniformMatrix3x2dv));
    Nan::SetTemplate(exports, "programUniformMatrix2x4dv", Nan::New<v8::FunctionTemplate>(EXPORT_programUniformMatrix2x4dv));
    Nan::SetTemplate(exports, "programUniformMatrix4x2dv", Nan::New<v8::FunctionTemplate>(EXPORT_programUniformMatrix4x2dv));
    Nan::SetTemplate(exports, "programUniformMatrix3x4dv", Nan::New<v8::FunctionTemplate>(EXPORT_programUniformMatrix3x4dv));
    Nan::SetTemplate(exports, "programUniformMatrix4x3dv", Nan::New<v8::FunctionTemplate>(EXPORT_programUniformMatrix4x3dv));
    Nan::SetTemplate(exports, "validateProgramPipeline", Nan::New<v8::FunctionTemplate>(EXPORT_validateProgramPipeline));
    Nan::SetTemplate(exports, "getProgramPipelineInfoLog", Nan::New<v8::FunctionTemplate>(EXPORT_getProgramPipelineInfoLog));
    Nan::SetTemplate(exports, "vertexAttribL1d", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttribL1d));
    Nan::SetTemplate(exports, "vertexAttribL2d", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttribL2d));
    Nan::SetTemplate(exports, "vertexAttribL3d", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttribL3d));
    Nan::SetTemplate(exports, "vertexAttribL4d", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttribL4d));
    Nan::SetTemplate(exports, "vertexAttribL1dv", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttribL1dv));
    Nan::SetTemplate(exports, "vertexAttribL2dv", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttribL2dv));
    Nan::SetTemplate(exports, "vertexAttribL3dv", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttribL3dv));
    Nan::SetTemplate(exports, "vertexAttribL4dv", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttribL4dv));
    Nan::SetTemplate(exports, "vertexAttribLPointer", Nan::New<v8::FunctionTemplate>(EXPORT_vertexAttribLPointer));
    Nan::SetTemplate(exports, "getVertexAttribLdv", Nan::New<v8::FunctionTemplate>(EXPORT_getVertexAttribLdv));
    Nan::SetTemplate(exports, "viewportArrayv", Nan::New<v8::FunctionTemplate>(EXPORT_viewportArrayv));
    Nan::SetTemplate(exports, "viewportIndexedf", Nan::New<v8::FunctionTemplate>(EXPORT_viewportIndexedf));
    Nan::SetTemplate(exports, "viewportIndexedfv", Nan::New<v8::FunctionTemplate>(EXPORT_viewportIndexedfv));
    Nan::SetTemplate(exports, "scissorArrayv", Nan::New<v8::FunctionTemplate>(EXPORT_scissorArrayv));
    Nan::SetTemplate(exports, "scissorIndexed", Nan::New<v8::FunctionTemplate>(EXPORT_scissorIndexed));
    Nan::SetTemplate(exports, "scissorIndexedv", Nan::New<v8::FunctionTemplate>(EXPORT_scissorIndexedv));
    Nan::SetTemplate(exports, "depthRangeArrayv", Nan::New<v8::FunctionTemplate>(EXPORT_depthRangeArrayv));
    Nan::SetTemplate(exports, "depthRangeIndexed", Nan::New<v8::FunctionTemplate>(EXPORT_depthRangeIndexed));
    Nan::SetTemplate(exports, "getFloati_v", Nan::New<v8::FunctionTemplate>(EXPORT_getFloati_v));
    Nan::SetTemplate(exports, "getDoublei_v", Nan::New<v8::FunctionTemplate>(EXPORT_getDoublei_v));
    Nan::SetTemplate(exports, "shaderSource", Nan::New<v8::FunctionTemplate>(EXPORT_shaderSource));
}


void gl3BindInit(v8::Handle<v8::Object> exports) {
    Nan::HandleScope scope;
    v8::Handle<v8::ObjectTemplate> GL3 = Nan::New<v8::ObjectTemplate>();
    GL3->SetInternalFieldCount(1);
    defineConstants(GL3);
    defineFunctions(GL3);
    defineObjects(GL3);
    Nan::Set(exports, Nan::New<v8::String>("GL3").ToLocalChecked(), GL3->NewInstance());
}
