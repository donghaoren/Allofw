#include <nan.h>
#include <node_buffer.h>

#include "node_opengl.h"
#include "allofw/opengl.h"

#include "gl3binding/glbind.h"

using namespace v8;

class NODE_OpenGLWindow : public node::ObjectWrap, public allofw::OpenGLWindow::Delegate {
public:
    static void Init(v8::Handle<v8::Object> exports) {
        NanScope();

        v8::Local<v8::FunctionTemplate> tpl = NanNew<v8::FunctionTemplate>(New);
        tpl->SetClassName(NanNew<v8::String>("OpenGLWindow"));
        tpl->InstanceTemplate()->SetInternalFieldCount(1);

        // Prototype.
        NODE_SET_PROTOTYPE_METHOD(tpl, "onMove", NODE_onMove);
        NODE_SET_PROTOTYPE_METHOD(tpl, "onResize", NODE_onResize);
        NODE_SET_PROTOTYPE_METHOD(tpl, "onClose", NODE_onClose);
        NODE_SET_PROTOTYPE_METHOD(tpl, "onRefresh", NODE_onRefresh);
        NODE_SET_PROTOTYPE_METHOD(tpl, "onFocus", NODE_onFocus);
        NODE_SET_PROTOTYPE_METHOD(tpl, "onIconify", NODE_onIconify);
        NODE_SET_PROTOTYPE_METHOD(tpl, "onFramebufferSize", NODE_onFramebufferSize);

        NODE_SET_PROTOTYPE_METHOD(tpl, "makeContextCurrent", NODE_makeContextCurrent);
        NODE_SET_PROTOTYPE_METHOD(tpl, "swapBuffers", NODE_swapBuffers);
        NODE_SET_PROTOTYPE_METHOD(tpl, "pollEvents", NODE_pollEvents);
        NODE_SET_PROTOTYPE_METHOD(tpl, "waitEvents", NODE_waitEvents);
        NODE_SET_PROTOTYPE_METHOD(tpl, "getFramebufferSize", NODE_getFramebufferSize);

        NanAssignPersistent(constructor, tpl->GetFunction());

        // Export constructor.
        exports->Set(NanNew<v8::String>("OpenGLWindow"), tpl->GetFunction());
    }

    virtual void onMove(int x, int y) override {
        if(!pf_onMove.IsEmpty()) {
            NanScope();
            Local<Value> argv[2] = { NanNew<Integer>(x), NanNew<Integer>(y) };
            NanNew(pf_onMove)->Call(NanObjectWrapHandle(this), 2, argv);
        }
    }
    virtual void onResize(int width, int height) override {
        if(!pf_onResize.IsEmpty()) {
            NanScope();
            Local<Value> argv[2] = { NanNew<Integer>(width), NanNew<Integer>(height) };
            NanNew(pf_onResize)->Call(NanObjectWrapHandle(this), 2, argv);
        }
    }
    virtual void onClose() override {
        if(!pf_onClose.IsEmpty()) {
            NanScope();
            NanNew(pf_onClose)->Call(NanObjectWrapHandle(this), 0, NULL);
        }
    }
    virtual void onRefresh() override {
        if(!pf_onRefresh.IsEmpty()) {
            NanScope();
            NanNew(pf_onRefresh)->Call(NanObjectWrapHandle(this), 0, NULL);
        }
    }
    virtual void onFocus(int focused) override {
        if(!pf_onFocus.IsEmpty()) {
            NanScope();
            Local<Value> argv[1] = { NanNew<Boolean>(focused == GL_TRUE) };
            NanNew(pf_onFocus)->Call(NanObjectWrapHandle(this), 1, argv);
        }
    }
    virtual void onIconify(int iconified) override {
        if(!pf_onIconify.IsEmpty()) {
            NanScope();
            Local<Value> argv[1] = { NanNew<Boolean>(iconified == GL_TRUE) };
            NanNew(pf_onIconify)->Call(NanObjectWrapHandle(this), 1, argv);
        }
    }
    virtual void onFramebufferSize(int width, int height) override {
        if(!pf_onFramebufferSize.IsEmpty()) {
            NanScope();
            Local<Value> argv[2] = { NanNew<Integer>(width), NanNew<Integer>(height) };
            NanNew(pf_onFramebufferSize)->Call(NanObjectWrapHandle(this), 2, argv);
        }
    }

private:
    explicit NODE_OpenGLWindow(allofw::OpenGLWindow* window_) : window(window_) {
        window->setDelegate(this);
    }
    ~NODE_OpenGLWindow() {
        NanDisposePersistent(pf_onMove);
        NanDisposePersistent(pf_onResize);
        NanDisposePersistent(pf_onClose);
        NanDisposePersistent(pf_onRefresh);
        NanDisposePersistent(pf_onFocus);
        NanDisposePersistent(pf_onIconify);
        NanDisposePersistent(pf_onFramebufferSize);
        if(window) {
            delete window;
        }
    }

