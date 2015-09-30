#include <nan.h>
#include <node_buffer.h>

#include "node_opengl.h"
#include "allofw/opengl.h"

#include "gl3binding/glbind.h"

using namespace v8;

class NODE_OpenGLWindow : public node::ObjectWrap, public allofw::OpenGLWindow::Delegate {
public:
    static void Init(v8::Handle<v8::Object> exports) {
        Nan::HandleScope scope;

        // New({ width:, height:, active_stereo:, fullscreen:, title:, config: (config rules all) }
        v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
        tpl->SetClassName(Nan::New<v8::String>("OpenGLWindow").ToLocalChecked());
        tpl->InstanceTemplate()->SetInternalFieldCount(1);

        // Prototype.
        Nan::SetPrototypeMethod(tpl, "onMove", NODE_onMove);
        Nan::SetPrototypeMethod(tpl, "onResize", NODE_onResize);
        Nan::SetPrototypeMethod(tpl, "onClose", NODE_onClose);
        Nan::SetPrototypeMethod(tpl, "onRefresh", NODE_onRefresh);
        Nan::SetPrototypeMethod(tpl, "onFocus", NODE_onFocus);
        Nan::SetPrototypeMethod(tpl, "onIconify", NODE_onIconify);
        Nan::SetPrototypeMethod(tpl, "onFramebufferSize", NODE_onFramebufferSize);
        Nan::SetPrototypeMethod(tpl, "onKeyboard", NODE_onKeyboard);

        Nan::SetPrototypeMethod(tpl, "makeContextCurrent", NODE_makeContextCurrent);
        Nan::SetPrototypeMethod(tpl, "swapBuffers", NODE_swapBuffers);
        Nan::SetPrototypeMethod(tpl, "pollEvents", NODE_pollEvents);
        Nan::SetPrototypeMethod(tpl, "waitEvents", NODE_waitEvents);
        Nan::SetPrototypeMethod(tpl, "getFramebufferSize", NODE_getFramebufferSize);
        Nan::SetPrototypeMethod(tpl, "close", NODE_close);

        constructor.Reset(tpl->GetFunction());

        // Export constructor.
        Nan::Set(exports, Nan::New<v8::String>("OpenGLWindow").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
    }

    virtual void onMove(int x, int y) override {
        if(!pf_onMove.IsEmpty()) {
            Nan::HandleScope scope;
            Local<Value> argv[2] = { Nan::New<Integer>(x), Nan::New<Integer>(y) };
            Nan::New(pf_onMove)->Call(handle(), 2, argv);
        }
    }
    virtual void onResize(int width, int height) override {
        if(!pf_onResize.IsEmpty()) {
            Nan::HandleScope scope;
            Local<Value> argv[2] = { Nan::New<Integer>(width), Nan::New<Integer>(height) };
            Nan::New(pf_onResize)->Call(handle(), 2, argv);
        }
    }
    virtual void onClose() override {
        if(!pf_onClose.IsEmpty()) {
            Nan::HandleScope scope;
            Nan::New(pf_onClose)->Call(handle(), 0, NULL);
        }
    }
    virtual void onRefresh() override {
        if(!pf_onRefresh.IsEmpty()) {
            Nan::HandleScope scope;
            Nan::New(pf_onRefresh)->Call(handle(), 0, NULL);
        }
    }
    virtual void onFocus(int focused) override {
        if(!pf_onFocus.IsEmpty()) {
            Nan::HandleScope scope;
            Local<Value> argv[1] = { Nan::New<Boolean>(focused == GL_TRUE) };
            Nan::New(pf_onFocus)->Call(handle(), 1, argv);
        }
    }
    virtual void onIconify(int iconified) override {
        if(!pf_onIconify.IsEmpty()) {
            Nan::HandleScope scope;
            Local<Value> argv[1] = { Nan::New<Boolean>(iconified == GL_TRUE) };
            Nan::New(pf_onIconify)->Call(handle(), 1, argv);
        }
    }
    virtual void onFramebufferSize(int width, int height) override {
        if(!pf_onFramebufferSize.IsEmpty()) {
            Nan::HandleScope scope;
            Local<Value> argv[2] = { Nan::New<Integer>(width), Nan::New<Integer>(height) };
            Nan::New(pf_onFramebufferSize)->Call(handle(), 2, argv);
        }
    }
    virtual void onKeyboard(const char* key, const char* action, const char* modifiers, int scancode) override {
        if(!pf_onKeyboard.IsEmpty()) {
            Nan::HandleScope scope;
            Local<Value> argv[4] = { Nan::New<String>(key).ToLocalChecked(), Nan::New<String>(action).ToLocalChecked(), Nan::New<String>(modifiers).ToLocalChecked(), Nan::New<Integer>(scancode) };
            Nan::New(pf_onKeyboard)->Call(handle(), 4, argv);
        }
    }

private:
    explicit NODE_OpenGLWindow(allofw::OpenGLWindow* window_) : window(window_) {
        window->setDelegate(this);
    }
    ~NODE_OpenGLWindow() {
        pf_onMove.Reset();
        pf_onResize.Reset();
        pf_onClose.Reset();
        pf_onRefresh.Reset();
        pf_onFocus.Reset();
        pf_onIconify.Reset();
        pf_onFramebufferSize.Reset();
        pf_onKeyboard.Reset();
        if(window) {
            allofw::OpenGLWindow::Destroy(window);
        }
    }

    allofw::OpenGLWindow* window;

    Nan::Persistent<v8::Function> pf_onMove;
    Nan::Persistent<v8::Function> pf_onResize;
    Nan::Persistent<v8::Function> pf_onClose;
    Nan::Persistent<v8::Function> pf_onRefresh;
    Nan::Persistent<v8::Function> pf_onFocus;
    Nan::Persistent<v8::Function> pf_onIconify;
    Nan::Persistent<v8::Function> pf_onFramebufferSize;
    Nan::Persistent<v8::Function> pf_onKeyboard;

    static NAN_METHOD(New);

    static NAN_METHOD(NODE_onMove);
    static NAN_METHOD(NODE_onResize);
    static NAN_METHOD(NODE_onClose);
    static NAN_METHOD(NODE_onRefresh);
    static NAN_METHOD(NODE_onFocus);
    static NAN_METHOD(NODE_onIconify);
    static NAN_METHOD(NODE_onFramebufferSize);
    static NAN_METHOD(NODE_onKeyboard);

    static NAN_METHOD(NODE_makeContextCurrent);
    static NAN_METHOD(NODE_swapBuffers);
    static NAN_METHOD(NODE_pollEvents);
    static NAN_METHOD(NODE_waitEvents);
    static NAN_METHOD(NODE_getFramebufferSize);
    static NAN_METHOD(NODE_close);

    static Nan::Persistent<v8::Function> constructor;
};

Nan::Persistent<v8::Function> NODE_OpenGLWindow::constructor;

NAN_METHOD(NODE_OpenGLWindow::New) {
    Nan::HandleScope scope;
    if (info.IsConstructCall()) {
        allofw::OpenGLWindow* window;
        if(info[0]->IsObject()) {
            Local<Object> params = info[0]->ToObject();
            if(Nan::Has(params, Nan::New<String>("config").ToLocalChecked()).FromMaybe(false)) {
                Nan::Utf8String str(Nan::Get(params, Nan::New<String>("config").ToLocalChecked()).ToLocalChecked());
                allofw::Configuration* config = allofw::Configuration::CreateFromFile(*str);
                window = allofw::OpenGLWindow::Create(config);
            } else {
                allofw::OpenGLWindow::Hint hint;
                std::string title("AllofwWindow");
                if(Nan::Has(params, Nan::New<String>("title").ToLocalChecked()).FromMaybe(false)) {
                    Nan::Utf8String str(Nan::Get(params, Nan::New<String>("title").ToLocalChecked()).ToLocalChecked());
                    title = *str;
                }
                if(Nan::Has(params, Nan::New<String>("width").ToLocalChecked()).FromMaybe(false)) {
                    hint.width = Nan::Get(params, Nan::New<String>("width").ToLocalChecked()).ToLocalChecked()->IntegerValue();
                }
                if(Nan::Has(params, Nan::New<String>("height").ToLocalChecked()).FromMaybe(false)) {
                    hint.height = Nan::Get(params, Nan::New<String>("height").ToLocalChecked()).ToLocalChecked()->IntegerValue();
                }
                if(Nan::Has(params, Nan::New<String>("active_stereo").ToLocalChecked()).FromMaybe(false)) {
                    hint.active_stereo = Nan::Get(params, Nan::New<String>("active_stereo").ToLocalChecked()).ToLocalChecked()->BooleanValue();
                }
                if(Nan::Has(params, Nan::New<String>("hide_cursor").ToLocalChecked()).FromMaybe(false)) {
                    hint.hide_cursor = Nan::Get(params, Nan::New<String>("active_stereo").ToLocalChecked()).ToLocalChecked()->BooleanValue();
                }
                if(Nan::Has(params, Nan::New<String>("fullscreen").ToLocalChecked()).FromMaybe(false)) {
                    hint.fullscreen = Nan::Get(params, Nan::New<String>("fullscreen").ToLocalChecked()).ToLocalChecked()->BooleanValue();
                }
                window = allofw::OpenGLWindow::Create(hint, title.c_str());
            }
        } else {
            allofw::OpenGLWindow::Hint hint;
            std::string title("Allofw.js Window");
            window = allofw::OpenGLWindow::Create(hint, title.c_str());
        }
        NODE_OpenGLWindow* obj = new NODE_OpenGLWindow(window);
        obj->Wrap(info.This());
        info.GetReturnValue().Set(info.This());
    }
    else {
        v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
        info.GetReturnValue().Set(cons->NewInstance());
    }
}

NAN_METHOD(NODE_OpenGLWindow::NODE_onMove) {
    Nan::HandleScope scope;
    if(info.Length() != 1) return Nan::ThrowError("E_INVALID_ARGUMENTS");
    if(!info[0]->IsFunction()) return Nan::ThrowError("E_INVALID_ARGUMENTS");
    NODE_OpenGLWindow* obj = node::ObjectWrap::Unwrap<NODE_OpenGLWindow>(info.This());
    obj->pf_onMove.Reset(info[0].As<v8::Function>());
}

NAN_METHOD(NODE_OpenGLWindow::NODE_onResize) {
    Nan::HandleScope scope;
    if(info.Length() != 1) return Nan::ThrowError("E_INVALID_ARGUMENTS");
    if(!info[0]->IsFunction()) return Nan::ThrowError("E_INVALID_ARGUMENTS");
    NODE_OpenGLWindow* obj = node::ObjectWrap::Unwrap<NODE_OpenGLWindow>(info.This());
    obj->pf_onResize.Reset(info[0].As<v8::Function>());
}

NAN_METHOD(NODE_OpenGLWindow::NODE_onClose) {
    Nan::HandleScope scope;
    if(info.Length() != 1) return Nan::ThrowError("E_INVALID_ARGUMENTS");
    if(!info[0]->IsFunction()) return Nan::ThrowError("E_INVALID_ARGUMENTS");
    NODE_OpenGLWindow* obj = node::ObjectWrap::Unwrap<NODE_OpenGLWindow>(info.This());
    obj->pf_onClose.Reset(info[0].As<v8::Function>());
}

NAN_METHOD(NODE_OpenGLWindow::NODE_onRefresh) {
    Nan::HandleScope scope;
    if(info.Length() != 1) return Nan::ThrowError("E_INVALID_ARGUMENTS");
    if(!info[0]->IsFunction()) return Nan::ThrowError("E_INVALID_ARGUMENTS");
    NODE_OpenGLWindow* obj = node::ObjectWrap::Unwrap<NODE_OpenGLWindow>(info.This());
    obj->pf_onRefresh.Reset(info[0].As<v8::Function>());
}

NAN_METHOD(NODE_OpenGLWindow::NODE_onFocus) {
    Nan::HandleScope scope;
    if(info.Length() != 1) return Nan::ThrowError("E_INVALID_ARGUMENTS");
    if(!info[0]->IsFunction()) return Nan::ThrowError("E_INVALID_ARGUMENTS");
    NODE_OpenGLWindow* obj = node::ObjectWrap::Unwrap<NODE_OpenGLWindow>(info.This());
    obj->pf_onFocus.Reset(info[0].As<v8::Function>());
}

NAN_METHOD(NODE_OpenGLWindow::NODE_onIconify) {
    Nan::HandleScope scope;
    if(info.Length() != 1) return Nan::ThrowError("E_INVALID_ARGUMENTS");
    if(!info[0]->IsFunction()) return Nan::ThrowError("E_INVALID_ARGUMENTS");
    NODE_OpenGLWindow* obj = node::ObjectWrap::Unwrap<NODE_OpenGLWindow>(info.This());
    obj->pf_onIconify.Reset(info[0].As<v8::Function>());
}

NAN_METHOD(NODE_OpenGLWindow::NODE_onFramebufferSize) {
    Nan::HandleScope scope;
    if(info.Length() != 1) return Nan::ThrowError("E_INVALID_ARGUMENTS");
    if(!info[0]->IsFunction()) return Nan::ThrowError("E_INVALID_ARGUMENTS");
    NODE_OpenGLWindow* obj = node::ObjectWrap::Unwrap<NODE_OpenGLWindow>(info.This());
    obj->pf_onFramebufferSize.Reset(info[0].As<v8::Function>());
}
NAN_METHOD(NODE_OpenGLWindow::NODE_onKeyboard) {
    Nan::HandleScope scope;
    if(info.Length() != 1) return Nan::ThrowError("E_INVALID_ARGUMENTS");
    if(!info[0]->IsFunction()) return Nan::ThrowError("E_INVALID_ARGUMENTS");
    NODE_OpenGLWindow* obj = node::ObjectWrap::Unwrap<NODE_OpenGLWindow>(info.This());
    obj->window->enableKeyboardInput();
    obj->pf_onKeyboard.Reset(info[0].As<v8::Function>());
}

NAN_METHOD(NODE_OpenGLWindow::NODE_makeContextCurrent) {
    Nan::HandleScope scope;
    NODE_OpenGLWindow* obj = node::ObjectWrap::Unwrap<NODE_OpenGLWindow>(info.This());
    obj->window->makeContextCurrent();
}
NAN_METHOD(NODE_OpenGLWindow::NODE_swapBuffers) {
    Nan::HandleScope scope;
    NODE_OpenGLWindow* obj = node::ObjectWrap::Unwrap<NODE_OpenGLWindow>(info.This());
    obj->window->swapBuffers();
}
NAN_METHOD(NODE_OpenGLWindow::NODE_pollEvents) {
    Nan::HandleScope scope;
    NODE_OpenGLWindow* obj = node::ObjectWrap::Unwrap<NODE_OpenGLWindow>(info.This());
    obj->window->pollEvents();
}
NAN_METHOD(NODE_OpenGLWindow::NODE_waitEvents) {
    Nan::HandleScope scope;
    NODE_OpenGLWindow* obj = node::ObjectWrap::Unwrap<NODE_OpenGLWindow>(info.This());
    obj->window->waitEvents();
}
NAN_METHOD(NODE_OpenGLWindow::NODE_getFramebufferSize) {
    Nan::HandleScope scope;
    NODE_OpenGLWindow* obj = node::ObjectWrap::Unwrap<NODE_OpenGLWindow>(info.This());
    allofw::Size2i result = obj->window->getFramebufferSize();
    v8::Local<Array> r = Nan::New<Array>(2);
    r->Set(0, Nan::New<Integer>(result.x));
    r->Set(1, Nan::New<Integer>(result.y));
    info.GetReturnValue().Set(r);
}
NAN_METHOD(NODE_OpenGLWindow::NODE_close) {
    Nan::HandleScope scope;
    NODE_OpenGLWindow* obj = node::ObjectWrap::Unwrap<NODE_OpenGLWindow>(info.This());
    obj->window->close();
}

void NODE_OpenGL_Init(v8::Handle<v8::Object> exports) {
    NODE_OpenGLWindow::Init(exports);
    gl3BindInit(exports);
}
