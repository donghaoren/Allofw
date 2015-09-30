#include <node.h>
#include <node_buffer.h>
#include <v8.h>

#include <iostream>

#include <allofw/graphics.h>

#include "node_graphics.h"

using namespace v8;
using namespace allofw;

GraphicalBackend* backend;

void NODE_Surface2D::Init(Handle<Object> exports) {
    Nan::HandleScope scope;
    // Prepare constructor template
    Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
    tpl->SetClassName(Nan::New<String>("Surface2D").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    Nan::SetPrototypeMethod(tpl, "width", NODE_width);
    Nan::SetPrototypeMethod(tpl, "height", NODE_height);
    Nan::SetPrototypeMethod(tpl, "pixels", NODE_pixels);
    Nan::SetPrototypeMethod(tpl, "bindTexture", NODE_bindTexture);
    Nan::SetPrototypeMethod(tpl, "uploadTexture", NODE_uploadTexture);
    Nan::SetPrototypeMethod(tpl, "unbindTexture", NODE_unbindTexture);
    Nan::SetPrototypeMethod(tpl, "save", NODE_save);

    constructor.Reset(tpl->GetFunction());

    // Export constructor.
    Nan::Set(exports, Nan::New<String>("Surface2D").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

NODE_Surface2D::NODE_Surface2D(int width, int height, int type) {
    if(type == SURFACETYPE_PDF) {
        surface = backend->createPDFSurface2D(width, height);
    } else {
        surface = backend->createSurface2D(width, height);
    }
}

NODE_Surface2D::NODE_Surface2D(int width, int height, void* pixels) {
    surface = backend->createSurface2DWithPixels(width, height, pixels);
}

NODE_Surface2D::NODE_Surface2D(const void* data, size_t length) {
    surface = backend->createSurface2DFromImage(data, length);
}

NODE_Surface2D::~NODE_Surface2D() {
    backend->destroySurface2D(surface);
}

NAN_METHOD(NODE_Surface2D::New) {
    Nan::HandleScope scope;

    if(info.IsConstructCall()) {
        try {
            // Invoked as constructor: `new MyObject(...)`
            if(node::Buffer::HasInstance(info[0])) {
                // Constructing with a buffer object.
                NODE_Surface2D* obj = new NODE_Surface2D(node::Buffer::Data(info[0]), node::Buffer::Length(info[0]));
                obj->Wrap(info.This());
                info.GetReturnValue().Set(info.This());
            } else {
                int width = info[0]->IsUndefined() ? 0 : info[0]->IntegerValue();
                int height = info[1]->IsUndefined() ? 0 : info[1]->IntegerValue();
                if(node::Buffer::HasInstance(info[2])) {
                    NODE_Surface2D* obj = new NODE_Surface2D(width, height, node::Buffer::Data(info[2]));
                    obj->Wrap(info.This());
                    info.GetReturnValue().Set(info.This());
                } else {
                    int type = info[2]->IsUndefined() ? SURFACETYPE_RASTER : info[2]->IntegerValue();
                    NODE_Surface2D* obj = new NODE_Surface2D(width, height, type);
                    obj->Wrap(info.This());
                    info.GetReturnValue().Set(info.This());
                }
            }
        } catch(const allofw::exception& e) {
            Nan::ThrowError(e.what());
        } catch(...) {
            Nan::ThrowError("unknown exception");
        }
    } else {
        // Invoked as plain function `MyObject(...)`, turn into construct call.
        const int argc = 2;
        Local<Value> argv[argc] = { info[0], info[1] };
        info.GetReturnValue().Set(Nan::New(constructor)->NewInstance(argc, argv));
    }
}

NAN_METHOD(NODE_Surface2D::NODE_width) {
    NODE_Surface2D* obj = ObjectWrap::Unwrap<NODE_Surface2D>(info.This());
    info.GetReturnValue().Set(Nan::New<Uint32>(obj->surface->width()));
}

NAN_METHOD(NODE_Surface2D::NODE_height) {
    NODE_Surface2D* obj = ObjectWrap::Unwrap<NODE_Surface2D>(info.This());
    info.GetReturnValue().Set(Nan::New<Uint32>(obj->surface->height()));
}

void do_nothing_callback2(char*, void*) { }

NAN_METHOD(NODE_Surface2D::NODE_pixels) {
    NODE_Surface2D* obj = ObjectWrap::Unwrap<NODE_Surface2D>(info.This());
    info.GetReturnValue().Set(Nan::NewBuffer((char*)obj->surface->pixels(), obj->surface->width() * obj->surface->height() * 4, do_nothing_callback2, NULL).ToLocalChecked());
}

NAN_METHOD(NODE_Surface2D::NODE_bindTexture) {
    NODE_Surface2D* obj = ObjectWrap::Unwrap<NODE_Surface2D>(info.This());
    obj->surface->bindTexture(info[0]->IntegerValue());
    return;
}

NAN_METHOD(NODE_Surface2D::NODE_uploadTexture) {
    NODE_Surface2D* obj = ObjectWrap::Unwrap<NODE_Surface2D>(info.This());
    obj->surface->uploadTexture();
    return;
}

NAN_METHOD(NODE_Surface2D::NODE_unbindTexture) {
    NODE_Surface2D* obj = ObjectWrap::Unwrap<NODE_Surface2D>(info.This());
    obj->surface->unbindTexture(info[0]->IntegerValue());
    return;
}

NAN_METHOD(NODE_Surface2D::NODE_save) {
    Nan::Utf8String str(info[0]);
    NODE_Surface2D* obj = ObjectWrap::Unwrap<NODE_Surface2D>(info.This());
    allofw::ByteStream* stream = allofw::ByteStream::OpenFile(*str, "w");
    obj->surface->save(stream);
    allofw::ByteStream::Destroy(stream);
    return;
}

void NODE_VideoSurface2D::Init(Handle<Object> exports) {
    // Prepare constructor template
    Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
    tpl->SetClassName(Nan::New<String>("VideoSurface2D").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    Nan::SetPrototypeMethod(tpl, "width", NODE_width);
    Nan::SetPrototypeMethod(tpl, "height", NODE_height);
    Nan::SetPrototypeMethod(tpl, "fps", NODE_fps);
    Nan::SetPrototypeMethod(tpl, "duration", NODE_duration);
    Nan::SetPrototypeMethod(tpl, "pixels", NODE_pixels);
    Nan::SetPrototypeMethod(tpl, "nextFrame", NODE_nextFrame);
    Nan::SetPrototypeMethod(tpl, "seek", NODE_seek);

    constructor.Reset(tpl->GetFunction());

    // Export constructor.
    Nan::Set(exports, Nan::New<String>("VideoSurface2D").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

NODE_VideoSurface2D::NODE_VideoSurface2D(const char* filename) {
    video = backend->createVideoSurface2DFromFile(filename);
}

NODE_VideoSurface2D::~NODE_VideoSurface2D() {
    backend->destroyVideoSurface2D(video);
}

NAN_METHOD(NODE_VideoSurface2D::New) {
    Nan::HandleScope scope;

    if(info.IsConstructCall()) {
        String::Utf8Value str(info[0]);
        try {
            NODE_VideoSurface2D* obj = new NODE_VideoSurface2D(std::string(*str, *str + str.length()).c_str());
            obj->Wrap(info.This());
            info.GetReturnValue().Set(info.This());
        } catch(const allofw::exception& e) {
            Nan::ThrowError(e.what());
        } catch(...) {
            Nan::ThrowError("unknown exception");
        }
    } else {
        // Invoked as plain function `MyObject(...)`, turn into construct call.
        const int argc = 2;
        Local<Value> argv[argc] = { info[0], info[1] };
        info.GetReturnValue().Set(Nan::New(constructor)->NewInstance(argc, argv));
    }
}

NAN_METHOD(NODE_VideoSurface2D::NODE_width) {
    NODE_VideoSurface2D* obj = ObjectWrap::Unwrap<NODE_VideoSurface2D>(info.This());
    info.GetReturnValue().Set(Nan::New<Uint32>(obj->video->width()));
}

NAN_METHOD(NODE_VideoSurface2D::NODE_height) {
    NODE_VideoSurface2D* obj = ObjectWrap::Unwrap<NODE_VideoSurface2D>(info.This());
    info.GetReturnValue().Set(Nan::New<Uint32>(obj->video->height()));
}

NAN_METHOD(NODE_VideoSurface2D::NODE_fps) {
    NODE_VideoSurface2D* obj = ObjectWrap::Unwrap<NODE_VideoSurface2D>(info.This());
    info.GetReturnValue().Set(Nan::New<Number>(obj->video->fps()));
}

NAN_METHOD(NODE_VideoSurface2D::NODE_duration) {
    NODE_VideoSurface2D* obj = ObjectWrap::Unwrap<NODE_VideoSurface2D>(info.This());
    info.GetReturnValue().Set(Nan::New<Number>(obj->video->duration()));
}

NAN_METHOD(NODE_VideoSurface2D::NODE_pixels) {
    NODE_VideoSurface2D* obj = ObjectWrap::Unwrap<NODE_VideoSurface2D>(info.This());
    info.GetReturnValue().Set(Nan::NewBuffer((char*)obj->video->pixels(), obj->video->width() * obj->video->height() * 4, do_nothing_callback2, NULL).ToLocalChecked());
}

NAN_METHOD(NODE_VideoSurface2D::NODE_nextFrame) {
    NODE_VideoSurface2D* obj = ObjectWrap::Unwrap<NODE_VideoSurface2D>(info.This());
    info.GetReturnValue().Set(Nan::New<Boolean>(obj->video->nextFrame()));
}

NAN_METHOD(NODE_VideoSurface2D::NODE_seek) {
    NODE_VideoSurface2D* obj = ObjectWrap::Unwrap<NODE_VideoSurface2D>(info.This());
    obj->video->seek(info[0]->NumberValue());
}

void NODE_GraphicalContext2D::Init(Handle<Object> exports) {
    // Prepare constructor template
    Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
    tpl->SetClassName(Nan::New<String>("GraphicalContext2D").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    Nan::SetPrototypeMethod(tpl, "path", NODE_path);
    Nan::SetPrototypeMethod(tpl, "paint", NODE_paint);
    Nan::SetPrototypeMethod(tpl, "drawPath", NODE_drawPath);
    Nan::SetPrototypeMethod(tpl, "drawText", NODE_drawText);
    Nan::SetPrototypeMethod(tpl, "drawLine", NODE_drawLine);
    Nan::SetPrototypeMethod(tpl, "drawCircle", NODE_drawCircle);
    Nan::SetPrototypeMethod(tpl, "drawRectangle", NODE_drawRectangle);
    Nan::SetPrototypeMethod(tpl, "drawSurface", NODE_drawSurface);
    Nan::SetPrototypeMethod(tpl, "rotate", NODE_rotate);
    Nan::SetPrototypeMethod(tpl, "translate", NODE_translate);
    Nan::SetPrototypeMethod(tpl, "scale", NODE_scale);
    Nan::SetPrototypeMethod(tpl, "concatTransform", NODE_concatTransform);
    Nan::SetPrototypeMethod(tpl, "setTransform", NODE_setTransform);
    Nan::SetPrototypeMethod(tpl, "getTransform", NODE_getTransform);
    Nan::SetPrototypeMethod(tpl, "clear", NODE_clear);
    Nan::SetPrototypeMethod(tpl, "reset", NODE_reset);
    // Nan::SetPrototypeMethod(tpl, "save", NODE_save);
    // Nan::SetPrototypeMethod(tpl, "load", NODE_load);
    Nan::SetPrototypeMethod(tpl, "save", NODE_save);
    Nan::SetPrototypeMethod(tpl, "restore", NODE_restore);
    Nan::SetPrototypeMethod(tpl, "flush", NODE_flush);

    constructor.Reset(tpl->GetFunction());

    // Export constructor.
    Nan::Set(exports, Nan::New<String>("GraphicalContext2D").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

NODE_GraphicalContext2D::NODE_GraphicalContext2D(NODE_Surface2D* surface) {
    context = backend->createGraphicalContext2D(surface->surface);
}

NODE_GraphicalContext2D::~NODE_GraphicalContext2D() {
    backend->destroyGraphicalContext2D(context);
}

NAN_METHOD(NODE_GraphicalContext2D::New) {
    Nan::HandleScope scope;

    if(info.IsConstructCall()) {
        // Invoked as constructor: `new MyObject(...)`
        NODE_Surface2D* surface = ObjectWrap::Unwrap<NODE_Surface2D>(info[0]->ToObject());
        NODE_GraphicalContext2D* obj = new NODE_GraphicalContext2D(surface);
        obj->Wrap(info.This());
        info.GetReturnValue().Set(info.This());
    } else {
        // Invoked as plain function `MyObject(...)`, turn into construct call.
        const int argc = 1;
        Local<Value> argv[argc] = { info[0] };
        info.GetReturnValue().Set(Nan::New(constructor)->NewInstance(argc, argv));
    }
}

NAN_METHOD(NODE_GraphicalContext2D::NODE_path) {
    const int argc = 1;
    Local<Value> argv[argc] = { info.This() };
    info.GetReturnValue().Set(Nan::New(NODE_Path2D::constructor)->NewInstance(argc, argv));
}

NAN_METHOD(NODE_GraphicalContext2D::NODE_paint) {
    const int argc = 1;
    Local<Value> argv[argc] = { info.This() };
    info.GetReturnValue().Set(Nan::New(NODE_Paint2D::constructor)->NewInstance(argc, argv));
}

NAN_METHOD(NODE_GraphicalContext2D::NODE_drawPath) {
    Path2D* path = ObjectWrap::Unwrap<NODE_Path2D>(info[0]->ToObject())->path;
    Paint2D* paint = ObjectWrap::Unwrap<NODE_Paint2D>(info[1]->ToObject())->paint;
    NODE_GraphicalContext2D* self = ObjectWrap::Unwrap<NODE_GraphicalContext2D>(info.This());
    self->context->drawPath(path, paint);
}

NAN_METHOD(NODE_GraphicalContext2D::NODE_drawText) {
    Nan::Utf8String str(info[0]);
    double x = info[1]->NumberValue();
    double y = info[2]->NumberValue();
    Paint2D* paint = ObjectWrap::Unwrap<NODE_Paint2D>(info[3]->ToObject())->paint;
    NODE_GraphicalContext2D* self = ObjectWrap::Unwrap<NODE_GraphicalContext2D>(info.This());
    self->context->drawText(*str, x, y, paint);
}

NAN_METHOD(NODE_GraphicalContext2D::NODE_drawLine) {
    double x1 = info[0]->NumberValue();
    double y1 = info[1]->NumberValue();
    double x2 = info[2]->NumberValue();
    double y2 = info[3]->NumberValue();
    Paint2D* paint = ObjectWrap::Unwrap<NODE_Paint2D>(info[4]->ToObject())->paint;
    NODE_GraphicalContext2D* self = ObjectWrap::Unwrap<NODE_GraphicalContext2D>(info.This());
    self->context->drawLine(x1, y1, x2, y2, paint);
}

NAN_METHOD(NODE_GraphicalContext2D::NODE_drawCircle) {
    double x = info[0]->NumberValue();
    double y = info[1]->NumberValue();
    double radius = info[2]->NumberValue();
    Paint2D* paint = ObjectWrap::Unwrap<NODE_Paint2D>(info[3]->ToObject())->paint;
    NODE_GraphicalContext2D* self = ObjectWrap::Unwrap<NODE_GraphicalContext2D>(info.This());
    self->context->drawCircle(x, y, radius, paint);
}

NAN_METHOD(NODE_GraphicalContext2D::NODE_drawRectangle) {
    double x = info[0]->NumberValue();
    double y = info[1]->NumberValue();
    double w = info[2]->NumberValue();
    double h = info[3]->NumberValue();
    Paint2D* paint = ObjectWrap::Unwrap<NODE_Paint2D>(info[4]->ToObject())->paint;
    NODE_GraphicalContext2D* self = ObjectWrap::Unwrap<NODE_GraphicalContext2D>(info.This());
    self->context->drawRectangle(allofw::Rectangle2d(x, y, w, h), paint);
}

NAN_METHOD(NODE_GraphicalContext2D::NODE_drawSurface) {
    NODE_GraphicalContext2D* self = ObjectWrap::Unwrap<NODE_GraphicalContext2D>(info.This());
    if(info.Length() == 4) { // surface, x, y, paint
        Surface2D* surface = ObjectWrap::Unwrap<NODE_Surface2D>(info[0]->ToObject())->surface;
        double x = info[1]->NumberValue();
        double y = info[2]->NumberValue();
        Paint2D* paint = ObjectWrap::Unwrap<NODE_Paint2D>(info[3]->ToObject())->paint;
        self->context->drawSurface(surface, x, y, paint);
    }
    if(info.Length() == 10) { // surface, x, y, paint
        Surface2D* surface = ObjectWrap::Unwrap<NODE_Surface2D>(info[0]->ToObject())->surface;
        double sx = info[1]->NumberValue();
        double sy = info[2]->NumberValue();
        double sw = info[3]->NumberValue();
        double sh = info[4]->NumberValue();
        double dx = info[5]->NumberValue();
        double dy = info[6]->NumberValue();
        double dw = info[7]->NumberValue();
        double dh = info[8]->NumberValue();
        Paint2D* paint = ObjectWrap::Unwrap<NODE_Paint2D>(info[9]->ToObject())->paint;
        self->context->drawSurface(surface, allofw::Rectangle2d(sx, sy, sw, sh), allofw::Rectangle2d(dx, dy, dw, dh), paint);
    }
}

NAN_METHOD(NODE_GraphicalContext2D::NODE_rotate) {
    NODE_GraphicalContext2D* self = ObjectWrap::Unwrap<NODE_GraphicalContext2D>(info.This());
    self->context->rotate(info[0]->NumberValue());
}

NAN_METHOD(NODE_GraphicalContext2D::NODE_translate) {
    NODE_GraphicalContext2D* self = ObjectWrap::Unwrap<NODE_GraphicalContext2D>(info.This());
    self->context->translate(info[0]->NumberValue(), info[1]->NumberValue());
}

NAN_METHOD(NODE_GraphicalContext2D::NODE_scale) {
    NODE_GraphicalContext2D* self = ObjectWrap::Unwrap<NODE_GraphicalContext2D>(info.This());
    self->context->scale(info[0]->NumberValue(), info[1]->NumberValue());
}

NAN_METHOD(NODE_GraphicalContext2D::NODE_concatTransform) {
    allofw::Matrix3 m;
    double a = info[0]->NumberValue();
    double b = info[1]->NumberValue();
    double c = info[2]->NumberValue();
    double d = info[3]->NumberValue();
    double e = info[4]->NumberValue();
    double f = info[5]->NumberValue();
    m.a11 = a; m.a12 = c; m.a13 = e;
    m.a21 = b; m.a22 = d; m.a23 = f;
    m.a31 = 0; m.a32 = 0; m.a33 = 1;
    NODE_GraphicalContext2D* self = ObjectWrap::Unwrap<NODE_GraphicalContext2D>(info.This());
    self->context->concatTransform(m);
}

NAN_METHOD(NODE_GraphicalContext2D::NODE_setTransform) {
    allofw::Matrix3 m;
    double a = info[0]->NumberValue();
    double b = info[1]->NumberValue();
    double c = info[2]->NumberValue();
    double d = info[3]->NumberValue();
    double e = info[4]->NumberValue();
    double f = info[5]->NumberValue();
    m.a11 = a; m.a12 = c; m.a13 = e;
    m.a21 = b; m.a22 = d; m.a23 = f;
    m.a31 = 0; m.a32 = 0; m.a33 = 1;
    NODE_GraphicalContext2D* self = ObjectWrap::Unwrap<NODE_GraphicalContext2D>(info.This());
    self->context->setTransform(m);
}

NAN_METHOD(NODE_GraphicalContext2D::NODE_getTransform) {
    Nan::HandleScope scope;
    NODE_GraphicalContext2D* self = ObjectWrap::Unwrap<NODE_GraphicalContext2D>(info.This());
    allofw::Matrix3 m = self->context->getTransform();
    Handle<Array> array = Nan::New<Array>(6);
    array->Set(0, Nan::New<Number>(m.a11));
    array->Set(1, Nan::New<Number>(m.a21));
    array->Set(2, Nan::New<Number>(m.a12));
    array->Set(3, Nan::New<Number>(m.a22));
    array->Set(4, Nan::New<Number>(m.a13));
    array->Set(5, Nan::New<Number>(m.a23));
    info.GetReturnValue().Set(array);
}

NAN_METHOD(NODE_GraphicalContext2D::NODE_clear) {
    double r = info[0]->NumberValue() / 255.0;
    double g = info[1]->NumberValue() / 255.0;
    double b = info[2]->NumberValue() / 255.0;
    double a = info.Length() < 4 ? 1 : info[3]->NumberValue();
    NODE_GraphicalContext2D* self = ObjectWrap::Unwrap<NODE_GraphicalContext2D>(info.This());
    self->context->clear(allofw::Color(r, g, b, a));
}

NAN_METHOD(NODE_GraphicalContext2D::NODE_reset) {
    NODE_GraphicalContext2D* self = ObjectWrap::Unwrap<NODE_GraphicalContext2D>(info.This());
    self->context->reset();
}

// NAN_METHOD(NODE_GraphicalContext2D::NODE_save) {
// }

// NAN_METHOD(NODE_GraphicalContext2D::NODE_load) {
// }

NAN_METHOD(NODE_GraphicalContext2D::NODE_save) {
    NODE_GraphicalContext2D* self = ObjectWrap::Unwrap<NODE_GraphicalContext2D>(info.This());
    self->context->save();
}

NAN_METHOD(NODE_GraphicalContext2D::NODE_restore) {
    NODE_GraphicalContext2D* self = ObjectWrap::Unwrap<NODE_GraphicalContext2D>(info.This());
    self->context->restore();
}

NAN_METHOD(NODE_GraphicalContext2D::NODE_flush) {
    NODE_GraphicalContext2D* self = ObjectWrap::Unwrap<NODE_GraphicalContext2D>(info.This());
    self->context->flush();
}

void NODE_Path2D::Init(Handle<Object> exports) {
    // Prepare constructor template
    Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
    tpl->SetClassName(Nan::New<String>("Path2D").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    // Prototype
    Nan::SetPrototypeMethod(tpl, "moveTo", NODE_moveTo);
    Nan::SetPrototypeMethod(tpl, "lineTo", NODE_lineTo);
    Nan::SetPrototypeMethod(tpl, "bezierCurveTo", NODE_bezierCurveTo);
    Nan::SetPrototypeMethod(tpl, "circle", NODE_circle);
    Nan::SetPrototypeMethod(tpl, "arc", NODE_arc);
    Nan::SetPrototypeMethod(tpl, "close", NODE_close);

    constructor.Reset(tpl->GetFunction());

    // Export constructor.
    Nan::Set(exports, Nan::New<String>("Path2D").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

NODE_Path2D::NODE_Path2D(NODE_GraphicalContext2D* node_context) {
    context = node_context;
    context->Ref();
    path = node_context->context->path();
}

NODE_Path2D::~NODE_Path2D() {
    context->context->destroyPath(path);
    context->Unref();
}

NAN_METHOD(NODE_Path2D::New) {
    Nan::HandleScope scope;

    if(info.IsConstructCall()) {
        // Invoked as constructor: `new MyObject(...)`
        NODE_GraphicalContext2D* context = ObjectWrap::Unwrap<NODE_GraphicalContext2D>(info[0]->ToObject());
        NODE_Path2D* obj = new NODE_Path2D(context);
        obj->Wrap(info.This());
        info.GetReturnValue().Set(info.This());
    } else {
        // Invoked as plain function `MyObject(...)`, turn into construct call.
        const int argc = 1;
        Local<Value> argv[argc] = { info[0] };
        info.GetReturnValue().Set(Nan::New(constructor)->NewInstance(argc, argv));
    }
}

NAN_METHOD(NODE_Path2D::NODE_moveTo) {
    NODE_Path2D* self = ObjectWrap::Unwrap<NODE_Path2D>(info.This());
    self->path->moveTo(info[0]->NumberValue(), info[1]->NumberValue());
    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(NODE_Path2D::NODE_lineTo) {
    NODE_Path2D* self = ObjectWrap::Unwrap<NODE_Path2D>(info.This());
    self->path->lineTo(info[0]->NumberValue(), info[1]->NumberValue());
    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(NODE_Path2D::NODE_bezierCurveTo) {
    NODE_Path2D* self = ObjectWrap::Unwrap<NODE_Path2D>(info.This());
    self->path->bezierCurveTo(info[0]->NumberValue(), info[1]->NumberValue(), // c1
                              info[2]->NumberValue(), info[3]->NumberValue(), // c2
                              info[4]->NumberValue(), info[5]->NumberValue()); // p
    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(NODE_Path2D::NODE_circle) {
    NODE_Path2D* self = ObjectWrap::Unwrap<NODE_Path2D>(info.This());
    self->path->circle(info[0]->NumberValue(), info[1]->NumberValue(), // center
                       info[2]->NumberValue()); // normal
    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(NODE_Path2D::NODE_arc) {
    NODE_Path2D* self = ObjectWrap::Unwrap<NODE_Path2D>(info.This());
    self->path->arc(info[0]->NumberValue(), info[1]->NumberValue(), // center
                    info[2]->NumberValue(), // radius
                    info[3]->NumberValue(), info[4]->NumberValue()); // angle1, angle2
    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(NODE_Path2D::NODE_close) {
    NODE_Path2D* self = ObjectWrap::Unwrap<NODE_Path2D>(info.This());
    self->path->close();
    info.GetReturnValue().Set(info.This());
}

void NODE_Paint2D::Init(Handle<Object> exports) {
    // Prepare constructor template
    Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
    tpl->SetClassName(Nan::New<String>("Paint2D").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    Nan::SetPrototypeMethod(tpl, "setMode", NODE_setMode);
    Nan::SetPrototypeMethod(tpl, "setColor", NODE_setColor);
    Nan::SetPrototypeMethod(tpl, "setStrokeWidth", NODE_setStrokeWidth);
    Nan::SetPrototypeMethod(tpl, "setLineCap", NODE_setLineCap);
    Nan::SetPrototypeMethod(tpl, "setLineJoin", NODE_setLineJoin);
    Nan::SetPrototypeMethod(tpl, "setTextSize", NODE_setTextSize);
    Nan::SetPrototypeMethod(tpl, "setTextAlign", NODE_setTextAlign);
    Nan::SetPrototypeMethod(tpl, "setTypeface", NODE_setTypeface);
    Nan::SetPrototypeMethod(tpl, "measureText", NODE_measureText);
    Nan::SetPrototypeMethod(tpl, "setColorMatrix", NODE_setColorMatrix);
    Nan::SetPrototypeMethod(tpl, "setColorMatrixScale", NODE_setColorMatrixScale);
    Nan::SetPrototypeMethod(tpl, "setColorMatrixScaleAlpha", NODE_setColorMatrixScaleAlpha);
    Nan::SetPrototypeMethod(tpl, "setTransferMode", NODE_setTransferMode);
    Nan::SetPrototypeMethod(tpl, "clone", NODE_clone);

    constructor.Reset(tpl->GetFunction());

    // Export constructor.
    Nan::Set(exports, Nan::New<String>("Paint2D").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

NODE_Paint2D::NODE_Paint2D(NODE_GraphicalContext2D* node_context) {
    context = node_context;
    context->Ref();
    paint = context->context->paint();
}
NODE_Paint2D::NODE_Paint2D(NODE_Paint2D* paint_) {
    context = paint_->context;
    context->Ref();
    paint = paint_->paint->clone();
}

NODE_Paint2D::~NODE_Paint2D() {
    context->context->destroyPaint(paint);
    context->Unref();
}

NAN_METHOD(NODE_Paint2D::New) {
    Nan::HandleScope scope;
    if(info.IsConstructCall()) {
        // Invoked as constructor: `new MyObject(...)`
        if(info.Length() == 1) {
            NODE_GraphicalContext2D* context = ObjectWrap::Unwrap<NODE_GraphicalContext2D>(info[0]->ToObject());
            NODE_Paint2D* obj = new NODE_Paint2D(context);
            obj->Wrap(info.This());
            info.GetReturnValue().Set(info.This());
        } else {
            NODE_Paint2D* paint = ObjectWrap::Unwrap<NODE_Paint2D>(info[0]->ToObject());
            NODE_Paint2D* obj = new NODE_Paint2D(paint);
            obj->Wrap(info.This());
            info.GetReturnValue().Set(info.This());
        }
    } else {
        // Invoked as plain function `MyObject(...)`, turn into construct call.
        const int argc = 1;
        Local<Value> argv[argc] = { info[0] };
        info.GetReturnValue().Set(Nan::New(constructor)->NewInstance(argc, argv));
    }
}

NAN_METHOD(NODE_Paint2D::NODE_setMode) {
    NODE_Paint2D* self = ObjectWrap::Unwrap<NODE_Paint2D>(info.This());
    self->paint->setMode((PaintMode)info[0]->IntegerValue());
    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(NODE_Paint2D::NODE_setColor) {
    double r = info[0]->NumberValue() / 255.0;
    double g = info[1]->NumberValue() / 255.0;
    double b = info[2]->NumberValue() / 255.0;
    double a = info.Length() < 4 ? 1 : info[3]->NumberValue();
    NODE_Paint2D* self = ObjectWrap::Unwrap<NODE_Paint2D>(info.This());
    self->paint->setColor(allofw::Color(r, g, b, a));
    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(NODE_Paint2D::NODE_setStrokeWidth) {
    NODE_Paint2D* self = ObjectWrap::Unwrap<NODE_Paint2D>(info.This());
    self->paint->setStrokeWidth(info[0]->NumberValue());
    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(NODE_Paint2D::NODE_setLineCap) {
    NODE_Paint2D* self = ObjectWrap::Unwrap<NODE_Paint2D>(info.This());
    self->paint->setLineCap((LineCap)info[0]->IntegerValue());
    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(NODE_Paint2D::NODE_setLineJoin) {
    NODE_Paint2D* self = ObjectWrap::Unwrap<NODE_Paint2D>(info.This());
    self->paint->setLineJoin((LineJoin)info[0]->IntegerValue());
    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(NODE_Paint2D::NODE_setTextSize) {
    NODE_Paint2D* self = ObjectWrap::Unwrap<NODE_Paint2D>(info.This());
    self->paint->setTextSize(info[0]->NumberValue());
    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(NODE_Paint2D::NODE_setTextAlign) {
    NODE_Paint2D* self = ObjectWrap::Unwrap<NODE_Paint2D>(info.This());
    self->paint->setTextAlign((TextAlign)info[0]->IntegerValue());
    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(NODE_Paint2D::NODE_setTypeface) {
    Nan::Utf8String str(info[0]);
    NODE_Paint2D* self = ObjectWrap::Unwrap<NODE_Paint2D>(info.This());
    FontStyle style = FontStyle::NORMAL;
    if(info.Length() >= 2) {
        style = (FontStyle)info[1]->IntegerValue();
    }
    self->paint->setTypeface(*str, style);
    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(NODE_Paint2D::NODE_measureText) {
    Nan::HandleScope scope;
    Nan::Utf8String str(info[0]);
    NODE_Paint2D* self = ObjectWrap::Unwrap<NODE_Paint2D>(info.This());
    double width = self->paint->measureText(*str);
    info.GetReturnValue().Set(Nan::New<Number>(width));
}

NAN_METHOD(NODE_Paint2D::NODE_setColorMatrix) {
    double m[20];
    for(int i = 0; i < 20; i++) m[i] = info[i]->NumberValue();
    NODE_Paint2D* self = ObjectWrap::Unwrap<NODE_Paint2D>(info.This());
    self->paint->setColorMatrix(m);
    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(NODE_Paint2D::NODE_setColorMatrixScale) {
    double r = info[0]->NumberValue();
    double g = info[1]->NumberValue();
    double b = info[2]->NumberValue();
    double a = info[3]->NumberValue();
    NODE_Paint2D* self = ObjectWrap::Unwrap<NODE_Paint2D>(info.This());
    self->paint->setColorMatrixScale(r, g, b, a);
    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(NODE_Paint2D::NODE_setColorMatrixScaleAlpha) {
    double a = info[0]->NumberValue();
    NODE_Paint2D* self = ObjectWrap::Unwrap<NODE_Paint2D>(info.This());
    self->paint->setColorMatrixScaleAlpha(a);
    info.GetReturnValue().Set(info.This());
}

NAN_METHOD(NODE_Paint2D::NODE_setTransferMode) {
    NODE_Paint2D* self = ObjectWrap::Unwrap<NODE_Paint2D>(info.This());
    self->paint->setTransferMode((TransferMode)info[0]->IntegerValue());
    info.GetReturnValue().Set(info.This());
}


NAN_METHOD(NODE_Paint2D::NODE_clone) {
    Nan::HandleScope scope;
    const int argc = 2;
    Local<Value> argv[argc] = { info.This(), info.This() };
    info.GetReturnValue().Set(Nan::New(NODE_Paint2D::constructor)->NewInstance(argc, argv));
}

Nan::Persistent<Function> NODE_Surface2D::constructor;
Nan::Persistent<Function> NODE_VideoSurface2D::constructor;
Nan::Persistent<Function> NODE_GraphicalContext2D::constructor;
Nan::Persistent<Function> NODE_Path2D::constructor;
Nan::Persistent<Function> NODE_Paint2D::constructor;

NAN_METHOD(NODE_loadImageData) {
    Nan::HandleScope scope;
    const int argc = 1;
    Local<Value> argv[argc] = { info[0] };
    info.GetReturnValue().Set(Nan::New(NODE_Surface2D::constructor)->NewInstance(argc, argv));
}

void NODE_Graphics_init(Handle<Object> exports) {
    backend = GraphicalBackend::CreateSkia();

    // Classes.
    NODE_Surface2D::Init(exports);
    NODE_VideoSurface2D::Init(exports);
    NODE_GraphicalContext2D::Init(exports);
    NODE_Path2D::Init(exports);
    NODE_Paint2D::Init(exports);

    Nan::Set(exports, Nan::New<String>("loadImageData").ToLocalChecked(), Nan::GetFunction(Nan::New<FunctionTemplate>(NODE_loadImageData)).ToLocalChecked());

    // Constant values.
    Nan::Set(exports, Nan::New<String>("SURFACETYPE_PDF").ToLocalChecked(), Nan::New<Uint32>((int32_t)NODE_Surface2D::SURFACETYPE_PDF));
    Nan::Set(exports, Nan::New<String>("SURFACETYPE_RASTER").ToLocalChecked(), Nan::New<Uint32>((int32_t)NODE_Surface2D::SURFACETYPE_RASTER));

    Nan::Set(exports, Nan::New<String>("LINECAP_BUTT").ToLocalChecked(), Nan::New<Uint32>((int32_t)LineCap::BUTT));
    Nan::Set(exports, Nan::New<String>("LINECAP_ROUND").ToLocalChecked(), Nan::New<Uint32>((int32_t)LineCap::ROUND));
    Nan::Set(exports, Nan::New<String>("LINECAP_SQUARE").ToLocalChecked(), Nan::New<Uint32>((int32_t)LineCap::SQUARE));

    Nan::Set(exports, Nan::New<String>("LINEJOIN_BEVEL").ToLocalChecked(), Nan::New<Uint32>((int32_t)LineJoin::BEVEL));
    Nan::Set(exports, Nan::New<String>("LINEJOIN_MITER").ToLocalChecked(), Nan::New<Uint32>((int32_t)LineJoin::MITER));
    Nan::Set(exports, Nan::New<String>("LINEJOIN_ROUND").ToLocalChecked(), Nan::New<Uint32>((int32_t)LineJoin::ROUND));

    Nan::Set(exports, Nan::New<String>("FONTSTYLE_NORMAL").ToLocalChecked(), Nan::New<Uint32>((int32_t)FontStyle::NORMAL));
    Nan::Set(exports, Nan::New<String>("FONTSTYLE_BOLD").ToLocalChecked(), Nan::New<Uint32>((int32_t)FontStyle::BOLD));
    Nan::Set(exports, Nan::New<String>("FONTSTYLE_ITALIC").ToLocalChecked(), Nan::New<Uint32>((int32_t)FontStyle::ITALIC));
    Nan::Set(exports, Nan::New<String>("FONTSTYLE_BOLDITALIC").ToLocalChecked(), Nan::New<Uint32>((int32_t)FontStyle::BOLDITALIC));

    Nan::Set(exports, Nan::New<String>("TEXTALIGN_LEFT").ToLocalChecked(), Nan::New<Uint32>((int32_t)TextAlign::LEFT));
    Nan::Set(exports, Nan::New<String>("TEXTALIGN_CENTER").ToLocalChecked(), Nan::New<Uint32>((int32_t)TextAlign::CENTER));
    Nan::Set(exports, Nan::New<String>("TEXTALIGN_RIGHT").ToLocalChecked(), Nan::New<Uint32>((int32_t)TextAlign::RIGHT));

    Nan::Set(exports, Nan::New<String>("PAINTMODE_STROKE").ToLocalChecked(), Nan::New<Uint32>((int32_t)PaintMode::STROKE));
    Nan::Set(exports, Nan::New<String>("PAINTMODE_FILL").ToLocalChecked(), Nan::New<Uint32>((int32_t)PaintMode::FILL));
    Nan::Set(exports, Nan::New<String>("PAINTMODE_STROKEFILL").ToLocalChecked(), Nan::New<Uint32>((int32_t)PaintMode::STROKEFILL));

    Nan::Set(exports, Nan::New<String>("TRANSFERMODE_SRC_OVER").ToLocalChecked(), Nan::New<Uint32>((int32_t)TransferMode::SRC_OVER));
}