    allofw::OpenGLWindow* window;

    v8::Persistent<v8::Function> pf_onMove;
    v8::Persistent<v8::Function> pf_onResize;
    v8::Persistent<v8::Function> pf_onClose;
    v8::Persistent<v8::Function> pf_onRefresh;
    v8::Persistent<v8::Function> pf_onFocus;
    v8::Persistent<v8::Function> pf_onIconify;
    v8::Persistent<v8::Function> pf_onFramebufferSize;

    static NAN_METHOD(New);

    static NAN_METHOD(NODE_onMove);
    static NAN_METHOD(NODE_onResize);
    static NAN_METHOD(NODE_onClose);
    static NAN_METHOD(NODE_onRefresh);
    static NAN_METHOD(NODE_onFocus);
    static NAN_METHOD(NODE_onIconify);
    static NAN_METHOD(NODE_onFramebufferSize);

    static NAN_METHOD(NODE_makeContextCurrent);
    static NAN_METHOD(NODE_swapBuffers);
    static NAN_METHOD(NODE_pollEvents);
    static NAN_METHOD(NODE_waitEvents);
    static NAN_METHOD(NODE_getFramebufferSize);

    static v8::Persistent<v8::Function> constructor;
};

v8::Persistent<v8::Function> NODE_OpenGLWindow::constructor;

NAN_METHOD(NODE_OpenGLWindow::New) {
    NanScope();
    if (args.IsConstructCall()) {
        allofw::OpenGLWindow::Hint hint;
        std::string title("AllofwWindow");
        allofw::OpenGLWindow* window = allofw::OpenGLWindow::Create(hint, title.c_str());
        NODE_OpenGLWindow* obj = new NODE_OpenGLWindow(window);
        obj->Wrap(args.This());
        NanReturnValue(args.This());
    }
    else {
        v8::Local<v8::Function> cons = NanNew<v8::Function>(constructor);
        NanReturnValue(cons->NewInstance());
    }
}

NAN_METHOD(NODE_OpenGLWindow::NODE_onMove) {
    NanScope();
    if(args.Length() != 1) return NanThrowError("E_INVALID_ARGUMENTS");
    if(!args[0]->IsFunction()) return NanThrowError("E_INVALID_ARGUMENTS");
    NODE_OpenGLWindow* obj = node::ObjectWrap::Unwrap<NODE_OpenGLWindow>(args.This());
    NanAssignPersistent(obj->pf_onMove, args[0].As<v8::Function>());
    NanReturnUndefined();
}

NAN_METHOD(NODE_OpenGLWindow::NODE_onResize) {
    NanScope();
    if(args.Length() != 1) return NanThrowError("E_INVALID_ARGUMENTS");
    if(!args[0]->IsFunction()) return NanThrowError("E_INVALID_ARGUMENTS");
    NODE_OpenGLWindow* obj = node::ObjectWrap::Unwrap<NODE_OpenGLWindow>(args.This());
    NanAssignPersistent(obj->pf_onResize, args[0].As<v8::Function>());
    NanReturnUndefined();
}

NAN_METHOD(NODE_OpenGLWindow::NODE_onClose) {
    NanScope();
    if(args.Length() != 1) return NanThrowError("E_INVALID_ARGUMENTS");
    if(!args[0]->IsFunction()) return NanThrowError("E_INVALID_ARGUMENTS");
    NODE_OpenGLWindow* obj = node::ObjectWrap::Unwrap<NODE_OpenGLWindow>(args.This());
    NanAssignPersistent(obj->pf_onClose, args[0].As<v8::Function>());
    NanReturnUndefined();
}

