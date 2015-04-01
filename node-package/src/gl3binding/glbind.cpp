#include "glbind.h"

void do_nothing_release_callback(char*, void*) { }


        class NODE_Sampler : public node::ObjectWrap {
        public:
            static void Init(v8::Handle<v8::ObjectTemplate> exports);
            GLuint gl_handle;

            static v8::Handle<v8::Value> fromGLHandle(GLuint handle) {
                NanScope();
                const int argc = 1;
                v8::Local<v8::Value> argv[argc] = { NanNew<v8::Integer>(handle) };
                NanReturnValue(constructor->NewInstance(argc, argv));
            }

        private:
            explicit NODE_Sampler(GLuint gl_handle_) { gl_handle = gl_handle_; }
            explicit NODE_Sampler() { glGenSamplers(1, &gl_handle); }
            ~NODE_Sampler() { glDeleteSamplers(1, &gl_handle); printf("Release Sampler: %d\n", gl_handle); }

            static v8::Handle<v8::Value> New(const v8::Arguments& args);

            static v8::Handle<v8::Value> NODE_id(const v8::Arguments& args);
            static v8::Handle<v8::Value> NODE_toString(const v8::Arguments& args);

            static v8::Persistent<v8::Function> constructor;
        };

        v8::Persistent<v8::Function> NODE_Sampler::constructor;

        void NODE_Sampler::Init(v8::Handle<v8::ObjectTemplate> exports) {
            v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(New);
            tpl->SetClassName(v8::String::NewSymbol("Sampler"));
            tpl->InstanceTemplate()->SetInternalFieldCount(1);

            // Prototype
            tpl->PrototypeTemplate()->Set(
                v8::String::NewSymbol("id"), v8::FunctionTemplate::New(NODE_id)->GetFunction());
            tpl->PrototypeTemplate()->Set(
                v8::String::NewSymbol("toString"), v8::FunctionTemplate::New(NODE_toString)->GetFunction());
            tpl->PrototypeTemplate()->Set(
                v8::String::NewSymbol("inspect"), v8::FunctionTemplate::New(NODE_toString)->GetFunction());

            constructor = v8::Persistent<v8::Function>::New(tpl->GetFunction());

            // Export constructor.
            exports->Set(v8::String::NewSymbol("Sampler"), constructor);
        }

        v8::Handle<v8::Value> NODE_Sampler::New(const v8::Arguments& args) {
            v8::HandleScope scope;

            if(args.IsConstructCall()) {
                if(args.Length() == 0) {
                    NODE_Sampler* obj = new NODE_Sampler();
                    obj->Wrap(args.This());
                    return args.This();
                } else {
                    NODE_Sampler* obj = new NODE_Sampler(args[0]->Uint32Value());
                    obj->Wrap(args.This());
                    return args.This();
                }
            } else {
                // Invoked as plain function `MyObject(...)`, turn into construct call.
                const int argc = 1;
                v8::Local<v8::Value> argv[argc] = { args[0] };
                return scope.Close(constructor->NewInstance(argc, argv));
            }
        }

        v8::Handle<v8::Value> NODE_Sampler::NODE_id(const v8::Arguments& args) {
            NODE_Sampler* obj = ObjectWrap::Unwrap<NODE_Sampler>(args.This());
            return v8::Uint32::New(obj->gl_handle);
        }

        v8::Handle<v8::Value> NODE_Sampler::NODE_toString(const v8::Arguments& args) {
            NODE_Sampler* obj = ObjectWrap::Unwrap<NODE_Sampler>(args.This());
            char buf[64];
            sprintf(buf, "Sampler:%d", obj->gl_handle);
            return v8::String::New(buf);
        }
        

        class NODE_Framebuffer : public node::ObjectWrap {
        public:
            static void Init(v8::Handle<v8::ObjectTemplate> exports);
            GLuint gl_handle;

            static v8::Handle<v8::Value> fromGLHandle(GLuint handle) {
                NanScope();
                const int argc = 1;
                v8::Local<v8::Value> argv[argc] = { NanNew<v8::Integer>(handle) };
                NanReturnValue(constructor->NewInstance(argc, argv));
            }

        private:
            explicit NODE_Framebuffer(GLuint gl_handle_) { gl_handle = gl_handle_; }
            explicit NODE_Framebuffer() { glGenFramebuffers(1, &gl_handle); }
            ~NODE_Framebuffer() { glDeleteFramebuffers(1, &gl_handle); printf("Release Framebuffer: %d\n", gl_handle); }

            static v8::Handle<v8::Value> New(const v8::Arguments& args);

            static v8::Handle<v8::Value> NODE_id(const v8::Arguments& args);
            static v8::Handle<v8::Value> NODE_toString(const v8::Arguments& args);

            static v8::Persistent<v8::Function> constructor;
        };

        v8::Persistent<v8::Function> NODE_Framebuffer::constructor;

        void NODE_Framebuffer::Init(v8::Handle<v8::ObjectTemplate> exports) {
            v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(New);
            tpl->SetClassName(v8::String::NewSymbol("Framebuffer"));
            tpl->InstanceTemplate()->SetInternalFieldCount(1);

            // Prototype
            tpl->PrototypeTemplate()->Set(
                v8::String::NewSymbol("id"), v8::FunctionTemplate::New(NODE_id)->GetFunction());
            tpl->PrototypeTemplate()->Set(
                v8::String::NewSymbol("toString"), v8::FunctionTemplate::New(NODE_toString)->GetFunction());
            tpl->PrototypeTemplate()->Set(
                v8::String::NewSymbol("inspect"), v8::FunctionTemplate::New(NODE_toString)->GetFunction());

            constructor = v8::Persistent<v8::Function>::New(tpl->GetFunction());

            // Export constructor.
            exports->Set(v8::String::NewSymbol("Framebuffer"), constructor);
        }

        v8::Handle<v8::Value> NODE_Framebuffer::New(const v8::Arguments& args) {
            v8::HandleScope scope;

            if(args.IsConstructCall()) {
                if(args.Length() == 0) {
                    NODE_Framebuffer* obj = new NODE_Framebuffer();
                    obj->Wrap(args.This());
                    return args.This();
                } else {
                    NODE_Framebuffer* obj = new NODE_Framebuffer(args[0]->Uint32Value());
                    obj->Wrap(args.This());
                    return args.This();
                }
            } else {
                // Invoked as plain function `MyObject(...)`, turn into construct call.
                const int argc = 1;
                v8::Local<v8::Value> argv[argc] = { args[0] };
                return scope.Close(constructor->NewInstance(argc, argv));
            }
        }

        v8::Handle<v8::Value> NODE_Framebuffer::NODE_id(const v8::Arguments& args) {
            NODE_Framebuffer* obj = ObjectWrap::Unwrap<NODE_Framebuffer>(args.This());
            return v8::Uint32::New(obj->gl_handle);
        }

        v8::Handle<v8::Value> NODE_Framebuffer::NODE_toString(const v8::Arguments& args) {
            NODE_Framebuffer* obj = ObjectWrap::Unwrap<NODE_Framebuffer>(args.This());
            char buf[64];
            sprintf(buf, "Framebuffer:%d", obj->gl_handle);
            return v8::String::New(buf);
        }
        

        class NODE_Shader : public node::ObjectWrap {
        public:
            static void Init(v8::Handle<v8::ObjectTemplate> exports);
            GLuint gl_handle;

            static v8::Handle<v8::Value> fromGLHandle(GLuint handle) {
                NanScope();
                const int argc = 1;
                v8::Local<v8::Value> argv[argc] = { NanNew<v8::Integer>(handle) };
                NanReturnValue(constructor->NewInstance(argc, argv));
            }

        private:
            explicit NODE_Shader(GLuint gl_handle_) { gl_handle = gl_handle_; }
            explicit NODE_Shader() { gl_handle = 0; }
            ~NODE_Shader() { glDeleteShader(gl_handle); printf("Release Shader: %d\n", gl_handle); }

            static v8::Handle<v8::Value> New(const v8::Arguments& args);

            static v8::Handle<v8::Value> NODE_id(const v8::Arguments& args);
            static v8::Handle<v8::Value> NODE_toString(const v8::Arguments& args);

            static v8::Persistent<v8::Function> constructor;
        };

        v8::Persistent<v8::Function> NODE_Shader::constructor;

        void NODE_Shader::Init(v8::Handle<v8::ObjectTemplate> exports) {
            v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(New);
            tpl->SetClassName(v8::String::NewSymbol("Shader"));
            tpl->InstanceTemplate()->SetInternalFieldCount(1);

            // Prototype
            tpl->PrototypeTemplate()->Set(
                v8::String::NewSymbol("id"), v8::FunctionTemplate::New(NODE_id)->GetFunction());
            tpl->PrototypeTemplate()->Set(
                v8::String::NewSymbol("toString"), v8::FunctionTemplate::New(NODE_toString)->GetFunction());
            tpl->PrototypeTemplate()->Set(
                v8::String::NewSymbol("inspect"), v8::FunctionTemplate::New(NODE_toString)->GetFunction());

            constructor = v8::Persistent<v8::Function>::New(tpl->GetFunction());

            // Export constructor.
            exports->Set(v8::String::NewSymbol("Shader"), constructor);
        }

        v8::Handle<v8::Value> NODE_Shader::New(const v8::Arguments& args) {
            v8::HandleScope scope;

            if(args.IsConstructCall()) {
                if(args.Length() == 0) {
                    NODE_Shader* obj = new NODE_Shader();
                    obj->Wrap(args.This());
                    return args.This();
                } else {
                    NODE_Shader* obj = new NODE_Shader(args[0]->Uint32Value());
                    obj->Wrap(args.This());
                    return args.This();
                }
            } else {
                // Invoked as plain function `MyObject(...)`, turn into construct call.
                const int argc = 1;
                v8::Local<v8::Value> argv[argc] = { args[0] };
                return scope.Close(constructor->NewInstance(argc, argv));
            }
        }

        v8::Handle<v8::Value> NODE_Shader::NODE_id(const v8::Arguments& args) {
            NODE_Shader* obj = ObjectWrap::Unwrap<NODE_Shader>(args.This());
            return v8::Uint32::New(obj->gl_handle);
        }

        v8::Handle<v8::Value> NODE_Shader::NODE_toString(const v8::Arguments& args) {
            NODE_Shader* obj = ObjectWrap::Unwrap<NODE_Shader>(args.This());
            char buf[64];
            sprintf(buf, "Shader:%d", obj->gl_handle);
            return v8::String::New(buf);
        }
        

        class NODE_Texture : public node::ObjectWrap {
        public:
            static void Init(v8::Handle<v8::ObjectTemplate> exports);
            GLuint gl_handle;

            static v8::Handle<v8::Value> fromGLHandle(GLuint handle) {
                NanScope();
                const int argc = 1;
                v8::Local<v8::Value> argv[argc] = { NanNew<v8::Integer>(handle) };
                NanReturnValue(constructor->NewInstance(argc, argv));
            }

        private:
            explicit NODE_Texture(GLuint gl_handle_) { gl_handle = gl_handle_; }
            explicit NODE_Texture() { glGenTextures(1, &gl_handle); }
            ~NODE_Texture() { glDeleteTextures(1, &gl_handle); printf("Release Texture: %d\n", gl_handle); }

            static v8::Handle<v8::Value> New(const v8::Arguments& args);

            static v8::Handle<v8::Value> NODE_id(const v8::Arguments& args);
            static v8::Handle<v8::Value> NODE_toString(const v8::Arguments& args);

            static v8::Persistent<v8::Function> constructor;
        };

        v8::Persistent<v8::Function> NODE_Texture::constructor;

        void NODE_Texture::Init(v8::Handle<v8::ObjectTemplate> exports) {
            v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(New);
            tpl->SetClassName(v8::String::NewSymbol("Texture"));
            tpl->InstanceTemplate()->SetInternalFieldCount(1);

            // Prototype
            tpl->PrototypeTemplate()->Set(
                v8::String::NewSymbol("id"), v8::FunctionTemplate::New(NODE_id)->GetFunction());
            tpl->PrototypeTemplate()->Set(
                v8::String::NewSymbol("toString"), v8::FunctionTemplate::New(NODE_toString)->GetFunction());
            tpl->PrototypeTemplate()->Set(
                v8::String::NewSymbol("inspect"), v8::FunctionTemplate::New(NODE_toString)->GetFunction());

            constructor = v8::Persistent<v8::Function>::New(tpl->GetFunction());

            // Export constructor.
            exports->Set(v8::String::NewSymbol("Texture"), constructor);
        }

        v8::Handle<v8::Value> NODE_Texture::New(const v8::Arguments& args) {
            v8::HandleScope scope;

            if(args.IsConstructCall()) {
                if(args.Length() == 0) {
                    NODE_Texture* obj = new NODE_Texture();
                    obj->Wrap(args.This());
                    return args.This();
                } else {
                    NODE_Texture* obj = new NODE_Texture(args[0]->Uint32Value());
                    obj->Wrap(args.This());
                    return args.This();
                }
            } else {
                // Invoked as plain function `MyObject(...)`, turn into construct call.
                const int argc = 1;
                v8::Local<v8::Value> argv[argc] = { args[0] };
                return scope.Close(constructor->NewInstance(argc, argv));
            }
        }

        v8::Handle<v8::Value> NODE_Texture::NODE_id(const v8::Arguments& args) {
            NODE_Texture* obj = ObjectWrap::Unwrap<NODE_Texture>(args.This());
            return v8::Uint32::New(obj->gl_handle);
        }

        v8::Handle<v8::Value> NODE_Texture::NODE_toString(const v8::Arguments& args) {
            NODE_Texture* obj = ObjectWrap::Unwrap<NODE_Texture>(args.This());
            char buf[64];
            sprintf(buf, "Texture:%d", obj->gl_handle);
            return v8::String::New(buf);
        }
        

        class NODE_ProgramPipeline : public node::ObjectWrap {
        public:
            static void Init(v8::Handle<v8::ObjectTemplate> exports);
            GLuint gl_handle;

            static v8::Handle<v8::Value> fromGLHandle(GLuint handle) {
                NanScope();
                const int argc = 1;
                v8::Local<v8::Value> argv[argc] = { NanNew<v8::Integer>(handle) };
                NanReturnValue(constructor->NewInstance(argc, argv));
            }

        private:
            explicit NODE_ProgramPipeline(GLuint gl_handle_) { gl_handle = gl_handle_; }
            explicit NODE_ProgramPipeline() { glGenProgramPipelines(1, &gl_handle); }
            ~NODE_ProgramPipeline() { glDeleteProgramPipelines(1, &gl_handle); printf("Release ProgramPipeline: %d\n", gl_handle); }

            static v8::Handle<v8::Value> New(const v8::Arguments& args);

            static v8::Handle<v8::Value> NODE_id(const v8::Arguments& args);
            static v8::Handle<v8::Value> NODE_toString(const v8::Arguments& args);

            static v8::Persistent<v8::Function> constructor;
        };

        v8::Persistent<v8::Function> NODE_ProgramPipeline::constructor;

        void NODE_ProgramPipeline::Init(v8::Handle<v8::ObjectTemplate> exports) {
            v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(New);
            tpl->SetClassName(v8::String::NewSymbol("ProgramPipeline"));
            tpl->InstanceTemplate()->SetInternalFieldCount(1);

            // Prototype
            tpl->PrototypeTemplate()->Set(
                v8::String::NewSymbol("id"), v8::FunctionTemplate::New(NODE_id)->GetFunction());
            tpl->PrototypeTemplate()->Set(
                v8::String::NewSymbol("toString"), v8::FunctionTemplate::New(NODE_toString)->GetFunction());
            tpl->PrototypeTemplate()->Set(
                v8::String::NewSymbol("inspect"), v8::FunctionTemplate::New(NODE_toString)->GetFunction());

            constructor = v8::Persistent<v8::Function>::New(tpl->GetFunction());

            // Export constructor.
            exports->Set(v8::String::NewSymbol("ProgramPipeline"), constructor);
        }

        v8::Handle<v8::Value> NODE_ProgramPipeline::New(const v8::Arguments& args) {
            v8::HandleScope scope;

            if(args.IsConstructCall()) {
                if(args.Length() == 0) {
                    NODE_ProgramPipeline* obj = new NODE_ProgramPipeline();
                    obj->Wrap(args.This());
                    return args.This();
                } else {
                    NODE_ProgramPipeline* obj = new NODE_ProgramPipeline(args[0]->Uint32Value());
                    obj->Wrap(args.This());
                    return args.This();
                }
            } else {
                // Invoked as plain function `MyObject(...)`, turn into construct call.
                const int argc = 1;
                v8::Local<v8::Value> argv[argc] = { args[0] };
                return scope.Close(constructor->NewInstance(argc, argv));
            }
        }

        v8::Handle<v8::Value> NODE_ProgramPipeline::NODE_id(const v8::Arguments& args) {
            NODE_ProgramPipeline* obj = ObjectWrap::Unwrap<NODE_ProgramPipeline>(args.This());
            return v8::Uint32::New(obj->gl_handle);
        }

        v8::Handle<v8::Value> NODE_ProgramPipeline::NODE_toString(const v8::Arguments& args) {
            NODE_ProgramPipeline* obj = ObjectWrap::Unwrap<NODE_ProgramPipeline>(args.This());
            char buf[64];
            sprintf(buf, "ProgramPipeline:%d", obj->gl_handle);
            return v8::String::New(buf);
        }
        

        class NODE_Renderbuffer : public node::ObjectWrap {
        public:
            static void Init(v8::Handle<v8::ObjectTemplate> exports);
            GLuint gl_handle;

            static v8::Handle<v8::Value> fromGLHandle(GLuint handle) {
                NanScope();
                const int argc = 1;
                v8::Local<v8::Value> argv[argc] = { NanNew<v8::Integer>(handle) };
                NanReturnValue(constructor->NewInstance(argc, argv));
            }

        private:
            explicit NODE_Renderbuffer(GLuint gl_handle_) { gl_handle = gl_handle_; }
            explicit NODE_Renderbuffer() { glGenRenderbuffers(1, &gl_handle); }
            ~NODE_Renderbuffer() { glDeleteRenderbuffers(1, &gl_handle); printf("Release Renderbuffer: %d\n", gl_handle); }

            static v8::Handle<v8::Value> New(const v8::Arguments& args);

            static v8::Handle<v8::Value> NODE_id(const v8::Arguments& args);
            static v8::Handle<v8::Value> NODE_toString(const v8::Arguments& args);

            static v8::Persistent<v8::Function> constructor;
        };

        v8::Persistent<v8::Function> NODE_Renderbuffer::constructor;

        void NODE_Renderbuffer::Init(v8::Handle<v8::ObjectTemplate> exports) {
            v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(New);
            tpl->SetClassName(v8::String::NewSymbol("Renderbuffer"));
            tpl->InstanceTemplate()->SetInternalFieldCount(1);

            // Prototype
            tpl->PrototypeTemplate()->Set(
                v8::String::NewSymbol("id"), v8::FunctionTemplate::New(NODE_id)->GetFunction());
            tpl->PrototypeTemplate()->Set(
                v8::String::NewSymbol("toString"), v8::FunctionTemplate::New(NODE_toString)->GetFunction());
            tpl->PrototypeTemplate()->Set(
                v8::String::NewSymbol("inspect"), v8::FunctionTemplate::New(NODE_toString)->GetFunction());

            constructor = v8::Persistent<v8::Function>::New(tpl->GetFunction());

            // Export constructor.
            exports->Set(v8::String::NewSymbol("Renderbuffer"), constructor);
        }

        v8::Handle<v8::Value> NODE_Renderbuffer::New(const v8::Arguments& args) {
            v8::HandleScope scope;

            if(args.IsConstructCall()) {
                if(args.Length() == 0) {
                    NODE_Renderbuffer* obj = new NODE_Renderbuffer();
                    obj->Wrap(args.This());
                    return args.This();
                } else {
                    NODE_Renderbuffer* obj = new NODE_Renderbuffer(args[0]->Uint32Value());
                    obj->Wrap(args.This());
                    return args.This();
                }
            } else {
                // Invoked as plain function `MyObject(...)`, turn into construct call.
                const int argc = 1;
                v8::Local<v8::Value> argv[argc] = { args[0] };
                return scope.Close(constructor->NewInstance(argc, argv));
            }
        }

        v8::Handle<v8::Value> NODE_Renderbuffer::NODE_id(const v8::Arguments& args) {
            NODE_Renderbuffer* obj = ObjectWrap::Unwrap<NODE_Renderbuffer>(args.This());
            return v8::Uint32::New(obj->gl_handle);
        }

        v8::Handle<v8::Value> NODE_Renderbuffer::NODE_toString(const v8::Arguments& args) {
            NODE_Renderbuffer* obj = ObjectWrap::Unwrap<NODE_Renderbuffer>(args.This());
            char buf[64];
            sprintf(buf, "Renderbuffer:%d", obj->gl_handle);
            return v8::String::New(buf);
        }
        

        class NODE_VertexArray : public node::ObjectWrap {
        public:
            static void Init(v8::Handle<v8::ObjectTemplate> exports);
            GLuint gl_handle;

            static v8::Handle<v8::Value> fromGLHandle(GLuint handle) {
                NanScope();
                const int argc = 1;
                v8::Local<v8::Value> argv[argc] = { NanNew<v8::Integer>(handle) };
                NanReturnValue(constructor->NewInstance(argc, argv));
            }

        private:
            explicit NODE_VertexArray(GLuint gl_handle_) { gl_handle = gl_handle_; }
            explicit NODE_VertexArray() { glGenVertexArrays(1, &gl_handle); }
            ~NODE_VertexArray() { glDeleteVertexArrays(1, &gl_handle); printf("Release VertexArray: %d\n", gl_handle); }

            static v8::Handle<v8::Value> New(const v8::Arguments& args);

            static v8::Handle<v8::Value> NODE_id(const v8::Arguments& args);
            static v8::Handle<v8::Value> NODE_toString(const v8::Arguments& args);

            static v8::Persistent<v8::Function> constructor;
        };

        v8::Persistent<v8::Function> NODE_VertexArray::constructor;

        void NODE_VertexArray::Init(v8::Handle<v8::ObjectTemplate> exports) {
            v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(New);
            tpl->SetClassName(v8::String::NewSymbol("VertexArray"));
            tpl->InstanceTemplate()->SetInternalFieldCount(1);

            // Prototype
            tpl->PrototypeTemplate()->Set(
                v8::String::NewSymbol("id"), v8::FunctionTemplate::New(NODE_id)->GetFunction());
            tpl->PrototypeTemplate()->Set(
                v8::String::NewSymbol("toString"), v8::FunctionTemplate::New(NODE_toString)->GetFunction());
            tpl->PrototypeTemplate()->Set(
                v8::String::NewSymbol("inspect"), v8::FunctionTemplate::New(NODE_toString)->GetFunction());

            constructor = v8::Persistent<v8::Function>::New(tpl->GetFunction());

            // Export constructor.
            exports->Set(v8::String::NewSymbol("VertexArray"), constructor);
        }

        v8::Handle<v8::Value> NODE_VertexArray::New(const v8::Arguments& args) {
            v8::HandleScope scope;

            if(args.IsConstructCall()) {
                if(args.Length() == 0) {
                    NODE_VertexArray* obj = new NODE_VertexArray();
                    obj->Wrap(args.This());
                    return args.This();
                } else {
                    NODE_VertexArray* obj = new NODE_VertexArray(args[0]->Uint32Value());
                    obj->Wrap(args.This());
                    return args.This();
                }
            } else {
                // Invoked as plain function `MyObject(...)`, turn into construct call.
                const int argc = 1;
                v8::Local<v8::Value> argv[argc] = { args[0] };
                return scope.Close(constructor->NewInstance(argc, argv));
            }
        }

        v8::Handle<v8::Value> NODE_VertexArray::NODE_id(const v8::Arguments& args) {
            NODE_VertexArray* obj = ObjectWrap::Unwrap<NODE_VertexArray>(args.This());
            return v8::Uint32::New(obj->gl_handle);
        }

        v8::Handle<v8::Value> NODE_VertexArray::NODE_toString(const v8::Arguments& args) {
            NODE_VertexArray* obj = ObjectWrap::Unwrap<NODE_VertexArray>(args.This());
            char buf[64];
            sprintf(buf, "VertexArray:%d", obj->gl_handle);
            return v8::String::New(buf);
        }
        

        class NODE_Query : public node::ObjectWrap {
        public:
            static void Init(v8::Handle<v8::ObjectTemplate> exports);
            GLuint gl_handle;

            static v8::Handle<v8::Value> fromGLHandle(GLuint handle) {
                NanScope();
                const int argc = 1;
                v8::Local<v8::Value> argv[argc] = { NanNew<v8::Integer>(handle) };
                NanReturnValue(constructor->NewInstance(argc, argv));
            }

        private:
            explicit NODE_Query(GLuint gl_handle_) { gl_handle = gl_handle_; }
            explicit NODE_Query() { glGenQueries(1, &gl_handle); }
            ~NODE_Query() { glDeleteQueries(1, &gl_handle); printf("Release Query: %d\n", gl_handle); }

            static v8::Handle<v8::Value> New(const v8::Arguments& args);

            static v8::Handle<v8::Value> NODE_id(const v8::Arguments& args);
            static v8::Handle<v8::Value> NODE_toString(const v8::Arguments& args);

            static v8::Persistent<v8::Function> constructor;
        };

        v8::Persistent<v8::Function> NODE_Query::constructor;

        void NODE_Query::Init(v8::Handle<v8::ObjectTemplate> exports) {
            v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(New);
            tpl->SetClassName(v8::String::NewSymbol("Query"));
            tpl->InstanceTemplate()->SetInternalFieldCount(1);

            // Prototype
            tpl->PrototypeTemplate()->Set(
                v8::String::NewSymbol("id"), v8::FunctionTemplate::New(NODE_id)->GetFunction());
            tpl->PrototypeTemplate()->Set(
                v8::String::NewSymbol("toString"), v8::FunctionTemplate::New(NODE_toString)->GetFunction());
            tpl->PrototypeTemplate()->Set(
                v8::String::NewSymbol("inspect"), v8::FunctionTemplate::New(NODE_toString)->GetFunction());

            constructor = v8::Persistent<v8::Function>::New(tpl->GetFunction());

            // Export constructor.
            exports->Set(v8::String::NewSymbol("Query"), constructor);
        }

        v8::Handle<v8::Value> NODE_Query::New(const v8::Arguments& args) {
            v8::HandleScope scope;

            if(args.IsConstructCall()) {
                if(args.Length() == 0) {
                    NODE_Query* obj = new NODE_Query();
                    obj->Wrap(args.This());
                    return args.This();
                } else {
                    NODE_Query* obj = new NODE_Query(args[0]->Uint32Value());
                    obj->Wrap(args.This());
                    return args.This();
                }
            } else {
                // Invoked as plain function `MyObject(...)`, turn into construct call.
                const int argc = 1;
                v8::Local<v8::Value> argv[argc] = { args[0] };
                return scope.Close(constructor->NewInstance(argc, argv));
            }
        }

        v8::Handle<v8::Value> NODE_Query::NODE_id(const v8::Arguments& args) {
            NODE_Query* obj = ObjectWrap::Unwrap<NODE_Query>(args.This());
            return v8::Uint32::New(obj->gl_handle);
        }

        v8::Handle<v8::Value> NODE_Query::NODE_toString(const v8::Arguments& args) {
            NODE_Query* obj = ObjectWrap::Unwrap<NODE_Query>(args.This());
            char buf[64];
            sprintf(buf, "Query:%d", obj->gl_handle);
            return v8::String::New(buf);
        }
        

        class NODE_Buffer : public node::ObjectWrap {
        public:
            static void Init(v8::Handle<v8::ObjectTemplate> exports);
            GLuint gl_handle;

            static v8::Handle<v8::Value> fromGLHandle(GLuint handle) {
                NanScope();
                const int argc = 1;
                v8::Local<v8::Value> argv[argc] = { NanNew<v8::Integer>(handle) };
                NanReturnValue(constructor->NewInstance(argc, argv));
            }

        private:
            explicit NODE_Buffer(GLuint gl_handle_) { gl_handle = gl_handle_; }
            explicit NODE_Buffer() { glGenBuffers(1, &gl_handle); }
            ~NODE_Buffer() { glDeleteBuffers(1, &gl_handle); printf("Release Buffer: %d\n", gl_handle); }

            static v8::Handle<v8::Value> New(const v8::Arguments& args);

            static v8::Handle<v8::Value> NODE_id(const v8::Arguments& args);
            static v8::Handle<v8::Value> NODE_toString(const v8::Arguments& args);

            static v8::Persistent<v8::Function> constructor;
        };

        v8::Persistent<v8::Function> NODE_Buffer::constructor;

        void NODE_Buffer::Init(v8::Handle<v8::ObjectTemplate> exports) {
            v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(New);
            tpl->SetClassName(v8::String::NewSymbol("Buffer"));
            tpl->InstanceTemplate()->SetInternalFieldCount(1);

            // Prototype
            tpl->PrototypeTemplate()->Set(
                v8::String::NewSymbol("id"), v8::FunctionTemplate::New(NODE_id)->GetFunction());
            tpl->PrototypeTemplate()->Set(
                v8::String::NewSymbol("toString"), v8::FunctionTemplate::New(NODE_toString)->GetFunction());
            tpl->PrototypeTemplate()->Set(
                v8::String::NewSymbol("inspect"), v8::FunctionTemplate::New(NODE_toString)->GetFunction());

            constructor = v8::Persistent<v8::Function>::New(tpl->GetFunction());

            // Export constructor.
            exports->Set(v8::String::NewSymbol("Buffer"), constructor);
        }

        v8::Handle<v8::Value> NODE_Buffer::New(const v8::Arguments& args) {
            v8::HandleScope scope;

            if(args.IsConstructCall()) {
                if(args.Length() == 0) {
                    NODE_Buffer* obj = new NODE_Buffer();
                    obj->Wrap(args.This());
                    return args.This();
                } else {
                    NODE_Buffer* obj = new NODE_Buffer(args[0]->Uint32Value());
                    obj->Wrap(args.This());
                    return args.This();
                }
            } else {
                // Invoked as plain function `MyObject(...)`, turn into construct call.
                const int argc = 1;
                v8::Local<v8::Value> argv[argc] = { args[0] };
                return scope.Close(constructor->NewInstance(argc, argv));
            }
        }

        v8::Handle<v8::Value> NODE_Buffer::NODE_id(const v8::Arguments& args) {
            NODE_Buffer* obj = ObjectWrap::Unwrap<NODE_Buffer>(args.This());
            return v8::Uint32::New(obj->gl_handle);
        }

        v8::Handle<v8::Value> NODE_Buffer::NODE_toString(const v8::Arguments& args) {
            NODE_Buffer* obj = ObjectWrap::Unwrap<NODE_Buffer>(args.This());
            char buf[64];
            sprintf(buf, "Buffer:%d", obj->gl_handle);
            return v8::String::New(buf);
        }
        

        class NODE_Program : public node::ObjectWrap {
        public:
            static void Init(v8::Handle<v8::ObjectTemplate> exports);
            GLuint gl_handle;

            static v8::Handle<v8::Value> fromGLHandle(GLuint handle) {
                NanScope();
                const int argc = 1;
                v8::Local<v8::Value> argv[argc] = { NanNew<v8::Integer>(handle) };
                NanReturnValue(constructor->NewInstance(argc, argv));
            }

        private:
            explicit NODE_Program(GLuint gl_handle_) { gl_handle = gl_handle_; }
            explicit NODE_Program() { gl_handle = 0; }
            ~NODE_Program() { glDeleteProgram(gl_handle); printf("Release Program: %d\n", gl_handle); }

            static v8::Handle<v8::Value> New(const v8::Arguments& args);

            static v8::Handle<v8::Value> NODE_id(const v8::Arguments& args);
            static v8::Handle<v8::Value> NODE_toString(const v8::Arguments& args);

            static v8::Persistent<v8::Function> constructor;
        };

        v8::Persistent<v8::Function> NODE_Program::constructor;

        void NODE_Program::Init(v8::Handle<v8::ObjectTemplate> exports) {
            v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(New);
            tpl->SetClassName(v8::String::NewSymbol("Program"));
            tpl->InstanceTemplate()->SetInternalFieldCount(1);

            // Prototype
            tpl->PrototypeTemplate()->Set(
                v8::String::NewSymbol("id"), v8::FunctionTemplate::New(NODE_id)->GetFunction());
            tpl->PrototypeTemplate()->Set(
                v8::String::NewSymbol("toString"), v8::FunctionTemplate::New(NODE_toString)->GetFunction());
            tpl->PrototypeTemplate()->Set(
                v8::String::NewSymbol("inspect"), v8::FunctionTemplate::New(NODE_toString)->GetFunction());

            constructor = v8::Persistent<v8::Function>::New(tpl->GetFunction());

            // Export constructor.
            exports->Set(v8::String::NewSymbol("Program"), constructor);
        }

        v8::Handle<v8::Value> NODE_Program::New(const v8::Arguments& args) {
            v8::HandleScope scope;

            if(args.IsConstructCall()) {
                if(args.Length() == 0) {
                    NODE_Program* obj = new NODE_Program();
                    obj->Wrap(args.This());
                    return args.This();
                } else {
                    NODE_Program* obj = new NODE_Program(args[0]->Uint32Value());
                    obj->Wrap(args.This());
                    return args.This();
                }
            } else {
                // Invoked as plain function `MyObject(...)`, turn into construct call.
                const int argc = 1;
                v8::Local<v8::Value> argv[argc] = { args[0] };
                return scope.Close(constructor->NewInstance(argc, argv));
            }
        }

        v8::Handle<v8::Value> NODE_Program::NODE_id(const v8::Arguments& args) {
            NODE_Program* obj = ObjectWrap::Unwrap<NODE_Program>(args.This());
            return v8::Uint32::New(obj->gl_handle);
        }

        v8::Handle<v8::Value> NODE_Program::NODE_toString(const v8::Arguments& args) {
            NODE_Program* obj = ObjectWrap::Unwrap<NODE_Program>(args.This());
            char buf[64];
            sprintf(buf, "Program:%d", obj->gl_handle);
            return v8::String::New(buf);
        }
        

        class NODE_TransformFeedback : public node::ObjectWrap {
        public:
            static void Init(v8::Handle<v8::ObjectTemplate> exports);
            GLuint gl_handle;

            static v8::Handle<v8::Value> fromGLHandle(GLuint handle) {
                NanScope();
                const int argc = 1;
                v8::Local<v8::Value> argv[argc] = { NanNew<v8::Integer>(handle) };
                NanReturnValue(constructor->NewInstance(argc, argv));
            }

        private:
            explicit NODE_TransformFeedback(GLuint gl_handle_) { gl_handle = gl_handle_; }
            explicit NODE_TransformFeedback() { glGenTransformFeedbacks(1, &gl_handle); }
            ~NODE_TransformFeedback() { glDeleteTransformFeedbacks(1, &gl_handle); printf("Release TransformFeedback: %d\n", gl_handle); }

            static v8::Handle<v8::Value> New(const v8::Arguments& args);

            static v8::Handle<v8::Value> NODE_id(const v8::Arguments& args);
            static v8::Handle<v8::Value> NODE_toString(const v8::Arguments& args);

            static v8::Persistent<v8::Function> constructor;
        };

        v8::Persistent<v8::Function> NODE_TransformFeedback::constructor;

        void NODE_TransformFeedback::Init(v8::Handle<v8::ObjectTemplate> exports) {
            v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(New);
            tpl->SetClassName(v8::String::NewSymbol("TransformFeedback"));
            tpl->InstanceTemplate()->SetInternalFieldCount(1);

            // Prototype
            tpl->PrototypeTemplate()->Set(
                v8::String::NewSymbol("id"), v8::FunctionTemplate::New(NODE_id)->GetFunction());
            tpl->PrototypeTemplate()->Set(
                v8::String::NewSymbol("toString"), v8::FunctionTemplate::New(NODE_toString)->GetFunction());
            tpl->PrototypeTemplate()->Set(
                v8::String::NewSymbol("inspect"), v8::FunctionTemplate::New(NODE_toString)->GetFunction());

            constructor = v8::Persistent<v8::Function>::New(tpl->GetFunction());

            // Export constructor.
            exports->Set(v8::String::NewSymbol("TransformFeedback"), constructor);
        }

        v8::Handle<v8::Value> NODE_TransformFeedback::New(const v8::Arguments& args) {
            v8::HandleScope scope;

            if(args.IsConstructCall()) {
                if(args.Length() == 0) {
                    NODE_TransformFeedback* obj = new NODE_TransformFeedback();
                    obj->Wrap(args.This());
                    return args.This();
                } else {
                    NODE_TransformFeedback* obj = new NODE_TransformFeedback(args[0]->Uint32Value());
                    obj->Wrap(args.This());
                    return args.This();
                }
            } else {
                // Invoked as plain function `MyObject(...)`, turn into construct call.
                const int argc = 1;
                v8::Local<v8::Value> argv[argc] = { args[0] };
                return scope.Close(constructor->NewInstance(argc, argv));
            }
        }

        v8::Handle<v8::Value> NODE_TransformFeedback::NODE_id(const v8::Arguments& args) {
            NODE_TransformFeedback* obj = ObjectWrap::Unwrap<NODE_TransformFeedback>(args.This());
            return v8::Uint32::New(obj->gl_handle);
        }

        v8::Handle<v8::Value> NODE_TransformFeedback::NODE_toString(const v8::Arguments& args) {
            NODE_TransformFeedback* obj = ObjectWrap::Unwrap<NODE_TransformFeedback>(args.This());
            char buf[64];
            sprintf(buf, "TransformFeedback:%d", obj->gl_handle);
            return v8::String::New(buf);
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
    exports->Set(v8::String::NewSymbol("DEPTH_BUFFER_BIT"), v8::Uint32::New(GL_DEPTH_BUFFER_BIT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("STENCIL_BUFFER_BIT"), v8::Uint32::New(GL_STENCIL_BUFFER_BIT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("COLOR_BUFFER_BIT"), v8::Uint32::New(GL_COLOR_BUFFER_BIT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("FALSE"), v8::Uint32::New(GL_FALSE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TRUE"), v8::Uint32::New(GL_TRUE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("POINTS"), v8::Uint32::New(GL_POINTS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("LINES"), v8::Uint32::New(GL_LINES), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("LINE_LOOP"), v8::Uint32::New(GL_LINE_LOOP), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("LINE_STRIP"), v8::Uint32::New(GL_LINE_STRIP), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TRIANGLES"), v8::Uint32::New(GL_TRIANGLES), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TRIANGLE_STRIP"), v8::Uint32::New(GL_TRIANGLE_STRIP), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TRIANGLE_FAN"), v8::Uint32::New(GL_TRIANGLE_FAN), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("NEVER"), v8::Uint32::New(GL_NEVER), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("LESS"), v8::Uint32::New(GL_LESS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("EQUAL"), v8::Uint32::New(GL_EQUAL), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("LEQUAL"), v8::Uint32::New(GL_LEQUAL), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("GREATER"), v8::Uint32::New(GL_GREATER), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("NOTEQUAL"), v8::Uint32::New(GL_NOTEQUAL), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("GEQUAL"), v8::Uint32::New(GL_GEQUAL), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("ALWAYS"), v8::Uint32::New(GL_ALWAYS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("ZERO"), v8::Uint32::New(GL_ZERO), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("ONE"), v8::Uint32::New(GL_ONE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("SRC_COLOR"), v8::Uint32::New(GL_SRC_COLOR), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("ONE_MINUS_SRC_COLOR"), v8::Uint32::New(GL_ONE_MINUS_SRC_COLOR), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("SRC_ALPHA"), v8::Uint32::New(GL_SRC_ALPHA), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("ONE_MINUS_SRC_ALPHA"), v8::Uint32::New(GL_ONE_MINUS_SRC_ALPHA), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("DST_ALPHA"), v8::Uint32::New(GL_DST_ALPHA), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("ONE_MINUS_DST_ALPHA"), v8::Uint32::New(GL_ONE_MINUS_DST_ALPHA), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("DST_COLOR"), v8::Uint32::New(GL_DST_COLOR), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("ONE_MINUS_DST_COLOR"), v8::Uint32::New(GL_ONE_MINUS_DST_COLOR), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("SRC_ALPHA_SATURATE"), v8::Uint32::New(GL_SRC_ALPHA_SATURATE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("NONE"), v8::Uint32::New(GL_NONE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("FRONT_LEFT"), v8::Uint32::New(GL_FRONT_LEFT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("FRONT_RIGHT"), v8::Uint32::New(GL_FRONT_RIGHT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("BACK_LEFT"), v8::Uint32::New(GL_BACK_LEFT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("BACK_RIGHT"), v8::Uint32::New(GL_BACK_RIGHT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("FRONT"), v8::Uint32::New(GL_FRONT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("BACK"), v8::Uint32::New(GL_BACK), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("LEFT"), v8::Uint32::New(GL_LEFT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RIGHT"), v8::Uint32::New(GL_RIGHT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("FRONT_AND_BACK"), v8::Uint32::New(GL_FRONT_AND_BACK), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("NO_ERROR"), v8::Uint32::New(GL_NO_ERROR), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("INVALID_ENUM"), v8::Uint32::New(GL_INVALID_ENUM), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("INVALID_VALUE"), v8::Uint32::New(GL_INVALID_VALUE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("INVALID_OPERATION"), v8::Uint32::New(GL_INVALID_OPERATION), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("OUT_OF_MEMORY"), v8::Uint32::New(GL_OUT_OF_MEMORY), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("CW"), v8::Uint32::New(GL_CW), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("CCW"), v8::Uint32::New(GL_CCW), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("POINT_SIZE"), v8::Uint32::New(GL_POINT_SIZE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("POINT_SIZE_RANGE"), v8::Uint32::New(GL_POINT_SIZE_RANGE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("POINT_SIZE_GRANULARITY"), v8::Uint32::New(GL_POINT_SIZE_GRANULARITY), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("LINE_SMOOTH"), v8::Uint32::New(GL_LINE_SMOOTH), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("LINE_WIDTH"), v8::Uint32::New(GL_LINE_WIDTH), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("LINE_WIDTH_RANGE"), v8::Uint32::New(GL_LINE_WIDTH_RANGE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("LINE_WIDTH_GRANULARITY"), v8::Uint32::New(GL_LINE_WIDTH_GRANULARITY), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("POLYGON_MODE"), v8::Uint32::New(GL_POLYGON_MODE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("POLYGON_SMOOTH"), v8::Uint32::New(GL_POLYGON_SMOOTH), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("CULL_FACE"), v8::Uint32::New(GL_CULL_FACE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("CULL_FACE_MODE"), v8::Uint32::New(GL_CULL_FACE_MODE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("FRONT_FACE"), v8::Uint32::New(GL_FRONT_FACE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("DEPTH_RANGE"), v8::Uint32::New(GL_DEPTH_RANGE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("DEPTH_TEST"), v8::Uint32::New(GL_DEPTH_TEST), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("DEPTH_WRITEMASK"), v8::Uint32::New(GL_DEPTH_WRITEMASK), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("DEPTH_CLEAR_VALUE"), v8::Uint32::New(GL_DEPTH_CLEAR_VALUE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("DEPTH_FUNC"), v8::Uint32::New(GL_DEPTH_FUNC), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("STENCIL_TEST"), v8::Uint32::New(GL_STENCIL_TEST), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("STENCIL_CLEAR_VALUE"), v8::Uint32::New(GL_STENCIL_CLEAR_VALUE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("STENCIL_FUNC"), v8::Uint32::New(GL_STENCIL_FUNC), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("STENCIL_VALUE_MASK"), v8::Uint32::New(GL_STENCIL_VALUE_MASK), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("STENCIL_FAIL"), v8::Uint32::New(GL_STENCIL_FAIL), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("STENCIL_PASS_DEPTH_FAIL"), v8::Uint32::New(GL_STENCIL_PASS_DEPTH_FAIL), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("STENCIL_PASS_DEPTH_PASS"), v8::Uint32::New(GL_STENCIL_PASS_DEPTH_PASS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("STENCIL_REF"), v8::Uint32::New(GL_STENCIL_REF), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("STENCIL_WRITEMASK"), v8::Uint32::New(GL_STENCIL_WRITEMASK), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("VIEWPORT"), v8::Uint32::New(GL_VIEWPORT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("DITHER"), v8::Uint32::New(GL_DITHER), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("BLEND_DST"), v8::Uint32::New(GL_BLEND_DST), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("BLEND_SRC"), v8::Uint32::New(GL_BLEND_SRC), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("BLEND"), v8::Uint32::New(GL_BLEND), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("LOGIC_OP_MODE"), v8::Uint32::New(GL_LOGIC_OP_MODE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("COLOR_LOGIC_OP"), v8::Uint32::New(GL_COLOR_LOGIC_OP), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("DRAW_BUFFER"), v8::Uint32::New(GL_DRAW_BUFFER), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("READ_BUFFER"), v8::Uint32::New(GL_READ_BUFFER), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("SCISSOR_BOX"), v8::Uint32::New(GL_SCISSOR_BOX), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("SCISSOR_TEST"), v8::Uint32::New(GL_SCISSOR_TEST), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("COLOR_CLEAR_VALUE"), v8::Uint32::New(GL_COLOR_CLEAR_VALUE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("COLOR_WRITEMASK"), v8::Uint32::New(GL_COLOR_WRITEMASK), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("DOUBLEBUFFER"), v8::Uint32::New(GL_DOUBLEBUFFER), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("STEREO"), v8::Uint32::New(GL_STEREO), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("LINE_SMOOTH_HINT"), v8::Uint32::New(GL_LINE_SMOOTH_HINT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("POLYGON_SMOOTH_HINT"), v8::Uint32::New(GL_POLYGON_SMOOTH_HINT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNPACK_SWAP_BYTES"), v8::Uint32::New(GL_UNPACK_SWAP_BYTES), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNPACK_LSB_FIRST"), v8::Uint32::New(GL_UNPACK_LSB_FIRST), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNPACK_ROW_LENGTH"), v8::Uint32::New(GL_UNPACK_ROW_LENGTH), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNPACK_SKIP_ROWS"), v8::Uint32::New(GL_UNPACK_SKIP_ROWS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNPACK_SKIP_PIXELS"), v8::Uint32::New(GL_UNPACK_SKIP_PIXELS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNPACK_ALIGNMENT"), v8::Uint32::New(GL_UNPACK_ALIGNMENT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("PACK_SWAP_BYTES"), v8::Uint32::New(GL_PACK_SWAP_BYTES), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("PACK_LSB_FIRST"), v8::Uint32::New(GL_PACK_LSB_FIRST), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("PACK_ROW_LENGTH"), v8::Uint32::New(GL_PACK_ROW_LENGTH), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("PACK_SKIP_ROWS"), v8::Uint32::New(GL_PACK_SKIP_ROWS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("PACK_SKIP_PIXELS"), v8::Uint32::New(GL_PACK_SKIP_PIXELS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("PACK_ALIGNMENT"), v8::Uint32::New(GL_PACK_ALIGNMENT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_TEXTURE_SIZE"), v8::Uint32::New(GL_MAX_TEXTURE_SIZE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_VIEWPORT_DIMS"), v8::Uint32::New(GL_MAX_VIEWPORT_DIMS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("SUBPIXEL_BITS"), v8::Uint32::New(GL_SUBPIXEL_BITS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_1D"), v8::Uint32::New(GL_TEXTURE_1D), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_2D"), v8::Uint32::New(GL_TEXTURE_2D), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("POLYGON_OFFSET_UNITS"), v8::Uint32::New(GL_POLYGON_OFFSET_UNITS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("POLYGON_OFFSET_POINT"), v8::Uint32::New(GL_POLYGON_OFFSET_POINT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("POLYGON_OFFSET_LINE"), v8::Uint32::New(GL_POLYGON_OFFSET_LINE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("POLYGON_OFFSET_FILL"), v8::Uint32::New(GL_POLYGON_OFFSET_FILL), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("POLYGON_OFFSET_FACTOR"), v8::Uint32::New(GL_POLYGON_OFFSET_FACTOR), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_BINDING_1D"), v8::Uint32::New(GL_TEXTURE_BINDING_1D), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_BINDING_2D"), v8::Uint32::New(GL_TEXTURE_BINDING_2D), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_WIDTH"), v8::Uint32::New(GL_TEXTURE_WIDTH), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_HEIGHT"), v8::Uint32::New(GL_TEXTURE_HEIGHT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_INTERNAL_FORMAT"), v8::Uint32::New(GL_TEXTURE_INTERNAL_FORMAT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_BORDER_COLOR"), v8::Uint32::New(GL_TEXTURE_BORDER_COLOR), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_RED_SIZE"), v8::Uint32::New(GL_TEXTURE_RED_SIZE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_GREEN_SIZE"), v8::Uint32::New(GL_TEXTURE_GREEN_SIZE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_BLUE_SIZE"), v8::Uint32::New(GL_TEXTURE_BLUE_SIZE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_ALPHA_SIZE"), v8::Uint32::New(GL_TEXTURE_ALPHA_SIZE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("DONT_CARE"), v8::Uint32::New(GL_DONT_CARE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("FASTEST"), v8::Uint32::New(GL_FASTEST), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("NICEST"), v8::Uint32::New(GL_NICEST), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("BYTE"), v8::Uint32::New(GL_BYTE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNSIGNED_BYTE"), v8::Uint32::New(GL_UNSIGNED_BYTE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("SHORT"), v8::Uint32::New(GL_SHORT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNSIGNED_SHORT"), v8::Uint32::New(GL_UNSIGNED_SHORT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("INT"), v8::Uint32::New(GL_INT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNSIGNED_INT"), v8::Uint32::New(GL_UNSIGNED_INT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("FLOAT"), v8::Uint32::New(GL_FLOAT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("DOUBLE"), v8::Uint32::New(GL_DOUBLE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("CLEAR"), v8::Uint32::New(GL_CLEAR), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("AND"), v8::Uint32::New(GL_AND), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("AND_REVERSE"), v8::Uint32::New(GL_AND_REVERSE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("COPY"), v8::Uint32::New(GL_COPY), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("AND_INVERTED"), v8::Uint32::New(GL_AND_INVERTED), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("NOOP"), v8::Uint32::New(GL_NOOP), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("XOR"), v8::Uint32::New(GL_XOR), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("OR"), v8::Uint32::New(GL_OR), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("NOR"), v8::Uint32::New(GL_NOR), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("EQUIV"), v8::Uint32::New(GL_EQUIV), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("INVERT"), v8::Uint32::New(GL_INVERT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("OR_REVERSE"), v8::Uint32::New(GL_OR_REVERSE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("COPY_INVERTED"), v8::Uint32::New(GL_COPY_INVERTED), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("OR_INVERTED"), v8::Uint32::New(GL_OR_INVERTED), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("NAND"), v8::Uint32::New(GL_NAND), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("SET"), v8::Uint32::New(GL_SET), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE"), v8::Uint32::New(GL_TEXTURE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("COLOR"), v8::Uint32::New(GL_COLOR), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("DEPTH"), v8::Uint32::New(GL_DEPTH), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("STENCIL"), v8::Uint32::New(GL_STENCIL), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("STENCIL_INDEX"), v8::Uint32::New(GL_STENCIL_INDEX), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("DEPTH_COMPONENT"), v8::Uint32::New(GL_DEPTH_COMPONENT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RED"), v8::Uint32::New(GL_RED), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("GREEN"), v8::Uint32::New(GL_GREEN), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("BLUE"), v8::Uint32::New(GL_BLUE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("ALPHA"), v8::Uint32::New(GL_ALPHA), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RGB"), v8::Uint32::New(GL_RGB), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RGBA"), v8::Uint32::New(GL_RGBA), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("POINT"), v8::Uint32::New(GL_POINT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("LINE"), v8::Uint32::New(GL_LINE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("FILL"), v8::Uint32::New(GL_FILL), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("KEEP"), v8::Uint32::New(GL_KEEP), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("REPLACE"), v8::Uint32::New(GL_REPLACE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("INCR"), v8::Uint32::New(GL_INCR), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("DECR"), v8::Uint32::New(GL_DECR), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("VENDOR"), v8::Uint32::New(GL_VENDOR), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RENDERER"), v8::Uint32::New(GL_RENDERER), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("VERSION"), v8::Uint32::New(GL_VERSION), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("EXTENSIONS"), v8::Uint32::New(GL_EXTENSIONS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("NEAREST"), v8::Uint32::New(GL_NEAREST), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("LINEAR"), v8::Uint32::New(GL_LINEAR), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("NEAREST_MIPMAP_NEAREST"), v8::Uint32::New(GL_NEAREST_MIPMAP_NEAREST), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("LINEAR_MIPMAP_NEAREST"), v8::Uint32::New(GL_LINEAR_MIPMAP_NEAREST), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("NEAREST_MIPMAP_LINEAR"), v8::Uint32::New(GL_NEAREST_MIPMAP_LINEAR), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("LINEAR_MIPMAP_LINEAR"), v8::Uint32::New(GL_LINEAR_MIPMAP_LINEAR), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_MAG_FILTER"), v8::Uint32::New(GL_TEXTURE_MAG_FILTER), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_MIN_FILTER"), v8::Uint32::New(GL_TEXTURE_MIN_FILTER), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_WRAP_S"), v8::Uint32::New(GL_TEXTURE_WRAP_S), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_WRAP_T"), v8::Uint32::New(GL_TEXTURE_WRAP_T), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("PROXY_TEXTURE_1D"), v8::Uint32::New(GL_PROXY_TEXTURE_1D), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("PROXY_TEXTURE_2D"), v8::Uint32::New(GL_PROXY_TEXTURE_2D), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("REPEAT"), v8::Uint32::New(GL_REPEAT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("R3_G3_B2"), v8::Uint32::New(GL_R3_G3_B2), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RGB4"), v8::Uint32::New(GL_RGB4), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RGB5"), v8::Uint32::New(GL_RGB5), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RGB8"), v8::Uint32::New(GL_RGB8), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RGB10"), v8::Uint32::New(GL_RGB10), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RGB12"), v8::Uint32::New(GL_RGB12), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RGB16"), v8::Uint32::New(GL_RGB16), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RGBA2"), v8::Uint32::New(GL_RGBA2), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RGBA4"), v8::Uint32::New(GL_RGBA4), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RGB5_A1"), v8::Uint32::New(GL_RGB5_A1), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RGBA8"), v8::Uint32::New(GL_RGBA8), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RGB10_A2"), v8::Uint32::New(GL_RGB10_A2), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RGBA12"), v8::Uint32::New(GL_RGBA12), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RGBA16"), v8::Uint32::New(GL_RGBA16), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNSIGNED_BYTE_3_3_2"), v8::Uint32::New(GL_UNSIGNED_BYTE_3_3_2), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNSIGNED_SHORT_4_4_4_4"), v8::Uint32::New(GL_UNSIGNED_SHORT_4_4_4_4), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNSIGNED_SHORT_5_5_5_1"), v8::Uint32::New(GL_UNSIGNED_SHORT_5_5_5_1), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNSIGNED_INT_8_8_8_8"), v8::Uint32::New(GL_UNSIGNED_INT_8_8_8_8), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNSIGNED_INT_10_10_10_2"), v8::Uint32::New(GL_UNSIGNED_INT_10_10_10_2), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_BINDING_3D"), v8::Uint32::New(GL_TEXTURE_BINDING_3D), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("PACK_SKIP_IMAGES"), v8::Uint32::New(GL_PACK_SKIP_IMAGES), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("PACK_IMAGE_HEIGHT"), v8::Uint32::New(GL_PACK_IMAGE_HEIGHT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNPACK_SKIP_IMAGES"), v8::Uint32::New(GL_UNPACK_SKIP_IMAGES), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNPACK_IMAGE_HEIGHT"), v8::Uint32::New(GL_UNPACK_IMAGE_HEIGHT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_3D"), v8::Uint32::New(GL_TEXTURE_3D), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("PROXY_TEXTURE_3D"), v8::Uint32::New(GL_PROXY_TEXTURE_3D), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_DEPTH"), v8::Uint32::New(GL_TEXTURE_DEPTH), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_WRAP_R"), v8::Uint32::New(GL_TEXTURE_WRAP_R), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_3D_TEXTURE_SIZE"), v8::Uint32::New(GL_MAX_3D_TEXTURE_SIZE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNSIGNED_BYTE_2_3_3_REV"), v8::Uint32::New(GL_UNSIGNED_BYTE_2_3_3_REV), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNSIGNED_SHORT_5_6_5"), v8::Uint32::New(GL_UNSIGNED_SHORT_5_6_5), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNSIGNED_SHORT_5_6_5_REV"), v8::Uint32::New(GL_UNSIGNED_SHORT_5_6_5_REV), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNSIGNED_SHORT_4_4_4_4_REV"), v8::Uint32::New(GL_UNSIGNED_SHORT_4_4_4_4_REV), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNSIGNED_SHORT_1_5_5_5_REV"), v8::Uint32::New(GL_UNSIGNED_SHORT_1_5_5_5_REV), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNSIGNED_INT_8_8_8_8_REV"), v8::Uint32::New(GL_UNSIGNED_INT_8_8_8_8_REV), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNSIGNED_INT_2_10_10_10_REV"), v8::Uint32::New(GL_UNSIGNED_INT_2_10_10_10_REV), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("BGR"), v8::Uint32::New(GL_BGR), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("BGRA"), v8::Uint32::New(GL_BGRA), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_ELEMENTS_VERTICES"), v8::Uint32::New(GL_MAX_ELEMENTS_VERTICES), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_ELEMENTS_INDICES"), v8::Uint32::New(GL_MAX_ELEMENTS_INDICES), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("CLAMP_TO_EDGE"), v8::Uint32::New(GL_CLAMP_TO_EDGE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_MIN_LOD"), v8::Uint32::New(GL_TEXTURE_MIN_LOD), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_MAX_LOD"), v8::Uint32::New(GL_TEXTURE_MAX_LOD), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_BASE_LEVEL"), v8::Uint32::New(GL_TEXTURE_BASE_LEVEL), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_MAX_LEVEL"), v8::Uint32::New(GL_TEXTURE_MAX_LEVEL), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("SMOOTH_POINT_SIZE_RANGE"), v8::Uint32::New(GL_SMOOTH_POINT_SIZE_RANGE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("SMOOTH_POINT_SIZE_GRANULARITY"), v8::Uint32::New(GL_SMOOTH_POINT_SIZE_GRANULARITY), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("SMOOTH_LINE_WIDTH_RANGE"), v8::Uint32::New(GL_SMOOTH_LINE_WIDTH_RANGE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("SMOOTH_LINE_WIDTH_GRANULARITY"), v8::Uint32::New(GL_SMOOTH_LINE_WIDTH_GRANULARITY), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("ALIASED_LINE_WIDTH_RANGE"), v8::Uint32::New(GL_ALIASED_LINE_WIDTH_RANGE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("CONSTANT_COLOR"), v8::Uint32::New(GL_CONSTANT_COLOR), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("ONE_MINUS_CONSTANT_COLOR"), v8::Uint32::New(GL_ONE_MINUS_CONSTANT_COLOR), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("CONSTANT_ALPHA"), v8::Uint32::New(GL_CONSTANT_ALPHA), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("ONE_MINUS_CONSTANT_ALPHA"), v8::Uint32::New(GL_ONE_MINUS_CONSTANT_ALPHA), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("BLEND_COLOR"), v8::Uint32::New(GL_BLEND_COLOR), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("FUNC_ADD"), v8::Uint32::New(GL_FUNC_ADD), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MIN"), v8::Uint32::New(GL_MIN), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX"), v8::Uint32::New(GL_MAX), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("BLEND_EQUATION"), v8::Uint32::New(GL_BLEND_EQUATION), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("FUNC_SUBTRACT"), v8::Uint32::New(GL_FUNC_SUBTRACT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("FUNC_REVERSE_SUBTRACT"), v8::Uint32::New(GL_FUNC_REVERSE_SUBTRACT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE0"), v8::Uint32::New(GL_TEXTURE0), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE1"), v8::Uint32::New(GL_TEXTURE1), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE2"), v8::Uint32::New(GL_TEXTURE2), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE3"), v8::Uint32::New(GL_TEXTURE3), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE4"), v8::Uint32::New(GL_TEXTURE4), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE5"), v8::Uint32::New(GL_TEXTURE5), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE6"), v8::Uint32::New(GL_TEXTURE6), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE7"), v8::Uint32::New(GL_TEXTURE7), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE8"), v8::Uint32::New(GL_TEXTURE8), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE9"), v8::Uint32::New(GL_TEXTURE9), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE10"), v8::Uint32::New(GL_TEXTURE10), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE11"), v8::Uint32::New(GL_TEXTURE11), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE12"), v8::Uint32::New(GL_TEXTURE12), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE13"), v8::Uint32::New(GL_TEXTURE13), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE14"), v8::Uint32::New(GL_TEXTURE14), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE15"), v8::Uint32::New(GL_TEXTURE15), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE16"), v8::Uint32::New(GL_TEXTURE16), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE17"), v8::Uint32::New(GL_TEXTURE17), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE18"), v8::Uint32::New(GL_TEXTURE18), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE19"), v8::Uint32::New(GL_TEXTURE19), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE20"), v8::Uint32::New(GL_TEXTURE20), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE21"), v8::Uint32::New(GL_TEXTURE21), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE22"), v8::Uint32::New(GL_TEXTURE22), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE23"), v8::Uint32::New(GL_TEXTURE23), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE24"), v8::Uint32::New(GL_TEXTURE24), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE25"), v8::Uint32::New(GL_TEXTURE25), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE26"), v8::Uint32::New(GL_TEXTURE26), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE27"), v8::Uint32::New(GL_TEXTURE27), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE28"), v8::Uint32::New(GL_TEXTURE28), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE29"), v8::Uint32::New(GL_TEXTURE29), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE30"), v8::Uint32::New(GL_TEXTURE30), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE31"), v8::Uint32::New(GL_TEXTURE31), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("ACTIVE_TEXTURE"), v8::Uint32::New(GL_ACTIVE_TEXTURE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MULTISAMPLE"), v8::Uint32::New(GL_MULTISAMPLE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("SAMPLE_ALPHA_TO_COVERAGE"), v8::Uint32::New(GL_SAMPLE_ALPHA_TO_COVERAGE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("SAMPLE_ALPHA_TO_ONE"), v8::Uint32::New(GL_SAMPLE_ALPHA_TO_ONE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("SAMPLE_COVERAGE"), v8::Uint32::New(GL_SAMPLE_COVERAGE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("SAMPLE_BUFFERS"), v8::Uint32::New(GL_SAMPLE_BUFFERS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("SAMPLES"), v8::Uint32::New(GL_SAMPLES), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("SAMPLE_COVERAGE_VALUE"), v8::Uint32::New(GL_SAMPLE_COVERAGE_VALUE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("SAMPLE_COVERAGE_INVERT"), v8::Uint32::New(GL_SAMPLE_COVERAGE_INVERT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_CUBE_MAP"), v8::Uint32::New(GL_TEXTURE_CUBE_MAP), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_BINDING_CUBE_MAP"), v8::Uint32::New(GL_TEXTURE_BINDING_CUBE_MAP), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_CUBE_MAP_POSITIVE_X"), v8::Uint32::New(GL_TEXTURE_CUBE_MAP_POSITIVE_X), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_CUBE_MAP_NEGATIVE_X"), v8::Uint32::New(GL_TEXTURE_CUBE_MAP_NEGATIVE_X), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_CUBE_MAP_POSITIVE_Y"), v8::Uint32::New(GL_TEXTURE_CUBE_MAP_POSITIVE_Y), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_CUBE_MAP_NEGATIVE_Y"), v8::Uint32::New(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_CUBE_MAP_POSITIVE_Z"), v8::Uint32::New(GL_TEXTURE_CUBE_MAP_POSITIVE_Z), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_CUBE_MAP_NEGATIVE_Z"), v8::Uint32::New(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("PROXY_TEXTURE_CUBE_MAP"), v8::Uint32::New(GL_PROXY_TEXTURE_CUBE_MAP), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_CUBE_MAP_TEXTURE_SIZE"), v8::Uint32::New(GL_MAX_CUBE_MAP_TEXTURE_SIZE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("COMPRESSED_RGB"), v8::Uint32::New(GL_COMPRESSED_RGB), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("COMPRESSED_RGBA"), v8::Uint32::New(GL_COMPRESSED_RGBA), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_COMPRESSION_HINT"), v8::Uint32::New(GL_TEXTURE_COMPRESSION_HINT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_COMPRESSED_IMAGE_SIZE"), v8::Uint32::New(GL_TEXTURE_COMPRESSED_IMAGE_SIZE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_COMPRESSED"), v8::Uint32::New(GL_TEXTURE_COMPRESSED), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("NUM_COMPRESSED_TEXTURE_FORMATS"), v8::Uint32::New(GL_NUM_COMPRESSED_TEXTURE_FORMATS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("COMPRESSED_TEXTURE_FORMATS"), v8::Uint32::New(GL_COMPRESSED_TEXTURE_FORMATS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("CLAMP_TO_BORDER"), v8::Uint32::New(GL_CLAMP_TO_BORDER), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("BLEND_DST_RGB"), v8::Uint32::New(GL_BLEND_DST_RGB), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("BLEND_SRC_RGB"), v8::Uint32::New(GL_BLEND_SRC_RGB), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("BLEND_DST_ALPHA"), v8::Uint32::New(GL_BLEND_DST_ALPHA), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("BLEND_SRC_ALPHA"), v8::Uint32::New(GL_BLEND_SRC_ALPHA), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("POINT_FADE_THRESHOLD_SIZE"), v8::Uint32::New(GL_POINT_FADE_THRESHOLD_SIZE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("DEPTH_COMPONENT16"), v8::Uint32::New(GL_DEPTH_COMPONENT16), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("DEPTH_COMPONENT24"), v8::Uint32::New(GL_DEPTH_COMPONENT24), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("DEPTH_COMPONENT32"), v8::Uint32::New(GL_DEPTH_COMPONENT32), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MIRRORED_REPEAT"), v8::Uint32::New(GL_MIRRORED_REPEAT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_TEXTURE_LOD_BIAS"), v8::Uint32::New(GL_MAX_TEXTURE_LOD_BIAS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_LOD_BIAS"), v8::Uint32::New(GL_TEXTURE_LOD_BIAS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("INCR_WRAP"), v8::Uint32::New(GL_INCR_WRAP), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("DECR_WRAP"), v8::Uint32::New(GL_DECR_WRAP), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_DEPTH_SIZE"), v8::Uint32::New(GL_TEXTURE_DEPTH_SIZE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_COMPARE_MODE"), v8::Uint32::New(GL_TEXTURE_COMPARE_MODE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_COMPARE_FUNC"), v8::Uint32::New(GL_TEXTURE_COMPARE_FUNC), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("BUFFER_SIZE"), v8::Uint32::New(GL_BUFFER_SIZE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("BUFFER_USAGE"), v8::Uint32::New(GL_BUFFER_USAGE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("QUERY_COUNTER_BITS"), v8::Uint32::New(GL_QUERY_COUNTER_BITS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("CURRENT_QUERY"), v8::Uint32::New(GL_CURRENT_QUERY), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("QUERY_RESULT"), v8::Uint32::New(GL_QUERY_RESULT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("QUERY_RESULT_AVAILABLE"), v8::Uint32::New(GL_QUERY_RESULT_AVAILABLE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("ARRAY_BUFFER"), v8::Uint32::New(GL_ARRAY_BUFFER), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("ELEMENT_ARRAY_BUFFER"), v8::Uint32::New(GL_ELEMENT_ARRAY_BUFFER), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("ARRAY_BUFFER_BINDING"), v8::Uint32::New(GL_ARRAY_BUFFER_BINDING), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("ELEMENT_ARRAY_BUFFER_BINDING"), v8::Uint32::New(GL_ELEMENT_ARRAY_BUFFER_BINDING), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("VERTEX_ATTRIB_ARRAY_BUFFER_BINDING"), v8::Uint32::New(GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("READ_ONLY"), v8::Uint32::New(GL_READ_ONLY), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("WRITE_ONLY"), v8::Uint32::New(GL_WRITE_ONLY), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("READ_WRITE"), v8::Uint32::New(GL_READ_WRITE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("BUFFER_ACCESS"), v8::Uint32::New(GL_BUFFER_ACCESS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("BUFFER_MAPPED"), v8::Uint32::New(GL_BUFFER_MAPPED), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("BUFFER_MAP_POINTER"), v8::Uint32::New(GL_BUFFER_MAP_POINTER), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("STREAM_DRAW"), v8::Uint32::New(GL_STREAM_DRAW), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("STREAM_READ"), v8::Uint32::New(GL_STREAM_READ), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("STREAM_COPY"), v8::Uint32::New(GL_STREAM_COPY), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("STATIC_DRAW"), v8::Uint32::New(GL_STATIC_DRAW), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("STATIC_READ"), v8::Uint32::New(GL_STATIC_READ), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("STATIC_COPY"), v8::Uint32::New(GL_STATIC_COPY), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("DYNAMIC_DRAW"), v8::Uint32::New(GL_DYNAMIC_DRAW), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("DYNAMIC_READ"), v8::Uint32::New(GL_DYNAMIC_READ), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("DYNAMIC_COPY"), v8::Uint32::New(GL_DYNAMIC_COPY), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("SAMPLES_PASSED"), v8::Uint32::New(GL_SAMPLES_PASSED), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("BLEND_EQUATION_RGB"), v8::Uint32::New(GL_BLEND_EQUATION_RGB), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("VERTEX_ATTRIB_ARRAY_ENABLED"), v8::Uint32::New(GL_VERTEX_ATTRIB_ARRAY_ENABLED), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("VERTEX_ATTRIB_ARRAY_SIZE"), v8::Uint32::New(GL_VERTEX_ATTRIB_ARRAY_SIZE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("VERTEX_ATTRIB_ARRAY_STRIDE"), v8::Uint32::New(GL_VERTEX_ATTRIB_ARRAY_STRIDE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("VERTEX_ATTRIB_ARRAY_TYPE"), v8::Uint32::New(GL_VERTEX_ATTRIB_ARRAY_TYPE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("CURRENT_VERTEX_ATTRIB"), v8::Uint32::New(GL_CURRENT_VERTEX_ATTRIB), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("VERTEX_PROGRAM_POINT_SIZE"), v8::Uint32::New(GL_VERTEX_PROGRAM_POINT_SIZE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("VERTEX_ATTRIB_ARRAY_POINTER"), v8::Uint32::New(GL_VERTEX_ATTRIB_ARRAY_POINTER), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("STENCIL_BACK_FUNC"), v8::Uint32::New(GL_STENCIL_BACK_FUNC), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("STENCIL_BACK_FAIL"), v8::Uint32::New(GL_STENCIL_BACK_FAIL), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("STENCIL_BACK_PASS_DEPTH_FAIL"), v8::Uint32::New(GL_STENCIL_BACK_PASS_DEPTH_FAIL), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("STENCIL_BACK_PASS_DEPTH_PASS"), v8::Uint32::New(GL_STENCIL_BACK_PASS_DEPTH_PASS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_DRAW_BUFFERS"), v8::Uint32::New(GL_MAX_DRAW_BUFFERS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("DRAW_BUFFER0"), v8::Uint32::New(GL_DRAW_BUFFER0), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("DRAW_BUFFER1"), v8::Uint32::New(GL_DRAW_BUFFER1), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("DRAW_BUFFER2"), v8::Uint32::New(GL_DRAW_BUFFER2), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("DRAW_BUFFER3"), v8::Uint32::New(GL_DRAW_BUFFER3), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("DRAW_BUFFER4"), v8::Uint32::New(GL_DRAW_BUFFER4), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("DRAW_BUFFER5"), v8::Uint32::New(GL_DRAW_BUFFER5), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("DRAW_BUFFER6"), v8::Uint32::New(GL_DRAW_BUFFER6), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("DRAW_BUFFER7"), v8::Uint32::New(GL_DRAW_BUFFER7), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("DRAW_BUFFER8"), v8::Uint32::New(GL_DRAW_BUFFER8), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("DRAW_BUFFER9"), v8::Uint32::New(GL_DRAW_BUFFER9), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("DRAW_BUFFER10"), v8::Uint32::New(GL_DRAW_BUFFER10), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("DRAW_BUFFER11"), v8::Uint32::New(GL_DRAW_BUFFER11), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("DRAW_BUFFER12"), v8::Uint32::New(GL_DRAW_BUFFER12), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("DRAW_BUFFER13"), v8::Uint32::New(GL_DRAW_BUFFER13), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("DRAW_BUFFER14"), v8::Uint32::New(GL_DRAW_BUFFER14), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("DRAW_BUFFER15"), v8::Uint32::New(GL_DRAW_BUFFER15), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("BLEND_EQUATION_ALPHA"), v8::Uint32::New(GL_BLEND_EQUATION_ALPHA), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_VERTEX_ATTRIBS"), v8::Uint32::New(GL_MAX_VERTEX_ATTRIBS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("VERTEX_ATTRIB_ARRAY_NORMALIZED"), v8::Uint32::New(GL_VERTEX_ATTRIB_ARRAY_NORMALIZED), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_TEXTURE_IMAGE_UNITS"), v8::Uint32::New(GL_MAX_TEXTURE_IMAGE_UNITS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("FRAGMENT_SHADER"), v8::Uint32::New(GL_FRAGMENT_SHADER), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("VERTEX_SHADER"), v8::Uint32::New(GL_VERTEX_SHADER), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_FRAGMENT_UNIFORM_COMPONENTS"), v8::Uint32::New(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_VERTEX_UNIFORM_COMPONENTS"), v8::Uint32::New(GL_MAX_VERTEX_UNIFORM_COMPONENTS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_VARYING_FLOATS"), v8::Uint32::New(GL_MAX_VARYING_FLOATS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_VERTEX_TEXTURE_IMAGE_UNITS"), v8::Uint32::New(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_COMBINED_TEXTURE_IMAGE_UNITS"), v8::Uint32::New(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("SHADER_TYPE"), v8::Uint32::New(GL_SHADER_TYPE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("FLOAT_VEC2"), v8::Uint32::New(GL_FLOAT_VEC2), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("FLOAT_VEC3"), v8::Uint32::New(GL_FLOAT_VEC3), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("FLOAT_VEC4"), v8::Uint32::New(GL_FLOAT_VEC4), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("INT_VEC2"), v8::Uint32::New(GL_INT_VEC2), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("INT_VEC3"), v8::Uint32::New(GL_INT_VEC3), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("INT_VEC4"), v8::Uint32::New(GL_INT_VEC4), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("BOOL"), v8::Uint32::New(GL_BOOL), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("BOOL_VEC2"), v8::Uint32::New(GL_BOOL_VEC2), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("BOOL_VEC3"), v8::Uint32::New(GL_BOOL_VEC3), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("BOOL_VEC4"), v8::Uint32::New(GL_BOOL_VEC4), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("FLOAT_MAT2"), v8::Uint32::New(GL_FLOAT_MAT2), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("FLOAT_MAT3"), v8::Uint32::New(GL_FLOAT_MAT3), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("FLOAT_MAT4"), v8::Uint32::New(GL_FLOAT_MAT4), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("SAMPLER_1D"), v8::Uint32::New(GL_SAMPLER_1D), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("SAMPLER_2D"), v8::Uint32::New(GL_SAMPLER_2D), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("SAMPLER_3D"), v8::Uint32::New(GL_SAMPLER_3D), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("SAMPLER_CUBE"), v8::Uint32::New(GL_SAMPLER_CUBE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("SAMPLER_1D_SHADOW"), v8::Uint32::New(GL_SAMPLER_1D_SHADOW), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("SAMPLER_2D_SHADOW"), v8::Uint32::New(GL_SAMPLER_2D_SHADOW), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("DELETE_STATUS"), v8::Uint32::New(GL_DELETE_STATUS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("COMPILE_STATUS"), v8::Uint32::New(GL_COMPILE_STATUS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("LINK_STATUS"), v8::Uint32::New(GL_LINK_STATUS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("VALIDATE_STATUS"), v8::Uint32::New(GL_VALIDATE_STATUS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("INFO_LOG_LENGTH"), v8::Uint32::New(GL_INFO_LOG_LENGTH), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("ATTACHED_SHADERS"), v8::Uint32::New(GL_ATTACHED_SHADERS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("ACTIVE_UNIFORMS"), v8::Uint32::New(GL_ACTIVE_UNIFORMS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("ACTIVE_UNIFORM_MAX_LENGTH"), v8::Uint32::New(GL_ACTIVE_UNIFORM_MAX_LENGTH), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("SHADER_SOURCE_LENGTH"), v8::Uint32::New(GL_SHADER_SOURCE_LENGTH), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("ACTIVE_ATTRIBUTES"), v8::Uint32::New(GL_ACTIVE_ATTRIBUTES), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("ACTIVE_ATTRIBUTE_MAX_LENGTH"), v8::Uint32::New(GL_ACTIVE_ATTRIBUTE_MAX_LENGTH), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("FRAGMENT_SHADER_DERIVATIVE_HINT"), v8::Uint32::New(GL_FRAGMENT_SHADER_DERIVATIVE_HINT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("SHADING_LANGUAGE_VERSION"), v8::Uint32::New(GL_SHADING_LANGUAGE_VERSION), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("CURRENT_PROGRAM"), v8::Uint32::New(GL_CURRENT_PROGRAM), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("POINT_SPRITE_COORD_ORIGIN"), v8::Uint32::New(GL_POINT_SPRITE_COORD_ORIGIN), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("LOWER_LEFT"), v8::Uint32::New(GL_LOWER_LEFT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UPPER_LEFT"), v8::Uint32::New(GL_UPPER_LEFT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("STENCIL_BACK_REF"), v8::Uint32::New(GL_STENCIL_BACK_REF), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("STENCIL_BACK_VALUE_MASK"), v8::Uint32::New(GL_STENCIL_BACK_VALUE_MASK), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("STENCIL_BACK_WRITEMASK"), v8::Uint32::New(GL_STENCIL_BACK_WRITEMASK), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("PIXEL_PACK_BUFFER"), v8::Uint32::New(GL_PIXEL_PACK_BUFFER), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("PIXEL_UNPACK_BUFFER"), v8::Uint32::New(GL_PIXEL_UNPACK_BUFFER), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("PIXEL_PACK_BUFFER_BINDING"), v8::Uint32::New(GL_PIXEL_PACK_BUFFER_BINDING), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("PIXEL_UNPACK_BUFFER_BINDING"), v8::Uint32::New(GL_PIXEL_UNPACK_BUFFER_BINDING), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("FLOAT_MAT2x3"), v8::Uint32::New(GL_FLOAT_MAT2x3), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("FLOAT_MAT2x4"), v8::Uint32::New(GL_FLOAT_MAT2x4), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("FLOAT_MAT3x2"), v8::Uint32::New(GL_FLOAT_MAT3x2), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("FLOAT_MAT3x4"), v8::Uint32::New(GL_FLOAT_MAT3x4), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("FLOAT_MAT4x2"), v8::Uint32::New(GL_FLOAT_MAT4x2), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("FLOAT_MAT4x3"), v8::Uint32::New(GL_FLOAT_MAT4x3), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("SRGB"), v8::Uint32::New(GL_SRGB), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("SRGB8"), v8::Uint32::New(GL_SRGB8), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("SRGB_ALPHA"), v8::Uint32::New(GL_SRGB_ALPHA), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("SRGB8_ALPHA8"), v8::Uint32::New(GL_SRGB8_ALPHA8), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("COMPRESSED_SRGB"), v8::Uint32::New(GL_COMPRESSED_SRGB), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("COMPRESSED_SRGB_ALPHA"), v8::Uint32::New(GL_COMPRESSED_SRGB_ALPHA), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("COMPARE_REF_TO_TEXTURE"), v8::Uint32::New(GL_COMPARE_REF_TO_TEXTURE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("CLIP_DISTANCE0"), v8::Uint32::New(GL_CLIP_DISTANCE0), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("CLIP_DISTANCE1"), v8::Uint32::New(GL_CLIP_DISTANCE1), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("CLIP_DISTANCE2"), v8::Uint32::New(GL_CLIP_DISTANCE2), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("CLIP_DISTANCE3"), v8::Uint32::New(GL_CLIP_DISTANCE3), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("CLIP_DISTANCE4"), v8::Uint32::New(GL_CLIP_DISTANCE4), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("CLIP_DISTANCE5"), v8::Uint32::New(GL_CLIP_DISTANCE5), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("CLIP_DISTANCE6"), v8::Uint32::New(GL_CLIP_DISTANCE6), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("CLIP_DISTANCE7"), v8::Uint32::New(GL_CLIP_DISTANCE7), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_CLIP_DISTANCES"), v8::Uint32::New(GL_MAX_CLIP_DISTANCES), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAJOR_VERSION"), v8::Uint32::New(GL_MAJOR_VERSION), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MINOR_VERSION"), v8::Uint32::New(GL_MINOR_VERSION), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("NUM_EXTENSIONS"), v8::Uint32::New(GL_NUM_EXTENSIONS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("CONTEXT_FLAGS"), v8::Uint32::New(GL_CONTEXT_FLAGS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("COMPRESSED_RED"), v8::Uint32::New(GL_COMPRESSED_RED), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("COMPRESSED_RG"), v8::Uint32::New(GL_COMPRESSED_RG), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT"), v8::Uint32::New(GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RGBA32F"), v8::Uint32::New(GL_RGBA32F), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RGB32F"), v8::Uint32::New(GL_RGB32F), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RGBA16F"), v8::Uint32::New(GL_RGBA16F), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RGB16F"), v8::Uint32::New(GL_RGB16F), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("VERTEX_ATTRIB_ARRAY_INTEGER"), v8::Uint32::New(GL_VERTEX_ATTRIB_ARRAY_INTEGER), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_ARRAY_TEXTURE_LAYERS"), v8::Uint32::New(GL_MAX_ARRAY_TEXTURE_LAYERS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MIN_PROGRAM_TEXEL_OFFSET"), v8::Uint32::New(GL_MIN_PROGRAM_TEXEL_OFFSET), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_PROGRAM_TEXEL_OFFSET"), v8::Uint32::New(GL_MAX_PROGRAM_TEXEL_OFFSET), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("CLAMP_READ_COLOR"), v8::Uint32::New(GL_CLAMP_READ_COLOR), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("FIXED_ONLY"), v8::Uint32::New(GL_FIXED_ONLY), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_VARYING_COMPONENTS"), v8::Uint32::New(GL_MAX_VARYING_COMPONENTS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_1D_ARRAY"), v8::Uint32::New(GL_TEXTURE_1D_ARRAY), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("PROXY_TEXTURE_1D_ARRAY"), v8::Uint32::New(GL_PROXY_TEXTURE_1D_ARRAY), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_2D_ARRAY"), v8::Uint32::New(GL_TEXTURE_2D_ARRAY), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("PROXY_TEXTURE_2D_ARRAY"), v8::Uint32::New(GL_PROXY_TEXTURE_2D_ARRAY), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_BINDING_1D_ARRAY"), v8::Uint32::New(GL_TEXTURE_BINDING_1D_ARRAY), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_BINDING_2D_ARRAY"), v8::Uint32::New(GL_TEXTURE_BINDING_2D_ARRAY), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("R11F_G11F_B10F"), v8::Uint32::New(GL_R11F_G11F_B10F), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNSIGNED_INT_10F_11F_11F_REV"), v8::Uint32::New(GL_UNSIGNED_INT_10F_11F_11F_REV), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RGB9_E5"), v8::Uint32::New(GL_RGB9_E5), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNSIGNED_INT_5_9_9_9_REV"), v8::Uint32::New(GL_UNSIGNED_INT_5_9_9_9_REV), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_SHARED_SIZE"), v8::Uint32::New(GL_TEXTURE_SHARED_SIZE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH"), v8::Uint32::New(GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TRANSFORM_FEEDBACK_BUFFER_MODE"), v8::Uint32::New(GL_TRANSFORM_FEEDBACK_BUFFER_MODE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS"), v8::Uint32::New(GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TRANSFORM_FEEDBACK_VARYINGS"), v8::Uint32::New(GL_TRANSFORM_FEEDBACK_VARYINGS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TRANSFORM_FEEDBACK_BUFFER_START"), v8::Uint32::New(GL_TRANSFORM_FEEDBACK_BUFFER_START), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TRANSFORM_FEEDBACK_BUFFER_SIZE"), v8::Uint32::New(GL_TRANSFORM_FEEDBACK_BUFFER_SIZE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("PRIMITIVES_GENERATED"), v8::Uint32::New(GL_PRIMITIVES_GENERATED), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN"), v8::Uint32::New(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RASTERIZER_DISCARD"), v8::Uint32::New(GL_RASTERIZER_DISCARD), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS"), v8::Uint32::New(GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS"), v8::Uint32::New(GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("INTERLEAVED_ATTRIBS"), v8::Uint32::New(GL_INTERLEAVED_ATTRIBS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("SEPARATE_ATTRIBS"), v8::Uint32::New(GL_SEPARATE_ATTRIBS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TRANSFORM_FEEDBACK_BUFFER"), v8::Uint32::New(GL_TRANSFORM_FEEDBACK_BUFFER), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TRANSFORM_FEEDBACK_BUFFER_BINDING"), v8::Uint32::New(GL_TRANSFORM_FEEDBACK_BUFFER_BINDING), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RGBA32UI"), v8::Uint32::New(GL_RGBA32UI), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RGB32UI"), v8::Uint32::New(GL_RGB32UI), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RGBA16UI"), v8::Uint32::New(GL_RGBA16UI), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RGB16UI"), v8::Uint32::New(GL_RGB16UI), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RGBA8UI"), v8::Uint32::New(GL_RGBA8UI), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RGB8UI"), v8::Uint32::New(GL_RGB8UI), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RGBA32I"), v8::Uint32::New(GL_RGBA32I), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RGB32I"), v8::Uint32::New(GL_RGB32I), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RGBA16I"), v8::Uint32::New(GL_RGBA16I), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RGB16I"), v8::Uint32::New(GL_RGB16I), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RGBA8I"), v8::Uint32::New(GL_RGBA8I), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RGB8I"), v8::Uint32::New(GL_RGB8I), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RED_INTEGER"), v8::Uint32::New(GL_RED_INTEGER), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("GREEN_INTEGER"), v8::Uint32::New(GL_GREEN_INTEGER), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("BLUE_INTEGER"), v8::Uint32::New(GL_BLUE_INTEGER), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RGB_INTEGER"), v8::Uint32::New(GL_RGB_INTEGER), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RGBA_INTEGER"), v8::Uint32::New(GL_RGBA_INTEGER), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("BGR_INTEGER"), v8::Uint32::New(GL_BGR_INTEGER), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("BGRA_INTEGER"), v8::Uint32::New(GL_BGRA_INTEGER), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("SAMPLER_1D_ARRAY"), v8::Uint32::New(GL_SAMPLER_1D_ARRAY), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("SAMPLER_2D_ARRAY"), v8::Uint32::New(GL_SAMPLER_2D_ARRAY), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("SAMPLER_1D_ARRAY_SHADOW"), v8::Uint32::New(GL_SAMPLER_1D_ARRAY_SHADOW), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("SAMPLER_2D_ARRAY_SHADOW"), v8::Uint32::New(GL_SAMPLER_2D_ARRAY_SHADOW), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("SAMPLER_CUBE_SHADOW"), v8::Uint32::New(GL_SAMPLER_CUBE_SHADOW), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNSIGNED_INT_VEC2"), v8::Uint32::New(GL_UNSIGNED_INT_VEC2), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNSIGNED_INT_VEC3"), v8::Uint32::New(GL_UNSIGNED_INT_VEC3), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNSIGNED_INT_VEC4"), v8::Uint32::New(GL_UNSIGNED_INT_VEC4), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("INT_SAMPLER_1D"), v8::Uint32::New(GL_INT_SAMPLER_1D), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("INT_SAMPLER_2D"), v8::Uint32::New(GL_INT_SAMPLER_2D), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("INT_SAMPLER_3D"), v8::Uint32::New(GL_INT_SAMPLER_3D), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("INT_SAMPLER_CUBE"), v8::Uint32::New(GL_INT_SAMPLER_CUBE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("INT_SAMPLER_1D_ARRAY"), v8::Uint32::New(GL_INT_SAMPLER_1D_ARRAY), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("INT_SAMPLER_2D_ARRAY"), v8::Uint32::New(GL_INT_SAMPLER_2D_ARRAY), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNSIGNED_INT_SAMPLER_1D"), v8::Uint32::New(GL_UNSIGNED_INT_SAMPLER_1D), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNSIGNED_INT_SAMPLER_2D"), v8::Uint32::New(GL_UNSIGNED_INT_SAMPLER_2D), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNSIGNED_INT_SAMPLER_3D"), v8::Uint32::New(GL_UNSIGNED_INT_SAMPLER_3D), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNSIGNED_INT_SAMPLER_CUBE"), v8::Uint32::New(GL_UNSIGNED_INT_SAMPLER_CUBE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNSIGNED_INT_SAMPLER_1D_ARRAY"), v8::Uint32::New(GL_UNSIGNED_INT_SAMPLER_1D_ARRAY), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNSIGNED_INT_SAMPLER_2D_ARRAY"), v8::Uint32::New(GL_UNSIGNED_INT_SAMPLER_2D_ARRAY), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("QUERY_WAIT"), v8::Uint32::New(GL_QUERY_WAIT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("QUERY_NO_WAIT"), v8::Uint32::New(GL_QUERY_NO_WAIT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("QUERY_BY_REGION_WAIT"), v8::Uint32::New(GL_QUERY_BY_REGION_WAIT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("QUERY_BY_REGION_NO_WAIT"), v8::Uint32::New(GL_QUERY_BY_REGION_NO_WAIT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("BUFFER_ACCESS_FLAGS"), v8::Uint32::New(GL_BUFFER_ACCESS_FLAGS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("BUFFER_MAP_LENGTH"), v8::Uint32::New(GL_BUFFER_MAP_LENGTH), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("BUFFER_MAP_OFFSET"), v8::Uint32::New(GL_BUFFER_MAP_OFFSET), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("SAMPLER_2D_RECT"), v8::Uint32::New(GL_SAMPLER_2D_RECT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("SAMPLER_2D_RECT_SHADOW"), v8::Uint32::New(GL_SAMPLER_2D_RECT_SHADOW), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("SAMPLER_BUFFER"), v8::Uint32::New(GL_SAMPLER_BUFFER), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("INT_SAMPLER_2D_RECT"), v8::Uint32::New(GL_INT_SAMPLER_2D_RECT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("INT_SAMPLER_BUFFER"), v8::Uint32::New(GL_INT_SAMPLER_BUFFER), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNSIGNED_INT_SAMPLER_2D_RECT"), v8::Uint32::New(GL_UNSIGNED_INT_SAMPLER_2D_RECT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNSIGNED_INT_SAMPLER_BUFFER"), v8::Uint32::New(GL_UNSIGNED_INT_SAMPLER_BUFFER), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_BUFFER"), v8::Uint32::New(GL_TEXTURE_BUFFER), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_TEXTURE_BUFFER_SIZE"), v8::Uint32::New(GL_MAX_TEXTURE_BUFFER_SIZE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_BINDING_BUFFER"), v8::Uint32::New(GL_TEXTURE_BINDING_BUFFER), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_BUFFER_DATA_STORE_BINDING"), v8::Uint32::New(GL_TEXTURE_BUFFER_DATA_STORE_BINDING), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_RECTANGLE"), v8::Uint32::New(GL_TEXTURE_RECTANGLE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_BINDING_RECTANGLE"), v8::Uint32::New(GL_TEXTURE_BINDING_RECTANGLE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("PROXY_TEXTURE_RECTANGLE"), v8::Uint32::New(GL_PROXY_TEXTURE_RECTANGLE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_RECTANGLE_TEXTURE_SIZE"), v8::Uint32::New(GL_MAX_RECTANGLE_TEXTURE_SIZE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("R8_SNORM"), v8::Uint32::New(GL_R8_SNORM), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RG8_SNORM"), v8::Uint32::New(GL_RG8_SNORM), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RGB8_SNORM"), v8::Uint32::New(GL_RGB8_SNORM), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RGBA8_SNORM"), v8::Uint32::New(GL_RGBA8_SNORM), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("R16_SNORM"), v8::Uint32::New(GL_R16_SNORM), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RG16_SNORM"), v8::Uint32::New(GL_RG16_SNORM), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RGB16_SNORM"), v8::Uint32::New(GL_RGB16_SNORM), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RGBA16_SNORM"), v8::Uint32::New(GL_RGBA16_SNORM), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("SIGNED_NORMALIZED"), v8::Uint32::New(GL_SIGNED_NORMALIZED), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("PRIMITIVE_RESTART"), v8::Uint32::New(GL_PRIMITIVE_RESTART), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("PRIMITIVE_RESTART_INDEX"), v8::Uint32::New(GL_PRIMITIVE_RESTART_INDEX), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("CONTEXT_CORE_PROFILE_BIT"), v8::Uint32::New(GL_CONTEXT_CORE_PROFILE_BIT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("CONTEXT_COMPATIBILITY_PROFILE_BIT"), v8::Uint32::New(GL_CONTEXT_COMPATIBILITY_PROFILE_BIT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("LINES_ADJACENCY"), v8::Uint32::New(GL_LINES_ADJACENCY), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("LINE_STRIP_ADJACENCY"), v8::Uint32::New(GL_LINE_STRIP_ADJACENCY), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TRIANGLES_ADJACENCY"), v8::Uint32::New(GL_TRIANGLES_ADJACENCY), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TRIANGLE_STRIP_ADJACENCY"), v8::Uint32::New(GL_TRIANGLE_STRIP_ADJACENCY), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("PROGRAM_POINT_SIZE"), v8::Uint32::New(GL_PROGRAM_POINT_SIZE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_GEOMETRY_TEXTURE_IMAGE_UNITS"), v8::Uint32::New(GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("FRAMEBUFFER_ATTACHMENT_LAYERED"), v8::Uint32::New(GL_FRAMEBUFFER_ATTACHMENT_LAYERED), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS"), v8::Uint32::New(GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("GEOMETRY_SHADER"), v8::Uint32::New(GL_GEOMETRY_SHADER), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("GEOMETRY_VERTICES_OUT"), v8::Uint32::New(GL_GEOMETRY_VERTICES_OUT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("GEOMETRY_INPUT_TYPE"), v8::Uint32::New(GL_GEOMETRY_INPUT_TYPE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("GEOMETRY_OUTPUT_TYPE"), v8::Uint32::New(GL_GEOMETRY_OUTPUT_TYPE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_GEOMETRY_UNIFORM_COMPONENTS"), v8::Uint32::New(GL_MAX_GEOMETRY_UNIFORM_COMPONENTS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_GEOMETRY_OUTPUT_VERTICES"), v8::Uint32::New(GL_MAX_GEOMETRY_OUTPUT_VERTICES), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS"), v8::Uint32::New(GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_VERTEX_OUTPUT_COMPONENTS"), v8::Uint32::New(GL_MAX_VERTEX_OUTPUT_COMPONENTS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_GEOMETRY_INPUT_COMPONENTS"), v8::Uint32::New(GL_MAX_GEOMETRY_INPUT_COMPONENTS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_GEOMETRY_OUTPUT_COMPONENTS"), v8::Uint32::New(GL_MAX_GEOMETRY_OUTPUT_COMPONENTS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_FRAGMENT_INPUT_COMPONENTS"), v8::Uint32::New(GL_MAX_FRAGMENT_INPUT_COMPONENTS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("CONTEXT_PROFILE_MASK"), v8::Uint32::New(GL_CONTEXT_PROFILE_MASK), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("VERTEX_ATTRIB_ARRAY_DIVISOR"), v8::Uint32::New(GL_VERTEX_ATTRIB_ARRAY_DIVISOR), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("SAMPLE_SHADING"), v8::Uint32::New(GL_SAMPLE_SHADING), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MIN_SAMPLE_SHADING_VALUE"), v8::Uint32::New(GL_MIN_SAMPLE_SHADING_VALUE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MIN_PROGRAM_TEXTURE_GATHER_OFFSET"), v8::Uint32::New(GL_MIN_PROGRAM_TEXTURE_GATHER_OFFSET), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_PROGRAM_TEXTURE_GATHER_OFFSET"), v8::Uint32::New(GL_MAX_PROGRAM_TEXTURE_GATHER_OFFSET), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_CUBE_MAP_ARRAY"), v8::Uint32::New(GL_TEXTURE_CUBE_MAP_ARRAY), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_BINDING_CUBE_MAP_ARRAY"), v8::Uint32::New(GL_TEXTURE_BINDING_CUBE_MAP_ARRAY), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("PROXY_TEXTURE_CUBE_MAP_ARRAY"), v8::Uint32::New(GL_PROXY_TEXTURE_CUBE_MAP_ARRAY), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("SAMPLER_CUBE_MAP_ARRAY"), v8::Uint32::New(GL_SAMPLER_CUBE_MAP_ARRAY), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("SAMPLER_CUBE_MAP_ARRAY_SHADOW"), v8::Uint32::New(GL_SAMPLER_CUBE_MAP_ARRAY_SHADOW), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("INT_SAMPLER_CUBE_MAP_ARRAY"), v8::Uint32::New(GL_INT_SAMPLER_CUBE_MAP_ARRAY), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNSIGNED_INT_SAMPLER_CUBE_MAP_ARRAY"), v8::Uint32::New(GL_UNSIGNED_INT_SAMPLER_CUBE_MAP_ARRAY), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("DEPTH_COMPONENT32F"), v8::Uint32::New(GL_DEPTH_COMPONENT32F), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("DEPTH32F_STENCIL8"), v8::Uint32::New(GL_DEPTH32F_STENCIL8), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("FLOAT_32_UNSIGNED_INT_24_8_REV"), v8::Uint32::New(GL_FLOAT_32_UNSIGNED_INT_24_8_REV), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("INVALID_FRAMEBUFFER_OPERATION"), v8::Uint32::New(GL_INVALID_FRAMEBUFFER_OPERATION), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING"), v8::Uint32::New(GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE"), v8::Uint32::New(GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("FRAMEBUFFER_ATTACHMENT_RED_SIZE"), v8::Uint32::New(GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("FRAMEBUFFER_ATTACHMENT_GREEN_SIZE"), v8::Uint32::New(GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("FRAMEBUFFER_ATTACHMENT_BLUE_SIZE"), v8::Uint32::New(GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE"), v8::Uint32::New(GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE"), v8::Uint32::New(GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE"), v8::Uint32::New(GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("FRAMEBUFFER_DEFAULT"), v8::Uint32::New(GL_FRAMEBUFFER_DEFAULT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("FRAMEBUFFER_UNDEFINED"), v8::Uint32::New(GL_FRAMEBUFFER_UNDEFINED), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("DEPTH_STENCIL_ATTACHMENT"), v8::Uint32::New(GL_DEPTH_STENCIL_ATTACHMENT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_RENDERBUFFER_SIZE"), v8::Uint32::New(GL_MAX_RENDERBUFFER_SIZE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("DEPTH_STENCIL"), v8::Uint32::New(GL_DEPTH_STENCIL), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNSIGNED_INT_24_8"), v8::Uint32::New(GL_UNSIGNED_INT_24_8), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("DEPTH24_STENCIL8"), v8::Uint32::New(GL_DEPTH24_STENCIL8), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_STENCIL_SIZE"), v8::Uint32::New(GL_TEXTURE_STENCIL_SIZE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_RED_TYPE"), v8::Uint32::New(GL_TEXTURE_RED_TYPE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_GREEN_TYPE"), v8::Uint32::New(GL_TEXTURE_GREEN_TYPE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_BLUE_TYPE"), v8::Uint32::New(GL_TEXTURE_BLUE_TYPE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_ALPHA_TYPE"), v8::Uint32::New(GL_TEXTURE_ALPHA_TYPE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_DEPTH_TYPE"), v8::Uint32::New(GL_TEXTURE_DEPTH_TYPE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNSIGNED_NORMALIZED"), v8::Uint32::New(GL_UNSIGNED_NORMALIZED), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("FRAMEBUFFER_BINDING"), v8::Uint32::New(GL_FRAMEBUFFER_BINDING), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RENDERBUFFER_BINDING"), v8::Uint32::New(GL_RENDERBUFFER_BINDING), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("READ_FRAMEBUFFER"), v8::Uint32::New(GL_READ_FRAMEBUFFER), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("DRAW_FRAMEBUFFER"), v8::Uint32::New(GL_DRAW_FRAMEBUFFER), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("READ_FRAMEBUFFER_BINDING"), v8::Uint32::New(GL_READ_FRAMEBUFFER_BINDING), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RENDERBUFFER_SAMPLES"), v8::Uint32::New(GL_RENDERBUFFER_SAMPLES), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE"), v8::Uint32::New(GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("FRAMEBUFFER_ATTACHMENT_OBJECT_NAME"), v8::Uint32::New(GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL"), v8::Uint32::New(GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE"), v8::Uint32::New(GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER"), v8::Uint32::New(GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("FRAMEBUFFER_COMPLETE"), v8::Uint32::New(GL_FRAMEBUFFER_COMPLETE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("FRAMEBUFFER_INCOMPLETE_ATTACHMENT"), v8::Uint32::New(GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT"), v8::Uint32::New(GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER"), v8::Uint32::New(GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("FRAMEBUFFER_INCOMPLETE_READ_BUFFER"), v8::Uint32::New(GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("FRAMEBUFFER_UNSUPPORTED"), v8::Uint32::New(GL_FRAMEBUFFER_UNSUPPORTED), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_COLOR_ATTACHMENTS"), v8::Uint32::New(GL_MAX_COLOR_ATTACHMENTS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("COLOR_ATTACHMENT0"), v8::Uint32::New(GL_COLOR_ATTACHMENT0), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("COLOR_ATTACHMENT1"), v8::Uint32::New(GL_COLOR_ATTACHMENT1), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("COLOR_ATTACHMENT2"), v8::Uint32::New(GL_COLOR_ATTACHMENT2), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("COLOR_ATTACHMENT3"), v8::Uint32::New(GL_COLOR_ATTACHMENT3), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("COLOR_ATTACHMENT4"), v8::Uint32::New(GL_COLOR_ATTACHMENT4), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("COLOR_ATTACHMENT5"), v8::Uint32::New(GL_COLOR_ATTACHMENT5), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("COLOR_ATTACHMENT6"), v8::Uint32::New(GL_COLOR_ATTACHMENT6), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("COLOR_ATTACHMENT7"), v8::Uint32::New(GL_COLOR_ATTACHMENT7), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("COLOR_ATTACHMENT8"), v8::Uint32::New(GL_COLOR_ATTACHMENT8), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("COLOR_ATTACHMENT9"), v8::Uint32::New(GL_COLOR_ATTACHMENT9), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("COLOR_ATTACHMENT10"), v8::Uint32::New(GL_COLOR_ATTACHMENT10), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("COLOR_ATTACHMENT11"), v8::Uint32::New(GL_COLOR_ATTACHMENT11), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("COLOR_ATTACHMENT12"), v8::Uint32::New(GL_COLOR_ATTACHMENT12), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("COLOR_ATTACHMENT13"), v8::Uint32::New(GL_COLOR_ATTACHMENT13), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("COLOR_ATTACHMENT14"), v8::Uint32::New(GL_COLOR_ATTACHMENT14), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("COLOR_ATTACHMENT15"), v8::Uint32::New(GL_COLOR_ATTACHMENT15), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("DEPTH_ATTACHMENT"), v8::Uint32::New(GL_DEPTH_ATTACHMENT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("STENCIL_ATTACHMENT"), v8::Uint32::New(GL_STENCIL_ATTACHMENT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("FRAMEBUFFER"), v8::Uint32::New(GL_FRAMEBUFFER), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RENDERBUFFER"), v8::Uint32::New(GL_RENDERBUFFER), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RENDERBUFFER_WIDTH"), v8::Uint32::New(GL_RENDERBUFFER_WIDTH), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RENDERBUFFER_HEIGHT"), v8::Uint32::New(GL_RENDERBUFFER_HEIGHT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RENDERBUFFER_INTERNAL_FORMAT"), v8::Uint32::New(GL_RENDERBUFFER_INTERNAL_FORMAT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("STENCIL_INDEX1"), v8::Uint32::New(GL_STENCIL_INDEX1), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("STENCIL_INDEX4"), v8::Uint32::New(GL_STENCIL_INDEX4), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("STENCIL_INDEX8"), v8::Uint32::New(GL_STENCIL_INDEX8), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("STENCIL_INDEX16"), v8::Uint32::New(GL_STENCIL_INDEX16), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RENDERBUFFER_RED_SIZE"), v8::Uint32::New(GL_RENDERBUFFER_RED_SIZE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RENDERBUFFER_GREEN_SIZE"), v8::Uint32::New(GL_RENDERBUFFER_GREEN_SIZE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RENDERBUFFER_BLUE_SIZE"), v8::Uint32::New(GL_RENDERBUFFER_BLUE_SIZE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RENDERBUFFER_ALPHA_SIZE"), v8::Uint32::New(GL_RENDERBUFFER_ALPHA_SIZE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RENDERBUFFER_DEPTH_SIZE"), v8::Uint32::New(GL_RENDERBUFFER_DEPTH_SIZE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RENDERBUFFER_STENCIL_SIZE"), v8::Uint32::New(GL_RENDERBUFFER_STENCIL_SIZE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("FRAMEBUFFER_INCOMPLETE_MULTISAMPLE"), v8::Uint32::New(GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_SAMPLES"), v8::Uint32::New(GL_MAX_SAMPLES), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("FRAMEBUFFER_SRGB"), v8::Uint32::New(GL_FRAMEBUFFER_SRGB), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("HALF_FLOAT"), v8::Uint32::New(GL_HALF_FLOAT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAP_READ_BIT"), v8::Uint32::New(GL_MAP_READ_BIT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAP_WRITE_BIT"), v8::Uint32::New(GL_MAP_WRITE_BIT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAP_INVALIDATE_RANGE_BIT"), v8::Uint32::New(GL_MAP_INVALIDATE_RANGE_BIT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAP_INVALIDATE_BUFFER_BIT"), v8::Uint32::New(GL_MAP_INVALIDATE_BUFFER_BIT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAP_FLUSH_EXPLICIT_BIT"), v8::Uint32::New(GL_MAP_FLUSH_EXPLICIT_BIT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAP_UNSYNCHRONIZED_BIT"), v8::Uint32::New(GL_MAP_UNSYNCHRONIZED_BIT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("COMPRESSED_RED_RGTC1"), v8::Uint32::New(GL_COMPRESSED_RED_RGTC1), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("COMPRESSED_SIGNED_RED_RGTC1"), v8::Uint32::New(GL_COMPRESSED_SIGNED_RED_RGTC1), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("COMPRESSED_RG_RGTC2"), v8::Uint32::New(GL_COMPRESSED_RG_RGTC2), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("COMPRESSED_SIGNED_RG_RGTC2"), v8::Uint32::New(GL_COMPRESSED_SIGNED_RG_RGTC2), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RG"), v8::Uint32::New(GL_RG), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RG_INTEGER"), v8::Uint32::New(GL_RG_INTEGER), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("R8"), v8::Uint32::New(GL_R8), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("R16"), v8::Uint32::New(GL_R16), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RG8"), v8::Uint32::New(GL_RG8), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RG16"), v8::Uint32::New(GL_RG16), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("R16F"), v8::Uint32::New(GL_R16F), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("R32F"), v8::Uint32::New(GL_R32F), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RG16F"), v8::Uint32::New(GL_RG16F), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RG32F"), v8::Uint32::New(GL_RG32F), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("R8I"), v8::Uint32::New(GL_R8I), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("R8UI"), v8::Uint32::New(GL_R8UI), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("R16I"), v8::Uint32::New(GL_R16I), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("R16UI"), v8::Uint32::New(GL_R16UI), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("R32I"), v8::Uint32::New(GL_R32I), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("R32UI"), v8::Uint32::New(GL_R32UI), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RG8I"), v8::Uint32::New(GL_RG8I), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RG8UI"), v8::Uint32::New(GL_RG8UI), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RG16I"), v8::Uint32::New(GL_RG16I), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RG16UI"), v8::Uint32::New(GL_RG16UI), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RG32I"), v8::Uint32::New(GL_RG32I), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RG32UI"), v8::Uint32::New(GL_RG32UI), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("VERTEX_ARRAY_BINDING"), v8::Uint32::New(GL_VERTEX_ARRAY_BINDING), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNIFORM_BUFFER"), v8::Uint32::New(GL_UNIFORM_BUFFER), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNIFORM_BUFFER_BINDING"), v8::Uint32::New(GL_UNIFORM_BUFFER_BINDING), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNIFORM_BUFFER_START"), v8::Uint32::New(GL_UNIFORM_BUFFER_START), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNIFORM_BUFFER_SIZE"), v8::Uint32::New(GL_UNIFORM_BUFFER_SIZE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_VERTEX_UNIFORM_BLOCKS"), v8::Uint32::New(GL_MAX_VERTEX_UNIFORM_BLOCKS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_GEOMETRY_UNIFORM_BLOCKS"), v8::Uint32::New(GL_MAX_GEOMETRY_UNIFORM_BLOCKS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_FRAGMENT_UNIFORM_BLOCKS"), v8::Uint32::New(GL_MAX_FRAGMENT_UNIFORM_BLOCKS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_COMBINED_UNIFORM_BLOCKS"), v8::Uint32::New(GL_MAX_COMBINED_UNIFORM_BLOCKS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_UNIFORM_BUFFER_BINDINGS"), v8::Uint32::New(GL_MAX_UNIFORM_BUFFER_BINDINGS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_UNIFORM_BLOCK_SIZE"), v8::Uint32::New(GL_MAX_UNIFORM_BLOCK_SIZE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS"), v8::Uint32::New(GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS"), v8::Uint32::New(GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS"), v8::Uint32::New(GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNIFORM_BUFFER_OFFSET_ALIGNMENT"), v8::Uint32::New(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH"), v8::Uint32::New(GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("ACTIVE_UNIFORM_BLOCKS"), v8::Uint32::New(GL_ACTIVE_UNIFORM_BLOCKS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNIFORM_TYPE"), v8::Uint32::New(GL_UNIFORM_TYPE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNIFORM_SIZE"), v8::Uint32::New(GL_UNIFORM_SIZE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNIFORM_NAME_LENGTH"), v8::Uint32::New(GL_UNIFORM_NAME_LENGTH), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNIFORM_BLOCK_INDEX"), v8::Uint32::New(GL_UNIFORM_BLOCK_INDEX), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNIFORM_OFFSET"), v8::Uint32::New(GL_UNIFORM_OFFSET), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNIFORM_ARRAY_STRIDE"), v8::Uint32::New(GL_UNIFORM_ARRAY_STRIDE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNIFORM_MATRIX_STRIDE"), v8::Uint32::New(GL_UNIFORM_MATRIX_STRIDE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNIFORM_IS_ROW_MAJOR"), v8::Uint32::New(GL_UNIFORM_IS_ROW_MAJOR), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNIFORM_BLOCK_BINDING"), v8::Uint32::New(GL_UNIFORM_BLOCK_BINDING), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNIFORM_BLOCK_DATA_SIZE"), v8::Uint32::New(GL_UNIFORM_BLOCK_DATA_SIZE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNIFORM_BLOCK_NAME_LENGTH"), v8::Uint32::New(GL_UNIFORM_BLOCK_NAME_LENGTH), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNIFORM_BLOCK_ACTIVE_UNIFORMS"), v8::Uint32::New(GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES"), v8::Uint32::New(GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER"), v8::Uint32::New(GL_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNIFORM_BLOCK_REFERENCED_BY_GEOMETRY_SHADER"), v8::Uint32::New(GL_UNIFORM_BLOCK_REFERENCED_BY_GEOMETRY_SHADER), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER"), v8::Uint32::New(GL_UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("COPY_READ_BUFFER"), v8::Uint32::New(GL_COPY_READ_BUFFER), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("COPY_WRITE_BUFFER"), v8::Uint32::New(GL_COPY_WRITE_BUFFER), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("DEPTH_CLAMP"), v8::Uint32::New(GL_DEPTH_CLAMP), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("FIRST_VERTEX_CONVENTION"), v8::Uint32::New(GL_FIRST_VERTEX_CONVENTION), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("LAST_VERTEX_CONVENTION"), v8::Uint32::New(GL_LAST_VERTEX_CONVENTION), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("PROVOKING_VERTEX"), v8::Uint32::New(GL_PROVOKING_VERTEX), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_CUBE_MAP_SEAMLESS"), v8::Uint32::New(GL_TEXTURE_CUBE_MAP_SEAMLESS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_SERVER_WAIT_TIMEOUT"), v8::Uint32::New(GL_MAX_SERVER_WAIT_TIMEOUT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("OBJECT_TYPE"), v8::Uint32::New(GL_OBJECT_TYPE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("SYNC_CONDITION"), v8::Uint32::New(GL_SYNC_CONDITION), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("SYNC_STATUS"), v8::Uint32::New(GL_SYNC_STATUS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("SYNC_FLAGS"), v8::Uint32::New(GL_SYNC_FLAGS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("SYNC_FENCE"), v8::Uint32::New(GL_SYNC_FENCE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("SYNC_GPU_COMMANDS_COMPLETE"), v8::Uint32::New(GL_SYNC_GPU_COMMANDS_COMPLETE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNSIGNALED"), v8::Uint32::New(GL_UNSIGNALED), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("SIGNALED"), v8::Uint32::New(GL_SIGNALED), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("ALREADY_SIGNALED"), v8::Uint32::New(GL_ALREADY_SIGNALED), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TIMEOUT_EXPIRED"), v8::Uint32::New(GL_TIMEOUT_EXPIRED), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("CONDITION_SATISFIED"), v8::Uint32::New(GL_CONDITION_SATISFIED), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("WAIT_FAILED"), v8::Uint32::New(GL_WAIT_FAILED), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("SYNC_FLUSH_COMMANDS_BIT"), v8::Uint32::New(GL_SYNC_FLUSH_COMMANDS_BIT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("SAMPLE_POSITION"), v8::Uint32::New(GL_SAMPLE_POSITION), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("SAMPLE_MASK"), v8::Uint32::New(GL_SAMPLE_MASK), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("SAMPLE_MASK_VALUE"), v8::Uint32::New(GL_SAMPLE_MASK_VALUE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_SAMPLE_MASK_WORDS"), v8::Uint32::New(GL_MAX_SAMPLE_MASK_WORDS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_2D_MULTISAMPLE"), v8::Uint32::New(GL_TEXTURE_2D_MULTISAMPLE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("PROXY_TEXTURE_2D_MULTISAMPLE"), v8::Uint32::New(GL_PROXY_TEXTURE_2D_MULTISAMPLE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_2D_MULTISAMPLE_ARRAY"), v8::Uint32::New(GL_TEXTURE_2D_MULTISAMPLE_ARRAY), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("PROXY_TEXTURE_2D_MULTISAMPLE_ARRAY"), v8::Uint32::New(GL_PROXY_TEXTURE_2D_MULTISAMPLE_ARRAY), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_BINDING_2D_MULTISAMPLE"), v8::Uint32::New(GL_TEXTURE_BINDING_2D_MULTISAMPLE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY"), v8::Uint32::New(GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_SAMPLES"), v8::Uint32::New(GL_TEXTURE_SAMPLES), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_FIXED_SAMPLE_LOCATIONS"), v8::Uint32::New(GL_TEXTURE_FIXED_SAMPLE_LOCATIONS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("SAMPLER_2D_MULTISAMPLE"), v8::Uint32::New(GL_SAMPLER_2D_MULTISAMPLE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("INT_SAMPLER_2D_MULTISAMPLE"), v8::Uint32::New(GL_INT_SAMPLER_2D_MULTISAMPLE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE"), v8::Uint32::New(GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("SAMPLER_2D_MULTISAMPLE_ARRAY"), v8::Uint32::New(GL_SAMPLER_2D_MULTISAMPLE_ARRAY), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("INT_SAMPLER_2D_MULTISAMPLE_ARRAY"), v8::Uint32::New(GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY"), v8::Uint32::New(GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_COLOR_TEXTURE_SAMPLES"), v8::Uint32::New(GL_MAX_COLOR_TEXTURE_SAMPLES), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_DEPTH_TEXTURE_SAMPLES"), v8::Uint32::New(GL_MAX_DEPTH_TEXTURE_SAMPLES), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_INTEGER_SAMPLES"), v8::Uint32::New(GL_MAX_INTEGER_SAMPLES), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("SRC1_COLOR"), v8::Uint32::New(GL_SRC1_COLOR), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("SRC1_ALPHA"), v8::Uint32::New(GL_SRC1_ALPHA), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("ONE_MINUS_SRC1_COLOR"), v8::Uint32::New(GL_ONE_MINUS_SRC1_COLOR), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("ONE_MINUS_SRC1_ALPHA"), v8::Uint32::New(GL_ONE_MINUS_SRC1_ALPHA), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_DUAL_SOURCE_DRAW_BUFFERS"), v8::Uint32::New(GL_MAX_DUAL_SOURCE_DRAW_BUFFERS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("ANY_SAMPLES_PASSED"), v8::Uint32::New(GL_ANY_SAMPLES_PASSED), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("SAMPLER_BINDING"), v8::Uint32::New(GL_SAMPLER_BINDING), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RGB10_A2UI"), v8::Uint32::New(GL_RGB10_A2UI), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_SWIZZLE_R"), v8::Uint32::New(GL_TEXTURE_SWIZZLE_R), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_SWIZZLE_G"), v8::Uint32::New(GL_TEXTURE_SWIZZLE_G), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_SWIZZLE_B"), v8::Uint32::New(GL_TEXTURE_SWIZZLE_B), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_SWIZZLE_A"), v8::Uint32::New(GL_TEXTURE_SWIZZLE_A), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TEXTURE_SWIZZLE_RGBA"), v8::Uint32::New(GL_TEXTURE_SWIZZLE_RGBA), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TIME_ELAPSED"), v8::Uint32::New(GL_TIME_ELAPSED), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TIMESTAMP"), v8::Uint32::New(GL_TIMESTAMP), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("INT_2_10_10_10_REV"), v8::Uint32::New(GL_INT_2_10_10_10_REV), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("DRAW_INDIRECT_BUFFER"), v8::Uint32::New(GL_DRAW_INDIRECT_BUFFER), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("DRAW_INDIRECT_BUFFER_BINDING"), v8::Uint32::New(GL_DRAW_INDIRECT_BUFFER_BINDING), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("GEOMETRY_SHADER_INVOCATIONS"), v8::Uint32::New(GL_GEOMETRY_SHADER_INVOCATIONS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_GEOMETRY_SHADER_INVOCATIONS"), v8::Uint32::New(GL_MAX_GEOMETRY_SHADER_INVOCATIONS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MIN_FRAGMENT_INTERPOLATION_OFFSET"), v8::Uint32::New(GL_MIN_FRAGMENT_INTERPOLATION_OFFSET), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_FRAGMENT_INTERPOLATION_OFFSET"), v8::Uint32::New(GL_MAX_FRAGMENT_INTERPOLATION_OFFSET), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("FRAGMENT_INTERPOLATION_OFFSET_BITS"), v8::Uint32::New(GL_FRAGMENT_INTERPOLATION_OFFSET_BITS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("DOUBLE_VEC2"), v8::Uint32::New(GL_DOUBLE_VEC2), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("DOUBLE_VEC3"), v8::Uint32::New(GL_DOUBLE_VEC3), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("DOUBLE_VEC4"), v8::Uint32::New(GL_DOUBLE_VEC4), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("DOUBLE_MAT2"), v8::Uint32::New(GL_DOUBLE_MAT2), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("DOUBLE_MAT3"), v8::Uint32::New(GL_DOUBLE_MAT3), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("DOUBLE_MAT4"), v8::Uint32::New(GL_DOUBLE_MAT4), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("DOUBLE_MAT2x3"), v8::Uint32::New(GL_DOUBLE_MAT2x3), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("DOUBLE_MAT2x4"), v8::Uint32::New(GL_DOUBLE_MAT2x4), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("DOUBLE_MAT3x2"), v8::Uint32::New(GL_DOUBLE_MAT3x2), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("DOUBLE_MAT3x4"), v8::Uint32::New(GL_DOUBLE_MAT3x4), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("DOUBLE_MAT4x2"), v8::Uint32::New(GL_DOUBLE_MAT4x2), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("DOUBLE_MAT4x3"), v8::Uint32::New(GL_DOUBLE_MAT4x3), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("ACTIVE_SUBROUTINES"), v8::Uint32::New(GL_ACTIVE_SUBROUTINES), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("ACTIVE_SUBROUTINE_UNIFORMS"), v8::Uint32::New(GL_ACTIVE_SUBROUTINE_UNIFORMS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("ACTIVE_SUBROUTINE_UNIFORM_LOCATIONS"), v8::Uint32::New(GL_ACTIVE_SUBROUTINE_UNIFORM_LOCATIONS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("ACTIVE_SUBROUTINE_MAX_LENGTH"), v8::Uint32::New(GL_ACTIVE_SUBROUTINE_MAX_LENGTH), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("ACTIVE_SUBROUTINE_UNIFORM_MAX_LENGTH"), v8::Uint32::New(GL_ACTIVE_SUBROUTINE_UNIFORM_MAX_LENGTH), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_SUBROUTINES"), v8::Uint32::New(GL_MAX_SUBROUTINES), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_SUBROUTINE_UNIFORM_LOCATIONS"), v8::Uint32::New(GL_MAX_SUBROUTINE_UNIFORM_LOCATIONS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("NUM_COMPATIBLE_SUBROUTINES"), v8::Uint32::New(GL_NUM_COMPATIBLE_SUBROUTINES), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("COMPATIBLE_SUBROUTINES"), v8::Uint32::New(GL_COMPATIBLE_SUBROUTINES), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("PATCHES"), v8::Uint32::New(GL_PATCHES), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("PATCH_VERTICES"), v8::Uint32::New(GL_PATCH_VERTICES), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("PATCH_DEFAULT_INNER_LEVEL"), v8::Uint32::New(GL_PATCH_DEFAULT_INNER_LEVEL), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("PATCH_DEFAULT_OUTER_LEVEL"), v8::Uint32::New(GL_PATCH_DEFAULT_OUTER_LEVEL), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TESS_CONTROL_OUTPUT_VERTICES"), v8::Uint32::New(GL_TESS_CONTROL_OUTPUT_VERTICES), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TESS_GEN_MODE"), v8::Uint32::New(GL_TESS_GEN_MODE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TESS_GEN_SPACING"), v8::Uint32::New(GL_TESS_GEN_SPACING), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TESS_GEN_VERTEX_ORDER"), v8::Uint32::New(GL_TESS_GEN_VERTEX_ORDER), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TESS_GEN_POINT_MODE"), v8::Uint32::New(GL_TESS_GEN_POINT_MODE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("QUADS"), v8::Uint32::New(GL_QUADS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("ISOLINES"), v8::Uint32::New(GL_ISOLINES), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("FRACTIONAL_ODD"), v8::Uint32::New(GL_FRACTIONAL_ODD), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("FRACTIONAL_EVEN"), v8::Uint32::New(GL_FRACTIONAL_EVEN), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_PATCH_VERTICES"), v8::Uint32::New(GL_MAX_PATCH_VERTICES), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_TESS_GEN_LEVEL"), v8::Uint32::New(GL_MAX_TESS_GEN_LEVEL), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_TESS_CONTROL_UNIFORM_COMPONENTS"), v8::Uint32::New(GL_MAX_TESS_CONTROL_UNIFORM_COMPONENTS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_TESS_EVALUATION_UNIFORM_COMPONENTS"), v8::Uint32::New(GL_MAX_TESS_EVALUATION_UNIFORM_COMPONENTS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_TESS_CONTROL_TEXTURE_IMAGE_UNITS"), v8::Uint32::New(GL_MAX_TESS_CONTROL_TEXTURE_IMAGE_UNITS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_TESS_EVALUATION_TEXTURE_IMAGE_UNITS"), v8::Uint32::New(GL_MAX_TESS_EVALUATION_TEXTURE_IMAGE_UNITS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_TESS_CONTROL_OUTPUT_COMPONENTS"), v8::Uint32::New(GL_MAX_TESS_CONTROL_OUTPUT_COMPONENTS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_TESS_PATCH_COMPONENTS"), v8::Uint32::New(GL_MAX_TESS_PATCH_COMPONENTS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_TESS_CONTROL_TOTAL_OUTPUT_COMPONENTS"), v8::Uint32::New(GL_MAX_TESS_CONTROL_TOTAL_OUTPUT_COMPONENTS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_TESS_EVALUATION_OUTPUT_COMPONENTS"), v8::Uint32::New(GL_MAX_TESS_EVALUATION_OUTPUT_COMPONENTS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_TESS_CONTROL_UNIFORM_BLOCKS"), v8::Uint32::New(GL_MAX_TESS_CONTROL_UNIFORM_BLOCKS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_TESS_EVALUATION_UNIFORM_BLOCKS"), v8::Uint32::New(GL_MAX_TESS_EVALUATION_UNIFORM_BLOCKS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_TESS_CONTROL_INPUT_COMPONENTS"), v8::Uint32::New(GL_MAX_TESS_CONTROL_INPUT_COMPONENTS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_TESS_EVALUATION_INPUT_COMPONENTS"), v8::Uint32::New(GL_MAX_TESS_EVALUATION_INPUT_COMPONENTS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_COMBINED_TESS_CONTROL_UNIFORM_COMPONENTS"), v8::Uint32::New(GL_MAX_COMBINED_TESS_CONTROL_UNIFORM_COMPONENTS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_COMBINED_TESS_EVALUATION_UNIFORM_COMPONENTS"), v8::Uint32::New(GL_MAX_COMBINED_TESS_EVALUATION_UNIFORM_COMPONENTS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNIFORM_BLOCK_REFERENCED_BY_TESS_CONTROL_SHADER"), v8::Uint32::New(GL_UNIFORM_BLOCK_REFERENCED_BY_TESS_CONTROL_SHADER), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNIFORM_BLOCK_REFERENCED_BY_TESS_EVALUATION_SHADER"), v8::Uint32::New(GL_UNIFORM_BLOCK_REFERENCED_BY_TESS_EVALUATION_SHADER), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TESS_EVALUATION_SHADER"), v8::Uint32::New(GL_TESS_EVALUATION_SHADER), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TESS_CONTROL_SHADER"), v8::Uint32::New(GL_TESS_CONTROL_SHADER), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TRANSFORM_FEEDBACK"), v8::Uint32::New(GL_TRANSFORM_FEEDBACK), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TRANSFORM_FEEDBACK_BUFFER_PAUSED"), v8::Uint32::New(GL_TRANSFORM_FEEDBACK_BUFFER_PAUSED), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TRANSFORM_FEEDBACK_BUFFER_ACTIVE"), v8::Uint32::New(GL_TRANSFORM_FEEDBACK_BUFFER_ACTIVE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TRANSFORM_FEEDBACK_BINDING"), v8::Uint32::New(GL_TRANSFORM_FEEDBACK_BINDING), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_TRANSFORM_FEEDBACK_BUFFERS"), v8::Uint32::New(GL_MAX_TRANSFORM_FEEDBACK_BUFFERS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_VERTEX_STREAMS"), v8::Uint32::New(GL_MAX_VERTEX_STREAMS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("FIXED"), v8::Uint32::New(GL_FIXED), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("IMPLEMENTATION_COLOR_READ_TYPE"), v8::Uint32::New(GL_IMPLEMENTATION_COLOR_READ_TYPE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("IMPLEMENTATION_COLOR_READ_FORMAT"), v8::Uint32::New(GL_IMPLEMENTATION_COLOR_READ_FORMAT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("LOW_FLOAT"), v8::Uint32::New(GL_LOW_FLOAT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MEDIUM_FLOAT"), v8::Uint32::New(GL_MEDIUM_FLOAT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("HIGH_FLOAT"), v8::Uint32::New(GL_HIGH_FLOAT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("LOW_INT"), v8::Uint32::New(GL_LOW_INT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MEDIUM_INT"), v8::Uint32::New(GL_MEDIUM_INT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("HIGH_INT"), v8::Uint32::New(GL_HIGH_INT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("SHADER_COMPILER"), v8::Uint32::New(GL_SHADER_COMPILER), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("SHADER_BINARY_FORMATS"), v8::Uint32::New(GL_SHADER_BINARY_FORMATS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("NUM_SHADER_BINARY_FORMATS"), v8::Uint32::New(GL_NUM_SHADER_BINARY_FORMATS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_VERTEX_UNIFORM_VECTORS"), v8::Uint32::New(GL_MAX_VERTEX_UNIFORM_VECTORS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_VARYING_VECTORS"), v8::Uint32::New(GL_MAX_VARYING_VECTORS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_FRAGMENT_UNIFORM_VECTORS"), v8::Uint32::New(GL_MAX_FRAGMENT_UNIFORM_VECTORS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("RGB565"), v8::Uint32::New(GL_RGB565), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("PROGRAM_BINARY_RETRIEVABLE_HINT"), v8::Uint32::New(GL_PROGRAM_BINARY_RETRIEVABLE_HINT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("PROGRAM_BINARY_LENGTH"), v8::Uint32::New(GL_PROGRAM_BINARY_LENGTH), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("NUM_PROGRAM_BINARY_FORMATS"), v8::Uint32::New(GL_NUM_PROGRAM_BINARY_FORMATS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("PROGRAM_BINARY_FORMATS"), v8::Uint32::New(GL_PROGRAM_BINARY_FORMATS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("VERTEX_SHADER_BIT"), v8::Uint32::New(GL_VERTEX_SHADER_BIT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("FRAGMENT_SHADER_BIT"), v8::Uint32::New(GL_FRAGMENT_SHADER_BIT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("GEOMETRY_SHADER_BIT"), v8::Uint32::New(GL_GEOMETRY_SHADER_BIT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TESS_CONTROL_SHADER_BIT"), v8::Uint32::New(GL_TESS_CONTROL_SHADER_BIT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("TESS_EVALUATION_SHADER_BIT"), v8::Uint32::New(GL_TESS_EVALUATION_SHADER_BIT), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("ALL_SHADER_BITS"), v8::Uint32::New(GL_ALL_SHADER_BITS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("PROGRAM_SEPARABLE"), v8::Uint32::New(GL_PROGRAM_SEPARABLE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("ACTIVE_PROGRAM"), v8::Uint32::New(GL_ACTIVE_PROGRAM), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("PROGRAM_PIPELINE_BINDING"), v8::Uint32::New(GL_PROGRAM_PIPELINE_BINDING), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("MAX_VIEWPORTS"), v8::Uint32::New(GL_MAX_VIEWPORTS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("VIEWPORT_SUBPIXEL_BITS"), v8::Uint32::New(GL_VIEWPORT_SUBPIXEL_BITS), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("VIEWPORT_BOUNDS_RANGE"), v8::Uint32::New(GL_VIEWPORT_BOUNDS_RANGE), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("LAYER_PROVOKING_VERTEX"), v8::Uint32::New(GL_LAYER_PROVOKING_VERTEX), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("VIEWPORT_INDEX_PROVOKING_VERTEX"), v8::Uint32::New(GL_VIEWPORT_INDEX_PROVOKING_VERTEX), v8::ReadOnly);
    exports->Set(v8::String::NewSymbol("UNDEFINED_VERTEX"), v8::Uint32::New(GL_UNDEFINED_VERTEX), v8::ReadOnly);
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
v8::Handle<v8::Value> EXPORT_cullFace(const v8::Arguments& args) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    glCullFace(arg0);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_frontFace(const v8::Arguments& args) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    glFrontFace(arg0);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_hint(const v8::Arguments& args) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    glHint(arg0, arg1);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_lineWidth(const v8::Arguments& args) {
    NanScope();
    // None
    GLfloat arg0;
    arg0 = (float)args[0]->NumberValue();
    glLineWidth(arg0);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_pointSize(const v8::Arguments& args) {
    NanScope();
    // None
    GLfloat arg0;
    arg0 = (float)args[0]->NumberValue();
    glPointSize(arg0);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_polygonMode(const v8::Arguments& args) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    glPolygonMode(arg0, arg1);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_scissor(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_texParameterf(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_texParameterfv(const v8::Arguments& args) {
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
        arg2 = (const GLfloat*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLfloat*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glTexParameterfv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_texParameteri(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_texParameteriv(const v8::Arguments& args) {
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
        arg2 = (const GLint*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glTexParameteriv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_texImage1D(const v8::Arguments& args) {
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
        arg7 = (const GLvoid*)node::Buffer::Data(args[7]);
    } else if(args[7]->IsNumber()) {
        arg7 = (const GLvoid*)(args[7]->IntegerValue());
    } else {
        arg7 = NULL;
    }
    glTexImage1D(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
    if(arg7_nonconst) delete [] arg7_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_texImage2D(const v8::Arguments& args) {
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
        arg8 = (const GLvoid*)node::Buffer::Data(args[8]);
    } else if(args[8]->IsNumber()) {
        arg8 = (const GLvoid*)(args[8]->IntegerValue());
    } else {
        arg8 = NULL;
    }
    glTexImage2D(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
    if(arg8_nonconst) delete [] arg8_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_drawBuffer(const v8::Arguments& args) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    glDrawBuffer(arg0);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_clear(const v8::Arguments& args) {
    NanScope();
    // None
    GLbitfield arg0;
    arg0 = args[0]->Uint32Value();
    glClear(arg0);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_clearColor(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_clearStencil(const v8::Arguments& args) {
    NanScope();
    // None
    GLint arg0;
    arg0 = args[0]->Int32Value();
    glClearStencil(arg0);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_clearDepth(const v8::Arguments& args) {
    NanScope();
    // None
    GLclampd arg0;
    arg0 = args[0]->NumberValue();
    glClearDepth(arg0);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_stencilMask(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    glStencilMask(arg0);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_colorMask(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_depthMask(const v8::Arguments& args) {
    NanScope();
    // None
    GLboolean arg0;
    arg0 = args[0]->Uint32Value();
    glDepthMask(arg0);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_disable(const v8::Arguments& args) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    glDisable(arg0);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_enable(const v8::Arguments& args) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    glEnable(arg0);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_finish(const v8::Arguments& args) {
    NanScope();
    // None
    glFinish();
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_flush(const v8::Arguments& args) {
    NanScope();
    // None
    glFlush();
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_blendFunc(const v8::Arguments& args) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    glBlendFunc(arg0, arg1);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_logicOp(const v8::Arguments& args) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    glLogicOp(arg0);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_stencilFunc(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_stencilOp(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_depthFunc(const v8::Arguments& args) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    glDepthFunc(arg0);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_pixelStoref(const v8::Arguments& args) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLfloat arg1;
    arg1 = (float)args[1]->NumberValue();
    glPixelStoref(arg0, arg1);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_pixelStorei(const v8::Arguments& args) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLint arg1;
    arg1 = args[1]->Int32Value();
    glPixelStorei(arg0, arg1);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_readBuffer(const v8::Arguments& args) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    glReadBuffer(arg0);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_readPixels(const v8::Arguments& args) {
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
        arg6 = (GLvoid*)node::Buffer::Data(args[6]);
    } else if(args[6]->IsNumber()) {
        arg6 = (GLvoid*)(args[6]->IntegerValue());
    } else {
        arg6 = NULL;
    }
    glReadPixels(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
    if(arg6_nonconst) delete [] arg6_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_getBooleanv(const v8::Arguments& args) {
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
        arg1 = (GLboolean*)node::Buffer::Data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (GLboolean*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glGetBooleanv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_getDoublev(const v8::Arguments& args) {
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
        arg1 = (GLdouble*)node::Buffer::Data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (GLdouble*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glGetDoublev(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_getError(const v8::Arguments& args) {
    NanScope();
    // <gltypes.Type instance at 0x1036a16c8>
    GLenum result;
    result = glGetError();
    v8::Handle<v8::Value> result_js = v8::Uint32::New(result);
    NanReturnValue(result_js);
}
v8::Handle<v8::Value> EXPORT_getFloatv(const v8::Arguments& args) {
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
        arg1 = (GLfloat*)node::Buffer::Data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (GLfloat*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glGetFloatv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_getIntegerv(const v8::Arguments& args) {
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
        arg1 = (GLint*)node::Buffer::Data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (GLint*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glGetIntegerv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_getString(const v8::Arguments& args) {
    NanScope();
    // <gltypes.UStringType instance at 0x1036a1b90>
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    const GLubyte* result;
    result = glGetString(arg0);
    v8::Handle<v8::Value> result_js;
    if(result) {
        result_js = v8::String::New((const char*)result, strlen((const char*)result));
    } else {
        result_js = v8::Undefined();
    }
    NanReturnValue(result_js);
}
v8::Handle<v8::Value> EXPORT_getTexImage(const v8::Arguments& args) {
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
        arg4 = (GLvoid*)node::Buffer::Data(args[4]);
    } else if(args[4]->IsNumber()) {
        arg4 = (GLvoid*)(args[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glGetTexImage(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_getTexParameterfv(const v8::Arguments& args) {
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
        arg2 = (GLfloat*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLfloat*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetTexParameterfv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_getTexParameteriv(const v8::Arguments& args) {
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
        arg2 = (GLint*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetTexParameteriv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_getTexLevelParameterfv(const v8::Arguments& args) {
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
        arg3 = (GLfloat*)node::Buffer::Data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (GLfloat*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glGetTexLevelParameterfv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_getTexLevelParameteriv(const v8::Arguments& args) {
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
        arg3 = (GLint*)node::Buffer::Data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (GLint*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glGetTexLevelParameteriv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_isEnabled(const v8::Arguments& args) {
    NanScope();
    // <gltypes.Type instance at 0x1036a15f0>
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLboolean result;
    result = glIsEnabled(arg0);
    v8::Handle<v8::Value> result_js = v8::Uint32::New(result);
    NanReturnValue(result_js);
}
v8::Handle<v8::Value> EXPORT_depthRange(const v8::Arguments& args) {
    NanScope();
    // None
    GLclampd arg0;
    arg0 = args[0]->NumberValue();
    GLclampd arg1;
    arg1 = args[1]->NumberValue();
    glDepthRange(arg0, arg1);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_viewport(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_drawArrays(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_drawElements(const v8::Arguments& args) {
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
        arg3 = (const GLvoid*)node::Buffer::Data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLvoid*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glDrawElements(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_polygonOffset(const v8::Arguments& args) {
    NanScope();
    // None
    GLfloat arg0;
    arg0 = (float)args[0]->NumberValue();
    GLfloat arg1;
    arg1 = (float)args[1]->NumberValue();
    glPolygonOffset(arg0, arg1);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_copyTexImage1D(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_copyTexImage2D(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_copyTexSubImage1D(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_copyTexSubImage2D(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_texSubImage1D(const v8::Arguments& args) {
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
        arg6 = (const GLvoid*)node::Buffer::Data(args[6]);
    } else if(args[6]->IsNumber()) {
        arg6 = (const GLvoid*)(args[6]->IntegerValue());
    } else {
        arg6 = NULL;
    }
    glTexSubImage1D(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
    if(arg6_nonconst) delete [] arg6_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_texSubImage2D(const v8::Arguments& args) {
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
        arg8 = (const GLvoid*)node::Buffer::Data(args[8]);
    } else if(args[8]->IsNumber()) {
        arg8 = (const GLvoid*)(args[8]->IntegerValue());
    } else {
        arg8 = NULL;
    }
    glTexSubImage2D(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
    if(arg8_nonconst) delete [] arg8_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_bindTexture(const v8::Arguments& args) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLuint arg1;
    arg1 = node::ObjectWrap::Unwrap<NODE_Texture>(args[1]->ToObject())->gl_handle;
    glBindTexture(arg0, arg1);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_genTextures(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_isTexture(const v8::Arguments& args) {
    NanScope();
    // <gltypes.Type instance at 0x1036a15f0>
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Texture>(args[0]->ToObject())->gl_handle;
    GLboolean result;
    result = glIsTexture(arg0);
    v8::Handle<v8::Value> result_js = v8::Uint32::New(result);
    NanReturnValue(result_js);
}
v8::Handle<v8::Value> EXPORT_blendColor(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_blendEquation(const v8::Arguments& args) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    glBlendEquation(arg0);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_drawRangeElements(const v8::Arguments& args) {
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
        arg5 = (const GLvoid*)node::Buffer::Data(args[5]);
    } else if(args[5]->IsNumber()) {
        arg5 = (const GLvoid*)(args[5]->IntegerValue());
    } else {
        arg5 = NULL;
    }
    glDrawRangeElements(arg0, arg1, arg2, arg3, arg4, arg5);
    if(arg5_nonconst) delete [] arg5_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_texImage3D(const v8::Arguments& args) {
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
        arg9 = (const GLvoid*)node::Buffer::Data(args[9]);
    } else if(args[9]->IsNumber()) {
        arg9 = (const GLvoid*)(args[9]->IntegerValue());
    } else {
        arg9 = NULL;
    }
    glTexImage3D(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
    if(arg9_nonconst) delete [] arg9_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_texSubImage3D(const v8::Arguments& args) {
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
        arg10 = (const GLvoid*)node::Buffer::Data(args[10]);
    } else if(args[10]->IsNumber()) {
        arg10 = (const GLvoid*)(args[10]->IntegerValue());
    } else {
        arg10 = NULL;
    }
    glTexSubImage3D(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10);
    if(arg10_nonconst) delete [] arg10_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_copyTexSubImage3D(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_activeTexture(const v8::Arguments& args) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    glActiveTexture(arg0);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_sampleCoverage(const v8::Arguments& args) {
    NanScope();
    // None
    GLclampf arg0;
    arg0 = (float)args[0]->NumberValue();
    GLboolean arg1;
    arg1 = args[1]->Uint32Value();
    glSampleCoverage(arg0, arg1);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_compressedTexImage3D(const v8::Arguments& args) {
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
        arg8 = (const GLvoid*)node::Buffer::Data(args[8]);
    } else if(args[8]->IsNumber()) {
        arg8 = (const GLvoid*)(args[8]->IntegerValue());
    } else {
        arg8 = NULL;
    }
    glCompressedTexImage3D(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
    if(arg8_nonconst) delete [] arg8_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_compressedTexImage2D(const v8::Arguments& args) {
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
        arg7 = (const GLvoid*)node::Buffer::Data(args[7]);
    } else if(args[7]->IsNumber()) {
        arg7 = (const GLvoid*)(args[7]->IntegerValue());
    } else {
        arg7 = NULL;
    }
    glCompressedTexImage2D(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
    if(arg7_nonconst) delete [] arg7_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_compressedTexImage1D(const v8::Arguments& args) {
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
        arg6 = (const GLvoid*)node::Buffer::Data(args[6]);
    } else if(args[6]->IsNumber()) {
        arg6 = (const GLvoid*)(args[6]->IntegerValue());
    } else {
        arg6 = NULL;
    }
    glCompressedTexImage1D(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
    if(arg6_nonconst) delete [] arg6_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_compressedTexSubImage3D(const v8::Arguments& args) {
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
        arg10 = (const GLvoid*)node::Buffer::Data(args[10]);
    } else if(args[10]->IsNumber()) {
        arg10 = (const GLvoid*)(args[10]->IntegerValue());
    } else {
        arg10 = NULL;
    }
    glCompressedTexSubImage3D(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10);
    if(arg10_nonconst) delete [] arg10_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_compressedTexSubImage2D(const v8::Arguments& args) {
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
        arg8 = (const GLvoid*)node::Buffer::Data(args[8]);
    } else if(args[8]->IsNumber()) {
        arg8 = (const GLvoid*)(args[8]->IntegerValue());
    } else {
        arg8 = NULL;
    }
    glCompressedTexSubImage2D(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
    if(arg8_nonconst) delete [] arg8_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_compressedTexSubImage1D(const v8::Arguments& args) {
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
        arg6 = (const GLvoid*)node::Buffer::Data(args[6]);
    } else if(args[6]->IsNumber()) {
        arg6 = (const GLvoid*)(args[6]->IntegerValue());
    } else {
        arg6 = NULL;
    }
    glCompressedTexSubImage1D(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
    if(arg6_nonconst) delete [] arg6_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_getCompressedTexImage(const v8::Arguments& args) {
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
        arg2 = (GLvoid*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLvoid*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetCompressedTexImage(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_blendFuncSeparate(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_multiDrawArrays(const v8::Arguments& args) {
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
        arg1 = (const GLint*)node::Buffer::Data(args[1]);
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
        arg2 = (const GLsizei*)node::Buffer::Data(args[2]);
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
v8::Handle<v8::Value> EXPORT_pointParameterf(const v8::Arguments& args) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLfloat arg1;
    arg1 = (float)args[1]->NumberValue();
    glPointParameterf(arg0, arg1);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_pointParameterfv(const v8::Arguments& args) {
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
        arg1 = (const GLfloat*)node::Buffer::Data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLfloat*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glPointParameterfv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_pointParameteri(const v8::Arguments& args) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLint arg1;
    arg1 = args[1]->Int32Value();
    glPointParameteri(arg0, arg1);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_pointParameteriv(const v8::Arguments& args) {
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
        arg1 = (const GLint*)node::Buffer::Data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLint*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glPointParameteriv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_genQueries(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_isQuery(const v8::Arguments& args) {
    NanScope();
    // <gltypes.Type instance at 0x1036a15f0>
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Query>(args[0]->ToObject())->gl_handle;
    GLboolean result;
    result = glIsQuery(arg0);
    v8::Handle<v8::Value> result_js = v8::Uint32::New(result);
    NanReturnValue(result_js);
}
v8::Handle<v8::Value> EXPORT_beginQuery(const v8::Arguments& args) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLuint arg1;
    arg1 = node::ObjectWrap::Unwrap<NODE_Query>(args[1]->ToObject())->gl_handle;
    glBeginQuery(arg0, arg1);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_endQuery(const v8::Arguments& args) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    glEndQuery(arg0);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_getQueryiv(const v8::Arguments& args) {
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
        arg2 = (GLint*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetQueryiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_getQueryObjectiv(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Query>(args[0]->ToObject())->gl_handle;
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
        arg2 = (GLint*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetQueryObjectiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_getQueryObjectuiv(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Query>(args[0]->ToObject())->gl_handle;
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
        arg2 = (GLuint*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLuint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetQueryObjectuiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_bindBuffer(const v8::Arguments& args) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLuint arg1;
    arg1 = node::ObjectWrap::Unwrap<NODE_Buffer>(args[1]->ToObject())->gl_handle;
    glBindBuffer(arg0, arg1);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_genBuffers(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_isBuffer(const v8::Arguments& args) {
    NanScope();
    // <gltypes.Type instance at 0x1036a15f0>
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Buffer>(args[0]->ToObject())->gl_handle;
    GLboolean result;
    result = glIsBuffer(arg0);
    v8::Handle<v8::Value> result_js = v8::Uint32::New(result);
    NanReturnValue(result_js);
}
v8::Handle<v8::Value> EXPORT_bufferData(const v8::Arguments& args) {
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
        arg2 = (const GLvoid*)node::Buffer::Data(args[2]);
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
v8::Handle<v8::Value> EXPORT_bufferSubData(const v8::Arguments& args) {
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
        arg3 = (const GLvoid*)node::Buffer::Data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLvoid*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glBufferSubData(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_getBufferSubData(const v8::Arguments& args) {
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
        arg3 = (GLvoid*)node::Buffer::Data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (GLvoid*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glGetBufferSubData(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_mapBuffer(const v8::Arguments& args) {
    NanScope();
    // <gltypes.PointerType instance at 0x103a6d5f0>
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLvoid *result;
    result = glMapBuffer(arg0, arg1);
    v8::Handle<v8::Value> result_js = node::Buffer::New((char*)result, 0, do_nothing_release_callback, NULL)->handle_;
    NanReturnValue(result_js);
}
v8::Handle<v8::Value> EXPORT_unmapBuffer(const v8::Arguments& args) {
    NanScope();
    // <gltypes.Type instance at 0x1036a15f0>
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLboolean result;
    result = glUnmapBuffer(arg0);
    v8::Handle<v8::Value> result_js = v8::Uint32::New(result);
    NanReturnValue(result_js);
}
v8::Handle<v8::Value> EXPORT_getBufferParameteriv(const v8::Arguments& args) {
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
        arg2 = (GLint*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetBufferParameteriv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_blendEquationSeparate(const v8::Arguments& args) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    glBlendEquationSeparate(arg0, arg1);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_drawBuffers(const v8::Arguments& args) {
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
        arg1 = (const GLenum*)node::Buffer::Data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLenum*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glDrawBuffers(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_stencilOpSeparate(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_stencilFuncSeparate(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_stencilMaskSeparate(const v8::Arguments& args) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLuint arg1;
    arg1 = args[1]->Uint32Value();
    glStencilMaskSeparate(arg0, arg1);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_attachShader(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    GLuint arg1;
    arg1 = node::ObjectWrap::Unwrap<NODE_Shader>(args[1]->ToObject())->gl_handle;
    glAttachShader(arg0, arg1);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_bindAttribLocation(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    GLuint arg1;
    arg1 = args[1]->Uint32Value();
    const GLchar* arg2;
    NanUtf8String arg2_utf8(args[2]);
    arg2 = (GLchar*)*arg2_utf8;
    glBindAttribLocation(arg0, arg1, arg2);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_compileShader(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Shader>(args[0]->ToObject())->gl_handle;
    glCompileShader(arg0);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_createProgram(const v8::Arguments& args) {
    NanScope();
    // <gltypes.ClassType instance at 0x1036a1cb0>
    GLuint result;
    result = glCreateProgram();
    v8::Handle<v8::Value> result_js = NODE_Program::fromGLHandle(result);
    NanReturnValue(result_js);
}
v8::Handle<v8::Value> EXPORT_createShader(const v8::Arguments& args) {
    NanScope();
    // <gltypes.ClassType instance at 0x1036a1cf8>
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLuint result;
    result = glCreateShader(arg0);
    v8::Handle<v8::Value> result_js = NODE_Shader::fromGLHandle(result);
    NanReturnValue(result_js);
}
v8::Handle<v8::Value> EXPORT_detachShader(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    GLuint arg1;
    arg1 = node::ObjectWrap::Unwrap<NODE_Shader>(args[1]->ToObject())->gl_handle;
    glDetachShader(arg0, arg1);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_disableVertexAttribArray(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    glDisableVertexAttribArray(arg0);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_enableVertexAttribArray(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    glEnableVertexAttribArray(arg0);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_getActiveAttrib(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
        arg3 = (GLsizei*)node::Buffer::Data(args[3]);
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
        arg4 = (GLint*)node::Buffer::Data(args[4]);
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
        arg5 = (GLenum*)node::Buffer::Data(args[5]);
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
        arg6 = (GLchar*)node::Buffer::Data(args[6]);
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
v8::Handle<v8::Value> EXPORT_getActiveUniform(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
        arg3 = (GLsizei*)node::Buffer::Data(args[3]);
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
        arg4 = (GLint*)node::Buffer::Data(args[4]);
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
        arg5 = (GLenum*)node::Buffer::Data(args[5]);
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
        arg6 = (GLchar*)node::Buffer::Data(args[6]);
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
v8::Handle<v8::Value> EXPORT_getAttachedShaders(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
        arg2 = (GLsizei*)node::Buffer::Data(args[2]);
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
v8::Handle<v8::Value> EXPORT_getAttribLocation(const v8::Arguments& args) {
    NanScope();
    // <gltypes.Type instance at 0x1036a1758>
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    const GLchar* arg1;
    NanUtf8String arg1_utf8(args[1]);
    arg1 = (GLchar*)*arg1_utf8;
    GLint result;
    result = glGetAttribLocation(arg0, arg1);
    v8::Handle<v8::Value> result_js = v8::Int32::New(result);
    NanReturnValue(result_js);
}
v8::Handle<v8::Value> EXPORT_getProgramiv(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
        arg2 = (GLint*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetProgramiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_getProgramInfoLog(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
        arg2 = (GLsizei*)node::Buffer::Data(args[2]);
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
        arg3 = (GLchar*)node::Buffer::Data(args[3]);
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
v8::Handle<v8::Value> EXPORT_getShaderiv(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Shader>(args[0]->ToObject())->gl_handle;
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
        arg2 = (GLint*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetShaderiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_getShaderInfoLog(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Shader>(args[0]->ToObject())->gl_handle;
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
        arg2 = (GLsizei*)node::Buffer::Data(args[2]);
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
        arg3 = (GLchar*)node::Buffer::Data(args[3]);
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
v8::Handle<v8::Value> EXPORT_getShaderSource(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Shader>(args[0]->ToObject())->gl_handle;
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
        arg2 = (GLsizei*)node::Buffer::Data(args[2]);
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
        arg3 = (GLchar*)node::Buffer::Data(args[3]);
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
v8::Handle<v8::Value> EXPORT_getUniformLocation(const v8::Arguments& args) {
    NanScope();
    // <gltypes.Type instance at 0x1036a1758>
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    const GLchar* arg1;
    NanUtf8String arg1_utf8(args[1]);
    arg1 = (GLchar*)*arg1_utf8;
    GLint result;
    result = glGetUniformLocation(arg0, arg1);
    v8::Handle<v8::Value> result_js = v8::Int32::New(result);
    NanReturnValue(result_js);
}
v8::Handle<v8::Value> EXPORT_getUniformfv(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
        arg2 = (GLfloat*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLfloat*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetUniformfv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_getUniformiv(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
        arg2 = (GLint*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetUniformiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_getVertexAttribdv(const v8::Arguments& args) {
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
        arg2 = (GLdouble*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLdouble*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetVertexAttribdv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_getVertexAttribfv(const v8::Arguments& args) {
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
        arg2 = (GLfloat*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLfloat*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetVertexAttribfv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_getVertexAttribiv(const v8::Arguments& args) {
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
        arg2 = (GLint*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetVertexAttribiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_isProgram(const v8::Arguments& args) {
    NanScope();
    // <gltypes.Type instance at 0x1036a15f0>
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    GLboolean result;
    result = glIsProgram(arg0);
    v8::Handle<v8::Value> result_js = v8::Uint32::New(result);
    NanReturnValue(result_js);
}
v8::Handle<v8::Value> EXPORT_isShader(const v8::Arguments& args) {
    NanScope();
    // <gltypes.Type instance at 0x1036a15f0>
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Shader>(args[0]->ToObject())->gl_handle;
    GLboolean result;
    result = glIsShader(arg0);
    v8::Handle<v8::Value> result_js = v8::Uint32::New(result);
    NanReturnValue(result_js);
}
v8::Handle<v8::Value> EXPORT_linkProgram(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    glLinkProgram(arg0);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_useProgram(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    glUseProgram(arg0);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_uniform1f(const v8::Arguments& args) {
    NanScope();
    // None
    GLint arg0;
    arg0 = args[0]->Int32Value();
    GLfloat arg1;
    arg1 = (float)args[1]->NumberValue();
    glUniform1f(arg0, arg1);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_uniform2f(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_uniform3f(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_uniform4f(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_uniform1i(const v8::Arguments& args) {
    NanScope();
    // None
    GLint arg0;
    arg0 = args[0]->Int32Value();
    GLint arg1;
    arg1 = args[1]->Int32Value();
    glUniform1i(arg0, arg1);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_uniform2i(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_uniform3i(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_uniform4i(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_uniform1fv(const v8::Arguments& args) {
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
        arg2 = (const GLfloat*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLfloat*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glUniform1fv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_uniform2fv(const v8::Arguments& args) {
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
        arg2 = (const GLfloat*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLfloat*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glUniform2fv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_uniform3fv(const v8::Arguments& args) {
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
        arg2 = (const GLfloat*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLfloat*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glUniform3fv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_uniform4fv(const v8::Arguments& args) {
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
        arg2 = (const GLfloat*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLfloat*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glUniform4fv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_uniform1iv(const v8::Arguments& args) {
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
        arg2 = (const GLint*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glUniform1iv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_uniform2iv(const v8::Arguments& args) {
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
        arg2 = (const GLint*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glUniform2iv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_uniform3iv(const v8::Arguments& args) {
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
        arg2 = (const GLint*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glUniform3iv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_uniform4iv(const v8::Arguments& args) {
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
        arg2 = (const GLint*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glUniform4iv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_uniformMatrix2fv(const v8::Arguments& args) {
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
        arg3 = (const GLfloat*)node::Buffer::Data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLfloat*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glUniformMatrix2fv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_uniformMatrix3fv(const v8::Arguments& args) {
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
        arg3 = (const GLfloat*)node::Buffer::Data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLfloat*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glUniformMatrix3fv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_uniformMatrix4fv(const v8::Arguments& args) {
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
        arg3 = (const GLfloat*)node::Buffer::Data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLfloat*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glUniformMatrix4fv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_validateProgram(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    glValidateProgram(arg0);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_vertexAttrib1d(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLdouble arg1;
    arg1 = args[1]->NumberValue();
    glVertexAttrib1d(arg0, arg1);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_vertexAttrib1dv(const v8::Arguments& args) {
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
        arg1 = (const GLdouble*)node::Buffer::Data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLdouble*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib1dv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_vertexAttrib1f(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLfloat arg1;
    arg1 = (float)args[1]->NumberValue();
    glVertexAttrib1f(arg0, arg1);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_vertexAttrib1fv(const v8::Arguments& args) {
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
        arg1 = (const GLfloat*)node::Buffer::Data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLfloat*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib1fv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_vertexAttrib1s(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLshort arg1;
    arg1 = args[1]->Int32Value();
    glVertexAttrib1s(arg0, arg1);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_vertexAttrib1sv(const v8::Arguments& args) {
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
        arg1 = (const GLshort*)node::Buffer::Data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLshort*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib1sv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_vertexAttrib2d(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_vertexAttrib2dv(const v8::Arguments& args) {
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
        arg1 = (const GLdouble*)node::Buffer::Data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLdouble*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib2dv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_vertexAttrib2f(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_vertexAttrib2fv(const v8::Arguments& args) {
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
        arg1 = (const GLfloat*)node::Buffer::Data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLfloat*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib2fv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_vertexAttrib2s(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_vertexAttrib2sv(const v8::Arguments& args) {
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
        arg1 = (const GLshort*)node::Buffer::Data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLshort*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib2sv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_vertexAttrib3d(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_vertexAttrib3dv(const v8::Arguments& args) {
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
        arg1 = (const GLdouble*)node::Buffer::Data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLdouble*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib3dv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_vertexAttrib3f(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_vertexAttrib3fv(const v8::Arguments& args) {
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
        arg1 = (const GLfloat*)node::Buffer::Data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLfloat*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib3fv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_vertexAttrib3s(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_vertexAttrib3sv(const v8::Arguments& args) {
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
        arg1 = (const GLshort*)node::Buffer::Data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLshort*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib3sv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_vertexAttrib4Nbv(const v8::Arguments& args) {
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
        arg1 = (const GLbyte*)node::Buffer::Data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLbyte*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib4Nbv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_vertexAttrib4Niv(const v8::Arguments& args) {
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
        arg1 = (const GLint*)node::Buffer::Data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLint*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib4Niv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_vertexAttrib4Nsv(const v8::Arguments& args) {
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
        arg1 = (const GLshort*)node::Buffer::Data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLshort*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib4Nsv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_vertexAttrib4Nub(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_vertexAttrib4Nubv(const v8::Arguments& args) {
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
        arg1 = (const GLubyte*)node::Buffer::Data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLubyte*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib4Nubv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_vertexAttrib4Nuiv(const v8::Arguments& args) {
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
        arg1 = (const GLuint*)node::Buffer::Data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLuint*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib4Nuiv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_vertexAttrib4Nusv(const v8::Arguments& args) {
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
        arg1 = (const GLushort*)node::Buffer::Data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLushort*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib4Nusv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_vertexAttrib4bv(const v8::Arguments& args) {
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
        arg1 = (const GLbyte*)node::Buffer::Data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLbyte*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib4bv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_vertexAttrib4d(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_vertexAttrib4dv(const v8::Arguments& args) {
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
        arg1 = (const GLdouble*)node::Buffer::Data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLdouble*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib4dv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_vertexAttrib4f(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_vertexAttrib4fv(const v8::Arguments& args) {
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
        arg1 = (const GLfloat*)node::Buffer::Data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLfloat*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib4fv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_vertexAttrib4iv(const v8::Arguments& args) {
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
        arg1 = (const GLint*)node::Buffer::Data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLint*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib4iv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_vertexAttrib4s(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_vertexAttrib4sv(const v8::Arguments& args) {
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
        arg1 = (const GLshort*)node::Buffer::Data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLshort*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib4sv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_vertexAttrib4ubv(const v8::Arguments& args) {
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
        arg1 = (const GLubyte*)node::Buffer::Data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLubyte*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib4ubv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_vertexAttrib4uiv(const v8::Arguments& args) {
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
        arg1 = (const GLuint*)node::Buffer::Data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLuint*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib4uiv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_vertexAttrib4usv(const v8::Arguments& args) {
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
        arg1 = (const GLushort*)node::Buffer::Data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLushort*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttrib4usv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_vertexAttribPointer(const v8::Arguments& args) {
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
        arg5 = (const GLvoid*)node::Buffer::Data(args[5]);
    } else if(args[5]->IsNumber()) {
        arg5 = (const GLvoid*)(args[5]->IntegerValue());
    } else {
        arg5 = NULL;
    }
    glVertexAttribPointer(arg0, arg1, arg2, arg3, arg4, arg5);
    if(arg5_nonconst) delete [] arg5_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_uniformMatrix2x3fv(const v8::Arguments& args) {
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
        arg3 = (const GLfloat*)node::Buffer::Data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLfloat*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glUniformMatrix2x3fv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_uniformMatrix3x2fv(const v8::Arguments& args) {
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
        arg3 = (const GLfloat*)node::Buffer::Data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLfloat*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glUniformMatrix3x2fv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_uniformMatrix2x4fv(const v8::Arguments& args) {
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
        arg3 = (const GLfloat*)node::Buffer::Data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLfloat*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glUniformMatrix2x4fv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_uniformMatrix4x2fv(const v8::Arguments& args) {
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
        arg3 = (const GLfloat*)node::Buffer::Data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLfloat*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glUniformMatrix4x2fv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_uniformMatrix3x4fv(const v8::Arguments& args) {
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
        arg3 = (const GLfloat*)node::Buffer::Data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLfloat*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glUniformMatrix3x4fv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_uniformMatrix4x3fv(const v8::Arguments& args) {
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
        arg3 = (const GLfloat*)node::Buffer::Data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLfloat*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glUniformMatrix4x3fv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_colorMaski(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_getBooleani_v(const v8::Arguments& args) {
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
        arg2 = (GLboolean*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLboolean*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetBooleani_v(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_getIntegeri_v(const v8::Arguments& args) {
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
        arg2 = (GLint*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetIntegeri_v(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_enablei(const v8::Arguments& args) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLuint arg1;
    arg1 = args[1]->Uint32Value();
    glEnablei(arg0, arg1);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_disablei(const v8::Arguments& args) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLuint arg1;
    arg1 = args[1]->Uint32Value();
    glDisablei(arg0, arg1);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_isEnabledi(const v8::Arguments& args) {
    NanScope();
    // <gltypes.Type instance at 0x1036a15f0>
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLuint arg1;
    arg1 = args[1]->Uint32Value();
    GLboolean result;
    result = glIsEnabledi(arg0, arg1);
    v8::Handle<v8::Value> result_js = v8::Uint32::New(result);
    NanReturnValue(result_js);
}
v8::Handle<v8::Value> EXPORT_beginTransformFeedback(const v8::Arguments& args) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    glBeginTransformFeedback(arg0);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_endTransformFeedback(const v8::Arguments& args) {
    NanScope();
    // None
    glEndTransformFeedback();
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_bindBufferRange(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_bindBufferBase(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_getTransformFeedbackVarying(const v8::Arguments& args) {
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
        arg3 = (GLsizei*)node::Buffer::Data(args[3]);
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
        arg4 = (GLsizei*)node::Buffer::Data(args[4]);
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
        arg5 = (GLenum*)node::Buffer::Data(args[5]);
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
        arg6 = (GLchar*)node::Buffer::Data(args[6]);
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
v8::Handle<v8::Value> EXPORT_clampColor(const v8::Arguments& args) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    glClampColor(arg0, arg1);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_beginConditionalRender(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    glBeginConditionalRender(arg0, arg1);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_endConditionalRender(const v8::Arguments& args) {
    NanScope();
    // None
    glEndConditionalRender();
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_vertexAttribIPointer(const v8::Arguments& args) {
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
        arg4 = (const GLvoid*)node::Buffer::Data(args[4]);
    } else if(args[4]->IsNumber()) {
        arg4 = (const GLvoid*)(args[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glVertexAttribIPointer(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_getVertexAttribIiv(const v8::Arguments& args) {
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
        arg2 = (GLint*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetVertexAttribIiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_getVertexAttribIuiv(const v8::Arguments& args) {
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
        arg2 = (GLuint*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLuint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetVertexAttribIuiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_vertexAttribI1i(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLint arg1;
    arg1 = args[1]->Int32Value();
    glVertexAttribI1i(arg0, arg1);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_vertexAttribI2i(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_vertexAttribI3i(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_vertexAttribI4i(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_vertexAttribI1ui(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLuint arg1;
    arg1 = args[1]->Uint32Value();
    glVertexAttribI1ui(arg0, arg1);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_vertexAttribI2ui(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_vertexAttribI3ui(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_vertexAttribI4ui(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_vertexAttribI1iv(const v8::Arguments& args) {
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
        arg1 = (const GLint*)node::Buffer::Data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLint*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttribI1iv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_vertexAttribI2iv(const v8::Arguments& args) {
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
        arg1 = (const GLint*)node::Buffer::Data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLint*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttribI2iv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_vertexAttribI3iv(const v8::Arguments& args) {
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
        arg1 = (const GLint*)node::Buffer::Data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLint*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttribI3iv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_vertexAttribI4iv(const v8::Arguments& args) {
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
        arg1 = (const GLint*)node::Buffer::Data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLint*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttribI4iv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_vertexAttribI1uiv(const v8::Arguments& args) {
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
        arg1 = (const GLuint*)node::Buffer::Data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLuint*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttribI1uiv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_vertexAttribI2uiv(const v8::Arguments& args) {
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
        arg1 = (const GLuint*)node::Buffer::Data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLuint*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttribI2uiv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_vertexAttribI3uiv(const v8::Arguments& args) {
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
        arg1 = (const GLuint*)node::Buffer::Data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLuint*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttribI3uiv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_vertexAttribI4uiv(const v8::Arguments& args) {
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
        arg1 = (const GLuint*)node::Buffer::Data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLuint*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttribI4uiv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_vertexAttribI4bv(const v8::Arguments& args) {
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
        arg1 = (const GLbyte*)node::Buffer::Data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLbyte*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttribI4bv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_vertexAttribI4sv(const v8::Arguments& args) {
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
        arg1 = (const GLshort*)node::Buffer::Data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLshort*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttribI4sv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_vertexAttribI4ubv(const v8::Arguments& args) {
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
        arg1 = (const GLubyte*)node::Buffer::Data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLubyte*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttribI4ubv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_vertexAttribI4usv(const v8::Arguments& args) {
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
        arg1 = (const GLushort*)node::Buffer::Data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLushort*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttribI4usv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_getUniformuiv(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
        arg2 = (GLuint*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLuint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetUniformuiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_bindFragDataLocation(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    GLuint arg1;
    arg1 = args[1]->Uint32Value();
    const GLchar* arg2;
    NanUtf8String arg2_utf8(args[2]);
    arg2 = (GLchar*)*arg2_utf8;
    glBindFragDataLocation(arg0, arg1, arg2);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_getFragDataLocation(const v8::Arguments& args) {
    NanScope();
    // <gltypes.Type instance at 0x1036a1758>
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    const GLchar* arg1;
    NanUtf8String arg1_utf8(args[1]);
    arg1 = (GLchar*)*arg1_utf8;
    GLint result;
    result = glGetFragDataLocation(arg0, arg1);
    v8::Handle<v8::Value> result_js = v8::Int32::New(result);
    NanReturnValue(result_js);
}
v8::Handle<v8::Value> EXPORT_uniform1ui(const v8::Arguments& args) {
    NanScope();
    // None
    GLint arg0;
    arg0 = args[0]->Int32Value();
    GLuint arg1;
    arg1 = args[1]->Uint32Value();
    glUniform1ui(arg0, arg1);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_uniform2ui(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_uniform3ui(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_uniform4ui(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_uniform1uiv(const v8::Arguments& args) {
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
        arg2 = (const GLuint*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLuint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glUniform1uiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_uniform2uiv(const v8::Arguments& args) {
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
        arg2 = (const GLuint*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLuint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glUniform2uiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_uniform3uiv(const v8::Arguments& args) {
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
        arg2 = (const GLuint*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLuint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glUniform3uiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_uniform4uiv(const v8::Arguments& args) {
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
        arg2 = (const GLuint*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLuint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glUniform4uiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_texParameterIiv(const v8::Arguments& args) {
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
        arg2 = (const GLint*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glTexParameterIiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_texParameterIuiv(const v8::Arguments& args) {
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
        arg2 = (const GLuint*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLuint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glTexParameterIuiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_getTexParameterIiv(const v8::Arguments& args) {
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
        arg2 = (GLint*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetTexParameterIiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_getTexParameterIuiv(const v8::Arguments& args) {
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
        arg2 = (GLuint*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLuint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetTexParameterIuiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_clearBufferiv(const v8::Arguments& args) {
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
        arg2 = (const GLint*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glClearBufferiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_clearBufferuiv(const v8::Arguments& args) {
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
        arg2 = (const GLuint*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLuint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glClearBufferuiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_clearBufferfv(const v8::Arguments& args) {
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
        arg2 = (const GLfloat*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLfloat*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glClearBufferfv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_clearBufferfi(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_getStringi(const v8::Arguments& args) {
    NanScope();
    // <gltypes.UStringType instance at 0x1036a1b90>
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLuint arg1;
    arg1 = args[1]->Uint32Value();
    const GLubyte* result;
    result = glGetStringi(arg0, arg1);
    v8::Handle<v8::Value> result_js;
    if(result) {
        result_js = v8::String::New((const char*)result, strlen((const char*)result));
    } else {
        result_js = v8::Undefined();
    }
    NanReturnValue(result_js);
}
v8::Handle<v8::Value> EXPORT_drawArraysInstanced(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_drawElementsInstanced(const v8::Arguments& args) {
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
        arg3 = (const GLvoid*)node::Buffer::Data(args[3]);
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
v8::Handle<v8::Value> EXPORT_texBuffer(const v8::Arguments& args) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLuint arg2;
    arg2 = node::ObjectWrap::Unwrap<NODE_Buffer>(args[2]->ToObject())->gl_handle;
    glTexBuffer(arg0, arg1, arg2);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_primitiveRestartIndex(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    glPrimitiveRestartIndex(arg0);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_getInteger64i_v(const v8::Arguments& args) {
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
        arg2 = (GLint64*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLint64*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetInteger64i_v(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_getBufferParameteri64v(const v8::Arguments& args) {
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
        arg2 = (GLint64*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLint64*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetBufferParameteri64v(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_framebufferTexture(const v8::Arguments& args) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLuint arg2;
    arg2 = node::ObjectWrap::Unwrap<NODE_Texture>(args[2]->ToObject())->gl_handle;
    GLint arg3;
    arg3 = args[3]->Int32Value();
    glFramebufferTexture(arg0, arg1, arg2, arg3);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_vertexAttribDivisor(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLuint arg1;
    arg1 = args[1]->Uint32Value();
    glVertexAttribDivisor(arg0, arg1);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_minSampleShading(const v8::Arguments& args) {
    NanScope();
    // None
    GLfloat arg0;
    arg0 = (float)args[0]->NumberValue();
    glMinSampleShading(arg0);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_blendEquationi(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    glBlendEquationi(arg0, arg1);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_blendEquationSeparatei(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_blendFunci(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_blendFuncSeparatei(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_isRenderbuffer(const v8::Arguments& args) {
    NanScope();
    // <gltypes.Type instance at 0x1036a15f0>
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Renderbuffer>(args[0]->ToObject())->gl_handle;
    GLboolean result;
    result = glIsRenderbuffer(arg0);
    v8::Handle<v8::Value> result_js = v8::Uint32::New(result);
    NanReturnValue(result_js);
}
v8::Handle<v8::Value> EXPORT_bindRenderbuffer(const v8::Arguments& args) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLuint arg1;
    arg1 = node::ObjectWrap::Unwrap<NODE_Renderbuffer>(args[1]->ToObject())->gl_handle;
    glBindRenderbuffer(arg0, arg1);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_genRenderbuffers(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_renderbufferStorage(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_getRenderbufferParameteriv(const v8::Arguments& args) {
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
        arg2 = (GLint*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetRenderbufferParameteriv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_isFramebuffer(const v8::Arguments& args) {
    NanScope();
    // <gltypes.Type instance at 0x1036a15f0>
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Framebuffer>(args[0]->ToObject())->gl_handle;
    GLboolean result;
    result = glIsFramebuffer(arg0);
    v8::Handle<v8::Value> result_js = v8::Uint32::New(result);
    NanReturnValue(result_js);
}
v8::Handle<v8::Value> EXPORT_bindFramebuffer(const v8::Arguments& args) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLuint arg1;
    arg1 = node::ObjectWrap::Unwrap<NODE_Framebuffer>(args[1]->ToObject())->gl_handle;
    glBindFramebuffer(arg0, arg1);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_genFramebuffers(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_checkFramebufferStatus(const v8::Arguments& args) {
    NanScope();
    // <gltypes.Type instance at 0x1036a16c8>
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLenum result;
    result = glCheckFramebufferStatus(arg0);
    v8::Handle<v8::Value> result_js = v8::Uint32::New(result);
    NanReturnValue(result_js);
}
v8::Handle<v8::Value> EXPORT_framebufferTexture1D(const v8::Arguments& args) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLenum arg2;
    arg2 = args[2]->Uint32Value();
    GLuint arg3;
    arg3 = node::ObjectWrap::Unwrap<NODE_Texture>(args[3]->ToObject())->gl_handle;
    GLint arg4;
    arg4 = args[4]->Int32Value();
    glFramebufferTexture1D(arg0, arg1, arg2, arg3, arg4);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_framebufferTexture2D(const v8::Arguments& args) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLenum arg2;
    arg2 = args[2]->Uint32Value();
    GLuint arg3;
    arg3 = node::ObjectWrap::Unwrap<NODE_Texture>(args[3]->ToObject())->gl_handle;
    GLint arg4;
    arg4 = args[4]->Int32Value();
    glFramebufferTexture2D(arg0, arg1, arg2, arg3, arg4);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_framebufferTexture3D(const v8::Arguments& args) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLenum arg2;
    arg2 = args[2]->Uint32Value();
    GLuint arg3;
    arg3 = node::ObjectWrap::Unwrap<NODE_Texture>(args[3]->ToObject())->gl_handle;
    GLint arg4;
    arg4 = args[4]->Int32Value();
    GLint arg5;
    arg5 = args[5]->Int32Value();
    glFramebufferTexture3D(arg0, arg1, arg2, arg3, arg4, arg5);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_framebufferRenderbuffer(const v8::Arguments& args) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLenum arg2;
    arg2 = args[2]->Uint32Value();
    GLuint arg3;
    arg3 = node::ObjectWrap::Unwrap<NODE_Renderbuffer>(args[3]->ToObject())->gl_handle;
    glFramebufferRenderbuffer(arg0, arg1, arg2, arg3);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_getFramebufferAttachmentParameteriv(const v8::Arguments& args) {
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
        arg3 = (GLint*)node::Buffer::Data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (GLint*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glGetFramebufferAttachmentParameteriv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_generateMipmap(const v8::Arguments& args) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    glGenerateMipmap(arg0);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_blitFramebuffer(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_renderbufferStorageMultisample(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_framebufferTextureLayer(const v8::Arguments& args) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLuint arg2;
    arg2 = node::ObjectWrap::Unwrap<NODE_Texture>(args[2]->ToObject())->gl_handle;
    GLint arg3;
    arg3 = args[3]->Int32Value();
    GLint arg4;
    arg4 = args[4]->Int32Value();
    glFramebufferTextureLayer(arg0, arg1, arg2, arg3, arg4);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_mapBufferRange(const v8::Arguments& args) {
    NanScope();
    // <gltypes.PointerType instance at 0x103a6def0>
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
    v8::Handle<v8::Value> result_js = node::Buffer::New((char*)result, 0, do_nothing_release_callback, NULL)->handle_;
    NanReturnValue(result_js);
}
v8::Handle<v8::Value> EXPORT_flushMappedBufferRange(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_bindVertexArray(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_VertexArray>(args[0]->ToObject())->gl_handle;
    glBindVertexArray(arg0);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_genVertexArrays(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_isVertexArray(const v8::Arguments& args) {
    NanScope();
    // <gltypes.Type instance at 0x1036a15f0>
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_VertexArray>(args[0]->ToObject())->gl_handle;
    GLboolean result;
    result = glIsVertexArray(arg0);
    v8::Handle<v8::Value> result_js = v8::Uint32::New(result);
    NanReturnValue(result_js);
}
v8::Handle<v8::Value> EXPORT_getActiveUniformsiv(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
        arg2 = (const GLuint*)node::Buffer::Data(args[2]);
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
        arg4 = (GLint*)node::Buffer::Data(args[4]);
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
v8::Handle<v8::Value> EXPORT_getActiveUniformName(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
        arg3 = (GLsizei*)node::Buffer::Data(args[3]);
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
        arg4 = (GLchar*)node::Buffer::Data(args[4]);
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
v8::Handle<v8::Value> EXPORT_getUniformBlockIndex(const v8::Arguments& args) {
    NanScope();
    // <gltypes.Type instance at 0x1036a1878>
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
        arg1 = (const GLchar*)node::Buffer::Data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLchar*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    GLuint result;
    result = glGetUniformBlockIndex(arg0, arg1);
    v8::Handle<v8::Value> result_js = v8::Uint32::New(result);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnValue(result_js);
}
v8::Handle<v8::Value> EXPORT_getActiveUniformBlockiv(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
        arg3 = (GLint*)node::Buffer::Data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (GLint*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glGetActiveUniformBlockiv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_getActiveUniformBlockName(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
        arg3 = (GLsizei*)node::Buffer::Data(args[3]);
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
        arg4 = (GLchar*)node::Buffer::Data(args[4]);
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
v8::Handle<v8::Value> EXPORT_uniformBlockBinding(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    GLuint arg1;
    arg1 = args[1]->Uint32Value();
    GLuint arg2;
    arg2 = args[2]->Uint32Value();
    glUniformBlockBinding(arg0, arg1, arg2);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_copyBufferSubData(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_drawElementsBaseVertex(const v8::Arguments& args) {
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
        arg3 = (const GLvoid*)node::Buffer::Data(args[3]);
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
v8::Handle<v8::Value> EXPORT_drawRangeElementsBaseVertex(const v8::Arguments& args) {
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
        arg5 = (const GLvoid*)node::Buffer::Data(args[5]);
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
v8::Handle<v8::Value> EXPORT_drawElementsInstancedBaseVertex(const v8::Arguments& args) {
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
        arg3 = (const GLvoid*)node::Buffer::Data(args[3]);
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
v8::Handle<v8::Value> EXPORT_provokingVertex(const v8::Arguments& args) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    glProvokingVertex(arg0);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_texImage2DMultisample(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_texImage3DMultisample(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_getMultisamplefv(const v8::Arguments& args) {
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
        arg2 = (GLfloat*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLfloat*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetMultisamplefv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_sampleMaski(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLbitfield arg1;
    arg1 = args[1]->Uint32Value();
    glSampleMaski(arg0, arg1);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_bindFragDataLocationIndexed(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
        arg3 = (const GLchar*)node::Buffer::Data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLchar*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glBindFragDataLocationIndexed(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_getFragDataIndex(const v8::Arguments& args) {
    NanScope();
    // <gltypes.Type instance at 0x1036a1758>
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
        arg1 = (const GLchar*)node::Buffer::Data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLchar*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    GLint result;
    result = glGetFragDataIndex(arg0, arg1);
    v8::Handle<v8::Value> result_js = v8::Int32::New(result);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnValue(result_js);
}
v8::Handle<v8::Value> EXPORT_genSamplers(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_isSampler(const v8::Arguments& args) {
    NanScope();
    // <gltypes.Type instance at 0x1036a15f0>
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Sampler>(args[0]->ToObject())->gl_handle;
    GLboolean result;
    result = glIsSampler(arg0);
    v8::Handle<v8::Value> result_js = v8::Uint32::New(result);
    NanReturnValue(result_js);
}
v8::Handle<v8::Value> EXPORT_bindSampler(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLuint arg1;
    arg1 = node::ObjectWrap::Unwrap<NODE_Sampler>(args[1]->ToObject())->gl_handle;
    glBindSampler(arg0, arg1);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_samplerParameteri(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Sampler>(args[0]->ToObject())->gl_handle;
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLint arg2;
    arg2 = args[2]->Int32Value();
    glSamplerParameteri(arg0, arg1, arg2);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_samplerParameteriv(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Sampler>(args[0]->ToObject())->gl_handle;
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
        arg2 = (const GLint*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glSamplerParameteriv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_samplerParameterf(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Sampler>(args[0]->ToObject())->gl_handle;
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLfloat arg2;
    arg2 = (float)args[2]->NumberValue();
    glSamplerParameterf(arg0, arg1, arg2);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_samplerParameterfv(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Sampler>(args[0]->ToObject())->gl_handle;
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
        arg2 = (const GLfloat*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLfloat*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glSamplerParameterfv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_samplerParameterIiv(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Sampler>(args[0]->ToObject())->gl_handle;
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
        arg2 = (const GLint*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glSamplerParameterIiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_samplerParameterIuiv(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Sampler>(args[0]->ToObject())->gl_handle;
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
        arg2 = (const GLuint*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLuint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glSamplerParameterIuiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_getSamplerParameteriv(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Sampler>(args[0]->ToObject())->gl_handle;
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
        arg2 = (GLint*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetSamplerParameteriv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_getSamplerParameterIiv(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Sampler>(args[0]->ToObject())->gl_handle;
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
        arg2 = (GLint*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetSamplerParameterIiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_getSamplerParameterfv(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Sampler>(args[0]->ToObject())->gl_handle;
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
        arg2 = (GLfloat*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLfloat*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetSamplerParameterfv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_getSamplerParameterIuiv(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Sampler>(args[0]->ToObject())->gl_handle;
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
        arg2 = (GLuint*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLuint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetSamplerParameterIuiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_queryCounter(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    glQueryCounter(arg0, arg1);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_getQueryObjecti64v(const v8::Arguments& args) {
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
        arg2 = (GLint64*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLint64*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetQueryObjecti64v(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_getQueryObjectui64v(const v8::Arguments& args) {
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
        arg2 = (GLuint64*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLuint64*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetQueryObjectui64v(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_vertexAttribP1ui(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_vertexAttribP1uiv(const v8::Arguments& args) {
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
        arg3 = (const GLuint*)node::Buffer::Data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLuint*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glVertexAttribP1uiv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_vertexAttribP2ui(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_vertexAttribP2uiv(const v8::Arguments& args) {
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
        arg3 = (const GLuint*)node::Buffer::Data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLuint*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glVertexAttribP2uiv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_vertexAttribP3ui(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_vertexAttribP3uiv(const v8::Arguments& args) {
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
        arg3 = (const GLuint*)node::Buffer::Data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLuint*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glVertexAttribP3uiv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_vertexAttribP4ui(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_vertexAttribP4uiv(const v8::Arguments& args) {
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
        arg3 = (const GLuint*)node::Buffer::Data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLuint*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glVertexAttribP4uiv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_drawArraysIndirect(const v8::Arguments& args) {
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
        arg1 = (const GLvoid*)node::Buffer::Data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLvoid*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glDrawArraysIndirect(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_drawElementsIndirect(const v8::Arguments& args) {
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
        arg2 = (const GLvoid*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLvoid*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glDrawElementsIndirect(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_uniform1d(const v8::Arguments& args) {
    NanScope();
    // None
    GLint arg0;
    arg0 = args[0]->Int32Value();
    GLdouble arg1;
    arg1 = args[1]->NumberValue();
    glUniform1d(arg0, arg1);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_uniform2d(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_uniform3d(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_uniform4d(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_uniform1dv(const v8::Arguments& args) {
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
        arg2 = (const GLdouble*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLdouble*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glUniform1dv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_uniform2dv(const v8::Arguments& args) {
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
        arg2 = (const GLdouble*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLdouble*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glUniform2dv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_uniform3dv(const v8::Arguments& args) {
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
        arg2 = (const GLdouble*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLdouble*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glUniform3dv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_uniform4dv(const v8::Arguments& args) {
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
        arg2 = (const GLdouble*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLdouble*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glUniform4dv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_uniformMatrix2dv(const v8::Arguments& args) {
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
        arg3 = (const GLdouble*)node::Buffer::Data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLdouble*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glUniformMatrix2dv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_uniformMatrix3dv(const v8::Arguments& args) {
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
        arg3 = (const GLdouble*)node::Buffer::Data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLdouble*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glUniformMatrix3dv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_uniformMatrix4dv(const v8::Arguments& args) {
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
        arg3 = (const GLdouble*)node::Buffer::Data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLdouble*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glUniformMatrix4dv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_uniformMatrix2x3dv(const v8::Arguments& args) {
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
        arg3 = (const GLdouble*)node::Buffer::Data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLdouble*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glUniformMatrix2x3dv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_uniformMatrix2x4dv(const v8::Arguments& args) {
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
        arg3 = (const GLdouble*)node::Buffer::Data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLdouble*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glUniformMatrix2x4dv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_uniformMatrix3x2dv(const v8::Arguments& args) {
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
        arg3 = (const GLdouble*)node::Buffer::Data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLdouble*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glUniformMatrix3x2dv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_uniformMatrix3x4dv(const v8::Arguments& args) {
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
        arg3 = (const GLdouble*)node::Buffer::Data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLdouble*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glUniformMatrix3x4dv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_uniformMatrix4x2dv(const v8::Arguments& args) {
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
        arg3 = (const GLdouble*)node::Buffer::Data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLdouble*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glUniformMatrix4x2dv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_uniformMatrix4x3dv(const v8::Arguments& args) {
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
        arg3 = (const GLdouble*)node::Buffer::Data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLdouble*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glUniformMatrix4x3dv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_getUniformdv(const v8::Arguments& args) {
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
        arg2 = (GLdouble*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLdouble*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetUniformdv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_getSubroutineUniformLocation(const v8::Arguments& args) {
    NanScope();
    // <gltypes.Type instance at 0x1036a1758>
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
        arg2 = (const GLchar*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLchar*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    GLint result;
    result = glGetSubroutineUniformLocation(arg0, arg1, arg2);
    v8::Handle<v8::Value> result_js = v8::Int32::New(result);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnValue(result_js);
}
v8::Handle<v8::Value> EXPORT_getSubroutineIndex(const v8::Arguments& args) {
    NanScope();
    // <gltypes.Type instance at 0x1036a1878>
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
        arg2 = (const GLchar*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLchar*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    GLuint result;
    result = glGetSubroutineIndex(arg0, arg1, arg2);
    v8::Handle<v8::Value> result_js = v8::Uint32::New(result);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnValue(result_js);
}
v8::Handle<v8::Value> EXPORT_getActiveSubroutineUniformiv(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
        arg4 = (GLint*)node::Buffer::Data(args[4]);
    } else if(args[4]->IsNumber()) {
        arg4 = (GLint*)(args[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glGetActiveSubroutineUniformiv(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_getActiveSubroutineUniformName(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
        arg4 = (GLsizei*)node::Buffer::Data(args[4]);
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
        arg5 = (GLchar*)node::Buffer::Data(args[5]);
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
v8::Handle<v8::Value> EXPORT_getActiveSubroutineName(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
        arg4 = (GLsizei*)node::Buffer::Data(args[4]);
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
        arg5 = (GLchar*)node::Buffer::Data(args[5]);
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
v8::Handle<v8::Value> EXPORT_uniformSubroutinesuiv(const v8::Arguments& args) {
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
        arg2 = (const GLuint*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLuint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glUniformSubroutinesuiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_getUniformSubroutineuiv(const v8::Arguments& args) {
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
        arg2 = (GLuint*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLuint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetUniformSubroutineuiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_getProgramStageiv(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
        arg3 = (GLint*)node::Buffer::Data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (GLint*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glGetProgramStageiv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_patchParameteri(const v8::Arguments& args) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLint arg1;
    arg1 = args[1]->Int32Value();
    glPatchParameteri(arg0, arg1);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_patchParameterfv(const v8::Arguments& args) {
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
        arg1 = (const GLfloat*)node::Buffer::Data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLfloat*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glPatchParameterfv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_bindTransformFeedback(const v8::Arguments& args) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLuint arg1;
    arg1 = node::ObjectWrap::Unwrap<NODE_TransformFeedback>(args[1]->ToObject())->gl_handle;
    glBindTransformFeedback(arg0, arg1);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_genTransformFeedbacks(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_isTransformFeedback(const v8::Arguments& args) {
    NanScope();
    // <gltypes.Type instance at 0x1036a15f0>
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_TransformFeedback>(args[0]->ToObject())->gl_handle;
    GLboolean result;
    result = glIsTransformFeedback(arg0);
    v8::Handle<v8::Value> result_js = v8::Uint32::New(result);
    NanReturnValue(result_js);
}
v8::Handle<v8::Value> EXPORT_pauseTransformFeedback(const v8::Arguments& args) {
    NanScope();
    // None
    glPauseTransformFeedback();
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_resumeTransformFeedback(const v8::Arguments& args) {
    NanScope();
    // None
    glResumeTransformFeedback();
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_drawTransformFeedback(const v8::Arguments& args) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLuint arg1;
    arg1 = node::ObjectWrap::Unwrap<NODE_TransformFeedback>(args[1]->ToObject())->gl_handle;
    glDrawTransformFeedback(arg0, arg1);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_drawTransformFeedbackStream(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_beginQueryIndexed(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_endQueryIndexed(const v8::Arguments& args) {
    NanScope();
    // None
    GLenum arg0;
    arg0 = args[0]->Uint32Value();
    GLuint arg1;
    arg1 = args[1]->Uint32Value();
    glEndQueryIndexed(arg0, arg1);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_getQueryIndexediv(const v8::Arguments& args) {
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
        arg3 = (GLint*)node::Buffer::Data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (GLint*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glGetQueryIndexediv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_releaseShaderCompiler(const v8::Arguments& args) {
    NanScope();
    // None
    glReleaseShaderCompiler();
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_shaderBinary(const v8::Arguments& args) {
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
        arg3 = (const GLvoid*)node::Buffer::Data(args[3]);
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
v8::Handle<v8::Value> EXPORT_getShaderPrecisionFormat(const v8::Arguments& args) {
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
        arg2 = (GLint*)node::Buffer::Data(args[2]);
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
        arg3 = (GLint*)node::Buffer::Data(args[3]);
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
v8::Handle<v8::Value> EXPORT_depthRangef(const v8::Arguments& args) {
    NanScope();
    // None
    GLclampf arg0;
    arg0 = (float)args[0]->NumberValue();
    GLclampf arg1;
    arg1 = (float)args[1]->NumberValue();
    glDepthRangef(arg0, arg1);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_clearDepthf(const v8::Arguments& args) {
    NanScope();
    // None
    GLclampf arg0;
    arg0 = (float)args[0]->NumberValue();
    glClearDepthf(arg0);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_getProgramBinary(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
        arg2 = (GLsizei*)node::Buffer::Data(args[2]);
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
        arg3 = (GLenum*)node::Buffer::Data(args[3]);
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
        arg4 = (GLvoid*)node::Buffer::Data(args[4]);
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
v8::Handle<v8::Value> EXPORT_programBinary(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
        arg2 = (const GLvoid*)node::Buffer::Data(args[2]);
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
v8::Handle<v8::Value> EXPORT_programParameteri(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    GLenum arg1;
    arg1 = args[1]->Uint32Value();
    GLint arg2;
    arg2 = args[2]->Int32Value();
    glProgramParameteri(arg0, arg1, arg2);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_useProgramStages(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_ProgramPipeline>(args[0]->ToObject())->gl_handle;
    GLbitfield arg1;
    arg1 = args[1]->Uint32Value();
    GLuint arg2;
    arg2 = node::ObjectWrap::Unwrap<NODE_Program>(args[2]->ToObject())->gl_handle;
    glUseProgramStages(arg0, arg1, arg2);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_activeShaderProgram(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_ProgramPipeline>(args[0]->ToObject())->gl_handle;
    GLuint arg1;
    arg1 = node::ObjectWrap::Unwrap<NODE_Program>(args[1]->ToObject())->gl_handle;
    glActiveShaderProgram(arg0, arg1);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_bindProgramPipeline(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_ProgramPipeline>(args[0]->ToObject())->gl_handle;
    glBindProgramPipeline(arg0);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_genProgramPipelines(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_isProgramPipeline(const v8::Arguments& args) {
    NanScope();
    // <gltypes.Type instance at 0x1036a15f0>
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_ProgramPipeline>(args[0]->ToObject())->gl_handle;
    GLboolean result;
    result = glIsProgramPipeline(arg0);
    v8::Handle<v8::Value> result_js = v8::Uint32::New(result);
    NanReturnValue(result_js);
}
v8::Handle<v8::Value> EXPORT_getProgramPipelineiv(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_ProgramPipeline>(args[0]->ToObject())->gl_handle;
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
        arg2 = (GLint*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetProgramPipelineiv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_programUniform1i(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLint arg2;
    arg2 = args[2]->Int32Value();
    glProgramUniform1i(arg0, arg1, arg2);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_programUniform1iv(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
        arg3 = (const GLint*)node::Buffer::Data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLint*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glProgramUniform1iv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_programUniform1f(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLfloat arg2;
    arg2 = (float)args[2]->NumberValue();
    glProgramUniform1f(arg0, arg1, arg2);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_programUniform1fv(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
        arg3 = (const GLfloat*)node::Buffer::Data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLfloat*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glProgramUniform1fv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_programUniform1d(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLdouble arg2;
    arg2 = args[2]->NumberValue();
    glProgramUniform1d(arg0, arg1, arg2);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_programUniform1dv(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
        arg3 = (const GLdouble*)node::Buffer::Data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLdouble*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glProgramUniform1dv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_programUniform1ui(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLuint arg2;
    arg2 = args[2]->Uint32Value();
    glProgramUniform1ui(arg0, arg1, arg2);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_programUniform1uiv(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
        arg3 = (const GLuint*)node::Buffer::Data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLuint*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glProgramUniform1uiv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_programUniform2i(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLint arg2;
    arg2 = args[2]->Int32Value();
    GLint arg3;
    arg3 = args[3]->Int32Value();
    glProgramUniform2i(arg0, arg1, arg2, arg3);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_programUniform2iv(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
        arg3 = (const GLint*)node::Buffer::Data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLint*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glProgramUniform2iv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_programUniform2f(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLfloat arg2;
    arg2 = (float)args[2]->NumberValue();
    GLfloat arg3;
    arg3 = (float)args[3]->NumberValue();
    glProgramUniform2f(arg0, arg1, arg2, arg3);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_programUniform2fv(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
        arg3 = (const GLfloat*)node::Buffer::Data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLfloat*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glProgramUniform2fv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_programUniform2d(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLdouble arg2;
    arg2 = args[2]->NumberValue();
    GLdouble arg3;
    arg3 = args[3]->NumberValue();
    glProgramUniform2d(arg0, arg1, arg2, arg3);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_programUniform2dv(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
        arg3 = (const GLdouble*)node::Buffer::Data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLdouble*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glProgramUniform2dv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_programUniform2ui(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
    GLint arg1;
    arg1 = args[1]->Int32Value();
    GLuint arg2;
    arg2 = args[2]->Uint32Value();
    GLuint arg3;
    arg3 = args[3]->Uint32Value();
    glProgramUniform2ui(arg0, arg1, arg2, arg3);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_programUniform2uiv(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
        arg3 = (const GLuint*)node::Buffer::Data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLuint*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glProgramUniform2uiv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_programUniform3i(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
v8::Handle<v8::Value> EXPORT_programUniform3iv(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
        arg3 = (const GLint*)node::Buffer::Data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLint*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glProgramUniform3iv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_programUniform3f(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
v8::Handle<v8::Value> EXPORT_programUniform3fv(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
        arg3 = (const GLfloat*)node::Buffer::Data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLfloat*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glProgramUniform3fv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_programUniform3d(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
v8::Handle<v8::Value> EXPORT_programUniform3dv(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
        arg3 = (const GLdouble*)node::Buffer::Data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLdouble*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glProgramUniform3dv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_programUniform3ui(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
v8::Handle<v8::Value> EXPORT_programUniform3uiv(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
        arg3 = (const GLuint*)node::Buffer::Data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLuint*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glProgramUniform3uiv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_programUniform4i(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
v8::Handle<v8::Value> EXPORT_programUniform4iv(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
        arg3 = (const GLint*)node::Buffer::Data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLint*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glProgramUniform4iv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_programUniform4f(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
v8::Handle<v8::Value> EXPORT_programUniform4fv(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
        arg3 = (const GLfloat*)node::Buffer::Data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLfloat*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glProgramUniform4fv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_programUniform4d(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
v8::Handle<v8::Value> EXPORT_programUniform4dv(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
        arg3 = (const GLdouble*)node::Buffer::Data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLdouble*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glProgramUniform4dv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_programUniform4ui(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
v8::Handle<v8::Value> EXPORT_programUniform4uiv(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
        arg3 = (const GLuint*)node::Buffer::Data(args[3]);
    } else if(args[3]->IsNumber()) {
        arg3 = (const GLuint*)(args[3]->IntegerValue());
    } else {
        arg3 = NULL;
    }
    glProgramUniform4uiv(arg0, arg1, arg2, arg3);
    if(arg3_nonconst) delete [] arg3_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_programUniformMatrix2fv(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
        arg4 = (const GLfloat*)node::Buffer::Data(args[4]);
    } else if(args[4]->IsNumber()) {
        arg4 = (const GLfloat*)(args[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glProgramUniformMatrix2fv(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_programUniformMatrix3fv(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
        arg4 = (const GLfloat*)node::Buffer::Data(args[4]);
    } else if(args[4]->IsNumber()) {
        arg4 = (const GLfloat*)(args[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glProgramUniformMatrix3fv(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_programUniformMatrix4fv(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
        arg4 = (const GLfloat*)node::Buffer::Data(args[4]);
    } else if(args[4]->IsNumber()) {
        arg4 = (const GLfloat*)(args[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glProgramUniformMatrix4fv(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_programUniformMatrix2dv(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
        arg4 = (const GLdouble*)node::Buffer::Data(args[4]);
    } else if(args[4]->IsNumber()) {
        arg4 = (const GLdouble*)(args[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glProgramUniformMatrix2dv(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_programUniformMatrix3dv(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
        arg4 = (const GLdouble*)node::Buffer::Data(args[4]);
    } else if(args[4]->IsNumber()) {
        arg4 = (const GLdouble*)(args[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glProgramUniformMatrix3dv(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_programUniformMatrix4dv(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
        arg4 = (const GLdouble*)node::Buffer::Data(args[4]);
    } else if(args[4]->IsNumber()) {
        arg4 = (const GLdouble*)(args[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glProgramUniformMatrix4dv(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_programUniformMatrix2x3fv(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
        arg4 = (const GLfloat*)node::Buffer::Data(args[4]);
    } else if(args[4]->IsNumber()) {
        arg4 = (const GLfloat*)(args[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glProgramUniformMatrix2x3fv(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_programUniformMatrix3x2fv(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
        arg4 = (const GLfloat*)node::Buffer::Data(args[4]);
    } else if(args[4]->IsNumber()) {
        arg4 = (const GLfloat*)(args[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glProgramUniformMatrix3x2fv(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_programUniformMatrix2x4fv(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
        arg4 = (const GLfloat*)node::Buffer::Data(args[4]);
    } else if(args[4]->IsNumber()) {
        arg4 = (const GLfloat*)(args[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glProgramUniformMatrix2x4fv(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_programUniformMatrix4x2fv(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
        arg4 = (const GLfloat*)node::Buffer::Data(args[4]);
    } else if(args[4]->IsNumber()) {
        arg4 = (const GLfloat*)(args[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glProgramUniformMatrix4x2fv(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_programUniformMatrix3x4fv(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
        arg4 = (const GLfloat*)node::Buffer::Data(args[4]);
    } else if(args[4]->IsNumber()) {
        arg4 = (const GLfloat*)(args[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glProgramUniformMatrix3x4fv(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_programUniformMatrix4x3fv(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
        arg4 = (const GLfloat*)node::Buffer::Data(args[4]);
    } else if(args[4]->IsNumber()) {
        arg4 = (const GLfloat*)(args[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glProgramUniformMatrix4x3fv(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_programUniformMatrix2x3dv(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
        arg4 = (const GLdouble*)node::Buffer::Data(args[4]);
    } else if(args[4]->IsNumber()) {
        arg4 = (const GLdouble*)(args[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glProgramUniformMatrix2x3dv(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_programUniformMatrix3x2dv(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
        arg4 = (const GLdouble*)node::Buffer::Data(args[4]);
    } else if(args[4]->IsNumber()) {
        arg4 = (const GLdouble*)(args[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glProgramUniformMatrix3x2dv(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_programUniformMatrix2x4dv(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
        arg4 = (const GLdouble*)node::Buffer::Data(args[4]);
    } else if(args[4]->IsNumber()) {
        arg4 = (const GLdouble*)(args[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glProgramUniformMatrix2x4dv(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_programUniformMatrix4x2dv(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
        arg4 = (const GLdouble*)node::Buffer::Data(args[4]);
    } else if(args[4]->IsNumber()) {
        arg4 = (const GLdouble*)(args[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glProgramUniformMatrix4x2dv(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_programUniformMatrix3x4dv(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
        arg4 = (const GLdouble*)node::Buffer::Data(args[4]);
    } else if(args[4]->IsNumber()) {
        arg4 = (const GLdouble*)(args[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glProgramUniformMatrix3x4dv(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_programUniformMatrix4x3dv(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_Program>(args[0]->ToObject())->gl_handle;
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
        arg4 = (const GLdouble*)node::Buffer::Data(args[4]);
    } else if(args[4]->IsNumber()) {
        arg4 = (const GLdouble*)(args[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glProgramUniformMatrix4x3dv(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_validateProgramPipeline(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_ProgramPipeline>(args[0]->ToObject())->gl_handle;
    glValidateProgramPipeline(arg0);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_getProgramPipelineInfoLog(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = node::ObjectWrap::Unwrap<NODE_ProgramPipeline>(args[0]->ToObject())->gl_handle;
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
        arg2 = (GLsizei*)node::Buffer::Data(args[2]);
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
        arg3 = (GLchar*)node::Buffer::Data(args[3]);
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
v8::Handle<v8::Value> EXPORT_vertexAttribL1d(const v8::Arguments& args) {
    NanScope();
    // None
    GLuint arg0;
    arg0 = args[0]->Uint32Value();
    GLdouble arg1;
    arg1 = args[1]->NumberValue();
    glVertexAttribL1d(arg0, arg1);
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_vertexAttribL2d(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_vertexAttribL3d(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_vertexAttribL4d(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_vertexAttribL1dv(const v8::Arguments& args) {
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
        arg1 = (const GLdouble*)node::Buffer::Data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLdouble*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttribL1dv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_vertexAttribL2dv(const v8::Arguments& args) {
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
        arg1 = (const GLdouble*)node::Buffer::Data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLdouble*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttribL2dv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_vertexAttribL3dv(const v8::Arguments& args) {
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
        arg1 = (const GLdouble*)node::Buffer::Data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLdouble*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttribL3dv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_vertexAttribL4dv(const v8::Arguments& args) {
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
        arg1 = (const GLdouble*)node::Buffer::Data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLdouble*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glVertexAttribL4dv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_vertexAttribLPointer(const v8::Arguments& args) {
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
        arg4 = (const GLvoid*)node::Buffer::Data(args[4]);
    } else if(args[4]->IsNumber()) {
        arg4 = (const GLvoid*)(args[4]->IntegerValue());
    } else {
        arg4 = NULL;
    }
    glVertexAttribLPointer(arg0, arg1, arg2, arg3, arg4);
    if(arg4_nonconst) delete [] arg4_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_getVertexAttribLdv(const v8::Arguments& args) {
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
        arg2 = (GLdouble*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLdouble*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetVertexAttribLdv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_viewportArrayv(const v8::Arguments& args) {
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
        arg2 = (const GLfloat*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLfloat*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glViewportArrayv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_viewportIndexedf(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_viewportIndexedfv(const v8::Arguments& args) {
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
        arg1 = (const GLfloat*)node::Buffer::Data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLfloat*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glViewportIndexedfv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_scissorArrayv(const v8::Arguments& args) {
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
        arg2 = (const GLint*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLint*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glScissorArrayv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_scissorIndexed(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_scissorIndexedv(const v8::Arguments& args) {
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
        arg1 = (const GLint*)node::Buffer::Data(args[1]);
    } else if(args[1]->IsNumber()) {
        arg1 = (const GLint*)(args[1]->IntegerValue());
    } else {
        arg1 = NULL;
    }
    glScissorIndexedv(arg0, arg1);
    if(arg1_nonconst) delete [] arg1_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_depthRangeArrayv(const v8::Arguments& args) {
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
        arg2 = (const GLclampd*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (const GLclampd*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glDepthRangeArrayv(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_depthRangeIndexed(const v8::Arguments& args) {
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
v8::Handle<v8::Value> EXPORT_getFloati_v(const v8::Arguments& args) {
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
        arg2 = (GLfloat*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLfloat*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetFloati_v(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_getDoublei_v(const v8::Arguments& args) {
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
        arg2 = (GLdouble*)node::Buffer::Data(args[2]);
    } else if(args[2]->IsNumber()) {
        arg2 = (GLdouble*)(args[2]->IntegerValue());
    } else {
        arg2 = NULL;
    }
    glGetDoublei_v(arg0, arg1, arg2);
    if(arg2_nonconst) delete [] arg2_nonconst;
    NanReturnUndefined();
}
v8::Handle<v8::Value> EXPORT_shaderSource(const v8::Arguments& args) {

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
    exports->Set(v8::String::NewSymbol("cullFace"), v8::FunctionTemplate::New(EXPORT_cullFace));
    exports->Set(v8::String::NewSymbol("frontFace"), v8::FunctionTemplate::New(EXPORT_frontFace));
    exports->Set(v8::String::NewSymbol("hint"), v8::FunctionTemplate::New(EXPORT_hint));
    exports->Set(v8::String::NewSymbol("lineWidth"), v8::FunctionTemplate::New(EXPORT_lineWidth));
    exports->Set(v8::String::NewSymbol("pointSize"), v8::FunctionTemplate::New(EXPORT_pointSize));
    exports->Set(v8::String::NewSymbol("polygonMode"), v8::FunctionTemplate::New(EXPORT_polygonMode));
    exports->Set(v8::String::NewSymbol("scissor"), v8::FunctionTemplate::New(EXPORT_scissor));
    exports->Set(v8::String::NewSymbol("texParameterf"), v8::FunctionTemplate::New(EXPORT_texParameterf));
    exports->Set(v8::String::NewSymbol("texParameterfv"), v8::FunctionTemplate::New(EXPORT_texParameterfv));
    exports->Set(v8::String::NewSymbol("texParameteri"), v8::FunctionTemplate::New(EXPORT_texParameteri));
    exports->Set(v8::String::NewSymbol("texParameteriv"), v8::FunctionTemplate::New(EXPORT_texParameteriv));
    exports->Set(v8::String::NewSymbol("texImage1D"), v8::FunctionTemplate::New(EXPORT_texImage1D));
    exports->Set(v8::String::NewSymbol("texImage2D"), v8::FunctionTemplate::New(EXPORT_texImage2D));
    exports->Set(v8::String::NewSymbol("drawBuffer"), v8::FunctionTemplate::New(EXPORT_drawBuffer));
    exports->Set(v8::String::NewSymbol("clear"), v8::FunctionTemplate::New(EXPORT_clear));
    exports->Set(v8::String::NewSymbol("clearColor"), v8::FunctionTemplate::New(EXPORT_clearColor));
    exports->Set(v8::String::NewSymbol("clearStencil"), v8::FunctionTemplate::New(EXPORT_clearStencil));
    exports->Set(v8::String::NewSymbol("clearDepth"), v8::FunctionTemplate::New(EXPORT_clearDepth));
    exports->Set(v8::String::NewSymbol("stencilMask"), v8::FunctionTemplate::New(EXPORT_stencilMask));
    exports->Set(v8::String::NewSymbol("colorMask"), v8::FunctionTemplate::New(EXPORT_colorMask));
    exports->Set(v8::String::NewSymbol("depthMask"), v8::FunctionTemplate::New(EXPORT_depthMask));
    exports->Set(v8::String::NewSymbol("disable"), v8::FunctionTemplate::New(EXPORT_disable));
    exports->Set(v8::String::NewSymbol("enable"), v8::FunctionTemplate::New(EXPORT_enable));
    exports->Set(v8::String::NewSymbol("finish"), v8::FunctionTemplate::New(EXPORT_finish));
    exports->Set(v8::String::NewSymbol("flush"), v8::FunctionTemplate::New(EXPORT_flush));
    exports->Set(v8::String::NewSymbol("blendFunc"), v8::FunctionTemplate::New(EXPORT_blendFunc));
    exports->Set(v8::String::NewSymbol("logicOp"), v8::FunctionTemplate::New(EXPORT_logicOp));
    exports->Set(v8::String::NewSymbol("stencilFunc"), v8::FunctionTemplate::New(EXPORT_stencilFunc));
    exports->Set(v8::String::NewSymbol("stencilOp"), v8::FunctionTemplate::New(EXPORT_stencilOp));
    exports->Set(v8::String::NewSymbol("depthFunc"), v8::FunctionTemplate::New(EXPORT_depthFunc));
    exports->Set(v8::String::NewSymbol("pixelStoref"), v8::FunctionTemplate::New(EXPORT_pixelStoref));
    exports->Set(v8::String::NewSymbol("pixelStorei"), v8::FunctionTemplate::New(EXPORT_pixelStorei));
    exports->Set(v8::String::NewSymbol("readBuffer"), v8::FunctionTemplate::New(EXPORT_readBuffer));
    exports->Set(v8::String::NewSymbol("readPixels"), v8::FunctionTemplate::New(EXPORT_readPixels));
    exports->Set(v8::String::NewSymbol("getBooleanv"), v8::FunctionTemplate::New(EXPORT_getBooleanv));
    exports->Set(v8::String::NewSymbol("getDoublev"), v8::FunctionTemplate::New(EXPORT_getDoublev));
    exports->Set(v8::String::NewSymbol("getError"), v8::FunctionTemplate::New(EXPORT_getError));
    exports->Set(v8::String::NewSymbol("getFloatv"), v8::FunctionTemplate::New(EXPORT_getFloatv));
    exports->Set(v8::String::NewSymbol("getIntegerv"), v8::FunctionTemplate::New(EXPORT_getIntegerv));
    exports->Set(v8::String::NewSymbol("getString"), v8::FunctionTemplate::New(EXPORT_getString));
    exports->Set(v8::String::NewSymbol("getTexImage"), v8::FunctionTemplate::New(EXPORT_getTexImage));
    exports->Set(v8::String::NewSymbol("getTexParameterfv"), v8::FunctionTemplate::New(EXPORT_getTexParameterfv));
    exports->Set(v8::String::NewSymbol("getTexParameteriv"), v8::FunctionTemplate::New(EXPORT_getTexParameteriv));
    exports->Set(v8::String::NewSymbol("getTexLevelParameterfv"), v8::FunctionTemplate::New(EXPORT_getTexLevelParameterfv));
    exports->Set(v8::String::NewSymbol("getTexLevelParameteriv"), v8::FunctionTemplate::New(EXPORT_getTexLevelParameteriv));
    exports->Set(v8::String::NewSymbol("isEnabled"), v8::FunctionTemplate::New(EXPORT_isEnabled));
    exports->Set(v8::String::NewSymbol("depthRange"), v8::FunctionTemplate::New(EXPORT_depthRange));
    exports->Set(v8::String::NewSymbol("viewport"), v8::FunctionTemplate::New(EXPORT_viewport));
    exports->Set(v8::String::NewSymbol("drawArrays"), v8::FunctionTemplate::New(EXPORT_drawArrays));
    exports->Set(v8::String::NewSymbol("drawElements"), v8::FunctionTemplate::New(EXPORT_drawElements));
    exports->Set(v8::String::NewSymbol("polygonOffset"), v8::FunctionTemplate::New(EXPORT_polygonOffset));
    exports->Set(v8::String::NewSymbol("copyTexImage1D"), v8::FunctionTemplate::New(EXPORT_copyTexImage1D));
    exports->Set(v8::String::NewSymbol("copyTexImage2D"), v8::FunctionTemplate::New(EXPORT_copyTexImage2D));
    exports->Set(v8::String::NewSymbol("copyTexSubImage1D"), v8::FunctionTemplate::New(EXPORT_copyTexSubImage1D));
    exports->Set(v8::String::NewSymbol("copyTexSubImage2D"), v8::FunctionTemplate::New(EXPORT_copyTexSubImage2D));
    exports->Set(v8::String::NewSymbol("texSubImage1D"), v8::FunctionTemplate::New(EXPORT_texSubImage1D));
    exports->Set(v8::String::NewSymbol("texSubImage2D"), v8::FunctionTemplate::New(EXPORT_texSubImage2D));
    exports->Set(v8::String::NewSymbol("bindTexture"), v8::FunctionTemplate::New(EXPORT_bindTexture));
    exports->Set(v8::String::NewSymbol("genTextures"), v8::FunctionTemplate::New(EXPORT_genTextures));
    exports->Set(v8::String::NewSymbol("isTexture"), v8::FunctionTemplate::New(EXPORT_isTexture));
    exports->Set(v8::String::NewSymbol("blendColor"), v8::FunctionTemplate::New(EXPORT_blendColor));
    exports->Set(v8::String::NewSymbol("blendEquation"), v8::FunctionTemplate::New(EXPORT_blendEquation));
    exports->Set(v8::String::NewSymbol("drawRangeElements"), v8::FunctionTemplate::New(EXPORT_drawRangeElements));
    exports->Set(v8::String::NewSymbol("texImage3D"), v8::FunctionTemplate::New(EXPORT_texImage3D));
    exports->Set(v8::String::NewSymbol("texSubImage3D"), v8::FunctionTemplate::New(EXPORT_texSubImage3D));
    exports->Set(v8::String::NewSymbol("copyTexSubImage3D"), v8::FunctionTemplate::New(EXPORT_copyTexSubImage3D));
    exports->Set(v8::String::NewSymbol("activeTexture"), v8::FunctionTemplate::New(EXPORT_activeTexture));
    exports->Set(v8::String::NewSymbol("sampleCoverage"), v8::FunctionTemplate::New(EXPORT_sampleCoverage));
    exports->Set(v8::String::NewSymbol("compressedTexImage3D"), v8::FunctionTemplate::New(EXPORT_compressedTexImage3D));
    exports->Set(v8::String::NewSymbol("compressedTexImage2D"), v8::FunctionTemplate::New(EXPORT_compressedTexImage2D));
    exports->Set(v8::String::NewSymbol("compressedTexImage1D"), v8::FunctionTemplate::New(EXPORT_compressedTexImage1D));
    exports->Set(v8::String::NewSymbol("compressedTexSubImage3D"), v8::FunctionTemplate::New(EXPORT_compressedTexSubImage3D));
    exports->Set(v8::String::NewSymbol("compressedTexSubImage2D"), v8::FunctionTemplate::New(EXPORT_compressedTexSubImage2D));
    exports->Set(v8::String::NewSymbol("compressedTexSubImage1D"), v8::FunctionTemplate::New(EXPORT_compressedTexSubImage1D));
    exports->Set(v8::String::NewSymbol("getCompressedTexImage"), v8::FunctionTemplate::New(EXPORT_getCompressedTexImage));
    exports->Set(v8::String::NewSymbol("blendFuncSeparate"), v8::FunctionTemplate::New(EXPORT_blendFuncSeparate));
    exports->Set(v8::String::NewSymbol("multiDrawArrays"), v8::FunctionTemplate::New(EXPORT_multiDrawArrays));
    exports->Set(v8::String::NewSymbol("pointParameterf"), v8::FunctionTemplate::New(EXPORT_pointParameterf));
    exports->Set(v8::String::NewSymbol("pointParameterfv"), v8::FunctionTemplate::New(EXPORT_pointParameterfv));
    exports->Set(v8::String::NewSymbol("pointParameteri"), v8::FunctionTemplate::New(EXPORT_pointParameteri));
    exports->Set(v8::String::NewSymbol("pointParameteriv"), v8::FunctionTemplate::New(EXPORT_pointParameteriv));
    exports->Set(v8::String::NewSymbol("genQueries"), v8::FunctionTemplate::New(EXPORT_genQueries));
    exports->Set(v8::String::NewSymbol("isQuery"), v8::FunctionTemplate::New(EXPORT_isQuery));
    exports->Set(v8::String::NewSymbol("beginQuery"), v8::FunctionTemplate::New(EXPORT_beginQuery));
    exports->Set(v8::String::NewSymbol("endQuery"), v8::FunctionTemplate::New(EXPORT_endQuery));
    exports->Set(v8::String::NewSymbol("getQueryiv"), v8::FunctionTemplate::New(EXPORT_getQueryiv));
    exports->Set(v8::String::NewSymbol("getQueryObjectiv"), v8::FunctionTemplate::New(EXPORT_getQueryObjectiv));
    exports->Set(v8::String::NewSymbol("getQueryObjectuiv"), v8::FunctionTemplate::New(EXPORT_getQueryObjectuiv));
    exports->Set(v8::String::NewSymbol("bindBuffer"), v8::FunctionTemplate::New(EXPORT_bindBuffer));
    exports->Set(v8::String::NewSymbol("genBuffers"), v8::FunctionTemplate::New(EXPORT_genBuffers));
    exports->Set(v8::String::NewSymbol("isBuffer"), v8::FunctionTemplate::New(EXPORT_isBuffer));
    exports->Set(v8::String::NewSymbol("bufferData"), v8::FunctionTemplate::New(EXPORT_bufferData));
    exports->Set(v8::String::NewSymbol("bufferSubData"), v8::FunctionTemplate::New(EXPORT_bufferSubData));
    exports->Set(v8::String::NewSymbol("getBufferSubData"), v8::FunctionTemplate::New(EXPORT_getBufferSubData));
    exports->Set(v8::String::NewSymbol("mapBuffer"), v8::FunctionTemplate::New(EXPORT_mapBuffer));
    exports->Set(v8::String::NewSymbol("unmapBuffer"), v8::FunctionTemplate::New(EXPORT_unmapBuffer));
    exports->Set(v8::String::NewSymbol("getBufferParameteriv"), v8::FunctionTemplate::New(EXPORT_getBufferParameteriv));
    exports->Set(v8::String::NewSymbol("blendEquationSeparate"), v8::FunctionTemplate::New(EXPORT_blendEquationSeparate));
    exports->Set(v8::String::NewSymbol("drawBuffers"), v8::FunctionTemplate::New(EXPORT_drawBuffers));
    exports->Set(v8::String::NewSymbol("stencilOpSeparate"), v8::FunctionTemplate::New(EXPORT_stencilOpSeparate));
    exports->Set(v8::String::NewSymbol("stencilFuncSeparate"), v8::FunctionTemplate::New(EXPORT_stencilFuncSeparate));
    exports->Set(v8::String::NewSymbol("stencilMaskSeparate"), v8::FunctionTemplate::New(EXPORT_stencilMaskSeparate));
    exports->Set(v8::String::NewSymbol("attachShader"), v8::FunctionTemplate::New(EXPORT_attachShader));
    exports->Set(v8::String::NewSymbol("bindAttribLocation"), v8::FunctionTemplate::New(EXPORT_bindAttribLocation));
    exports->Set(v8::String::NewSymbol("compileShader"), v8::FunctionTemplate::New(EXPORT_compileShader));
    exports->Set(v8::String::NewSymbol("createProgram"), v8::FunctionTemplate::New(EXPORT_createProgram));
    exports->Set(v8::String::NewSymbol("createShader"), v8::FunctionTemplate::New(EXPORT_createShader));
    exports->Set(v8::String::NewSymbol("detachShader"), v8::FunctionTemplate::New(EXPORT_detachShader));
    exports->Set(v8::String::NewSymbol("disableVertexAttribArray"), v8::FunctionTemplate::New(EXPORT_disableVertexAttribArray));
    exports->Set(v8::String::NewSymbol("enableVertexAttribArray"), v8::FunctionTemplate::New(EXPORT_enableVertexAttribArray));
    exports->Set(v8::String::NewSymbol("getActiveAttrib"), v8::FunctionTemplate::New(EXPORT_getActiveAttrib));
    exports->Set(v8::String::NewSymbol("getActiveUniform"), v8::FunctionTemplate::New(EXPORT_getActiveUniform));
    exports->Set(v8::String::NewSymbol("getAttachedShaders"), v8::FunctionTemplate::New(EXPORT_getAttachedShaders));
    exports->Set(v8::String::NewSymbol("getAttribLocation"), v8::FunctionTemplate::New(EXPORT_getAttribLocation));
    exports->Set(v8::String::NewSymbol("getProgramiv"), v8::FunctionTemplate::New(EXPORT_getProgramiv));
    exports->Set(v8::String::NewSymbol("getProgramInfoLog"), v8::FunctionTemplate::New(EXPORT_getProgramInfoLog));
    exports->Set(v8::String::NewSymbol("getShaderiv"), v8::FunctionTemplate::New(EXPORT_getShaderiv));
    exports->Set(v8::String::NewSymbol("getShaderInfoLog"), v8::FunctionTemplate::New(EXPORT_getShaderInfoLog));
    exports->Set(v8::String::NewSymbol("getShaderSource"), v8::FunctionTemplate::New(EXPORT_getShaderSource));
    exports->Set(v8::String::NewSymbol("getUniformLocation"), v8::FunctionTemplate::New(EXPORT_getUniformLocation));
    exports->Set(v8::String::NewSymbol("getUniformfv"), v8::FunctionTemplate::New(EXPORT_getUniformfv));
    exports->Set(v8::String::NewSymbol("getUniformiv"), v8::FunctionTemplate::New(EXPORT_getUniformiv));
    exports->Set(v8::String::NewSymbol("getVertexAttribdv"), v8::FunctionTemplate::New(EXPORT_getVertexAttribdv));
    exports->Set(v8::String::NewSymbol("getVertexAttribfv"), v8::FunctionTemplate::New(EXPORT_getVertexAttribfv));
    exports->Set(v8::String::NewSymbol("getVertexAttribiv"), v8::FunctionTemplate::New(EXPORT_getVertexAttribiv));
    exports->Set(v8::String::NewSymbol("isProgram"), v8::FunctionTemplate::New(EXPORT_isProgram));
    exports->Set(v8::String::NewSymbol("isShader"), v8::FunctionTemplate::New(EXPORT_isShader));
    exports->Set(v8::String::NewSymbol("linkProgram"), v8::FunctionTemplate::New(EXPORT_linkProgram));
    exports->Set(v8::String::NewSymbol("useProgram"), v8::FunctionTemplate::New(EXPORT_useProgram));
    exports->Set(v8::String::NewSymbol("uniform1f"), v8::FunctionTemplate::New(EXPORT_uniform1f));
    exports->Set(v8::String::NewSymbol("uniform2f"), v8::FunctionTemplate::New(EXPORT_uniform2f));
    exports->Set(v8::String::NewSymbol("uniform3f"), v8::FunctionTemplate::New(EXPORT_uniform3f));
    exports->Set(v8::String::NewSymbol("uniform4f"), v8::FunctionTemplate::New(EXPORT_uniform4f));
    exports->Set(v8::String::NewSymbol("uniform1i"), v8::FunctionTemplate::New(EXPORT_uniform1i));
    exports->Set(v8::String::NewSymbol("uniform2i"), v8::FunctionTemplate::New(EXPORT_uniform2i));
    exports->Set(v8::String::NewSymbol("uniform3i"), v8::FunctionTemplate::New(EXPORT_uniform3i));
    exports->Set(v8::String::NewSymbol("uniform4i"), v8::FunctionTemplate::New(EXPORT_uniform4i));
    exports->Set(v8::String::NewSymbol("uniform1fv"), v8::FunctionTemplate::New(EXPORT_uniform1fv));
    exports->Set(v8::String::NewSymbol("uniform2fv"), v8::FunctionTemplate::New(EXPORT_uniform2fv));
    exports->Set(v8::String::NewSymbol("uniform3fv"), v8::FunctionTemplate::New(EXPORT_uniform3fv));
    exports->Set(v8::String::NewSymbol("uniform4fv"), v8::FunctionTemplate::New(EXPORT_uniform4fv));
    exports->Set(v8::String::NewSymbol("uniform1iv"), v8::FunctionTemplate::New(EXPORT_uniform1iv));
    exports->Set(v8::String::NewSymbol("uniform2iv"), v8::FunctionTemplate::New(EXPORT_uniform2iv));
    exports->Set(v8::String::NewSymbol("uniform3iv"), v8::FunctionTemplate::New(EXPORT_uniform3iv));
    exports->Set(v8::String::NewSymbol("uniform4iv"), v8::FunctionTemplate::New(EXPORT_uniform4iv));
    exports->Set(v8::String::NewSymbol("uniformMatrix2fv"), v8::FunctionTemplate::New(EXPORT_uniformMatrix2fv));
    exports->Set(v8::String::NewSymbol("uniformMatrix3fv"), v8::FunctionTemplate::New(EXPORT_uniformMatrix3fv));
    exports->Set(v8::String::NewSymbol("uniformMatrix4fv"), v8::FunctionTemplate::New(EXPORT_uniformMatrix4fv));
    exports->Set(v8::String::NewSymbol("validateProgram"), v8::FunctionTemplate::New(EXPORT_validateProgram));
    exports->Set(v8::String::NewSymbol("vertexAttrib1d"), v8::FunctionTemplate::New(EXPORT_vertexAttrib1d));
    exports->Set(v8::String::NewSymbol("vertexAttrib1dv"), v8::FunctionTemplate::New(EXPORT_vertexAttrib1dv));
    exports->Set(v8::String::NewSymbol("vertexAttrib1f"), v8::FunctionTemplate::New(EXPORT_vertexAttrib1f));
    exports->Set(v8::String::NewSymbol("vertexAttrib1fv"), v8::FunctionTemplate::New(EXPORT_vertexAttrib1fv));
    exports->Set(v8::String::NewSymbol("vertexAttrib1s"), v8::FunctionTemplate::New(EXPORT_vertexAttrib1s));
    exports->Set(v8::String::NewSymbol("vertexAttrib1sv"), v8::FunctionTemplate::New(EXPORT_vertexAttrib1sv));
    exports->Set(v8::String::NewSymbol("vertexAttrib2d"), v8::FunctionTemplate::New(EXPORT_vertexAttrib2d));
    exports->Set(v8::String::NewSymbol("vertexAttrib2dv"), v8::FunctionTemplate::New(EXPORT_vertexAttrib2dv));
    exports->Set(v8::String::NewSymbol("vertexAttrib2f"), v8::FunctionTemplate::New(EXPORT_vertexAttrib2f));
    exports->Set(v8::String::NewSymbol("vertexAttrib2fv"), v8::FunctionTemplate::New(EXPORT_vertexAttrib2fv));
    exports->Set(v8::String::NewSymbol("vertexAttrib2s"), v8::FunctionTemplate::New(EXPORT_vertexAttrib2s));
    exports->Set(v8::String::NewSymbol("vertexAttrib2sv"), v8::FunctionTemplate::New(EXPORT_vertexAttrib2sv));
    exports->Set(v8::String::NewSymbol("vertexAttrib3d"), v8::FunctionTemplate::New(EXPORT_vertexAttrib3d));
    exports->Set(v8::String::NewSymbol("vertexAttrib3dv"), v8::FunctionTemplate::New(EXPORT_vertexAttrib3dv));
    exports->Set(v8::String::NewSymbol("vertexAttrib3f"), v8::FunctionTemplate::New(EXPORT_vertexAttrib3f));
    exports->Set(v8::String::NewSymbol("vertexAttrib3fv"), v8::FunctionTemplate::New(EXPORT_vertexAttrib3fv));
    exports->Set(v8::String::NewSymbol("vertexAttrib3s"), v8::FunctionTemplate::New(EXPORT_vertexAttrib3s));
    exports->Set(v8::String::NewSymbol("vertexAttrib3sv"), v8::FunctionTemplate::New(EXPORT_vertexAttrib3sv));
    exports->Set(v8::String::NewSymbol("vertexAttrib4Nbv"), v8::FunctionTemplate::New(EXPORT_vertexAttrib4Nbv));
    exports->Set(v8::String::NewSymbol("vertexAttrib4Niv"), v8::FunctionTemplate::New(EXPORT_vertexAttrib4Niv));
    exports->Set(v8::String::NewSymbol("vertexAttrib4Nsv"), v8::FunctionTemplate::New(EXPORT_vertexAttrib4Nsv));
    exports->Set(v8::String::NewSymbol("vertexAttrib4Nub"), v8::FunctionTemplate::New(EXPORT_vertexAttrib4Nub));
    exports->Set(v8::String::NewSymbol("vertexAttrib4Nubv"), v8::FunctionTemplate::New(EXPORT_vertexAttrib4Nubv));
    exports->Set(v8::String::NewSymbol("vertexAttrib4Nuiv"), v8::FunctionTemplate::New(EXPORT_vertexAttrib4Nuiv));
    exports->Set(v8::String::NewSymbol("vertexAttrib4Nusv"), v8::FunctionTemplate::New(EXPORT_vertexAttrib4Nusv));
    exports->Set(v8::String::NewSymbol("vertexAttrib4bv"), v8::FunctionTemplate::New(EXPORT_vertexAttrib4bv));
    exports->Set(v8::String::NewSymbol("vertexAttrib4d"), v8::FunctionTemplate::New(EXPORT_vertexAttrib4d));
    exports->Set(v8::String::NewSymbol("vertexAttrib4dv"), v8::FunctionTemplate::New(EXPORT_vertexAttrib4dv));
    exports->Set(v8::String::NewSymbol("vertexAttrib4f"), v8::FunctionTemplate::New(EXPORT_vertexAttrib4f));
    exports->Set(v8::String::NewSymbol("vertexAttrib4fv"), v8::FunctionTemplate::New(EXPORT_vertexAttrib4fv));
    exports->Set(v8::String::NewSymbol("vertexAttrib4iv"), v8::FunctionTemplate::New(EXPORT_vertexAttrib4iv));
    exports->Set(v8::String::NewSymbol("vertexAttrib4s"), v8::FunctionTemplate::New(EXPORT_vertexAttrib4s));
    exports->Set(v8::String::NewSymbol("vertexAttrib4sv"), v8::FunctionTemplate::New(EXPORT_vertexAttrib4sv));
    exports->Set(v8::String::NewSymbol("vertexAttrib4ubv"), v8::FunctionTemplate::New(EXPORT_vertexAttrib4ubv));
    exports->Set(v8::String::NewSymbol("vertexAttrib4uiv"), v8::FunctionTemplate::New(EXPORT_vertexAttrib4uiv));
    exports->Set(v8::String::NewSymbol("vertexAttrib4usv"), v8::FunctionTemplate::New(EXPORT_vertexAttrib4usv));
    exports->Set(v8::String::NewSymbol("vertexAttribPointer"), v8::FunctionTemplate::New(EXPORT_vertexAttribPointer));
    exports->Set(v8::String::NewSymbol("uniformMatrix2x3fv"), v8::FunctionTemplate::New(EXPORT_uniformMatrix2x3fv));
    exports->Set(v8::String::NewSymbol("uniformMatrix3x2fv"), v8::FunctionTemplate::New(EXPORT_uniformMatrix3x2fv));
    exports->Set(v8::String::NewSymbol("uniformMatrix2x4fv"), v8::FunctionTemplate::New(EXPORT_uniformMatrix2x4fv));
    exports->Set(v8::String::NewSymbol("uniformMatrix4x2fv"), v8::FunctionTemplate::New(EXPORT_uniformMatrix4x2fv));
    exports->Set(v8::String::NewSymbol("uniformMatrix3x4fv"), v8::FunctionTemplate::New(EXPORT_uniformMatrix3x4fv));
    exports->Set(v8::String::NewSymbol("uniformMatrix4x3fv"), v8::FunctionTemplate::New(EXPORT_uniformMatrix4x3fv));
    exports->Set(v8::String::NewSymbol("colorMaski"), v8::FunctionTemplate::New(EXPORT_colorMaski));
    exports->Set(v8::String::NewSymbol("getBooleani_v"), v8::FunctionTemplate::New(EXPORT_getBooleani_v));
    exports->Set(v8::String::NewSymbol("getIntegeri_v"), v8::FunctionTemplate::New(EXPORT_getIntegeri_v));
    exports->Set(v8::String::NewSymbol("enablei"), v8::FunctionTemplate::New(EXPORT_enablei));
    exports->Set(v8::String::NewSymbol("disablei"), v8::FunctionTemplate::New(EXPORT_disablei));
    exports->Set(v8::String::NewSymbol("isEnabledi"), v8::FunctionTemplate::New(EXPORT_isEnabledi));
    exports->Set(v8::String::NewSymbol("beginTransformFeedback"), v8::FunctionTemplate::New(EXPORT_beginTransformFeedback));
    exports->Set(v8::String::NewSymbol("endTransformFeedback"), v8::FunctionTemplate::New(EXPORT_endTransformFeedback));
    exports->Set(v8::String::NewSymbol("bindBufferRange"), v8::FunctionTemplate::New(EXPORT_bindBufferRange));
    exports->Set(v8::String::NewSymbol("bindBufferBase"), v8::FunctionTemplate::New(EXPORT_bindBufferBase));
    exports->Set(v8::String::NewSymbol("getTransformFeedbackVarying"), v8::FunctionTemplate::New(EXPORT_getTransformFeedbackVarying));
    exports->Set(v8::String::NewSymbol("clampColor"), v8::FunctionTemplate::New(EXPORT_clampColor));
    exports->Set(v8::String::NewSymbol("beginConditionalRender"), v8::FunctionTemplate::New(EXPORT_beginConditionalRender));
    exports->Set(v8::String::NewSymbol("endConditionalRender"), v8::FunctionTemplate::New(EXPORT_endConditionalRender));
    exports->Set(v8::String::NewSymbol("vertexAttribIPointer"), v8::FunctionTemplate::New(EXPORT_vertexAttribIPointer));
    exports->Set(v8::String::NewSymbol("getVertexAttribIiv"), v8::FunctionTemplate::New(EXPORT_getVertexAttribIiv));
    exports->Set(v8::String::NewSymbol("getVertexAttribIuiv"), v8::FunctionTemplate::New(EXPORT_getVertexAttribIuiv));
    exports->Set(v8::String::NewSymbol("vertexAttribI1i"), v8::FunctionTemplate::New(EXPORT_vertexAttribI1i));
    exports->Set(v8::String::NewSymbol("vertexAttribI2i"), v8::FunctionTemplate::New(EXPORT_vertexAttribI2i));
    exports->Set(v8::String::NewSymbol("vertexAttribI3i"), v8::FunctionTemplate::New(EXPORT_vertexAttribI3i));
    exports->Set(v8::String::NewSymbol("vertexAttribI4i"), v8::FunctionTemplate::New(EXPORT_vertexAttribI4i));
    exports->Set(v8::String::NewSymbol("vertexAttribI1ui"), v8::FunctionTemplate::New(EXPORT_vertexAttribI1ui));
    exports->Set(v8::String::NewSymbol("vertexAttribI2ui"), v8::FunctionTemplate::New(EXPORT_vertexAttribI2ui));
    exports->Set(v8::String::NewSymbol("vertexAttribI3ui"), v8::FunctionTemplate::New(EXPORT_vertexAttribI3ui));
    exports->Set(v8::String::NewSymbol("vertexAttribI4ui"), v8::FunctionTemplate::New(EXPORT_vertexAttribI4ui));
    exports->Set(v8::String::NewSymbol("vertexAttribI1iv"), v8::FunctionTemplate::New(EXPORT_vertexAttribI1iv));
    exports->Set(v8::String::NewSymbol("vertexAttribI2iv"), v8::FunctionTemplate::New(EXPORT_vertexAttribI2iv));
    exports->Set(v8::String::NewSymbol("vertexAttribI3iv"), v8::FunctionTemplate::New(EXPORT_vertexAttribI3iv));
    exports->Set(v8::String::NewSymbol("vertexAttribI4iv"), v8::FunctionTemplate::New(EXPORT_vertexAttribI4iv));
    exports->Set(v8::String::NewSymbol("vertexAttribI1uiv"), v8::FunctionTemplate::New(EXPORT_vertexAttribI1uiv));
    exports->Set(v8::String::NewSymbol("vertexAttribI2uiv"), v8::FunctionTemplate::New(EXPORT_vertexAttribI2uiv));
    exports->Set(v8::String::NewSymbol("vertexAttribI3uiv"), v8::FunctionTemplate::New(EXPORT_vertexAttribI3uiv));
    exports->Set(v8::String::NewSymbol("vertexAttribI4uiv"), v8::FunctionTemplate::New(EXPORT_vertexAttribI4uiv));
    exports->Set(v8::String::NewSymbol("vertexAttribI4bv"), v8::FunctionTemplate::New(EXPORT_vertexAttribI4bv));
    exports->Set(v8::String::NewSymbol("vertexAttribI4sv"), v8::FunctionTemplate::New(EXPORT_vertexAttribI4sv));
    exports->Set(v8::String::NewSymbol("vertexAttribI4ubv"), v8::FunctionTemplate::New(EXPORT_vertexAttribI4ubv));
    exports->Set(v8::String::NewSymbol("vertexAttribI4usv"), v8::FunctionTemplate::New(EXPORT_vertexAttribI4usv));
    exports->Set(v8::String::NewSymbol("getUniformuiv"), v8::FunctionTemplate::New(EXPORT_getUniformuiv));
    exports->Set(v8::String::NewSymbol("bindFragDataLocation"), v8::FunctionTemplate::New(EXPORT_bindFragDataLocation));
    exports->Set(v8::String::NewSymbol("getFragDataLocation"), v8::FunctionTemplate::New(EXPORT_getFragDataLocation));
    exports->Set(v8::String::NewSymbol("uniform1ui"), v8::FunctionTemplate::New(EXPORT_uniform1ui));
    exports->Set(v8::String::NewSymbol("uniform2ui"), v8::FunctionTemplate::New(EXPORT_uniform2ui));
    exports->Set(v8::String::NewSymbol("uniform3ui"), v8::FunctionTemplate::New(EXPORT_uniform3ui));
    exports->Set(v8::String::NewSymbol("uniform4ui"), v8::FunctionTemplate::New(EXPORT_uniform4ui));
    exports->Set(v8::String::NewSymbol("uniform1uiv"), v8::FunctionTemplate::New(EXPORT_uniform1uiv));
    exports->Set(v8::String::NewSymbol("uniform2uiv"), v8::FunctionTemplate::New(EXPORT_uniform2uiv));
    exports->Set(v8::String::NewSymbol("uniform3uiv"), v8::FunctionTemplate::New(EXPORT_uniform3uiv));
    exports->Set(v8::String::NewSymbol("uniform4uiv"), v8::FunctionTemplate::New(EXPORT_uniform4uiv));
    exports->Set(v8::String::NewSymbol("texParameterIiv"), v8::FunctionTemplate::New(EXPORT_texParameterIiv));
    exports->Set(v8::String::NewSymbol("texParameterIuiv"), v8::FunctionTemplate::New(EXPORT_texParameterIuiv));
    exports->Set(v8::String::NewSymbol("getTexParameterIiv"), v8::FunctionTemplate::New(EXPORT_getTexParameterIiv));
    exports->Set(v8::String::NewSymbol("getTexParameterIuiv"), v8::FunctionTemplate::New(EXPORT_getTexParameterIuiv));
    exports->Set(v8::String::NewSymbol("clearBufferiv"), v8::FunctionTemplate::New(EXPORT_clearBufferiv));
    exports->Set(v8::String::NewSymbol("clearBufferuiv"), v8::FunctionTemplate::New(EXPORT_clearBufferuiv));
    exports->Set(v8::String::NewSymbol("clearBufferfv"), v8::FunctionTemplate::New(EXPORT_clearBufferfv));
    exports->Set(v8::String::NewSymbol("clearBufferfi"), v8::FunctionTemplate::New(EXPORT_clearBufferfi));
    exports->Set(v8::String::NewSymbol("getStringi"), v8::FunctionTemplate::New(EXPORT_getStringi));
    exports->Set(v8::String::NewSymbol("drawArraysInstanced"), v8::FunctionTemplate::New(EXPORT_drawArraysInstanced));
    exports->Set(v8::String::NewSymbol("drawElementsInstanced"), v8::FunctionTemplate::New(EXPORT_drawElementsInstanced));
    exports->Set(v8::String::NewSymbol("texBuffer"), v8::FunctionTemplate::New(EXPORT_texBuffer));
    exports->Set(v8::String::NewSymbol("primitiveRestartIndex"), v8::FunctionTemplate::New(EXPORT_primitiveRestartIndex));
    exports->Set(v8::String::NewSymbol("getInteger64i_v"), v8::FunctionTemplate::New(EXPORT_getInteger64i_v));
    exports->Set(v8::String::NewSymbol("getBufferParameteri64v"), v8::FunctionTemplate::New(EXPORT_getBufferParameteri64v));
    exports->Set(v8::String::NewSymbol("framebufferTexture"), v8::FunctionTemplate::New(EXPORT_framebufferTexture));
    exports->Set(v8::String::NewSymbol("vertexAttribDivisor"), v8::FunctionTemplate::New(EXPORT_vertexAttribDivisor));
    exports->Set(v8::String::NewSymbol("minSampleShading"), v8::FunctionTemplate::New(EXPORT_minSampleShading));
    exports->Set(v8::String::NewSymbol("blendEquationi"), v8::FunctionTemplate::New(EXPORT_blendEquationi));
    exports->Set(v8::String::NewSymbol("blendEquationSeparatei"), v8::FunctionTemplate::New(EXPORT_blendEquationSeparatei));
    exports->Set(v8::String::NewSymbol("blendFunci"), v8::FunctionTemplate::New(EXPORT_blendFunci));
    exports->Set(v8::String::NewSymbol("blendFuncSeparatei"), v8::FunctionTemplate::New(EXPORT_blendFuncSeparatei));
    exports->Set(v8::String::NewSymbol("isRenderbuffer"), v8::FunctionTemplate::New(EXPORT_isRenderbuffer));
    exports->Set(v8::String::NewSymbol("bindRenderbuffer"), v8::FunctionTemplate::New(EXPORT_bindRenderbuffer));
    exports->Set(v8::String::NewSymbol("genRenderbuffers"), v8::FunctionTemplate::New(EXPORT_genRenderbuffers));
    exports->Set(v8::String::NewSymbol("renderbufferStorage"), v8::FunctionTemplate::New(EXPORT_renderbufferStorage));
    exports->Set(v8::String::NewSymbol("getRenderbufferParameteriv"), v8::FunctionTemplate::New(EXPORT_getRenderbufferParameteriv));
    exports->Set(v8::String::NewSymbol("isFramebuffer"), v8::FunctionTemplate::New(EXPORT_isFramebuffer));
    exports->Set(v8::String::NewSymbol("bindFramebuffer"), v8::FunctionTemplate::New(EXPORT_bindFramebuffer));
    exports->Set(v8::String::NewSymbol("genFramebuffers"), v8::FunctionTemplate::New(EXPORT_genFramebuffers));
    exports->Set(v8::String::NewSymbol("checkFramebufferStatus"), v8::FunctionTemplate::New(EXPORT_checkFramebufferStatus));
    exports->Set(v8::String::NewSymbol("framebufferTexture1D"), v8::FunctionTemplate::New(EXPORT_framebufferTexture1D));
    exports->Set(v8::String::NewSymbol("framebufferTexture2D"), v8::FunctionTemplate::New(EXPORT_framebufferTexture2D));
    exports->Set(v8::String::NewSymbol("framebufferTexture3D"), v8::FunctionTemplate::New(EXPORT_framebufferTexture3D));
    exports->Set(v8::String::NewSymbol("framebufferRenderbuffer"), v8::FunctionTemplate::New(EXPORT_framebufferRenderbuffer));
    exports->Set(v8::String::NewSymbol("getFramebufferAttachmentParameteriv"), v8::FunctionTemplate::New(EXPORT_getFramebufferAttachmentParameteriv));
    exports->Set(v8::String::NewSymbol("generateMipmap"), v8::FunctionTemplate::New(EXPORT_generateMipmap));
    exports->Set(v8::String::NewSymbol("blitFramebuffer"), v8::FunctionTemplate::New(EXPORT_blitFramebuffer));
    exports->Set(v8::String::NewSymbol("renderbufferStorageMultisample"), v8::FunctionTemplate::New(EXPORT_renderbufferStorageMultisample));
    exports->Set(v8::String::NewSymbol("framebufferTextureLayer"), v8::FunctionTemplate::New(EXPORT_framebufferTextureLayer));
    exports->Set(v8::String::NewSymbol("mapBufferRange"), v8::FunctionTemplate::New(EXPORT_mapBufferRange));
    exports->Set(v8::String::NewSymbol("flushMappedBufferRange"), v8::FunctionTemplate::New(EXPORT_flushMappedBufferRange));
    exports->Set(v8::String::NewSymbol("bindVertexArray"), v8::FunctionTemplate::New(EXPORT_bindVertexArray));
    exports->Set(v8::String::NewSymbol("genVertexArrays"), v8::FunctionTemplate::New(EXPORT_genVertexArrays));
    exports->Set(v8::String::NewSymbol("isVertexArray"), v8::FunctionTemplate::New(EXPORT_isVertexArray));
    exports->Set(v8::String::NewSymbol("getActiveUniformsiv"), v8::FunctionTemplate::New(EXPORT_getActiveUniformsiv));
    exports->Set(v8::String::NewSymbol("getActiveUniformName"), v8::FunctionTemplate::New(EXPORT_getActiveUniformName));
    exports->Set(v8::String::NewSymbol("getUniformBlockIndex"), v8::FunctionTemplate::New(EXPORT_getUniformBlockIndex));
    exports->Set(v8::String::NewSymbol("getActiveUniformBlockiv"), v8::FunctionTemplate::New(EXPORT_getActiveUniformBlockiv));
    exports->Set(v8::String::NewSymbol("getActiveUniformBlockName"), v8::FunctionTemplate::New(EXPORT_getActiveUniformBlockName));
    exports->Set(v8::String::NewSymbol("uniformBlockBinding"), v8::FunctionTemplate::New(EXPORT_uniformBlockBinding));
    exports->Set(v8::String::NewSymbol("copyBufferSubData"), v8::FunctionTemplate::New(EXPORT_copyBufferSubData));
    exports->Set(v8::String::NewSymbol("drawElementsBaseVertex"), v8::FunctionTemplate::New(EXPORT_drawElementsBaseVertex));
    exports->Set(v8::String::NewSymbol("drawRangeElementsBaseVertex"), v8::FunctionTemplate::New(EXPORT_drawRangeElementsBaseVertex));
    exports->Set(v8::String::NewSymbol("drawElementsInstancedBaseVertex"), v8::FunctionTemplate::New(EXPORT_drawElementsInstancedBaseVertex));
    exports->Set(v8::String::NewSymbol("provokingVertex"), v8::FunctionTemplate::New(EXPORT_provokingVertex));
    exports->Set(v8::String::NewSymbol("texImage2DMultisample"), v8::FunctionTemplate::New(EXPORT_texImage2DMultisample));
    exports->Set(v8::String::NewSymbol("texImage3DMultisample"), v8::FunctionTemplate::New(EXPORT_texImage3DMultisample));
    exports->Set(v8::String::NewSymbol("getMultisamplefv"), v8::FunctionTemplate::New(EXPORT_getMultisamplefv));
    exports->Set(v8::String::NewSymbol("sampleMaski"), v8::FunctionTemplate::New(EXPORT_sampleMaski));
    exports->Set(v8::String::NewSymbol("bindFragDataLocationIndexed"), v8::FunctionTemplate::New(EXPORT_bindFragDataLocationIndexed));
    exports->Set(v8::String::NewSymbol("getFragDataIndex"), v8::FunctionTemplate::New(EXPORT_getFragDataIndex));
    exports->Set(v8::String::NewSymbol("genSamplers"), v8::FunctionTemplate::New(EXPORT_genSamplers));
    exports->Set(v8::String::NewSymbol("isSampler"), v8::FunctionTemplate::New(EXPORT_isSampler));
    exports->Set(v8::String::NewSymbol("bindSampler"), v8::FunctionTemplate::New(EXPORT_bindSampler));
    exports->Set(v8::String::NewSymbol("samplerParameteri"), v8::FunctionTemplate::New(EXPORT_samplerParameteri));
    exports->Set(v8::String::NewSymbol("samplerParameteriv"), v8::FunctionTemplate::New(EXPORT_samplerParameteriv));
    exports->Set(v8::String::NewSymbol("samplerParameterf"), v8::FunctionTemplate::New(EXPORT_samplerParameterf));
    exports->Set(v8::String::NewSymbol("samplerParameterfv"), v8::FunctionTemplate::New(EXPORT_samplerParameterfv));
    exports->Set(v8::String::NewSymbol("samplerParameterIiv"), v8::FunctionTemplate::New(EXPORT_samplerParameterIiv));
    exports->Set(v8::String::NewSymbol("samplerParameterIuiv"), v8::FunctionTemplate::New(EXPORT_samplerParameterIuiv));
    exports->Set(v8::String::NewSymbol("getSamplerParameteriv"), v8::FunctionTemplate::New(EXPORT_getSamplerParameteriv));
    exports->Set(v8::String::NewSymbol("getSamplerParameterIiv"), v8::FunctionTemplate::New(EXPORT_getSamplerParameterIiv));
    exports->Set(v8::String::NewSymbol("getSamplerParameterfv"), v8::FunctionTemplate::New(EXPORT_getSamplerParameterfv));
    exports->Set(v8::String::NewSymbol("getSamplerParameterIuiv"), v8::FunctionTemplate::New(EXPORT_getSamplerParameterIuiv));
    exports->Set(v8::String::NewSymbol("queryCounter"), v8::FunctionTemplate::New(EXPORT_queryCounter));
    exports->Set(v8::String::NewSymbol("getQueryObjecti64v"), v8::FunctionTemplate::New(EXPORT_getQueryObjecti64v));
    exports->Set(v8::String::NewSymbol("getQueryObjectui64v"), v8::FunctionTemplate::New(EXPORT_getQueryObjectui64v));
    exports->Set(v8::String::NewSymbol("vertexAttribP1ui"), v8::FunctionTemplate::New(EXPORT_vertexAttribP1ui));
    exports->Set(v8::String::NewSymbol("vertexAttribP1uiv"), v8::FunctionTemplate::New(EXPORT_vertexAttribP1uiv));
    exports->Set(v8::String::NewSymbol("vertexAttribP2ui"), v8::FunctionTemplate::New(EXPORT_vertexAttribP2ui));
    exports->Set(v8::String::NewSymbol("vertexAttribP2uiv"), v8::FunctionTemplate::New(EXPORT_vertexAttribP2uiv));
    exports->Set(v8::String::NewSymbol("vertexAttribP3ui"), v8::FunctionTemplate::New(EXPORT_vertexAttribP3ui));
    exports->Set(v8::String::NewSymbol("vertexAttribP3uiv"), v8::FunctionTemplate::New(EXPORT_vertexAttribP3uiv));
    exports->Set(v8::String::NewSymbol("vertexAttribP4ui"), v8::FunctionTemplate::New(EXPORT_vertexAttribP4ui));
    exports->Set(v8::String::NewSymbol("vertexAttribP4uiv"), v8::FunctionTemplate::New(EXPORT_vertexAttribP4uiv));
    exports->Set(v8::String::NewSymbol("drawArraysIndirect"), v8::FunctionTemplate::New(EXPORT_drawArraysIndirect));
    exports->Set(v8::String::NewSymbol("drawElementsIndirect"), v8::FunctionTemplate::New(EXPORT_drawElementsIndirect));
    exports->Set(v8::String::NewSymbol("uniform1d"), v8::FunctionTemplate::New(EXPORT_uniform1d));
    exports->Set(v8::String::NewSymbol("uniform2d"), v8::FunctionTemplate::New(EXPORT_uniform2d));
    exports->Set(v8::String::NewSymbol("uniform3d"), v8::FunctionTemplate::New(EXPORT_uniform3d));
    exports->Set(v8::String::NewSymbol("uniform4d"), v8::FunctionTemplate::New(EXPORT_uniform4d));
    exports->Set(v8::String::NewSymbol("uniform1dv"), v8::FunctionTemplate::New(EXPORT_uniform1dv));
    exports->Set(v8::String::NewSymbol("uniform2dv"), v8::FunctionTemplate::New(EXPORT_uniform2dv));
    exports->Set(v8::String::NewSymbol("uniform3dv"), v8::FunctionTemplate::New(EXPORT_uniform3dv));
    exports->Set(v8::String::NewSymbol("uniform4dv"), v8::FunctionTemplate::New(EXPORT_uniform4dv));
    exports->Set(v8::String::NewSymbol("uniformMatrix2dv"), v8::FunctionTemplate::New(EXPORT_uniformMatrix2dv));
    exports->Set(v8::String::NewSymbol("uniformMatrix3dv"), v8::FunctionTemplate::New(EXPORT_uniformMatrix3dv));
    exports->Set(v8::String::NewSymbol("uniformMatrix4dv"), v8::FunctionTemplate::New(EXPORT_uniformMatrix4dv));
    exports->Set(v8::String::NewSymbol("uniformMatrix2x3dv"), v8::FunctionTemplate::New(EXPORT_uniformMatrix2x3dv));
    exports->Set(v8::String::NewSymbol("uniformMatrix2x4dv"), v8::FunctionTemplate::New(EXPORT_uniformMatrix2x4dv));
    exports->Set(v8::String::NewSymbol("uniformMatrix3x2dv"), v8::FunctionTemplate::New(EXPORT_uniformMatrix3x2dv));
    exports->Set(v8::String::NewSymbol("uniformMatrix3x4dv"), v8::FunctionTemplate::New(EXPORT_uniformMatrix3x4dv));
    exports->Set(v8::String::NewSymbol("uniformMatrix4x2dv"), v8::FunctionTemplate::New(EXPORT_uniformMatrix4x2dv));
    exports->Set(v8::String::NewSymbol("uniformMatrix4x3dv"), v8::FunctionTemplate::New(EXPORT_uniformMatrix4x3dv));
    exports->Set(v8::String::NewSymbol("getUniformdv"), v8::FunctionTemplate::New(EXPORT_getUniformdv));
    exports->Set(v8::String::NewSymbol("getSubroutineUniformLocation"), v8::FunctionTemplate::New(EXPORT_getSubroutineUniformLocation));
    exports->Set(v8::String::NewSymbol("getSubroutineIndex"), v8::FunctionTemplate::New(EXPORT_getSubroutineIndex));
    exports->Set(v8::String::NewSymbol("getActiveSubroutineUniformiv"), v8::FunctionTemplate::New(EXPORT_getActiveSubroutineUniformiv));
    exports->Set(v8::String::NewSymbol("getActiveSubroutineUniformName"), v8::FunctionTemplate::New(EXPORT_getActiveSubroutineUniformName));
    exports->Set(v8::String::NewSymbol("getActiveSubroutineName"), v8::FunctionTemplate::New(EXPORT_getActiveSubroutineName));
    exports->Set(v8::String::NewSymbol("uniformSubroutinesuiv"), v8::FunctionTemplate::New(EXPORT_uniformSubroutinesuiv));
    exports->Set(v8::String::NewSymbol("getUniformSubroutineuiv"), v8::FunctionTemplate::New(EXPORT_getUniformSubroutineuiv));
    exports->Set(v8::String::NewSymbol("getProgramStageiv"), v8::FunctionTemplate::New(EXPORT_getProgramStageiv));
    exports->Set(v8::String::NewSymbol("patchParameteri"), v8::FunctionTemplate::New(EXPORT_patchParameteri));
    exports->Set(v8::String::NewSymbol("patchParameterfv"), v8::FunctionTemplate::New(EXPORT_patchParameterfv));
    exports->Set(v8::String::NewSymbol("bindTransformFeedback"), v8::FunctionTemplate::New(EXPORT_bindTransformFeedback));
    exports->Set(v8::String::NewSymbol("genTransformFeedbacks"), v8::FunctionTemplate::New(EXPORT_genTransformFeedbacks));
    exports->Set(v8::String::NewSymbol("isTransformFeedback"), v8::FunctionTemplate::New(EXPORT_isTransformFeedback));
    exports->Set(v8::String::NewSymbol("pauseTransformFeedback"), v8::FunctionTemplate::New(EXPORT_pauseTransformFeedback));
    exports->Set(v8::String::NewSymbol("resumeTransformFeedback"), v8::FunctionTemplate::New(EXPORT_resumeTransformFeedback));
    exports->Set(v8::String::NewSymbol("drawTransformFeedback"), v8::FunctionTemplate::New(EXPORT_drawTransformFeedback));
    exports->Set(v8::String::NewSymbol("drawTransformFeedbackStream"), v8::FunctionTemplate::New(EXPORT_drawTransformFeedbackStream));
    exports->Set(v8::String::NewSymbol("beginQueryIndexed"), v8::FunctionTemplate::New(EXPORT_beginQueryIndexed));
    exports->Set(v8::String::NewSymbol("endQueryIndexed"), v8::FunctionTemplate::New(EXPORT_endQueryIndexed));
    exports->Set(v8::String::NewSymbol("getQueryIndexediv"), v8::FunctionTemplate::New(EXPORT_getQueryIndexediv));
    exports->Set(v8::String::NewSymbol("releaseShaderCompiler"), v8::FunctionTemplate::New(EXPORT_releaseShaderCompiler));
    exports->Set(v8::String::NewSymbol("shaderBinary"), v8::FunctionTemplate::New(EXPORT_shaderBinary));
    exports->Set(v8::String::NewSymbol("getShaderPrecisionFormat"), v8::FunctionTemplate::New(EXPORT_getShaderPrecisionFormat));
    exports->Set(v8::String::NewSymbol("depthRangef"), v8::FunctionTemplate::New(EXPORT_depthRangef));
    exports->Set(v8::String::NewSymbol("clearDepthf"), v8::FunctionTemplate::New(EXPORT_clearDepthf));
    exports->Set(v8::String::NewSymbol("getProgramBinary"), v8::FunctionTemplate::New(EXPORT_getProgramBinary));
    exports->Set(v8::String::NewSymbol("programBinary"), v8::FunctionTemplate::New(EXPORT_programBinary));
    exports->Set(v8::String::NewSymbol("programParameteri"), v8::FunctionTemplate::New(EXPORT_programParameteri));
    exports->Set(v8::String::NewSymbol("useProgramStages"), v8::FunctionTemplate::New(EXPORT_useProgramStages));
    exports->Set(v8::String::NewSymbol("activeShaderProgram"), v8::FunctionTemplate::New(EXPORT_activeShaderProgram));
    exports->Set(v8::String::NewSymbol("bindProgramPipeline"), v8::FunctionTemplate::New(EXPORT_bindProgramPipeline));
    exports->Set(v8::String::NewSymbol("genProgramPipelines"), v8::FunctionTemplate::New(EXPORT_genProgramPipelines));
    exports->Set(v8::String::NewSymbol("isProgramPipeline"), v8::FunctionTemplate::New(EXPORT_isProgramPipeline));
    exports->Set(v8::String::NewSymbol("getProgramPipelineiv"), v8::FunctionTemplate::New(EXPORT_getProgramPipelineiv));
    exports->Set(v8::String::NewSymbol("programUniform1i"), v8::FunctionTemplate::New(EXPORT_programUniform1i));
    exports->Set(v8::String::NewSymbol("programUniform1iv"), v8::FunctionTemplate::New(EXPORT_programUniform1iv));
    exports->Set(v8::String::NewSymbol("programUniform1f"), v8::FunctionTemplate::New(EXPORT_programUniform1f));
    exports->Set(v8::String::NewSymbol("programUniform1fv"), v8::FunctionTemplate::New(EXPORT_programUniform1fv));
    exports->Set(v8::String::NewSymbol("programUniform1d"), v8::FunctionTemplate::New(EXPORT_programUniform1d));
    exports->Set(v8::String::NewSymbol("programUniform1dv"), v8::FunctionTemplate::New(EXPORT_programUniform1dv));
    exports->Set(v8::String::NewSymbol("programUniform1ui"), v8::FunctionTemplate::New(EXPORT_programUniform1ui));
    exports->Set(v8::String::NewSymbol("programUniform1uiv"), v8::FunctionTemplate::New(EXPORT_programUniform1uiv));
    exports->Set(v8::String::NewSymbol("programUniform2i"), v8::FunctionTemplate::New(EXPORT_programUniform2i));
    exports->Set(v8::String::NewSymbol("programUniform2iv"), v8::FunctionTemplate::New(EXPORT_programUniform2iv));
    exports->Set(v8::String::NewSymbol("programUniform2f"), v8::FunctionTemplate::New(EXPORT_programUniform2f));
    exports->Set(v8::String::NewSymbol("programUniform2fv"), v8::FunctionTemplate::New(EXPORT_programUniform2fv));
    exports->Set(v8::String::NewSymbol("programUniform2d"), v8::FunctionTemplate::New(EXPORT_programUniform2d));
    exports->Set(v8::String::NewSymbol("programUniform2dv"), v8::FunctionTemplate::New(EXPORT_programUniform2dv));
    exports->Set(v8::String::NewSymbol("programUniform2ui"), v8::FunctionTemplate::New(EXPORT_programUniform2ui));
    exports->Set(v8::String::NewSymbol("programUniform2uiv"), v8::FunctionTemplate::New(EXPORT_programUniform2uiv));
    exports->Set(v8::String::NewSymbol("programUniform3i"), v8::FunctionTemplate::New(EXPORT_programUniform3i));
    exports->Set(v8::String::NewSymbol("programUniform3iv"), v8::FunctionTemplate::New(EXPORT_programUniform3iv));
    exports->Set(v8::String::NewSymbol("programUniform3f"), v8::FunctionTemplate::New(EXPORT_programUniform3f));
    exports->Set(v8::String::NewSymbol("programUniform3fv"), v8::FunctionTemplate::New(EXPORT_programUniform3fv));
    exports->Set(v8::String::NewSymbol("programUniform3d"), v8::FunctionTemplate::New(EXPORT_programUniform3d));
    exports->Set(v8::String::NewSymbol("programUniform3dv"), v8::FunctionTemplate::New(EXPORT_programUniform3dv));
    exports->Set(v8::String::NewSymbol("programUniform3ui"), v8::FunctionTemplate::New(EXPORT_programUniform3ui));
    exports->Set(v8::String::NewSymbol("programUniform3uiv"), v8::FunctionTemplate::New(EXPORT_programUniform3uiv));
    exports->Set(v8::String::NewSymbol("programUniform4i"), v8::FunctionTemplate::New(EXPORT_programUniform4i));
    exports->Set(v8::String::NewSymbol("programUniform4iv"), v8::FunctionTemplate::New(EXPORT_programUniform4iv));
    exports->Set(v8::String::NewSymbol("programUniform4f"), v8::FunctionTemplate::New(EXPORT_programUniform4f));
    exports->Set(v8::String::NewSymbol("programUniform4fv"), v8::FunctionTemplate::New(EXPORT_programUniform4fv));
    exports->Set(v8::String::NewSymbol("programUniform4d"), v8::FunctionTemplate::New(EXPORT_programUniform4d));
    exports->Set(v8::String::NewSymbol("programUniform4dv"), v8::FunctionTemplate::New(EXPORT_programUniform4dv));
    exports->Set(v8::String::NewSymbol("programUniform4ui"), v8::FunctionTemplate::New(EXPORT_programUniform4ui));
    exports->Set(v8::String::NewSymbol("programUniform4uiv"), v8::FunctionTemplate::New(EXPORT_programUniform4uiv));
    exports->Set(v8::String::NewSymbol("programUniformMatrix2fv"), v8::FunctionTemplate::New(EXPORT_programUniformMatrix2fv));
    exports->Set(v8::String::NewSymbol("programUniformMatrix3fv"), v8::FunctionTemplate::New(EXPORT_programUniformMatrix3fv));
    exports->Set(v8::String::NewSymbol("programUniformMatrix4fv"), v8::FunctionTemplate::New(EXPORT_programUniformMatrix4fv));
    exports->Set(v8::String::NewSymbol("programUniformMatrix2dv"), v8::FunctionTemplate::New(EXPORT_programUniformMatrix2dv));
    exports->Set(v8::String::NewSymbol("programUniformMatrix3dv"), v8::FunctionTemplate::New(EXPORT_programUniformMatrix3dv));
    exports->Set(v8::String::NewSymbol("programUniformMatrix4dv"), v8::FunctionTemplate::New(EXPORT_programUniformMatrix4dv));
    exports->Set(v8::String::NewSymbol("programUniformMatrix2x3fv"), v8::FunctionTemplate::New(EXPORT_programUniformMatrix2x3fv));
    exports->Set(v8::String::NewSymbol("programUniformMatrix3x2fv"), v8::FunctionTemplate::New(EXPORT_programUniformMatrix3x2fv));
    exports->Set(v8::String::NewSymbol("programUniformMatrix2x4fv"), v8::FunctionTemplate::New(EXPORT_programUniformMatrix2x4fv));
    exports->Set(v8::String::NewSymbol("programUniformMatrix4x2fv"), v8::FunctionTemplate::New(EXPORT_programUniformMatrix4x2fv));
    exports->Set(v8::String::NewSymbol("programUniformMatrix3x4fv"), v8::FunctionTemplate::New(EXPORT_programUniformMatrix3x4fv));
    exports->Set(v8::String::NewSymbol("programUniformMatrix4x3fv"), v8::FunctionTemplate::New(EXPORT_programUniformMatrix4x3fv));
    exports->Set(v8::String::NewSymbol("programUniformMatrix2x3dv"), v8::FunctionTemplate::New(EXPORT_programUniformMatrix2x3dv));
    exports->Set(v8::String::NewSymbol("programUniformMatrix3x2dv"), v8::FunctionTemplate::New(EXPORT_programUniformMatrix3x2dv));
    exports->Set(v8::String::NewSymbol("programUniformMatrix2x4dv"), v8::FunctionTemplate::New(EXPORT_programUniformMatrix2x4dv));
    exports->Set(v8::String::NewSymbol("programUniformMatrix4x2dv"), v8::FunctionTemplate::New(EXPORT_programUniformMatrix4x2dv));
    exports->Set(v8::String::NewSymbol("programUniformMatrix3x4dv"), v8::FunctionTemplate::New(EXPORT_programUniformMatrix3x4dv));
    exports->Set(v8::String::NewSymbol("programUniformMatrix4x3dv"), v8::FunctionTemplate::New(EXPORT_programUniformMatrix4x3dv));
    exports->Set(v8::String::NewSymbol("validateProgramPipeline"), v8::FunctionTemplate::New(EXPORT_validateProgramPipeline));
    exports->Set(v8::String::NewSymbol("getProgramPipelineInfoLog"), v8::FunctionTemplate::New(EXPORT_getProgramPipelineInfoLog));
    exports->Set(v8::String::NewSymbol("vertexAttribL1d"), v8::FunctionTemplate::New(EXPORT_vertexAttribL1d));
    exports->Set(v8::String::NewSymbol("vertexAttribL2d"), v8::FunctionTemplate::New(EXPORT_vertexAttribL2d));
    exports->Set(v8::String::NewSymbol("vertexAttribL3d"), v8::FunctionTemplate::New(EXPORT_vertexAttribL3d));
    exports->Set(v8::String::NewSymbol("vertexAttribL4d"), v8::FunctionTemplate::New(EXPORT_vertexAttribL4d));
    exports->Set(v8::String::NewSymbol("vertexAttribL1dv"), v8::FunctionTemplate::New(EXPORT_vertexAttribL1dv));
    exports->Set(v8::String::NewSymbol("vertexAttribL2dv"), v8::FunctionTemplate::New(EXPORT_vertexAttribL2dv));
    exports->Set(v8::String::NewSymbol("vertexAttribL3dv"), v8::FunctionTemplate::New(EXPORT_vertexAttribL3dv));
    exports->Set(v8::String::NewSymbol("vertexAttribL4dv"), v8::FunctionTemplate::New(EXPORT_vertexAttribL4dv));
    exports->Set(v8::String::NewSymbol("vertexAttribLPointer"), v8::FunctionTemplate::New(EXPORT_vertexAttribLPointer));
    exports->Set(v8::String::NewSymbol("getVertexAttribLdv"), v8::FunctionTemplate::New(EXPORT_getVertexAttribLdv));
    exports->Set(v8::String::NewSymbol("viewportArrayv"), v8::FunctionTemplate::New(EXPORT_viewportArrayv));
    exports->Set(v8::String::NewSymbol("viewportIndexedf"), v8::FunctionTemplate::New(EXPORT_viewportIndexedf));
    exports->Set(v8::String::NewSymbol("viewportIndexedfv"), v8::FunctionTemplate::New(EXPORT_viewportIndexedfv));
    exports->Set(v8::String::NewSymbol("scissorArrayv"), v8::FunctionTemplate::New(EXPORT_scissorArrayv));
    exports->Set(v8::String::NewSymbol("scissorIndexed"), v8::FunctionTemplate::New(EXPORT_scissorIndexed));
    exports->Set(v8::String::NewSymbol("scissorIndexedv"), v8::FunctionTemplate::New(EXPORT_scissorIndexedv));
    exports->Set(v8::String::NewSymbol("depthRangeArrayv"), v8::FunctionTemplate::New(EXPORT_depthRangeArrayv));
    exports->Set(v8::String::NewSymbol("depthRangeIndexed"), v8::FunctionTemplate::New(EXPORT_depthRangeIndexed));
    exports->Set(v8::String::NewSymbol("getFloati_v"), v8::FunctionTemplate::New(EXPORT_getFloati_v));
    exports->Set(v8::String::NewSymbol("getDoublei_v"), v8::FunctionTemplate::New(EXPORT_getDoublei_v));
    exports->Set(v8::String::NewSymbol("shaderSource"), v8::FunctionTemplate::New(EXPORT_shaderSource));
}


void gl3BindInit(v8::Handle<v8::Object> exports) {
    v8::HandleScope scope;
    v8::Handle<v8::ObjectTemplate> GL3 = v8::ObjectTemplate::New();
    GL3->SetInternalFieldCount(1);
    defineConstants(GL3);
    defineFunctions(GL3);
    defineObjects(GL3);
    exports->Set(v8::String::NewSymbol("GL3"), GL3->NewInstance());
}
