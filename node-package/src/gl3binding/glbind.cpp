#include "glbind.h"

void do_nothing_release_callback(char*, void*) { }
void* glbind_get_buffer_data(v8::Handle<v8::Value> value) {
    if(value->IsArrayBufferView()) {
        v8::ArrayBufferView* view = v8::ArrayBufferView::Cast(*value);
        unsigned char* ptr = (unsigned char*)view->Buffer()->Externalize().Data();
        ptr += view->ByteOffset();
        // This is because ArrayBuffer::GetContents is not available yet (v8 version too low).
        // Here, we use Externalize, by not deallocating the memory here, the client side is still able to access it.
        // However, the GC won't collect this memory, so it's leaked.
        // Use this: new Buffer(new Uint8Array(fa.buffer))
        fprintf(stderr, "Warning: memory leak occurred. This problem is currently unsolvable, please avoid using ArrayBuffers with GL3 binding functions.\n");
        return ptr;
    } else if(node::Buffer::HasInstance(value)) {
        return node::Buffer::Data(value);
    } else {
        return NULL;
    }
}


class NODE_Sampler : public node::ObjectWrap {
public:
    static void Init(v8::Handle<v8::ObjectTemplate> exports);
    GLuint gl_handle;

    static v8::Handle<v8::Value> fromGLHandle(GLuint handle) {
        const int argc = 1;
        v8::Local<v8::Value> argv[argc] = { NanNew<v8::Integer>(handle) };
        return NanNew(constructor)->NewInstance(argc, argv);
    }

private:
    explicit NODE_Sampler(GLuint gl_handle_) { gl_handle = gl_handle_; }
    explicit NODE_Sampler() { glGenSamplers(1, &gl_handle); }
    ~NODE_Sampler() { glDeleteSamplers(1, &gl_handle); }

    static NAN_METHOD(New);

    static NAN_METHOD(NODE_id);
    static NAN_METHOD(NODE_delete);
    static NAN_METHOD(NODE_toString);

    static v8::Persistent<v8::Function> constructor;
};

v8::Persistent<v8::Function> NODE_Sampler::constructor;

void NODE_Sampler::Init(v8::Handle<v8::ObjectTemplate> exports) {
    v8::Local<v8::FunctionTemplate> tpl = NanNew<v8::FunctionTemplate>(New);
    tpl->SetClassName(NanNew<v8::String>("Sampler"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    NODE_SET_PROTOTYPE_METHOD(tpl, "id", NODE_id);
    NODE_SET_PROTOTYPE_METHOD(tpl, "toString", NODE_toString);
    NODE_SET_PROTOTYPE_METHOD(tpl, "inspect", NODE_toString);

    NODE_SET_PROTOTYPE_METHOD(tpl, "delete", NODE_delete);

    NanAssignPersistent(constructor, tpl->GetFunction());

    // Export constructor.
    exports->Set(NanNew<v8::String>("Sampler"), tpl->GetFunction());
}

NAN_METHOD(NODE_Sampler::New) {
    NanScope();

    if(args.IsConstructCall()) {
        if(args.Length() == 0) {
            NODE_Sampler* obj = new NODE_Sampler();
            obj->Wrap(args.This());
            NanReturnThis();
        } else {
            NODE_Sampler* obj = new NODE_Sampler(args[0]->Uint32Value());
            obj->Wrap(args.This());
            NanReturnThis();
        }
    } else {
        // Invoked as plain function `MyObject(...)`, turn into construct call.
        const int argc = 1;
        v8::Local<v8::Value> argv[argc] = { args[0] };
        NanReturnValue(NanNew(constructor)->NewInstance(argc, argv));
    }
}

NAN_METHOD(NODE_Sampler::NODE_id) {
    NODE_Sampler* obj = ObjectWrap::Unwrap<NODE_Sampler>(args.This());
    NanReturnValue(NanNew<v8::Uint32>(obj->gl_handle));
}

NAN_METHOD(NODE_Sampler::NODE_delete) {
    NODE_Sampler* obj = ObjectWrap::Unwrap<NODE_Sampler>(args.This());
    GLuint gl_handle = obj->gl_handle;
    glDeleteSamplers(1, &gl_handle);
    obj->gl_handle = 0;
    NanReturnUndefined();
}

NAN_METHOD(NODE_Sampler::NODE_toString) {
    NODE_Sampler* obj = ObjectWrap::Unwrap<NODE_Sampler>(args.This());
    char buf[64];
    sprintf(buf, "Sampler:%d", obj->gl_handle);
    NanReturnValue(NanNew<v8::String>(buf));
}


class NODE_Framebuffer : public node::ObjectWrap {
public:
    static void Init(v8::Handle<v8::ObjectTemplate> exports);
    GLuint gl_handle;

    static v8::Handle<v8::Value> fromGLHandle(GLuint handle) {
        const int argc = 1;
        v8::Local<v8::Value> argv[argc] = { NanNew<v8::Integer>(handle) };
        return NanNew(constructor)->NewInstance(argc, argv);
    }

private:
    explicit NODE_Framebuffer(GLuint gl_handle_) { gl_handle = gl_handle_; }
    explicit NODE_Framebuffer() { glGenFramebuffers(1, &gl_handle); }
    ~NODE_Framebuffer() { glDeleteFramebuffers(1, &gl_handle); }

    static NAN_METHOD(New);

    static NAN_METHOD(NODE_id);
    static NAN_METHOD(NODE_delete);
    static NAN_METHOD(NODE_toString);

    static v8::Persistent<v8::Function> constructor;
};

v8::Persistent<v8::Function> NODE_Framebuffer::constructor;

void NODE_Framebuffer::Init(v8::Handle<v8::ObjectTemplate> exports) {
    v8::Local<v8::FunctionTemplate> tpl = NanNew<v8::FunctionTemplate>(New);
    tpl->SetClassName(NanNew<v8::String>("Framebuffer"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    NODE_SET_PROTOTYPE_METHOD(tpl, "id", NODE_id);
    NODE_SET_PROTOTYPE_METHOD(tpl, "toString", NODE_toString);
    NODE_SET_PROTOTYPE_METHOD(tpl, "inspect", NODE_toString);

    NODE_SET_PROTOTYPE_METHOD(tpl, "delete", NODE_delete);

    NanAssignPersistent(constructor, tpl->GetFunction());

    // Export constructor.
    exports->Set(NanNew<v8::String>("Framebuffer"), tpl->GetFunction());
}

NAN_METHOD(NODE_Framebuffer::New) {
    NanScope();

    if(args.IsConstructCall()) {
        if(args.Length() == 0) {
            NODE_Framebuffer* obj = new NODE_Framebuffer();
            obj->Wrap(args.This());
            NanReturnThis();
        } else {
            NODE_Framebuffer* obj = new NODE_Framebuffer(args[0]->Uint32Value());
            obj->Wrap(args.This());
            NanReturnThis();
        }
    } else {
        // Invoked as plain function `MyObject(...)`, turn into construct call.
        const int argc = 1;
        v8::Local<v8::Value> argv[argc] = { args[0] };
        NanReturnValue(NanNew(constructor)->NewInstance(argc, argv));
    }
}

NAN_METHOD(NODE_Framebuffer::NODE_id) {
    NODE_Framebuffer* obj = ObjectWrap::Unwrap<NODE_Framebuffer>(args.This());
    NanReturnValue(NanNew<v8::Uint32>(obj->gl_handle));
}

NAN_METHOD(NODE_Framebuffer::NODE_delete) {
    NODE_Framebuffer* obj = ObjectWrap::Unwrap<NODE_Framebuffer>(args.This());
    GLuint gl_handle = obj->gl_handle;
    glDeleteFramebuffers(1, &gl_handle);
    obj->gl_handle = 0;
    NanReturnUndefined();
}

NAN_METHOD(NODE_Framebuffer::NODE_toString) {
    NODE_Framebuffer* obj = ObjectWrap::Unwrap<NODE_Framebuffer>(args.This());
    char buf[64];
    sprintf(buf, "Framebuffer:%d", obj->gl_handle);
    NanReturnValue(NanNew<v8::String>(buf));
}


class NODE_Shader : public node::ObjectWrap {
public:
    static void Init(v8::Handle<v8::ObjectTemplate> exports);
    GLuint gl_handle;

    static v8::Handle<v8::Value> fromGLHandle(GLuint handle) {
        const int argc = 1;
        v8::Local<v8::Value> argv[argc] = { NanNew<v8::Integer>(handle) };
        return NanNew(constructor)->NewInstance(argc, argv);
    }

private:
    explicit NODE_Shader(GLuint gl_handle_) { gl_handle = gl_handle_; }
    explicit NODE_Shader() { gl_handle = 0; }
    ~NODE_Shader() { glDeleteShader(gl_handle); }

    static NAN_METHOD(New);

    static NAN_METHOD(NODE_id);
    static NAN_METHOD(NODE_delete);
    static NAN_METHOD(NODE_toString);

    static v8::Persistent<v8::Function> constructor;
};

v8::Persistent<v8::Function> NODE_Shader::constructor;

void NODE_Shader::Init(v8::Handle<v8::ObjectTemplate> exports) {
    v8::Local<v8::FunctionTemplate> tpl = NanNew<v8::FunctionTemplate>(New);
    tpl->SetClassName(NanNew<v8::String>("Shader"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    NODE_SET_PROTOTYPE_METHOD(tpl, "id", NODE_id);
    NODE_SET_PROTOTYPE_METHOD(tpl, "toString", NODE_toString);
    NODE_SET_PROTOTYPE_METHOD(tpl, "inspect", NODE_toString);

    NODE_SET_PROTOTYPE_METHOD(tpl, "delete", NODE_delete);

    NanAssignPersistent(constructor, tpl->GetFunction());

    // Export constructor.
    exports->Set(NanNew<v8::String>("Shader"), tpl->GetFunction());
}

NAN_METHOD(NODE_Shader::New) {
    NanScope();

    if(args.IsConstructCall()) {
        if(args.Length() == 0) {
            NODE_Shader* obj = new NODE_Shader();
            obj->Wrap(args.This());
            NanReturnThis();
        } else {
            NODE_Shader* obj = new NODE_Shader(args[0]->Uint32Value());
            obj->Wrap(args.This());
            NanReturnThis();
        }
    } else {
        // Invoked as plain function `MyObject(...)`, turn into construct call.
        const int argc = 1;
        v8::Local<v8::Value> argv[argc] = { args[0] };
        NanReturnValue(NanNew(constructor)->NewInstance(argc, argv));
    }
}

NAN_METHOD(NODE_Shader::NODE_id) {
    NODE_Shader* obj = ObjectWrap::Unwrap<NODE_Shader>(args.This());
    NanReturnValue(NanNew<v8::Uint32>(obj->gl_handle));
}

NAN_METHOD(NODE_Shader::NODE_delete) {
    NODE_Shader* obj = ObjectWrap::Unwrap<NODE_Shader>(args.This());
    GLuint gl_handle = obj->gl_handle;
    glDeleteShader(gl_handle);
    obj->gl_handle = 0;
    NanReturnUndefined();
}

NAN_METHOD(NODE_Shader::NODE_toString) {
    NODE_Shader* obj = ObjectWrap::Unwrap<NODE_Shader>(args.This());
    char buf[64];
    sprintf(buf, "Shader:%d", obj->gl_handle);
    NanReturnValue(NanNew<v8::String>(buf));
}


class NODE_Texture : public node::ObjectWrap {
public:
    static void Init(v8::Handle<v8::ObjectTemplate> exports);
    GLuint gl_handle;

    static v8::Handle<v8::Value> fromGLHandle(GLuint handle) {
        const int argc = 1;
        v8::Local<v8::Value> argv[argc] = { NanNew<v8::Integer>(handle) };
        return NanNew(constructor)->NewInstance(argc, argv);
    }

private:
    explicit NODE_Texture(GLuint gl_handle_) { gl_handle = gl_handle_; }
    explicit NODE_Texture() { glGenTextures(1, &gl_handle); }
    ~NODE_Texture() { glDeleteTextures(1, &gl_handle); }

    static NAN_METHOD(New);

    static NAN_METHOD(NODE_id);
    static NAN_METHOD(NODE_delete);
    static NAN_METHOD(NODE_toString);

    static v8::Persistent<v8::Function> constructor;
};

v8::Persistent<v8::Function> NODE_Texture::constructor;

void NODE_Texture::Init(v8::Handle<v8::ObjectTemplate> exports) {
    v8::Local<v8::FunctionTemplate> tpl = NanNew<v8::FunctionTemplate>(New);
    tpl->SetClassName(NanNew<v8::String>("Texture"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    NODE_SET_PROTOTYPE_METHOD(tpl, "id", NODE_id);
    NODE_SET_PROTOTYPE_METHOD(tpl, "toString", NODE_toString);
    NODE_SET_PROTOTYPE_METHOD(tpl, "inspect", NODE_toString);

    NODE_SET_PROTOTYPE_METHOD(tpl, "delete", NODE_delete);

    NanAssignPersistent(constructor, tpl->GetFunction());

    // Export constructor.
    exports->Set(NanNew<v8::String>("Texture"), tpl->GetFunction());
}

NAN_METHOD(NODE_Texture::New) {
    NanScope();

    if(args.IsConstructCall()) {
        if(args.Length() == 0) {
            NODE_Texture* obj = new NODE_Texture();
            obj->Wrap(args.This());
            NanReturnThis();
        } else {
            NODE_Texture* obj = new NODE_Texture(args[0]->Uint32Value());
            obj->Wrap(args.This());
            NanReturnThis();
        }
    } else {
        // Invoked as plain function `MyObject(...)`, turn into construct call.
        const int argc = 1;
        v8::Local<v8::Value> argv[argc] = { args[0] };
        NanReturnValue(NanNew(constructor)->NewInstance(argc, argv));
    }
}

NAN_METHOD(NODE_Texture::NODE_id) {
    NODE_Texture* obj = ObjectWrap::Unwrap<NODE_Texture>(args.This());
    NanReturnValue(NanNew<v8::Uint32>(obj->gl_handle));
}

NAN_METHOD(NODE_Texture::NODE_delete) {
    NODE_Texture* obj = ObjectWrap::Unwrap<NODE_Texture>(args.This());
    GLuint gl_handle = obj->gl_handle;
    glDeleteTextures(1, &gl_handle);
    obj->gl_handle = 0;
    NanReturnUndefined();
}

NAN_METHOD(NODE_Texture::NODE_toString) {
    NODE_Texture* obj = ObjectWrap::Unwrap<NODE_Texture>(args.This());
    char buf[64];
    sprintf(buf, "Texture:%d", obj->gl_handle);
    NanReturnValue(NanNew<v8::String>(buf));
}


class NODE_ProgramPipeline : public node::ObjectWrap {
public:
    static void Init(v8::Handle<v8::ObjectTemplate> exports);
    GLuint gl_handle;

    static v8::Handle<v8::Value> fromGLHandle(GLuint handle) {
        const int argc = 1;
        v8::Local<v8::Value> argv[argc] = { NanNew<v8::Integer>(handle) };
        return NanNew(constructor)->NewInstance(argc, argv);
    }

private:
    explicit NODE_ProgramPipeline(GLuint gl_handle_) { gl_handle = gl_handle_; }
    explicit NODE_ProgramPipeline() { glGenProgramPipelines(1, &gl_handle); }
    ~NODE_ProgramPipeline() { glDeleteProgramPipelines(1, &gl_handle); }

    static NAN_METHOD(New);

    static NAN_METHOD(NODE_id);
    static NAN_METHOD(NODE_delete);
    static NAN_METHOD(NODE_toString);

    static v8::Persistent<v8::Function> constructor;
};

v8::Persistent<v8::Function> NODE_ProgramPipeline::constructor;

void NODE_ProgramPipeline::Init(v8::Handle<v8::ObjectTemplate> exports) {
    v8::Local<v8::FunctionTemplate> tpl = NanNew<v8::FunctionTemplate>(New);
    tpl->SetClassName(NanNew<v8::String>("ProgramPipeline"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    NODE_SET_PROTOTYPE_METHOD(tpl, "id", NODE_id);
    NODE_SET_PROTOTYPE_METHOD(tpl, "toString", NODE_toString);
    NODE_SET_PROTOTYPE_METHOD(tpl, "inspect", NODE_toString);

    NODE_SET_PROTOTYPE_METHOD(tpl, "delete", NODE_delete);

    NanAssignPersistent(constructor, tpl->GetFunction());

    // Export constructor.
    exports->Set(NanNew<v8::String>("ProgramPipeline"), tpl->GetFunction());
}

NAN_METHOD(NODE_ProgramPipeline::New) {
    NanScope();

    if(args.IsConstructCall()) {
        if(args.Length() == 0) {
            NODE_ProgramPipeline* obj = new NODE_ProgramPipeline();
            obj->Wrap(args.This());
            NanReturnThis();
        } else {
            NODE_ProgramPipeline* obj = new NODE_ProgramPipeline(args[0]->Uint32Value());
            obj->Wrap(args.This());
            NanReturnThis();
        }
    } else {
        // Invoked as plain function `MyObject(...)`, turn into construct call.
        const int argc = 1;
        v8::Local<v8::Value> argv[argc] = { args[0] };
        NanReturnValue(NanNew(constructor)->NewInstance(argc, argv));
    }
}

NAN_METHOD(NODE_ProgramPipeline::NODE_id) {
    NODE_ProgramPipeline* obj = ObjectWrap::Unwrap<NODE_ProgramPipeline>(args.This());
    NanReturnValue(NanNew<v8::Uint32>(obj->gl_handle));
}

NAN_METHOD(NODE_ProgramPipeline::NODE_delete) {
    NODE_ProgramPipeline* obj = ObjectWrap::Unwrap<NODE_ProgramPipeline>(args.This());
    GLuint gl_handle = obj->gl_handle;
    glDeleteProgramPipelines(1, &gl_handle);
    obj->gl_handle = 0;
    NanReturnUndefined();
}

NAN_METHOD(NODE_ProgramPipeline::NODE_toString) {
    NODE_ProgramPipeline* obj = ObjectWrap::Unwrap<NODE_ProgramPipeline>(args.This());
    char buf[64];
    sprintf(buf, "ProgramPipeline:%d", obj->gl_handle);
    NanReturnValue(NanNew<v8::String>(buf));
}


class NODE_Renderbuffer : public node::ObjectWrap {
public:
    static void Init(v8::Handle<v8::ObjectTemplate> exports);
    GLuint gl_handle;

    static v8::Handle<v8::Value> fromGLHandle(GLuint handle) {
        const int argc = 1;
        v8::Local<v8::Value> argv[argc] = { NanNew<v8::Integer>(handle) };
        return NanNew(constructor)->NewInstance(argc, argv);
    }

private:
    explicit NODE_Renderbuffer(GLuint gl_handle_) { gl_handle = gl_handle_; }
    explicit NODE_Renderbuffer() { glGenRenderbuffers(1, &gl_handle); }
    ~NODE_Renderbuffer() { glDeleteRenderbuffers(1, &gl_handle); }

    static NAN_METHOD(New);

    static NAN_METHOD(NODE_id);
    static NAN_METHOD(NODE_delete);
    static NAN_METHOD(NODE_toString);

    static v8::Persistent<v8::Function> constructor;
};

v8::Persistent<v8::Function> NODE_Renderbuffer::constructor;

void NODE_Renderbuffer::Init(v8::Handle<v8::ObjectTemplate> exports) {
    v8::Local<v8::FunctionTemplate> tpl = NanNew<v8::FunctionTemplate>(New);
    tpl->SetClassName(NanNew<v8::String>("Renderbuffer"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    NODE_SET_PROTOTYPE_METHOD(tpl, "id", NODE_id);
    NODE_SET_PROTOTYPE_METHOD(tpl, "toString", NODE_toString);
    NODE_SET_PROTOTYPE_METHOD(tpl, "inspect", NODE_toString);

    NODE_SET_PROTOTYPE_METHOD(tpl, "delete", NODE_delete);

    NanAssignPersistent(constructor, tpl->GetFunction());

    // Export constructor.
    exports->Set(NanNew<v8::String>("Renderbuffer"), tpl->GetFunction());
}

NAN_METHOD(NODE_Renderbuffer::New) {
    NanScope();

    if(args.IsConstructCall()) {
        if(args.Length() == 0) {
            NODE_Renderbuffer* obj = new NODE_Renderbuffer();
            obj->Wrap(args.This());
            NanReturnThis();
        } else {
            NODE_Renderbuffer* obj = new NODE_Renderbuffer(args[0]->Uint32Value());
            obj->Wrap(args.This());
            NanReturnThis();
        }
    } else {
        // Invoked as plain function `MyObject(...)`, turn into construct call.
        const int argc = 1;
        v8::Local<v8::Value> argv[argc] = { args[0] };
        NanReturnValue(NanNew(constructor)->NewInstance(argc, argv));
    }
}

NAN_METHOD(NODE_Renderbuffer::NODE_id) {
    NODE_Renderbuffer* obj = ObjectWrap::Unwrap<NODE_Renderbuffer>(args.This());
    NanReturnValue(NanNew<v8::Uint32>(obj->gl_handle));
}

NAN_METHOD(NODE_Renderbuffer::NODE_delete) {
    NODE_Renderbuffer* obj = ObjectWrap::Unwrap<NODE_Renderbuffer>(args.This());
    GLuint gl_handle = obj->gl_handle;
    glDeleteRenderbuffers(1, &gl_handle);
    obj->gl_handle = 0;
    NanReturnUndefined();
}

NAN_METHOD(NODE_Renderbuffer::NODE_toString) {
    NODE_Renderbuffer* obj = ObjectWrap::Unwrap<NODE_Renderbuffer>(args.This());
    char buf[64];
    sprintf(buf, "Renderbuffer:%d", obj->gl_handle);
    NanReturnValue(NanNew<v8::String>(buf));
}


class NODE_VertexArray : public node::ObjectWrap {
public:
    static void Init(v8::Handle<v8::ObjectTemplate> exports);
    GLuint gl_handle;

    static v8::Handle<v8::Value> fromGLHandle(GLuint handle) {
        const int argc = 1;
        v8::Local<v8::Value> argv[argc] = { NanNew<v8::Integer>(handle) };
        return NanNew(constructor)->NewInstance(argc, argv);
    }

private:
    explicit NODE_VertexArray(GLuint gl_handle_) { gl_handle = gl_handle_; }
    explicit NODE_VertexArray() { glGenVertexArrays(1, &gl_handle); }
    ~NODE_VertexArray() { glDeleteVertexArrays(1, &gl_handle); }

    static NAN_METHOD(New);

    static NAN_METHOD(NODE_id);
    static NAN_METHOD(NODE_delete);
    static NAN_METHOD(NODE_toString);

    static v8::Persistent<v8::Function> constructor;
};

v8::Persistent<v8::Function> NODE_VertexArray::constructor;

void NODE_VertexArray::Init(v8::Handle<v8::ObjectTemplate> exports) {
    v8::Local<v8::FunctionTemplate> tpl = NanNew<v8::FunctionTemplate>(New);
    tpl->SetClassName(NanNew<v8::String>("VertexArray"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    NODE_SET_PROTOTYPE_METHOD(tpl, "id", NODE_id);
    NODE_SET_PROTOTYPE_METHOD(tpl, "toString", NODE_toString);
    NODE_SET_PROTOTYPE_METHOD(tpl, "inspect", NODE_toString);

    NODE_SET_PROTOTYPE_METHOD(tpl, "delete", NODE_delete);

    NanAssignPersistent(constructor, tpl->GetFunction());

    // Export constructor.
    exports->Set(NanNew<v8::String>("VertexArray"), tpl->GetFunction());
}

NAN_METHOD(NODE_VertexArray::New) {
    NanScope();

    if(args.IsConstructCall()) {
        if(args.Length() == 0) {
            NODE_VertexArray* obj = new NODE_VertexArray();
            obj->Wrap(args.This());
            NanReturnThis();
        } else {
            NODE_VertexArray* obj = new NODE_VertexArray(args[0]->Uint32Value());
            obj->Wrap(args.This());
            NanReturnThis();
        }
    } else {
        // Invoked as plain function `MyObject(...)`, turn into construct call.
        const int argc = 1;
        v8::Local<v8::Value> argv[argc] = { args[0] };
        NanReturnValue(NanNew(constructor)->NewInstance(argc, argv));
    }
}

NAN_METHOD(NODE_VertexArray::NODE_id) {
    NODE_VertexArray* obj = ObjectWrap::Unwrap<NODE_VertexArray>(args.This());
    NanReturnValue(NanNew<v8::Uint32>(obj->gl_handle));
}

NAN_METHOD(NODE_VertexArray::NODE_delete) {
    NODE_VertexArray* obj = ObjectWrap::Unwrap<NODE_VertexArray>(args.This());
    GLuint gl_handle = obj->gl_handle;
    glDeleteVertexArrays(1, &gl_handle);
    obj->gl_handle = 0;
    NanReturnUndefined();
}

NAN_METHOD(NODE_VertexArray::NODE_toString) {
    NODE_VertexArray* obj = ObjectWrap::Unwrap<NODE_VertexArray>(args.This());
    char buf[64];
    sprintf(buf, "VertexArray:%d", obj->gl_handle);
    NanReturnValue(NanNew<v8::String>(buf));
}


class NODE_Query : public node::ObjectWrap {
public:
    static void Init(v8::Handle<v8::ObjectTemplate> exports);
    GLuint gl_handle;

    static v8::Handle<v8::Value> fromGLHandle(GLuint handle) {
        const int argc = 1;
        v8::Local<v8::Value> argv[argc] = { NanNew<v8::Integer>(handle) };
        return NanNew(constructor)->NewInstance(argc, argv);
    }

private:
    explicit NODE_Query(GLuint gl_handle_) { gl_handle = gl_handle_; }
    explicit NODE_Query() { glGenQueries(1, &gl_handle); }
    ~NODE_Query() { glDeleteQueries(1, &gl_handle); }

    static NAN_METHOD(New);

    static NAN_METHOD(NODE_id);
    static NAN_METHOD(NODE_delete);
    static NAN_METHOD(NODE_toString);

    static v8::Persistent<v8::Function> constructor;
};

v8::Persistent<v8::Function> NODE_Query::constructor;

void NODE_Query::Init(v8::Handle<v8::ObjectTemplate> exports) {
    v8::Local<v8::FunctionTemplate> tpl = NanNew<v8::FunctionTemplate>(New);
    tpl->SetClassName(NanNew<v8::String>("Query"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    NODE_SET_PROTOTYPE_METHOD(tpl, "id", NODE_id);
    NODE_SET_PROTOTYPE_METHOD(tpl, "toString", NODE_toString);
    NODE_SET_PROTOTYPE_METHOD(tpl, "inspect", NODE_toString);

    NODE_SET_PROTOTYPE_METHOD(tpl, "delete", NODE_delete);

    NanAssignPersistent(constructor, tpl->GetFunction());

    // Export constructor.
    exports->Set(NanNew<v8::String>("Query"), tpl->GetFunction());
}

NAN_METHOD(NODE_Query::New) {
    NanScope();

    if(args.IsConstructCall()) {
        if(args.Length() == 0) {
            NODE_Query* obj = new NODE_Query();
            obj->Wrap(args.This());
            NanReturnThis();
        } else {
            NODE_Query* obj = new NODE_Query(args[0]->Uint32Value());
            obj->Wrap(args.This());
            NanReturnThis();
        }
    } else {
        // Invoked as plain function `MyObject(...)`, turn into construct call.
        const int argc = 1;
        v8::Local<v8::Value> argv[argc] = { args[0] };
        NanReturnValue(NanNew(constructor)->NewInstance(argc, argv));
    }
}

NAN_METHOD(NODE_Query::NODE_id) {
    NODE_Query* obj = ObjectWrap::Unwrap<NODE_Query>(args.This());
    NanReturnValue(NanNew<v8::Uint32>(obj->gl_handle));
}

NAN_METHOD(NODE_Query::NODE_delete) {
    NODE_Query* obj = ObjectWrap::Unwrap<NODE_Query>(args.This());
    GLuint gl_handle = obj->gl_handle;
    glDeleteQueries(1, &gl_handle);
    obj->gl_handle = 0;
    NanReturnUndefined();
}

NAN_METHOD(NODE_Query::NODE_toString) {
    NODE_Query* obj = ObjectWrap::Unwrap<NODE_Query>(args.This());
    char buf[64];
    sprintf(buf, "Query:%d", obj->gl_handle);
    NanReturnValue(NanNew<v8::String>(buf));
}


class NODE_Buffer : public node::ObjectWrap {
public:
    static void Init(v8::Handle<v8::ObjectTemplate> exports);
    GLuint gl_handle;

    static v8::Handle<v8::Value> fromGLHandle(GLuint handle) {
        const int argc = 1;
        v8::Local<v8::Value> argv[argc] = { NanNew<v8::Integer>(handle) };
        return NanNew(constructor)->NewInstance(argc, argv);
    }

private:
    explicit NODE_Buffer(GLuint gl_handle_) { gl_handle = gl_handle_; }
    explicit NODE_Buffer() { glGenBuffers(1, &gl_handle); }
    ~NODE_Buffer() { glDeleteBuffers(1, &gl_handle); }

    static NAN_METHOD(New);

    static NAN_METHOD(NODE_id);
    static NAN_METHOD(NODE_delete);
    static NAN_METHOD(NODE_toString);

    static v8::Persistent<v8::Function> constructor;
};

v8::Persistent<v8::Function> NODE_Buffer::constructor;

void NODE_Buffer::Init(v8::Handle<v8::ObjectTemplate> exports) {
    v8::Local<v8::FunctionTemplate> tpl = NanNew<v8::FunctionTemplate>(New);
    tpl->SetClassName(NanNew<v8::String>("Buffer"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    NODE_SET_PROTOTYPE_METHOD(tpl, "id", NODE_id);
    NODE_SET_PROTOTYPE_METHOD(tpl, "toString", NODE_toString);
    NODE_SET_PROTOTYPE_METHOD(tpl, "inspect", NODE_toString);

    NODE_SET_PROTOTYPE_METHOD(tpl, "delete", NODE_delete);

    NanAssignPersistent(constructor, tpl->GetFunction());

    // Export constructor.
    exports->Set(NanNew<v8::String>("Buffer"), tpl->GetFunction());
}

NAN_METHOD(NODE_Buffer::New) {
    NanScope();

    if(args.IsConstructCall()) {
        if(args.Length() == 0) {
            NODE_Buffer* obj = new NODE_Buffer();
            obj->Wrap(args.This());
            NanReturnThis();
        } else {
            NODE_Buffer* obj = new NODE_Buffer(args[0]->Uint32Value());
            obj->Wrap(args.This());
            NanReturnThis();
        }
    } else {
        // Invoked as plain function `MyObject(...)`, turn into construct call.
        const int argc = 1;
        v8::Local<v8::Value> argv[argc] = { args[0] };
        NanReturnValue(NanNew(constructor)->NewInstance(argc, argv));
    }
}

NAN_METHOD(NODE_Buffer::NODE_id) {
    NODE_Buffer* obj = ObjectWrap::Unwrap<NODE_Buffer>(args.This());
    NanReturnValue(NanNew<v8::Uint32>(obj->gl_handle));
}

NAN_METHOD(NODE_Buffer::NODE_delete) {
    NODE_Buffer* obj = ObjectWrap::Unwrap<NODE_Buffer>(args.This());
    GLuint gl_handle = obj->gl_handle;
    glDeleteBuffers(1, &gl_handle);
    obj->gl_handle = 0;
    NanReturnUndefined();
}

NAN_METHOD(NODE_Buffer::NODE_toString) {
    NODE_Buffer* obj = ObjectWrap::Unwrap<NODE_Buffer>(args.This());
    char buf[64];
    sprintf(buf, "Buffer:%d", obj->gl_handle);
    NanReturnValue(NanNew<v8::String>(buf));
}


class NODE_Program : public node::ObjectWrap {
public:
    static void Init(v8::Handle<v8::ObjectTemplate> exports);
    GLuint gl_handle;

    static v8::Handle<v8::Value> fromGLHandle(GLuint handle) {
        const int argc = 1;
        v8::Local<v8::Value> argv[argc] = { NanNew<v8::Integer>(handle) };
        return NanNew(constructor)->NewInstance(argc, argv);
    }

private:
    explicit NODE_Program(GLuint gl_handle_) { gl_handle = gl_handle_; }
    explicit NODE_Program() { gl_handle = 0; }
    ~NODE_Program() { glDeleteProgram(gl_handle); }

    static NAN_METHOD(New);

    static NAN_METHOD(NODE_id);
    static NAN_METHOD(NODE_delete);
    static NAN_METHOD(NODE_toString);

    static v8::Persistent<v8::Function> constructor;
};

v8::Persistent<v8::Function> NODE_Program::constructor;

void NODE_Program::Init(v8::Handle<v8::ObjectTemplate> exports) {
    v8::Local<v8::FunctionTemplate> tpl = NanNew<v8::FunctionTemplate>(New);
    tpl->SetClassName(NanNew<v8::String>("Program"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    NODE_SET_PROTOTYPE_METHOD(tpl, "id", NODE_id);
    NODE_SET_PROTOTYPE_METHOD(tpl, "toString", NODE_toString);
    NODE_SET_PROTOTYPE_METHOD(tpl, "inspect", NODE_toString);

    NODE_SET_PROTOTYPE_METHOD(tpl, "delete", NODE_delete);

    NanAssignPersistent(constructor, tpl->GetFunction());

    // Export constructor.
    exports->Set(NanNew<v8::String>("Program"), tpl->GetFunction());
}

NAN_METHOD(NODE_Program::New) {
    NanScope();

    if(args.IsConstructCall()) {
        if(args.Length() == 0) {
            NODE_Program* obj = new NODE_Program();
            obj->Wrap(args.This());
            NanReturnThis();
        } else {
            NODE_Program* obj = new NODE_Program(args[0]->Uint32Value());
            obj->Wrap(args.This());
            NanReturnThis();
        }
    } else {
        // Invoked as plain function `MyObject(...)`, turn into construct call.
        const int argc = 1;
        v8::Local<v8::Value> argv[argc] = { args[0] };
        NanReturnValue(NanNew(constructor)->NewInstance(argc, argv));
    }
}

NAN_METHOD(NODE_Program::NODE_id) {
    NODE_Program* obj = ObjectWrap::Unwrap<NODE_Program>(args.This());
    NanReturnValue(NanNew<v8::Uint32>(obj->gl_handle));
}

NAN_METHOD(NODE_Program::NODE_delete) {
    NODE_Program* obj = ObjectWrap::Unwrap<NODE_Program>(args.This());
    GLuint gl_handle = obj->gl_handle;
    glDeleteProgram(gl_handle);
    obj->gl_handle = 0;
    NanReturnUndefined();
}

NAN_METHOD(NODE_Program::NODE_toString) {
    NODE_Program* obj = ObjectWrap::Unwrap<NODE_Program>(args.This());
    char buf[64];
    sprintf(buf, "Program:%d", obj->gl_handle);
    NanReturnValue(NanNew<v8::String>(buf));
}


class NODE_TransformFeedback : public node::ObjectWrap {
public:
    static void Init(v8::Handle<v8::ObjectTemplate> exports);
    GLuint gl_handle;

    static v8::Handle<v8::Value> fromGLHandle(GLuint handle) {
        const int argc = 1;
        v8::Local<v8::Value> argv[argc] = { NanNew<v8::Integer>(handle) };
        return NanNew(constructor)->NewInstance(argc, argv);
    }

private:
    explicit NODE_TransformFeedback(GLuint gl_handle_) { gl_handle = gl_handle_; }
    explicit NODE_TransformFeedback() { glGenTransformFeedbacks(1, &gl_handle); }
    ~NODE_TransformFeedback() { glDeleteTransformFeedbacks(1, &gl_handle); }

    static NAN_METHOD(New);

    static NAN_METHOD(NODE_id);
    static NAN_METHOD(NODE_delete);
    static NAN_METHOD(NODE_toString);

    static v8::Persistent<v8::Function> constructor;
};

v8::Persistent<v8::Function> NODE_TransformFeedback::constructor;

void NODE_TransformFeedback::Init(v8::Handle<v8::ObjectTemplate> exports) {
    v8::Local<v8::FunctionTemplate> tpl = NanNew<v8::FunctionTemplate>(New);
    tpl->SetClassName(NanNew<v8::String>("TransformFeedback"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    NODE_SET_PROTOTYPE_METHOD(tpl, "id", NODE_id);
    NODE_SET_PROTOTYPE_METHOD(tpl, "toString", NODE_toString);
    NODE_SET_PROTOTYPE_METHOD(tpl, "inspect", NODE_toString);

    NODE_SET_PROTOTYPE_METHOD(tpl, "delete", NODE_delete);

    NanAssignPersistent(constructor, tpl->GetFunction());

    // Export constructor.
    exports->Set(NanNew<v8::String>("TransformFeedback"), tpl->GetFunction());
}

NAN_METHOD(NODE_TransformFeedback::New) {
    NanScope();

    if(args.IsConstructCall()) {
        if(args.Length() == 0) {
            NODE_TransformFeedback* obj = new NODE_TransformFeedback();
            obj->Wrap(args.This());
            NanReturnThis();
        } else {
            NODE_TransformFeedback* obj = new NODE_TransformFeedback(args[0]->Uint32Value());
            obj->Wrap(args.This());
            NanReturnThis();
        }
    } else {
        // Invoked as plain function `MyObject(...)`, turn into construct call.
        const int argc = 1;
        v8::Local<v8::Value> argv[argc] = { args[0] };
        NanReturnValue(NanNew(constructor)->NewInstance(argc, argv));
    }
}

NAN_METHOD(NODE_TransformFeedback::NODE_id) {
    NODE_TransformFeedback* obj = ObjectWrap::Unwrap<NODE_TransformFeedback>(args.This());
    NanReturnValue(NanNew<v8::Uint32>(obj->gl_handle));
}

NAN_METHOD(NODE_TransformFeedback::NODE_delete) {
    NODE_TransformFeedback* obj = ObjectWrap::Unwrap<NODE_TransformFeedback>(args.This());
    GLuint gl_handle = obj->gl_handle;
    glDeleteTransformFeedbacks(1, &gl_handle);
    obj->gl_handle = 0;
    NanReturnUndefined();
}

NAN_METHOD(NODE_TransformFeedback::NODE_toString) {
    NODE_TransformFeedback* obj = ObjectWrap::Unwrap<NODE_TransformFeedback>(args.This());
    char buf[64];
    sprintf(buf, "TransformFeedback:%d", obj->gl_handle);
    NanReturnValue(NanNew<v8::String>(buf));
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
    exports->Set(NanNew<v8::String>("DEPTH_BUFFER_BIT"), NanNew<v8::Uint32>(GL_DEPTH_BUFFER_BIT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("STENCIL_BUFFER_BIT"), NanNew<v8::Uint32>(GL_STENCIL_BUFFER_BIT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("COLOR_BUFFER_BIT"), NanNew<v8::Uint32>(GL_COLOR_BUFFER_BIT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("FALSE"), NanNew<v8::Uint32>(GL_FALSE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TRUE"), NanNew<v8::Uint32>(GL_TRUE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("POINTS"), NanNew<v8::Uint32>(GL_POINTS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("LINES"), NanNew<v8::Uint32>(GL_LINES), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("LINE_LOOP"), NanNew<v8::Uint32>(GL_LINE_LOOP), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("LINE_STRIP"), NanNew<v8::Uint32>(GL_LINE_STRIP), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TRIANGLES"), NanNew<v8::Uint32>(GL_TRIANGLES), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TRIANGLE_STRIP"), NanNew<v8::Uint32>(GL_TRIANGLE_STRIP), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TRIANGLE_FAN"), NanNew<v8::Uint32>(GL_TRIANGLE_FAN), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("NEVER"), NanNew<v8::Uint32>(GL_NEVER), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("LESS"), NanNew<v8::Uint32>(GL_LESS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("EQUAL"), NanNew<v8::Uint32>(GL_EQUAL), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("LEQUAL"), NanNew<v8::Uint32>(GL_LEQUAL), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("GREATER"), NanNew<v8::Uint32>(GL_GREATER), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("NOTEQUAL"), NanNew<v8::Uint32>(GL_NOTEQUAL), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("GEQUAL"), NanNew<v8::Uint32>(GL_GEQUAL), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("ALWAYS"), NanNew<v8::Uint32>(GL_ALWAYS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("ZERO"), NanNew<v8::Uint32>(GL_ZERO), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("ONE"), NanNew<v8::Uint32>(GL_ONE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("SRC_COLOR"), NanNew<v8::Uint32>(GL_SRC_COLOR), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("ONE_MINUS_SRC_COLOR"), NanNew<v8::Uint32>(GL_ONE_MINUS_SRC_COLOR), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("SRC_ALPHA"), NanNew<v8::Uint32>(GL_SRC_ALPHA), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("ONE_MINUS_SRC_ALPHA"), NanNew<v8::Uint32>(GL_ONE_MINUS_SRC_ALPHA), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("DST_ALPHA"), NanNew<v8::Uint32>(GL_DST_ALPHA), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("ONE_MINUS_DST_ALPHA"), NanNew<v8::Uint32>(GL_ONE_MINUS_DST_ALPHA), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("DST_COLOR"), NanNew<v8::Uint32>(GL_DST_COLOR), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("ONE_MINUS_DST_COLOR"), NanNew<v8::Uint32>(GL_ONE_MINUS_DST_COLOR), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("SRC_ALPHA_SATURATE"), NanNew<v8::Uint32>(GL_SRC_ALPHA_SATURATE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("NONE"), NanNew<v8::Uint32>(GL_NONE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("FRONT_LEFT"), NanNew<v8::Uint32>(GL_FRONT_LEFT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("FRONT_RIGHT"), NanNew<v8::Uint32>(GL_FRONT_RIGHT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("BACK_LEFT"), NanNew<v8::Uint32>(GL_BACK_LEFT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("BACK_RIGHT"), NanNew<v8::Uint32>(GL_BACK_RIGHT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("FRONT"), NanNew<v8::Uint32>(GL_FRONT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("BACK"), NanNew<v8::Uint32>(GL_BACK), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("LEFT"), NanNew<v8::Uint32>(GL_LEFT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RIGHT"), NanNew<v8::Uint32>(GL_RIGHT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("FRONT_AND_BACK"), NanNew<v8::Uint32>(GL_FRONT_AND_BACK), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("NO_ERROR"), NanNew<v8::Uint32>(GL_NO_ERROR), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("INVALID_ENUM"), NanNew<v8::Uint32>(GL_INVALID_ENUM), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("INVALID_VALUE"), NanNew<v8::Uint32>(GL_INVALID_VALUE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("INVALID_OPERATION"), NanNew<v8::Uint32>(GL_INVALID_OPERATION), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("OUT_OF_MEMORY"), NanNew<v8::Uint32>(GL_OUT_OF_MEMORY), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("CW"), NanNew<v8::Uint32>(GL_CW), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("CCW"), NanNew<v8::Uint32>(GL_CCW), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("POINT_SIZE"), NanNew<v8::Uint32>(GL_POINT_SIZE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("POINT_SIZE_RANGE"), NanNew<v8::Uint32>(GL_POINT_SIZE_RANGE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("POINT_SIZE_GRANULARITY"), NanNew<v8::Uint32>(GL_POINT_SIZE_GRANULARITY), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("LINE_SMOOTH"), NanNew<v8::Uint32>(GL_LINE_SMOOTH), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("LINE_WIDTH"), NanNew<v8::Uint32>(GL_LINE_WIDTH), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("LINE_WIDTH_RANGE"), NanNew<v8::Uint32>(GL_LINE_WIDTH_RANGE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("LINE_WIDTH_GRANULARITY"), NanNew<v8::Uint32>(GL_LINE_WIDTH_GRANULARITY), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("POLYGON_MODE"), NanNew<v8::Uint32>(GL_POLYGON_MODE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("POLYGON_SMOOTH"), NanNew<v8::Uint32>(GL_POLYGON_SMOOTH), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("CULL_FACE"), NanNew<v8::Uint32>(GL_CULL_FACE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("CULL_FACE_MODE"), NanNew<v8::Uint32>(GL_CULL_FACE_MODE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("FRONT_FACE"), NanNew<v8::Uint32>(GL_FRONT_FACE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("DEPTH_RANGE"), NanNew<v8::Uint32>(GL_DEPTH_RANGE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("DEPTH_TEST"), NanNew<v8::Uint32>(GL_DEPTH_TEST), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("DEPTH_WRITEMASK"), NanNew<v8::Uint32>(GL_DEPTH_WRITEMASK), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("DEPTH_CLEAR_VALUE"), NanNew<v8::Uint32>(GL_DEPTH_CLEAR_VALUE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("DEPTH_FUNC"), NanNew<v8::Uint32>(GL_DEPTH_FUNC), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("STENCIL_TEST"), NanNew<v8::Uint32>(GL_STENCIL_TEST), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("STENCIL_CLEAR_VALUE"), NanNew<v8::Uint32>(GL_STENCIL_CLEAR_VALUE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("STENCIL_FUNC"), NanNew<v8::Uint32>(GL_STENCIL_FUNC), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("STENCIL_VALUE_MASK"), NanNew<v8::Uint32>(GL_STENCIL_VALUE_MASK), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("STENCIL_FAIL"), NanNew<v8::Uint32>(GL_STENCIL_FAIL), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("STENCIL_PASS_DEPTH_FAIL"), NanNew<v8::Uint32>(GL_STENCIL_PASS_DEPTH_FAIL), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("STENCIL_PASS_DEPTH_PASS"), NanNew<v8::Uint32>(GL_STENCIL_PASS_DEPTH_PASS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("STENCIL_REF"), NanNew<v8::Uint32>(GL_STENCIL_REF), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("STENCIL_WRITEMASK"), NanNew<v8::Uint32>(GL_STENCIL_WRITEMASK), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("VIEWPORT"), NanNew<v8::Uint32>(GL_VIEWPORT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("DITHER"), NanNew<v8::Uint32>(GL_DITHER), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("BLEND_DST"), NanNew<v8::Uint32>(GL_BLEND_DST), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("BLEND_SRC"), NanNew<v8::Uint32>(GL_BLEND_SRC), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("BLEND"), NanNew<v8::Uint32>(GL_BLEND), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("LOGIC_OP_MODE"), NanNew<v8::Uint32>(GL_LOGIC_OP_MODE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("COLOR_LOGIC_OP"), NanNew<v8::Uint32>(GL_COLOR_LOGIC_OP), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("DRAW_BUFFER"), NanNew<v8::Uint32>(GL_DRAW_BUFFER), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("READ_BUFFER"), NanNew<v8::Uint32>(GL_READ_BUFFER), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("SCISSOR_BOX"), NanNew<v8::Uint32>(GL_SCISSOR_BOX), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("SCISSOR_TEST"), NanNew<v8::Uint32>(GL_SCISSOR_TEST), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("COLOR_CLEAR_VALUE"), NanNew<v8::Uint32>(GL_COLOR_CLEAR_VALUE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("COLOR_WRITEMASK"), NanNew<v8::Uint32>(GL_COLOR_WRITEMASK), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("DOUBLEBUFFER"), NanNew<v8::Uint32>(GL_DOUBLEBUFFER), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("STEREO"), NanNew<v8::Uint32>(GL_STEREO), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("LINE_SMOOTH_HINT"), NanNew<v8::Uint32>(GL_LINE_SMOOTH_HINT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("POLYGON_SMOOTH_HINT"), NanNew<v8::Uint32>(GL_POLYGON_SMOOTH_HINT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNPACK_SWAP_BYTES"), NanNew<v8::Uint32>(GL_UNPACK_SWAP_BYTES), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNPACK_LSB_FIRST"), NanNew<v8::Uint32>(GL_UNPACK_LSB_FIRST), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNPACK_ROW_LENGTH"), NanNew<v8::Uint32>(GL_UNPACK_ROW_LENGTH), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNPACK_SKIP_ROWS"), NanNew<v8::Uint32>(GL_UNPACK_SKIP_ROWS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNPACK_SKIP_PIXELS"), NanNew<v8::Uint32>(GL_UNPACK_SKIP_PIXELS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNPACK_ALIGNMENT"), NanNew<v8::Uint32>(GL_UNPACK_ALIGNMENT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("PACK_SWAP_BYTES"), NanNew<v8::Uint32>(GL_PACK_SWAP_BYTES), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("PACK_LSB_FIRST"), NanNew<v8::Uint32>(GL_PACK_LSB_FIRST), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("PACK_ROW_LENGTH"), NanNew<v8::Uint32>(GL_PACK_ROW_LENGTH), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("PACK_SKIP_ROWS"), NanNew<v8::Uint32>(GL_PACK_SKIP_ROWS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("PACK_SKIP_PIXELS"), NanNew<v8::Uint32>(GL_PACK_SKIP_PIXELS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("PACK_ALIGNMENT"), NanNew<v8::Uint32>(GL_PACK_ALIGNMENT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_TEXTURE_SIZE"), NanNew<v8::Uint32>(GL_MAX_TEXTURE_SIZE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_VIEWPORT_DIMS"), NanNew<v8::Uint32>(GL_MAX_VIEWPORT_DIMS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("SUBPIXEL_BITS"), NanNew<v8::Uint32>(GL_SUBPIXEL_BITS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_1D"), NanNew<v8::Uint32>(GL_TEXTURE_1D), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_2D"), NanNew<v8::Uint32>(GL_TEXTURE_2D), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("POLYGON_OFFSET_UNITS"), NanNew<v8::Uint32>(GL_POLYGON_OFFSET_UNITS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("POLYGON_OFFSET_POINT"), NanNew<v8::Uint32>(GL_POLYGON_OFFSET_POINT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("POLYGON_OFFSET_LINE"), NanNew<v8::Uint32>(GL_POLYGON_OFFSET_LINE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("POLYGON_OFFSET_FILL"), NanNew<v8::Uint32>(GL_POLYGON_OFFSET_FILL), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("POLYGON_OFFSET_FACTOR"), NanNew<v8::Uint32>(GL_POLYGON_OFFSET_FACTOR), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_BINDING_1D"), NanNew<v8::Uint32>(GL_TEXTURE_BINDING_1D), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_BINDING_2D"), NanNew<v8::Uint32>(GL_TEXTURE_BINDING_2D), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_WIDTH"), NanNew<v8::Uint32>(GL_TEXTURE_WIDTH), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_HEIGHT"), NanNew<v8::Uint32>(GL_TEXTURE_HEIGHT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_INTERNAL_FORMAT"), NanNew<v8::Uint32>(GL_TEXTURE_INTERNAL_FORMAT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_BORDER_COLOR"), NanNew<v8::Uint32>(GL_TEXTURE_BORDER_COLOR), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_RED_SIZE"), NanNew<v8::Uint32>(GL_TEXTURE_RED_SIZE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_GREEN_SIZE"), NanNew<v8::Uint32>(GL_TEXTURE_GREEN_SIZE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_BLUE_SIZE"), NanNew<v8::Uint32>(GL_TEXTURE_BLUE_SIZE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_ALPHA_SIZE"), NanNew<v8::Uint32>(GL_TEXTURE_ALPHA_SIZE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("DONT_CARE"), NanNew<v8::Uint32>(GL_DONT_CARE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("FASTEST"), NanNew<v8::Uint32>(GL_FASTEST), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("NICEST"), NanNew<v8::Uint32>(GL_NICEST), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("BYTE"), NanNew<v8::Uint32>(GL_BYTE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNSIGNED_BYTE"), NanNew<v8::Uint32>(GL_UNSIGNED_BYTE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("SHORT"), NanNew<v8::Uint32>(GL_SHORT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNSIGNED_SHORT"), NanNew<v8::Uint32>(GL_UNSIGNED_SHORT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("INT"), NanNew<v8::Uint32>(GL_INT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNSIGNED_INT"), NanNew<v8::Uint32>(GL_UNSIGNED_INT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("FLOAT"), NanNew<v8::Uint32>(GL_FLOAT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("DOUBLE"), NanNew<v8::Uint32>(GL_DOUBLE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("CLEAR"), NanNew<v8::Uint32>(GL_CLEAR), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("AND"), NanNew<v8::Uint32>(GL_AND), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("AND_REVERSE"), NanNew<v8::Uint32>(GL_AND_REVERSE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("COPY"), NanNew<v8::Uint32>(GL_COPY), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("AND_INVERTED"), NanNew<v8::Uint32>(GL_AND_INVERTED), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("NOOP"), NanNew<v8::Uint32>(GL_NOOP), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("XOR"), NanNew<v8::Uint32>(GL_XOR), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("OR"), NanNew<v8::Uint32>(GL_OR), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("NOR"), NanNew<v8::Uint32>(GL_NOR), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("EQUIV"), NanNew<v8::Uint32>(GL_EQUIV), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("INVERT"), NanNew<v8::Uint32>(GL_INVERT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("OR_REVERSE"), NanNew<v8::Uint32>(GL_OR_REVERSE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("COPY_INVERTED"), NanNew<v8::Uint32>(GL_COPY_INVERTED), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("OR_INVERTED"), NanNew<v8::Uint32>(GL_OR_INVERTED), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("NAND"), NanNew<v8::Uint32>(GL_NAND), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("SET"), NanNew<v8::Uint32>(GL_SET), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE"), NanNew<v8::Uint32>(GL_TEXTURE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("COLOR"), NanNew<v8::Uint32>(GL_COLOR), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("DEPTH"), NanNew<v8::Uint32>(GL_DEPTH), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("STENCIL"), NanNew<v8::Uint32>(GL_STENCIL), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("STENCIL_INDEX"), NanNew<v8::Uint32>(GL_STENCIL_INDEX), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("DEPTH_COMPONENT"), NanNew<v8::Uint32>(GL_DEPTH_COMPONENT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RED"), NanNew<v8::Uint32>(GL_RED), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("GREEN"), NanNew<v8::Uint32>(GL_GREEN), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("BLUE"), NanNew<v8::Uint32>(GL_BLUE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("ALPHA"), NanNew<v8::Uint32>(GL_ALPHA), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RGB"), NanNew<v8::Uint32>(GL_RGB), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RGBA"), NanNew<v8::Uint32>(GL_RGBA), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("POINT"), NanNew<v8::Uint32>(GL_POINT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("LINE"), NanNew<v8::Uint32>(GL_LINE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("FILL"), NanNew<v8::Uint32>(GL_FILL), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("KEEP"), NanNew<v8::Uint32>(GL_KEEP), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("REPLACE"), NanNew<v8::Uint32>(GL_REPLACE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("INCR"), NanNew<v8::Uint32>(GL_INCR), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("DECR"), NanNew<v8::Uint32>(GL_DECR), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("VENDOR"), NanNew<v8::Uint32>(GL_VENDOR), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RENDERER"), NanNew<v8::Uint32>(GL_RENDERER), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("VERSION"), NanNew<v8::Uint32>(GL_VERSION), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("EXTENSIONS"), NanNew<v8::Uint32>(GL_EXTENSIONS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("NEAREST"), NanNew<v8::Uint32>(GL_NEAREST), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("LINEAR"), NanNew<v8::Uint32>(GL_LINEAR), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("NEAREST_MIPMAP_NEAREST"), NanNew<v8::Uint32>(GL_NEAREST_MIPMAP_NEAREST), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("LINEAR_MIPMAP_NEAREST"), NanNew<v8::Uint32>(GL_LINEAR_MIPMAP_NEAREST), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("NEAREST_MIPMAP_LINEAR"), NanNew<v8::Uint32>(GL_NEAREST_MIPMAP_LINEAR), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("LINEAR_MIPMAP_LINEAR"), NanNew<v8::Uint32>(GL_LINEAR_MIPMAP_LINEAR), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_MAG_FILTER"), NanNew<v8::Uint32>(GL_TEXTURE_MAG_FILTER), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_MIN_FILTER"), NanNew<v8::Uint32>(GL_TEXTURE_MIN_FILTER), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_WRAP_S"), NanNew<v8::Uint32>(GL_TEXTURE_WRAP_S), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_WRAP_T"), NanNew<v8::Uint32>(GL_TEXTURE_WRAP_T), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("PROXY_TEXTURE_1D"), NanNew<v8::Uint32>(GL_PROXY_TEXTURE_1D), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("PROXY_TEXTURE_2D"), NanNew<v8::Uint32>(GL_PROXY_TEXTURE_2D), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("REPEAT"), NanNew<v8::Uint32>(GL_REPEAT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("R3_G3_B2"), NanNew<v8::Uint32>(GL_R3_G3_B2), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RGB4"), NanNew<v8::Uint32>(GL_RGB4), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RGB5"), NanNew<v8::Uint32>(GL_RGB5), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RGB8"), NanNew<v8::Uint32>(GL_RGB8), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RGB10"), NanNew<v8::Uint32>(GL_RGB10), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RGB12"), NanNew<v8::Uint32>(GL_RGB12), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RGB16"), NanNew<v8::Uint32>(GL_RGB16), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RGBA2"), NanNew<v8::Uint32>(GL_RGBA2), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RGBA4"), NanNew<v8::Uint32>(GL_RGBA4), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RGB5_A1"), NanNew<v8::Uint32>(GL_RGB5_A1), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RGBA8"), NanNew<v8::Uint32>(GL_RGBA8), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RGB10_A2"), NanNew<v8::Uint32>(GL_RGB10_A2), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RGBA12"), NanNew<v8::Uint32>(GL_RGBA12), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RGBA16"), NanNew<v8::Uint32>(GL_RGBA16), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNSIGNED_BYTE_3_3_2"), NanNew<v8::Uint32>(GL_UNSIGNED_BYTE_3_3_2), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNSIGNED_SHORT_4_4_4_4"), NanNew<v8::Uint32>(GL_UNSIGNED_SHORT_4_4_4_4), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNSIGNED_SHORT_5_5_5_1"), NanNew<v8::Uint32>(GL_UNSIGNED_SHORT_5_5_5_1), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNSIGNED_INT_8_8_8_8"), NanNew<v8::Uint32>(GL_UNSIGNED_INT_8_8_8_8), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNSIGNED_INT_10_10_10_2"), NanNew<v8::Uint32>(GL_UNSIGNED_INT_10_10_10_2), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_BINDING_3D"), NanNew<v8::Uint32>(GL_TEXTURE_BINDING_3D), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("PACK_SKIP_IMAGES"), NanNew<v8::Uint32>(GL_PACK_SKIP_IMAGES), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("PACK_IMAGE_HEIGHT"), NanNew<v8::Uint32>(GL_PACK_IMAGE_HEIGHT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNPACK_SKIP_IMAGES"), NanNew<v8::Uint32>(GL_UNPACK_SKIP_IMAGES), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNPACK_IMAGE_HEIGHT"), NanNew<v8::Uint32>(GL_UNPACK_IMAGE_HEIGHT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_3D"), NanNew<v8::Uint32>(GL_TEXTURE_3D), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("PROXY_TEXTURE_3D"), NanNew<v8::Uint32>(GL_PROXY_TEXTURE_3D), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_DEPTH"), NanNew<v8::Uint32>(GL_TEXTURE_DEPTH), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_WRAP_R"), NanNew<v8::Uint32>(GL_TEXTURE_WRAP_R), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_3D_TEXTURE_SIZE"), NanNew<v8::Uint32>(GL_MAX_3D_TEXTURE_SIZE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNSIGNED_BYTE_2_3_3_REV"), NanNew<v8::Uint32>(GL_UNSIGNED_BYTE_2_3_3_REV), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNSIGNED_SHORT_5_6_5"), NanNew<v8::Uint32>(GL_UNSIGNED_SHORT_5_6_5), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNSIGNED_SHORT_5_6_5_REV"), NanNew<v8::Uint32>(GL_UNSIGNED_SHORT_5_6_5_REV), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNSIGNED_SHORT_4_4_4_4_REV"), NanNew<v8::Uint32>(GL_UNSIGNED_SHORT_4_4_4_4_REV), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNSIGNED_SHORT_1_5_5_5_REV"), NanNew<v8::Uint32>(GL_UNSIGNED_SHORT_1_5_5_5_REV), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNSIGNED_INT_8_8_8_8_REV"), NanNew<v8::Uint32>(GL_UNSIGNED_INT_8_8_8_8_REV), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNSIGNED_INT_2_10_10_10_REV"), NanNew<v8::Uint32>(GL_UNSIGNED_INT_2_10_10_10_REV), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("BGR"), NanNew<v8::Uint32>(GL_BGR), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("BGRA"), NanNew<v8::Uint32>(GL_BGRA), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_ELEMENTS_VERTICES"), NanNew<v8::Uint32>(GL_MAX_ELEMENTS_VERTICES), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_ELEMENTS_INDICES"), NanNew<v8::Uint32>(GL_MAX_ELEMENTS_INDICES), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("CLAMP_TO_EDGE"), NanNew<v8::Uint32>(GL_CLAMP_TO_EDGE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_MIN_LOD"), NanNew<v8::Uint32>(GL_TEXTURE_MIN_LOD), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_MAX_LOD"), NanNew<v8::Uint32>(GL_TEXTURE_MAX_LOD), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_BASE_LEVEL"), NanNew<v8::Uint32>(GL_TEXTURE_BASE_LEVEL), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_MAX_LEVEL"), NanNew<v8::Uint32>(GL_TEXTURE_MAX_LEVEL), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("SMOOTH_POINT_SIZE_RANGE"), NanNew<v8::Uint32>(GL_SMOOTH_POINT_SIZE_RANGE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("SMOOTH_POINT_SIZE_GRANULARITY"), NanNew<v8::Uint32>(GL_SMOOTH_POINT_SIZE_GRANULARITY), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("SMOOTH_LINE_WIDTH_RANGE"), NanNew<v8::Uint32>(GL_SMOOTH_LINE_WIDTH_RANGE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("SMOOTH_LINE_WIDTH_GRANULARITY"), NanNew<v8::Uint32>(GL_SMOOTH_LINE_WIDTH_GRANULARITY), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("ALIASED_LINE_WIDTH_RANGE"), NanNew<v8::Uint32>(GL_ALIASED_LINE_WIDTH_RANGE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("CONSTANT_COLOR"), NanNew<v8::Uint32>(GL_CONSTANT_COLOR), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("ONE_MINUS_CONSTANT_COLOR"), NanNew<v8::Uint32>(GL_ONE_MINUS_CONSTANT_COLOR), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("CONSTANT_ALPHA"), NanNew<v8::Uint32>(GL_CONSTANT_ALPHA), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("ONE_MINUS_CONSTANT_ALPHA"), NanNew<v8::Uint32>(GL_ONE_MINUS_CONSTANT_ALPHA), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("BLEND_COLOR"), NanNew<v8::Uint32>(GL_BLEND_COLOR), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("FUNC_ADD"), NanNew<v8::Uint32>(GL_FUNC_ADD), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MIN"), NanNew<v8::Uint32>(GL_MIN), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX"), NanNew<v8::Uint32>(GL_MAX), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("BLEND_EQUATION"), NanNew<v8::Uint32>(GL_BLEND_EQUATION), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("FUNC_SUBTRACT"), NanNew<v8::Uint32>(GL_FUNC_SUBTRACT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("FUNC_REVERSE_SUBTRACT"), NanNew<v8::Uint32>(GL_FUNC_REVERSE_SUBTRACT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE0"), NanNew<v8::Uint32>(GL_TEXTURE0), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE1"), NanNew<v8::Uint32>(GL_TEXTURE1), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE2"), NanNew<v8::Uint32>(GL_TEXTURE2), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE3"), NanNew<v8::Uint32>(GL_TEXTURE3), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE4"), NanNew<v8::Uint32>(GL_TEXTURE4), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE5"), NanNew<v8::Uint32>(GL_TEXTURE5), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE6"), NanNew<v8::Uint32>(GL_TEXTURE6), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE7"), NanNew<v8::Uint32>(GL_TEXTURE7), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE8"), NanNew<v8::Uint32>(GL_TEXTURE8), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE9"), NanNew<v8::Uint32>(GL_TEXTURE9), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE10"), NanNew<v8::Uint32>(GL_TEXTURE10), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE11"), NanNew<v8::Uint32>(GL_TEXTURE11), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE12"), NanNew<v8::Uint32>(GL_TEXTURE12), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE13"), NanNew<v8::Uint32>(GL_TEXTURE13), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE14"), NanNew<v8::Uint32>(GL_TEXTURE14), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE15"), NanNew<v8::Uint32>(GL_TEXTURE15), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE16"), NanNew<v8::Uint32>(GL_TEXTURE16), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE17"), NanNew<v8::Uint32>(GL_TEXTURE17), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE18"), NanNew<v8::Uint32>(GL_TEXTURE18), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE19"), NanNew<v8::Uint32>(GL_TEXTURE19), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE20"), NanNew<v8::Uint32>(GL_TEXTURE20), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE21"), NanNew<v8::Uint32>(GL_TEXTURE21), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE22"), NanNew<v8::Uint32>(GL_TEXTURE22), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE23"), NanNew<v8::Uint32>(GL_TEXTURE23), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE24"), NanNew<v8::Uint32>(GL_TEXTURE24), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE25"), NanNew<v8::Uint32>(GL_TEXTURE25), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE26"), NanNew<v8::Uint32>(GL_TEXTURE26), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE27"), NanNew<v8::Uint32>(GL_TEXTURE27), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE28"), NanNew<v8::Uint32>(GL_TEXTURE28), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE29"), NanNew<v8::Uint32>(GL_TEXTURE29), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE30"), NanNew<v8::Uint32>(GL_TEXTURE30), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE31"), NanNew<v8::Uint32>(GL_TEXTURE31), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("ACTIVE_TEXTURE"), NanNew<v8::Uint32>(GL_ACTIVE_TEXTURE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MULTISAMPLE"), NanNew<v8::Uint32>(GL_MULTISAMPLE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("SAMPLE_ALPHA_TO_COVERAGE"), NanNew<v8::Uint32>(GL_SAMPLE_ALPHA_TO_COVERAGE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("SAMPLE_ALPHA_TO_ONE"), NanNew<v8::Uint32>(GL_SAMPLE_ALPHA_TO_ONE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("SAMPLE_COVERAGE"), NanNew<v8::Uint32>(GL_SAMPLE_COVERAGE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("SAMPLE_BUFFERS"), NanNew<v8::Uint32>(GL_SAMPLE_BUFFERS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("SAMPLES"), NanNew<v8::Uint32>(GL_SAMPLES), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("SAMPLE_COVERAGE_VALUE"), NanNew<v8::Uint32>(GL_SAMPLE_COVERAGE_VALUE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("SAMPLE_COVERAGE_INVERT"), NanNew<v8::Uint32>(GL_SAMPLE_COVERAGE_INVERT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_CUBE_MAP"), NanNew<v8::Uint32>(GL_TEXTURE_CUBE_MAP), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_BINDING_CUBE_MAP"), NanNew<v8::Uint32>(GL_TEXTURE_BINDING_CUBE_MAP), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_CUBE_MAP_POSITIVE_X"), NanNew<v8::Uint32>(GL_TEXTURE_CUBE_MAP_POSITIVE_X), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_CUBE_MAP_NEGATIVE_X"), NanNew<v8::Uint32>(GL_TEXTURE_CUBE_MAP_NEGATIVE_X), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_CUBE_MAP_POSITIVE_Y"), NanNew<v8::Uint32>(GL_TEXTURE_CUBE_MAP_POSITIVE_Y), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_CUBE_MAP_NEGATIVE_Y"), NanNew<v8::Uint32>(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_CUBE_MAP_POSITIVE_Z"), NanNew<v8::Uint32>(GL_TEXTURE_CUBE_MAP_POSITIVE_Z), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_CUBE_MAP_NEGATIVE_Z"), NanNew<v8::Uint32>(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("PROXY_TEXTURE_CUBE_MAP"), NanNew<v8::Uint32>(GL_PROXY_TEXTURE_CUBE_MAP), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_CUBE_MAP_TEXTURE_SIZE"), NanNew<v8::Uint32>(GL_MAX_CUBE_MAP_TEXTURE_SIZE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("COMPRESSED_RGB"), NanNew<v8::Uint32>(GL_COMPRESSED_RGB), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("COMPRESSED_RGBA"), NanNew<v8::Uint32>(GL_COMPRESSED_RGBA), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_COMPRESSION_HINT"), NanNew<v8::Uint32>(GL_TEXTURE_COMPRESSION_HINT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_COMPRESSED_IMAGE_SIZE"), NanNew<v8::Uint32>(GL_TEXTURE_COMPRESSED_IMAGE_SIZE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_COMPRESSED"), NanNew<v8::Uint32>(GL_TEXTURE_COMPRESSED), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("NUM_COMPRESSED_TEXTURE_FORMATS"), NanNew<v8::Uint32>(GL_NUM_COMPRESSED_TEXTURE_FORMATS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("COMPRESSED_TEXTURE_FORMATS"), NanNew<v8::Uint32>(GL_COMPRESSED_TEXTURE_FORMATS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("CLAMP_TO_BORDER"), NanNew<v8::Uint32>(GL_CLAMP_TO_BORDER), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("BLEND_DST_RGB"), NanNew<v8::Uint32>(GL_BLEND_DST_RGB), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("BLEND_SRC_RGB"), NanNew<v8::Uint32>(GL_BLEND_SRC_RGB), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("BLEND_DST_ALPHA"), NanNew<v8::Uint32>(GL_BLEND_DST_ALPHA), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("BLEND_SRC_ALPHA"), NanNew<v8::Uint32>(GL_BLEND_SRC_ALPHA), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("POINT_FADE_THRESHOLD_SIZE"), NanNew<v8::Uint32>(GL_POINT_FADE_THRESHOLD_SIZE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("DEPTH_COMPONENT16"), NanNew<v8::Uint32>(GL_DEPTH_COMPONENT16), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("DEPTH_COMPONENT24"), NanNew<v8::Uint32>(GL_DEPTH_COMPONENT24), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("DEPTH_COMPONENT32"), NanNew<v8::Uint32>(GL_DEPTH_COMPONENT32), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MIRRORED_REPEAT"), NanNew<v8::Uint32>(GL_MIRRORED_REPEAT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_TEXTURE_LOD_BIAS"), NanNew<v8::Uint32>(GL_MAX_TEXTURE_LOD_BIAS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_LOD_BIAS"), NanNew<v8::Uint32>(GL_TEXTURE_LOD_BIAS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("INCR_WRAP"), NanNew<v8::Uint32>(GL_INCR_WRAP), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("DECR_WRAP"), NanNew<v8::Uint32>(GL_DECR_WRAP), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_DEPTH_SIZE"), NanNew<v8::Uint32>(GL_TEXTURE_DEPTH_SIZE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_COMPARE_MODE"), NanNew<v8::Uint32>(GL_TEXTURE_COMPARE_MODE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_COMPARE_FUNC"), NanNew<v8::Uint32>(GL_TEXTURE_COMPARE_FUNC), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("BUFFER_SIZE"), NanNew<v8::Uint32>(GL_BUFFER_SIZE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("BUFFER_USAGE"), NanNew<v8::Uint32>(GL_BUFFER_USAGE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("QUERY_COUNTER_BITS"), NanNew<v8::Uint32>(GL_QUERY_COUNTER_BITS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("CURRENT_QUERY"), NanNew<v8::Uint32>(GL_CURRENT_QUERY), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("QUERY_RESULT"), NanNew<v8::Uint32>(GL_QUERY_RESULT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("QUERY_RESULT_AVAILABLE"), NanNew<v8::Uint32>(GL_QUERY_RESULT_AVAILABLE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("ARRAY_BUFFER"), NanNew<v8::Uint32>(GL_ARRAY_BUFFER), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("ELEMENT_ARRAY_BUFFER"), NanNew<v8::Uint32>(GL_ELEMENT_ARRAY_BUFFER), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("ARRAY_BUFFER_BINDING"), NanNew<v8::Uint32>(GL_ARRAY_BUFFER_BINDING), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("ELEMENT_ARRAY_BUFFER_BINDING"), NanNew<v8::Uint32>(GL_ELEMENT_ARRAY_BUFFER_BINDING), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("VERTEX_ATTRIB_ARRAY_BUFFER_BINDING"), NanNew<v8::Uint32>(GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("READ_ONLY"), NanNew<v8::Uint32>(GL_READ_ONLY), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("WRITE_ONLY"), NanNew<v8::Uint32>(GL_WRITE_ONLY), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("READ_WRITE"), NanNew<v8::Uint32>(GL_READ_WRITE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("BUFFER_ACCESS"), NanNew<v8::Uint32>(GL_BUFFER_ACCESS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("BUFFER_MAPPED"), NanNew<v8::Uint32>(GL_BUFFER_MAPPED), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("BUFFER_MAP_POINTER"), NanNew<v8::Uint32>(GL_BUFFER_MAP_POINTER), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("STREAM_DRAW"), NanNew<v8::Uint32>(GL_STREAM_DRAW), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("STREAM_READ"), NanNew<v8::Uint32>(GL_STREAM_READ), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("STREAM_COPY"), NanNew<v8::Uint32>(GL_STREAM_COPY), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("STATIC_DRAW"), NanNew<v8::Uint32>(GL_STATIC_DRAW), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("STATIC_READ"), NanNew<v8::Uint32>(GL_STATIC_READ), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("STATIC_COPY"), NanNew<v8::Uint32>(GL_STATIC_COPY), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("DYNAMIC_DRAW"), NanNew<v8::Uint32>(GL_DYNAMIC_DRAW), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("DYNAMIC_READ"), NanNew<v8::Uint32>(GL_DYNAMIC_READ), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("DYNAMIC_COPY"), NanNew<v8::Uint32>(GL_DYNAMIC_COPY), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("SAMPLES_PASSED"), NanNew<v8::Uint32>(GL_SAMPLES_PASSED), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("BLEND_EQUATION_RGB"), NanNew<v8::Uint32>(GL_BLEND_EQUATION_RGB), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("VERTEX_ATTRIB_ARRAY_ENABLED"), NanNew<v8::Uint32>(GL_VERTEX_ATTRIB_ARRAY_ENABLED), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("VERTEX_ATTRIB_ARRAY_SIZE"), NanNew<v8::Uint32>(GL_VERTEX_ATTRIB_ARRAY_SIZE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("VERTEX_ATTRIB_ARRAY_STRIDE"), NanNew<v8::Uint32>(GL_VERTEX_ATTRIB_ARRAY_STRIDE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("VERTEX_ATTRIB_ARRAY_TYPE"), NanNew<v8::Uint32>(GL_VERTEX_ATTRIB_ARRAY_TYPE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("CURRENT_VERTEX_ATTRIB"), NanNew<v8::Uint32>(GL_CURRENT_VERTEX_ATTRIB), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("VERTEX_PROGRAM_POINT_SIZE"), NanNew<v8::Uint32>(GL_VERTEX_PROGRAM_POINT_SIZE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("VERTEX_ATTRIB_ARRAY_POINTER"), NanNew<v8::Uint32>(GL_VERTEX_ATTRIB_ARRAY_POINTER), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("STENCIL_BACK_FUNC"), NanNew<v8::Uint32>(GL_STENCIL_BACK_FUNC), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("STENCIL_BACK_FAIL"), NanNew<v8::Uint32>(GL_STENCIL_BACK_FAIL), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("STENCIL_BACK_PASS_DEPTH_FAIL"), NanNew<v8::Uint32>(GL_STENCIL_BACK_PASS_DEPTH_FAIL), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("STENCIL_BACK_PASS_DEPTH_PASS"), NanNew<v8::Uint32>(GL_STENCIL_BACK_PASS_DEPTH_PASS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_DRAW_BUFFERS"), NanNew<v8::Uint32>(GL_MAX_DRAW_BUFFERS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("DRAW_BUFFER0"), NanNew<v8::Uint32>(GL_DRAW_BUFFER0), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("DRAW_BUFFER1"), NanNew<v8::Uint32>(GL_DRAW_BUFFER1), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("DRAW_BUFFER2"), NanNew<v8::Uint32>(GL_DRAW_BUFFER2), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("DRAW_BUFFER3"), NanNew<v8::Uint32>(GL_DRAW_BUFFER3), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("DRAW_BUFFER4"), NanNew<v8::Uint32>(GL_DRAW_BUFFER4), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("DRAW_BUFFER5"), NanNew<v8::Uint32>(GL_DRAW_BUFFER5), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("DRAW_BUFFER6"), NanNew<v8::Uint32>(GL_DRAW_BUFFER6), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("DRAW_BUFFER7"), NanNew<v8::Uint32>(GL_DRAW_BUFFER7), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("DRAW_BUFFER8"), NanNew<v8::Uint32>(GL_DRAW_BUFFER8), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("DRAW_BUFFER9"), NanNew<v8::Uint32>(GL_DRAW_BUFFER9), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("DRAW_BUFFER10"), NanNew<v8::Uint32>(GL_DRAW_BUFFER10), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("DRAW_BUFFER11"), NanNew<v8::Uint32>(GL_DRAW_BUFFER11), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("DRAW_BUFFER12"), NanNew<v8::Uint32>(GL_DRAW_BUFFER12), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("DRAW_BUFFER13"), NanNew<v8::Uint32>(GL_DRAW_BUFFER13), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("DRAW_BUFFER14"), NanNew<v8::Uint32>(GL_DRAW_BUFFER14), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("DRAW_BUFFER15"), NanNew<v8::Uint32>(GL_DRAW_BUFFER15), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("BLEND_EQUATION_ALPHA"), NanNew<v8::Uint32>(GL_BLEND_EQUATION_ALPHA), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_VERTEX_ATTRIBS"), NanNew<v8::Uint32>(GL_MAX_VERTEX_ATTRIBS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("VERTEX_ATTRIB_ARRAY_NORMALIZED"), NanNew<v8::Uint32>(GL_VERTEX_ATTRIB_ARRAY_NORMALIZED), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_TEXTURE_IMAGE_UNITS"), NanNew<v8::Uint32>(GL_MAX_TEXTURE_IMAGE_UNITS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("FRAGMENT_SHADER"), NanNew<v8::Uint32>(GL_FRAGMENT_SHADER), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("VERTEX_SHADER"), NanNew<v8::Uint32>(GL_VERTEX_SHADER), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_FRAGMENT_UNIFORM_COMPONENTS"), NanNew<v8::Uint32>(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_VERTEX_UNIFORM_COMPONENTS"), NanNew<v8::Uint32>(GL_MAX_VERTEX_UNIFORM_COMPONENTS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_VARYING_FLOATS"), NanNew<v8::Uint32>(GL_MAX_VARYING_FLOATS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_VERTEX_TEXTURE_IMAGE_UNITS"), NanNew<v8::Uint32>(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_COMBINED_TEXTURE_IMAGE_UNITS"), NanNew<v8::Uint32>(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("SHADER_TYPE"), NanNew<v8::Uint32>(GL_SHADER_TYPE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("FLOAT_VEC2"), NanNew<v8::Uint32>(GL_FLOAT_VEC2), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("FLOAT_VEC3"), NanNew<v8::Uint32>(GL_FLOAT_VEC3), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("FLOAT_VEC4"), NanNew<v8::Uint32>(GL_FLOAT_VEC4), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("INT_VEC2"), NanNew<v8::Uint32>(GL_INT_VEC2), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("INT_VEC3"), NanNew<v8::Uint32>(GL_INT_VEC3), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("INT_VEC4"), NanNew<v8::Uint32>(GL_INT_VEC4), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("BOOL"), NanNew<v8::Uint32>(GL_BOOL), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("BOOL_VEC2"), NanNew<v8::Uint32>(GL_BOOL_VEC2), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("BOOL_VEC3"), NanNew<v8::Uint32>(GL_BOOL_VEC3), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("BOOL_VEC4"), NanNew<v8::Uint32>(GL_BOOL_VEC4), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("FLOAT_MAT2"), NanNew<v8::Uint32>(GL_FLOAT_MAT2), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("FLOAT_MAT3"), NanNew<v8::Uint32>(GL_FLOAT_MAT3), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("FLOAT_MAT4"), NanNew<v8::Uint32>(GL_FLOAT_MAT4), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("SAMPLER_1D"), NanNew<v8::Uint32>(GL_SAMPLER_1D), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("SAMPLER_2D"), NanNew<v8::Uint32>(GL_SAMPLER_2D), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("SAMPLER_3D"), NanNew<v8::Uint32>(GL_SAMPLER_3D), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("SAMPLER_CUBE"), NanNew<v8::Uint32>(GL_SAMPLER_CUBE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("SAMPLER_1D_SHADOW"), NanNew<v8::Uint32>(GL_SAMPLER_1D_SHADOW), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("SAMPLER_2D_SHADOW"), NanNew<v8::Uint32>(GL_SAMPLER_2D_SHADOW), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("DELETE_STATUS"), NanNew<v8::Uint32>(GL_DELETE_STATUS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("COMPILE_STATUS"), NanNew<v8::Uint32>(GL_COMPILE_STATUS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("LINK_STATUS"), NanNew<v8::Uint32>(GL_LINK_STATUS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("VALIDATE_STATUS"), NanNew<v8::Uint32>(GL_VALIDATE_STATUS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("INFO_LOG_LENGTH"), NanNew<v8::Uint32>(GL_INFO_LOG_LENGTH), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("ATTACHED_SHADERS"), NanNew<v8::Uint32>(GL_ATTACHED_SHADERS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("ACTIVE_UNIFORMS"), NanNew<v8::Uint32>(GL_ACTIVE_UNIFORMS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("ACTIVE_UNIFORM_MAX_LENGTH"), NanNew<v8::Uint32>(GL_ACTIVE_UNIFORM_MAX_LENGTH), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("SHADER_SOURCE_LENGTH"), NanNew<v8::Uint32>(GL_SHADER_SOURCE_LENGTH), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("ACTIVE_ATTRIBUTES"), NanNew<v8::Uint32>(GL_ACTIVE_ATTRIBUTES), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("ACTIVE_ATTRIBUTE_MAX_LENGTH"), NanNew<v8::Uint32>(GL_ACTIVE_ATTRIBUTE_MAX_LENGTH), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("FRAGMENT_SHADER_DERIVATIVE_HINT"), NanNew<v8::Uint32>(GL_FRAGMENT_SHADER_DERIVATIVE_HINT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("SHADING_LANGUAGE_VERSION"), NanNew<v8::Uint32>(GL_SHADING_LANGUAGE_VERSION), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("CURRENT_PROGRAM"), NanNew<v8::Uint32>(GL_CURRENT_PROGRAM), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("POINT_SPRITE_COORD_ORIGIN"), NanNew<v8::Uint32>(GL_POINT_SPRITE_COORD_ORIGIN), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("LOWER_LEFT"), NanNew<v8::Uint32>(GL_LOWER_LEFT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UPPER_LEFT"), NanNew<v8::Uint32>(GL_UPPER_LEFT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("STENCIL_BACK_REF"), NanNew<v8::Uint32>(GL_STENCIL_BACK_REF), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("STENCIL_BACK_VALUE_MASK"), NanNew<v8::Uint32>(GL_STENCIL_BACK_VALUE_MASK), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("STENCIL_BACK_WRITEMASK"), NanNew<v8::Uint32>(GL_STENCIL_BACK_WRITEMASK), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("PIXEL_PACK_BUFFER"), NanNew<v8::Uint32>(GL_PIXEL_PACK_BUFFER), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("PIXEL_UNPACK_BUFFER"), NanNew<v8::Uint32>(GL_PIXEL_UNPACK_BUFFER), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("PIXEL_PACK_BUFFER_BINDING"), NanNew<v8::Uint32>(GL_PIXEL_PACK_BUFFER_BINDING), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("PIXEL_UNPACK_BUFFER_BINDING"), NanNew<v8::Uint32>(GL_PIXEL_UNPACK_BUFFER_BINDING), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("FLOAT_MAT2x3"), NanNew<v8::Uint32>(GL_FLOAT_MAT2x3), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("FLOAT_MAT2x4"), NanNew<v8::Uint32>(GL_FLOAT_MAT2x4), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("FLOAT_MAT3x2"), NanNew<v8::Uint32>(GL_FLOAT_MAT3x2), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("FLOAT_MAT3x4"), NanNew<v8::Uint32>(GL_FLOAT_MAT3x4), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("FLOAT_MAT4x2"), NanNew<v8::Uint32>(GL_FLOAT_MAT4x2), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("FLOAT_MAT4x3"), NanNew<v8::Uint32>(GL_FLOAT_MAT4x3), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("SRGB"), NanNew<v8::Uint32>(GL_SRGB), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("SRGB8"), NanNew<v8::Uint32>(GL_SRGB8), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("SRGB_ALPHA"), NanNew<v8::Uint32>(GL_SRGB_ALPHA), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("SRGB8_ALPHA8"), NanNew<v8::Uint32>(GL_SRGB8_ALPHA8), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("COMPRESSED_SRGB"), NanNew<v8::Uint32>(GL_COMPRESSED_SRGB), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("COMPRESSED_SRGB_ALPHA"), NanNew<v8::Uint32>(GL_COMPRESSED_SRGB_ALPHA), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("COMPARE_REF_TO_TEXTURE"), NanNew<v8::Uint32>(GL_COMPARE_REF_TO_TEXTURE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("CLIP_DISTANCE0"), NanNew<v8::Uint32>(GL_CLIP_DISTANCE0), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("CLIP_DISTANCE1"), NanNew<v8::Uint32>(GL_CLIP_DISTANCE1), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("CLIP_DISTANCE2"), NanNew<v8::Uint32>(GL_CLIP_DISTANCE2), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("CLIP_DISTANCE3"), NanNew<v8::Uint32>(GL_CLIP_DISTANCE3), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("CLIP_DISTANCE4"), NanNew<v8::Uint32>(GL_CLIP_DISTANCE4), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("CLIP_DISTANCE5"), NanNew<v8::Uint32>(GL_CLIP_DISTANCE5), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("CLIP_DISTANCE6"), NanNew<v8::Uint32>(GL_CLIP_DISTANCE6), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("CLIP_DISTANCE7"), NanNew<v8::Uint32>(GL_CLIP_DISTANCE7), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_CLIP_DISTANCES"), NanNew<v8::Uint32>(GL_MAX_CLIP_DISTANCES), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAJOR_VERSION"), NanNew<v8::Uint32>(GL_MAJOR_VERSION), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MINOR_VERSION"), NanNew<v8::Uint32>(GL_MINOR_VERSION), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("NUM_EXTENSIONS"), NanNew<v8::Uint32>(GL_NUM_EXTENSIONS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("CONTEXT_FLAGS"), NanNew<v8::Uint32>(GL_CONTEXT_FLAGS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("COMPRESSED_RED"), NanNew<v8::Uint32>(GL_COMPRESSED_RED), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("COMPRESSED_RG"), NanNew<v8::Uint32>(GL_COMPRESSED_RG), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT"), NanNew<v8::Uint32>(GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RGBA32F"), NanNew<v8::Uint32>(GL_RGBA32F), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RGB32F"), NanNew<v8::Uint32>(GL_RGB32F), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RGBA16F"), NanNew<v8::Uint32>(GL_RGBA16F), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RGB16F"), NanNew<v8::Uint32>(GL_RGB16F), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("VERTEX_ATTRIB_ARRAY_INTEGER"), NanNew<v8::Uint32>(GL_VERTEX_ATTRIB_ARRAY_INTEGER), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_ARRAY_TEXTURE_LAYERS"), NanNew<v8::Uint32>(GL_MAX_ARRAY_TEXTURE_LAYERS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MIN_PROGRAM_TEXEL_OFFSET"), NanNew<v8::Uint32>(GL_MIN_PROGRAM_TEXEL_OFFSET), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_PROGRAM_TEXEL_OFFSET"), NanNew<v8::Uint32>(GL_MAX_PROGRAM_TEXEL_OFFSET), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("CLAMP_READ_COLOR"), NanNew<v8::Uint32>(GL_CLAMP_READ_COLOR), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("FIXED_ONLY"), NanNew<v8::Uint32>(GL_FIXED_ONLY), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_VARYING_COMPONENTS"), NanNew<v8::Uint32>(GL_MAX_VARYING_COMPONENTS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_1D_ARRAY"), NanNew<v8::Uint32>(GL_TEXTURE_1D_ARRAY), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("PROXY_TEXTURE_1D_ARRAY"), NanNew<v8::Uint32>(GL_PROXY_TEXTURE_1D_ARRAY), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_2D_ARRAY"), NanNew<v8::Uint32>(GL_TEXTURE_2D_ARRAY), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("PROXY_TEXTURE_2D_ARRAY"), NanNew<v8::Uint32>(GL_PROXY_TEXTURE_2D_ARRAY), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_BINDING_1D_ARRAY"), NanNew<v8::Uint32>(GL_TEXTURE_BINDING_1D_ARRAY), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_BINDING_2D_ARRAY"), NanNew<v8::Uint32>(GL_TEXTURE_BINDING_2D_ARRAY), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("R11F_G11F_B10F"), NanNew<v8::Uint32>(GL_R11F_G11F_B10F), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNSIGNED_INT_10F_11F_11F_REV"), NanNew<v8::Uint32>(GL_UNSIGNED_INT_10F_11F_11F_REV), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RGB9_E5"), NanNew<v8::Uint32>(GL_RGB9_E5), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNSIGNED_INT_5_9_9_9_REV"), NanNew<v8::Uint32>(GL_UNSIGNED_INT_5_9_9_9_REV), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_SHARED_SIZE"), NanNew<v8::Uint32>(GL_TEXTURE_SHARED_SIZE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH"), NanNew<v8::Uint32>(GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TRANSFORM_FEEDBACK_BUFFER_MODE"), NanNew<v8::Uint32>(GL_TRANSFORM_FEEDBACK_BUFFER_MODE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS"), NanNew<v8::Uint32>(GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TRANSFORM_FEEDBACK_VARYINGS"), NanNew<v8::Uint32>(GL_TRANSFORM_FEEDBACK_VARYINGS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TRANSFORM_FEEDBACK_BUFFER_START"), NanNew<v8::Uint32>(GL_TRANSFORM_FEEDBACK_BUFFER_START), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TRANSFORM_FEEDBACK_BUFFER_SIZE"), NanNew<v8::Uint32>(GL_TRANSFORM_FEEDBACK_BUFFER_SIZE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("PRIMITIVES_GENERATED"), NanNew<v8::Uint32>(GL_PRIMITIVES_GENERATED), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN"), NanNew<v8::Uint32>(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RASTERIZER_DISCARD"), NanNew<v8::Uint32>(GL_RASTERIZER_DISCARD), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS"), NanNew<v8::Uint32>(GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS"), NanNew<v8::Uint32>(GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("INTERLEAVED_ATTRIBS"), NanNew<v8::Uint32>(GL_INTERLEAVED_ATTRIBS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("SEPARATE_ATTRIBS"), NanNew<v8::Uint32>(GL_SEPARATE_ATTRIBS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TRANSFORM_FEEDBACK_BUFFER"), NanNew<v8::Uint32>(GL_TRANSFORM_FEEDBACK_BUFFER), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TRANSFORM_FEEDBACK_BUFFER_BINDING"), NanNew<v8::Uint32>(GL_TRANSFORM_FEEDBACK_BUFFER_BINDING), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RGBA32UI"), NanNew<v8::Uint32>(GL_RGBA32UI), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RGB32UI"), NanNew<v8::Uint32>(GL_RGB32UI), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RGBA16UI"), NanNew<v8::Uint32>(GL_RGBA16UI), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RGB16UI"), NanNew<v8::Uint32>(GL_RGB16UI), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RGBA8UI"), NanNew<v8::Uint32>(GL_RGBA8UI), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RGB8UI"), NanNew<v8::Uint32>(GL_RGB8UI), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RGBA32I"), NanNew<v8::Uint32>(GL_RGBA32I), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RGB32I"), NanNew<v8::Uint32>(GL_RGB32I), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RGBA16I"), NanNew<v8::Uint32>(GL_RGBA16I), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RGB16I"), NanNew<v8::Uint32>(GL_RGB16I), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RGBA8I"), NanNew<v8::Uint32>(GL_RGBA8I), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RGB8I"), NanNew<v8::Uint32>(GL_RGB8I), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RED_INTEGER"), NanNew<v8::Uint32>(GL_RED_INTEGER), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("GREEN_INTEGER"), NanNew<v8::Uint32>(GL_GREEN_INTEGER), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("BLUE_INTEGER"), NanNew<v8::Uint32>(GL_BLUE_INTEGER), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RGB_INTEGER"), NanNew<v8::Uint32>(GL_RGB_INTEGER), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RGBA_INTEGER"), NanNew<v8::Uint32>(GL_RGBA_INTEGER), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("BGR_INTEGER"), NanNew<v8::Uint32>(GL_BGR_INTEGER), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("BGRA_INTEGER"), NanNew<v8::Uint32>(GL_BGRA_INTEGER), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("SAMPLER_1D_ARRAY"), NanNew<v8::Uint32>(GL_SAMPLER_1D_ARRAY), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("SAMPLER_2D_ARRAY"), NanNew<v8::Uint32>(GL_SAMPLER_2D_ARRAY), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("SAMPLER_1D_ARRAY_SHADOW"), NanNew<v8::Uint32>(GL_SAMPLER_1D_ARRAY_SHADOW), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("SAMPLER_2D_ARRAY_SHADOW"), NanNew<v8::Uint32>(GL_SAMPLER_2D_ARRAY_SHADOW), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("SAMPLER_CUBE_SHADOW"), NanNew<v8::Uint32>(GL_SAMPLER_CUBE_SHADOW), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNSIGNED_INT_VEC2"), NanNew<v8::Uint32>(GL_UNSIGNED_INT_VEC2), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNSIGNED_INT_VEC3"), NanNew<v8::Uint32>(GL_UNSIGNED_INT_VEC3), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNSIGNED_INT_VEC4"), NanNew<v8::Uint32>(GL_UNSIGNED_INT_VEC4), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("INT_SAMPLER_1D"), NanNew<v8::Uint32>(GL_INT_SAMPLER_1D), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("INT_SAMPLER_2D"), NanNew<v8::Uint32>(GL_INT_SAMPLER_2D), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("INT_SAMPLER_3D"), NanNew<v8::Uint32>(GL_INT_SAMPLER_3D), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("INT_SAMPLER_CUBE"), NanNew<v8::Uint32>(GL_INT_SAMPLER_CUBE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("INT_SAMPLER_1D_ARRAY"), NanNew<v8::Uint32>(GL_INT_SAMPLER_1D_ARRAY), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("INT_SAMPLER_2D_ARRAY"), NanNew<v8::Uint32>(GL_INT_SAMPLER_2D_ARRAY), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNSIGNED_INT_SAMPLER_1D"), NanNew<v8::Uint32>(GL_UNSIGNED_INT_SAMPLER_1D), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNSIGNED_INT_SAMPLER_2D"), NanNew<v8::Uint32>(GL_UNSIGNED_INT_SAMPLER_2D), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNSIGNED_INT_SAMPLER_3D"), NanNew<v8::Uint32>(GL_UNSIGNED_INT_SAMPLER_3D), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNSIGNED_INT_SAMPLER_CUBE"), NanNew<v8::Uint32>(GL_UNSIGNED_INT_SAMPLER_CUBE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNSIGNED_INT_SAMPLER_1D_ARRAY"), NanNew<v8::Uint32>(GL_UNSIGNED_INT_SAMPLER_1D_ARRAY), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNSIGNED_INT_SAMPLER_2D_ARRAY"), NanNew<v8::Uint32>(GL_UNSIGNED_INT_SAMPLER_2D_ARRAY), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("QUERY_WAIT"), NanNew<v8::Uint32>(GL_QUERY_WAIT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("QUERY_NO_WAIT"), NanNew<v8::Uint32>(GL_QUERY_NO_WAIT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("QUERY_BY_REGION_WAIT"), NanNew<v8::Uint32>(GL_QUERY_BY_REGION_WAIT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("QUERY_BY_REGION_NO_WAIT"), NanNew<v8::Uint32>(GL_QUERY_BY_REGION_NO_WAIT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("BUFFER_ACCESS_FLAGS"), NanNew<v8::Uint32>(GL_BUFFER_ACCESS_FLAGS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("BUFFER_MAP_LENGTH"), NanNew<v8::Uint32>(GL_BUFFER_MAP_LENGTH), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("BUFFER_MAP_OFFSET"), NanNew<v8::Uint32>(GL_BUFFER_MAP_OFFSET), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("SAMPLER_2D_RECT"), NanNew<v8::Uint32>(GL_SAMPLER_2D_RECT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("SAMPLER_2D_RECT_SHADOW"), NanNew<v8::Uint32>(GL_SAMPLER_2D_RECT_SHADOW), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("SAMPLER_BUFFER"), NanNew<v8::Uint32>(GL_SAMPLER_BUFFER), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("INT_SAMPLER_2D_RECT"), NanNew<v8::Uint32>(GL_INT_SAMPLER_2D_RECT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("INT_SAMPLER_BUFFER"), NanNew<v8::Uint32>(GL_INT_SAMPLER_BUFFER), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNSIGNED_INT_SAMPLER_2D_RECT"), NanNew<v8::Uint32>(GL_UNSIGNED_INT_SAMPLER_2D_RECT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNSIGNED_INT_SAMPLER_BUFFER"), NanNew<v8::Uint32>(GL_UNSIGNED_INT_SAMPLER_BUFFER), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_BUFFER"), NanNew<v8::Uint32>(GL_TEXTURE_BUFFER), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_TEXTURE_BUFFER_SIZE"), NanNew<v8::Uint32>(GL_MAX_TEXTURE_BUFFER_SIZE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_BINDING_BUFFER"), NanNew<v8::Uint32>(GL_TEXTURE_BINDING_BUFFER), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_BUFFER_DATA_STORE_BINDING"), NanNew<v8::Uint32>(GL_TEXTURE_BUFFER_DATA_STORE_BINDING), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_RECTANGLE"), NanNew<v8::Uint32>(GL_TEXTURE_RECTANGLE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_BINDING_RECTANGLE"), NanNew<v8::Uint32>(GL_TEXTURE_BINDING_RECTANGLE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("PROXY_TEXTURE_RECTANGLE"), NanNew<v8::Uint32>(GL_PROXY_TEXTURE_RECTANGLE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_RECTANGLE_TEXTURE_SIZE"), NanNew<v8::Uint32>(GL_MAX_RECTANGLE_TEXTURE_SIZE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("R8_SNORM"), NanNew<v8::Uint32>(GL_R8_SNORM), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RG8_SNORM"), NanNew<v8::Uint32>(GL_RG8_SNORM), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RGB8_SNORM"), NanNew<v8::Uint32>(GL_RGB8_SNORM), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RGBA8_SNORM"), NanNew<v8::Uint32>(GL_RGBA8_SNORM), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("R16_SNORM"), NanNew<v8::Uint32>(GL_R16_SNORM), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RG16_SNORM"), NanNew<v8::Uint32>(GL_RG16_SNORM), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RGB16_SNORM"), NanNew<v8::Uint32>(GL_RGB16_SNORM), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RGBA16_SNORM"), NanNew<v8::Uint32>(GL_RGBA16_SNORM), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("SIGNED_NORMALIZED"), NanNew<v8::Uint32>(GL_SIGNED_NORMALIZED), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("PRIMITIVE_RESTART"), NanNew<v8::Uint32>(GL_PRIMITIVE_RESTART), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("PRIMITIVE_RESTART_INDEX"), NanNew<v8::Uint32>(GL_PRIMITIVE_RESTART_INDEX), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("CONTEXT_CORE_PROFILE_BIT"), NanNew<v8::Uint32>(GL_CONTEXT_CORE_PROFILE_BIT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("CONTEXT_COMPATIBILITY_PROFILE_BIT"), NanNew<v8::Uint32>(GL_CONTEXT_COMPATIBILITY_PROFILE_BIT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("LINES_ADJACENCY"), NanNew<v8::Uint32>(GL_LINES_ADJACENCY), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("LINE_STRIP_ADJACENCY"), NanNew<v8::Uint32>(GL_LINE_STRIP_ADJACENCY), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TRIANGLES_ADJACENCY"), NanNew<v8::Uint32>(GL_TRIANGLES_ADJACENCY), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TRIANGLE_STRIP_ADJACENCY"), NanNew<v8::Uint32>(GL_TRIANGLE_STRIP_ADJACENCY), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("PROGRAM_POINT_SIZE"), NanNew<v8::Uint32>(GL_PROGRAM_POINT_SIZE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_GEOMETRY_TEXTURE_IMAGE_UNITS"), NanNew<v8::Uint32>(GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("FRAMEBUFFER_ATTACHMENT_LAYERED"), NanNew<v8::Uint32>(GL_FRAMEBUFFER_ATTACHMENT_LAYERED), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS"), NanNew<v8::Uint32>(GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("GEOMETRY_SHADER"), NanNew<v8::Uint32>(GL_GEOMETRY_SHADER), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("GEOMETRY_VERTICES_OUT"), NanNew<v8::Uint32>(GL_GEOMETRY_VERTICES_OUT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("GEOMETRY_INPUT_TYPE"), NanNew<v8::Uint32>(GL_GEOMETRY_INPUT_TYPE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("GEOMETRY_OUTPUT_TYPE"), NanNew<v8::Uint32>(GL_GEOMETRY_OUTPUT_TYPE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_GEOMETRY_UNIFORM_COMPONENTS"), NanNew<v8::Uint32>(GL_MAX_GEOMETRY_UNIFORM_COMPONENTS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_GEOMETRY_OUTPUT_VERTICES"), NanNew<v8::Uint32>(GL_MAX_GEOMETRY_OUTPUT_VERTICES), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS"), NanNew<v8::Uint32>(GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_VERTEX_OUTPUT_COMPONENTS"), NanNew<v8::Uint32>(GL_MAX_VERTEX_OUTPUT_COMPONENTS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_GEOMETRY_INPUT_COMPONENTS"), NanNew<v8::Uint32>(GL_MAX_GEOMETRY_INPUT_COMPONENTS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_GEOMETRY_OUTPUT_COMPONENTS"), NanNew<v8::Uint32>(GL_MAX_GEOMETRY_OUTPUT_COMPONENTS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_FRAGMENT_INPUT_COMPONENTS"), NanNew<v8::Uint32>(GL_MAX_FRAGMENT_INPUT_COMPONENTS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("CONTEXT_PROFILE_MASK"), NanNew<v8::Uint32>(GL_CONTEXT_PROFILE_MASK), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("VERTEX_ATTRIB_ARRAY_DIVISOR"), NanNew<v8::Uint32>(GL_VERTEX_ATTRIB_ARRAY_DIVISOR), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("SAMPLE_SHADING"), NanNew<v8::Uint32>(GL_SAMPLE_SHADING), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MIN_SAMPLE_SHADING_VALUE"), NanNew<v8::Uint32>(GL_MIN_SAMPLE_SHADING_VALUE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MIN_PROGRAM_TEXTURE_GATHER_OFFSET"), NanNew<v8::Uint32>(GL_MIN_PROGRAM_TEXTURE_GATHER_OFFSET), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_PROGRAM_TEXTURE_GATHER_OFFSET"), NanNew<v8::Uint32>(GL_MAX_PROGRAM_TEXTURE_GATHER_OFFSET), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_CUBE_MAP_ARRAY"), NanNew<v8::Uint32>(GL_TEXTURE_CUBE_MAP_ARRAY), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_BINDING_CUBE_MAP_ARRAY"), NanNew<v8::Uint32>(GL_TEXTURE_BINDING_CUBE_MAP_ARRAY), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("PROXY_TEXTURE_CUBE_MAP_ARRAY"), NanNew<v8::Uint32>(GL_PROXY_TEXTURE_CUBE_MAP_ARRAY), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("SAMPLER_CUBE_MAP_ARRAY"), NanNew<v8::Uint32>(GL_SAMPLER_CUBE_MAP_ARRAY), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("SAMPLER_CUBE_MAP_ARRAY_SHADOW"), NanNew<v8::Uint32>(GL_SAMPLER_CUBE_MAP_ARRAY_SHADOW), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("INT_SAMPLER_CUBE_MAP_ARRAY"), NanNew<v8::Uint32>(GL_INT_SAMPLER_CUBE_MAP_ARRAY), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNSIGNED_INT_SAMPLER_CUBE_MAP_ARRAY"), NanNew<v8::Uint32>(GL_UNSIGNED_INT_SAMPLER_CUBE_MAP_ARRAY), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("DEPTH_COMPONENT32F"), NanNew<v8::Uint32>(GL_DEPTH_COMPONENT32F), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("DEPTH32F_STENCIL8"), NanNew<v8::Uint32>(GL_DEPTH32F_STENCIL8), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("FLOAT_32_UNSIGNED_INT_24_8_REV"), NanNew<v8::Uint32>(GL_FLOAT_32_UNSIGNED_INT_24_8_REV), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("INVALID_FRAMEBUFFER_OPERATION"), NanNew<v8::Uint32>(GL_INVALID_FRAMEBUFFER_OPERATION), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING"), NanNew<v8::Uint32>(GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE"), NanNew<v8::Uint32>(GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("FRAMEBUFFER_ATTACHMENT_RED_SIZE"), NanNew<v8::Uint32>(GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("FRAMEBUFFER_ATTACHMENT_GREEN_SIZE"), NanNew<v8::Uint32>(GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("FRAMEBUFFER_ATTACHMENT_BLUE_SIZE"), NanNew<v8::Uint32>(GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE"), NanNew<v8::Uint32>(GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE"), NanNew<v8::Uint32>(GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE"), NanNew<v8::Uint32>(GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("FRAMEBUFFER_DEFAULT"), NanNew<v8::Uint32>(GL_FRAMEBUFFER_DEFAULT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("FRAMEBUFFER_UNDEFINED"), NanNew<v8::Uint32>(GL_FRAMEBUFFER_UNDEFINED), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("DEPTH_STENCIL_ATTACHMENT"), NanNew<v8::Uint32>(GL_DEPTH_STENCIL_ATTACHMENT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_RENDERBUFFER_SIZE"), NanNew<v8::Uint32>(GL_MAX_RENDERBUFFER_SIZE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("DEPTH_STENCIL"), NanNew<v8::Uint32>(GL_DEPTH_STENCIL), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNSIGNED_INT_24_8"), NanNew<v8::Uint32>(GL_UNSIGNED_INT_24_8), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("DEPTH24_STENCIL8"), NanNew<v8::Uint32>(GL_DEPTH24_STENCIL8), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_STENCIL_SIZE"), NanNew<v8::Uint32>(GL_TEXTURE_STENCIL_SIZE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_RED_TYPE"), NanNew<v8::Uint32>(GL_TEXTURE_RED_TYPE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_GREEN_TYPE"), NanNew<v8::Uint32>(GL_TEXTURE_GREEN_TYPE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_BLUE_TYPE"), NanNew<v8::Uint32>(GL_TEXTURE_BLUE_TYPE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_ALPHA_TYPE"), NanNew<v8::Uint32>(GL_TEXTURE_ALPHA_TYPE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_DEPTH_TYPE"), NanNew<v8::Uint32>(GL_TEXTURE_DEPTH_TYPE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNSIGNED_NORMALIZED"), NanNew<v8::Uint32>(GL_UNSIGNED_NORMALIZED), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("FRAMEBUFFER_BINDING"), NanNew<v8::Uint32>(GL_FRAMEBUFFER_BINDING), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RENDERBUFFER_BINDING"), NanNew<v8::Uint32>(GL_RENDERBUFFER_BINDING), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("READ_FRAMEBUFFER"), NanNew<v8::Uint32>(GL_READ_FRAMEBUFFER), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("DRAW_FRAMEBUFFER"), NanNew<v8::Uint32>(GL_DRAW_FRAMEBUFFER), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("READ_FRAMEBUFFER_BINDING"), NanNew<v8::Uint32>(GL_READ_FRAMEBUFFER_BINDING), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RENDERBUFFER_SAMPLES"), NanNew<v8::Uint32>(GL_RENDERBUFFER_SAMPLES), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE"), NanNew<v8::Uint32>(GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("FRAMEBUFFER_ATTACHMENT_OBJECT_NAME"), NanNew<v8::Uint32>(GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL"), NanNew<v8::Uint32>(GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE"), NanNew<v8::Uint32>(GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER"), NanNew<v8::Uint32>(GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("FRAMEBUFFER_COMPLETE"), NanNew<v8::Uint32>(GL_FRAMEBUFFER_COMPLETE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("FRAMEBUFFER_INCOMPLETE_ATTACHMENT"), NanNew<v8::Uint32>(GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT"), NanNew<v8::Uint32>(GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER"), NanNew<v8::Uint32>(GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("FRAMEBUFFER_INCOMPLETE_READ_BUFFER"), NanNew<v8::Uint32>(GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("FRAMEBUFFER_UNSUPPORTED"), NanNew<v8::Uint32>(GL_FRAMEBUFFER_UNSUPPORTED), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_COLOR_ATTACHMENTS"), NanNew<v8::Uint32>(GL_MAX_COLOR_ATTACHMENTS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("COLOR_ATTACHMENT0"), NanNew<v8::Uint32>(GL_COLOR_ATTACHMENT0), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("COLOR_ATTACHMENT1"), NanNew<v8::Uint32>(GL_COLOR_ATTACHMENT1), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("COLOR_ATTACHMENT2"), NanNew<v8::Uint32>(GL_COLOR_ATTACHMENT2), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("COLOR_ATTACHMENT3"), NanNew<v8::Uint32>(GL_COLOR_ATTACHMENT3), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("COLOR_ATTACHMENT4"), NanNew<v8::Uint32>(GL_COLOR_ATTACHMENT4), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("COLOR_ATTACHMENT5"), NanNew<v8::Uint32>(GL_COLOR_ATTACHMENT5), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("COLOR_ATTACHMENT6"), NanNew<v8::Uint32>(GL_COLOR_ATTACHMENT6), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("COLOR_ATTACHMENT7"), NanNew<v8::Uint32>(GL_COLOR_ATTACHMENT7), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("COLOR_ATTACHMENT8"), NanNew<v8::Uint32>(GL_COLOR_ATTACHMENT8), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("COLOR_ATTACHMENT9"), NanNew<v8::Uint32>(GL_COLOR_ATTACHMENT9), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("COLOR_ATTACHMENT10"), NanNew<v8::Uint32>(GL_COLOR_ATTACHMENT10), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("COLOR_ATTACHMENT11"), NanNew<v8::Uint32>(GL_COLOR_ATTACHMENT11), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("COLOR_ATTACHMENT12"), NanNew<v8::Uint32>(GL_COLOR_ATTACHMENT12), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("COLOR_ATTACHMENT13"), NanNew<v8::Uint32>(GL_COLOR_ATTACHMENT13), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("COLOR_ATTACHMENT14"), NanNew<v8::Uint32>(GL_COLOR_ATTACHMENT14), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("COLOR_ATTACHMENT15"), NanNew<v8::Uint32>(GL_COLOR_ATTACHMENT15), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("DEPTH_ATTACHMENT"), NanNew<v8::Uint32>(GL_DEPTH_ATTACHMENT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("STENCIL_ATTACHMENT"), NanNew<v8::Uint32>(GL_STENCIL_ATTACHMENT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("FRAMEBUFFER"), NanNew<v8::Uint32>(GL_FRAMEBUFFER), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RENDERBUFFER"), NanNew<v8::Uint32>(GL_RENDERBUFFER), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RENDERBUFFER_WIDTH"), NanNew<v8::Uint32>(GL_RENDERBUFFER_WIDTH), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RENDERBUFFER_HEIGHT"), NanNew<v8::Uint32>(GL_RENDERBUFFER_HEIGHT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RENDERBUFFER_INTERNAL_FORMAT"), NanNew<v8::Uint32>(GL_RENDERBUFFER_INTERNAL_FORMAT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("STENCIL_INDEX1"), NanNew<v8::Uint32>(GL_STENCIL_INDEX1), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("STENCIL_INDEX4"), NanNew<v8::Uint32>(GL_STENCIL_INDEX4), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("STENCIL_INDEX8"), NanNew<v8::Uint32>(GL_STENCIL_INDEX8), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("STENCIL_INDEX16"), NanNew<v8::Uint32>(GL_STENCIL_INDEX16), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RENDERBUFFER_RED_SIZE"), NanNew<v8::Uint32>(GL_RENDERBUFFER_RED_SIZE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RENDERBUFFER_GREEN_SIZE"), NanNew<v8::Uint32>(GL_RENDERBUFFER_GREEN_SIZE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RENDERBUFFER_BLUE_SIZE"), NanNew<v8::Uint32>(GL_RENDERBUFFER_BLUE_SIZE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RENDERBUFFER_ALPHA_SIZE"), NanNew<v8::Uint32>(GL_RENDERBUFFER_ALPHA_SIZE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RENDERBUFFER_DEPTH_SIZE"), NanNew<v8::Uint32>(GL_RENDERBUFFER_DEPTH_SIZE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RENDERBUFFER_STENCIL_SIZE"), NanNew<v8::Uint32>(GL_RENDERBUFFER_STENCIL_SIZE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("FRAMEBUFFER_INCOMPLETE_MULTISAMPLE"), NanNew<v8::Uint32>(GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_SAMPLES"), NanNew<v8::Uint32>(GL_MAX_SAMPLES), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("FRAMEBUFFER_SRGB"), NanNew<v8::Uint32>(GL_FRAMEBUFFER_SRGB), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("HALF_FLOAT"), NanNew<v8::Uint32>(GL_HALF_FLOAT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAP_READ_BIT"), NanNew<v8::Uint32>(GL_MAP_READ_BIT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAP_WRITE_BIT"), NanNew<v8::Uint32>(GL_MAP_WRITE_BIT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAP_INVALIDATE_RANGE_BIT"), NanNew<v8::Uint32>(GL_MAP_INVALIDATE_RANGE_BIT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAP_INVALIDATE_BUFFER_BIT"), NanNew<v8::Uint32>(GL_MAP_INVALIDATE_BUFFER_BIT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAP_FLUSH_EXPLICIT_BIT"), NanNew<v8::Uint32>(GL_MAP_FLUSH_EXPLICIT_BIT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAP_UNSYNCHRONIZED_BIT"), NanNew<v8::Uint32>(GL_MAP_UNSYNCHRONIZED_BIT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("COMPRESSED_RED_RGTC1"), NanNew<v8::Uint32>(GL_COMPRESSED_RED_RGTC1), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("COMPRESSED_SIGNED_RED_RGTC1"), NanNew<v8::Uint32>(GL_COMPRESSED_SIGNED_RED_RGTC1), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("COMPRESSED_RG_RGTC2"), NanNew<v8::Uint32>(GL_COMPRESSED_RG_RGTC2), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("COMPRESSED_SIGNED_RG_RGTC2"), NanNew<v8::Uint32>(GL_COMPRESSED_SIGNED_RG_RGTC2), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RG"), NanNew<v8::Uint32>(GL_RG), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RG_INTEGER"), NanNew<v8::Uint32>(GL_RG_INTEGER), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("R8"), NanNew<v8::Uint32>(GL_R8), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("R16"), NanNew<v8::Uint32>(GL_R16), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RG8"), NanNew<v8::Uint32>(GL_RG8), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RG16"), NanNew<v8::Uint32>(GL_RG16), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("R16F"), NanNew<v8::Uint32>(GL_R16F), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("R32F"), NanNew<v8::Uint32>(GL_R32F), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RG16F"), NanNew<v8::Uint32>(GL_RG16F), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RG32F"), NanNew<v8::Uint32>(GL_RG32F), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("R8I"), NanNew<v8::Uint32>(GL_R8I), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("R8UI"), NanNew<v8::Uint32>(GL_R8UI), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("R16I"), NanNew<v8::Uint32>(GL_R16I), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("R16UI"), NanNew<v8::Uint32>(GL_R16UI), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("R32I"), NanNew<v8::Uint32>(GL_R32I), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("R32UI"), NanNew<v8::Uint32>(GL_R32UI), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RG8I"), NanNew<v8::Uint32>(GL_RG8I), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RG8UI"), NanNew<v8::Uint32>(GL_RG8UI), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RG16I"), NanNew<v8::Uint32>(GL_RG16I), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RG16UI"), NanNew<v8::Uint32>(GL_RG16UI), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RG32I"), NanNew<v8::Uint32>(GL_RG32I), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RG32UI"), NanNew<v8::Uint32>(GL_RG32UI), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("VERTEX_ARRAY_BINDING"), NanNew<v8::Uint32>(GL_VERTEX_ARRAY_BINDING), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNIFORM_BUFFER"), NanNew<v8::Uint32>(GL_UNIFORM_BUFFER), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNIFORM_BUFFER_BINDING"), NanNew<v8::Uint32>(GL_UNIFORM_BUFFER_BINDING), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNIFORM_BUFFER_START"), NanNew<v8::Uint32>(GL_UNIFORM_BUFFER_START), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNIFORM_BUFFER_SIZE"), NanNew<v8::Uint32>(GL_UNIFORM_BUFFER_SIZE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_VERTEX_UNIFORM_BLOCKS"), NanNew<v8::Uint32>(GL_MAX_VERTEX_UNIFORM_BLOCKS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_GEOMETRY_UNIFORM_BLOCKS"), NanNew<v8::Uint32>(GL_MAX_GEOMETRY_UNIFORM_BLOCKS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_FRAGMENT_UNIFORM_BLOCKS"), NanNew<v8::Uint32>(GL_MAX_FRAGMENT_UNIFORM_BLOCKS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_COMBINED_UNIFORM_BLOCKS"), NanNew<v8::Uint32>(GL_MAX_COMBINED_UNIFORM_BLOCKS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_UNIFORM_BUFFER_BINDINGS"), NanNew<v8::Uint32>(GL_MAX_UNIFORM_BUFFER_BINDINGS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_UNIFORM_BLOCK_SIZE"), NanNew<v8::Uint32>(GL_MAX_UNIFORM_BLOCK_SIZE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS"), NanNew<v8::Uint32>(GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS"), NanNew<v8::Uint32>(GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS"), NanNew<v8::Uint32>(GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNIFORM_BUFFER_OFFSET_ALIGNMENT"), NanNew<v8::Uint32>(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH"), NanNew<v8::Uint32>(GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("ACTIVE_UNIFORM_BLOCKS"), NanNew<v8::Uint32>(GL_ACTIVE_UNIFORM_BLOCKS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNIFORM_TYPE"), NanNew<v8::Uint32>(GL_UNIFORM_TYPE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNIFORM_SIZE"), NanNew<v8::Uint32>(GL_UNIFORM_SIZE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNIFORM_NAME_LENGTH"), NanNew<v8::Uint32>(GL_UNIFORM_NAME_LENGTH), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNIFORM_BLOCK_INDEX"), NanNew<v8::Uint32>(GL_UNIFORM_BLOCK_INDEX), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNIFORM_OFFSET"), NanNew<v8::Uint32>(GL_UNIFORM_OFFSET), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNIFORM_ARRAY_STRIDE"), NanNew<v8::Uint32>(GL_UNIFORM_ARRAY_STRIDE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNIFORM_MATRIX_STRIDE"), NanNew<v8::Uint32>(GL_UNIFORM_MATRIX_STRIDE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNIFORM_IS_ROW_MAJOR"), NanNew<v8::Uint32>(GL_UNIFORM_IS_ROW_MAJOR), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNIFORM_BLOCK_BINDING"), NanNew<v8::Uint32>(GL_UNIFORM_BLOCK_BINDING), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNIFORM_BLOCK_DATA_SIZE"), NanNew<v8::Uint32>(GL_UNIFORM_BLOCK_DATA_SIZE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNIFORM_BLOCK_NAME_LENGTH"), NanNew<v8::Uint32>(GL_UNIFORM_BLOCK_NAME_LENGTH), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNIFORM_BLOCK_ACTIVE_UNIFORMS"), NanNew<v8::Uint32>(GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES"), NanNew<v8::Uint32>(GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER"), NanNew<v8::Uint32>(GL_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNIFORM_BLOCK_REFERENCED_BY_GEOMETRY_SHADER"), NanNew<v8::Uint32>(GL_UNIFORM_BLOCK_REFERENCED_BY_GEOMETRY_SHADER), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER"), NanNew<v8::Uint32>(GL_UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("COPY_READ_BUFFER"), NanNew<v8::Uint32>(GL_COPY_READ_BUFFER), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("COPY_WRITE_BUFFER"), NanNew<v8::Uint32>(GL_COPY_WRITE_BUFFER), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("DEPTH_CLAMP"), NanNew<v8::Uint32>(GL_DEPTH_CLAMP), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("FIRST_VERTEX_CONVENTION"), NanNew<v8::Uint32>(GL_FIRST_VERTEX_CONVENTION), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("LAST_VERTEX_CONVENTION"), NanNew<v8::Uint32>(GL_LAST_VERTEX_CONVENTION), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("PROVOKING_VERTEX"), NanNew<v8::Uint32>(GL_PROVOKING_VERTEX), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_CUBE_MAP_SEAMLESS"), NanNew<v8::Uint32>(GL_TEXTURE_CUBE_MAP_SEAMLESS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_SERVER_WAIT_TIMEOUT"), NanNew<v8::Uint32>(GL_MAX_SERVER_WAIT_TIMEOUT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("OBJECT_TYPE"), NanNew<v8::Uint32>(GL_OBJECT_TYPE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("SYNC_CONDITION"), NanNew<v8::Uint32>(GL_SYNC_CONDITION), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("SYNC_STATUS"), NanNew<v8::Uint32>(GL_SYNC_STATUS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("SYNC_FLAGS"), NanNew<v8::Uint32>(GL_SYNC_FLAGS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("SYNC_FENCE"), NanNew<v8::Uint32>(GL_SYNC_FENCE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("SYNC_GPU_COMMANDS_COMPLETE"), NanNew<v8::Uint32>(GL_SYNC_GPU_COMMANDS_COMPLETE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNSIGNALED"), NanNew<v8::Uint32>(GL_UNSIGNALED), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("SIGNALED"), NanNew<v8::Uint32>(GL_SIGNALED), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("ALREADY_SIGNALED"), NanNew<v8::Uint32>(GL_ALREADY_SIGNALED), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TIMEOUT_EXPIRED"), NanNew<v8::Uint32>(GL_TIMEOUT_EXPIRED), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("CONDITION_SATISFIED"), NanNew<v8::Uint32>(GL_CONDITION_SATISFIED), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("WAIT_FAILED"), NanNew<v8::Uint32>(GL_WAIT_FAILED), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("SYNC_FLUSH_COMMANDS_BIT"), NanNew<v8::Uint32>(GL_SYNC_FLUSH_COMMANDS_BIT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("SAMPLE_POSITION"), NanNew<v8::Uint32>(GL_SAMPLE_POSITION), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("SAMPLE_MASK"), NanNew<v8::Uint32>(GL_SAMPLE_MASK), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("SAMPLE_MASK_VALUE"), NanNew<v8::Uint32>(GL_SAMPLE_MASK_VALUE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_SAMPLE_MASK_WORDS"), NanNew<v8::Uint32>(GL_MAX_SAMPLE_MASK_WORDS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_2D_MULTISAMPLE"), NanNew<v8::Uint32>(GL_TEXTURE_2D_MULTISAMPLE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("PROXY_TEXTURE_2D_MULTISAMPLE"), NanNew<v8::Uint32>(GL_PROXY_TEXTURE_2D_MULTISAMPLE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_2D_MULTISAMPLE_ARRAY"), NanNew<v8::Uint32>(GL_TEXTURE_2D_MULTISAMPLE_ARRAY), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("PROXY_TEXTURE_2D_MULTISAMPLE_ARRAY"), NanNew<v8::Uint32>(GL_PROXY_TEXTURE_2D_MULTISAMPLE_ARRAY), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_BINDING_2D_MULTISAMPLE"), NanNew<v8::Uint32>(GL_TEXTURE_BINDING_2D_MULTISAMPLE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY"), NanNew<v8::Uint32>(GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_SAMPLES"), NanNew<v8::Uint32>(GL_TEXTURE_SAMPLES), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_FIXED_SAMPLE_LOCATIONS"), NanNew<v8::Uint32>(GL_TEXTURE_FIXED_SAMPLE_LOCATIONS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("SAMPLER_2D_MULTISAMPLE"), NanNew<v8::Uint32>(GL_SAMPLER_2D_MULTISAMPLE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("INT_SAMPLER_2D_MULTISAMPLE"), NanNew<v8::Uint32>(GL_INT_SAMPLER_2D_MULTISAMPLE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE"), NanNew<v8::Uint32>(GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("SAMPLER_2D_MULTISAMPLE_ARRAY"), NanNew<v8::Uint32>(GL_SAMPLER_2D_MULTISAMPLE_ARRAY), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("INT_SAMPLER_2D_MULTISAMPLE_ARRAY"), NanNew<v8::Uint32>(GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY"), NanNew<v8::Uint32>(GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_COLOR_TEXTURE_SAMPLES"), NanNew<v8::Uint32>(GL_MAX_COLOR_TEXTURE_SAMPLES), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_DEPTH_TEXTURE_SAMPLES"), NanNew<v8::Uint32>(GL_MAX_DEPTH_TEXTURE_SAMPLES), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_INTEGER_SAMPLES"), NanNew<v8::Uint32>(GL_MAX_INTEGER_SAMPLES), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("SRC1_COLOR"), NanNew<v8::Uint32>(GL_SRC1_COLOR), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("SRC1_ALPHA"), NanNew<v8::Uint32>(GL_SRC1_ALPHA), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("ONE_MINUS_SRC1_COLOR"), NanNew<v8::Uint32>(GL_ONE_MINUS_SRC1_COLOR), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("ONE_MINUS_SRC1_ALPHA"), NanNew<v8::Uint32>(GL_ONE_MINUS_SRC1_ALPHA), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_DUAL_SOURCE_DRAW_BUFFERS"), NanNew<v8::Uint32>(GL_MAX_DUAL_SOURCE_DRAW_BUFFERS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("ANY_SAMPLES_PASSED"), NanNew<v8::Uint32>(GL_ANY_SAMPLES_PASSED), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("SAMPLER_BINDING"), NanNew<v8::Uint32>(GL_SAMPLER_BINDING), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RGB10_A2UI"), NanNew<v8::Uint32>(GL_RGB10_A2UI), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_SWIZZLE_R"), NanNew<v8::Uint32>(GL_TEXTURE_SWIZZLE_R), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_SWIZZLE_G"), NanNew<v8::Uint32>(GL_TEXTURE_SWIZZLE_G), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_SWIZZLE_B"), NanNew<v8::Uint32>(GL_TEXTURE_SWIZZLE_B), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_SWIZZLE_A"), NanNew<v8::Uint32>(GL_TEXTURE_SWIZZLE_A), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TEXTURE_SWIZZLE_RGBA"), NanNew<v8::Uint32>(GL_TEXTURE_SWIZZLE_RGBA), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TIME_ELAPSED"), NanNew<v8::Uint32>(GL_TIME_ELAPSED), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TIMESTAMP"), NanNew<v8::Uint32>(GL_TIMESTAMP), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("INT_2_10_10_10_REV"), NanNew<v8::Uint32>(GL_INT_2_10_10_10_REV), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("DRAW_INDIRECT_BUFFER"), NanNew<v8::Uint32>(GL_DRAW_INDIRECT_BUFFER), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("DRAW_INDIRECT_BUFFER_BINDING"), NanNew<v8::Uint32>(GL_DRAW_INDIRECT_BUFFER_BINDING), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("GEOMETRY_SHADER_INVOCATIONS"), NanNew<v8::Uint32>(GL_GEOMETRY_SHADER_INVOCATIONS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_GEOMETRY_SHADER_INVOCATIONS"), NanNew<v8::Uint32>(GL_MAX_GEOMETRY_SHADER_INVOCATIONS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MIN_FRAGMENT_INTERPOLATION_OFFSET"), NanNew<v8::Uint32>(GL_MIN_FRAGMENT_INTERPOLATION_OFFSET), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_FRAGMENT_INTERPOLATION_OFFSET"), NanNew<v8::Uint32>(GL_MAX_FRAGMENT_INTERPOLATION_OFFSET), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("FRAGMENT_INTERPOLATION_OFFSET_BITS"), NanNew<v8::Uint32>(GL_FRAGMENT_INTERPOLATION_OFFSET_BITS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("DOUBLE_VEC2"), NanNew<v8::Uint32>(GL_DOUBLE_VEC2), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("DOUBLE_VEC3"), NanNew<v8::Uint32>(GL_DOUBLE_VEC3), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("DOUBLE_VEC4"), NanNew<v8::Uint32>(GL_DOUBLE_VEC4), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("DOUBLE_MAT2"), NanNew<v8::Uint32>(GL_DOUBLE_MAT2), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("DOUBLE_MAT3"), NanNew<v8::Uint32>(GL_DOUBLE_MAT3), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("DOUBLE_MAT4"), NanNew<v8::Uint32>(GL_DOUBLE_MAT4), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("DOUBLE_MAT2x3"), NanNew<v8::Uint32>(GL_DOUBLE_MAT2x3), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("DOUBLE_MAT2x4"), NanNew<v8::Uint32>(GL_DOUBLE_MAT2x4), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("DOUBLE_MAT3x2"), NanNew<v8::Uint32>(GL_DOUBLE_MAT3x2), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("DOUBLE_MAT3x4"), NanNew<v8::Uint32>(GL_DOUBLE_MAT3x4), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("DOUBLE_MAT4x2"), NanNew<v8::Uint32>(GL_DOUBLE_MAT4x2), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("DOUBLE_MAT4x3"), NanNew<v8::Uint32>(GL_DOUBLE_MAT4x3), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("ACTIVE_SUBROUTINES"), NanNew<v8::Uint32>(GL_ACTIVE_SUBROUTINES), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("ACTIVE_SUBROUTINE_UNIFORMS"), NanNew<v8::Uint32>(GL_ACTIVE_SUBROUTINE_UNIFORMS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("ACTIVE_SUBROUTINE_UNIFORM_LOCATIONS"), NanNew<v8::Uint32>(GL_ACTIVE_SUBROUTINE_UNIFORM_LOCATIONS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("ACTIVE_SUBROUTINE_MAX_LENGTH"), NanNew<v8::Uint32>(GL_ACTIVE_SUBROUTINE_MAX_LENGTH), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("ACTIVE_SUBROUTINE_UNIFORM_MAX_LENGTH"), NanNew<v8::Uint32>(GL_ACTIVE_SUBROUTINE_UNIFORM_MAX_LENGTH), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_SUBROUTINES"), NanNew<v8::Uint32>(GL_MAX_SUBROUTINES), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_SUBROUTINE_UNIFORM_LOCATIONS"), NanNew<v8::Uint32>(GL_MAX_SUBROUTINE_UNIFORM_LOCATIONS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("NUM_COMPATIBLE_SUBROUTINES"), NanNew<v8::Uint32>(GL_NUM_COMPATIBLE_SUBROUTINES), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("COMPATIBLE_SUBROUTINES"), NanNew<v8::Uint32>(GL_COMPATIBLE_SUBROUTINES), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("PATCHES"), NanNew<v8::Uint32>(GL_PATCHES), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("PATCH_VERTICES"), NanNew<v8::Uint32>(GL_PATCH_VERTICES), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("PATCH_DEFAULT_INNER_LEVEL"), NanNew<v8::Uint32>(GL_PATCH_DEFAULT_INNER_LEVEL), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("PATCH_DEFAULT_OUTER_LEVEL"), NanNew<v8::Uint32>(GL_PATCH_DEFAULT_OUTER_LEVEL), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TESS_CONTROL_OUTPUT_VERTICES"), NanNew<v8::Uint32>(GL_TESS_CONTROL_OUTPUT_VERTICES), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TESS_GEN_MODE"), NanNew<v8::Uint32>(GL_TESS_GEN_MODE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TESS_GEN_SPACING"), NanNew<v8::Uint32>(GL_TESS_GEN_SPACING), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TESS_GEN_VERTEX_ORDER"), NanNew<v8::Uint32>(GL_TESS_GEN_VERTEX_ORDER), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TESS_GEN_POINT_MODE"), NanNew<v8::Uint32>(GL_TESS_GEN_POINT_MODE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("QUADS"), NanNew<v8::Uint32>(GL_QUADS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("ISOLINES"), NanNew<v8::Uint32>(GL_ISOLINES), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("FRACTIONAL_ODD"), NanNew<v8::Uint32>(GL_FRACTIONAL_ODD), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("FRACTIONAL_EVEN"), NanNew<v8::Uint32>(GL_FRACTIONAL_EVEN), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_PATCH_VERTICES"), NanNew<v8::Uint32>(GL_MAX_PATCH_VERTICES), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_TESS_GEN_LEVEL"), NanNew<v8::Uint32>(GL_MAX_TESS_GEN_LEVEL), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_TESS_CONTROL_UNIFORM_COMPONENTS"), NanNew<v8::Uint32>(GL_MAX_TESS_CONTROL_UNIFORM_COMPONENTS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_TESS_EVALUATION_UNIFORM_COMPONENTS"), NanNew<v8::Uint32>(GL_MAX_TESS_EVALUATION_UNIFORM_COMPONENTS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_TESS_CONTROL_TEXTURE_IMAGE_UNITS"), NanNew<v8::Uint32>(GL_MAX_TESS_CONTROL_TEXTURE_IMAGE_UNITS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_TESS_EVALUATION_TEXTURE_IMAGE_UNITS"), NanNew<v8::Uint32>(GL_MAX_TESS_EVALUATION_TEXTURE_IMAGE_UNITS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_TESS_CONTROL_OUTPUT_COMPONENTS"), NanNew<v8::Uint32>(GL_MAX_TESS_CONTROL_OUTPUT_COMPONENTS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_TESS_PATCH_COMPONENTS"), NanNew<v8::Uint32>(GL_MAX_TESS_PATCH_COMPONENTS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_TESS_CONTROL_TOTAL_OUTPUT_COMPONENTS"), NanNew<v8::Uint32>(GL_MAX_TESS_CONTROL_TOTAL_OUTPUT_COMPONENTS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_TESS_EVALUATION_OUTPUT_COMPONENTS"), NanNew<v8::Uint32>(GL_MAX_TESS_EVALUATION_OUTPUT_COMPONENTS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_TESS_CONTROL_UNIFORM_BLOCKS"), NanNew<v8::Uint32>(GL_MAX_TESS_CONTROL_UNIFORM_BLOCKS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_TESS_EVALUATION_UNIFORM_BLOCKS"), NanNew<v8::Uint32>(GL_MAX_TESS_EVALUATION_UNIFORM_BLOCKS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_TESS_CONTROL_INPUT_COMPONENTS"), NanNew<v8::Uint32>(GL_MAX_TESS_CONTROL_INPUT_COMPONENTS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_TESS_EVALUATION_INPUT_COMPONENTS"), NanNew<v8::Uint32>(GL_MAX_TESS_EVALUATION_INPUT_COMPONENTS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_COMBINED_TESS_CONTROL_UNIFORM_COMPONENTS"), NanNew<v8::Uint32>(GL_MAX_COMBINED_TESS_CONTROL_UNIFORM_COMPONENTS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_COMBINED_TESS_EVALUATION_UNIFORM_COMPONENTS"), NanNew<v8::Uint32>(GL_MAX_COMBINED_TESS_EVALUATION_UNIFORM_COMPONENTS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNIFORM_BLOCK_REFERENCED_BY_TESS_CONTROL_SHADER"), NanNew<v8::Uint32>(GL_UNIFORM_BLOCK_REFERENCED_BY_TESS_CONTROL_SHADER), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNIFORM_BLOCK_REFERENCED_BY_TESS_EVALUATION_SHADER"), NanNew<v8::Uint32>(GL_UNIFORM_BLOCK_REFERENCED_BY_TESS_EVALUATION_SHADER), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TESS_EVALUATION_SHADER"), NanNew<v8::Uint32>(GL_TESS_EVALUATION_SHADER), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TESS_CONTROL_SHADER"), NanNew<v8::Uint32>(GL_TESS_CONTROL_SHADER), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TRANSFORM_FEEDBACK"), NanNew<v8::Uint32>(GL_TRANSFORM_FEEDBACK), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TRANSFORM_FEEDBACK_BUFFER_PAUSED"), NanNew<v8::Uint32>(GL_TRANSFORM_FEEDBACK_BUFFER_PAUSED), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TRANSFORM_FEEDBACK_BUFFER_ACTIVE"), NanNew<v8::Uint32>(GL_TRANSFORM_FEEDBACK_BUFFER_ACTIVE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TRANSFORM_FEEDBACK_BINDING"), NanNew<v8::Uint32>(GL_TRANSFORM_FEEDBACK_BINDING), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_TRANSFORM_FEEDBACK_BUFFERS"), NanNew<v8::Uint32>(GL_MAX_TRANSFORM_FEEDBACK_BUFFERS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_VERTEX_STREAMS"), NanNew<v8::Uint32>(GL_MAX_VERTEX_STREAMS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("FIXED"), NanNew<v8::Uint32>(GL_FIXED), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("IMPLEMENTATION_COLOR_READ_TYPE"), NanNew<v8::Uint32>(GL_IMPLEMENTATION_COLOR_READ_TYPE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("IMPLEMENTATION_COLOR_READ_FORMAT"), NanNew<v8::Uint32>(GL_IMPLEMENTATION_COLOR_READ_FORMAT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("LOW_FLOAT"), NanNew<v8::Uint32>(GL_LOW_FLOAT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MEDIUM_FLOAT"), NanNew<v8::Uint32>(GL_MEDIUM_FLOAT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("HIGH_FLOAT"), NanNew<v8::Uint32>(GL_HIGH_FLOAT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("LOW_INT"), NanNew<v8::Uint32>(GL_LOW_INT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MEDIUM_INT"), NanNew<v8::Uint32>(GL_MEDIUM_INT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("HIGH_INT"), NanNew<v8::Uint32>(GL_HIGH_INT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("SHADER_COMPILER"), NanNew<v8::Uint32>(GL_SHADER_COMPILER), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("SHADER_BINARY_FORMATS"), NanNew<v8::Uint32>(GL_SHADER_BINARY_FORMATS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("NUM_SHADER_BINARY_FORMATS"), NanNew<v8::Uint32>(GL_NUM_SHADER_BINARY_FORMATS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_VERTEX_UNIFORM_VECTORS"), NanNew<v8::Uint32>(GL_MAX_VERTEX_UNIFORM_VECTORS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_VARYING_VECTORS"), NanNew<v8::Uint32>(GL_MAX_VARYING_VECTORS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_FRAGMENT_UNIFORM_VECTORS"), NanNew<v8::Uint32>(GL_MAX_FRAGMENT_UNIFORM_VECTORS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("RGB565"), NanNew<v8::Uint32>(GL_RGB565), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("PROGRAM_BINARY_RETRIEVABLE_HINT"), NanNew<v8::Uint32>(GL_PROGRAM_BINARY_RETRIEVABLE_HINT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("PROGRAM_BINARY_LENGTH"), NanNew<v8::Uint32>(GL_PROGRAM_BINARY_LENGTH), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("NUM_PROGRAM_BINARY_FORMATS"), NanNew<v8::Uint32>(GL_NUM_PROGRAM_BINARY_FORMATS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("PROGRAM_BINARY_FORMATS"), NanNew<v8::Uint32>(GL_PROGRAM_BINARY_FORMATS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("VERTEX_SHADER_BIT"), NanNew<v8::Uint32>(GL_VERTEX_SHADER_BIT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("FRAGMENT_SHADER_BIT"), NanNew<v8::Uint32>(GL_FRAGMENT_SHADER_BIT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("GEOMETRY_SHADER_BIT"), NanNew<v8::Uint32>(GL_GEOMETRY_SHADER_BIT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TESS_CONTROL_SHADER_BIT"), NanNew<v8::Uint32>(GL_TESS_CONTROL_SHADER_BIT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("TESS_EVALUATION_SHADER_BIT"), NanNew<v8::Uint32>(GL_TESS_EVALUATION_SHADER_BIT), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("ALL_SHADER_BITS"), NanNew<v8::Uint32>(GL_ALL_SHADER_BITS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("PROGRAM_SEPARABLE"), NanNew<v8::Uint32>(GL_PROGRAM_SEPARABLE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("ACTIVE_PROGRAM"), NanNew<v8::Uint32>(GL_ACTIVE_PROGRAM), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("PROGRAM_PIPELINE_BINDING"), NanNew<v8::Uint32>(GL_PROGRAM_PIPELINE_BINDING), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("MAX_VIEWPORTS"), NanNew<v8::Uint32>(GL_MAX_VIEWPORTS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("VIEWPORT_SUBPIXEL_BITS"), NanNew<v8::Uint32>(GL_VIEWPORT_SUBPIXEL_BITS), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("VIEWPORT_BOUNDS_RANGE"), NanNew<v8::Uint32>(GL_VIEWPORT_BOUNDS_RANGE), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("LAYER_PROVOKING_VERTEX"), NanNew<v8::Uint32>(GL_LAYER_PROVOKING_VERTEX), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("VIEWPORT_INDEX_PROVOKING_VERTEX"), NanNew<v8::Uint32>(GL_VIEWPORT_INDEX_PROVOKING_VERTEX), v8::ReadOnly);
    exports->Set(NanNew<v8::String>("UNDEFINED_VERTEX"), NanNew<v8::Uint32>(GL_UNDEFINED_VERTEX), v8::ReadOnly);
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
#if (GL_CLIP_DISTANCE6 != 12294)
 #error "GL3 constant CLIP_DISTANCE6's value is incorrect."
#endif
#if (GL_CLIP_DISTANCE7 != 12295)
 #error "GL3 constant CLIP_DISTANCE7's value is incorrect."
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
NAN_METHOD(EXPORT_cullFace) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    glCullFace(arg0);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_frontFace) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    glFrontFace(arg0);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_hint) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    glHint(arg0, arg1);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_lineWidth) {
    NanScope();
    // None
    GLfloat arg0;
    arg0 = (float)args[0]->NumberValue();
    glLineWidth(arg0);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_pointSize) {
    NanScope();
    // None
    GLfloat arg0;
    arg0 = (float)args[0]->NumberValue();
    glPointSize(arg0);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_polygonMode) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    glPolygonMode(arg0, arg1);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_scissor) {
    NanScope();
    // None
    GLint arg0;
    arg0 = args[0]->Int32Value();
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLsizei arg2;
    arg2 = args[2]->Int32Value();
    GLsizei arg3;
    arg3 = args[3]->Int32Value();
    glScissor(arg0, arg1, arg2, arg3);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_texParameterf) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLfloat arg2;
    arg2 = (float)args[2]->NumberValue();
    glTexParameterf(arg0, arg1, arg2);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_texParameterfv) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    const GLfloat *arg2;
    GLfloat* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (const GLfloat*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLfloat[arg2_array->Length()];
        arg2 = (const GLfloat*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (const GLfloat*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLfloat*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glTexParameterfv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_texParameteri) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLint arg2;
    arg2 = args[2]->Int32Value();
    glTexParameteri(arg0, arg1, arg2);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_texParameteriv) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    const GLint *arg2;
    GLint* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (const GLint*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLint[arg2_array->Length()];
        arg2 = (const GLint*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (const GLint*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glTexParameteriv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_texImage1D) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLint arg2;
    arg2 = args[2]->Int32Value();
    GLsizei arg3;
    arg3 = args[3]->Int32Value();
    GLint arg4;
    arg4 = args[4]->Int32Value();
    GLenum arg5;
    arg5 = args[5]->Uint32Value();
    GLenum arg6;
    arg6 = args[6]->Uint32Value();
    const GLvoid *arg7;
    GLubyte* arg7_nonconst = NULL;
    if(args[7]->IsString()) {
        NanUtf8String arg7_utf8(args[7]);
        arg7 = (const GLvoid*)(*arg7_utf8);
    } else if(args[7]->IsArray()) {
        v8::Handle<v8::Array> arg7_array = v8::Handle<v8::Array>::Cast(args[7]);
        arg7_nonconst = new GLubyte[arg7_array->Length()];
        arg7 = (const GLvoid*)arg7_nonconst;
    } else if(args[7]->IsObject()) {
        arg7 = (const GLvoid*)glbind_get_buffer_data(args[7]);
    } else if(args[7]->IsNumber()) {
        arg7 = (const GLvoid*)(args[7]->IntegerValue());
    } else {
        arg7 = NULL;
    }
    glTexImage1D(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
    if(arg7_nonconst) delete [] arg7_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_texImage2D) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLint arg2;
    arg2 = args[2]->Int32Value();
    GLsizei arg3;
    arg3 = args[3]->Int32Value();
    GLsizei arg4;
    arg4 = args[4]->Int32Value();
    GLint arg5;
    arg5 = args[5]->Int32Value();
    GLenum arg6;
    arg6 = args[6]->Uint32Value();
    GLenum arg7;
    arg7 = args[7]->Uint32Value();
    const GLvoid *arg8;
    GLubyte* arg8_nonconst = NULL;
    if(args[8]->IsString()) {
        NanUtf8String arg8_utf8(args[8]);
        arg8 = (const GLvoid*)(*arg8_utf8);
    } else if(args[8]->IsArray()) {
        v8::Handle<v8::Array> arg8_array = v8::Handle<v8::Array>::Cast(args[8]);
        arg8_nonconst = new GLubyte[arg8_array->Length()];
        arg8 = (const GLvoid*)arg8_nonconst;
    } else if(args[8]->IsObject()) {
        arg8 = (const GLvoid*)glbind_get_buffer_data(args[8]);
    } else if(args[8]->IsNumber()) {
        arg8 = (const GLvoid*)(args[8]->IntegerValue());
    } else {
        arg8 = NULL;
    }
    glTexImage2D(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
    if(arg8_nonconst) delete [] arg8_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_drawBuffer) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    glDrawBuffer(arg0);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_clear) {
    NanScope();
    // None
    GLbitfield arg0;
    arg0 = args[0]->Uint32Value();
    glClear(arg0);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_clearColor) {
    NanScope();
    // None
    GLfloat arg0;
    arg0 = (float)args[0]->NumberValue();
    GLfloat arg1;
    arg1 = (float)args[1]->NumberValue();
    GLfloat arg2;
    arg2 = (float)args[2]->NumberValue();
    GLfloat arg3;
    arg3 = (float)args[3]->NumberValue();
    glClearColor(arg0, arg1, arg2, arg3);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_clearStencil) {
    NanScope();
    // None
    GLint arg0;
    arg0 = args[0]->Int32Value();
    glClearStencil(arg0);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_clearDepth) {
    NanScope();
    // None
    GLclampd arg0;
    arg0 = args[0]->NumberValue();
    glClearDepth(arg0);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_stencilMask) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    glStencilMask(arg0);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_colorMask) {
    NanScope();
    // None
    GLboolean arg0;
    arg0 = args[0]->Uint32Value();
    GLboolean arg1;
    arg1 = args[1]->Uint32Value();
    GLboolean arg2;
    arg2 = args[2]->Uint32Value();
    GLboolean arg3;
    arg3 = args[3]->Uint32Value();
    glColorMask(arg0, arg1, arg2, arg3);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_depthMask) {
    NanScope();
    // None
    GLboolean arg0;
    arg0 = args[0]->Uint32Value();
    glDepthMask(arg0);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_disable) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    glDisable(arg0);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_enable) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    glEnable(arg0);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_finish) {
    NanScope();
    // None
    glFinish();
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_flush) {
    NanScope();
    // None
    glFlush();
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_blendFunc) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    glBlendFunc(arg0, arg1);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_logicOp) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    glLogicOp(arg0);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_stencilFunc) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLuint arg2;
    arg2 = args[2]->Uint32Value();
    glStencilFunc(arg0, arg1, arg2);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_stencilOp) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLenum arg2;
    arg2 = args[2]->Uint32Value();
    glStencilOp(arg0, arg1, arg2);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_depthFunc) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    glDepthFunc(arg0);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_pixelStoref) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLfloat arg1;
    arg1 = (float)args[1]->NumberValue();
    glPixelStoref(arg0, arg1);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_pixelStorei) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLint arg1;
    arg1 = args[1]->Int32Value();
    glPixelStorei(arg0, arg1);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_readBuffer) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    glReadBuffer(arg0);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_readPixels) {
    NanScope();
    // None
    GLint arg0;
    arg0 = args[0]->Int32Value();
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLsizei arg2;
    arg2 = args[2]->Int32Value();
    GLsizei arg3;
    arg3 = args[3]->Int32Value();
    GLenum arg4;
    arg4 = args[4]->Uint32Value();
    GLenum arg5;
    arg5 = args[5]->Uint32Value();
    GLvoid *arg6;
    GLubyte* arg6_nonconst = NULL;
    if(args[6]->IsString()) {
        NanUtf8String arg6_utf8(args[6]);
        arg6 = (GLvoid*)(*arg6_utf8);
    } else if(args[6]->IsArray()) {
        v8::Handle<v8::Array> arg6_array = v8::Handle<v8::Array>::Cast(args[6]);
        arg6_nonconst = new GLubyte[arg6_array->Length()];
        arg6 = (GLvoid*)arg6_nonconst;
    } else if(args[6]->IsObject()) {
        arg6 = (GLvoid*)glbind_get_buffer_data(args[6]);
    } else if(args[6]->IsNumber()) {
        arg6 = (GLvoid*)(args[6]->IntegerValue());
    } else {
        arg6 = NULL;
    }
    glReadPixels(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
    if(arg6_nonconst) delete [] arg6_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getBooleanv) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLboolean *arg1;
    GLboolean* arg1_nonconst = NULL;
    if(args[1]->IsString()) {
        NanUtf8String arg1_utf8(args[1]);
        arg1 = (GLboolean*)(*arg1_utf8);
    } else if(args[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(args[1]);
        arg1_nonconst = new GLboolean[arg1_array->Length()];
        arg1 = (GLboolean*)arg1_nonconst;
    } else if(args[1]->IsObject()) {
        arg1 = (GLboolean*)glbind_get_buffer_data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (GLboolean*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glGetBooleanv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getDoublev) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLdouble *arg1;
    GLdouble* arg1_nonconst = NULL;
    if(args[1]->IsString()) {
        NanUtf8String arg1_utf8(args[1]);
        arg1 = (GLdouble*)(*arg1_utf8);
    } else if(args[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(args[1]);
        arg1_nonconst = new GLdouble[arg1_array->Length()];
        arg1 = (GLdouble*)arg1_nonconst;
    } else if(args[1]->IsObject()) {
        arg1 = (GLdouble*)glbind_get_buffer_data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (GLdouble*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glGetDoublev(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getError) {
    NanScope();
    // <gltypes.Type instance at 0x104edf7e8>
    GLenum result;
    result = glGetError();
    v8::Handle<v8::Value> result_js = NanNew<v8::Uint32>(result);
    NanReturnValue(result_js);
}
NAN_METHOD(EXPORT_getFloatv) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLfloat *arg1;
    GLfloat* arg1_nonconst = NULL;
    if(args[1]->IsString()) {
        NanUtf8String arg1_utf8(args[1]);
        arg1 = (GLfloat*)(*arg1_utf8);
    } else if(args[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(args[1]);
        arg1_nonconst = new GLfloat[arg1_array->Length()];
        arg1 = (GLfloat*)arg1_nonconst;
    } else if(args[1]->IsObject()) {
        arg1 = (GLfloat*)glbind_get_buffer_data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (GLfloat*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glGetFloatv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getIntegerv) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLint *arg1;
    GLint* arg1_nonconst = NULL;
    if(args[1]->IsString()) {
        NanUtf8String arg1_utf8(args[1]);
        arg1 = (GLint*)(*arg1_utf8);
    } else if(args[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(args[1]);
        arg1_nonconst = new GLint[arg1_array->Length()];
        arg1 = (GLint*)arg1_nonconst;
    } else if(args[1]->IsObject()) {
        arg1 = (GLint*)glbind_get_buffer_data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (GLint*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glGetIntegerv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getString) {
    NanScope();
    // <gltypes.UStringType instance at 0x104edfcb0>
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    const GLubyte* result;
    result = glGetString(arg0);
    v8::Handle<v8::Value> result_js;
    if(result) {
        result_js = NanNew<v8::String>((const char*)result, strlen((const char*)result));
    } else {
        result_js = NanUndefined();
    }
    NanReturnValue(result_js);
}
NAN_METHOD(EXPORT_getTexImage) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLenum arg2;
    arg2 = args[2]->Uint32Value();
    GLenum arg3;
    arg3 = args[3]->Uint32Value();
    GLvoid *arg4;
    GLubyte* arg4_nonconst = NULL;
    if(args[4]->IsString()) {
        NanUtf8String arg4_utf8(args[4]);
        arg4 = (GLvoid*)(*arg4_utf8);
    } else if(args[4]->IsArray()) {
        v8::Handle<v8::Array> arg4_array = v8::Handle<v8::Array>::Cast(args[4]);
        arg4_nonconst = new GLubyte[arg4_array->Length()];
        arg4 = (GLvoid*)arg4_nonconst;
    } else if(args[4]->IsObject()) {
        arg4 = (GLvoid*)glbind_get_buffer_data(args[4]);
    } else if(args[4]->IsNumber()) {
        arg4 = (GLvoid*)(args[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glGetTexImage(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getTexParameterfv) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLfloat *arg2;
    GLfloat* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (GLfloat*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLfloat[arg2_array->Length()];
        arg2 = (GLfloat*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (GLfloat*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLfloat*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetTexParameterfv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getTexParameteriv) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLint *arg2;
    GLint* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (GLint*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLint[arg2_array->Length()];
        arg2 = (GLint*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (GLint*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetTexParameteriv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getTexLevelParameterfv) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLenum arg2;
    arg2 = args[2]->Uint32Value();
    GLfloat *arg3;
    GLfloat* arg3_nonconst = NULL;
    if(args[3]->IsString()) {
        NanUtf8String arg3_utf8(args[3]);
        arg3 = (GLfloat*)(*arg3_utf8);
    } else if(args[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(args[3]);
        arg3_nonconst = new GLfloat[arg3_array->Length()];
        arg3 = (GLfloat*)arg3_nonconst;
    } else if(args[3]->IsObject()) {
        arg3 = (GLfloat*)glbind_get_buffer_data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (GLfloat*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glGetTexLevelParameterfv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getTexLevelParameteriv) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLenum arg2;
    arg2 = args[2]->Uint32Value();
    GLint *arg3;
    GLint* arg3_nonconst = NULL;
    if(args[3]->IsString()) {
        NanUtf8String arg3_utf8(args[3]);
        arg3 = (GLint*)(*arg3_utf8);
    } else if(args[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(args[3]);
        arg3_nonconst = new GLint[arg3_array->Length()];
        arg3 = (GLint*)arg3_nonconst;
    } else if(args[3]->IsObject()) {
        arg3 = (GLint*)glbind_get_buffer_data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (GLint*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glGetTexLevelParameteriv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_isEnabled) {
    NanScope();
    // <gltypes.Type instance at 0x104edf710>
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLboolean result;
    result = glIsEnabled(arg0);
    v8::Handle<v8::Value> result_js = NanNew<v8::Uint32>(result);
    NanReturnValue(result_js);
}
NAN_METHOD(EXPORT_depthRange) {
    NanScope();
    // None
    GLclampd arg0;
    arg0 = args[0]->NumberValue();
    GLclampd arg1;
    arg1 = args[1]->NumberValue();
    glDepthRange(arg0, arg1);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_viewport) {
    NanScope();
    // None
    GLint arg0;
    arg0 = args[0]->Int32Value();
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLsizei arg2;
    arg2 = args[2]->Int32Value();
    GLsizei arg3;
    arg3 = args[3]->Int32Value();
    glViewport(arg0, arg1, arg2, arg3);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_drawArrays) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLsizei arg2;
    arg2 = args[2]->Int32Value();
    glDrawArrays(arg0, arg1, arg2);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_drawElements) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLsizei arg1;
    arg1 = args[1]->Int32Value();
    GLenum arg2;
    arg2 = args[2]->Uint32Value();
    const GLvoid *arg3;
    GLubyte* arg3_nonconst = NULL;
    if(args[3]->IsString()) {
        NanUtf8String arg3_utf8(args[3]);
        arg3 = (const GLvoid*)(*arg3_utf8);
    } else if(args[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(args[3]);
        arg3_nonconst = new GLubyte[arg3_array->Length()];
        arg3 = (const GLvoid*)arg3_nonconst;
    } else if(args[3]->IsObject()) {
        arg3 = (const GLvoid*)glbind_get_buffer_data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLvoid*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glDrawElements(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_polygonOffset) {
    NanScope();
    // None
    GLfloat arg0;
    arg0 = (float)args[0]->NumberValue();
    GLfloat arg1;
    arg1 = (float)args[1]->NumberValue();
    glPolygonOffset(arg0, arg1);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_copyTexImage1D) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLenum arg2;
    arg2 = args[2]->Uint32Value();
    GLint arg3;
    arg3 = args[3]->Int32Value();
    GLint arg4;
    arg4 = args[4]->Int32Value();
    GLsizei arg5;
    arg5 = args[5]->Int32Value();
    GLint arg6;
    arg6 = args[6]->Int32Value();
    glCopyTexImage1D(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_copyTexImage2D) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLenum arg2;
    arg2 = args[2]->Uint32Value();
    GLint arg3;
    arg3 = args[3]->Int32Value();
    GLint arg4;
    arg4 = args[4]->Int32Value();
    GLsizei arg5;
    arg5 = args[5]->Int32Value();
    GLsizei arg6;
    arg6 = args[6]->Int32Value();
    GLint arg7;
    arg7 = args[7]->Int32Value();
    glCopyTexImage2D(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_copyTexSubImage1D) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLint arg2;
    arg2 = args[2]->Int32Value();
    GLint arg3;
    arg3 = args[3]->Int32Value();
    GLint arg4;
    arg4 = args[4]->Int32Value();
    GLsizei arg5;
    arg5 = args[5]->Int32Value();
    glCopyTexSubImage1D(arg0, arg1, arg2, arg3, arg4, arg5);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_copyTexSubImage2D) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLint arg2;
    arg2 = args[2]->Int32Value();
    GLint arg3;
    arg3 = args[3]->Int32Value();
    GLint arg4;
    arg4 = args[4]->Int32Value();
    GLint arg5;
    arg5 = args[5]->Int32Value();
    GLsizei arg6;
    arg6 = args[6]->Int32Value();
    GLsizei arg7;
    arg7 = args[7]->Int32Value();
    glCopyTexSubImage2D(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_texSubImage1D) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLint arg2;
    arg2 = args[2]->Int32Value();
    GLsizei arg3;
    arg3 = args[3]->Int32Value();
    GLenum arg4;
    arg4 = args[4]->Uint32Value();
    GLenum arg5;
    arg5 = args[5]->Uint32Value();
    const GLvoid *arg6;
    GLubyte* arg6_nonconst = NULL;
    if(args[6]->IsString()) {
        NanUtf8String arg6_utf8(args[6]);
        arg6 = (const GLvoid*)(*arg6_utf8);
    } else if(args[6]->IsArray()) {
        v8::Handle<v8::Array> arg6_array = v8::Handle<v8::Array>::Cast(args[6]);
        arg6_nonconst = new GLubyte[arg6_array->Length()];
        arg6 = (const GLvoid*)arg6_nonconst;
    } else if(args[6]->IsObject()) {
        arg6 = (const GLvoid*)glbind_get_buffer_data(args[6]);
    } else if(args[6]->IsNumber()) {
        arg6 = (const GLvoid*)(args[6]->IntegerValue());
    } else {
        arg6 = NULL;
    }
    glTexSubImage1D(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
    if(arg6_nonconst) delete [] arg6_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_texSubImage2D) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLint arg2;
    arg2 = args[2]->Int32Value();
    GLint arg3;
    arg3 = args[3]->Int32Value();
    GLsizei arg4;
    arg4 = args[4]->Int32Value();
    GLsizei arg5;
    arg5 = args[5]->Int32Value();
    GLenum arg6;
    arg6 = args[6]->Uint32Value();
    GLenum arg7;
    arg7 = args[7]->Uint32Value();
    const GLvoid *arg8;
    GLubyte* arg8_nonconst = NULL;
    if(args[8]->IsString()) {
        NanUtf8String arg8_utf8(args[8]);
        arg8 = (const GLvoid*)(*arg8_utf8);
    } else if(args[8]->IsArray()) {
        v8::Handle<v8::Array> arg8_array = v8::Handle<v8::Array>::Cast(args[8]);
        arg8_nonconst = new GLubyte[arg8_array->Length()];
        arg8 = (const GLvoid*)arg8_nonconst;
    } else if(args[8]->IsObject()) {
        arg8 = (const GLvoid*)glbind_get_buffer_data(args[8]);
    } else if(args[8]->IsNumber()) {
        arg8 = (const GLvoid*)(args[8]->IntegerValue());
    } else {
        arg8 = NULL;
    }
    glTexSubImage2D(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
    if(arg8_nonconst) delete [] arg8_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_bindTexture) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLuint arg1;
    if(args[1]->IsNumber()) {
        arg1 = args[1]->IntegerValue();
    } else {
        arg1 = node::ObjectWrap::Unwrap<NODE_Texture>(args[1]->ToObject())->gl_handle;
    }
    glBindTexture(arg0, arg1);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_genTextures) {
    NanScope();
    // None
    GLsizei arg0;
    arg0 = args[0]->Int32Value();
    GLuint* arg1;
    v8::Handle<v8::Array> args_1_array = v8::Handle<v8::Array>::Cast(args[1]);
    arg1 = new GLuint[args_1_array->Length()];
    glGenTextures(arg0, arg1);
    for(uint32_t i = 0; i < args_1_array->Length(); i++) {
        args_1_array->Set(i, NODE_Texture::fromGLHandle(arg1[i]));
    }
    delete [] arg1;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_isTexture) {
    NanScope();
    // <gltypes.Type instance at 0x104edf710>
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Texture>(args[0]->ToObject())->gl_handle;
    }
    GLboolean result;
    result = glIsTexture(arg0);
    v8::Handle<v8::Value> result_js = NanNew<v8::Uint32>(result);
    NanReturnValue(result_js);
}
NAN_METHOD(EXPORT_blendColor) {
    NanScope();
    // None
    GLfloat arg0;
    arg0 = (float)args[0]->NumberValue();
    GLfloat arg1;
    arg1 = (float)args[1]->NumberValue();
    GLfloat arg2;
    arg2 = (float)args[2]->NumberValue();
    GLfloat arg3;
    arg3 = (float)args[3]->NumberValue();
    glBlendColor(arg0, arg1, arg2, arg3);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_blendEquation) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    glBlendEquation(arg0);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_drawRangeElements) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLuint arg1;
    arg1 = args[1]->Uint32Value();
    GLuint arg2;
    arg2 = args[2]->Uint32Value();
    GLsizei arg3;
    arg3 = args[3]->Int32Value();
    GLenum arg4;
    arg4 = args[4]->Uint32Value();
    const GLvoid *arg5;
    GLubyte* arg5_nonconst = NULL;
    if(args[5]->IsString()) {
        NanUtf8String arg5_utf8(args[5]);
        arg5 = (const GLvoid*)(*arg5_utf8);
    } else if(args[5]->IsArray()) {
        v8::Handle<v8::Array> arg5_array = v8::Handle<v8::Array>::Cast(args[5]);
        arg5_nonconst = new GLubyte[arg5_array->Length()];
        arg5 = (const GLvoid*)arg5_nonconst;
    } else if(args[5]->IsObject()) {
        arg5 = (const GLvoid*)glbind_get_buffer_data(args[5]);
    } else if(args[5]->IsNumber()) {
        arg5 = (const GLvoid*)(args[5]->IntegerValue());
    } else {
        arg5 = NULL;
    }
    glDrawRangeElements(arg0, arg1, arg2, arg3, arg4, arg5);
    if(arg5_nonconst) delete [] arg5_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_texImage3D) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLint arg2;
    arg2 = args[2]->Int32Value();
    GLsizei arg3;
    arg3 = args[3]->Int32Value();
    GLsizei arg4;
    arg4 = args[4]->Int32Value();
    GLsizei arg5;
    arg5 = args[5]->Int32Value();
    GLint arg6;
    arg6 = args[6]->Int32Value();
    GLenum arg7;
    arg7 = args[7]->Uint32Value();
    GLenum arg8;
    arg8 = args[8]->Uint32Value();
    const GLvoid *arg9;
    GLubyte* arg9_nonconst = NULL;
    if(args[9]->IsString()) {
        NanUtf8String arg9_utf8(args[9]);
        arg9 = (const GLvoid*)(*arg9_utf8);
    } else if(args[9]->IsArray()) {
        v8::Handle<v8::Array> arg9_array = v8::Handle<v8::Array>::Cast(args[9]);
        arg9_nonconst = new GLubyte[arg9_array->Length()];
        arg9 = (const GLvoid*)arg9_nonconst;
    } else if(args[9]->IsObject()) {
        arg9 = (const GLvoid*)glbind_get_buffer_data(args[9]);
    } else if(args[9]->IsNumber()) {
        arg9 = (const GLvoid*)(args[9]->IntegerValue());
    } else {
        arg9 = NULL;
    }
    glTexImage3D(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
    if(arg9_nonconst) delete [] arg9_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_texSubImage3D) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLint arg2;
    arg2 = args[2]->Int32Value();
    GLint arg3;
    arg3 = args[3]->Int32Value();
    GLint arg4;
    arg4 = args[4]->Int32Value();
    GLsizei arg5;
    arg5 = args[5]->Int32Value();
    GLsizei arg6;
    arg6 = args[6]->Int32Value();
    GLsizei arg7;
    arg7 = args[7]->Int32Value();
    GLenum arg8;
    arg8 = args[8]->Uint32Value();
    GLenum arg9;
    arg9 = args[9]->Uint32Value();
    const GLvoid *arg10;
    GLubyte* arg10_nonconst = NULL;
    if(args[10]->IsString()) {
        NanUtf8String arg10_utf8(args[10]);
        arg10 = (const GLvoid*)(*arg10_utf8);
    } else if(args[10]->IsArray()) {
        v8::Handle<v8::Array> arg10_array = v8::Handle<v8::Array>::Cast(args[10]);
        arg10_nonconst = new GLubyte[arg10_array->Length()];
        arg10 = (const GLvoid*)arg10_nonconst;
    } else if(args[10]->IsObject()) {
        arg10 = (const GLvoid*)glbind_get_buffer_data(args[10]);
    } else if(args[10]->IsNumber()) {
        arg10 = (const GLvoid*)(args[10]->IntegerValue());
    } else {
        arg10 = NULL;
    }
    glTexSubImage3D(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10);
    if(arg10_nonconst) delete [] arg10_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_copyTexSubImage3D) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLint arg2;
    arg2 = args[2]->Int32Value();
    GLint arg3;
    arg3 = args[3]->Int32Value();
    GLint arg4;
    arg4 = args[4]->Int32Value();
    GLint arg5;
    arg5 = args[5]->Int32Value();
    GLint arg6;
    arg6 = args[6]->Int32Value();
    GLsizei arg7;
    arg7 = args[7]->Int32Value();
    GLsizei arg8;
    arg8 = args[8]->Int32Value();
    glCopyTexSubImage3D(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_activeTexture) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    glActiveTexture(arg0);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_sampleCoverage) {
    NanScope();
    // None
    GLclampf arg0;
    arg0 = (float)args[0]->NumberValue();
    GLboolean arg1;
    arg1 = args[1]->Uint32Value();
    glSampleCoverage(arg0, arg1);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_compressedTexImage3D) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLenum arg2;
    arg2 = args[2]->Uint32Value();
    GLsizei arg3;
    arg3 = args[3]->Int32Value();
    GLsizei arg4;
    arg4 = args[4]->Int32Value();
    GLsizei arg5;
    arg5 = args[5]->Int32Value();
    GLint arg6;
    arg6 = args[6]->Int32Value();
    GLsizei arg7;
    arg7 = args[7]->Int32Value();
    const GLvoid *arg8;
    GLubyte* arg8_nonconst = NULL;
    if(args[8]->IsString()) {
        NanUtf8String arg8_utf8(args[8]);
        arg8 = (const GLvoid*)(*arg8_utf8);
    } else if(args[8]->IsArray()) {
        v8::Handle<v8::Array> arg8_array = v8::Handle<v8::Array>::Cast(args[8]);
        arg8_nonconst = new GLubyte[arg8_array->Length()];
        arg8 = (const GLvoid*)arg8_nonconst;
    } else if(args[8]->IsObject()) {
        arg8 = (const GLvoid*)glbind_get_buffer_data(args[8]);
    } else if(args[8]->IsNumber()) {
        arg8 = (const GLvoid*)(args[8]->IntegerValue());
    } else {
        arg8 = NULL;
    }
    glCompressedTexImage3D(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
    if(arg8_nonconst) delete [] arg8_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_compressedTexImage2D) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLenum arg2;
    arg2 = args[2]->Uint32Value();
    GLsizei arg3;
    arg3 = args[3]->Int32Value();
    GLsizei arg4;
    arg4 = args[4]->Int32Value();
    GLint arg5;
    arg5 = args[5]->Int32Value();
    GLsizei arg6;
    arg6 = args[6]->Int32Value();
    const GLvoid *arg7;
    GLubyte* arg7_nonconst = NULL;
    if(args[7]->IsString()) {
        NanUtf8String arg7_utf8(args[7]);
        arg7 = (const GLvoid*)(*arg7_utf8);
    } else if(args[7]->IsArray()) {
        v8::Handle<v8::Array> arg7_array = v8::Handle<v8::Array>::Cast(args[7]);
        arg7_nonconst = new GLubyte[arg7_array->Length()];
        arg7 = (const GLvoid*)arg7_nonconst;
    } else if(args[7]->IsObject()) {
        arg7 = (const GLvoid*)glbind_get_buffer_data(args[7]);
    } else if(args[7]->IsNumber()) {
        arg7 = (const GLvoid*)(args[7]->IntegerValue());
    } else {
        arg7 = NULL;
    }
    glCompressedTexImage2D(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
    if(arg7_nonconst) delete [] arg7_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_compressedTexImage1D) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLenum arg2;
    arg2 = args[2]->Uint32Value();
    GLsizei arg3;
    arg3 = args[3]->Int32Value();
    GLint arg4;
    arg4 = args[4]->Int32Value();
    GLsizei arg5;
    arg5 = args[5]->Int32Value();
    const GLvoid *arg6;
    GLubyte* arg6_nonconst = NULL;
    if(args[6]->IsString()) {
        NanUtf8String arg6_utf8(args[6]);
        arg6 = (const GLvoid*)(*arg6_utf8);
    } else if(args[6]->IsArray()) {
        v8::Handle<v8::Array> arg6_array = v8::Handle<v8::Array>::Cast(args[6]);
        arg6_nonconst = new GLubyte[arg6_array->Length()];
        arg6 = (const GLvoid*)arg6_nonconst;
    } else if(args[6]->IsObject()) {
        arg6 = (const GLvoid*)glbind_get_buffer_data(args[6]);
    } else if(args[6]->IsNumber()) {
        arg6 = (const GLvoid*)(args[6]->IntegerValue());
    } else {
        arg6 = NULL;
    }
    glCompressedTexImage1D(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
    if(arg6_nonconst) delete [] arg6_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_compressedTexSubImage3D) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLint arg2;
    arg2 = args[2]->Int32Value();
    GLint arg3;
    arg3 = args[3]->Int32Value();
    GLint arg4;
    arg4 = args[4]->Int32Value();
    GLsizei arg5;
    arg5 = args[5]->Int32Value();
    GLsizei arg6;
    arg6 = args[6]->Int32Value();
    GLsizei arg7;
    arg7 = args[7]->Int32Value();
    GLenum arg8;
    arg8 = args[8]->Uint32Value();
    GLsizei arg9;
    arg9 = args[9]->Int32Value();
    const GLvoid *arg10;
    GLubyte* arg10_nonconst = NULL;
    if(args[10]->IsString()) {
        NanUtf8String arg10_utf8(args[10]);
        arg10 = (const GLvoid*)(*arg10_utf8);
    } else if(args[10]->IsArray()) {
        v8::Handle<v8::Array> arg10_array = v8::Handle<v8::Array>::Cast(args[10]);
        arg10_nonconst = new GLubyte[arg10_array->Length()];
        arg10 = (const GLvoid*)arg10_nonconst;
    } else if(args[10]->IsObject()) {
        arg10 = (const GLvoid*)glbind_get_buffer_data(args[10]);
    } else if(args[10]->IsNumber()) {
        arg10 = (const GLvoid*)(args[10]->IntegerValue());
    } else {
        arg10 = NULL;
    }
    glCompressedTexSubImage3D(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10);
    if(arg10_nonconst) delete [] arg10_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_compressedTexSubImage2D) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLint arg2;
    arg2 = args[2]->Int32Value();
    GLint arg3;
    arg3 = args[3]->Int32Value();
    GLsizei arg4;
    arg4 = args[4]->Int32Value();
    GLsizei arg5;
    arg5 = args[5]->Int32Value();
    GLenum arg6;
    arg6 = args[6]->Uint32Value();
    GLsizei arg7;
    arg7 = args[7]->Int32Value();
    const GLvoid *arg8;
    GLubyte* arg8_nonconst = NULL;
    if(args[8]->IsString()) {
        NanUtf8String arg8_utf8(args[8]);
        arg8 = (const GLvoid*)(*arg8_utf8);
    } else if(args[8]->IsArray()) {
        v8::Handle<v8::Array> arg8_array = v8::Handle<v8::Array>::Cast(args[8]);
        arg8_nonconst = new GLubyte[arg8_array->Length()];
        arg8 = (const GLvoid*)arg8_nonconst;
    } else if(args[8]->IsObject()) {
        arg8 = (const GLvoid*)glbind_get_buffer_data(args[8]);
    } else if(args[8]->IsNumber()) {
        arg8 = (const GLvoid*)(args[8]->IntegerValue());
    } else {
        arg8 = NULL;
    }
    glCompressedTexSubImage2D(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
    if(arg8_nonconst) delete [] arg8_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_compressedTexSubImage1D) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLint arg2;
    arg2 = args[2]->Int32Value();
    GLsizei arg3;
    arg3 = args[3]->Int32Value();
    GLenum arg4;
    arg4 = args[4]->Uint32Value();
    GLsizei arg5;
    arg5 = args[5]->Int32Value();
    const GLvoid *arg6;
    GLubyte* arg6_nonconst = NULL;
    if(args[6]->IsString()) {
        NanUtf8String arg6_utf8(args[6]);
        arg6 = (const GLvoid*)(*arg6_utf8);
    } else if(args[6]->IsArray()) {
        v8::Handle<v8::Array> arg6_array = v8::Handle<v8::Array>::Cast(args[6]);
        arg6_nonconst = new GLubyte[arg6_array->Length()];
        arg6 = (const GLvoid*)arg6_nonconst;
    } else if(args[6]->IsObject()) {
        arg6 = (const GLvoid*)glbind_get_buffer_data(args[6]);
    } else if(args[6]->IsNumber()) {
        arg6 = (const GLvoid*)(args[6]->IntegerValue());
    } else {
        arg6 = NULL;
    }
    glCompressedTexSubImage1D(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
    if(arg6_nonconst) delete [] arg6_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getCompressedTexImage) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLvoid *arg2;
    GLubyte* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (GLvoid*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLubyte[arg2_array->Length()];
        arg2 = (GLvoid*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (GLvoid*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLvoid*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetCompressedTexImage(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_blendFuncSeparate) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLenum arg2;
    arg2 = args[2]->Uint32Value();
    GLenum arg3;
    arg3 = args[3]->Uint32Value();
    glBlendFuncSeparate(arg0, arg1, arg2, arg3);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_multiDrawArrays) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    const GLint *arg1;
    GLint* arg1_nonconst = NULL;
    if(args[1]->IsString()) {
        NanUtf8String arg1_utf8(args[1]);
        arg1 = (const GLint*)(*arg1_utf8);
    } else if(args[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(args[1]);
        arg1_nonconst = new GLint[arg1_array->Length()];
        arg1 = (const GLint*)arg1_nonconst;
    } else if(args[1]->IsObject()) {
        arg1 = (const GLint*)glbind_get_buffer_data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLint*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    const GLsizei *arg2;
    GLsizei* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (const GLsizei*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLsizei[arg2_array->Length()];
        arg2 = (const GLsizei*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (const GLsizei*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLsizei*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    GLsizei arg3;
    arg3 = args[3]->Int32Value();
    glMultiDrawArrays(arg0, arg1, arg2, arg3);
    if(arg1_nonconst) delete [] arg1_nonconst;
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_pointParameterf) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLfloat arg1;
    arg1 = (float)args[1]->NumberValue();
    glPointParameterf(arg0, arg1);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_pointParameterfv) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    const GLfloat *arg1;
    GLfloat* arg1_nonconst = NULL;
    if(args[1]->IsString()) {
        NanUtf8String arg1_utf8(args[1]);
        arg1 = (const GLfloat*)(*arg1_utf8);
    } else if(args[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(args[1]);
        arg1_nonconst = new GLfloat[arg1_array->Length()];
        arg1 = (const GLfloat*)arg1_nonconst;
    } else if(args[1]->IsObject()) {
        arg1 = (const GLfloat*)glbind_get_buffer_data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLfloat*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glPointParameterfv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_pointParameteri) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLint arg1;
    arg1 = args[1]->Int32Value();
    glPointParameteri(arg0, arg1);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_pointParameteriv) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    const GLint *arg1;
    GLint* arg1_nonconst = NULL;
    if(args[1]->IsString()) {
        NanUtf8String arg1_utf8(args[1]);
        arg1 = (const GLint*)(*arg1_utf8);
    } else if(args[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(args[1]);
        arg1_nonconst = new GLint[arg1_array->Length()];
        arg1 = (const GLint*)arg1_nonconst;
    } else if(args[1]->IsObject()) {
        arg1 = (const GLint*)glbind_get_buffer_data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLint*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glPointParameteriv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_genQueries) {
    NanScope();
    // None
    GLsizei arg0;
    arg0 = args[0]->Int32Value();
    GLuint* arg1;
    v8::Handle<v8::Array> args_1_array = v8::Handle<v8::Array>::Cast(args[1]);
    arg1 = new GLuint[args_1_array->Length()];
    glGenQueries(arg0, arg1);
    for(uint32_t i = 0; i < args_1_array->Length(); i++) {
        args_1_array->Set(i, NODE_Query::fromGLHandle(arg1[i]));
    }
    delete [] arg1;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_isQuery) {
    NanScope();
    // <gltypes.Type instance at 0x104edf710>
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Query>(args[0]->ToObject())->gl_handle;
    }
    GLboolean result;
    result = glIsQuery(arg0);
    v8::Handle<v8::Value> result_js = NanNew<v8::Uint32>(result);
    NanReturnValue(result_js);
}
NAN_METHOD(EXPORT_beginQuery) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLuint arg1;
    if(args[1]->IsNumber()) {
        arg1 = args[1]->IntegerValue();
    } else {
        arg1 = node::ObjectWrap::Unwrap<NODE_Query>(args[1]->ToObject())->gl_handle;
    }
    glBeginQuery(arg0, arg1);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_endQuery) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    glEndQuery(arg0);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getQueryiv) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLint *arg2;
    GLint* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (GLint*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLint[arg2_array->Length()];
        arg2 = (GLint*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (GLint*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetQueryiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getQueryObjectiv) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Query>(args[0]->ToObject())->gl_handle;
    }
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLint *arg2;
    GLint* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (GLint*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLint[arg2_array->Length()];
        arg2 = (GLint*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (GLint*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetQueryObjectiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getQueryObjectuiv) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Query>(args[0]->ToObject())->gl_handle;
    }
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLuint *arg2;
    GLuint* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (GLuint*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLuint[arg2_array->Length()];
        arg2 = (GLuint*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (GLuint*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLuint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetQueryObjectuiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_bindBuffer) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLuint arg1;
    if(args[1]->IsNumber()) {
        arg1 = args[1]->IntegerValue();
    } else {
        arg1 = node::ObjectWrap::Unwrap<NODE_Buffer>(args[1]->ToObject())->gl_handle;
    }
    glBindBuffer(arg0, arg1);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_genBuffers) {
    NanScope();
    // None
    GLsizei arg0;
    arg0 = args[0]->Int32Value();
    GLuint* arg1;
    v8::Handle<v8::Array> args_1_array = v8::Handle<v8::Array>::Cast(args[1]);
    arg1 = new GLuint[args_1_array->Length()];
    glGenBuffers(arg0, arg1);
    for(uint32_t i = 0; i < args_1_array->Length(); i++) {
        args_1_array->Set(i, NODE_Buffer::fromGLHandle(arg1[i]));
    }
    delete [] arg1;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_isBuffer) {
    NanScope();
    // <gltypes.Type instance at 0x104edf710>
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Buffer>(args[0]->ToObject())->gl_handle;
    }
    GLboolean result;
    result = glIsBuffer(arg0);
    v8::Handle<v8::Value> result_js = NanNew<v8::Uint32>(result);
    NanReturnValue(result_js);
}
NAN_METHOD(EXPORT_bufferData) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLsizeiptr arg1;
    arg1 = (uint64_t)args[1]->IntegerValue();
    const GLvoid *arg2;
    GLubyte* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (const GLvoid*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLubyte[arg2_array->Length()];
        arg2 = (const GLvoid*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (const GLvoid*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLvoid*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    GLenum arg3;
    arg3 = args[3]->Uint32Value();
    glBufferData(arg0, arg1, arg2, arg3);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_bufferSubData) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLintptr arg1;
    arg1 = args[1]->IntegerValue();
    GLsizeiptr arg2;
    arg2 = (uint64_t)args[2]->IntegerValue();
    const GLvoid *arg3;
    GLubyte* arg3_nonconst = NULL;
    if(args[3]->IsString()) {
        NanUtf8String arg3_utf8(args[3]);
        arg3 = (const GLvoid*)(*arg3_utf8);
    } else if(args[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(args[3]);
        arg3_nonconst = new GLubyte[arg3_array->Length()];
        arg3 = (const GLvoid*)arg3_nonconst;
    } else if(args[3]->IsObject()) {
        arg3 = (const GLvoid*)glbind_get_buffer_data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLvoid*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glBufferSubData(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getBufferSubData) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLintptr arg1;
    arg1 = args[1]->IntegerValue();
    GLsizeiptr arg2;
    arg2 = (uint64_t)args[2]->IntegerValue();
    GLvoid *arg3;
    GLubyte* arg3_nonconst = NULL;
    if(args[3]->IsString()) {
        NanUtf8String arg3_utf8(args[3]);
        arg3 = (GLvoid*)(*arg3_utf8);
    } else if(args[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(args[3]);
        arg3_nonconst = new GLubyte[arg3_array->Length()];
        arg3 = (GLvoid*)arg3_nonconst;
    } else if(args[3]->IsObject()) {
        arg3 = (GLvoid*)glbind_get_buffer_data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (GLvoid*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glGetBufferSubData(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_mapBuffer) {
    NanScope();
    // <gltypes.PointerType instance at 0x10535a710>
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLvoid *result;
    result = glMapBuffer(arg0, arg1);
    v8::Handle<v8::Value> result_js = NanNewBufferHandle((char*)result, 0, do_nothing_release_callback, NULL);
    NanReturnValue(result_js);
}
NAN_METHOD(EXPORT_unmapBuffer) {
    NanScope();
    // <gltypes.Type instance at 0x104edf710>
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLboolean result;
    result = glUnmapBuffer(arg0);
    v8::Handle<v8::Value> result_js = NanNew<v8::Uint32>(result);
    NanReturnValue(result_js);
}
NAN_METHOD(EXPORT_getBufferParameteriv) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLint *arg2;
    GLint* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (GLint*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLint[arg2_array->Length()];
        arg2 = (GLint*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (GLint*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetBufferParameteriv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_blendEquationSeparate) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    glBlendEquationSeparate(arg0, arg1);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_drawBuffers) {
    NanScope();
    // None
    GLsizei arg0;
    arg0 = args[0]->Int32Value();
    const GLenum *arg1;
    GLenum* arg1_nonconst = NULL;
    if(args[1]->IsString()) {
        NanUtf8String arg1_utf8(args[1]);
        arg1 = (const GLenum*)(*arg1_utf8);
    } else if(args[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(args[1]);
        arg1_nonconst = new GLenum[arg1_array->Length()];
        arg1 = (const GLenum*)arg1_nonconst;
    } else if(args[1]->IsObject()) {
        arg1 = (const GLenum*)glbind_get_buffer_data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLenum*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glDrawBuffers(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_stencilOpSeparate) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLenum arg2;
    arg2 = args[2]->Uint32Value();
    GLenum arg3;
    arg3 = args[3]->Uint32Value();
    glStencilOpSeparate(arg0, arg1, arg2, arg3);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_stencilFuncSeparate) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLint arg2;
    arg2 = args[2]->Int32Value();
    GLuint arg3;
    arg3 = args[3]->Uint32Value();
    glStencilFuncSeparate(arg0, arg1, arg2, arg3);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_stencilMaskSeparate) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLuint arg1;
    arg1 = args[1]->Uint32Value();
    glStencilMaskSeparate(arg0, arg1);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_attachShader) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLuint arg1;
    if(args[1]->IsNumber()) {
        arg1 = args[1]->IntegerValue();
    } else {
        arg1 = node::ObjectWrap::Unwrap<NODE_Shader>(args[1]->ToObject())->gl_handle;
    }
    glAttachShader(arg0, arg1);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_bindAttribLocation) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLuint arg1;
    arg1 = args[1]->Uint32Value();
    const GLchar* arg2;
    NanUtf8String arg2_utf8(args[2]);
    arg2 = (GLchar*)*arg2_utf8;
    glBindAttribLocation(arg0, arg1, arg2);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_compileShader) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Shader>(args[0]->ToObject())->gl_handle;
    }
    glCompileShader(arg0);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_createProgram) {
    NanScope();
    // <gltypes.ClassType instance at 0x104edfdd0>
    GLuint result;
    result = glCreateProgram();
    v8::Handle<v8::Value> result_js = NODE_Program::fromGLHandle(result);
    NanReturnValue(result_js);
}
NAN_METHOD(EXPORT_createShader) {
    NanScope();
    // <gltypes.ClassType instance at 0x104edfe18>
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLuint result;
    result = glCreateShader(arg0);
    v8::Handle<v8::Value> result_js = NODE_Shader::fromGLHandle(result);
    NanReturnValue(result_js);
}
NAN_METHOD(EXPORT_detachShader) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLuint arg1;
    if(args[1]->IsNumber()) {
        arg1 = args[1]->IntegerValue();
    } else {
        arg1 = node::ObjectWrap::Unwrap<NODE_Shader>(args[1]->ToObject())->gl_handle;
    }
    glDetachShader(arg0, arg1);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_disableVertexAttribArray) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    glDisableVertexAttribArray(arg0);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_enableVertexAttribArray) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    glEnableVertexAttribArray(arg0);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getActiveAttrib) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLuint arg1;
    arg1 = args[1]->Uint32Value();
    GLsizei arg2;
    arg2 = args[2]->Int32Value();
    GLsizei *arg3;
    GLsizei* arg3_nonconst = NULL;
    if(args[3]->IsString()) {
        NanUtf8String arg3_utf8(args[3]);
        arg3 = (GLsizei*)(*arg3_utf8);
    } else if(args[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(args[3]);
        arg3_nonconst = new GLsizei[arg3_array->Length()];
        arg3 = (GLsizei*)arg3_nonconst;
    } else if(args[3]->IsObject()) {
        arg3 = (GLsizei*)glbind_get_buffer_data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (GLsizei*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    GLint *arg4;
    GLint* arg4_nonconst = NULL;
    if(args[4]->IsString()) {
        NanUtf8String arg4_utf8(args[4]);
        arg4 = (GLint*)(*arg4_utf8);
    } else if(args[4]->IsArray()) {
        v8::Handle<v8::Array> arg4_array = v8::Handle<v8::Array>::Cast(args[4]);
        arg4_nonconst = new GLint[arg4_array->Length()];
        arg4 = (GLint*)arg4_nonconst;
    } else if(args[4]->IsObject()) {
        arg4 = (GLint*)glbind_get_buffer_data(args[4]);
    } else if(args[4]->IsNumber()) {
        arg4 = (GLint*)(args[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    GLenum *arg5;
    GLenum* arg5_nonconst = NULL;
    if(args[5]->IsString()) {
        NanUtf8String arg5_utf8(args[5]);
        arg5 = (GLenum*)(*arg5_utf8);
    } else if(args[5]->IsArray()) {
        v8::Handle<v8::Array> arg5_array = v8::Handle<v8::Array>::Cast(args[5]);
        arg5_nonconst = new GLenum[arg5_array->Length()];
        arg5 = (GLenum*)arg5_nonconst;
    } else if(args[5]->IsObject()) {
        arg5 = (GLenum*)glbind_get_buffer_data(args[5]);
    } else if(args[5]->IsNumber()) {
        arg5 = (GLenum*)(args[5]->IntegerValue());
    } else {
        arg5 = NULL;
    }
    GLchar *arg6;
    GLchar* arg6_nonconst = NULL;
    if(args[6]->IsString()) {
        NanUtf8String arg6_utf8(args[6]);
        arg6 = (GLchar*)(*arg6_utf8);
    } else if(args[6]->IsArray()) {
        v8::Handle<v8::Array> arg6_array = v8::Handle<v8::Array>::Cast(args[6]);
        arg6_nonconst = new GLchar[arg6_array->Length()];
        arg6 = (GLchar*)arg6_nonconst;
    } else if(args[6]->IsObject()) {
        arg6 = (GLchar*)glbind_get_buffer_data(args[6]);
    } else if(args[6]->IsNumber()) {
        arg6 = (GLchar*)(args[6]->IntegerValue());
    } else {
        arg6 = NULL;
    }
    glGetActiveAttrib(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
    if(arg3_nonconst) delete [] arg3_nonconst;
    if(arg4_nonconst) delete [] arg4_nonconst;
    if(arg5_nonconst) delete [] arg5_nonconst;
    if(arg6_nonconst) delete [] arg6_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getActiveUniform) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLuint arg1;
    arg1 = args[1]->Uint32Value();
    GLsizei arg2;
    arg2 = args[2]->Int32Value();
    GLsizei *arg3;
    GLsizei* arg3_nonconst = NULL;
    if(args[3]->IsString()) {
        NanUtf8String arg3_utf8(args[3]);
        arg3 = (GLsizei*)(*arg3_utf8);
    } else if(args[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(args[3]);
        arg3_nonconst = new GLsizei[arg3_array->Length()];
        arg3 = (GLsizei*)arg3_nonconst;
    } else if(args[3]->IsObject()) {
        arg3 = (GLsizei*)glbind_get_buffer_data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (GLsizei*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    GLint *arg4;
    GLint* arg4_nonconst = NULL;
    if(args[4]->IsString()) {
        NanUtf8String arg4_utf8(args[4]);
        arg4 = (GLint*)(*arg4_utf8);
    } else if(args[4]->IsArray()) {
        v8::Handle<v8::Array> arg4_array = v8::Handle<v8::Array>::Cast(args[4]);
        arg4_nonconst = new GLint[arg4_array->Length()];
        arg4 = (GLint*)arg4_nonconst;
    } else if(args[4]->IsObject()) {
        arg4 = (GLint*)glbind_get_buffer_data(args[4]);
    } else if(args[4]->IsNumber()) {
        arg4 = (GLint*)(args[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    GLenum *arg5;
    GLenum* arg5_nonconst = NULL;
    if(args[5]->IsString()) {
        NanUtf8String arg5_utf8(args[5]);
        arg5 = (GLenum*)(*arg5_utf8);
    } else if(args[5]->IsArray()) {
        v8::Handle<v8::Array> arg5_array = v8::Handle<v8::Array>::Cast(args[5]);
        arg5_nonconst = new GLenum[arg5_array->Length()];
        arg5 = (GLenum*)arg5_nonconst;
    } else if(args[5]->IsObject()) {
        arg5 = (GLenum*)glbind_get_buffer_data(args[5]);
    } else if(args[5]->IsNumber()) {
        arg5 = (GLenum*)(args[5]->IntegerValue());
    } else {
        arg5 = NULL;
    }
    GLchar *arg6;
    GLchar* arg6_nonconst = NULL;
    if(args[6]->IsString()) {
        NanUtf8String arg6_utf8(args[6]);
        arg6 = (GLchar*)(*arg6_utf8);
    } else if(args[6]->IsArray()) {
        v8::Handle<v8::Array> arg6_array = v8::Handle<v8::Array>::Cast(args[6]);
        arg6_nonconst = new GLchar[arg6_array->Length()];
        arg6 = (GLchar*)arg6_nonconst;
    } else if(args[6]->IsObject()) {
        arg6 = (GLchar*)glbind_get_buffer_data(args[6]);
    } else if(args[6]->IsNumber()) {
        arg6 = (GLchar*)(args[6]->IntegerValue());
    } else {
        arg6 = NULL;
    }
    glGetActiveUniform(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
    if(arg3_nonconst) delete [] arg3_nonconst;
    if(arg4_nonconst) delete [] arg4_nonconst;
    if(arg5_nonconst) delete [] arg5_nonconst;
    if(arg6_nonconst) delete [] arg6_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getAttachedShaders) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLsizei arg1;
    arg1 = args[1]->Int32Value();
    GLsizei *arg2;
    GLsizei* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (GLsizei*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLsizei[arg2_array->Length()];
        arg2 = (GLsizei*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (GLsizei*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLsizei*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    GLuint* arg3;
    v8::Handle<v8::Array> args_3_array = v8::Handle<v8::Array>::Cast(args[3]);
    arg3 = new GLuint[args_3_array->Length()];
    glGetAttachedShaders(arg0, arg1, arg2, arg3);
    if(arg2_nonconst) delete [] arg2_nonconst;
    for(uint32_t i = 0; i < args_3_array->Length(); i++) {
        args_3_array->Set(i, NODE_Shader::fromGLHandle(arg3[i]));
    }
    delete [] arg3;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getAttribLocation) {
    NanScope();
    // <gltypes.Type instance at 0x104edf878>
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    const GLchar* arg1;
    NanUtf8String arg1_utf8(args[1]);
    arg1 = (GLchar*)*arg1_utf8;
    GLint result;
    result = glGetAttribLocation(arg0, arg1);
    v8::Handle<v8::Value> result_js = NanNew<v8::Int32>(result);
    NanReturnValue(result_js);
}
NAN_METHOD(EXPORT_getProgramiv) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLint *arg2;
    GLint* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (GLint*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLint[arg2_array->Length()];
        arg2 = (GLint*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (GLint*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetProgramiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getProgramInfoLog) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLsizei arg1;
    arg1 = args[1]->Int32Value();
    GLsizei *arg2;
    GLsizei* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (GLsizei*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLsizei[arg2_array->Length()];
        arg2 = (GLsizei*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (GLsizei*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLsizei*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    GLchar *arg3;
    GLchar* arg3_nonconst = NULL;
    if(args[3]->IsString()) {
        NanUtf8String arg3_utf8(args[3]);
        arg3 = (GLchar*)(*arg3_utf8);
    } else if(args[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(args[3]);
        arg3_nonconst = new GLchar[arg3_array->Length()];
        arg3 = (GLchar*)arg3_nonconst;
    } else if(args[3]->IsObject()) {
        arg3 = (GLchar*)glbind_get_buffer_data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (GLchar*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glGetProgramInfoLog(arg0, arg1, arg2, arg3);
    if(arg2_nonconst) delete [] arg2_nonconst;
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getShaderiv) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Shader>(args[0]->ToObject())->gl_handle;
    }
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLint *arg2;
    GLint* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (GLint*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLint[arg2_array->Length()];
        arg2 = (GLint*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (GLint*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetShaderiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getShaderInfoLog) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Shader>(args[0]->ToObject())->gl_handle;
    }
    GLsizei arg1;
    arg1 = args[1]->Int32Value();
    GLsizei *arg2;
    GLsizei* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (GLsizei*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLsizei[arg2_array->Length()];
        arg2 = (GLsizei*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (GLsizei*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLsizei*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    GLchar *arg3;
    GLchar* arg3_nonconst = NULL;
    if(args[3]->IsString()) {
        NanUtf8String arg3_utf8(args[3]);
        arg3 = (GLchar*)(*arg3_utf8);
    } else if(args[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(args[3]);
        arg3_nonconst = new GLchar[arg3_array->Length()];
        arg3 = (GLchar*)arg3_nonconst;
    } else if(args[3]->IsObject()) {
        arg3 = (GLchar*)glbind_get_buffer_data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (GLchar*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glGetShaderInfoLog(arg0, arg1, arg2, arg3);
    if(arg2_nonconst) delete [] arg2_nonconst;
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getShaderSource) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Shader>(args[0]->ToObject())->gl_handle;
    }
    GLsizei arg1;
    arg1 = args[1]->Int32Value();
    GLsizei *arg2;
    GLsizei* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (GLsizei*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLsizei[arg2_array->Length()];
        arg2 = (GLsizei*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (GLsizei*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLsizei*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    GLchar *arg3;
    GLchar* arg3_nonconst = NULL;
    if(args[3]->IsString()) {
        NanUtf8String arg3_utf8(args[3]);
        arg3 = (GLchar*)(*arg3_utf8);
    } else if(args[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(args[3]);
        arg3_nonconst = new GLchar[arg3_array->Length()];
        arg3 = (GLchar*)arg3_nonconst;
    } else if(args[3]->IsObject()) {
        arg3 = (GLchar*)glbind_get_buffer_data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (GLchar*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glGetShaderSource(arg0, arg1, arg2, arg3);
    if(arg2_nonconst) delete [] arg2_nonconst;
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getUniformLocation) {
    NanScope();
    // <gltypes.Type instance at 0x104edf878>
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    const GLchar* arg1;
    NanUtf8String arg1_utf8(args[1]);
    arg1 = (GLchar*)*arg1_utf8;
    GLint result;
    result = glGetUniformLocation(arg0, arg1);
    v8::Handle<v8::Value> result_js = NanNew<v8::Int32>(result);
    NanReturnValue(result_js);
}
NAN_METHOD(EXPORT_getUniformfv) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLfloat *arg2;
    GLfloat* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (GLfloat*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLfloat[arg2_array->Length()];
        arg2 = (GLfloat*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (GLfloat*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLfloat*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetUniformfv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getUniformiv) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLint *arg2;
    GLint* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (GLint*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLint[arg2_array->Length()];
        arg2 = (GLint*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (GLint*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetUniformiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getVertexAttribdv) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLdouble *arg2;
    GLdouble* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (GLdouble*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLdouble[arg2_array->Length()];
        arg2 = (GLdouble*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (GLdouble*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLdouble*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetVertexAttribdv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getVertexAttribfv) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLfloat *arg2;
    GLfloat* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (GLfloat*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLfloat[arg2_array->Length()];
        arg2 = (GLfloat*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (GLfloat*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLfloat*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetVertexAttribfv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getVertexAttribiv) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLint *arg2;
    GLint* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (GLint*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLint[arg2_array->Length()];
        arg2 = (GLint*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (GLint*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetVertexAttribiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_isProgram) {
    NanScope();
    // <gltypes.Type instance at 0x104edf710>
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLboolean result;
    result = glIsProgram(arg0);
    v8::Handle<v8::Value> result_js = NanNew<v8::Uint32>(result);
    NanReturnValue(result_js);
}
NAN_METHOD(EXPORT_isShader) {
    NanScope();
    // <gltypes.Type instance at 0x104edf710>
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Shader>(args[0]->ToObject())->gl_handle;
    }
    GLboolean result;
    result = glIsShader(arg0);
    v8::Handle<v8::Value> result_js = NanNew<v8::Uint32>(result);
    NanReturnValue(result_js);
}
NAN_METHOD(EXPORT_linkProgram) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    glLinkProgram(arg0);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_useProgram) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    glUseProgram(arg0);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_uniform1f) {
    NanScope();
    // None
    GLint arg0;
    arg0 = args[0]->Int32Value();
    GLfloat arg1;
    arg1 = (float)args[1]->NumberValue();
    glUniform1f(arg0, arg1);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_uniform2f) {
    NanScope();
    // None
    GLint arg0;
    arg0 = args[0]->Int32Value();
    GLfloat arg1;
    arg1 = (float)args[1]->NumberValue();
    GLfloat arg2;
    arg2 = (float)args[2]->NumberValue();
    glUniform2f(arg0, arg1, arg2);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_uniform3f) {
    NanScope();
    // None
    GLint arg0;
    arg0 = args[0]->Int32Value();
    GLfloat arg1;
    arg1 = (float)args[1]->NumberValue();
    GLfloat arg2;
    arg2 = (float)args[2]->NumberValue();
    GLfloat arg3;
    arg3 = (float)args[3]->NumberValue();
    glUniform3f(arg0, arg1, arg2, arg3);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_uniform4f) {
    NanScope();
    // None
    GLint arg0;
    arg0 = args[0]->Int32Value();
    GLfloat arg1;
    arg1 = (float)args[1]->NumberValue();
    GLfloat arg2;
    arg2 = (float)args[2]->NumberValue();
    GLfloat arg3;
    arg3 = (float)args[3]->NumberValue();
    GLfloat arg4;
    arg4 = (float)args[4]->NumberValue();
    glUniform4f(arg0, arg1, arg2, arg3, arg4);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_uniform1i) {
    NanScope();
    // None
    GLint arg0;
    arg0 = args[0]->Int32Value();
    GLint arg1;
    arg1 = args[1]->Int32Value();
    glUniform1i(arg0, arg1);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_uniform2i) {
    NanScope();
    // None
    GLint arg0;
    arg0 = args[0]->Int32Value();
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLint arg2;
    arg2 = args[2]->Int32Value();
    glUniform2i(arg0, arg1, arg2);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_uniform3i) {
    NanScope();
    // None
    GLint arg0;
    arg0 = args[0]->Int32Value();
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLint arg2;
    arg2 = args[2]->Int32Value();
    GLint arg3;
    arg3 = args[3]->Int32Value();
    glUniform3i(arg0, arg1, arg2, arg3);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_uniform4i) {
    NanScope();
    // None
    GLint arg0;
    arg0 = args[0]->Int32Value();
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLint arg2;
    arg2 = args[2]->Int32Value();
    GLint arg3;
    arg3 = args[3]->Int32Value();
    GLint arg4;
    arg4 = args[4]->Int32Value();
    glUniform4i(arg0, arg1, arg2, arg3, arg4);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_uniform1fv) {
    NanScope();
    // None
    GLint arg0;
    arg0 = args[0]->Int32Value();
    GLsizei arg1;
    arg1 = args[1]->Int32Value();
    const GLfloat *arg2;
    GLfloat* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (const GLfloat*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLfloat[arg2_array->Length()];
        arg2 = (const GLfloat*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (const GLfloat*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLfloat*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glUniform1fv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_uniform2fv) {
    NanScope();
    // None
    GLint arg0;
    arg0 = args[0]->Int32Value();
    GLsizei arg1;
    arg1 = args[1]->Int32Value();
    const GLfloat *arg2;
    GLfloat* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (const GLfloat*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLfloat[arg2_array->Length()];
        arg2 = (const GLfloat*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (const GLfloat*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLfloat*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glUniform2fv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_uniform3fv) {
    NanScope();
    // None
    GLint arg0;
    arg0 = args[0]->Int32Value();
    GLsizei arg1;
    arg1 = args[1]->Int32Value();
    const GLfloat *arg2;
    GLfloat* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (const GLfloat*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLfloat[arg2_array->Length()];
        arg2 = (const GLfloat*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (const GLfloat*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLfloat*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glUniform3fv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_uniform4fv) {
    NanScope();
    // None
    GLint arg0;
    arg0 = args[0]->Int32Value();
    GLsizei arg1;
    arg1 = args[1]->Int32Value();
    const GLfloat *arg2;
    GLfloat* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (const GLfloat*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLfloat[arg2_array->Length()];
        arg2 = (const GLfloat*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (const GLfloat*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLfloat*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glUniform4fv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_uniform1iv) {
    NanScope();
    // None
    GLint arg0;
    arg0 = args[0]->Int32Value();
    GLsizei arg1;
    arg1 = args[1]->Int32Value();
    const GLint *arg2;
    GLint* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (const GLint*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLint[arg2_array->Length()];
        arg2 = (const GLint*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (const GLint*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glUniform1iv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_uniform2iv) {
    NanScope();
    // None
    GLint arg0;
    arg0 = args[0]->Int32Value();
    GLsizei arg1;
    arg1 = args[1]->Int32Value();
    const GLint *arg2;
    GLint* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (const GLint*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLint[arg2_array->Length()];
        arg2 = (const GLint*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (const GLint*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glUniform2iv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_uniform3iv) {
    NanScope();
    // None
    GLint arg0;
    arg0 = args[0]->Int32Value();
    GLsizei arg1;
    arg1 = args[1]->Int32Value();
    const GLint *arg2;
    GLint* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (const GLint*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLint[arg2_array->Length()];
        arg2 = (const GLint*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (const GLint*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glUniform3iv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_uniform4iv) {
    NanScope();
    // None
    GLint arg0;
    arg0 = args[0]->Int32Value();
    GLsizei arg1;
    arg1 = args[1]->Int32Value();
    const GLint *arg2;
    GLint* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (const GLint*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLint[arg2_array->Length()];
        arg2 = (const GLint*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (const GLint*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glUniform4iv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_uniformMatrix2fv) {
    NanScope();
    // None
    GLint arg0;
    arg0 = args[0]->Int32Value();
    GLsizei arg1;
    arg1 = args[1]->Int32Value();
    GLboolean arg2;
    arg2 = args[2]->Uint32Value();
    const GLfloat *arg3;
    GLfloat* arg3_nonconst = NULL;
    if(args[3]->IsString()) {
        NanUtf8String arg3_utf8(args[3]);
        arg3 = (const GLfloat*)(*arg3_utf8);
    } else if(args[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(args[3]);
        arg3_nonconst = new GLfloat[arg3_array->Length()];
        arg3 = (const GLfloat*)arg3_nonconst;
    } else if(args[3]->IsObject()) {
        arg3 = (const GLfloat*)glbind_get_buffer_data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLfloat*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glUniformMatrix2fv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_uniformMatrix3fv) {
    NanScope();
    // None
    GLint arg0;
    arg0 = args[0]->Int32Value();
    GLsizei arg1;
    arg1 = args[1]->Int32Value();
    GLboolean arg2;
    arg2 = args[2]->Uint32Value();
    const GLfloat *arg3;
    GLfloat* arg3_nonconst = NULL;
    if(args[3]->IsString()) {
        NanUtf8String arg3_utf8(args[3]);
        arg3 = (const GLfloat*)(*arg3_utf8);
    } else if(args[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(args[3]);
        arg3_nonconst = new GLfloat[arg3_array->Length()];
        arg3 = (const GLfloat*)arg3_nonconst;
    } else if(args[3]->IsObject()) {
        arg3 = (const GLfloat*)glbind_get_buffer_data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLfloat*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glUniformMatrix3fv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_uniformMatrix4fv) {
    NanScope();
    // None
    GLint arg0;
    arg0 = args[0]->Int32Value();
    GLsizei arg1;
    arg1 = args[1]->Int32Value();
    GLboolean arg2;
    arg2 = args[2]->Uint32Value();
    const GLfloat *arg3;
    GLfloat* arg3_nonconst = NULL;
    if(args[3]->IsString()) {
        NanUtf8String arg3_utf8(args[3]);
        arg3 = (const GLfloat*)(*arg3_utf8);
    } else if(args[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(args[3]);
        arg3_nonconst = new GLfloat[arg3_array->Length()];
        arg3 = (const GLfloat*)arg3_nonconst;
    } else if(args[3]->IsObject()) {
        arg3 = (const GLfloat*)glbind_get_buffer_data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLfloat*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glUniformMatrix4fv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_validateProgram) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    glValidateProgram(arg0);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttrib1d) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLdouble arg1;
    arg1 = args[1]->NumberValue();
    glVertexAttrib1d(arg0, arg1);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttrib1dv) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    const GLdouble *arg1;
    GLdouble* arg1_nonconst = NULL;
    if(args[1]->IsString()) {
        NanUtf8String arg1_utf8(args[1]);
        arg1 = (const GLdouble*)(*arg1_utf8);
    } else if(args[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(args[1]);
        arg1_nonconst = new GLdouble[arg1_array->Length()];
        arg1 = (const GLdouble*)arg1_nonconst;
    } else if(args[1]->IsObject()) {
        arg1 = (const GLdouble*)glbind_get_buffer_data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLdouble*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib1dv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttrib1f) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLfloat arg1;
    arg1 = (float)args[1]->NumberValue();
    glVertexAttrib1f(arg0, arg1);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttrib1fv) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    const GLfloat *arg1;
    GLfloat* arg1_nonconst = NULL;
    if(args[1]->IsString()) {
        NanUtf8String arg1_utf8(args[1]);
        arg1 = (const GLfloat*)(*arg1_utf8);
    } else if(args[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(args[1]);
        arg1_nonconst = new GLfloat[arg1_array->Length()];
        arg1 = (const GLfloat*)arg1_nonconst;
    } else if(args[1]->IsObject()) {
        arg1 = (const GLfloat*)glbind_get_buffer_data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLfloat*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib1fv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttrib1s) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLshort arg1;
    arg1 = args[1]->Int32Value();
    glVertexAttrib1s(arg0, arg1);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttrib1sv) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    const GLshort *arg1;
    GLshort* arg1_nonconst = NULL;
    if(args[1]->IsString()) {
        NanUtf8String arg1_utf8(args[1]);
        arg1 = (const GLshort*)(*arg1_utf8);
    } else if(args[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(args[1]);
        arg1_nonconst = new GLshort[arg1_array->Length()];
        arg1 = (const GLshort*)arg1_nonconst;
    } else if(args[1]->IsObject()) {
        arg1 = (const GLshort*)glbind_get_buffer_data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLshort*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib1sv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttrib2d) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLdouble arg1;
    arg1 = args[1]->NumberValue();
    GLdouble arg2;
    arg2 = args[2]->NumberValue();
    glVertexAttrib2d(arg0, arg1, arg2);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttrib2dv) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    const GLdouble *arg1;
    GLdouble* arg1_nonconst = NULL;
    if(args[1]->IsString()) {
        NanUtf8String arg1_utf8(args[1]);
        arg1 = (const GLdouble*)(*arg1_utf8);
    } else if(args[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(args[1]);
        arg1_nonconst = new GLdouble[arg1_array->Length()];
        arg1 = (const GLdouble*)arg1_nonconst;
    } else if(args[1]->IsObject()) {
        arg1 = (const GLdouble*)glbind_get_buffer_data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLdouble*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib2dv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttrib2f) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLfloat arg1;
    arg1 = (float)args[1]->NumberValue();
    GLfloat arg2;
    arg2 = (float)args[2]->NumberValue();
    glVertexAttrib2f(arg0, arg1, arg2);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttrib2fv) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    const GLfloat *arg1;
    GLfloat* arg1_nonconst = NULL;
    if(args[1]->IsString()) {
        NanUtf8String arg1_utf8(args[1]);
        arg1 = (const GLfloat*)(*arg1_utf8);
    } else if(args[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(args[1]);
        arg1_nonconst = new GLfloat[arg1_array->Length()];
        arg1 = (const GLfloat*)arg1_nonconst;
    } else if(args[1]->IsObject()) {
        arg1 = (const GLfloat*)glbind_get_buffer_data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLfloat*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib2fv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttrib2s) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLshort arg1;
    arg1 = args[1]->Int32Value();
    GLshort arg2;
    arg2 = args[2]->Int32Value();
    glVertexAttrib2s(arg0, arg1, arg2);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttrib2sv) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    const GLshort *arg1;
    GLshort* arg1_nonconst = NULL;
    if(args[1]->IsString()) {
        NanUtf8String arg1_utf8(args[1]);
        arg1 = (const GLshort*)(*arg1_utf8);
    } else if(args[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(args[1]);
        arg1_nonconst = new GLshort[arg1_array->Length()];
        arg1 = (const GLshort*)arg1_nonconst;
    } else if(args[1]->IsObject()) {
        arg1 = (const GLshort*)glbind_get_buffer_data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLshort*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib2sv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttrib3d) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLdouble arg1;
    arg1 = args[1]->NumberValue();
    GLdouble arg2;
    arg2 = args[2]->NumberValue();
    GLdouble arg3;
    arg3 = args[3]->NumberValue();
    glVertexAttrib3d(arg0, arg1, arg2, arg3);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttrib3dv) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    const GLdouble *arg1;
    GLdouble* arg1_nonconst = NULL;
    if(args[1]->IsString()) {
        NanUtf8String arg1_utf8(args[1]);
        arg1 = (const GLdouble*)(*arg1_utf8);
    } else if(args[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(args[1]);
        arg1_nonconst = new GLdouble[arg1_array->Length()];
        arg1 = (const GLdouble*)arg1_nonconst;
    } else if(args[1]->IsObject()) {
        arg1 = (const GLdouble*)glbind_get_buffer_data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLdouble*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib3dv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttrib3f) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLfloat arg1;
    arg1 = (float)args[1]->NumberValue();
    GLfloat arg2;
    arg2 = (float)args[2]->NumberValue();
    GLfloat arg3;
    arg3 = (float)args[3]->NumberValue();
    glVertexAttrib3f(arg0, arg1, arg2, arg3);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttrib3fv) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    const GLfloat *arg1;
    GLfloat* arg1_nonconst = NULL;
    if(args[1]->IsString()) {
        NanUtf8String arg1_utf8(args[1]);
        arg1 = (const GLfloat*)(*arg1_utf8);
    } else if(args[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(args[1]);
        arg1_nonconst = new GLfloat[arg1_array->Length()];
        arg1 = (const GLfloat*)arg1_nonconst;
    } else if(args[1]->IsObject()) {
        arg1 = (const GLfloat*)glbind_get_buffer_data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLfloat*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib3fv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttrib3s) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLshort arg1;
    arg1 = args[1]->Int32Value();
    GLshort arg2;
    arg2 = args[2]->Int32Value();
    GLshort arg3;
    arg3 = args[3]->Int32Value();
    glVertexAttrib3s(arg0, arg1, arg2, arg3);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttrib3sv) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    const GLshort *arg1;
    GLshort* arg1_nonconst = NULL;
    if(args[1]->IsString()) {
        NanUtf8String arg1_utf8(args[1]);
        arg1 = (const GLshort*)(*arg1_utf8);
    } else if(args[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(args[1]);
        arg1_nonconst = new GLshort[arg1_array->Length()];
        arg1 = (const GLshort*)arg1_nonconst;
    } else if(args[1]->IsObject()) {
        arg1 = (const GLshort*)glbind_get_buffer_data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLshort*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib3sv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttrib4Nbv) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    const GLbyte *arg1;
    GLbyte* arg1_nonconst = NULL;
    if(args[1]->IsString()) {
        NanUtf8String arg1_utf8(args[1]);
        arg1 = (const GLbyte*)(*arg1_utf8);
    } else if(args[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(args[1]);
        arg1_nonconst = new GLbyte[arg1_array->Length()];
        arg1 = (const GLbyte*)arg1_nonconst;
    } else if(args[1]->IsObject()) {
        arg1 = (const GLbyte*)glbind_get_buffer_data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLbyte*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib4Nbv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttrib4Niv) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    const GLint *arg1;
    GLint* arg1_nonconst = NULL;
    if(args[1]->IsString()) {
        NanUtf8String arg1_utf8(args[1]);
        arg1 = (const GLint*)(*arg1_utf8);
    } else if(args[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(args[1]);
        arg1_nonconst = new GLint[arg1_array->Length()];
        arg1 = (const GLint*)arg1_nonconst;
    } else if(args[1]->IsObject()) {
        arg1 = (const GLint*)glbind_get_buffer_data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLint*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib4Niv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttrib4Nsv) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    const GLshort *arg1;
    GLshort* arg1_nonconst = NULL;
    if(args[1]->IsString()) {
        NanUtf8String arg1_utf8(args[1]);
        arg1 = (const GLshort*)(*arg1_utf8);
    } else if(args[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(args[1]);
        arg1_nonconst = new GLshort[arg1_array->Length()];
        arg1 = (const GLshort*)arg1_nonconst;
    } else if(args[1]->IsObject()) {
        arg1 = (const GLshort*)glbind_get_buffer_data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLshort*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib4Nsv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttrib4Nub) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLubyte arg1;
    arg1 = args[1]->Uint32Value();
    GLubyte arg2;
    arg2 = args[2]->Uint32Value();
    GLubyte arg3;
    arg3 = args[3]->Uint32Value();
    GLubyte arg4;
    arg4 = args[4]->Uint32Value();
    glVertexAttrib4Nub(arg0, arg1, arg2, arg3, arg4);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttrib4Nubv) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    const GLubyte *arg1;
    GLubyte* arg1_nonconst = NULL;
    if(args[1]->IsString()) {
        NanUtf8String arg1_utf8(args[1]);
        arg1 = (const GLubyte*)(*arg1_utf8);
    } else if(args[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(args[1]);
        arg1_nonconst = new GLubyte[arg1_array->Length()];
        arg1 = (const GLubyte*)arg1_nonconst;
    } else if(args[1]->IsObject()) {
        arg1 = (const GLubyte*)glbind_get_buffer_data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLubyte*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib4Nubv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttrib4Nuiv) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    const GLuint *arg1;
    GLuint* arg1_nonconst = NULL;
    if(args[1]->IsString()) {
        NanUtf8String arg1_utf8(args[1]);
        arg1 = (const GLuint*)(*arg1_utf8);
    } else if(args[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(args[1]);
        arg1_nonconst = new GLuint[arg1_array->Length()];
        arg1 = (const GLuint*)arg1_nonconst;
    } else if(args[1]->IsObject()) {
        arg1 = (const GLuint*)glbind_get_buffer_data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLuint*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib4Nuiv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttrib4Nusv) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    const GLushort *arg1;
    GLushort* arg1_nonconst = NULL;
    if(args[1]->IsString()) {
        NanUtf8String arg1_utf8(args[1]);
        arg1 = (const GLushort*)(*arg1_utf8);
    } else if(args[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(args[1]);
        arg1_nonconst = new GLushort[arg1_array->Length()];
        arg1 = (const GLushort*)arg1_nonconst;
    } else if(args[1]->IsObject()) {
        arg1 = (const GLushort*)glbind_get_buffer_data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLushort*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib4Nusv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttrib4bv) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    const GLbyte *arg1;
    GLbyte* arg1_nonconst = NULL;
    if(args[1]->IsString()) {
        NanUtf8String arg1_utf8(args[1]);
        arg1 = (const GLbyte*)(*arg1_utf8);
    } else if(args[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(args[1]);
        arg1_nonconst = new GLbyte[arg1_array->Length()];
        arg1 = (const GLbyte*)arg1_nonconst;
    } else if(args[1]->IsObject()) {
        arg1 = (const GLbyte*)glbind_get_buffer_data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLbyte*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib4bv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttrib4d) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLdouble arg1;
    arg1 = args[1]->NumberValue();
    GLdouble arg2;
    arg2 = args[2]->NumberValue();
    GLdouble arg3;
    arg3 = args[3]->NumberValue();
    GLdouble arg4;
    arg4 = args[4]->NumberValue();
    glVertexAttrib4d(arg0, arg1, arg2, arg3, arg4);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttrib4dv) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    const GLdouble *arg1;
    GLdouble* arg1_nonconst = NULL;
    if(args[1]->IsString()) {
        NanUtf8String arg1_utf8(args[1]);
        arg1 = (const GLdouble*)(*arg1_utf8);
    } else if(args[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(args[1]);
        arg1_nonconst = new GLdouble[arg1_array->Length()];
        arg1 = (const GLdouble*)arg1_nonconst;
    } else if(args[1]->IsObject()) {
        arg1 = (const GLdouble*)glbind_get_buffer_data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLdouble*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib4dv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttrib4f) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLfloat arg1;
    arg1 = (float)args[1]->NumberValue();
    GLfloat arg2;
    arg2 = (float)args[2]->NumberValue();
    GLfloat arg3;
    arg3 = (float)args[3]->NumberValue();
    GLfloat arg4;
    arg4 = (float)args[4]->NumberValue();
    glVertexAttrib4f(arg0, arg1, arg2, arg3, arg4);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttrib4fv) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    const GLfloat *arg1;
    GLfloat* arg1_nonconst = NULL;
    if(args[1]->IsString()) {
        NanUtf8String arg1_utf8(args[1]);
        arg1 = (const GLfloat*)(*arg1_utf8);
    } else if(args[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(args[1]);
        arg1_nonconst = new GLfloat[arg1_array->Length()];
        arg1 = (const GLfloat*)arg1_nonconst;
    } else if(args[1]->IsObject()) {
        arg1 = (const GLfloat*)glbind_get_buffer_data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLfloat*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib4fv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttrib4iv) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    const GLint *arg1;
    GLint* arg1_nonconst = NULL;
    if(args[1]->IsString()) {
        NanUtf8String arg1_utf8(args[1]);
        arg1 = (const GLint*)(*arg1_utf8);
    } else if(args[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(args[1]);
        arg1_nonconst = new GLint[arg1_array->Length()];
        arg1 = (const GLint*)arg1_nonconst;
    } else if(args[1]->IsObject()) {
        arg1 = (const GLint*)glbind_get_buffer_data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLint*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib4iv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttrib4s) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLshort arg1;
    arg1 = args[1]->Int32Value();
    GLshort arg2;
    arg2 = args[2]->Int32Value();
    GLshort arg3;
    arg3 = args[3]->Int32Value();
    GLshort arg4;
    arg4 = args[4]->Int32Value();
    glVertexAttrib4s(arg0, arg1, arg2, arg3, arg4);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttrib4sv) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    const GLshort *arg1;
    GLshort* arg1_nonconst = NULL;
    if(args[1]->IsString()) {
        NanUtf8String arg1_utf8(args[1]);
        arg1 = (const GLshort*)(*arg1_utf8);
    } else if(args[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(args[1]);
        arg1_nonconst = new GLshort[arg1_array->Length()];
        arg1 = (const GLshort*)arg1_nonconst;
    } else if(args[1]->IsObject()) {
        arg1 = (const GLshort*)glbind_get_buffer_data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLshort*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib4sv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttrib4ubv) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    const GLubyte *arg1;
    GLubyte* arg1_nonconst = NULL;
    if(args[1]->IsString()) {
        NanUtf8String arg1_utf8(args[1]);
        arg1 = (const GLubyte*)(*arg1_utf8);
    } else if(args[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(args[1]);
        arg1_nonconst = new GLubyte[arg1_array->Length()];
        arg1 = (const GLubyte*)arg1_nonconst;
    } else if(args[1]->IsObject()) {
        arg1 = (const GLubyte*)glbind_get_buffer_data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLubyte*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib4ubv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttrib4uiv) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    const GLuint *arg1;
    GLuint* arg1_nonconst = NULL;
    if(args[1]->IsString()) {
        NanUtf8String arg1_utf8(args[1]);
        arg1 = (const GLuint*)(*arg1_utf8);
    } else if(args[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(args[1]);
        arg1_nonconst = new GLuint[arg1_array->Length()];
        arg1 = (const GLuint*)arg1_nonconst;
    } else if(args[1]->IsObject()) {
        arg1 = (const GLuint*)glbind_get_buffer_data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLuint*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib4uiv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttrib4usv) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    const GLushort *arg1;
    GLushort* arg1_nonconst = NULL;
    if(args[1]->IsString()) {
        NanUtf8String arg1_utf8(args[1]);
        arg1 = (const GLushort*)(*arg1_utf8);
    } else if(args[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(args[1]);
        arg1_nonconst = new GLushort[arg1_array->Length()];
        arg1 = (const GLushort*)arg1_nonconst;
    } else if(args[1]->IsObject()) {
        arg1 = (const GLushort*)glbind_get_buffer_data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLushort*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib4usv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttribPointer) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLenum arg2;
    arg2 = args[2]->Uint32Value();
    GLboolean arg3;
    arg3 = args[3]->Uint32Value();
    GLsizei arg4;
    arg4 = args[4]->Int32Value();
    const GLvoid *arg5;
    GLubyte* arg5_nonconst = NULL;
    if(args[5]->IsString()) {
        NanUtf8String arg5_utf8(args[5]);
        arg5 = (const GLvoid*)(*arg5_utf8);
    } else if(args[5]->IsArray()) {
        v8::Handle<v8::Array> arg5_array = v8::Handle<v8::Array>::Cast(args[5]);
        arg5_nonconst = new GLubyte[arg5_array->Length()];
        arg5 = (const GLvoid*)arg5_nonconst;
    } else if(args[5]->IsObject()) {
        arg5 = (const GLvoid*)glbind_get_buffer_data(args[5]);
    } else if(args[5]->IsNumber()) {
        arg5 = (const GLvoid*)(args[5]->IntegerValue());
    } else {
        arg5 = NULL;
    }
    glVertexAttribPointer(arg0, arg1, arg2, arg3, arg4, arg5);
    if(arg5_nonconst) delete [] arg5_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_uniformMatrix2x3fv) {
    NanScope();
    // None
    GLint arg0;
    arg0 = args[0]->Int32Value();
    GLsizei arg1;
    arg1 = args[1]->Int32Value();
    GLboolean arg2;
    arg2 = args[2]->Uint32Value();
    const GLfloat *arg3;
    GLfloat* arg3_nonconst = NULL;
    if(args[3]->IsString()) {
        NanUtf8String arg3_utf8(args[3]);
        arg3 = (const GLfloat*)(*arg3_utf8);
    } else if(args[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(args[3]);
        arg3_nonconst = new GLfloat[arg3_array->Length()];
        arg3 = (const GLfloat*)arg3_nonconst;
    } else if(args[3]->IsObject()) {
        arg3 = (const GLfloat*)glbind_get_buffer_data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLfloat*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glUniformMatrix2x3fv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_uniformMatrix3x2fv) {
    NanScope();
    // None
    GLint arg0;
    arg0 = args[0]->Int32Value();
    GLsizei arg1;
    arg1 = args[1]->Int32Value();
    GLboolean arg2;
    arg2 = args[2]->Uint32Value();
    const GLfloat *arg3;
    GLfloat* arg3_nonconst = NULL;
    if(args[3]->IsString()) {
        NanUtf8String arg3_utf8(args[3]);
        arg3 = (const GLfloat*)(*arg3_utf8);
    } else if(args[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(args[3]);
        arg3_nonconst = new GLfloat[arg3_array->Length()];
        arg3 = (const GLfloat*)arg3_nonconst;
    } else if(args[3]->IsObject()) {
        arg3 = (const GLfloat*)glbind_get_buffer_data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLfloat*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glUniformMatrix3x2fv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_uniformMatrix2x4fv) {
    NanScope();
    // None
    GLint arg0;
    arg0 = args[0]->Int32Value();
    GLsizei arg1;
    arg1 = args[1]->Int32Value();
    GLboolean arg2;
    arg2 = args[2]->Uint32Value();
    const GLfloat *arg3;
    GLfloat* arg3_nonconst = NULL;
    if(args[3]->IsString()) {
        NanUtf8String arg3_utf8(args[3]);
        arg3 = (const GLfloat*)(*arg3_utf8);
    } else if(args[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(args[3]);
        arg3_nonconst = new GLfloat[arg3_array->Length()];
        arg3 = (const GLfloat*)arg3_nonconst;
    } else if(args[3]->IsObject()) {
        arg3 = (const GLfloat*)glbind_get_buffer_data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLfloat*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glUniformMatrix2x4fv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_uniformMatrix4x2fv) {
    NanScope();
    // None
    GLint arg0;
    arg0 = args[0]->Int32Value();
    GLsizei arg1;
    arg1 = args[1]->Int32Value();
    GLboolean arg2;
    arg2 = args[2]->Uint32Value();
    const GLfloat *arg3;
    GLfloat* arg3_nonconst = NULL;
    if(args[3]->IsString()) {
        NanUtf8String arg3_utf8(args[3]);
        arg3 = (const GLfloat*)(*arg3_utf8);
    } else if(args[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(args[3]);
        arg3_nonconst = new GLfloat[arg3_array->Length()];
        arg3 = (const GLfloat*)arg3_nonconst;
    } else if(args[3]->IsObject()) {
        arg3 = (const GLfloat*)glbind_get_buffer_data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLfloat*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glUniformMatrix4x2fv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_uniformMatrix3x4fv) {
    NanScope();
    // None
    GLint arg0;
    arg0 = args[0]->Int32Value();
    GLsizei arg1;
    arg1 = args[1]->Int32Value();
    GLboolean arg2;
    arg2 = args[2]->Uint32Value();
    const GLfloat *arg3;
    GLfloat* arg3_nonconst = NULL;
    if(args[3]->IsString()) {
        NanUtf8String arg3_utf8(args[3]);
        arg3 = (const GLfloat*)(*arg3_utf8);
    } else if(args[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(args[3]);
        arg3_nonconst = new GLfloat[arg3_array->Length()];
        arg3 = (const GLfloat*)arg3_nonconst;
    } else if(args[3]->IsObject()) {
        arg3 = (const GLfloat*)glbind_get_buffer_data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLfloat*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glUniformMatrix3x4fv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_uniformMatrix4x3fv) {
    NanScope();
    // None
    GLint arg0;
    arg0 = args[0]->Int32Value();
    GLsizei arg1;
    arg1 = args[1]->Int32Value();
    GLboolean arg2;
    arg2 = args[2]->Uint32Value();
    const GLfloat *arg3;
    GLfloat* arg3_nonconst = NULL;
    if(args[3]->IsString()) {
        NanUtf8String arg3_utf8(args[3]);
        arg3 = (const GLfloat*)(*arg3_utf8);
    } else if(args[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(args[3]);
        arg3_nonconst = new GLfloat[arg3_array->Length()];
        arg3 = (const GLfloat*)arg3_nonconst;
    } else if(args[3]->IsObject()) {
        arg3 = (const GLfloat*)glbind_get_buffer_data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLfloat*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glUniformMatrix4x3fv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_colorMaski) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLboolean arg1;
    arg1 = args[1]->Uint32Value();
    GLboolean arg2;
    arg2 = args[2]->Uint32Value();
    GLboolean arg3;
    arg3 = args[3]->Uint32Value();
    GLboolean arg4;
    arg4 = args[4]->Uint32Value();
    glColorMaski(arg0, arg1, arg2, arg3, arg4);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getBooleani_v) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLuint arg1;
    arg1 = args[1]->Uint32Value();
    GLboolean *arg2;
    GLboolean* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (GLboolean*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLboolean[arg2_array->Length()];
        arg2 = (GLboolean*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (GLboolean*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLboolean*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetBooleani_v(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getIntegeri_v) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLuint arg1;
    arg1 = args[1]->Uint32Value();
    GLint *arg2;
    GLint* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (GLint*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLint[arg2_array->Length()];
        arg2 = (GLint*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (GLint*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetIntegeri_v(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_enablei) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLuint arg1;
    arg1 = args[1]->Uint32Value();
    glEnablei(arg0, arg1);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_disablei) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLuint arg1;
    arg1 = args[1]->Uint32Value();
    glDisablei(arg0, arg1);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_isEnabledi) {
    NanScope();
    // <gltypes.Type instance at 0x104edf710>
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLuint arg1;
    arg1 = args[1]->Uint32Value();
    GLboolean result;
    result = glIsEnabledi(arg0, arg1);
    v8::Handle<v8::Value> result_js = NanNew<v8::Uint32>(result);
    NanReturnValue(result_js);
}
NAN_METHOD(EXPORT_beginTransformFeedback) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    glBeginTransformFeedback(arg0);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_endTransformFeedback) {
    NanScope();
    // None
    glEndTransformFeedback();
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_bindBufferRange) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLuint arg1;
    arg1 = args[1]->Uint32Value();
    GLuint arg2;
    arg2 = args[2]->Uint32Value();
    GLintptr arg3;
    arg3 = args[3]->IntegerValue();
    GLsizeiptr arg4;
    arg4 = (uint64_t)args[4]->IntegerValue();
    glBindBufferRange(arg0, arg1, arg2, arg3, arg4);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_bindBufferBase) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLuint arg1;
    arg1 = args[1]->Uint32Value();
    GLuint arg2;
    arg2 = args[2]->Uint32Value();
    glBindBufferBase(arg0, arg1, arg2);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getTransformFeedbackVarying) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLuint arg1;
    arg1 = args[1]->Uint32Value();
    GLsizei arg2;
    arg2 = args[2]->Int32Value();
    GLsizei *arg3;
    GLsizei* arg3_nonconst = NULL;
    if(args[3]->IsString()) {
        NanUtf8String arg3_utf8(args[3]);
        arg3 = (GLsizei*)(*arg3_utf8);
    } else if(args[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(args[3]);
        arg3_nonconst = new GLsizei[arg3_array->Length()];
        arg3 = (GLsizei*)arg3_nonconst;
    } else if(args[3]->IsObject()) {
        arg3 = (GLsizei*)glbind_get_buffer_data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (GLsizei*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    GLsizei *arg4;
    GLsizei* arg4_nonconst = NULL;
    if(args[4]->IsString()) {
        NanUtf8String arg4_utf8(args[4]);
        arg4 = (GLsizei*)(*arg4_utf8);
    } else if(args[4]->IsArray()) {
        v8::Handle<v8::Array> arg4_array = v8::Handle<v8::Array>::Cast(args[4]);
        arg4_nonconst = new GLsizei[arg4_array->Length()];
        arg4 = (GLsizei*)arg4_nonconst;
    } else if(args[4]->IsObject()) {
        arg4 = (GLsizei*)glbind_get_buffer_data(args[4]);
    } else if(args[4]->IsNumber()) {
        arg4 = (GLsizei*)(args[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    GLenum *arg5;
    GLenum* arg5_nonconst = NULL;
    if(args[5]->IsString()) {
        NanUtf8String arg5_utf8(args[5]);
        arg5 = (GLenum*)(*arg5_utf8);
    } else if(args[5]->IsArray()) {
        v8::Handle<v8::Array> arg5_array = v8::Handle<v8::Array>::Cast(args[5]);
        arg5_nonconst = new GLenum[arg5_array->Length()];
        arg5 = (GLenum*)arg5_nonconst;
    } else if(args[5]->IsObject()) {
        arg5 = (GLenum*)glbind_get_buffer_data(args[5]);
    } else if(args[5]->IsNumber()) {
        arg5 = (GLenum*)(args[5]->IntegerValue());
    } else {
        arg5 = NULL;
    }
    GLchar *arg6;
    GLchar* arg6_nonconst = NULL;
    if(args[6]->IsString()) {
        NanUtf8String arg6_utf8(args[6]);
        arg6 = (GLchar*)(*arg6_utf8);
    } else if(args[6]->IsArray()) {
        v8::Handle<v8::Array> arg6_array = v8::Handle<v8::Array>::Cast(args[6]);
        arg6_nonconst = new GLchar[arg6_array->Length()];
        arg6 = (GLchar*)arg6_nonconst;
    } else if(args[6]->IsObject()) {
        arg6 = (GLchar*)glbind_get_buffer_data(args[6]);
    } else if(args[6]->IsNumber()) {
        arg6 = (GLchar*)(args[6]->IntegerValue());
    } else {
        arg6 = NULL;
    }
    glGetTransformFeedbackVarying(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
    if(arg3_nonconst) delete [] arg3_nonconst;
    if(arg4_nonconst) delete [] arg4_nonconst;
    if(arg5_nonconst) delete [] arg5_nonconst;
    if(arg6_nonconst) delete [] arg6_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_clampColor) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    glClampColor(arg0, arg1);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_beginConditionalRender) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    glBeginConditionalRender(arg0, arg1);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_endConditionalRender) {
    NanScope();
    // None
    glEndConditionalRender();
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttribIPointer) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLenum arg2;
    arg2 = args[2]->Uint32Value();
    GLsizei arg3;
    arg3 = args[3]->Int32Value();
    const GLvoid *arg4;
    GLubyte* arg4_nonconst = NULL;
    if(args[4]->IsString()) {
        NanUtf8String arg4_utf8(args[4]);
        arg4 = (const GLvoid*)(*arg4_utf8);
    } else if(args[4]->IsArray()) {
        v8::Handle<v8::Array> arg4_array = v8::Handle<v8::Array>::Cast(args[4]);
        arg4_nonconst = new GLubyte[arg4_array->Length()];
        arg4 = (const GLvoid*)arg4_nonconst;
    } else if(args[4]->IsObject()) {
        arg4 = (const GLvoid*)glbind_get_buffer_data(args[4]);
    } else if(args[4]->IsNumber()) {
        arg4 = (const GLvoid*)(args[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glVertexAttribIPointer(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getVertexAttribIiv) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLint *arg2;
    GLint* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (GLint*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLint[arg2_array->Length()];
        arg2 = (GLint*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (GLint*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetVertexAttribIiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getVertexAttribIuiv) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLuint *arg2;
    GLuint* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (GLuint*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLuint[arg2_array->Length()];
        arg2 = (GLuint*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (GLuint*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLuint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetVertexAttribIuiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttribI1i) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLint arg1;
    arg1 = args[1]->Int32Value();
    glVertexAttribI1i(arg0, arg1);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttribI2i) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLint arg2;
    arg2 = args[2]->Int32Value();
    glVertexAttribI2i(arg0, arg1, arg2);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttribI3i) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLint arg2;
    arg2 = args[2]->Int32Value();
    GLint arg3;
    arg3 = args[3]->Int32Value();
    glVertexAttribI3i(arg0, arg1, arg2, arg3);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttribI4i) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLint arg2;
    arg2 = args[2]->Int32Value();
    GLint arg3;
    arg3 = args[3]->Int32Value();
    GLint arg4;
    arg4 = args[4]->Int32Value();
    glVertexAttribI4i(arg0, arg1, arg2, arg3, arg4);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttribI1ui) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLuint arg1;
    arg1 = args[1]->Uint32Value();
    glVertexAttribI1ui(arg0, arg1);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttribI2ui) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLuint arg1;
    arg1 = args[1]->Uint32Value();
    GLuint arg2;
    arg2 = args[2]->Uint32Value();
    glVertexAttribI2ui(arg0, arg1, arg2);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttribI3ui) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLuint arg1;
    arg1 = args[1]->Uint32Value();
    GLuint arg2;
    arg2 = args[2]->Uint32Value();
    GLuint arg3;
    arg3 = args[3]->Uint32Value();
    glVertexAttribI3ui(arg0, arg1, arg2, arg3);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttribI4ui) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLuint arg1;
    arg1 = args[1]->Uint32Value();
    GLuint arg2;
    arg2 = args[2]->Uint32Value();
    GLuint arg3;
    arg3 = args[3]->Uint32Value();
    GLuint arg4;
    arg4 = args[4]->Uint32Value();
    glVertexAttribI4ui(arg0, arg1, arg2, arg3, arg4);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttribI1iv) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    const GLint *arg1;
    GLint* arg1_nonconst = NULL;
    if(args[1]->IsString()) {
        NanUtf8String arg1_utf8(args[1]);
        arg1 = (const GLint*)(*arg1_utf8);
    } else if(args[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(args[1]);
        arg1_nonconst = new GLint[arg1_array->Length()];
        arg1 = (const GLint*)arg1_nonconst;
    } else if(args[1]->IsObject()) {
        arg1 = (const GLint*)glbind_get_buffer_data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLint*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttribI1iv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttribI2iv) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    const GLint *arg1;
    GLint* arg1_nonconst = NULL;
    if(args[1]->IsString()) {
        NanUtf8String arg1_utf8(args[1]);
        arg1 = (const GLint*)(*arg1_utf8);
    } else if(args[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(args[1]);
        arg1_nonconst = new GLint[arg1_array->Length()];
        arg1 = (const GLint*)arg1_nonconst;
    } else if(args[1]->IsObject()) {
        arg1 = (const GLint*)glbind_get_buffer_data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLint*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttribI2iv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttribI3iv) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    const GLint *arg1;
    GLint* arg1_nonconst = NULL;
    if(args[1]->IsString()) {
        NanUtf8String arg1_utf8(args[1]);
        arg1 = (const GLint*)(*arg1_utf8);
    } else if(args[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(args[1]);
        arg1_nonconst = new GLint[arg1_array->Length()];
        arg1 = (const GLint*)arg1_nonconst;
    } else if(args[1]->IsObject()) {
        arg1 = (const GLint*)glbind_get_buffer_data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLint*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttribI3iv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttribI4iv) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    const GLint *arg1;
    GLint* arg1_nonconst = NULL;
    if(args[1]->IsString()) {
        NanUtf8String arg1_utf8(args[1]);
        arg1 = (const GLint*)(*arg1_utf8);
    } else if(args[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(args[1]);
        arg1_nonconst = new GLint[arg1_array->Length()];
        arg1 = (const GLint*)arg1_nonconst;
    } else if(args[1]->IsObject()) {
        arg1 = (const GLint*)glbind_get_buffer_data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLint*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttribI4iv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttribI1uiv) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    const GLuint *arg1;
    GLuint* arg1_nonconst = NULL;
    if(args[1]->IsString()) {
        NanUtf8String arg1_utf8(args[1]);
        arg1 = (const GLuint*)(*arg1_utf8);
    } else if(args[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(args[1]);
        arg1_nonconst = new GLuint[arg1_array->Length()];
        arg1 = (const GLuint*)arg1_nonconst;
    } else if(args[1]->IsObject()) {
        arg1 = (const GLuint*)glbind_get_buffer_data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLuint*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttribI1uiv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttribI2uiv) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    const GLuint *arg1;
    GLuint* arg1_nonconst = NULL;
    if(args[1]->IsString()) {
        NanUtf8String arg1_utf8(args[1]);
        arg1 = (const GLuint*)(*arg1_utf8);
    } else if(args[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(args[1]);
        arg1_nonconst = new GLuint[arg1_array->Length()];
        arg1 = (const GLuint*)arg1_nonconst;
    } else if(args[1]->IsObject()) {
        arg1 = (const GLuint*)glbind_get_buffer_data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLuint*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttribI2uiv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttribI3uiv) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    const GLuint *arg1;
    GLuint* arg1_nonconst = NULL;
    if(args[1]->IsString()) {
        NanUtf8String arg1_utf8(args[1]);
        arg1 = (const GLuint*)(*arg1_utf8);
    } else if(args[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(args[1]);
        arg1_nonconst = new GLuint[arg1_array->Length()];
        arg1 = (const GLuint*)arg1_nonconst;
    } else if(args[1]->IsObject()) {
        arg1 = (const GLuint*)glbind_get_buffer_data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLuint*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttribI3uiv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttribI4uiv) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    const GLuint *arg1;
    GLuint* arg1_nonconst = NULL;
    if(args[1]->IsString()) {
        NanUtf8String arg1_utf8(args[1]);
        arg1 = (const GLuint*)(*arg1_utf8);
    } else if(args[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(args[1]);
        arg1_nonconst = new GLuint[arg1_array->Length()];
        arg1 = (const GLuint*)arg1_nonconst;
    } else if(args[1]->IsObject()) {
        arg1 = (const GLuint*)glbind_get_buffer_data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLuint*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttribI4uiv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttribI4bv) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    const GLbyte *arg1;
    GLbyte* arg1_nonconst = NULL;
    if(args[1]->IsString()) {
        NanUtf8String arg1_utf8(args[1]);
        arg1 = (const GLbyte*)(*arg1_utf8);
    } else if(args[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(args[1]);
        arg1_nonconst = new GLbyte[arg1_array->Length()];
        arg1 = (const GLbyte*)arg1_nonconst;
    } else if(args[1]->IsObject()) {
        arg1 = (const GLbyte*)glbind_get_buffer_data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLbyte*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttribI4bv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttribI4sv) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    const GLshort *arg1;
    GLshort* arg1_nonconst = NULL;
    if(args[1]->IsString()) {
        NanUtf8String arg1_utf8(args[1]);
        arg1 = (const GLshort*)(*arg1_utf8);
    } else if(args[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(args[1]);
        arg1_nonconst = new GLshort[arg1_array->Length()];
        arg1 = (const GLshort*)arg1_nonconst;
    } else if(args[1]->IsObject()) {
        arg1 = (const GLshort*)glbind_get_buffer_data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLshort*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttribI4sv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttribI4ubv) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    const GLubyte *arg1;
    GLubyte* arg1_nonconst = NULL;
    if(args[1]->IsString()) {
        NanUtf8String arg1_utf8(args[1]);
        arg1 = (const GLubyte*)(*arg1_utf8);
    } else if(args[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(args[1]);
        arg1_nonconst = new GLubyte[arg1_array->Length()];
        arg1 = (const GLubyte*)arg1_nonconst;
    } else if(args[1]->IsObject()) {
        arg1 = (const GLubyte*)glbind_get_buffer_data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLubyte*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttribI4ubv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttribI4usv) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    const GLushort *arg1;
    GLushort* arg1_nonconst = NULL;
    if(args[1]->IsString()) {
        NanUtf8String arg1_utf8(args[1]);
        arg1 = (const GLushort*)(*arg1_utf8);
    } else if(args[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(args[1]);
        arg1_nonconst = new GLushort[arg1_array->Length()];
        arg1 = (const GLushort*)arg1_nonconst;
    } else if(args[1]->IsObject()) {
        arg1 = (const GLushort*)glbind_get_buffer_data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLushort*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttribI4usv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getUniformuiv) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLuint *arg2;
    GLuint* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (GLuint*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLuint[arg2_array->Length()];
        arg2 = (GLuint*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (GLuint*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLuint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetUniformuiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_bindFragDataLocation) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLuint arg1;
    arg1 = args[1]->Uint32Value();
    const GLchar* arg2;
    NanUtf8String arg2_utf8(args[2]);
    arg2 = (GLchar*)*arg2_utf8;
    glBindFragDataLocation(arg0, arg1, arg2);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getFragDataLocation) {
    NanScope();
    // <gltypes.Type instance at 0x104edf878>
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    const GLchar* arg1;
    NanUtf8String arg1_utf8(args[1]);
    arg1 = (GLchar*)*arg1_utf8;
    GLint result;
    result = glGetFragDataLocation(arg0, arg1);
    v8::Handle<v8::Value> result_js = NanNew<v8::Int32>(result);
    NanReturnValue(result_js);
}
NAN_METHOD(EXPORT_uniform1ui) {
    NanScope();
    // None
    GLint arg0;
    arg0 = args[0]->Int32Value();
    GLuint arg1;
    arg1 = args[1]->Uint32Value();
    glUniform1ui(arg0, arg1);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_uniform2ui) {
    NanScope();
    // None
    GLint arg0;
    arg0 = args[0]->Int32Value();
    GLuint arg1;
    arg1 = args[1]->Uint32Value();
    GLuint arg2;
    arg2 = args[2]->Uint32Value();
    glUniform2ui(arg0, arg1, arg2);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_uniform3ui) {
    NanScope();
    // None
    GLint arg0;
    arg0 = args[0]->Int32Value();
    GLuint arg1;
    arg1 = args[1]->Uint32Value();
    GLuint arg2;
    arg2 = args[2]->Uint32Value();
    GLuint arg3;
    arg3 = args[3]->Uint32Value();
    glUniform3ui(arg0, arg1, arg2, arg3);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_uniform4ui) {
    NanScope();
    // None
    GLint arg0;
    arg0 = args[0]->Int32Value();
    GLuint arg1;
    arg1 = args[1]->Uint32Value();
    GLuint arg2;
    arg2 = args[2]->Uint32Value();
    GLuint arg3;
    arg3 = args[3]->Uint32Value();
    GLuint arg4;
    arg4 = args[4]->Uint32Value();
    glUniform4ui(arg0, arg1, arg2, arg3, arg4);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_uniform1uiv) {
    NanScope();
    // None
    GLint arg0;
    arg0 = args[0]->Int32Value();
    GLsizei arg1;
    arg1 = args[1]->Int32Value();
    const GLuint *arg2;
    GLuint* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (const GLuint*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLuint[arg2_array->Length()];
        arg2 = (const GLuint*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (const GLuint*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLuint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glUniform1uiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_uniform2uiv) {
    NanScope();
    // None
    GLint arg0;
    arg0 = args[0]->Int32Value();
    GLsizei arg1;
    arg1 = args[1]->Int32Value();
    const GLuint *arg2;
    GLuint* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (const GLuint*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLuint[arg2_array->Length()];
        arg2 = (const GLuint*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (const GLuint*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLuint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glUniform2uiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_uniform3uiv) {
    NanScope();
    // None
    GLint arg0;
    arg0 = args[0]->Int32Value();
    GLsizei arg1;
    arg1 = args[1]->Int32Value();
    const GLuint *arg2;
    GLuint* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (const GLuint*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLuint[arg2_array->Length()];
        arg2 = (const GLuint*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (const GLuint*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLuint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glUniform3uiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_uniform4uiv) {
    NanScope();
    // None
    GLint arg0;
    arg0 = args[0]->Int32Value();
    GLsizei arg1;
    arg1 = args[1]->Int32Value();
    const GLuint *arg2;
    GLuint* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (const GLuint*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLuint[arg2_array->Length()];
        arg2 = (const GLuint*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (const GLuint*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLuint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glUniform4uiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_texParameterIiv) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    const GLint *arg2;
    GLint* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (const GLint*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLint[arg2_array->Length()];
        arg2 = (const GLint*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (const GLint*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glTexParameterIiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_texParameterIuiv) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    const GLuint *arg2;
    GLuint* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (const GLuint*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLuint[arg2_array->Length()];
        arg2 = (const GLuint*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (const GLuint*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLuint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glTexParameterIuiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getTexParameterIiv) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLint *arg2;
    GLint* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (GLint*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLint[arg2_array->Length()];
        arg2 = (GLint*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (GLint*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetTexParameterIiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getTexParameterIuiv) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLuint *arg2;
    GLuint* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (GLuint*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLuint[arg2_array->Length()];
        arg2 = (GLuint*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (GLuint*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLuint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetTexParameterIuiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_clearBufferiv) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLint arg1;
    arg1 = args[1]->Int32Value();
    const GLint *arg2;
    GLint* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (const GLint*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLint[arg2_array->Length()];
        arg2 = (const GLint*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (const GLint*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glClearBufferiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_clearBufferuiv) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLint arg1;
    arg1 = args[1]->Int32Value();
    const GLuint *arg2;
    GLuint* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (const GLuint*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLuint[arg2_array->Length()];
        arg2 = (const GLuint*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (const GLuint*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLuint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glClearBufferuiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_clearBufferfv) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLint arg1;
    arg1 = args[1]->Int32Value();
    const GLfloat *arg2;
    GLfloat* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (const GLfloat*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLfloat[arg2_array->Length()];
        arg2 = (const GLfloat*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (const GLfloat*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLfloat*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glClearBufferfv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_clearBufferfi) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLfloat arg2;
    arg2 = (float)args[2]->NumberValue();
    GLint arg3;
    arg3 = args[3]->Int32Value();
    glClearBufferfi(arg0, arg1, arg2, arg3);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getStringi) {
    NanScope();
    // <gltypes.UStringType instance at 0x104edfcb0>
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLuint arg1;
    arg1 = args[1]->Uint32Value();
    const GLubyte* result;
    result = glGetStringi(arg0, arg1);
    v8::Handle<v8::Value> result_js;
    if(result) {
        result_js = NanNew<v8::String>((const char*)result, strlen((const char*)result));
    } else {
        result_js = NanUndefined();
    }
    NanReturnValue(result_js);
}
NAN_METHOD(EXPORT_drawArraysInstanced) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLsizei arg2;
    arg2 = args[2]->Int32Value();
    GLsizei arg3;
    arg3 = args[3]->Int32Value();
    glDrawArraysInstanced(arg0, arg1, arg2, arg3);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_drawElementsInstanced) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLsizei arg1;
    arg1 = args[1]->Int32Value();
    GLenum arg2;
    arg2 = args[2]->Uint32Value();
    const GLvoid *arg3;
    GLubyte* arg3_nonconst = NULL;
    if(args[3]->IsString()) {
        NanUtf8String arg3_utf8(args[3]);
        arg3 = (const GLvoid*)(*arg3_utf8);
    } else if(args[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(args[3]);
        arg3_nonconst = new GLubyte[arg3_array->Length()];
        arg3 = (const GLvoid*)arg3_nonconst;
    } else if(args[3]->IsObject()) {
        arg3 = (const GLvoid*)glbind_get_buffer_data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLvoid*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    GLsizei arg4;
    arg4 = args[4]->Int32Value();
    glDrawElementsInstanced(arg0, arg1, arg2, arg3, arg4);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_texBuffer) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLuint arg2;
    if(args[2]->IsNumber()) {
        arg2 = args[2]->IntegerValue();
    } else {
        arg2 = node::ObjectWrap::Unwrap<NODE_Buffer>(args[2]->ToObject())->gl_handle;
    }
    glTexBuffer(arg0, arg1, arg2);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_primitiveRestartIndex) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    glPrimitiveRestartIndex(arg0);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getInteger64i_v) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLuint arg1;
    arg1 = args[1]->Uint32Value();
    GLint64 *arg2;
    GLint64* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (GLint64*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLint64[arg2_array->Length()];
        arg2 = (GLint64*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (GLint64*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLint64*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetInteger64i_v(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getBufferParameteri64v) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLint64 *arg2;
    GLint64* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (GLint64*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLint64[arg2_array->Length()];
        arg2 = (GLint64*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (GLint64*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLint64*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetBufferParameteri64v(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_framebufferTexture) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLuint arg2;
    if(args[2]->IsNumber()) {
        arg2 = args[2]->IntegerValue();
    } else {
        arg2 = node::ObjectWrap::Unwrap<NODE_Texture>(args[2]->ToObject())->gl_handle;
    }
    GLint arg3;
    arg3 = args[3]->Int32Value();
    glFramebufferTexture(arg0, arg1, arg2, arg3);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttribDivisor) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLuint arg1;
    arg1 = args[1]->Uint32Value();
    glVertexAttribDivisor(arg0, arg1);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_minSampleShading) {
    NanScope();
    // None
    GLfloat arg0;
    arg0 = (float)args[0]->NumberValue();
    glMinSampleShading(arg0);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_blendEquationi) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    glBlendEquationi(arg0, arg1);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_blendEquationSeparatei) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLenum arg2;
    arg2 = args[2]->Uint32Value();
    glBlendEquationSeparatei(arg0, arg1, arg2);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_blendFunci) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLenum arg2;
    arg2 = args[2]->Uint32Value();
    glBlendFunci(arg0, arg1, arg2);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_blendFuncSeparatei) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLenum arg2;
    arg2 = args[2]->Uint32Value();
    GLenum arg3;
    arg3 = args[3]->Uint32Value();
    GLenum arg4;
    arg4 = args[4]->Uint32Value();
    glBlendFuncSeparatei(arg0, arg1, arg2, arg3, arg4);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_isRenderbuffer) {
    NanScope();
    // <gltypes.Type instance at 0x104edf710>
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Renderbuffer>(args[0]->ToObject())->gl_handle;
    }
    GLboolean result;
    result = glIsRenderbuffer(arg0);
    v8::Handle<v8::Value> result_js = NanNew<v8::Uint32>(result);
    NanReturnValue(result_js);
}
NAN_METHOD(EXPORT_bindRenderbuffer) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLuint arg1;
    if(args[1]->IsNumber()) {
        arg1 = args[1]->IntegerValue();
    } else {
        arg1 = node::ObjectWrap::Unwrap<NODE_Renderbuffer>(args[1]->ToObject())->gl_handle;
    }
    glBindRenderbuffer(arg0, arg1);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_genRenderbuffers) {
    NanScope();
    // None
    GLsizei arg0;
    arg0 = args[0]->Int32Value();
    GLuint* arg1;
    v8::Handle<v8::Array> args_1_array = v8::Handle<v8::Array>::Cast(args[1]);
    arg1 = new GLuint[args_1_array->Length()];
    glGenRenderbuffers(arg0, arg1);
    for(uint32_t i = 0; i < args_1_array->Length(); i++) {
        args_1_array->Set(i, NODE_Renderbuffer::fromGLHandle(arg1[i]));
    }
    delete [] arg1;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_renderbufferStorage) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLsizei arg2;
    arg2 = args[2]->Int32Value();
    GLsizei arg3;
    arg3 = args[3]->Int32Value();
    glRenderbufferStorage(arg0, arg1, arg2, arg3);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getRenderbufferParameteriv) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLint *arg2;
    GLint* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (GLint*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLint[arg2_array->Length()];
        arg2 = (GLint*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (GLint*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetRenderbufferParameteriv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_isFramebuffer) {
    NanScope();
    // <gltypes.Type instance at 0x104edf710>
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Framebuffer>(args[0]->ToObject())->gl_handle;
    }
    GLboolean result;
    result = glIsFramebuffer(arg0);
    v8::Handle<v8::Value> result_js = NanNew<v8::Uint32>(result);
    NanReturnValue(result_js);
}
NAN_METHOD(EXPORT_bindFramebuffer) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLuint arg1;
    if(args[1]->IsNumber()) {
        arg1 = args[1]->IntegerValue();
    } else {
        arg1 = node::ObjectWrap::Unwrap<NODE_Framebuffer>(args[1]->ToObject())->gl_handle;
    }
    glBindFramebuffer(arg0, arg1);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_genFramebuffers) {
    NanScope();
    // None
    GLsizei arg0;
    arg0 = args[0]->Int32Value();
    GLuint* arg1;
    v8::Handle<v8::Array> args_1_array = v8::Handle<v8::Array>::Cast(args[1]);
    arg1 = new GLuint[args_1_array->Length()];
    glGenFramebuffers(arg0, arg1);
    for(uint32_t i = 0; i < args_1_array->Length(); i++) {
        args_1_array->Set(i, NODE_Framebuffer::fromGLHandle(arg1[i]));
    }
    delete [] arg1;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_checkFramebufferStatus) {
    NanScope();
    // <gltypes.Type instance at 0x104edf7e8>
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLenum result;
    result = glCheckFramebufferStatus(arg0);
    v8::Handle<v8::Value> result_js = NanNew<v8::Uint32>(result);
    NanReturnValue(result_js);
}
NAN_METHOD(EXPORT_framebufferTexture1D) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLenum arg2;
    arg2 = args[2]->Uint32Value();
    GLuint arg3;
    if(args[3]->IsNumber()) {
        arg3 = args[3]->IntegerValue();
    } else {
        arg3 = node::ObjectWrap::Unwrap<NODE_Texture>(args[3]->ToObject())->gl_handle;
    }
    GLint arg4;
    arg4 = args[4]->Int32Value();
    glFramebufferTexture1D(arg0, arg1, arg2, arg3, arg4);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_framebufferTexture2D) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLenum arg2;
    arg2 = args[2]->Uint32Value();
    GLuint arg3;
    if(args[3]->IsNumber()) {
        arg3 = args[3]->IntegerValue();
    } else {
        arg3 = node::ObjectWrap::Unwrap<NODE_Texture>(args[3]->ToObject())->gl_handle;
    }
    GLint arg4;
    arg4 = args[4]->Int32Value();
    glFramebufferTexture2D(arg0, arg1, arg2, arg3, arg4);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_framebufferTexture3D) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLenum arg2;
    arg2 = args[2]->Uint32Value();
    GLuint arg3;
    if(args[3]->IsNumber()) {
        arg3 = args[3]->IntegerValue();
    } else {
        arg3 = node::ObjectWrap::Unwrap<NODE_Texture>(args[3]->ToObject())->gl_handle;
    }
    GLint arg4;
    arg4 = args[4]->Int32Value();
    GLint arg5;
    arg5 = args[5]->Int32Value();
    glFramebufferTexture3D(arg0, arg1, arg2, arg3, arg4, arg5);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_framebufferRenderbuffer) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLenum arg2;
    arg2 = args[2]->Uint32Value();
    GLuint arg3;
    if(args[3]->IsNumber()) {
        arg3 = args[3]->IntegerValue();
    } else {
        arg3 = node::ObjectWrap::Unwrap<NODE_Renderbuffer>(args[3]->ToObject())->gl_handle;
    }
    glFramebufferRenderbuffer(arg0, arg1, arg2, arg3);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getFramebufferAttachmentParameteriv) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLenum arg2;
    arg2 = args[2]->Uint32Value();
    GLint *arg3;
    GLint* arg3_nonconst = NULL;
    if(args[3]->IsString()) {
        NanUtf8String arg3_utf8(args[3]);
        arg3 = (GLint*)(*arg3_utf8);
    } else if(args[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(args[3]);
        arg3_nonconst = new GLint[arg3_array->Length()];
        arg3 = (GLint*)arg3_nonconst;
    } else if(args[3]->IsObject()) {
        arg3 = (GLint*)glbind_get_buffer_data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (GLint*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glGetFramebufferAttachmentParameteriv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_generateMipmap) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    glGenerateMipmap(arg0);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_blitFramebuffer) {
    NanScope();
    // None
    GLint arg0;
    arg0 = args[0]->Int32Value();
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLint arg2;
    arg2 = args[2]->Int32Value();
    GLint arg3;
    arg3 = args[3]->Int32Value();
    GLint arg4;
    arg4 = args[4]->Int32Value();
    GLint arg5;
    arg5 = args[5]->Int32Value();
    GLint arg6;
    arg6 = args[6]->Int32Value();
    GLint arg7;
    arg7 = args[7]->Int32Value();
    GLbitfield arg8;
    arg8 = args[8]->Uint32Value();
    GLenum arg9;
    arg9 = args[9]->Uint32Value();
    glBlitFramebuffer(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_renderbufferStorageMultisample) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLsizei arg1;
    arg1 = args[1]->Int32Value();
    GLenum arg2;
    arg2 = args[2]->Uint32Value();
    GLsizei arg3;
    arg3 = args[3]->Int32Value();
    GLsizei arg4;
    arg4 = args[4]->Int32Value();
    glRenderbufferStorageMultisample(arg0, arg1, arg2, arg3, arg4);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_framebufferTextureLayer) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLuint arg2;
    if(args[2]->IsNumber()) {
        arg2 = args[2]->IntegerValue();
    } else {
        arg2 = node::ObjectWrap::Unwrap<NODE_Texture>(args[2]->ToObject())->gl_handle;
    }
    GLint arg3;
    arg3 = args[3]->Int32Value();
    GLint arg4;
    arg4 = args[4]->Int32Value();
    glFramebufferTextureLayer(arg0, arg1, arg2, arg3, arg4);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_mapBufferRange) {
    NanScope();
    // <gltypes.PointerType instance at 0x1053d7e60>
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLintptr arg1;
    arg1 = args[1]->IntegerValue();
    GLsizeiptr arg2;
    arg2 = (uint64_t)args[2]->IntegerValue();
    GLbitfield arg3;
    arg3 = args[3]->Uint32Value();
    GLvoid *result;
    result = glMapBufferRange(arg0, arg1, arg2, arg3);
    v8::Handle<v8::Value> result_js = NanNewBufferHandle((char*)result, 0, do_nothing_release_callback, NULL);
    NanReturnValue(result_js);
}
NAN_METHOD(EXPORT_flushMappedBufferRange) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLintptr arg1;
    arg1 = args[1]->IntegerValue();
    GLsizeiptr arg2;
    arg2 = (uint64_t)args[2]->IntegerValue();
    glFlushMappedBufferRange(arg0, arg1, arg2);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_bindVertexArray) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_VertexArray>(args[0]->ToObject())->gl_handle;
    }
    glBindVertexArray(arg0);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_genVertexArrays) {
    NanScope();
    // None
    GLsizei arg0;
    arg0 = args[0]->Int32Value();
    GLuint* arg1;
    v8::Handle<v8::Array> args_1_array = v8::Handle<v8::Array>::Cast(args[1]);
    arg1 = new GLuint[args_1_array->Length()];
    glGenVertexArrays(arg0, arg1);
    for(uint32_t i = 0; i < args_1_array->Length(); i++) {
        args_1_array->Set(i, NODE_VertexArray::fromGLHandle(arg1[i]));
    }
    delete [] arg1;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_isVertexArray) {
    NanScope();
    // <gltypes.Type instance at 0x104edf710>
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_VertexArray>(args[0]->ToObject())->gl_handle;
    }
    GLboolean result;
    result = glIsVertexArray(arg0);
    v8::Handle<v8::Value> result_js = NanNew<v8::Uint32>(result);
    NanReturnValue(result_js);
}
NAN_METHOD(EXPORT_getActiveUniformsiv) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLsizei arg1;
    arg1 = args[1]->Int32Value();
    const GLuint *arg2;
    GLuint* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (const GLuint*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLuint[arg2_array->Length()];
        arg2 = (const GLuint*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (const GLuint*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLuint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    GLenum arg3;
    arg3 = args[3]->Uint32Value();
    GLint *arg4;
    GLint* arg4_nonconst = NULL;
    if(args[4]->IsString()) {
        NanUtf8String arg4_utf8(args[4]);
        arg4 = (GLint*)(*arg4_utf8);
    } else if(args[4]->IsArray()) {
        v8::Handle<v8::Array> arg4_array = v8::Handle<v8::Array>::Cast(args[4]);
        arg4_nonconst = new GLint[arg4_array->Length()];
        arg4 = (GLint*)arg4_nonconst;
    } else if(args[4]->IsObject()) {
        arg4 = (GLint*)glbind_get_buffer_data(args[4]);
    } else if(args[4]->IsNumber()) {
        arg4 = (GLint*)(args[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glGetActiveUniformsiv(arg0, arg1, arg2, arg3, arg4);
    if(arg2_nonconst) delete [] arg2_nonconst;
    if(arg4_nonconst) delete [] arg4_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getActiveUniformName) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLuint arg1;
    arg1 = args[1]->Uint32Value();
    GLsizei arg2;
    arg2 = args[2]->Int32Value();
    GLsizei *arg3;
    GLsizei* arg3_nonconst = NULL;
    if(args[3]->IsString()) {
        NanUtf8String arg3_utf8(args[3]);
        arg3 = (GLsizei*)(*arg3_utf8);
    } else if(args[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(args[3]);
        arg3_nonconst = new GLsizei[arg3_array->Length()];
        arg3 = (GLsizei*)arg3_nonconst;
    } else if(args[3]->IsObject()) {
        arg3 = (GLsizei*)glbind_get_buffer_data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (GLsizei*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    GLchar *arg4;
    GLchar* arg4_nonconst = NULL;
    if(args[4]->IsString()) {
        NanUtf8String arg4_utf8(args[4]);
        arg4 = (GLchar*)(*arg4_utf8);
    } else if(args[4]->IsArray()) {
        v8::Handle<v8::Array> arg4_array = v8::Handle<v8::Array>::Cast(args[4]);
        arg4_nonconst = new GLchar[arg4_array->Length()];
        arg4 = (GLchar*)arg4_nonconst;
    } else if(args[4]->IsObject()) {
        arg4 = (GLchar*)glbind_get_buffer_data(args[4]);
    } else if(args[4]->IsNumber()) {
        arg4 = (GLchar*)(args[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glGetActiveUniformName(arg0, arg1, arg2, arg3, arg4);
    if(arg3_nonconst) delete [] arg3_nonconst;
    if(arg4_nonconst) delete [] arg4_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getUniformBlockIndex) {
    NanScope();
    // <gltypes.Type instance at 0x104edf998>
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    const GLchar *arg1;
    GLchar* arg1_nonconst = NULL;
    if(args[1]->IsString()) {
        NanUtf8String arg1_utf8(args[1]);
        arg1 = (const GLchar*)(*arg1_utf8);
    } else if(args[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(args[1]);
        arg1_nonconst = new GLchar[arg1_array->Length()];
        arg1 = (const GLchar*)arg1_nonconst;
    } else if(args[1]->IsObject()) {
        arg1 = (const GLchar*)glbind_get_buffer_data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLchar*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    GLuint result;
    result = glGetUniformBlockIndex(arg0, arg1);
    v8::Handle<v8::Value> result_js = NanNew<v8::Uint32>(result);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnValue(result_js);
}
NAN_METHOD(EXPORT_getActiveUniformBlockiv) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLuint arg1;
    arg1 = args[1]->Uint32Value();
    GLenum arg2;
    arg2 = args[2]->Uint32Value();
    GLint *arg3;
    GLint* arg3_nonconst = NULL;
    if(args[3]->IsString()) {
        NanUtf8String arg3_utf8(args[3]);
        arg3 = (GLint*)(*arg3_utf8);
    } else if(args[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(args[3]);
        arg3_nonconst = new GLint[arg3_array->Length()];
        arg3 = (GLint*)arg3_nonconst;
    } else if(args[3]->IsObject()) {
        arg3 = (GLint*)glbind_get_buffer_data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (GLint*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glGetActiveUniformBlockiv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getActiveUniformBlockName) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLuint arg1;
    arg1 = args[1]->Uint32Value();
    GLsizei arg2;
    arg2 = args[2]->Int32Value();
    GLsizei *arg3;
    GLsizei* arg3_nonconst = NULL;
    if(args[3]->IsString()) {
        NanUtf8String arg3_utf8(args[3]);
        arg3 = (GLsizei*)(*arg3_utf8);
    } else if(args[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(args[3]);
        arg3_nonconst = new GLsizei[arg3_array->Length()];
        arg3 = (GLsizei*)arg3_nonconst;
    } else if(args[3]->IsObject()) {
        arg3 = (GLsizei*)glbind_get_buffer_data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (GLsizei*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    GLchar *arg4;
    GLchar* arg4_nonconst = NULL;
    if(args[4]->IsString()) {
        NanUtf8String arg4_utf8(args[4]);
        arg4 = (GLchar*)(*arg4_utf8);
    } else if(args[4]->IsArray()) {
        v8::Handle<v8::Array> arg4_array = v8::Handle<v8::Array>::Cast(args[4]);
        arg4_nonconst = new GLchar[arg4_array->Length()];
        arg4 = (GLchar*)arg4_nonconst;
    } else if(args[4]->IsObject()) {
        arg4 = (GLchar*)glbind_get_buffer_data(args[4]);
    } else if(args[4]->IsNumber()) {
        arg4 = (GLchar*)(args[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glGetActiveUniformBlockName(arg0, arg1, arg2, arg3, arg4);
    if(arg3_nonconst) delete [] arg3_nonconst;
    if(arg4_nonconst) delete [] arg4_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_uniformBlockBinding) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLuint arg1;
    arg1 = args[1]->Uint32Value();
    GLuint arg2;
    arg2 = args[2]->Uint32Value();
    glUniformBlockBinding(arg0, arg1, arg2);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_copyBufferSubData) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLintptr arg2;
    arg2 = args[2]->IntegerValue();
    GLintptr arg3;
    arg3 = args[3]->IntegerValue();
    GLsizeiptr arg4;
    arg4 = (uint64_t)args[4]->IntegerValue();
    glCopyBufferSubData(arg0, arg1, arg2, arg3, arg4);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_drawElementsBaseVertex) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLsizei arg1;
    arg1 = args[1]->Int32Value();
    GLenum arg2;
    arg2 = args[2]->Uint32Value();
    const GLvoid *arg3;
    GLubyte* arg3_nonconst = NULL;
    if(args[3]->IsString()) {
        NanUtf8String arg3_utf8(args[3]);
        arg3 = (const GLvoid*)(*arg3_utf8);
    } else if(args[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(args[3]);
        arg3_nonconst = new GLubyte[arg3_array->Length()];
        arg3 = (const GLvoid*)arg3_nonconst;
    } else if(args[3]->IsObject()) {
        arg3 = (const GLvoid*)glbind_get_buffer_data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLvoid*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    GLint arg4;
    arg4 = args[4]->Int32Value();
    glDrawElementsBaseVertex(arg0, arg1, arg2, arg3, arg4);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_drawRangeElementsBaseVertex) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLuint arg1;
    arg1 = args[1]->Uint32Value();
    GLuint arg2;
    arg2 = args[2]->Uint32Value();
    GLsizei arg3;
    arg3 = args[3]->Int32Value();
    GLenum arg4;
    arg4 = args[4]->Uint32Value();
    const GLvoid *arg5;
    GLubyte* arg5_nonconst = NULL;
    if(args[5]->IsString()) {
        NanUtf8String arg5_utf8(args[5]);
        arg5 = (const GLvoid*)(*arg5_utf8);
    } else if(args[5]->IsArray()) {
        v8::Handle<v8::Array> arg5_array = v8::Handle<v8::Array>::Cast(args[5]);
        arg5_nonconst = new GLubyte[arg5_array->Length()];
        arg5 = (const GLvoid*)arg5_nonconst;
    } else if(args[5]->IsObject()) {
        arg5 = (const GLvoid*)glbind_get_buffer_data(args[5]);
    } else if(args[5]->IsNumber()) {
        arg5 = (const GLvoid*)(args[5]->IntegerValue());
    } else {
        arg5 = NULL;
    }
    GLint arg6;
    arg6 = args[6]->Int32Value();
    glDrawRangeElementsBaseVertex(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
    if(arg5_nonconst) delete [] arg5_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_drawElementsInstancedBaseVertex) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLsizei arg1;
    arg1 = args[1]->Int32Value();
    GLenum arg2;
    arg2 = args[2]->Uint32Value();
    const GLvoid *arg3;
    GLubyte* arg3_nonconst = NULL;
    if(args[3]->IsString()) {
        NanUtf8String arg3_utf8(args[3]);
        arg3 = (const GLvoid*)(*arg3_utf8);
    } else if(args[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(args[3]);
        arg3_nonconst = new GLubyte[arg3_array->Length()];
        arg3 = (const GLvoid*)arg3_nonconst;
    } else if(args[3]->IsObject()) {
        arg3 = (const GLvoid*)glbind_get_buffer_data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLvoid*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    GLsizei arg4;
    arg4 = args[4]->Int32Value();
    GLint arg5;
    arg5 = args[5]->Int32Value();
    glDrawElementsInstancedBaseVertex(arg0, arg1, arg2, arg3, arg4, arg5);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_provokingVertex) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    glProvokingVertex(arg0);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_texImage2DMultisample) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLsizei arg1;
    arg1 = args[1]->Int32Value();
    GLint arg2;
    arg2 = args[2]->Int32Value();
    GLsizei arg3;
    arg3 = args[3]->Int32Value();
    GLsizei arg4;
    arg4 = args[4]->Int32Value();
    GLboolean arg5;
    arg5 = args[5]->Uint32Value();
    glTexImage2DMultisample(arg0, arg1, arg2, arg3, arg4, arg5);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_texImage3DMultisample) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLsizei arg1;
    arg1 = args[1]->Int32Value();
    GLint arg2;
    arg2 = args[2]->Int32Value();
    GLsizei arg3;
    arg3 = args[3]->Int32Value();
    GLsizei arg4;
    arg4 = args[4]->Int32Value();
    GLsizei arg5;
    arg5 = args[5]->Int32Value();
    GLboolean arg6;
    arg6 = args[6]->Uint32Value();
    glTexImage3DMultisample(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getMultisamplefv) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLuint arg1;
    arg1 = args[1]->Uint32Value();
    GLfloat *arg2;
    GLfloat* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (GLfloat*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLfloat[arg2_array->Length()];
        arg2 = (GLfloat*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (GLfloat*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLfloat*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetMultisamplefv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_sampleMaski) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLbitfield arg1;
    arg1 = args[1]->Uint32Value();
    glSampleMaski(arg0, arg1);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_bindFragDataLocationIndexed) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLuint arg1;
    arg1 = args[1]->Uint32Value();
    GLuint arg2;
    arg2 = args[2]->Uint32Value();
    const GLchar *arg3;
    GLchar* arg3_nonconst = NULL;
    if(args[3]->IsString()) {
        NanUtf8String arg3_utf8(args[3]);
        arg3 = (const GLchar*)(*arg3_utf8);
    } else if(args[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(args[3]);
        arg3_nonconst = new GLchar[arg3_array->Length()];
        arg3 = (const GLchar*)arg3_nonconst;
    } else if(args[3]->IsObject()) {
        arg3 = (const GLchar*)glbind_get_buffer_data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLchar*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glBindFragDataLocationIndexed(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getFragDataIndex) {
    NanScope();
    // <gltypes.Type instance at 0x104edf878>
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    const GLchar *arg1;
    GLchar* arg1_nonconst = NULL;
    if(args[1]->IsString()) {
        NanUtf8String arg1_utf8(args[1]);
        arg1 = (const GLchar*)(*arg1_utf8);
    } else if(args[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(args[1]);
        arg1_nonconst = new GLchar[arg1_array->Length()];
        arg1 = (const GLchar*)arg1_nonconst;
    } else if(args[1]->IsObject()) {
        arg1 = (const GLchar*)glbind_get_buffer_data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLchar*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    GLint result;
    result = glGetFragDataIndex(arg0, arg1);
    v8::Handle<v8::Value> result_js = NanNew<v8::Int32>(result);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnValue(result_js);
}
NAN_METHOD(EXPORT_genSamplers) {
    NanScope();
    // None
    GLsizei arg0;
    arg0 = args[0]->Int32Value();
    GLuint* arg1;
    v8::Handle<v8::Array> args_1_array = v8::Handle<v8::Array>::Cast(args[1]);
    arg1 = new GLuint[args_1_array->Length()];
    glGenSamplers(arg0, arg1);
    for(uint32_t i = 0; i < args_1_array->Length(); i++) {
        args_1_array->Set(i, NODE_Sampler::fromGLHandle(arg1[i]));
    }
    delete [] arg1;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_isSampler) {
    NanScope();
    // <gltypes.Type instance at 0x104edf710>
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Sampler>(args[0]->ToObject())->gl_handle;
    }
    GLboolean result;
    result = glIsSampler(arg0);
    v8::Handle<v8::Value> result_js = NanNew<v8::Uint32>(result);
    NanReturnValue(result_js);
}
NAN_METHOD(EXPORT_bindSampler) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLuint arg1;
    if(args[1]->IsNumber()) {
        arg1 = args[1]->IntegerValue();
    } else {
        arg1 = node::ObjectWrap::Unwrap<NODE_Sampler>(args[1]->ToObject())->gl_handle;
    }
    glBindSampler(arg0, arg1);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_samplerParameteri) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Sampler>(args[0]->ToObject())->gl_handle;
    }
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLint arg2;
    arg2 = args[2]->Int32Value();
    glSamplerParameteri(arg0, arg1, arg2);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_samplerParameteriv) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Sampler>(args[0]->ToObject())->gl_handle;
    }
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    const GLint *arg2;
    GLint* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (const GLint*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLint[arg2_array->Length()];
        arg2 = (const GLint*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (const GLint*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glSamplerParameteriv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_samplerParameterf) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Sampler>(args[0]->ToObject())->gl_handle;
    }
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLfloat arg2;
    arg2 = (float)args[2]->NumberValue();
    glSamplerParameterf(arg0, arg1, arg2);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_samplerParameterfv) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Sampler>(args[0]->ToObject())->gl_handle;
    }
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    const GLfloat *arg2;
    GLfloat* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (const GLfloat*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLfloat[arg2_array->Length()];
        arg2 = (const GLfloat*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (const GLfloat*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLfloat*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glSamplerParameterfv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_samplerParameterIiv) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Sampler>(args[0]->ToObject())->gl_handle;
    }
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    const GLint *arg2;
    GLint* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (const GLint*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLint[arg2_array->Length()];
        arg2 = (const GLint*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (const GLint*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glSamplerParameterIiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_samplerParameterIuiv) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Sampler>(args[0]->ToObject())->gl_handle;
    }
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    const GLuint *arg2;
    GLuint* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (const GLuint*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLuint[arg2_array->Length()];
        arg2 = (const GLuint*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (const GLuint*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLuint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glSamplerParameterIuiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getSamplerParameteriv) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Sampler>(args[0]->ToObject())->gl_handle;
    }
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLint *arg2;
    GLint* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (GLint*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLint[arg2_array->Length()];
        arg2 = (GLint*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (GLint*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetSamplerParameteriv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getSamplerParameterIiv) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Sampler>(args[0]->ToObject())->gl_handle;
    }
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLint *arg2;
    GLint* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (GLint*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLint[arg2_array->Length()];
        arg2 = (GLint*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (GLint*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetSamplerParameterIiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getSamplerParameterfv) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Sampler>(args[0]->ToObject())->gl_handle;
    }
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLfloat *arg2;
    GLfloat* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (GLfloat*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLfloat[arg2_array->Length()];
        arg2 = (GLfloat*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (GLfloat*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLfloat*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetSamplerParameterfv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getSamplerParameterIuiv) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Sampler>(args[0]->ToObject())->gl_handle;
    }
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLuint *arg2;
    GLuint* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (GLuint*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLuint[arg2_array->Length()];
        arg2 = (GLuint*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (GLuint*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLuint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetSamplerParameterIuiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_queryCounter) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    glQueryCounter(arg0, arg1);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getQueryObjecti64v) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLint64 *arg2;
    GLint64* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (GLint64*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLint64[arg2_array->Length()];
        arg2 = (GLint64*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (GLint64*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLint64*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetQueryObjecti64v(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getQueryObjectui64v) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLuint64 *arg2;
    GLuint64* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (GLuint64*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLuint64[arg2_array->Length()];
        arg2 = (GLuint64*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (GLuint64*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLuint64*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetQueryObjectui64v(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttribP1ui) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLboolean arg2;
    arg2 = args[2]->Uint32Value();
    GLuint arg3;
    arg3 = args[3]->Uint32Value();
    glVertexAttribP1ui(arg0, arg1, arg2, arg3);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttribP1uiv) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLboolean arg2;
    arg2 = args[2]->Uint32Value();
    const GLuint *arg3;
    GLuint* arg3_nonconst = NULL;
    if(args[3]->IsString()) {
        NanUtf8String arg3_utf8(args[3]);
        arg3 = (const GLuint*)(*arg3_utf8);
    } else if(args[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(args[3]);
        arg3_nonconst = new GLuint[arg3_array->Length()];
        arg3 = (const GLuint*)arg3_nonconst;
    } else if(args[3]->IsObject()) {
        arg3 = (const GLuint*)glbind_get_buffer_data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLuint*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glVertexAttribP1uiv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttribP2ui) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLboolean arg2;
    arg2 = args[2]->Uint32Value();
    GLuint arg3;
    arg3 = args[3]->Uint32Value();
    glVertexAttribP2ui(arg0, arg1, arg2, arg3);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttribP2uiv) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLboolean arg2;
    arg2 = args[2]->Uint32Value();
    const GLuint *arg3;
    GLuint* arg3_nonconst = NULL;
    if(args[3]->IsString()) {
        NanUtf8String arg3_utf8(args[3]);
        arg3 = (const GLuint*)(*arg3_utf8);
    } else if(args[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(args[3]);
        arg3_nonconst = new GLuint[arg3_array->Length()];
        arg3 = (const GLuint*)arg3_nonconst;
    } else if(args[3]->IsObject()) {
        arg3 = (const GLuint*)glbind_get_buffer_data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLuint*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glVertexAttribP2uiv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttribP3ui) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLboolean arg2;
    arg2 = args[2]->Uint32Value();
    GLuint arg3;
    arg3 = args[3]->Uint32Value();
    glVertexAttribP3ui(arg0, arg1, arg2, arg3);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttribP3uiv) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLboolean arg2;
    arg2 = args[2]->Uint32Value();
    const GLuint *arg3;
    GLuint* arg3_nonconst = NULL;
    if(args[3]->IsString()) {
        NanUtf8String arg3_utf8(args[3]);
        arg3 = (const GLuint*)(*arg3_utf8);
    } else if(args[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(args[3]);
        arg3_nonconst = new GLuint[arg3_array->Length()];
        arg3 = (const GLuint*)arg3_nonconst;
    } else if(args[3]->IsObject()) {
        arg3 = (const GLuint*)glbind_get_buffer_data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLuint*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glVertexAttribP3uiv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttribP4ui) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLboolean arg2;
    arg2 = args[2]->Uint32Value();
    GLuint arg3;
    arg3 = args[3]->Uint32Value();
    glVertexAttribP4ui(arg0, arg1, arg2, arg3);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttribP4uiv) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLboolean arg2;
    arg2 = args[2]->Uint32Value();
    const GLuint *arg3;
    GLuint* arg3_nonconst = NULL;
    if(args[3]->IsString()) {
        NanUtf8String arg3_utf8(args[3]);
        arg3 = (const GLuint*)(*arg3_utf8);
    } else if(args[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(args[3]);
        arg3_nonconst = new GLuint[arg3_array->Length()];
        arg3 = (const GLuint*)arg3_nonconst;
    } else if(args[3]->IsObject()) {
        arg3 = (const GLuint*)glbind_get_buffer_data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLuint*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glVertexAttribP4uiv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_drawArraysIndirect) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    const GLvoid *arg1;
    GLubyte* arg1_nonconst = NULL;
    if(args[1]->IsString()) {
        NanUtf8String arg1_utf8(args[1]);
        arg1 = (const GLvoid*)(*arg1_utf8);
    } else if(args[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(args[1]);
        arg1_nonconst = new GLubyte[arg1_array->Length()];
        arg1 = (const GLvoid*)arg1_nonconst;
    } else if(args[1]->IsObject()) {
        arg1 = (const GLvoid*)glbind_get_buffer_data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLvoid*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glDrawArraysIndirect(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_drawElementsIndirect) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    const GLvoid *arg2;
    GLubyte* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (const GLvoid*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLubyte[arg2_array->Length()];
        arg2 = (const GLvoid*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (const GLvoid*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLvoid*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glDrawElementsIndirect(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_uniform1d) {
    NanScope();
    // None
    GLint arg0;
    arg0 = args[0]->Int32Value();
    GLdouble arg1;
    arg1 = args[1]->NumberValue();
    glUniform1d(arg0, arg1);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_uniform2d) {
    NanScope();
    // None
    GLint arg0;
    arg0 = args[0]->Int32Value();
    GLdouble arg1;
    arg1 = args[1]->NumberValue();
    GLdouble arg2;
    arg2 = args[2]->NumberValue();
    glUniform2d(arg0, arg1, arg2);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_uniform3d) {
    NanScope();
    // None
    GLint arg0;
    arg0 = args[0]->Int32Value();
    GLdouble arg1;
    arg1 = args[1]->NumberValue();
    GLdouble arg2;
    arg2 = args[2]->NumberValue();
    GLdouble arg3;
    arg3 = args[3]->NumberValue();
    glUniform3d(arg0, arg1, arg2, arg3);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_uniform4d) {
    NanScope();
    // None
    GLint arg0;
    arg0 = args[0]->Int32Value();
    GLdouble arg1;
    arg1 = args[1]->NumberValue();
    GLdouble arg2;
    arg2 = args[2]->NumberValue();
    GLdouble arg3;
    arg3 = args[3]->NumberValue();
    GLdouble arg4;
    arg4 = args[4]->NumberValue();
    glUniform4d(arg0, arg1, arg2, arg3, arg4);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_uniform1dv) {
    NanScope();
    // None
    GLint arg0;
    arg0 = args[0]->Int32Value();
    GLsizei arg1;
    arg1 = args[1]->Int32Value();
    const GLdouble *arg2;
    GLdouble* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (const GLdouble*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLdouble[arg2_array->Length()];
        arg2 = (const GLdouble*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (const GLdouble*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLdouble*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glUniform1dv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_uniform2dv) {
    NanScope();
    // None
    GLint arg0;
    arg0 = args[0]->Int32Value();
    GLsizei arg1;
    arg1 = args[1]->Int32Value();
    const GLdouble *arg2;
    GLdouble* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (const GLdouble*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLdouble[arg2_array->Length()];
        arg2 = (const GLdouble*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (const GLdouble*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLdouble*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glUniform2dv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_uniform3dv) {
    NanScope();
    // None
    GLint arg0;
    arg0 = args[0]->Int32Value();
    GLsizei arg1;
    arg1 = args[1]->Int32Value();
    const GLdouble *arg2;
    GLdouble* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (const GLdouble*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLdouble[arg2_array->Length()];
        arg2 = (const GLdouble*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (const GLdouble*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLdouble*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glUniform3dv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_uniform4dv) {
    NanScope();
    // None
    GLint arg0;
    arg0 = args[0]->Int32Value();
    GLsizei arg1;
    arg1 = args[1]->Int32Value();
    const GLdouble *arg2;
    GLdouble* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (const GLdouble*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLdouble[arg2_array->Length()];
        arg2 = (const GLdouble*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (const GLdouble*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLdouble*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glUniform4dv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_uniformMatrix2dv) {
    NanScope();
    // None
    GLint arg0;
    arg0 = args[0]->Int32Value();
    GLsizei arg1;
    arg1 = args[1]->Int32Value();
    GLboolean arg2;
    arg2 = args[2]->Uint32Value();
    const GLdouble *arg3;
    GLdouble* arg3_nonconst = NULL;
    if(args[3]->IsString()) {
        NanUtf8String arg3_utf8(args[3]);
        arg3 = (const GLdouble*)(*arg3_utf8);
    } else if(args[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(args[3]);
        arg3_nonconst = new GLdouble[arg3_array->Length()];
        arg3 = (const GLdouble*)arg3_nonconst;
    } else if(args[3]->IsObject()) {
        arg3 = (const GLdouble*)glbind_get_buffer_data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLdouble*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glUniformMatrix2dv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_uniformMatrix3dv) {
    NanScope();
    // None
    GLint arg0;
    arg0 = args[0]->Int32Value();
    GLsizei arg1;
    arg1 = args[1]->Int32Value();
    GLboolean arg2;
    arg2 = args[2]->Uint32Value();
    const GLdouble *arg3;
    GLdouble* arg3_nonconst = NULL;
    if(args[3]->IsString()) {
        NanUtf8String arg3_utf8(args[3]);
        arg3 = (const GLdouble*)(*arg3_utf8);
    } else if(args[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(args[3]);
        arg3_nonconst = new GLdouble[arg3_array->Length()];
        arg3 = (const GLdouble*)arg3_nonconst;
    } else if(args[3]->IsObject()) {
        arg3 = (const GLdouble*)glbind_get_buffer_data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLdouble*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glUniformMatrix3dv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_uniformMatrix4dv) {
    NanScope();
    // None
    GLint arg0;
    arg0 = args[0]->Int32Value();
    GLsizei arg1;
    arg1 = args[1]->Int32Value();
    GLboolean arg2;
    arg2 = args[2]->Uint32Value();
    const GLdouble *arg3;
    GLdouble* arg3_nonconst = NULL;
    if(args[3]->IsString()) {
        NanUtf8String arg3_utf8(args[3]);
        arg3 = (const GLdouble*)(*arg3_utf8);
    } else if(args[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(args[3]);
        arg3_nonconst = new GLdouble[arg3_array->Length()];
        arg3 = (const GLdouble*)arg3_nonconst;
    } else if(args[3]->IsObject()) {
        arg3 = (const GLdouble*)glbind_get_buffer_data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLdouble*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glUniformMatrix4dv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_uniformMatrix2x3dv) {
    NanScope();
    // None
    GLint arg0;
    arg0 = args[0]->Int32Value();
    GLsizei arg1;
    arg1 = args[1]->Int32Value();
    GLboolean arg2;
    arg2 = args[2]->Uint32Value();
    const GLdouble *arg3;
    GLdouble* arg3_nonconst = NULL;
    if(args[3]->IsString()) {
        NanUtf8String arg3_utf8(args[3]);
        arg3 = (const GLdouble*)(*arg3_utf8);
    } else if(args[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(args[3]);
        arg3_nonconst = new GLdouble[arg3_array->Length()];
        arg3 = (const GLdouble*)arg3_nonconst;
    } else if(args[3]->IsObject()) {
        arg3 = (const GLdouble*)glbind_get_buffer_data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLdouble*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glUniformMatrix2x3dv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_uniformMatrix2x4dv) {
    NanScope();
    // None
    GLint arg0;
    arg0 = args[0]->Int32Value();
    GLsizei arg1;
    arg1 = args[1]->Int32Value();
    GLboolean arg2;
    arg2 = args[2]->Uint32Value();
    const GLdouble *arg3;
    GLdouble* arg3_nonconst = NULL;
    if(args[3]->IsString()) {
        NanUtf8String arg3_utf8(args[3]);
        arg3 = (const GLdouble*)(*arg3_utf8);
    } else if(args[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(args[3]);
        arg3_nonconst = new GLdouble[arg3_array->Length()];
        arg3 = (const GLdouble*)arg3_nonconst;
    } else if(args[3]->IsObject()) {
        arg3 = (const GLdouble*)glbind_get_buffer_data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLdouble*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glUniformMatrix2x4dv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_uniformMatrix3x2dv) {
    NanScope();
    // None
    GLint arg0;
    arg0 = args[0]->Int32Value();
    GLsizei arg1;
    arg1 = args[1]->Int32Value();
    GLboolean arg2;
    arg2 = args[2]->Uint32Value();
    const GLdouble *arg3;
    GLdouble* arg3_nonconst = NULL;
    if(args[3]->IsString()) {
        NanUtf8String arg3_utf8(args[3]);
        arg3 = (const GLdouble*)(*arg3_utf8);
    } else if(args[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(args[3]);
        arg3_nonconst = new GLdouble[arg3_array->Length()];
        arg3 = (const GLdouble*)arg3_nonconst;
    } else if(args[3]->IsObject()) {
        arg3 = (const GLdouble*)glbind_get_buffer_data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLdouble*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glUniformMatrix3x2dv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_uniformMatrix3x4dv) {
    NanScope();
    // None
    GLint arg0;
    arg0 = args[0]->Int32Value();
    GLsizei arg1;
    arg1 = args[1]->Int32Value();
    GLboolean arg2;
    arg2 = args[2]->Uint32Value();
    const GLdouble *arg3;
    GLdouble* arg3_nonconst = NULL;
    if(args[3]->IsString()) {
        NanUtf8String arg3_utf8(args[3]);
        arg3 = (const GLdouble*)(*arg3_utf8);
    } else if(args[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(args[3]);
        arg3_nonconst = new GLdouble[arg3_array->Length()];
        arg3 = (const GLdouble*)arg3_nonconst;
    } else if(args[3]->IsObject()) {
        arg3 = (const GLdouble*)glbind_get_buffer_data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLdouble*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glUniformMatrix3x4dv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_uniformMatrix4x2dv) {
    NanScope();
    // None
    GLint arg0;
    arg0 = args[0]->Int32Value();
    GLsizei arg1;
    arg1 = args[1]->Int32Value();
    GLboolean arg2;
    arg2 = args[2]->Uint32Value();
    const GLdouble *arg3;
    GLdouble* arg3_nonconst = NULL;
    if(args[3]->IsString()) {
        NanUtf8String arg3_utf8(args[3]);
        arg3 = (const GLdouble*)(*arg3_utf8);
    } else if(args[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(args[3]);
        arg3_nonconst = new GLdouble[arg3_array->Length()];
        arg3 = (const GLdouble*)arg3_nonconst;
    } else if(args[3]->IsObject()) {
        arg3 = (const GLdouble*)glbind_get_buffer_data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLdouble*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glUniformMatrix4x2dv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_uniformMatrix4x3dv) {
    NanScope();
    // None
    GLint arg0;
    arg0 = args[0]->Int32Value();
    GLsizei arg1;
    arg1 = args[1]->Int32Value();
    GLboolean arg2;
    arg2 = args[2]->Uint32Value();
    const GLdouble *arg3;
    GLdouble* arg3_nonconst = NULL;
    if(args[3]->IsString()) {
        NanUtf8String arg3_utf8(args[3]);
        arg3 = (const GLdouble*)(*arg3_utf8);
    } else if(args[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(args[3]);
        arg3_nonconst = new GLdouble[arg3_array->Length()];
        arg3 = (const GLdouble*)arg3_nonconst;
    } else if(args[3]->IsObject()) {
        arg3 = (const GLdouble*)glbind_get_buffer_data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLdouble*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glUniformMatrix4x3dv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getUniformdv) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLdouble *arg2;
    GLdouble* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (GLdouble*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLdouble[arg2_array->Length()];
        arg2 = (GLdouble*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (GLdouble*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLdouble*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetUniformdv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getSubroutineUniformLocation) {
    NanScope();
    // <gltypes.Type instance at 0x104edf878>
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    const GLchar *arg2;
    GLchar* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (const GLchar*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLchar[arg2_array->Length()];
        arg2 = (const GLchar*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (const GLchar*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLchar*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    GLint result;
    result = glGetSubroutineUniformLocation(arg0, arg1, arg2);
    v8::Handle<v8::Value> result_js = NanNew<v8::Int32>(result);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnValue(result_js);
}
NAN_METHOD(EXPORT_getSubroutineIndex) {
    NanScope();
    // <gltypes.Type instance at 0x104edf998>
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    const GLchar *arg2;
    GLchar* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (const GLchar*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLchar[arg2_array->Length()];
        arg2 = (const GLchar*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (const GLchar*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLchar*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    GLuint result;
    result = glGetSubroutineIndex(arg0, arg1, arg2);
    v8::Handle<v8::Value> result_js = NanNew<v8::Uint32>(result);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnValue(result_js);
}
NAN_METHOD(EXPORT_getActiveSubroutineUniformiv) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLuint arg2;
    arg2 = args[2]->Uint32Value();
    GLenum arg3;
    arg3 = args[3]->Uint32Value();
    GLint *arg4;
    GLint* arg4_nonconst = NULL;
    if(args[4]->IsString()) {
        NanUtf8String arg4_utf8(args[4]);
        arg4 = (GLint*)(*arg4_utf8);
    } else if(args[4]->IsArray()) {
        v8::Handle<v8::Array> arg4_array = v8::Handle<v8::Array>::Cast(args[4]);
        arg4_nonconst = new GLint[arg4_array->Length()];
        arg4 = (GLint*)arg4_nonconst;
    } else if(args[4]->IsObject()) {
        arg4 = (GLint*)glbind_get_buffer_data(args[4]);
    } else if(args[4]->IsNumber()) {
        arg4 = (GLint*)(args[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glGetActiveSubroutineUniformiv(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getActiveSubroutineUniformName) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLuint arg2;
    arg2 = args[2]->Uint32Value();
    GLsizei arg3;
    arg3 = args[3]->Int32Value();
    GLsizei *arg4;
    GLsizei* arg4_nonconst = NULL;
    if(args[4]->IsString()) {
        NanUtf8String arg4_utf8(args[4]);
        arg4 = (GLsizei*)(*arg4_utf8);
    } else if(args[4]->IsArray()) {
        v8::Handle<v8::Array> arg4_array = v8::Handle<v8::Array>::Cast(args[4]);
        arg4_nonconst = new GLsizei[arg4_array->Length()];
        arg4 = (GLsizei*)arg4_nonconst;
    } else if(args[4]->IsObject()) {
        arg4 = (GLsizei*)glbind_get_buffer_data(args[4]);
    } else if(args[4]->IsNumber()) {
        arg4 = (GLsizei*)(args[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    GLchar *arg5;
    GLchar* arg5_nonconst = NULL;
    if(args[5]->IsString()) {
        NanUtf8String arg5_utf8(args[5]);
        arg5 = (GLchar*)(*arg5_utf8);
    } else if(args[5]->IsArray()) {
        v8::Handle<v8::Array> arg5_array = v8::Handle<v8::Array>::Cast(args[5]);
        arg5_nonconst = new GLchar[arg5_array->Length()];
        arg5 = (GLchar*)arg5_nonconst;
    } else if(args[5]->IsObject()) {
        arg5 = (GLchar*)glbind_get_buffer_data(args[5]);
    } else if(args[5]->IsNumber()) {
        arg5 = (GLchar*)(args[5]->IntegerValue());
    } else {
        arg5 = NULL;
    }
    glGetActiveSubroutineUniformName(arg0, arg1, arg2, arg3, arg4, arg5);
    if(arg4_nonconst) delete [] arg4_nonconst;
    if(arg5_nonconst) delete [] arg5_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getActiveSubroutineName) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLuint arg2;
    arg2 = args[2]->Uint32Value();
    GLsizei arg3;
    arg3 = args[3]->Int32Value();
    GLsizei *arg4;
    GLsizei* arg4_nonconst = NULL;
    if(args[4]->IsString()) {
        NanUtf8String arg4_utf8(args[4]);
        arg4 = (GLsizei*)(*arg4_utf8);
    } else if(args[4]->IsArray()) {
        v8::Handle<v8::Array> arg4_array = v8::Handle<v8::Array>::Cast(args[4]);
        arg4_nonconst = new GLsizei[arg4_array->Length()];
        arg4 = (GLsizei*)arg4_nonconst;
    } else if(args[4]->IsObject()) {
        arg4 = (GLsizei*)glbind_get_buffer_data(args[4]);
    } else if(args[4]->IsNumber()) {
        arg4 = (GLsizei*)(args[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    GLchar *arg5;
    GLchar* arg5_nonconst = NULL;
    if(args[5]->IsString()) {
        NanUtf8String arg5_utf8(args[5]);
        arg5 = (GLchar*)(*arg5_utf8);
    } else if(args[5]->IsArray()) {
        v8::Handle<v8::Array> arg5_array = v8::Handle<v8::Array>::Cast(args[5]);
        arg5_nonconst = new GLchar[arg5_array->Length()];
        arg5 = (GLchar*)arg5_nonconst;
    } else if(args[5]->IsObject()) {
        arg5 = (GLchar*)glbind_get_buffer_data(args[5]);
    } else if(args[5]->IsNumber()) {
        arg5 = (GLchar*)(args[5]->IntegerValue());
    } else {
        arg5 = NULL;
    }
    glGetActiveSubroutineName(arg0, arg1, arg2, arg3, arg4, arg5);
    if(arg4_nonconst) delete [] arg4_nonconst;
    if(arg5_nonconst) delete [] arg5_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_uniformSubroutinesuiv) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLsizei arg1;
    arg1 = args[1]->Int32Value();
    const GLuint *arg2;
    GLuint* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (const GLuint*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLuint[arg2_array->Length()];
        arg2 = (const GLuint*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (const GLuint*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLuint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glUniformSubroutinesuiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getUniformSubroutineuiv) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLuint *arg2;
    GLuint* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (GLuint*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLuint[arg2_array->Length()];
        arg2 = (GLuint*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (GLuint*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLuint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetUniformSubroutineuiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getProgramStageiv) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLenum arg2;
    arg2 = args[2]->Uint32Value();
    GLint *arg3;
    GLint* arg3_nonconst = NULL;
    if(args[3]->IsString()) {
        NanUtf8String arg3_utf8(args[3]);
        arg3 = (GLint*)(*arg3_utf8);
    } else if(args[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(args[3]);
        arg3_nonconst = new GLint[arg3_array->Length()];
        arg3 = (GLint*)arg3_nonconst;
    } else if(args[3]->IsObject()) {
        arg3 = (GLint*)glbind_get_buffer_data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (GLint*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glGetProgramStageiv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_patchParameteri) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLint arg1;
    arg1 = args[1]->Int32Value();
    glPatchParameteri(arg0, arg1);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_patchParameterfv) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    const GLfloat *arg1;
    GLfloat* arg1_nonconst = NULL;
    if(args[1]->IsString()) {
        NanUtf8String arg1_utf8(args[1]);
        arg1 = (const GLfloat*)(*arg1_utf8);
    } else if(args[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(args[1]);
        arg1_nonconst = new GLfloat[arg1_array->Length()];
        arg1 = (const GLfloat*)arg1_nonconst;
    } else if(args[1]->IsObject()) {
        arg1 = (const GLfloat*)glbind_get_buffer_data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLfloat*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glPatchParameterfv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_bindTransformFeedback) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLuint arg1;
    if(args[1]->IsNumber()) {
        arg1 = args[1]->IntegerValue();
    } else {
        arg1 = node::ObjectWrap::Unwrap<NODE_TransformFeedback>(args[1]->ToObject())->gl_handle;
    }
    glBindTransformFeedback(arg0, arg1);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_genTransformFeedbacks) {
    NanScope();
    // None
    GLsizei arg0;
    arg0 = args[0]->Int32Value();
    GLuint* arg1;
    v8::Handle<v8::Array> args_1_array = v8::Handle<v8::Array>::Cast(args[1]);
    arg1 = new GLuint[args_1_array->Length()];
    glGenTransformFeedbacks(arg0, arg1);
    for(uint32_t i = 0; i < args_1_array->Length(); i++) {
        args_1_array->Set(i, NODE_TransformFeedback::fromGLHandle(arg1[i]));
    }
    delete [] arg1;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_isTransformFeedback) {
    NanScope();
    // <gltypes.Type instance at 0x104edf710>
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_TransformFeedback>(args[0]->ToObject())->gl_handle;
    }
    GLboolean result;
    result = glIsTransformFeedback(arg0);
    v8::Handle<v8::Value> result_js = NanNew<v8::Uint32>(result);
    NanReturnValue(result_js);
}
NAN_METHOD(EXPORT_pauseTransformFeedback) {
    NanScope();
    // None
    glPauseTransformFeedback();
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_resumeTransformFeedback) {
    NanScope();
    // None
    glResumeTransformFeedback();
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_drawTransformFeedback) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLuint arg1;
    if(args[1]->IsNumber()) {
        arg1 = args[1]->IntegerValue();
    } else {
        arg1 = node::ObjectWrap::Unwrap<NODE_TransformFeedback>(args[1]->ToObject())->gl_handle;
    }
    glDrawTransformFeedback(arg0, arg1);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_drawTransformFeedbackStream) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLuint arg1;
    arg1 = args[1]->Uint32Value();
    GLuint arg2;
    arg2 = args[2]->Uint32Value();
    glDrawTransformFeedbackStream(arg0, arg1, arg2);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_beginQueryIndexed) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLuint arg1;
    arg1 = args[1]->Uint32Value();
    GLuint arg2;
    arg2 = args[2]->Uint32Value();
    glBeginQueryIndexed(arg0, arg1, arg2);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_endQueryIndexed) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLuint arg1;
    arg1 = args[1]->Uint32Value();
    glEndQueryIndexed(arg0, arg1);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getQueryIndexediv) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLuint arg1;
    arg1 = args[1]->Uint32Value();
    GLenum arg2;
    arg2 = args[2]->Uint32Value();
    GLint *arg3;
    GLint* arg3_nonconst = NULL;
    if(args[3]->IsString()) {
        NanUtf8String arg3_utf8(args[3]);
        arg3 = (GLint*)(*arg3_utf8);
    } else if(args[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(args[3]);
        arg3_nonconst = new GLint[arg3_array->Length()];
        arg3 = (GLint*)arg3_nonconst;
    } else if(args[3]->IsObject()) {
        arg3 = (GLint*)glbind_get_buffer_data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (GLint*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glGetQueryIndexediv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_releaseShaderCompiler) {
    NanScope();
    // None
    glReleaseShaderCompiler();
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_shaderBinary) {
    NanScope();
    // None
    GLsizei arg0;
    arg0 = args[0]->Int32Value();
    GLuint* arg1;
    v8::Handle<v8::Array> args_1_array = v8::Handle<v8::Array>::Cast(args[1]);
    arg1 = new GLuint[args_1_array->Length()];
    for(uint32_t i = 0; i < args_1_array->Length(); i++) {
        arg1[i] = node::ObjectWrap::Unwrap<NODE_Shader>(args_1_array->Get(i)->ToObject())->gl_handle;}
    GLenum arg2;
    arg2 = args[2]->Uint32Value();
    const GLvoid *arg3;
    GLubyte* arg3_nonconst = NULL;
    if(args[3]->IsString()) {
        NanUtf8String arg3_utf8(args[3]);
        arg3 = (const GLvoid*)(*arg3_utf8);
    } else if(args[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(args[3]);
        arg3_nonconst = new GLubyte[arg3_array->Length()];
        arg3 = (const GLvoid*)arg3_nonconst;
    } else if(args[3]->IsObject()) {
        arg3 = (const GLvoid*)glbind_get_buffer_data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLvoid*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    GLsizei arg4;
    arg4 = args[4]->Int32Value();
    glShaderBinary(arg0, arg1, arg2, arg3, arg4);
    delete [] arg1;
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getShaderPrecisionFormat) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLint *arg2;
    GLint* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (GLint*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLint[arg2_array->Length()];
        arg2 = (GLint*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (GLint*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    GLint *arg3;
    GLint* arg3_nonconst = NULL;
    if(args[3]->IsString()) {
        NanUtf8String arg3_utf8(args[3]);
        arg3 = (GLint*)(*arg3_utf8);
    } else if(args[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(args[3]);
        arg3_nonconst = new GLint[arg3_array->Length()];
        arg3 = (GLint*)arg3_nonconst;
    } else if(args[3]->IsObject()) {
        arg3 = (GLint*)glbind_get_buffer_data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (GLint*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glGetShaderPrecisionFormat(arg0, arg1, arg2, arg3);
    if(arg2_nonconst) delete [] arg2_nonconst;
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_depthRangef) {
    NanScope();
    // None
    GLclampf arg0;
    arg0 = (float)args[0]->NumberValue();
    GLclampf arg1;
    arg1 = (float)args[1]->NumberValue();
    glDepthRangef(arg0, arg1);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_clearDepthf) {
    NanScope();
    // None
    GLclampf arg0;
    arg0 = (float)args[0]->NumberValue();
    glClearDepthf(arg0);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getProgramBinary) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLsizei arg1;
    arg1 = args[1]->Int32Value();
    GLsizei *arg2;
    GLsizei* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (GLsizei*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLsizei[arg2_array->Length()];
        arg2 = (GLsizei*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (GLsizei*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLsizei*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    GLenum *arg3;
    GLenum* arg3_nonconst = NULL;
    if(args[3]->IsString()) {
        NanUtf8String arg3_utf8(args[3]);
        arg3 = (GLenum*)(*arg3_utf8);
    } else if(args[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(args[3]);
        arg3_nonconst = new GLenum[arg3_array->Length()];
        arg3 = (GLenum*)arg3_nonconst;
    } else if(args[3]->IsObject()) {
        arg3 = (GLenum*)glbind_get_buffer_data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (GLenum*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    GLvoid *arg4;
    GLubyte* arg4_nonconst = NULL;
    if(args[4]->IsString()) {
        NanUtf8String arg4_utf8(args[4]);
        arg4 = (GLvoid*)(*arg4_utf8);
    } else if(args[4]->IsArray()) {
        v8::Handle<v8::Array> arg4_array = v8::Handle<v8::Array>::Cast(args[4]);
        arg4_nonconst = new GLubyte[arg4_array->Length()];
        arg4 = (GLvoid*)arg4_nonconst;
    } else if(args[4]->IsObject()) {
        arg4 = (GLvoid*)glbind_get_buffer_data(args[4]);
    } else if(args[4]->IsNumber()) {
        arg4 = (GLvoid*)(args[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glGetProgramBinary(arg0, arg1, arg2, arg3, arg4);
    if(arg2_nonconst) delete [] arg2_nonconst;
    if(arg3_nonconst) delete [] arg3_nonconst;
    if(arg4_nonconst) delete [] arg4_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_programBinary) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    const GLvoid *arg2;
    GLubyte* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (const GLvoid*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLubyte[arg2_array->Length()];
        arg2 = (const GLvoid*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (const GLvoid*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLvoid*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    GLsizei arg3;
    arg3 = args[3]->Int32Value();
    glProgramBinary(arg0, arg1, arg2, arg3);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_programParameteri) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLint arg2;
    arg2 = args[2]->Int32Value();
    glProgramParameteri(arg0, arg1, arg2);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_useProgramStages) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_ProgramPipeline>(args[0]->ToObject())->gl_handle;
    }
    GLbitfield arg1;
    arg1 = args[1]->Uint32Value();
    GLuint arg2;
    if(args[2]->IsNumber()) {
        arg2 = args[2]->IntegerValue();
    } else {
        arg2 = node::ObjectWrap::Unwrap<NODE_Program>(args[2]->ToObject())->gl_handle;
    }
    glUseProgramStages(arg0, arg1, arg2);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_activeShaderProgram) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_ProgramPipeline>(args[0]->ToObject())->gl_handle;
    }
    GLuint arg1;
    if(args[1]->IsNumber()) {
        arg1 = args[1]->IntegerValue();
    } else {
        arg1 = node::ObjectWrap::Unwrap<NODE_Program>(args[1]->ToObject())->gl_handle;
    }
    glActiveShaderProgram(arg0, arg1);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_bindProgramPipeline) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_ProgramPipeline>(args[0]->ToObject())->gl_handle;
    }
    glBindProgramPipeline(arg0);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_genProgramPipelines) {
    NanScope();
    // None
    GLsizei arg0;
    arg0 = args[0]->Int32Value();
    GLuint* arg1;
    v8::Handle<v8::Array> args_1_array = v8::Handle<v8::Array>::Cast(args[1]);
    arg1 = new GLuint[args_1_array->Length()];
    glGenProgramPipelines(arg0, arg1);
    for(uint32_t i = 0; i < args_1_array->Length(); i++) {
        args_1_array->Set(i, NODE_ProgramPipeline::fromGLHandle(arg1[i]));
    }
    delete [] arg1;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_isProgramPipeline) {
    NanScope();
    // <gltypes.Type instance at 0x104edf710>
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_ProgramPipeline>(args[0]->ToObject())->gl_handle;
    }
    GLboolean result;
    result = glIsProgramPipeline(arg0);
    v8::Handle<v8::Value> result_js = NanNew<v8::Uint32>(result);
    NanReturnValue(result_js);
}
NAN_METHOD(EXPORT_getProgramPipelineiv) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_ProgramPipeline>(args[0]->ToObject())->gl_handle;
    }
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLint *arg2;
    GLint* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (GLint*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLint[arg2_array->Length()];
        arg2 = (GLint*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (GLint*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetProgramPipelineiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_programUniform1i) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLint arg2;
    arg2 = args[2]->Int32Value();
    glProgramUniform1i(arg0, arg1, arg2);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_programUniform1iv) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLsizei arg2;
    arg2 = args[2]->Int32Value();
    const GLint *arg3;
    GLint* arg3_nonconst = NULL;
    if(args[3]->IsString()) {
        NanUtf8String arg3_utf8(args[3]);
        arg3 = (const GLint*)(*arg3_utf8);
    } else if(args[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(args[3]);
        arg3_nonconst = new GLint[arg3_array->Length()];
        arg3 = (const GLint*)arg3_nonconst;
    } else if(args[3]->IsObject()) {
        arg3 = (const GLint*)glbind_get_buffer_data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLint*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glProgramUniform1iv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_programUniform1f) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLfloat arg2;
    arg2 = (float)args[2]->NumberValue();
    glProgramUniform1f(arg0, arg1, arg2);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_programUniform1fv) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLsizei arg2;
    arg2 = args[2]->Int32Value();
    const GLfloat *arg3;
    GLfloat* arg3_nonconst = NULL;
    if(args[3]->IsString()) {
        NanUtf8String arg3_utf8(args[3]);
        arg3 = (const GLfloat*)(*arg3_utf8);
    } else if(args[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(args[3]);
        arg3_nonconst = new GLfloat[arg3_array->Length()];
        arg3 = (const GLfloat*)arg3_nonconst;
    } else if(args[3]->IsObject()) {
        arg3 = (const GLfloat*)glbind_get_buffer_data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLfloat*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glProgramUniform1fv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_programUniform1d) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLdouble arg2;
    arg2 = args[2]->NumberValue();
    glProgramUniform1d(arg0, arg1, arg2);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_programUniform1dv) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLsizei arg2;
    arg2 = args[2]->Int32Value();
    const GLdouble *arg3;
    GLdouble* arg3_nonconst = NULL;
    if(args[3]->IsString()) {
        NanUtf8String arg3_utf8(args[3]);
        arg3 = (const GLdouble*)(*arg3_utf8);
    } else if(args[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(args[3]);
        arg3_nonconst = new GLdouble[arg3_array->Length()];
        arg3 = (const GLdouble*)arg3_nonconst;
    } else if(args[3]->IsObject()) {
        arg3 = (const GLdouble*)glbind_get_buffer_data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLdouble*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glProgramUniform1dv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_programUniform1ui) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLuint arg2;
    arg2 = args[2]->Uint32Value();
    glProgramUniform1ui(arg0, arg1, arg2);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_programUniform1uiv) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLsizei arg2;
    arg2 = args[2]->Int32Value();
    const GLuint *arg3;
    GLuint* arg3_nonconst = NULL;
    if(args[3]->IsString()) {
        NanUtf8String arg3_utf8(args[3]);
        arg3 = (const GLuint*)(*arg3_utf8);
    } else if(args[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(args[3]);
        arg3_nonconst = new GLuint[arg3_array->Length()];
        arg3 = (const GLuint*)arg3_nonconst;
    } else if(args[3]->IsObject()) {
        arg3 = (const GLuint*)glbind_get_buffer_data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLuint*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glProgramUniform1uiv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_programUniform2i) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLint arg2;
    arg2 = args[2]->Int32Value();
    GLint arg3;
    arg3 = args[3]->Int32Value();
    glProgramUniform2i(arg0, arg1, arg2, arg3);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_programUniform2iv) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLsizei arg2;
    arg2 = args[2]->Int32Value();
    const GLint *arg3;
    GLint* arg3_nonconst = NULL;
    if(args[3]->IsString()) {
        NanUtf8String arg3_utf8(args[3]);
        arg3 = (const GLint*)(*arg3_utf8);
    } else if(args[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(args[3]);
        arg3_nonconst = new GLint[arg3_array->Length()];
        arg3 = (const GLint*)arg3_nonconst;
    } else if(args[3]->IsObject()) {
        arg3 = (const GLint*)glbind_get_buffer_data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLint*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glProgramUniform2iv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_programUniform2f) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLfloat arg2;
    arg2 = (float)args[2]->NumberValue();
    GLfloat arg3;
    arg3 = (float)args[3]->NumberValue();
    glProgramUniform2f(arg0, arg1, arg2, arg3);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_programUniform2fv) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLsizei arg2;
    arg2 = args[2]->Int32Value();
    const GLfloat *arg3;
    GLfloat* arg3_nonconst = NULL;
    if(args[3]->IsString()) {
        NanUtf8String arg3_utf8(args[3]);
        arg3 = (const GLfloat*)(*arg3_utf8);
    } else if(args[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(args[3]);
        arg3_nonconst = new GLfloat[arg3_array->Length()];
        arg3 = (const GLfloat*)arg3_nonconst;
    } else if(args[3]->IsObject()) {
        arg3 = (const GLfloat*)glbind_get_buffer_data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLfloat*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glProgramUniform2fv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_programUniform2d) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLdouble arg2;
    arg2 = args[2]->NumberValue();
    GLdouble arg3;
    arg3 = args[3]->NumberValue();
    glProgramUniform2d(arg0, arg1, arg2, arg3);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_programUniform2dv) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLsizei arg2;
    arg2 = args[2]->Int32Value();
    const GLdouble *arg3;
    GLdouble* arg3_nonconst = NULL;
    if(args[3]->IsString()) {
        NanUtf8String arg3_utf8(args[3]);
        arg3 = (const GLdouble*)(*arg3_utf8);
    } else if(args[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(args[3]);
        arg3_nonconst = new GLdouble[arg3_array->Length()];
        arg3 = (const GLdouble*)arg3_nonconst;
    } else if(args[3]->IsObject()) {
        arg3 = (const GLdouble*)glbind_get_buffer_data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLdouble*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glProgramUniform2dv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_programUniform2ui) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLuint arg2;
    arg2 = args[2]->Uint32Value();
    GLuint arg3;
    arg3 = args[3]->Uint32Value();
    glProgramUniform2ui(arg0, arg1, arg2, arg3);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_programUniform2uiv) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLsizei arg2;
    arg2 = args[2]->Int32Value();
    const GLuint *arg3;
    GLuint* arg3_nonconst = NULL;
    if(args[3]->IsString()) {
        NanUtf8String arg3_utf8(args[3]);
        arg3 = (const GLuint*)(*arg3_utf8);
    } else if(args[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(args[3]);
        arg3_nonconst = new GLuint[arg3_array->Length()];
        arg3 = (const GLuint*)arg3_nonconst;
    } else if(args[3]->IsObject()) {
        arg3 = (const GLuint*)glbind_get_buffer_data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLuint*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glProgramUniform2uiv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_programUniform3i) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLint arg2;
    arg2 = args[2]->Int32Value();
    GLint arg3;
    arg3 = args[3]->Int32Value();
    GLint arg4;
    arg4 = args[4]->Int32Value();
    glProgramUniform3i(arg0, arg1, arg2, arg3, arg4);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_programUniform3iv) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLsizei arg2;
    arg2 = args[2]->Int32Value();
    const GLint *arg3;
    GLint* arg3_nonconst = NULL;
    if(args[3]->IsString()) {
        NanUtf8String arg3_utf8(args[3]);
        arg3 = (const GLint*)(*arg3_utf8);
    } else if(args[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(args[3]);
        arg3_nonconst = new GLint[arg3_array->Length()];
        arg3 = (const GLint*)arg3_nonconst;
    } else if(args[3]->IsObject()) {
        arg3 = (const GLint*)glbind_get_buffer_data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLint*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glProgramUniform3iv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_programUniform3f) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLfloat arg2;
    arg2 = (float)args[2]->NumberValue();
    GLfloat arg3;
    arg3 = (float)args[3]->NumberValue();
    GLfloat arg4;
    arg4 = (float)args[4]->NumberValue();
    glProgramUniform3f(arg0, arg1, arg2, arg3, arg4);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_programUniform3fv) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLsizei arg2;
    arg2 = args[2]->Int32Value();
    const GLfloat *arg3;
    GLfloat* arg3_nonconst = NULL;
    if(args[3]->IsString()) {
        NanUtf8String arg3_utf8(args[3]);
        arg3 = (const GLfloat*)(*arg3_utf8);
    } else if(args[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(args[3]);
        arg3_nonconst = new GLfloat[arg3_array->Length()];
        arg3 = (const GLfloat*)arg3_nonconst;
    } else if(args[3]->IsObject()) {
        arg3 = (const GLfloat*)glbind_get_buffer_data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLfloat*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glProgramUniform3fv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_programUniform3d) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLdouble arg2;
    arg2 = args[2]->NumberValue();
    GLdouble arg3;
    arg3 = args[3]->NumberValue();
    GLdouble arg4;
    arg4 = args[4]->NumberValue();
    glProgramUniform3d(arg0, arg1, arg2, arg3, arg4);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_programUniform3dv) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLsizei arg2;
    arg2 = args[2]->Int32Value();
    const GLdouble *arg3;
    GLdouble* arg3_nonconst = NULL;
    if(args[3]->IsString()) {
        NanUtf8String arg3_utf8(args[3]);
        arg3 = (const GLdouble*)(*arg3_utf8);
    } else if(args[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(args[3]);
        arg3_nonconst = new GLdouble[arg3_array->Length()];
        arg3 = (const GLdouble*)arg3_nonconst;
    } else if(args[3]->IsObject()) {
        arg3 = (const GLdouble*)glbind_get_buffer_data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLdouble*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glProgramUniform3dv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_programUniform3ui) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLuint arg2;
    arg2 = args[2]->Uint32Value();
    GLuint arg3;
    arg3 = args[3]->Uint32Value();
    GLuint arg4;
    arg4 = args[4]->Uint32Value();
    glProgramUniform3ui(arg0, arg1, arg2, arg3, arg4);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_programUniform3uiv) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLsizei arg2;
    arg2 = args[2]->Int32Value();
    const GLuint *arg3;
    GLuint* arg3_nonconst = NULL;
    if(args[3]->IsString()) {
        NanUtf8String arg3_utf8(args[3]);
        arg3 = (const GLuint*)(*arg3_utf8);
    } else if(args[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(args[3]);
        arg3_nonconst = new GLuint[arg3_array->Length()];
        arg3 = (const GLuint*)arg3_nonconst;
    } else if(args[3]->IsObject()) {
        arg3 = (const GLuint*)glbind_get_buffer_data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLuint*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glProgramUniform3uiv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_programUniform4i) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLint arg2;
    arg2 = args[2]->Int32Value();
    GLint arg3;
    arg3 = args[3]->Int32Value();
    GLint arg4;
    arg4 = args[4]->Int32Value();
    GLint arg5;
    arg5 = args[5]->Int32Value();
    glProgramUniform4i(arg0, arg1, arg2, arg3, arg4, arg5);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_programUniform4iv) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLsizei arg2;
    arg2 = args[2]->Int32Value();
    const GLint *arg3;
    GLint* arg3_nonconst = NULL;
    if(args[3]->IsString()) {
        NanUtf8String arg3_utf8(args[3]);
        arg3 = (const GLint*)(*arg3_utf8);
    } else if(args[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(args[3]);
        arg3_nonconst = new GLint[arg3_array->Length()];
        arg3 = (const GLint*)arg3_nonconst;
    } else if(args[3]->IsObject()) {
        arg3 = (const GLint*)glbind_get_buffer_data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLint*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glProgramUniform4iv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_programUniform4f) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLfloat arg2;
    arg2 = (float)args[2]->NumberValue();
    GLfloat arg3;
    arg3 = (float)args[3]->NumberValue();
    GLfloat arg4;
    arg4 = (float)args[4]->NumberValue();
    GLfloat arg5;
    arg5 = (float)args[5]->NumberValue();
    glProgramUniform4f(arg0, arg1, arg2, arg3, arg4, arg5);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_programUniform4fv) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLsizei arg2;
    arg2 = args[2]->Int32Value();
    const GLfloat *arg3;
    GLfloat* arg3_nonconst = NULL;
    if(args[3]->IsString()) {
        NanUtf8String arg3_utf8(args[3]);
        arg3 = (const GLfloat*)(*arg3_utf8);
    } else if(args[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(args[3]);
        arg3_nonconst = new GLfloat[arg3_array->Length()];
        arg3 = (const GLfloat*)arg3_nonconst;
    } else if(args[3]->IsObject()) {
        arg3 = (const GLfloat*)glbind_get_buffer_data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLfloat*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glProgramUniform4fv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_programUniform4d) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLdouble arg2;
    arg2 = args[2]->NumberValue();
    GLdouble arg3;
    arg3 = args[3]->NumberValue();
    GLdouble arg4;
    arg4 = args[4]->NumberValue();
    GLdouble arg5;
    arg5 = args[5]->NumberValue();
    glProgramUniform4d(arg0, arg1, arg2, arg3, arg4, arg5);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_programUniform4dv) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLsizei arg2;
    arg2 = args[2]->Int32Value();
    const GLdouble *arg3;
    GLdouble* arg3_nonconst = NULL;
    if(args[3]->IsString()) {
        NanUtf8String arg3_utf8(args[3]);
        arg3 = (const GLdouble*)(*arg3_utf8);
    } else if(args[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(args[3]);
        arg3_nonconst = new GLdouble[arg3_array->Length()];
        arg3 = (const GLdouble*)arg3_nonconst;
    } else if(args[3]->IsObject()) {
        arg3 = (const GLdouble*)glbind_get_buffer_data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLdouble*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glProgramUniform4dv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_programUniform4ui) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLuint arg2;
    arg2 = args[2]->Uint32Value();
    GLuint arg3;
    arg3 = args[3]->Uint32Value();
    GLuint arg4;
    arg4 = args[4]->Uint32Value();
    GLuint arg5;
    arg5 = args[5]->Uint32Value();
    glProgramUniform4ui(arg0, arg1, arg2, arg3, arg4, arg5);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_programUniform4uiv) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLsizei arg2;
    arg2 = args[2]->Int32Value();
    const GLuint *arg3;
    GLuint* arg3_nonconst = NULL;
    if(args[3]->IsString()) {
        NanUtf8String arg3_utf8(args[3]);
        arg3 = (const GLuint*)(*arg3_utf8);
    } else if(args[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(args[3]);
        arg3_nonconst = new GLuint[arg3_array->Length()];
        arg3 = (const GLuint*)arg3_nonconst;
    } else if(args[3]->IsObject()) {
        arg3 = (const GLuint*)glbind_get_buffer_data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLuint*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glProgramUniform4uiv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_programUniformMatrix2fv) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLsizei arg2;
    arg2 = args[2]->Int32Value();
    GLboolean arg3;
    arg3 = args[3]->Uint32Value();
    const GLfloat *arg4;
    GLfloat* arg4_nonconst = NULL;
    if(args[4]->IsString()) {
        NanUtf8String arg4_utf8(args[4]);
        arg4 = (const GLfloat*)(*arg4_utf8);
    } else if(args[4]->IsArray()) {
        v8::Handle<v8::Array> arg4_array = v8::Handle<v8::Array>::Cast(args[4]);
        arg4_nonconst = new GLfloat[arg4_array->Length()];
        arg4 = (const GLfloat*)arg4_nonconst;
    } else if(args[4]->IsObject()) {
        arg4 = (const GLfloat*)glbind_get_buffer_data(args[4]);
    } else if(args[4]->IsNumber()) {
        arg4 = (const GLfloat*)(args[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glProgramUniformMatrix2fv(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_programUniformMatrix3fv) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLsizei arg2;
    arg2 = args[2]->Int32Value();
    GLboolean arg3;
    arg3 = args[3]->Uint32Value();
    const GLfloat *arg4;
    GLfloat* arg4_nonconst = NULL;
    if(args[4]->IsString()) {
        NanUtf8String arg4_utf8(args[4]);
        arg4 = (const GLfloat*)(*arg4_utf8);
    } else if(args[4]->IsArray()) {
        v8::Handle<v8::Array> arg4_array = v8::Handle<v8::Array>::Cast(args[4]);
        arg4_nonconst = new GLfloat[arg4_array->Length()];
        arg4 = (const GLfloat*)arg4_nonconst;
    } else if(args[4]->IsObject()) {
        arg4 = (const GLfloat*)glbind_get_buffer_data(args[4]);
    } else if(args[4]->IsNumber()) {
        arg4 = (const GLfloat*)(args[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glProgramUniformMatrix3fv(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_programUniformMatrix4fv) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLsizei arg2;
    arg2 = args[2]->Int32Value();
    GLboolean arg3;
    arg3 = args[3]->Uint32Value();
    const GLfloat *arg4;
    GLfloat* arg4_nonconst = NULL;
    if(args[4]->IsString()) {
        NanUtf8String arg4_utf8(args[4]);
        arg4 = (const GLfloat*)(*arg4_utf8);
    } else if(args[4]->IsArray()) {
        v8::Handle<v8::Array> arg4_array = v8::Handle<v8::Array>::Cast(args[4]);
        arg4_nonconst = new GLfloat[arg4_array->Length()];
        arg4 = (const GLfloat*)arg4_nonconst;
    } else if(args[4]->IsObject()) {
        arg4 = (const GLfloat*)glbind_get_buffer_data(args[4]);
    } else if(args[4]->IsNumber()) {
        arg4 = (const GLfloat*)(args[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glProgramUniformMatrix4fv(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_programUniformMatrix2dv) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLsizei arg2;
    arg2 = args[2]->Int32Value();
    GLboolean arg3;
    arg3 = args[3]->Uint32Value();
    const GLdouble *arg4;
    GLdouble* arg4_nonconst = NULL;
    if(args[4]->IsString()) {
        NanUtf8String arg4_utf8(args[4]);
        arg4 = (const GLdouble*)(*arg4_utf8);
    } else if(args[4]->IsArray()) {
        v8::Handle<v8::Array> arg4_array = v8::Handle<v8::Array>::Cast(args[4]);
        arg4_nonconst = new GLdouble[arg4_array->Length()];
        arg4 = (const GLdouble*)arg4_nonconst;
    } else if(args[4]->IsObject()) {
        arg4 = (const GLdouble*)glbind_get_buffer_data(args[4]);
    } else if(args[4]->IsNumber()) {
        arg4 = (const GLdouble*)(args[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glProgramUniformMatrix2dv(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_programUniformMatrix3dv) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLsizei arg2;
    arg2 = args[2]->Int32Value();
    GLboolean arg3;
    arg3 = args[3]->Uint32Value();
    const GLdouble *arg4;
    GLdouble* arg4_nonconst = NULL;
    if(args[4]->IsString()) {
        NanUtf8String arg4_utf8(args[4]);
        arg4 = (const GLdouble*)(*arg4_utf8);
    } else if(args[4]->IsArray()) {
        v8::Handle<v8::Array> arg4_array = v8::Handle<v8::Array>::Cast(args[4]);
        arg4_nonconst = new GLdouble[arg4_array->Length()];
        arg4 = (const GLdouble*)arg4_nonconst;
    } else if(args[4]->IsObject()) {
        arg4 = (const GLdouble*)glbind_get_buffer_data(args[4]);
    } else if(args[4]->IsNumber()) {
        arg4 = (const GLdouble*)(args[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glProgramUniformMatrix3dv(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_programUniformMatrix4dv) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLsizei arg2;
    arg2 = args[2]->Int32Value();
    GLboolean arg3;
    arg3 = args[3]->Uint32Value();
    const GLdouble *arg4;
    GLdouble* arg4_nonconst = NULL;
    if(args[4]->IsString()) {
        NanUtf8String arg4_utf8(args[4]);
        arg4 = (const GLdouble*)(*arg4_utf8);
    } else if(args[4]->IsArray()) {
        v8::Handle<v8::Array> arg4_array = v8::Handle<v8::Array>::Cast(args[4]);
        arg4_nonconst = new GLdouble[arg4_array->Length()];
        arg4 = (const GLdouble*)arg4_nonconst;
    } else if(args[4]->IsObject()) {
        arg4 = (const GLdouble*)glbind_get_buffer_data(args[4]);
    } else if(args[4]->IsNumber()) {
        arg4 = (const GLdouble*)(args[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glProgramUniformMatrix4dv(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_programUniformMatrix2x3fv) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLsizei arg2;
    arg2 = args[2]->Int32Value();
    GLboolean arg3;
    arg3 = args[3]->Uint32Value();
    const GLfloat *arg4;
    GLfloat* arg4_nonconst = NULL;
    if(args[4]->IsString()) {
        NanUtf8String arg4_utf8(args[4]);
        arg4 = (const GLfloat*)(*arg4_utf8);
    } else if(args[4]->IsArray()) {
        v8::Handle<v8::Array> arg4_array = v8::Handle<v8::Array>::Cast(args[4]);
        arg4_nonconst = new GLfloat[arg4_array->Length()];
        arg4 = (const GLfloat*)arg4_nonconst;
    } else if(args[4]->IsObject()) {
        arg4 = (const GLfloat*)glbind_get_buffer_data(args[4]);
    } else if(args[4]->IsNumber()) {
        arg4 = (const GLfloat*)(args[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glProgramUniformMatrix2x3fv(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_programUniformMatrix3x2fv) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLsizei arg2;
    arg2 = args[2]->Int32Value();
    GLboolean arg3;
    arg3 = args[3]->Uint32Value();
    const GLfloat *arg4;
    GLfloat* arg4_nonconst = NULL;
    if(args[4]->IsString()) {
        NanUtf8String arg4_utf8(args[4]);
        arg4 = (const GLfloat*)(*arg4_utf8);
    } else if(args[4]->IsArray()) {
        v8::Handle<v8::Array> arg4_array = v8::Handle<v8::Array>::Cast(args[4]);
        arg4_nonconst = new GLfloat[arg4_array->Length()];
        arg4 = (const GLfloat*)arg4_nonconst;
    } else if(args[4]->IsObject()) {
        arg4 = (const GLfloat*)glbind_get_buffer_data(args[4]);
    } else if(args[4]->IsNumber()) {
        arg4 = (const GLfloat*)(args[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glProgramUniformMatrix3x2fv(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_programUniformMatrix2x4fv) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLsizei arg2;
    arg2 = args[2]->Int32Value();
    GLboolean arg3;
    arg3 = args[3]->Uint32Value();
    const GLfloat *arg4;
    GLfloat* arg4_nonconst = NULL;
    if(args[4]->IsString()) {
        NanUtf8String arg4_utf8(args[4]);
        arg4 = (const GLfloat*)(*arg4_utf8);
    } else if(args[4]->IsArray()) {
        v8::Handle<v8::Array> arg4_array = v8::Handle<v8::Array>::Cast(args[4]);
        arg4_nonconst = new GLfloat[arg4_array->Length()];
        arg4 = (const GLfloat*)arg4_nonconst;
    } else if(args[4]->IsObject()) {
        arg4 = (const GLfloat*)glbind_get_buffer_data(args[4]);
    } else if(args[4]->IsNumber()) {
        arg4 = (const GLfloat*)(args[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glProgramUniformMatrix2x4fv(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_programUniformMatrix4x2fv) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLsizei arg2;
    arg2 = args[2]->Int32Value();
    GLboolean arg3;
    arg3 = args[3]->Uint32Value();
    const GLfloat *arg4;
    GLfloat* arg4_nonconst = NULL;
    if(args[4]->IsString()) {
        NanUtf8String arg4_utf8(args[4]);
        arg4 = (const GLfloat*)(*arg4_utf8);
    } else if(args[4]->IsArray()) {
        v8::Handle<v8::Array> arg4_array = v8::Handle<v8::Array>::Cast(args[4]);
        arg4_nonconst = new GLfloat[arg4_array->Length()];
        arg4 = (const GLfloat*)arg4_nonconst;
    } else if(args[4]->IsObject()) {
        arg4 = (const GLfloat*)glbind_get_buffer_data(args[4]);
    } else if(args[4]->IsNumber()) {
        arg4 = (const GLfloat*)(args[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glProgramUniformMatrix4x2fv(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_programUniformMatrix3x4fv) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLsizei arg2;
    arg2 = args[2]->Int32Value();
    GLboolean arg3;
    arg3 = args[3]->Uint32Value();
    const GLfloat *arg4;
    GLfloat* arg4_nonconst = NULL;
    if(args[4]->IsString()) {
        NanUtf8String arg4_utf8(args[4]);
        arg4 = (const GLfloat*)(*arg4_utf8);
    } else if(args[4]->IsArray()) {
        v8::Handle<v8::Array> arg4_array = v8::Handle<v8::Array>::Cast(args[4]);
        arg4_nonconst = new GLfloat[arg4_array->Length()];
        arg4 = (const GLfloat*)arg4_nonconst;
    } else if(args[4]->IsObject()) {
        arg4 = (const GLfloat*)glbind_get_buffer_data(args[4]);
    } else if(args[4]->IsNumber()) {
        arg4 = (const GLfloat*)(args[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glProgramUniformMatrix3x4fv(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_programUniformMatrix4x3fv) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLsizei arg2;
    arg2 = args[2]->Int32Value();
    GLboolean arg3;
    arg3 = args[3]->Uint32Value();
    const GLfloat *arg4;
    GLfloat* arg4_nonconst = NULL;
    if(args[4]->IsString()) {
        NanUtf8String arg4_utf8(args[4]);
        arg4 = (const GLfloat*)(*arg4_utf8);
    } else if(args[4]->IsArray()) {
        v8::Handle<v8::Array> arg4_array = v8::Handle<v8::Array>::Cast(args[4]);
        arg4_nonconst = new GLfloat[arg4_array->Length()];
        arg4 = (const GLfloat*)arg4_nonconst;
    } else if(args[4]->IsObject()) {
        arg4 = (const GLfloat*)glbind_get_buffer_data(args[4]);
    } else if(args[4]->IsNumber()) {
        arg4 = (const GLfloat*)(args[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glProgramUniformMatrix4x3fv(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_programUniformMatrix2x3dv) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLsizei arg2;
    arg2 = args[2]->Int32Value();
    GLboolean arg3;
    arg3 = args[3]->Uint32Value();
    const GLdouble *arg4;
    GLdouble* arg4_nonconst = NULL;
    if(args[4]->IsString()) {
        NanUtf8String arg4_utf8(args[4]);
        arg4 = (const GLdouble*)(*arg4_utf8);
    } else if(args[4]->IsArray()) {
        v8::Handle<v8::Array> arg4_array = v8::Handle<v8::Array>::Cast(args[4]);
        arg4_nonconst = new GLdouble[arg4_array->Length()];
        arg4 = (const GLdouble*)arg4_nonconst;
    } else if(args[4]->IsObject()) {
        arg4 = (const GLdouble*)glbind_get_buffer_data(args[4]);
    } else if(args[4]->IsNumber()) {
        arg4 = (const GLdouble*)(args[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glProgramUniformMatrix2x3dv(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_programUniformMatrix3x2dv) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLsizei arg2;
    arg2 = args[2]->Int32Value();
    GLboolean arg3;
    arg3 = args[3]->Uint32Value();
    const GLdouble *arg4;
    GLdouble* arg4_nonconst = NULL;
    if(args[4]->IsString()) {
        NanUtf8String arg4_utf8(args[4]);
        arg4 = (const GLdouble*)(*arg4_utf8);
    } else if(args[4]->IsArray()) {
        v8::Handle<v8::Array> arg4_array = v8::Handle<v8::Array>::Cast(args[4]);
        arg4_nonconst = new GLdouble[arg4_array->Length()];
        arg4 = (const GLdouble*)arg4_nonconst;
    } else if(args[4]->IsObject()) {
        arg4 = (const GLdouble*)glbind_get_buffer_data(args[4]);
    } else if(args[4]->IsNumber()) {
        arg4 = (const GLdouble*)(args[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glProgramUniformMatrix3x2dv(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_programUniformMatrix2x4dv) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLsizei arg2;
    arg2 = args[2]->Int32Value();
    GLboolean arg3;
    arg3 = args[3]->Uint32Value();
    const GLdouble *arg4;
    GLdouble* arg4_nonconst = NULL;
    if(args[4]->IsString()) {
        NanUtf8String arg4_utf8(args[4]);
        arg4 = (const GLdouble*)(*arg4_utf8);
    } else if(args[4]->IsArray()) {
        v8::Handle<v8::Array> arg4_array = v8::Handle<v8::Array>::Cast(args[4]);
        arg4_nonconst = new GLdouble[arg4_array->Length()];
        arg4 = (const GLdouble*)arg4_nonconst;
    } else if(args[4]->IsObject()) {
        arg4 = (const GLdouble*)glbind_get_buffer_data(args[4]);
    } else if(args[4]->IsNumber()) {
        arg4 = (const GLdouble*)(args[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glProgramUniformMatrix2x4dv(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_programUniformMatrix4x2dv) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLsizei arg2;
    arg2 = args[2]->Int32Value();
    GLboolean arg3;
    arg3 = args[3]->Uint32Value();
    const GLdouble *arg4;
    GLdouble* arg4_nonconst = NULL;
    if(args[4]->IsString()) {
        NanUtf8String arg4_utf8(args[4]);
        arg4 = (const GLdouble*)(*arg4_utf8);
    } else if(args[4]->IsArray()) {
        v8::Handle<v8::Array> arg4_array = v8::Handle<v8::Array>::Cast(args[4]);
        arg4_nonconst = new GLdouble[arg4_array->Length()];
        arg4 = (const GLdouble*)arg4_nonconst;
    } else if(args[4]->IsObject()) {
        arg4 = (const GLdouble*)glbind_get_buffer_data(args[4]);
    } else if(args[4]->IsNumber()) {
        arg4 = (const GLdouble*)(args[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glProgramUniformMatrix4x2dv(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_programUniformMatrix3x4dv) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLsizei arg2;
    arg2 = args[2]->Int32Value();
    GLboolean arg3;
    arg3 = args[3]->Uint32Value();
    const GLdouble *arg4;
    GLdouble* arg4_nonconst = NULL;
    if(args[4]->IsString()) {
        NanUtf8String arg4_utf8(args[4]);
        arg4 = (const GLdouble*)(*arg4_utf8);
    } else if(args[4]->IsArray()) {
        v8::Handle<v8::Array> arg4_array = v8::Handle<v8::Array>::Cast(args[4]);
        arg4_nonconst = new GLdouble[arg4_array->Length()];
        arg4 = (const GLdouble*)arg4_nonconst;
    } else if(args[4]->IsObject()) {
        arg4 = (const GLdouble*)glbind_get_buffer_data(args[4]);
    } else if(args[4]->IsNumber()) {
        arg4 = (const GLdouble*)(args[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glProgramUniformMatrix3x4dv(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_programUniformMatrix4x3dv) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    }
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLsizei arg2;
    arg2 = args[2]->Int32Value();
    GLboolean arg3;
    arg3 = args[3]->Uint32Value();
    const GLdouble *arg4;
    GLdouble* arg4_nonconst = NULL;
    if(args[4]->IsString()) {
        NanUtf8String arg4_utf8(args[4]);
        arg4 = (const GLdouble*)(*arg4_utf8);
    } else if(args[4]->IsArray()) {
        v8::Handle<v8::Array> arg4_array = v8::Handle<v8::Array>::Cast(args[4]);
        arg4_nonconst = new GLdouble[arg4_array->Length()];
        arg4 = (const GLdouble*)arg4_nonconst;
    } else if(args[4]->IsObject()) {
        arg4 = (const GLdouble*)glbind_get_buffer_data(args[4]);
    } else if(args[4]->IsNumber()) {
        arg4 = (const GLdouble*)(args[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glProgramUniformMatrix4x3dv(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_validateProgramPipeline) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_ProgramPipeline>(args[0]->ToObject())->gl_handle;
    }
    glValidateProgramPipeline(arg0);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getProgramPipelineInfoLog) {
    NanScope();
    // None
    GLuint arg0;
    if(args[0]->IsNumber()) {
        arg0 = args[0]->IntegerValue();
    } else {
        arg0 = node::ObjectWrap::Unwrap<NODE_ProgramPipeline>(args[0]->ToObject())->gl_handle;
    }
    GLsizei arg1;
    arg1 = args[1]->Int32Value();
    GLsizei *arg2;
    GLsizei* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (GLsizei*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLsizei[arg2_array->Length()];
        arg2 = (GLsizei*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (GLsizei*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLsizei*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    GLchar *arg3;
    GLchar* arg3_nonconst = NULL;
    if(args[3]->IsString()) {
        NanUtf8String arg3_utf8(args[3]);
        arg3 = (GLchar*)(*arg3_utf8);
    } else if(args[3]->IsArray()) {
        v8::Handle<v8::Array> arg3_array = v8::Handle<v8::Array>::Cast(args[3]);
        arg3_nonconst = new GLchar[arg3_array->Length()];
        arg3 = (GLchar*)arg3_nonconst;
    } else if(args[3]->IsObject()) {
        arg3 = (GLchar*)glbind_get_buffer_data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (GLchar*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glGetProgramPipelineInfoLog(arg0, arg1, arg2, arg3);
    if(arg2_nonconst) delete [] arg2_nonconst;
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttribL1d) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLdouble arg1;
    arg1 = args[1]->NumberValue();
    glVertexAttribL1d(arg0, arg1);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttribL2d) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLdouble arg1;
    arg1 = args[1]->NumberValue();
    GLdouble arg2;
    arg2 = args[2]->NumberValue();
    glVertexAttribL2d(arg0, arg1, arg2);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttribL3d) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLdouble arg1;
    arg1 = args[1]->NumberValue();
    GLdouble arg2;
    arg2 = args[2]->NumberValue();
    GLdouble arg3;
    arg3 = args[3]->NumberValue();
    glVertexAttribL3d(arg0, arg1, arg2, arg3);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttribL4d) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLdouble arg1;
    arg1 = args[1]->NumberValue();
    GLdouble arg2;
    arg2 = args[2]->NumberValue();
    GLdouble arg3;
    arg3 = args[3]->NumberValue();
    GLdouble arg4;
    arg4 = args[4]->NumberValue();
    glVertexAttribL4d(arg0, arg1, arg2, arg3, arg4);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttribL1dv) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    const GLdouble *arg1;
    GLdouble* arg1_nonconst = NULL;
    if(args[1]->IsString()) {
        NanUtf8String arg1_utf8(args[1]);
        arg1 = (const GLdouble*)(*arg1_utf8);
    } else if(args[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(args[1]);
        arg1_nonconst = new GLdouble[arg1_array->Length()];
        arg1 = (const GLdouble*)arg1_nonconst;
    } else if(args[1]->IsObject()) {
        arg1 = (const GLdouble*)glbind_get_buffer_data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLdouble*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttribL1dv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttribL2dv) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    const GLdouble *arg1;
    GLdouble* arg1_nonconst = NULL;
    if(args[1]->IsString()) {
        NanUtf8String arg1_utf8(args[1]);
        arg1 = (const GLdouble*)(*arg1_utf8);
    } else if(args[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(args[1]);
        arg1_nonconst = new GLdouble[arg1_array->Length()];
        arg1 = (const GLdouble*)arg1_nonconst;
    } else if(args[1]->IsObject()) {
        arg1 = (const GLdouble*)glbind_get_buffer_data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLdouble*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttribL2dv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttribL3dv) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    const GLdouble *arg1;
    GLdouble* arg1_nonconst = NULL;
    if(args[1]->IsString()) {
        NanUtf8String arg1_utf8(args[1]);
        arg1 = (const GLdouble*)(*arg1_utf8);
    } else if(args[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(args[1]);
        arg1_nonconst = new GLdouble[arg1_array->Length()];
        arg1 = (const GLdouble*)arg1_nonconst;
    } else if(args[1]->IsObject()) {
        arg1 = (const GLdouble*)glbind_get_buffer_data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLdouble*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttribL3dv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttribL4dv) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    const GLdouble *arg1;
    GLdouble* arg1_nonconst = NULL;
    if(args[1]->IsString()) {
        NanUtf8String arg1_utf8(args[1]);
        arg1 = (const GLdouble*)(*arg1_utf8);
    } else if(args[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(args[1]);
        arg1_nonconst = new GLdouble[arg1_array->Length()];
        arg1 = (const GLdouble*)arg1_nonconst;
    } else if(args[1]->IsObject()) {
        arg1 = (const GLdouble*)glbind_get_buffer_data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLdouble*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttribL4dv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_vertexAttribLPointer) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLenum arg2;
    arg2 = args[2]->Uint32Value();
    GLsizei arg3;
    arg3 = args[3]->Int32Value();
    const GLvoid *arg4;
    GLubyte* arg4_nonconst = NULL;
    if(args[4]->IsString()) {
        NanUtf8String arg4_utf8(args[4]);
        arg4 = (const GLvoid*)(*arg4_utf8);
    } else if(args[4]->IsArray()) {
        v8::Handle<v8::Array> arg4_array = v8::Handle<v8::Array>::Cast(args[4]);
        arg4_nonconst = new GLubyte[arg4_array->Length()];
        arg4 = (const GLvoid*)arg4_nonconst;
    } else if(args[4]->IsObject()) {
        arg4 = (const GLvoid*)glbind_get_buffer_data(args[4]);
    } else if(args[4]->IsNumber()) {
        arg4 = (const GLvoid*)(args[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glVertexAttribLPointer(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getVertexAttribLdv) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLdouble *arg2;
    GLdouble* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (GLdouble*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLdouble[arg2_array->Length()];
        arg2 = (GLdouble*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (GLdouble*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLdouble*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetVertexAttribLdv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_viewportArrayv) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLsizei arg1;
    arg1 = args[1]->Int32Value();
    const GLfloat *arg2;
    GLfloat* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (const GLfloat*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLfloat[arg2_array->Length()];
        arg2 = (const GLfloat*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (const GLfloat*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLfloat*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glViewportArrayv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_viewportIndexedf) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLfloat arg1;
    arg1 = (float)args[1]->NumberValue();
    GLfloat arg2;
    arg2 = (float)args[2]->NumberValue();
    GLfloat arg3;
    arg3 = (float)args[3]->NumberValue();
    GLfloat arg4;
    arg4 = (float)args[4]->NumberValue();
    glViewportIndexedf(arg0, arg1, arg2, arg3, arg4);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_viewportIndexedfv) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    const GLfloat *arg1;
    GLfloat* arg1_nonconst = NULL;
    if(args[1]->IsString()) {
        NanUtf8String arg1_utf8(args[1]);
        arg1 = (const GLfloat*)(*arg1_utf8);
    } else if(args[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(args[1]);
        arg1_nonconst = new GLfloat[arg1_array->Length()];
        arg1 = (const GLfloat*)arg1_nonconst;
    } else if(args[1]->IsObject()) {
        arg1 = (const GLfloat*)glbind_get_buffer_data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLfloat*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glViewportIndexedfv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_scissorArrayv) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLsizei arg1;
    arg1 = args[1]->Int32Value();
    const GLint *arg2;
    GLint* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (const GLint*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLint[arg2_array->Length()];
        arg2 = (const GLint*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (const GLint*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glScissorArrayv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_scissorIndexed) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLint arg2;
    arg2 = args[2]->Int32Value();
    GLsizei arg3;
    arg3 = args[3]->Int32Value();
    GLsizei arg4;
    arg4 = args[4]->Int32Value();
    glScissorIndexed(arg0, arg1, arg2, arg3, arg4);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_scissorIndexedv) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    const GLint *arg1;
    GLint* arg1_nonconst = NULL;
    if(args[1]->IsString()) {
        NanUtf8String arg1_utf8(args[1]);
        arg1 = (const GLint*)(*arg1_utf8);
    } else if(args[1]->IsArray()) {
        v8::Handle<v8::Array> arg1_array = v8::Handle<v8::Array>::Cast(args[1]);
        arg1_nonconst = new GLint[arg1_array->Length()];
        arg1 = (const GLint*)arg1_nonconst;
    } else if(args[1]->IsObject()) {
        arg1 = (const GLint*)glbind_get_buffer_data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLint*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glScissorIndexedv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_depthRangeArrayv) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLsizei arg1;
    arg1 = args[1]->Int32Value();
    const GLclampd *arg2;
    GLclampd* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (const GLclampd*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLclampd[arg2_array->Length()];
        arg2 = (const GLclampd*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (const GLclampd*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLclampd*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glDepthRangeArrayv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_depthRangeIndexed) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLclampd arg1;
    arg1 = args[1]->NumberValue();
    GLclampd arg2;
    arg2 = args[2]->NumberValue();
    glDepthRangeIndexed(arg0, arg1, arg2);
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getFloati_v) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLuint arg1;
    arg1 = args[1]->Uint32Value();
    GLfloat *arg2;
    GLfloat* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (GLfloat*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLfloat[arg2_array->Length()];
        arg2 = (GLfloat*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (GLfloat*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLfloat*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetFloati_v(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_getDoublei_v) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLuint arg1;
    arg1 = args[1]->Uint32Value();
    GLdouble *arg2;
    GLdouble* arg2_nonconst = NULL;
    if(args[2]->IsString()) {
        NanUtf8String arg2_utf8(args[2]);
        arg2 = (GLdouble*)(*arg2_utf8);
    } else if(args[2]->IsArray()) {
        v8::Handle<v8::Array> arg2_array = v8::Handle<v8::Array>::Cast(args[2]);
        arg2_nonconst = new GLdouble[arg2_array->Length()];
        arg2 = (GLdouble*)arg2_nonconst;
    } else if(args[2]->IsObject()) {
        arg2 = (GLdouble*)glbind_get_buffer_data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLdouble*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetDoublei_v(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
NAN_METHOD(EXPORT_shaderSource) {

    GLuint shader = node::ObjectWrap::Unwrap<NODE_Shader>(args[0]->ToObject())->gl_handle;
    v8::Handle<v8::Array> sources = v8::Handle<v8::Array>::Cast(args[1]);
    GLsizei count = sources->Length();
    const GLchar* *strings = new const GLchar*[count];
    GLint* lengths = new GLint[count];
    NanUtf8String **jstrings = new NanUtf8String*[count];
    for(GLsizei i = 0; i < count; i++) {
        jstrings[i] = new NanUtf8String(sources->Get(i));
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
    NanReturnUndefined();

}
void defineFunctions(v8::Handle<v8::ObjectTemplate> exports) {
    exports->Set(NanNew<v8::String>("cullFace"), NanNew<v8::FunctionTemplate>(EXPORT_cullFace));
    exports->Set(NanNew<v8::String>("frontFace"), NanNew<v8::FunctionTemplate>(EXPORT_frontFace));
    exports->Set(NanNew<v8::String>("hint"), NanNew<v8::FunctionTemplate>(EXPORT_hint));
    exports->Set(NanNew<v8::String>("lineWidth"), NanNew<v8::FunctionTemplate>(EXPORT_lineWidth));
    exports->Set(NanNew<v8::String>("pointSize"), NanNew<v8::FunctionTemplate>(EXPORT_pointSize));
    exports->Set(NanNew<v8::String>("polygonMode"), NanNew<v8::FunctionTemplate>(EXPORT_polygonMode));
    exports->Set(NanNew<v8::String>("scissor"), NanNew<v8::FunctionTemplate>(EXPORT_scissor));
    exports->Set(NanNew<v8::String>("texParameterf"), NanNew<v8::FunctionTemplate>(EXPORT_texParameterf));
    exports->Set(NanNew<v8::String>("texParameterfv"), NanNew<v8::FunctionTemplate>(EXPORT_texParameterfv));
    exports->Set(NanNew<v8::String>("texParameteri"), NanNew<v8::FunctionTemplate>(EXPORT_texParameteri));
    exports->Set(NanNew<v8::String>("texParameteriv"), NanNew<v8::FunctionTemplate>(EXPORT_texParameteriv));
    exports->Set(NanNew<v8::String>("texImage1D"), NanNew<v8::FunctionTemplate>(EXPORT_texImage1D));
    exports->Set(NanNew<v8::String>("texImage2D"), NanNew<v8::FunctionTemplate>(EXPORT_texImage2D));
    exports->Set(NanNew<v8::String>("drawBuffer"), NanNew<v8::FunctionTemplate>(EXPORT_drawBuffer));
    exports->Set(NanNew<v8::String>("clear"), NanNew<v8::FunctionTemplate>(EXPORT_clear));
    exports->Set(NanNew<v8::String>("clearColor"), NanNew<v8::FunctionTemplate>(EXPORT_clearColor));
    exports->Set(NanNew<v8::String>("clearStencil"), NanNew<v8::FunctionTemplate>(EXPORT_clearStencil));
    exports->Set(NanNew<v8::String>("clearDepth"), NanNew<v8::FunctionTemplate>(EXPORT_clearDepth));
    exports->Set(NanNew<v8::String>("stencilMask"), NanNew<v8::FunctionTemplate>(EXPORT_stencilMask));
    exports->Set(NanNew<v8::String>("colorMask"), NanNew<v8::FunctionTemplate>(EXPORT_colorMask));
    exports->Set(NanNew<v8::String>("depthMask"), NanNew<v8::FunctionTemplate>(EXPORT_depthMask));
    exports->Set(NanNew<v8::String>("disable"), NanNew<v8::FunctionTemplate>(EXPORT_disable));
    exports->Set(NanNew<v8::String>("enable"), NanNew<v8::FunctionTemplate>(EXPORT_enable));
    exports->Set(NanNew<v8::String>("finish"), NanNew<v8::FunctionTemplate>(EXPORT_finish));
    exports->Set(NanNew<v8::String>("flush"), NanNew<v8::FunctionTemplate>(EXPORT_flush));
    exports->Set(NanNew<v8::String>("blendFunc"), NanNew<v8::FunctionTemplate>(EXPORT_blendFunc));
    exports->Set(NanNew<v8::String>("logicOp"), NanNew<v8::FunctionTemplate>(EXPORT_logicOp));
    exports->Set(NanNew<v8::String>("stencilFunc"), NanNew<v8::FunctionTemplate>(EXPORT_stencilFunc));
    exports->Set(NanNew<v8::String>("stencilOp"), NanNew<v8::FunctionTemplate>(EXPORT_stencilOp));
    exports->Set(NanNew<v8::String>("depthFunc"), NanNew<v8::FunctionTemplate>(EXPORT_depthFunc));
    exports->Set(NanNew<v8::String>("pixelStoref"), NanNew<v8::FunctionTemplate>(EXPORT_pixelStoref));
    exports->Set(NanNew<v8::String>("pixelStorei"), NanNew<v8::FunctionTemplate>(EXPORT_pixelStorei));
    exports->Set(NanNew<v8::String>("readBuffer"), NanNew<v8::FunctionTemplate>(EXPORT_readBuffer));
    exports->Set(NanNew<v8::String>("readPixels"), NanNew<v8::FunctionTemplate>(EXPORT_readPixels));
    exports->Set(NanNew<v8::String>("getBooleanv"), NanNew<v8::FunctionTemplate>(EXPORT_getBooleanv));
    exports->Set(NanNew<v8::String>("getDoublev"), NanNew<v8::FunctionTemplate>(EXPORT_getDoublev));
    exports->Set(NanNew<v8::String>("getError"), NanNew<v8::FunctionTemplate>(EXPORT_getError));
    exports->Set(NanNew<v8::String>("getFloatv"), NanNew<v8::FunctionTemplate>(EXPORT_getFloatv));
    exports->Set(NanNew<v8::String>("getIntegerv"), NanNew<v8::FunctionTemplate>(EXPORT_getIntegerv));
    exports->Set(NanNew<v8::String>("getString"), NanNew<v8::FunctionTemplate>(EXPORT_getString));
    exports->Set(NanNew<v8::String>("getTexImage"), NanNew<v8::FunctionTemplate>(EXPORT_getTexImage));
    exports->Set(NanNew<v8::String>("getTexParameterfv"), NanNew<v8::FunctionTemplate>(EXPORT_getTexParameterfv));
    exports->Set(NanNew<v8::String>("getTexParameteriv"), NanNew<v8::FunctionTemplate>(EXPORT_getTexParameteriv));
    exports->Set(NanNew<v8::String>("getTexLevelParameterfv"), NanNew<v8::FunctionTemplate>(EXPORT_getTexLevelParameterfv));
    exports->Set(NanNew<v8::String>("getTexLevelParameteriv"), NanNew<v8::FunctionTemplate>(EXPORT_getTexLevelParameteriv));
    exports->Set(NanNew<v8::String>("isEnabled"), NanNew<v8::FunctionTemplate>(EXPORT_isEnabled));
    exports->Set(NanNew<v8::String>("depthRange"), NanNew<v8::FunctionTemplate>(EXPORT_depthRange));
    exports->Set(NanNew<v8::String>("viewport"), NanNew<v8::FunctionTemplate>(EXPORT_viewport));
    exports->Set(NanNew<v8::String>("drawArrays"), NanNew<v8::FunctionTemplate>(EXPORT_drawArrays));
    exports->Set(NanNew<v8::String>("drawElements"), NanNew<v8::FunctionTemplate>(EXPORT_drawElements));
    exports->Set(NanNew<v8::String>("polygonOffset"), NanNew<v8::FunctionTemplate>(EXPORT_polygonOffset));
    exports->Set(NanNew<v8::String>("copyTexImage1D"), NanNew<v8::FunctionTemplate>(EXPORT_copyTexImage1D));
    exports->Set(NanNew<v8::String>("copyTexImage2D"), NanNew<v8::FunctionTemplate>(EXPORT_copyTexImage2D));
    exports->Set(NanNew<v8::String>("copyTexSubImage1D"), NanNew<v8::FunctionTemplate>(EXPORT_copyTexSubImage1D));
    exports->Set(NanNew<v8::String>("copyTexSubImage2D"), NanNew<v8::FunctionTemplate>(EXPORT_copyTexSubImage2D));
    exports->Set(NanNew<v8::String>("texSubImage1D"), NanNew<v8::FunctionTemplate>(EXPORT_texSubImage1D));
    exports->Set(NanNew<v8::String>("texSubImage2D"), NanNew<v8::FunctionTemplate>(EXPORT_texSubImage2D));
    exports->Set(NanNew<v8::String>("bindTexture"), NanNew<v8::FunctionTemplate>(EXPORT_bindTexture));
    exports->Set(NanNew<v8::String>("genTextures"), NanNew<v8::FunctionTemplate>(EXPORT_genTextures));
    exports->Set(NanNew<v8::String>("isTexture"), NanNew<v8::FunctionTemplate>(EXPORT_isTexture));
    exports->Set(NanNew<v8::String>("blendColor"), NanNew<v8::FunctionTemplate>(EXPORT_blendColor));
    exports->Set(NanNew<v8::String>("blendEquation"), NanNew<v8::FunctionTemplate>(EXPORT_blendEquation));
    exports->Set(NanNew<v8::String>("drawRangeElements"), NanNew<v8::FunctionTemplate>(EXPORT_drawRangeElements));
    exports->Set(NanNew<v8::String>("texImage3D"), NanNew<v8::FunctionTemplate>(EXPORT_texImage3D));
    exports->Set(NanNew<v8::String>("texSubImage3D"), NanNew<v8::FunctionTemplate>(EXPORT_texSubImage3D));
    exports->Set(NanNew<v8::String>("copyTexSubImage3D"), NanNew<v8::FunctionTemplate>(EXPORT_copyTexSubImage3D));
    exports->Set(NanNew<v8::String>("activeTexture"), NanNew<v8::FunctionTemplate>(EXPORT_activeTexture));
    exports->Set(NanNew<v8::String>("sampleCoverage"), NanNew<v8::FunctionTemplate>(EXPORT_sampleCoverage));
    exports->Set(NanNew<v8::String>("compressedTexImage3D"), NanNew<v8::FunctionTemplate>(EXPORT_compressedTexImage3D));
    exports->Set(NanNew<v8::String>("compressedTexImage2D"), NanNew<v8::FunctionTemplate>(EXPORT_compressedTexImage2D));
    exports->Set(NanNew<v8::String>("compressedTexImage1D"), NanNew<v8::FunctionTemplate>(EXPORT_compressedTexImage1D));
    exports->Set(NanNew<v8::String>("compressedTexSubImage3D"), NanNew<v8::FunctionTemplate>(EXPORT_compressedTexSubImage3D));
    exports->Set(NanNew<v8::String>("compressedTexSubImage2D"), NanNew<v8::FunctionTemplate>(EXPORT_compressedTexSubImage2D));
    exports->Set(NanNew<v8::String>("compressedTexSubImage1D"), NanNew<v8::FunctionTemplate>(EXPORT_compressedTexSubImage1D));
    exports->Set(NanNew<v8::String>("getCompressedTexImage"), NanNew<v8::FunctionTemplate>(EXPORT_getCompressedTexImage));
    exports->Set(NanNew<v8::String>("blendFuncSeparate"), NanNew<v8::FunctionTemplate>(EXPORT_blendFuncSeparate));
    exports->Set(NanNew<v8::String>("multiDrawArrays"), NanNew<v8::FunctionTemplate>(EXPORT_multiDrawArrays));
    exports->Set(NanNew<v8::String>("pointParameterf"), NanNew<v8::FunctionTemplate>(EXPORT_pointParameterf));
    exports->Set(NanNew<v8::String>("pointParameterfv"), NanNew<v8::FunctionTemplate>(EXPORT_pointParameterfv));
    exports->Set(NanNew<v8::String>("pointParameteri"), NanNew<v8::FunctionTemplate>(EXPORT_pointParameteri));
    exports->Set(NanNew<v8::String>("pointParameteriv"), NanNew<v8::FunctionTemplate>(EXPORT_pointParameteriv));
    exports->Set(NanNew<v8::String>("genQueries"), NanNew<v8::FunctionTemplate>(EXPORT_genQueries));
    exports->Set(NanNew<v8::String>("isQuery"), NanNew<v8::FunctionTemplate>(EXPORT_isQuery));
    exports->Set(NanNew<v8::String>("beginQuery"), NanNew<v8::FunctionTemplate>(EXPORT_beginQuery));
    exports->Set(NanNew<v8::String>("endQuery"), NanNew<v8::FunctionTemplate>(EXPORT_endQuery));
    exports->Set(NanNew<v8::String>("getQueryiv"), NanNew<v8::FunctionTemplate>(EXPORT_getQueryiv));
    exports->Set(NanNew<v8::String>("getQueryObjectiv"), NanNew<v8::FunctionTemplate>(EXPORT_getQueryObjectiv));
    exports->Set(NanNew<v8::String>("getQueryObjectuiv"), NanNew<v8::FunctionTemplate>(EXPORT_getQueryObjectuiv));
    exports->Set(NanNew<v8::String>("bindBuffer"), NanNew<v8::FunctionTemplate>(EXPORT_bindBuffer));
    exports->Set(NanNew<v8::String>("genBuffers"), NanNew<v8::FunctionTemplate>(EXPORT_genBuffers));
    exports->Set(NanNew<v8::String>("isBuffer"), NanNew<v8::FunctionTemplate>(EXPORT_isBuffer));
    exports->Set(NanNew<v8::String>("bufferData"), NanNew<v8::FunctionTemplate>(EXPORT_bufferData));
    exports->Set(NanNew<v8::String>("bufferSubData"), NanNew<v8::FunctionTemplate>(EXPORT_bufferSubData));
    exports->Set(NanNew<v8::String>("getBufferSubData"), NanNew<v8::FunctionTemplate>(EXPORT_getBufferSubData));
    exports->Set(NanNew<v8::String>("mapBuffer"), NanNew<v8::FunctionTemplate>(EXPORT_mapBuffer));
    exports->Set(NanNew<v8::String>("unmapBuffer"), NanNew<v8::FunctionTemplate>(EXPORT_unmapBuffer));
    exports->Set(NanNew<v8::String>("getBufferParameteriv"), NanNew<v8::FunctionTemplate>(EXPORT_getBufferParameteriv));
    exports->Set(NanNew<v8::String>("blendEquationSeparate"), NanNew<v8::FunctionTemplate>(EXPORT_blendEquationSeparate));
    exports->Set(NanNew<v8::String>("drawBuffers"), NanNew<v8::FunctionTemplate>(EXPORT_drawBuffers));
    exports->Set(NanNew<v8::String>("stencilOpSeparate"), NanNew<v8::FunctionTemplate>(EXPORT_stencilOpSeparate));
    exports->Set(NanNew<v8::String>("stencilFuncSeparate"), NanNew<v8::FunctionTemplate>(EXPORT_stencilFuncSeparate));
    exports->Set(NanNew<v8::String>("stencilMaskSeparate"), NanNew<v8::FunctionTemplate>(EXPORT_stencilMaskSeparate));
    exports->Set(NanNew<v8::String>("attachShader"), NanNew<v8::FunctionTemplate>(EXPORT_attachShader));
    exports->Set(NanNew<v8::String>("bindAttribLocation"), NanNew<v8::FunctionTemplate>(EXPORT_bindAttribLocation));
    exports->Set(NanNew<v8::String>("compileShader"), NanNew<v8::FunctionTemplate>(EXPORT_compileShader));
    exports->Set(NanNew<v8::String>("createProgram"), NanNew<v8::FunctionTemplate>(EXPORT_createProgram));
    exports->Set(NanNew<v8::String>("createShader"), NanNew<v8::FunctionTemplate>(EXPORT_createShader));
    exports->Set(NanNew<v8::String>("detachShader"), NanNew<v8::FunctionTemplate>(EXPORT_detachShader));
    exports->Set(NanNew<v8::String>("disableVertexAttribArray"), NanNew<v8::FunctionTemplate>(EXPORT_disableVertexAttribArray));
    exports->Set(NanNew<v8::String>("enableVertexAttribArray"), NanNew<v8::FunctionTemplate>(EXPORT_enableVertexAttribArray));
    exports->Set(NanNew<v8::String>("getActiveAttrib"), NanNew<v8::FunctionTemplate>(EXPORT_getActiveAttrib));
    exports->Set(NanNew<v8::String>("getActiveUniform"), NanNew<v8::FunctionTemplate>(EXPORT_getActiveUniform));
    exports->Set(NanNew<v8::String>("getAttachedShaders"), NanNew<v8::FunctionTemplate>(EXPORT_getAttachedShaders));
    exports->Set(NanNew<v8::String>("getAttribLocation"), NanNew<v8::FunctionTemplate>(EXPORT_getAttribLocation));
    exports->Set(NanNew<v8::String>("getProgramiv"), NanNew<v8::FunctionTemplate>(EXPORT_getProgramiv));
    exports->Set(NanNew<v8::String>("getProgramInfoLog"), NanNew<v8::FunctionTemplate>(EXPORT_getProgramInfoLog));
    exports->Set(NanNew<v8::String>("getShaderiv"), NanNew<v8::FunctionTemplate>(EXPORT_getShaderiv));
    exports->Set(NanNew<v8::String>("getShaderInfoLog"), NanNew<v8::FunctionTemplate>(EXPORT_getShaderInfoLog));
    exports->Set(NanNew<v8::String>("getShaderSource"), NanNew<v8::FunctionTemplate>(EXPORT_getShaderSource));
    exports->Set(NanNew<v8::String>("getUniformLocation"), NanNew<v8::FunctionTemplate>(EXPORT_getUniformLocation));
    exports->Set(NanNew<v8::String>("getUniformfv"), NanNew<v8::FunctionTemplate>(EXPORT_getUniformfv));
    exports->Set(NanNew<v8::String>("getUniformiv"), NanNew<v8::FunctionTemplate>(EXPORT_getUniformiv));
    exports->Set(NanNew<v8::String>("getVertexAttribdv"), NanNew<v8::FunctionTemplate>(EXPORT_getVertexAttribdv));
    exports->Set(NanNew<v8::String>("getVertexAttribfv"), NanNew<v8::FunctionTemplate>(EXPORT_getVertexAttribfv));
    exports->Set(NanNew<v8::String>("getVertexAttribiv"), NanNew<v8::FunctionTemplate>(EXPORT_getVertexAttribiv));
    exports->Set(NanNew<v8::String>("isProgram"), NanNew<v8::FunctionTemplate>(EXPORT_isProgram));
    exports->Set(NanNew<v8::String>("isShader"), NanNew<v8::FunctionTemplate>(EXPORT_isShader));
    exports->Set(NanNew<v8::String>("linkProgram"), NanNew<v8::FunctionTemplate>(EXPORT_linkProgram));
    exports->Set(NanNew<v8::String>("useProgram"), NanNew<v8::FunctionTemplate>(EXPORT_useProgram));
    exports->Set(NanNew<v8::String>("uniform1f"), NanNew<v8::FunctionTemplate>(EXPORT_uniform1f));
    exports->Set(NanNew<v8::String>("uniform2f"), NanNew<v8::FunctionTemplate>(EXPORT_uniform2f));
    exports->Set(NanNew<v8::String>("uniform3f"), NanNew<v8::FunctionTemplate>(EXPORT_uniform3f));
    exports->Set(NanNew<v8::String>("uniform4f"), NanNew<v8::FunctionTemplate>(EXPORT_uniform4f));
    exports->Set(NanNew<v8::String>("uniform1i"), NanNew<v8::FunctionTemplate>(EXPORT_uniform1i));
    exports->Set(NanNew<v8::String>("uniform2i"), NanNew<v8::FunctionTemplate>(EXPORT_uniform2i));
    exports->Set(NanNew<v8::String>("uniform3i"), NanNew<v8::FunctionTemplate>(EXPORT_uniform3i));
    exports->Set(NanNew<v8::String>("uniform4i"), NanNew<v8::FunctionTemplate>(EXPORT_uniform4i));
    exports->Set(NanNew<v8::String>("uniform1fv"), NanNew<v8::FunctionTemplate>(EXPORT_uniform1fv));
    exports->Set(NanNew<v8::String>("uniform2fv"), NanNew<v8::FunctionTemplate>(EXPORT_uniform2fv));
    exports->Set(NanNew<v8::String>("uniform3fv"), NanNew<v8::FunctionTemplate>(EXPORT_uniform3fv));
    exports->Set(NanNew<v8::String>("uniform4fv"), NanNew<v8::FunctionTemplate>(EXPORT_uniform4fv));
    exports->Set(NanNew<v8::String>("uniform1iv"), NanNew<v8::FunctionTemplate>(EXPORT_uniform1iv));
    exports->Set(NanNew<v8::String>("uniform2iv"), NanNew<v8::FunctionTemplate>(EXPORT_uniform2iv));
    exports->Set(NanNew<v8::String>("uniform3iv"), NanNew<v8::FunctionTemplate>(EXPORT_uniform3iv));
    exports->Set(NanNew<v8::String>("uniform4iv"), NanNew<v8::FunctionTemplate>(EXPORT_uniform4iv));
    exports->Set(NanNew<v8::String>("uniformMatrix2fv"), NanNew<v8::FunctionTemplate>(EXPORT_uniformMatrix2fv));
    exports->Set(NanNew<v8::String>("uniformMatrix3fv"), NanNew<v8::FunctionTemplate>(EXPORT_uniformMatrix3fv));
    exports->Set(NanNew<v8::String>("uniformMatrix4fv"), NanNew<v8::FunctionTemplate>(EXPORT_uniformMatrix4fv));
    exports->Set(NanNew<v8::String>("validateProgram"), NanNew<v8::FunctionTemplate>(EXPORT_validateProgram));
    exports->Set(NanNew<v8::String>("vertexAttrib1d"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttrib1d));
    exports->Set(NanNew<v8::String>("vertexAttrib1dv"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttrib1dv));
    exports->Set(NanNew<v8::String>("vertexAttrib1f"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttrib1f));
    exports->Set(NanNew<v8::String>("vertexAttrib1fv"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttrib1fv));
    exports->Set(NanNew<v8::String>("vertexAttrib1s"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttrib1s));
    exports->Set(NanNew<v8::String>("vertexAttrib1sv"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttrib1sv));
    exports->Set(NanNew<v8::String>("vertexAttrib2d"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttrib2d));
    exports->Set(NanNew<v8::String>("vertexAttrib2dv"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttrib2dv));
    exports->Set(NanNew<v8::String>("vertexAttrib2f"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttrib2f));
    exports->Set(NanNew<v8::String>("vertexAttrib2fv"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttrib2fv));
    exports->Set(NanNew<v8::String>("vertexAttrib2s"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttrib2s));
    exports->Set(NanNew<v8::String>("vertexAttrib2sv"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttrib2sv));
    exports->Set(NanNew<v8::String>("vertexAttrib3d"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttrib3d));
    exports->Set(NanNew<v8::String>("vertexAttrib3dv"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttrib3dv));
    exports->Set(NanNew<v8::String>("vertexAttrib3f"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttrib3f));
    exports->Set(NanNew<v8::String>("vertexAttrib3fv"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttrib3fv));
    exports->Set(NanNew<v8::String>("vertexAttrib3s"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttrib3s));
    exports->Set(NanNew<v8::String>("vertexAttrib3sv"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttrib3sv));
    exports->Set(NanNew<v8::String>("vertexAttrib4Nbv"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttrib4Nbv));
    exports->Set(NanNew<v8::String>("vertexAttrib4Niv"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttrib4Niv));
    exports->Set(NanNew<v8::String>("vertexAttrib4Nsv"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttrib4Nsv));
    exports->Set(NanNew<v8::String>("vertexAttrib4Nub"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttrib4Nub));
    exports->Set(NanNew<v8::String>("vertexAttrib4Nubv"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttrib4Nubv));
    exports->Set(NanNew<v8::String>("vertexAttrib4Nuiv"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttrib4Nuiv));
    exports->Set(NanNew<v8::String>("vertexAttrib4Nusv"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttrib4Nusv));
    exports->Set(NanNew<v8::String>("vertexAttrib4bv"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttrib4bv));
    exports->Set(NanNew<v8::String>("vertexAttrib4d"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttrib4d));
    exports->Set(NanNew<v8::String>("vertexAttrib4dv"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttrib4dv));
    exports->Set(NanNew<v8::String>("vertexAttrib4f"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttrib4f));
    exports->Set(NanNew<v8::String>("vertexAttrib4fv"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttrib4fv));
    exports->Set(NanNew<v8::String>("vertexAttrib4iv"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttrib4iv));
    exports->Set(NanNew<v8::String>("vertexAttrib4s"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttrib4s));
    exports->Set(NanNew<v8::String>("vertexAttrib4sv"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttrib4sv));
    exports->Set(NanNew<v8::String>("vertexAttrib4ubv"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttrib4ubv));
    exports->Set(NanNew<v8::String>("vertexAttrib4uiv"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttrib4uiv));
    exports->Set(NanNew<v8::String>("vertexAttrib4usv"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttrib4usv));
    exports->Set(NanNew<v8::String>("vertexAttribPointer"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttribPointer));
    exports->Set(NanNew<v8::String>("uniformMatrix2x3fv"), NanNew<v8::FunctionTemplate>(EXPORT_uniformMatrix2x3fv));
    exports->Set(NanNew<v8::String>("uniformMatrix3x2fv"), NanNew<v8::FunctionTemplate>(EXPORT_uniformMatrix3x2fv));
    exports->Set(NanNew<v8::String>("uniformMatrix2x4fv"), NanNew<v8::FunctionTemplate>(EXPORT_uniformMatrix2x4fv));
    exports->Set(NanNew<v8::String>("uniformMatrix4x2fv"), NanNew<v8::FunctionTemplate>(EXPORT_uniformMatrix4x2fv));
    exports->Set(NanNew<v8::String>("uniformMatrix3x4fv"), NanNew<v8::FunctionTemplate>(EXPORT_uniformMatrix3x4fv));
    exports->Set(NanNew<v8::String>("uniformMatrix4x3fv"), NanNew<v8::FunctionTemplate>(EXPORT_uniformMatrix4x3fv));
    exports->Set(NanNew<v8::String>("colorMaski"), NanNew<v8::FunctionTemplate>(EXPORT_colorMaski));
    exports->Set(NanNew<v8::String>("getBooleani_v"), NanNew<v8::FunctionTemplate>(EXPORT_getBooleani_v));
    exports->Set(NanNew<v8::String>("getIntegeri_v"), NanNew<v8::FunctionTemplate>(EXPORT_getIntegeri_v));
    exports->Set(NanNew<v8::String>("enablei"), NanNew<v8::FunctionTemplate>(EXPORT_enablei));
    exports->Set(NanNew<v8::String>("disablei"), NanNew<v8::FunctionTemplate>(EXPORT_disablei));
    exports->Set(NanNew<v8::String>("isEnabledi"), NanNew<v8::FunctionTemplate>(EXPORT_isEnabledi));
    exports->Set(NanNew<v8::String>("beginTransformFeedback"), NanNew<v8::FunctionTemplate>(EXPORT_beginTransformFeedback));
    exports->Set(NanNew<v8::String>("endTransformFeedback"), NanNew<v8::FunctionTemplate>(EXPORT_endTransformFeedback));
    exports->Set(NanNew<v8::String>("bindBufferRange"), NanNew<v8::FunctionTemplate>(EXPORT_bindBufferRange));
    exports->Set(NanNew<v8::String>("bindBufferBase"), NanNew<v8::FunctionTemplate>(EXPORT_bindBufferBase));
    exports->Set(NanNew<v8::String>("getTransformFeedbackVarying"), NanNew<v8::FunctionTemplate>(EXPORT_getTransformFeedbackVarying));
    exports->Set(NanNew<v8::String>("clampColor"), NanNew<v8::FunctionTemplate>(EXPORT_clampColor));
    exports->Set(NanNew<v8::String>("beginConditionalRender"), NanNew<v8::FunctionTemplate>(EXPORT_beginConditionalRender));
    exports->Set(NanNew<v8::String>("endConditionalRender"), NanNew<v8::FunctionTemplate>(EXPORT_endConditionalRender));
    exports->Set(NanNew<v8::String>("vertexAttribIPointer"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttribIPointer));
    exports->Set(NanNew<v8::String>("getVertexAttribIiv"), NanNew<v8::FunctionTemplate>(EXPORT_getVertexAttribIiv));
    exports->Set(NanNew<v8::String>("getVertexAttribIuiv"), NanNew<v8::FunctionTemplate>(EXPORT_getVertexAttribIuiv));
    exports->Set(NanNew<v8::String>("vertexAttribI1i"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttribI1i));
    exports->Set(NanNew<v8::String>("vertexAttribI2i"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttribI2i));
    exports->Set(NanNew<v8::String>("vertexAttribI3i"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttribI3i));
    exports->Set(NanNew<v8::String>("vertexAttribI4i"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttribI4i));
    exports->Set(NanNew<v8::String>("vertexAttribI1ui"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttribI1ui));
    exports->Set(NanNew<v8::String>("vertexAttribI2ui"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttribI2ui));
    exports->Set(NanNew<v8::String>("vertexAttribI3ui"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttribI3ui));
    exports->Set(NanNew<v8::String>("vertexAttribI4ui"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttribI4ui));
    exports->Set(NanNew<v8::String>("vertexAttribI1iv"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttribI1iv));
    exports->Set(NanNew<v8::String>("vertexAttribI2iv"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttribI2iv));
    exports->Set(NanNew<v8::String>("vertexAttribI3iv"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttribI3iv));
    exports->Set(NanNew<v8::String>("vertexAttribI4iv"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttribI4iv));
    exports->Set(NanNew<v8::String>("vertexAttribI1uiv"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttribI1uiv));
    exports->Set(NanNew<v8::String>("vertexAttribI2uiv"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttribI2uiv));
    exports->Set(NanNew<v8::String>("vertexAttribI3uiv"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttribI3uiv));
    exports->Set(NanNew<v8::String>("vertexAttribI4uiv"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttribI4uiv));
    exports->Set(NanNew<v8::String>("vertexAttribI4bv"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttribI4bv));
    exports->Set(NanNew<v8::String>("vertexAttribI4sv"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttribI4sv));
    exports->Set(NanNew<v8::String>("vertexAttribI4ubv"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttribI4ubv));
    exports->Set(NanNew<v8::String>("vertexAttribI4usv"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttribI4usv));
    exports->Set(NanNew<v8::String>("getUniformuiv"), NanNew<v8::FunctionTemplate>(EXPORT_getUniformuiv));
    exports->Set(NanNew<v8::String>("bindFragDataLocation"), NanNew<v8::FunctionTemplate>(EXPORT_bindFragDataLocation));
    exports->Set(NanNew<v8::String>("getFragDataLocation"), NanNew<v8::FunctionTemplate>(EXPORT_getFragDataLocation));
    exports->Set(NanNew<v8::String>("uniform1ui"), NanNew<v8::FunctionTemplate>(EXPORT_uniform1ui));
    exports->Set(NanNew<v8::String>("uniform2ui"), NanNew<v8::FunctionTemplate>(EXPORT_uniform2ui));
    exports->Set(NanNew<v8::String>("uniform3ui"), NanNew<v8::FunctionTemplate>(EXPORT_uniform3ui));
    exports->Set(NanNew<v8::String>("uniform4ui"), NanNew<v8::FunctionTemplate>(EXPORT_uniform4ui));
    exports->Set(NanNew<v8::String>("uniform1uiv"), NanNew<v8::FunctionTemplate>(EXPORT_uniform1uiv));
    exports->Set(NanNew<v8::String>("uniform2uiv"), NanNew<v8::FunctionTemplate>(EXPORT_uniform2uiv));
    exports->Set(NanNew<v8::String>("uniform3uiv"), NanNew<v8::FunctionTemplate>(EXPORT_uniform3uiv));
    exports->Set(NanNew<v8::String>("uniform4uiv"), NanNew<v8::FunctionTemplate>(EXPORT_uniform4uiv));
    exports->Set(NanNew<v8::String>("texParameterIiv"), NanNew<v8::FunctionTemplate>(EXPORT_texParameterIiv));
    exports->Set(NanNew<v8::String>("texParameterIuiv"), NanNew<v8::FunctionTemplate>(EXPORT_texParameterIuiv));
    exports->Set(NanNew<v8::String>("getTexParameterIiv"), NanNew<v8::FunctionTemplate>(EXPORT_getTexParameterIiv));
    exports->Set(NanNew<v8::String>("getTexParameterIuiv"), NanNew<v8::FunctionTemplate>(EXPORT_getTexParameterIuiv));
    exports->Set(NanNew<v8::String>("clearBufferiv"), NanNew<v8::FunctionTemplate>(EXPORT_clearBufferiv));
    exports->Set(NanNew<v8::String>("clearBufferuiv"), NanNew<v8::FunctionTemplate>(EXPORT_clearBufferuiv));
    exports->Set(NanNew<v8::String>("clearBufferfv"), NanNew<v8::FunctionTemplate>(EXPORT_clearBufferfv));
    exports->Set(NanNew<v8::String>("clearBufferfi"), NanNew<v8::FunctionTemplate>(EXPORT_clearBufferfi));
    exports->Set(NanNew<v8::String>("getStringi"), NanNew<v8::FunctionTemplate>(EXPORT_getStringi));
    exports->Set(NanNew<v8::String>("drawArraysInstanced"), NanNew<v8::FunctionTemplate>(EXPORT_drawArraysInstanced));
    exports->Set(NanNew<v8::String>("drawElementsInstanced"), NanNew<v8::FunctionTemplate>(EXPORT_drawElementsInstanced));
    exports->Set(NanNew<v8::String>("texBuffer"), NanNew<v8::FunctionTemplate>(EXPORT_texBuffer));
    exports->Set(NanNew<v8::String>("primitiveRestartIndex"), NanNew<v8::FunctionTemplate>(EXPORT_primitiveRestartIndex));
    exports->Set(NanNew<v8::String>("getInteger64i_v"), NanNew<v8::FunctionTemplate>(EXPORT_getInteger64i_v));
    exports->Set(NanNew<v8::String>("getBufferParameteri64v"), NanNew<v8::FunctionTemplate>(EXPORT_getBufferParameteri64v));
    exports->Set(NanNew<v8::String>("framebufferTexture"), NanNew<v8::FunctionTemplate>(EXPORT_framebufferTexture));
    exports->Set(NanNew<v8::String>("vertexAttribDivisor"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttribDivisor));
    exports->Set(NanNew<v8::String>("minSampleShading"), NanNew<v8::FunctionTemplate>(EXPORT_minSampleShading));
    exports->Set(NanNew<v8::String>("blendEquationi"), NanNew<v8::FunctionTemplate>(EXPORT_blendEquationi));
    exports->Set(NanNew<v8::String>("blendEquationSeparatei"), NanNew<v8::FunctionTemplate>(EXPORT_blendEquationSeparatei));
    exports->Set(NanNew<v8::String>("blendFunci"), NanNew<v8::FunctionTemplate>(EXPORT_blendFunci));
    exports->Set(NanNew<v8::String>("blendFuncSeparatei"), NanNew<v8::FunctionTemplate>(EXPORT_blendFuncSeparatei));
    exports->Set(NanNew<v8::String>("isRenderbuffer"), NanNew<v8::FunctionTemplate>(EXPORT_isRenderbuffer));
    exports->Set(NanNew<v8::String>("bindRenderbuffer"), NanNew<v8::FunctionTemplate>(EXPORT_bindRenderbuffer));
    exports->Set(NanNew<v8::String>("genRenderbuffers"), NanNew<v8::FunctionTemplate>(EXPORT_genRenderbuffers));
    exports->Set(NanNew<v8::String>("renderbufferStorage"), NanNew<v8::FunctionTemplate>(EXPORT_renderbufferStorage));
    exports->Set(NanNew<v8::String>("getRenderbufferParameteriv"), NanNew<v8::FunctionTemplate>(EXPORT_getRenderbufferParameteriv));
    exports->Set(NanNew<v8::String>("isFramebuffer"), NanNew<v8::FunctionTemplate>(EXPORT_isFramebuffer));
    exports->Set(NanNew<v8::String>("bindFramebuffer"), NanNew<v8::FunctionTemplate>(EXPORT_bindFramebuffer));
    exports->Set(NanNew<v8::String>("genFramebuffers"), NanNew<v8::FunctionTemplate>(EXPORT_genFramebuffers));
    exports->Set(NanNew<v8::String>("checkFramebufferStatus"), NanNew<v8::FunctionTemplate>(EXPORT_checkFramebufferStatus));
    exports->Set(NanNew<v8::String>("framebufferTexture1D"), NanNew<v8::FunctionTemplate>(EXPORT_framebufferTexture1D));
    exports->Set(NanNew<v8::String>("framebufferTexture2D"), NanNew<v8::FunctionTemplate>(EXPORT_framebufferTexture2D));
    exports->Set(NanNew<v8::String>("framebufferTexture3D"), NanNew<v8::FunctionTemplate>(EXPORT_framebufferTexture3D));
    exports->Set(NanNew<v8::String>("framebufferRenderbuffer"), NanNew<v8::FunctionTemplate>(EXPORT_framebufferRenderbuffer));
    exports->Set(NanNew<v8::String>("getFramebufferAttachmentParameteriv"), NanNew<v8::FunctionTemplate>(EXPORT_getFramebufferAttachmentParameteriv));
    exports->Set(NanNew<v8::String>("generateMipmap"), NanNew<v8::FunctionTemplate>(EXPORT_generateMipmap));
    exports->Set(NanNew<v8::String>("blitFramebuffer"), NanNew<v8::FunctionTemplate>(EXPORT_blitFramebuffer));
    exports->Set(NanNew<v8::String>("renderbufferStorageMultisample"), NanNew<v8::FunctionTemplate>(EXPORT_renderbufferStorageMultisample));
    exports->Set(NanNew<v8::String>("framebufferTextureLayer"), NanNew<v8::FunctionTemplate>(EXPORT_framebufferTextureLayer));
    exports->Set(NanNew<v8::String>("mapBufferRange"), NanNew<v8::FunctionTemplate>(EXPORT_mapBufferRange));
    exports->Set(NanNew<v8::String>("flushMappedBufferRange"), NanNew<v8::FunctionTemplate>(EXPORT_flushMappedBufferRange));
    exports->Set(NanNew<v8::String>("bindVertexArray"), NanNew<v8::FunctionTemplate>(EXPORT_bindVertexArray));
    exports->Set(NanNew<v8::String>("genVertexArrays"), NanNew<v8::FunctionTemplate>(EXPORT_genVertexArrays));
    exports->Set(NanNew<v8::String>("isVertexArray"), NanNew<v8::FunctionTemplate>(EXPORT_isVertexArray));
    exports->Set(NanNew<v8::String>("getActiveUniformsiv"), NanNew<v8::FunctionTemplate>(EXPORT_getActiveUniformsiv));
    exports->Set(NanNew<v8::String>("getActiveUniformName"), NanNew<v8::FunctionTemplate>(EXPORT_getActiveUniformName));
    exports->Set(NanNew<v8::String>("getUniformBlockIndex"), NanNew<v8::FunctionTemplate>(EXPORT_getUniformBlockIndex));
    exports->Set(NanNew<v8::String>("getActiveUniformBlockiv"), NanNew<v8::FunctionTemplate>(EXPORT_getActiveUniformBlockiv));
    exports->Set(NanNew<v8::String>("getActiveUniformBlockName"), NanNew<v8::FunctionTemplate>(EXPORT_getActiveUniformBlockName));
    exports->Set(NanNew<v8::String>("uniformBlockBinding"), NanNew<v8::FunctionTemplate>(EXPORT_uniformBlockBinding));
    exports->Set(NanNew<v8::String>("copyBufferSubData"), NanNew<v8::FunctionTemplate>(EXPORT_copyBufferSubData));
    exports->Set(NanNew<v8::String>("drawElementsBaseVertex"), NanNew<v8::FunctionTemplate>(EXPORT_drawElementsBaseVertex));
    exports->Set(NanNew<v8::String>("drawRangeElementsBaseVertex"), NanNew<v8::FunctionTemplate>(EXPORT_drawRangeElementsBaseVertex));
    exports->Set(NanNew<v8::String>("drawElementsInstancedBaseVertex"), NanNew<v8::FunctionTemplate>(EXPORT_drawElementsInstancedBaseVertex));
    exports->Set(NanNew<v8::String>("provokingVertex"), NanNew<v8::FunctionTemplate>(EXPORT_provokingVertex));
    exports->Set(NanNew<v8::String>("texImage2DMultisample"), NanNew<v8::FunctionTemplate>(EXPORT_texImage2DMultisample));
    exports->Set(NanNew<v8::String>("texImage3DMultisample"), NanNew<v8::FunctionTemplate>(EXPORT_texImage3DMultisample));
    exports->Set(NanNew<v8::String>("getMultisamplefv"), NanNew<v8::FunctionTemplate>(EXPORT_getMultisamplefv));
    exports->Set(NanNew<v8::String>("sampleMaski"), NanNew<v8::FunctionTemplate>(EXPORT_sampleMaski));
    exports->Set(NanNew<v8::String>("bindFragDataLocationIndexed"), NanNew<v8::FunctionTemplate>(EXPORT_bindFragDataLocationIndexed));
    exports->Set(NanNew<v8::String>("getFragDataIndex"), NanNew<v8::FunctionTemplate>(EXPORT_getFragDataIndex));
    exports->Set(NanNew<v8::String>("genSamplers"), NanNew<v8::FunctionTemplate>(EXPORT_genSamplers));
    exports->Set(NanNew<v8::String>("isSampler"), NanNew<v8::FunctionTemplate>(EXPORT_isSampler));
    exports->Set(NanNew<v8::String>("bindSampler"), NanNew<v8::FunctionTemplate>(EXPORT_bindSampler));
    exports->Set(NanNew<v8::String>("samplerParameteri"), NanNew<v8::FunctionTemplate>(EXPORT_samplerParameteri));
    exports->Set(NanNew<v8::String>("samplerParameteriv"), NanNew<v8::FunctionTemplate>(EXPORT_samplerParameteriv));
    exports->Set(NanNew<v8::String>("samplerParameterf"), NanNew<v8::FunctionTemplate>(EXPORT_samplerParameterf));
    exports->Set(NanNew<v8::String>("samplerParameterfv"), NanNew<v8::FunctionTemplate>(EXPORT_samplerParameterfv));
    exports->Set(NanNew<v8::String>("samplerParameterIiv"), NanNew<v8::FunctionTemplate>(EXPORT_samplerParameterIiv));
    exports->Set(NanNew<v8::String>("samplerParameterIuiv"), NanNew<v8::FunctionTemplate>(EXPORT_samplerParameterIuiv));
    exports->Set(NanNew<v8::String>("getSamplerParameteriv"), NanNew<v8::FunctionTemplate>(EXPORT_getSamplerParameteriv));
    exports->Set(NanNew<v8::String>("getSamplerParameterIiv"), NanNew<v8::FunctionTemplate>(EXPORT_getSamplerParameterIiv));
    exports->Set(NanNew<v8::String>("getSamplerParameterfv"), NanNew<v8::FunctionTemplate>(EXPORT_getSamplerParameterfv));
    exports->Set(NanNew<v8::String>("getSamplerParameterIuiv"), NanNew<v8::FunctionTemplate>(EXPORT_getSamplerParameterIuiv));
    exports->Set(NanNew<v8::String>("queryCounter"), NanNew<v8::FunctionTemplate>(EXPORT_queryCounter));
    exports->Set(NanNew<v8::String>("getQueryObjecti64v"), NanNew<v8::FunctionTemplate>(EXPORT_getQueryObjecti64v));
    exports->Set(NanNew<v8::String>("getQueryObjectui64v"), NanNew<v8::FunctionTemplate>(EXPORT_getQueryObjectui64v));
    exports->Set(NanNew<v8::String>("vertexAttribP1ui"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttribP1ui));
    exports->Set(NanNew<v8::String>("vertexAttribP1uiv"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttribP1uiv));
    exports->Set(NanNew<v8::String>("vertexAttribP2ui"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttribP2ui));
    exports->Set(NanNew<v8::String>("vertexAttribP2uiv"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttribP2uiv));
    exports->Set(NanNew<v8::String>("vertexAttribP3ui"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttribP3ui));
    exports->Set(NanNew<v8::String>("vertexAttribP3uiv"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttribP3uiv));
    exports->Set(NanNew<v8::String>("vertexAttribP4ui"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttribP4ui));
    exports->Set(NanNew<v8::String>("vertexAttribP4uiv"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttribP4uiv));
    exports->Set(NanNew<v8::String>("drawArraysIndirect"), NanNew<v8::FunctionTemplate>(EXPORT_drawArraysIndirect));
    exports->Set(NanNew<v8::String>("drawElementsIndirect"), NanNew<v8::FunctionTemplate>(EXPORT_drawElementsIndirect));
    exports->Set(NanNew<v8::String>("uniform1d"), NanNew<v8::FunctionTemplate>(EXPORT_uniform1d));
    exports->Set(NanNew<v8::String>("uniform2d"), NanNew<v8::FunctionTemplate>(EXPORT_uniform2d));
    exports->Set(NanNew<v8::String>("uniform3d"), NanNew<v8::FunctionTemplate>(EXPORT_uniform3d));
    exports->Set(NanNew<v8::String>("uniform4d"), NanNew<v8::FunctionTemplate>(EXPORT_uniform4d));
    exports->Set(NanNew<v8::String>("uniform1dv"), NanNew<v8::FunctionTemplate>(EXPORT_uniform1dv));
    exports->Set(NanNew<v8::String>("uniform2dv"), NanNew<v8::FunctionTemplate>(EXPORT_uniform2dv));
    exports->Set(NanNew<v8::String>("uniform3dv"), NanNew<v8::FunctionTemplate>(EXPORT_uniform3dv));
    exports->Set(NanNew<v8::String>("uniform4dv"), NanNew<v8::FunctionTemplate>(EXPORT_uniform4dv));
    exports->Set(NanNew<v8::String>("uniformMatrix2dv"), NanNew<v8::FunctionTemplate>(EXPORT_uniformMatrix2dv));
    exports->Set(NanNew<v8::String>("uniformMatrix3dv"), NanNew<v8::FunctionTemplate>(EXPORT_uniformMatrix3dv));
    exports->Set(NanNew<v8::String>("uniformMatrix4dv"), NanNew<v8::FunctionTemplate>(EXPORT_uniformMatrix4dv));
    exports->Set(NanNew<v8::String>("uniformMatrix2x3dv"), NanNew<v8::FunctionTemplate>(EXPORT_uniformMatrix2x3dv));
    exports->Set(NanNew<v8::String>("uniformMatrix2x4dv"), NanNew<v8::FunctionTemplate>(EXPORT_uniformMatrix2x4dv));
    exports->Set(NanNew<v8::String>("uniformMatrix3x2dv"), NanNew<v8::FunctionTemplate>(EXPORT_uniformMatrix3x2dv));
    exports->Set(NanNew<v8::String>("uniformMatrix3x4dv"), NanNew<v8::FunctionTemplate>(EXPORT_uniformMatrix3x4dv));
    exports->Set(NanNew<v8::String>("uniformMatrix4x2dv"), NanNew<v8::FunctionTemplate>(EXPORT_uniformMatrix4x2dv));
    exports->Set(NanNew<v8::String>("uniformMatrix4x3dv"), NanNew<v8::FunctionTemplate>(EXPORT_uniformMatrix4x3dv));
    exports->Set(NanNew<v8::String>("getUniformdv"), NanNew<v8::FunctionTemplate>(EXPORT_getUniformdv));
    exports->Set(NanNew<v8::String>("getSubroutineUniformLocation"), NanNew<v8::FunctionTemplate>(EXPORT_getSubroutineUniformLocation));
    exports->Set(NanNew<v8::String>("getSubroutineIndex"), NanNew<v8::FunctionTemplate>(EXPORT_getSubroutineIndex));
    exports->Set(NanNew<v8::String>("getActiveSubroutineUniformiv"), NanNew<v8::FunctionTemplate>(EXPORT_getActiveSubroutineUniformiv));
    exports->Set(NanNew<v8::String>("getActiveSubroutineUniformName"), NanNew<v8::FunctionTemplate>(EXPORT_getActiveSubroutineUniformName));
    exports->Set(NanNew<v8::String>("getActiveSubroutineName"), NanNew<v8::FunctionTemplate>(EXPORT_getActiveSubroutineName));
    exports->Set(NanNew<v8::String>("uniformSubroutinesuiv"), NanNew<v8::FunctionTemplate>(EXPORT_uniformSubroutinesuiv));
    exports->Set(NanNew<v8::String>("getUniformSubroutineuiv"), NanNew<v8::FunctionTemplate>(EXPORT_getUniformSubroutineuiv));
    exports->Set(NanNew<v8::String>("getProgramStageiv"), NanNew<v8::FunctionTemplate>(EXPORT_getProgramStageiv));
    exports->Set(NanNew<v8::String>("patchParameteri"), NanNew<v8::FunctionTemplate>(EXPORT_patchParameteri));
    exports->Set(NanNew<v8::String>("patchParameterfv"), NanNew<v8::FunctionTemplate>(EXPORT_patchParameterfv));
    exports->Set(NanNew<v8::String>("bindTransformFeedback"), NanNew<v8::FunctionTemplate>(EXPORT_bindTransformFeedback));
    exports->Set(NanNew<v8::String>("genTransformFeedbacks"), NanNew<v8::FunctionTemplate>(EXPORT_genTransformFeedbacks));
    exports->Set(NanNew<v8::String>("isTransformFeedback"), NanNew<v8::FunctionTemplate>(EXPORT_isTransformFeedback));
    exports->Set(NanNew<v8::String>("pauseTransformFeedback"), NanNew<v8::FunctionTemplate>(EXPORT_pauseTransformFeedback));
    exports->Set(NanNew<v8::String>("resumeTransformFeedback"), NanNew<v8::FunctionTemplate>(EXPORT_resumeTransformFeedback));
    exports->Set(NanNew<v8::String>("drawTransformFeedback"), NanNew<v8::FunctionTemplate>(EXPORT_drawTransformFeedback));
    exports->Set(NanNew<v8::String>("drawTransformFeedbackStream"), NanNew<v8::FunctionTemplate>(EXPORT_drawTransformFeedbackStream));
    exports->Set(NanNew<v8::String>("beginQueryIndexed"), NanNew<v8::FunctionTemplate>(EXPORT_beginQueryIndexed));
    exports->Set(NanNew<v8::String>("endQueryIndexed"), NanNew<v8::FunctionTemplate>(EXPORT_endQueryIndexed));
    exports->Set(NanNew<v8::String>("getQueryIndexediv"), NanNew<v8::FunctionTemplate>(EXPORT_getQueryIndexediv));
    exports->Set(NanNew<v8::String>("releaseShaderCompiler"), NanNew<v8::FunctionTemplate>(EXPORT_releaseShaderCompiler));
    exports->Set(NanNew<v8::String>("shaderBinary"), NanNew<v8::FunctionTemplate>(EXPORT_shaderBinary));
    exports->Set(NanNew<v8::String>("getShaderPrecisionFormat"), NanNew<v8::FunctionTemplate>(EXPORT_getShaderPrecisionFormat));
    exports->Set(NanNew<v8::String>("depthRangef"), NanNew<v8::FunctionTemplate>(EXPORT_depthRangef));
    exports->Set(NanNew<v8::String>("clearDepthf"), NanNew<v8::FunctionTemplate>(EXPORT_clearDepthf));
    exports->Set(NanNew<v8::String>("getProgramBinary"), NanNew<v8::FunctionTemplate>(EXPORT_getProgramBinary));
    exports->Set(NanNew<v8::String>("programBinary"), NanNew<v8::FunctionTemplate>(EXPORT_programBinary));
    exports->Set(NanNew<v8::String>("programParameteri"), NanNew<v8::FunctionTemplate>(EXPORT_programParameteri));
    exports->Set(NanNew<v8::String>("useProgramStages"), NanNew<v8::FunctionTemplate>(EXPORT_useProgramStages));
    exports->Set(NanNew<v8::String>("activeShaderProgram"), NanNew<v8::FunctionTemplate>(EXPORT_activeShaderProgram));
    exports->Set(NanNew<v8::String>("bindProgramPipeline"), NanNew<v8::FunctionTemplate>(EXPORT_bindProgramPipeline));
    exports->Set(NanNew<v8::String>("genProgramPipelines"), NanNew<v8::FunctionTemplate>(EXPORT_genProgramPipelines));
    exports->Set(NanNew<v8::String>("isProgramPipeline"), NanNew<v8::FunctionTemplate>(EXPORT_isProgramPipeline));
    exports->Set(NanNew<v8::String>("getProgramPipelineiv"), NanNew<v8::FunctionTemplate>(EXPORT_getProgramPipelineiv));
    exports->Set(NanNew<v8::String>("programUniform1i"), NanNew<v8::FunctionTemplate>(EXPORT_programUniform1i));
    exports->Set(NanNew<v8::String>("programUniform1iv"), NanNew<v8::FunctionTemplate>(EXPORT_programUniform1iv));
    exports->Set(NanNew<v8::String>("programUniform1f"), NanNew<v8::FunctionTemplate>(EXPORT_programUniform1f));
    exports->Set(NanNew<v8::String>("programUniform1fv"), NanNew<v8::FunctionTemplate>(EXPORT_programUniform1fv));
    exports->Set(NanNew<v8::String>("programUniform1d"), NanNew<v8::FunctionTemplate>(EXPORT_programUniform1d));
    exports->Set(NanNew<v8::String>("programUniform1dv"), NanNew<v8::FunctionTemplate>(EXPORT_programUniform1dv));
    exports->Set(NanNew<v8::String>("programUniform1ui"), NanNew<v8::FunctionTemplate>(EXPORT_programUniform1ui));
    exports->Set(NanNew<v8::String>("programUniform1uiv"), NanNew<v8::FunctionTemplate>(EXPORT_programUniform1uiv));
    exports->Set(NanNew<v8::String>("programUniform2i"), NanNew<v8::FunctionTemplate>(EXPORT_programUniform2i));
    exports->Set(NanNew<v8::String>("programUniform2iv"), NanNew<v8::FunctionTemplate>(EXPORT_programUniform2iv));
    exports->Set(NanNew<v8::String>("programUniform2f"), NanNew<v8::FunctionTemplate>(EXPORT_programUniform2f));
    exports->Set(NanNew<v8::String>("programUniform2fv"), NanNew<v8::FunctionTemplate>(EXPORT_programUniform2fv));
    exports->Set(NanNew<v8::String>("programUniform2d"), NanNew<v8::FunctionTemplate>(EXPORT_programUniform2d));
    exports->Set(NanNew<v8::String>("programUniform2dv"), NanNew<v8::FunctionTemplate>(EXPORT_programUniform2dv));
    exports->Set(NanNew<v8::String>("programUniform2ui"), NanNew<v8::FunctionTemplate>(EXPORT_programUniform2ui));
    exports->Set(NanNew<v8::String>("programUniform2uiv"), NanNew<v8::FunctionTemplate>(EXPORT_programUniform2uiv));
    exports->Set(NanNew<v8::String>("programUniform3i"), NanNew<v8::FunctionTemplate>(EXPORT_programUniform3i));
    exports->Set(NanNew<v8::String>("programUniform3iv"), NanNew<v8::FunctionTemplate>(EXPORT_programUniform3iv));
    exports->Set(NanNew<v8::String>("programUniform3f"), NanNew<v8::FunctionTemplate>(EXPORT_programUniform3f));
    exports->Set(NanNew<v8::String>("programUniform3fv"), NanNew<v8::FunctionTemplate>(EXPORT_programUniform3fv));
    exports->Set(NanNew<v8::String>("programUniform3d"), NanNew<v8::FunctionTemplate>(EXPORT_programUniform3d));
    exports->Set(NanNew<v8::String>("programUniform3dv"), NanNew<v8::FunctionTemplate>(EXPORT_programUniform3dv));
    exports->Set(NanNew<v8::String>("programUniform3ui"), NanNew<v8::FunctionTemplate>(EXPORT_programUniform3ui));
    exports->Set(NanNew<v8::String>("programUniform3uiv"), NanNew<v8::FunctionTemplate>(EXPORT_programUniform3uiv));
    exports->Set(NanNew<v8::String>("programUniform4i"), NanNew<v8::FunctionTemplate>(EXPORT_programUniform4i));
    exports->Set(NanNew<v8::String>("programUniform4iv"), NanNew<v8::FunctionTemplate>(EXPORT_programUniform4iv));
    exports->Set(NanNew<v8::String>("programUniform4f"), NanNew<v8::FunctionTemplate>(EXPORT_programUniform4f));
    exports->Set(NanNew<v8::String>("programUniform4fv"), NanNew<v8::FunctionTemplate>(EXPORT_programUniform4fv));
    exports->Set(NanNew<v8::String>("programUniform4d"), NanNew<v8::FunctionTemplate>(EXPORT_programUniform4d));
    exports->Set(NanNew<v8::String>("programUniform4dv"), NanNew<v8::FunctionTemplate>(EXPORT_programUniform4dv));
    exports->Set(NanNew<v8::String>("programUniform4ui"), NanNew<v8::FunctionTemplate>(EXPORT_programUniform4ui));
    exports->Set(NanNew<v8::String>("programUniform4uiv"), NanNew<v8::FunctionTemplate>(EXPORT_programUniform4uiv));
    exports->Set(NanNew<v8::String>("programUniformMatrix2fv"), NanNew<v8::FunctionTemplate>(EXPORT_programUniformMatrix2fv));
    exports->Set(NanNew<v8::String>("programUniformMatrix3fv"), NanNew<v8::FunctionTemplate>(EXPORT_programUniformMatrix3fv));
    exports->Set(NanNew<v8::String>("programUniformMatrix4fv"), NanNew<v8::FunctionTemplate>(EXPORT_programUniformMatrix4fv));
    exports->Set(NanNew<v8::String>("programUniformMatrix2dv"), NanNew<v8::FunctionTemplate>(EXPORT_programUniformMatrix2dv));
    exports->Set(NanNew<v8::String>("programUniformMatrix3dv"), NanNew<v8::FunctionTemplate>(EXPORT_programUniformMatrix3dv));
    exports->Set(NanNew<v8::String>("programUniformMatrix4dv"), NanNew<v8::FunctionTemplate>(EXPORT_programUniformMatrix4dv));
    exports->Set(NanNew<v8::String>("programUniformMatrix2x3fv"), NanNew<v8::FunctionTemplate>(EXPORT_programUniformMatrix2x3fv));
    exports->Set(NanNew<v8::String>("programUniformMatrix3x2fv"), NanNew<v8::FunctionTemplate>(EXPORT_programUniformMatrix3x2fv));
    exports->Set(NanNew<v8::String>("programUniformMatrix2x4fv"), NanNew<v8::FunctionTemplate>(EXPORT_programUniformMatrix2x4fv));
    exports->Set(NanNew<v8::String>("programUniformMatrix4x2fv"), NanNew<v8::FunctionTemplate>(EXPORT_programUniformMatrix4x2fv));
    exports->Set(NanNew<v8::String>("programUniformMatrix3x4fv"), NanNew<v8::FunctionTemplate>(EXPORT_programUniformMatrix3x4fv));
    exports->Set(NanNew<v8::String>("programUniformMatrix4x3fv"), NanNew<v8::FunctionTemplate>(EXPORT_programUniformMatrix4x3fv));
    exports->Set(NanNew<v8::String>("programUniformMatrix2x3dv"), NanNew<v8::FunctionTemplate>(EXPORT_programUniformMatrix2x3dv));
    exports->Set(NanNew<v8::String>("programUniformMatrix3x2dv"), NanNew<v8::FunctionTemplate>(EXPORT_programUniformMatrix3x2dv));
    exports->Set(NanNew<v8::String>("programUniformMatrix2x4dv"), NanNew<v8::FunctionTemplate>(EXPORT_programUniformMatrix2x4dv));
    exports->Set(NanNew<v8::String>("programUniformMatrix4x2dv"), NanNew<v8::FunctionTemplate>(EXPORT_programUniformMatrix4x2dv));
    exports->Set(NanNew<v8::String>("programUniformMatrix3x4dv"), NanNew<v8::FunctionTemplate>(EXPORT_programUniformMatrix3x4dv));
    exports->Set(NanNew<v8::String>("programUniformMatrix4x3dv"), NanNew<v8::FunctionTemplate>(EXPORT_programUniformMatrix4x3dv));
    exports->Set(NanNew<v8::String>("validateProgramPipeline"), NanNew<v8::FunctionTemplate>(EXPORT_validateProgramPipeline));
    exports->Set(NanNew<v8::String>("getProgramPipelineInfoLog"), NanNew<v8::FunctionTemplate>(EXPORT_getProgramPipelineInfoLog));
    exports->Set(NanNew<v8::String>("vertexAttribL1d"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttribL1d));
    exports->Set(NanNew<v8::String>("vertexAttribL2d"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttribL2d));
    exports->Set(NanNew<v8::String>("vertexAttribL3d"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttribL3d));
    exports->Set(NanNew<v8::String>("vertexAttribL4d"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttribL4d));
    exports->Set(NanNew<v8::String>("vertexAttribL1dv"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttribL1dv));
    exports->Set(NanNew<v8::String>("vertexAttribL2dv"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttribL2dv));
    exports->Set(NanNew<v8::String>("vertexAttribL3dv"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttribL3dv));
    exports->Set(NanNew<v8::String>("vertexAttribL4dv"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttribL4dv));
    exports->Set(NanNew<v8::String>("vertexAttribLPointer"), NanNew<v8::FunctionTemplate>(EXPORT_vertexAttribLPointer));
    exports->Set(NanNew<v8::String>("getVertexAttribLdv"), NanNew<v8::FunctionTemplate>(EXPORT_getVertexAttribLdv));
    exports->Set(NanNew<v8::String>("viewportArrayv"), NanNew<v8::FunctionTemplate>(EXPORT_viewportArrayv));
    exports->Set(NanNew<v8::String>("viewportIndexedf"), NanNew<v8::FunctionTemplate>(EXPORT_viewportIndexedf));
    exports->Set(NanNew<v8::String>("viewportIndexedfv"), NanNew<v8::FunctionTemplate>(EXPORT_viewportIndexedfv));
    exports->Set(NanNew<v8::String>("scissorArrayv"), NanNew<v8::FunctionTemplate>(EXPORT_scissorArrayv));
    exports->Set(NanNew<v8::String>("scissorIndexed"), NanNew<v8::FunctionTemplate>(EXPORT_scissorIndexed));
    exports->Set(NanNew<v8::String>("scissorIndexedv"), NanNew<v8::FunctionTemplate>(EXPORT_scissorIndexedv));
    exports->Set(NanNew<v8::String>("depthRangeArrayv"), NanNew<v8::FunctionTemplate>(EXPORT_depthRangeArrayv));
    exports->Set(NanNew<v8::String>("depthRangeIndexed"), NanNew<v8::FunctionTemplate>(EXPORT_depthRangeIndexed));
    exports->Set(NanNew<v8::String>("getFloati_v"), NanNew<v8::FunctionTemplate>(EXPORT_getFloati_v));
    exports->Set(NanNew<v8::String>("getDoublei_v"), NanNew<v8::FunctionTemplate>(EXPORT_getDoublei_v));
    exports->Set(NanNew<v8::String>("shaderSource"), NanNew<v8::FunctionTemplate>(EXPORT_shaderSource));
}


void gl3BindInit(v8::Handle<v8::Object> exports) {
    NanScope();
    v8::Handle<v8::ObjectTemplate> GL3 = NanNew<v8::ObjectTemplate>();
    GL3->SetInternalFieldCount(1);
    defineConstants(GL3);
    defineFunctions(GL3);
    defineObjects(GL3);
    exports->Set(NanNew<v8::String>("GL3"), GL3->NewInstance());
}