NAN_METHOD(NODE_OpenGLWindow::NODE_onRefresh) {
    NanScope();
    if(args.Length() != 1) return NanThrowError("E_INVALID_ARGUMENTS");
    if(!args[0]->IsFunction()) return NanThrowError("E_INVALID_ARGUMENTS");
    NODE_OpenGLWindow* obj = node::ObjectWrap::Unwrap<NODE_OpenGLWindow>(args.This());
    NanAssignPersistent(obj->pf_onRefresh, args[0].As<v8::Function>());
    NanReturnUndefined();
}

NAN_METHOD(NODE_OpenGLWindow::NODE_onFocus) {
    NanScope();
    if(args.Length() != 1) return NanThrowError("E_INVALID_ARGUMENTS");
    if(!args[0]->IsFunction()) return NanThrowError("E_INVALID_ARGUMENTS");
    NODE_OpenGLWindow* obj = node::ObjectWrap::Unwrap<NODE_OpenGLWindow>(args.This());
    NanAssignPersistent(obj->pf_onFocus, args[0].As<v8::Function>());
    NanReturnUndefined();
}

NAN_METHOD(NODE_OpenGLWindow::NODE_onIconify) {
    NanScope();
    if(args.Length() != 1) return NanThrowError("E_INVALID_ARGUMENTS");
    if(!args[0]->IsFunction()) return NanThrowError("E_INVALID_ARGUMENTS");
    NODE_OpenGLWindow* obj = node::ObjectWrap::Unwrap<NODE_OpenGLWindow>(args.This());
    NanAssignPersistent(obj->pf_onIconify, args[0].As<v8::Function>());
    NanReturnUndefined();
}

NAN_METHOD(NODE_OpenGLWindow::NODE_onFramebufferSize) {
    NanScope();
    if(args.Length() != 1) return NanThrowError("E_INVALID_ARGUMENTS");
    if(!args[0]->IsFunction()) return NanThrowError("E_INVALID_ARGUMENTS");
    NODE_OpenGLWindow* obj = node::ObjectWrap::Unwrap<NODE_OpenGLWindow>(args.This());
    NanAssignPersistent(obj->pf_onFramebufferSize, args[0].As<v8::Function>());
    NanReturnUndefined();
}

NAN_METHOD(NODE_OpenGLWindow::NODE_makeContextCurrent) {
    NanScope();
    NODE_OpenGLWindow* obj = node::ObjectWrap::Unwrap<NODE_OpenGLWindow>(args.This());
    obj->window->makeContextCurrent();
    NanReturnUndefined();
}
NAN_METHOD(NODE_OpenGLWindow::NODE_swapBuffers) {
    NanScope();
    NODE_OpenGLWindow* obj = node::ObjectWrap::Unwrap<NODE_OpenGLWindow>(args.This());
    obj->window->swapBuffers();
    NanReturnUndefined();
}
NAN_METHOD(NODE_OpenGLWindow::NODE_pollEvents) {
    NanScope();
    NODE_OpenGLWindow* obj = node::ObjectWrap::Unwrap<NODE_OpenGLWindow>(args.This());
    obj->window->pollEvents();
    NanReturnUndefined();
}
NAN_METHOD(NODE_OpenGLWindow::NODE_waitEvents) {
    NanScope();
    NODE_OpenGLWindow* obj = node::ObjectWrap::Unwrap<NODE_OpenGLWindow>(args.This());
    obj->window->waitEvents();
    NanReturnUndefined();
}
NAN_METHOD(NODE_OpenGLWindow::NODE_getFramebufferSize) {
    NanScope();
    NODE_OpenGLWindow* obj = node::ObjectWrap::Unwrap<NODE_OpenGLWindow>(args.This());
    allofw::Size2i result = obj->window->getFramebufferSize();
    v8::Local<Array> r = NanNew<Array>(2);
    r->Set(0, NanNew<Integer>(result.x));
    r->Set(1, NanNew<Integer>(result.y));
    NanReturnValue(r);
}

void NODE_OpenGL_Init(v8::Handle<v8::Object> exports) {
    NODE_OpenGLWindow::Init(exports);
    gl3BindInit(exports);
}
