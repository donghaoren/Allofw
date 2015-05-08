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
    NanScope();
    // Prepare constructor template
    Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
    tpl->SetClassName(NanNew<String>("Surface2D"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    NODE_SET_PROTOTYPE_METHOD(tpl, "width", NODE_width);
    NODE_SET_PROTOTYPE_METHOD(tpl, "height", NODE_height);
    NODE_SET_PROTOTYPE_METHOD(tpl, "pixels", NODE_pixels);
    NODE_SET_PROTOTYPE_METHOD(tpl, "bindTexture", NODE_bindTexture);
    NODE_SET_PROTOTYPE_METHOD(tpl, "uploadTexture", NODE_uploadTexture);
    NODE_SET_PROTOTYPE_METHOD(tpl, "unbindTexture", NODE_unbindTexture);
    NODE_SET_PROTOTYPE_METHOD(tpl, "save", NODE_save);

    NanAssignPersistent(constructor, tpl->GetFunction());

    // Export constructor.
    exports->Set(NanNew<String>("Surface2D"), tpl->GetFunction());
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
    delete surface;
}

NAN_METHOD(NODE_Surface2D::New) {
    NanScope();

    if(args.IsConstructCall()) {
        try {
            // Invoked as constructor: `new MyObject(...)`
            if(node::Buffer::HasInstance(args[0])) {
                // Constructing with a buffer object.
                NODE_Surface2D* obj = new NODE_Surface2D(node::Buffer::Data(args[0]), node::Buffer::Length(args[0]));
                obj->Wrap(args.This());
                NanReturnValue(args.This());
            } else {
                int width = args[0]->IsUndefined() ? 0 : args[0]->IntegerValue();
                int height = args[1]->IsUndefined() ? 0 : args[1]->IntegerValue();
                if(node::Buffer::HasInstance(args[2])) {
                    NODE_Surface2D* obj = new NODE_Surface2D(width, height, node::Buffer::Data(args[2]));
                    obj->Wrap(args.This());
                    NanReturnValue(args.This());
                } else {
                    int type = args[2]->IsUndefined() ? SURFACETYPE_RASTER : args[2]->IntegerValue();
                    NODE_Surface2D* obj = new NODE_Surface2D(width, height, type);
                    obj->Wrap(args.This());
                    NanReturnValue(args.This());
                }
            }
        } catch(const allofw::exception& e) {
            NanThrowError(e.what());
        } catch(...) {
            NanThrowError("unknown exception");
        }
    } else {
        // Invoked as plain function `MyObject(...)`, turn into construct call.
        const int argc = 2;
        Local<Value> argv[argc] = { args[0], args[1] };
        NanReturnValue(NanNew(constructor)->NewInstance(argc, argv));
    }
}

NAN_METHOD(NODE_Surface2D::NODE_width) {
    NODE_Surface2D* obj = ObjectWrap::Unwrap<NODE_Surface2D>(args.This());
    NanReturnValue(NanNew<Uint32>(obj->surface->width()));
}

NAN_METHOD(NODE_Surface2D::NODE_height) {
    NODE_Surface2D* obj = ObjectWrap::Unwrap<NODE_Surface2D>(args.This());
    NanReturnValue(NanNew<Uint32>(obj->surface->height()));
}

void do_nothing_callback2(char*, void*) { }

NAN_METHOD(NODE_Surface2D::NODE_pixels) {
    NODE_Surface2D* obj = ObjectWrap::Unwrap<NODE_Surface2D>(args.This());
    NanReturnValue(NanNewBufferHandle((char*)obj->surface->pixels(), obj->surface->width() * obj->surface->height() * 4, do_nothing_callback2, NULL));
}

NAN_METHOD(NODE_Surface2D::NODE_bindTexture) {
    NODE_Surface2D* obj = ObjectWrap::Unwrap<NODE_Surface2D>(args.This());
    obj->surface->bindTexture(args[0]->IntegerValue());
    NanReturnUndefined();
}

NAN_METHOD(NODE_Surface2D::NODE_uploadTexture) {
    NODE_Surface2D* obj = ObjectWrap::Unwrap<NODE_Surface2D>(args.This());
    obj->surface->uploadTexture();
    NanReturnUndefined();
}

NAN_METHOD(NODE_Surface2D::NODE_unbindTexture) {
    NODE_Surface2D* obj = ObjectWrap::Unwrap<NODE_Surface2D>(args.This());
    obj->surface->unbindTexture(args[0]->IntegerValue());
    NanReturnUndefined();
}

NAN_METHOD(NODE_Surface2D::NODE_save) {
    NanUtf8String str(args[0]);
    NODE_Surface2D* obj = ObjectWrap::Unwrap<NODE_Surface2D>(args.This());
    allofw::ByteStream* stream = allofw::ByteStream::OpenFile(*str, "w");
    obj->surface->save(stream);
    delete stream;
    NanReturnUndefined();
}

void NODE_VideoSurface2D::Init(Handle<Object> exports) {
    // Prepare constructor template
    Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
    tpl->SetClassName(NanNew<String>("VideoSurface2D"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    NODE_SET_PROTOTYPE_METHOD(tpl, "width", NODE_width);
    NODE_SET_PROTOTYPE_METHOD(tpl, "height", NODE_height);
    NODE_SET_PROTOTYPE_METHOD(tpl, "fps", NODE_fps);
    NODE_SET_PROTOTYPE_METHOD(tpl, "duration", NODE_duration);
    NODE_SET_PROTOTYPE_METHOD(tpl, "pixels", NODE_pixels);
    NODE_SET_PROTOTYPE_METHOD(tpl, "nextFrame", NODE_nextFrame);
    NODE_SET_PROTOTYPE_METHOD(tpl, "seek", NODE_seek);

    NanAssignPersistent(constructor, tpl->GetFunction());

    // Export constructor.
    exports->Set(NanNew<String>("VideoSurface2D"), tpl->GetFunction());
}

NODE_VideoSurface2D::NODE_VideoSurface2D(const char* filename) {
    video = VideoSurface2D::FromFile(filename);
}

NODE_VideoSurface2D::~NODE_VideoSurface2D() {
    delete video;
}

NAN_METHOD(NODE_VideoSurface2D::New) {
    NanScope();

    if(args.IsConstructCall()) {
        String::Utf8Value str(args[0]);
        try {
            NODE_VideoSurface2D* obj = new NODE_VideoSurface2D(std::string(*str, *str + str.length()).c_str());
            obj->Wrap(args.This());
            NanReturnValue(args.This());
        } catch(const allofw::exception& e) {
            NanThrowError(e.what());
        } catch(...) {
            NanThrowError("unknown exception");
        }
    } else {
        // Invoked as plain function `MyObject(...)`, turn into construct call.
        const int argc = 2;
        Local<Value> argv[argc] = { args[0], args[1] };
        NanReturnValue(NanNew(constructor)->NewInstance(argc, argv));
    }
}

NAN_METHOD(NODE_VideoSurface2D::NODE_width) {
    NODE_VideoSurface2D* obj = ObjectWrap::Unwrap<NODE_VideoSurface2D>(args.This());
    NanReturnValue(NanNew<Uint32>(obj->video->width()));
}

NAN_METHOD(NODE_VideoSurface2D::NODE_height) {
    NODE_VideoSurface2D* obj = ObjectWrap::Unwrap<NODE_VideoSurface2D>(args.This());
    NanReturnValue(NanNew<Uint32>(obj->video->height()));
}

NAN_METHOD(NODE_VideoSurface2D::NODE_fps) {
    NODE_VideoSurface2D* obj = ObjectWrap::Unwrap<NODE_VideoSurface2D>(args.This());
    NanReturnValue(NanNew<Number>(obj->video->fps()));
}

NAN_METHOD(NODE_VideoSurface2D::NODE_duration) {
    NODE_VideoSurface2D* obj = ObjectWrap::Unwrap<NODE_VideoSurface2D>(args.This());
    NanReturnValue(NanNew<Number>(obj->video->duration()));
}

NAN_METHOD(NODE_VideoSurface2D::NODE_pixels) {
    NODE_VideoSurface2D* obj = ObjectWrap::Unwrap<NODE_VideoSurface2D>(args.This());
    NanReturnValue(NanNewBufferHandle((char*)obj->video->pixels(), obj->video->width() * obj->video->height() * 4, do_nothing_callback2, NULL));
}

NAN_METHOD(NODE_VideoSurface2D::NODE_nextFrame) {
    NODE_VideoSurface2D* obj = ObjectWrap::Unwrap<NODE_VideoSurface2D>(args.This());
    NanReturnValue(NanNew<Boolean>(obj->video->nextFrame()));
}

NAN_METHOD(NODE_VideoSurface2D::NODE_seek) {
    NODE_VideoSurface2D* obj = ObjectWrap::Unwrap<NODE_VideoSurface2D>(args.This());
    obj->video->seek(args[0]->NumberValue());
    NanReturnUndefined();
}

void NODE_GraphicalContext2D::Init(Handle<Object> exports) {
    // Prepare constructor template
    Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
    tpl->SetClassName(NanNew<String>("GraphicalContext2D"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    NODE_SET_PROTOTYPE_METHOD(tpl, "path", NODE_path);
    NODE_SET_PROTOTYPE_METHOD(tpl, "paint", NODE_paint);
    NODE_SET_PROTOTYPE_METHOD(tpl, "drawPath", NODE_drawPath);
    NODE_SET_PROTOTYPE_METHOD(tpl, "drawText", NODE_drawText);
    NODE_SET_PROTOTYPE_METHOD(tpl, "drawLine", NODE_drawLine);
    NODE_SET_PROTOTYPE_METHOD(tpl, "drawCircle", NODE_drawCircle);
    NODE_SET_PROTOTYPE_METHOD(tpl, "drawRectangle", NODE_drawRectangle);
    NODE_SET_PROTOTYPE_METHOD(tpl, "drawSurface", NODE_drawSurface);
    NODE_SET_PROTOTYPE_METHOD(tpl, "rotate", NODE_rotate);
    NODE_SET_PROTOTYPE_METHOD(tpl, "translate", NODE_translate);
    NODE_SET_PROTOTYPE_METHOD(tpl, "scale", NODE_scale);
    NODE_SET_PROTOTYPE_METHOD(tpl, "concatTransform", NODE_concatTransform);
    NODE_SET_PROTOTYPE_METHOD(tpl, "setTransform", NODE_setTransform);
    NODE_SET_PROTOTYPE_METHOD(tpl, "getTransform", NODE_getTransform);
    NODE_SET_PROTOTYPE_METHOD(tpl, "clear", NODE_clear);
    NODE_SET_PROTOTYPE_METHOD(tpl, "reset", NODE_reset);
    // NODE_SET_PROTOTYPE_METHOD(tpl, "save", NODE_save);
    // NODE_SET_PROTOTYPE_METHOD(tpl, "load", NODE_load);
    NODE_SET_PROTOTYPE_METHOD(tpl, "save", NODE_save);
    NODE_SET_PROTOTYPE_METHOD(tpl, "restore", NODE_restore);
    NODE_SET_PROTOTYPE_METHOD(tpl, "flush", NODE_flush);

    NanAssignPersistent(constructor, tpl->GetFunction());

    // Export constructor.
    exports->Set(NanNew<String>("GraphicalContext2D"), tpl->GetFunction());
}

NODE_GraphicalContext2D::NODE_GraphicalContext2D(NODE_Surface2D* surface) {
    context = backend->createGraphicalContext2D(surface->surface);
}

NODE_GraphicalContext2D::~NODE_GraphicalContext2D() {
    delete context;
}

NAN_METHOD(NODE_GraphicalContext2D::New) {
    NanScope();

    if(args.IsConstructCall()) {
        // Invoked as constructor: `new MyObject(...)`
        NODE_Surface2D* surface = node::ObjectWrap::Unwrap<NODE_Surface2D>(args[0]->ToObject());
        NODE_GraphicalContext2D* obj = new NODE_GraphicalContext2D(surface);
        obj->Wrap(args.This());
        NanReturnValue(args.This());
    } else {
        // Invoked as plain function `MyObject(...)`, turn into construct call.
        const int argc = 1;
        Local<Value> argv[argc] = { args[0] };
        NanReturnValue(NanNew(constructor)->NewInstance(argc, argv));
    }
}

NAN_METHOD(NODE_GraphicalContext2D::NODE_path) {
    const int argc = 1;
    Local<Value> argv[argc] = { args.This() };
    NanReturnValue(NanNew(NODE_Path2D::constructor)->NewInstance(argc, argv));
}

NAN_METHOD(NODE_GraphicalContext2D::NODE_paint) {
    const int argc = 1;
    Local<Value> argv[argc] = { args.This() };
    NanReturnValue(NanNew(NODE_Paint2D::constructor)->NewInstance(argc, argv));
}

NAN_METHOD(NODE_GraphicalContext2D::NODE_drawPath) {
    Path2D* path = ObjectWrap::Unwrap<NODE_Path2D>(args[0]->ToObject())->path;
    Paint2D* paint = ObjectWrap::Unwrap<NODE_Paint2D>(args[1]->ToObject())->paint;
    NODE_GraphicalContext2D* self = ObjectWrap::Unwrap<NODE_GraphicalContext2D>(args.This());
    self->context->drawPath(path, paint);
    NanReturnUndefined();
}

NAN_METHOD(NODE_GraphicalContext2D::NODE_drawText) {
    NanUtf8String str(args[0]);
    double x = args[1]->NumberValue();
    double y = args[2]->NumberValue();
    Paint2D* paint = ObjectWrap::Unwrap<NODE_Paint2D>(args[3]->ToObject())->paint;
    NODE_GraphicalContext2D* self = ObjectWrap::Unwrap<NODE_GraphicalContext2D>(args.This());
    self->context->drawText(*str, x, y, paint);
    NanReturnUndefined();
}

NAN_METHOD(NODE_GraphicalContext2D::NODE_drawLine) {
    double x1 = args[0]->NumberValue();
    double y1 = args[1]->NumberValue();
    double x2 = args[2]->NumberValue();
    double y2 = args[3]->NumberValue();
    Paint2D* paint = ObjectWrap::Unwrap<NODE_Paint2D>(args[4]->ToObject())->paint;
    NODE_GraphicalContext2D* self = ObjectWrap::Unwrap<NODE_GraphicalContext2D>(args.This());
    self->context->drawLine(x1, y1, x2, y2, paint);
    NanReturnUndefined();
}

NAN_METHOD(NODE_GraphicalContext2D::NODE_drawCircle) {
    double x = args[0]->NumberValue();
    double y = args[1]->NumberValue();
    double radius = args[2]->NumberValue();
    Paint2D* paint = ObjectWrap::Unwrap<NODE_Paint2D>(args[3]->ToObject())->paint;
    NODE_GraphicalContext2D* self = ObjectWrap::Unwrap<NODE_GraphicalContext2D>(args.This());
    self->context->drawCircle(x, y, radius, paint);
    NanReturnUndefined();
}

NAN_METHOD(NODE_GraphicalContext2D::NODE_drawRectangle) {
    double x = args[0]->NumberValue();
    double y = args[1]->NumberValue();
    double w = args[2]->NumberValue();
    double h = args[3]->NumberValue();
    Paint2D* paint = ObjectWrap::Unwrap<NODE_Paint2D>(args[4]->ToObject())->paint;
    NODE_GraphicalContext2D* self = ObjectWrap::Unwrap<NODE_GraphicalContext2D>(args.This());
    self->context->drawRectangle(allofw::Rectangle2d(x, y, w, h), paint);
    NanReturnUndefined();
}

NAN_METHOD(NODE_GraphicalContext2D::NODE_drawSurface) {
    NODE_GraphicalContext2D* self = ObjectWrap::Unwrap<NODE_GraphicalContext2D>(args.This());
    if(args.Length() == 4) { // surface, x, y, paint
        Surface2D* surface = ObjectWrap::Unwrap<NODE_Surface2D>(args[0]->ToObject())->surface;
        double x = args[1]->NumberValue();
        double y = args[2]->NumberValue();
        Paint2D* paint = ObjectWrap::Unwrap<NODE_Paint2D>(args[3]->ToObject())->paint;
        self->context->drawSurface(surface, x, y, paint);
    }
    if(args.Length() == 10) { // surface, x, y, paint
        Surface2D* surface = ObjectWrap::Unwrap<NODE_Surface2D>(args[0]->ToObject())->surface;
        double sx = args[1]->NumberValue();
        double sy = args[2]->NumberValue();
        double sw = args[3]->NumberValue();
        double sh = args[4]->NumberValue();
        double dx = args[5]->NumberValue();
        double dy = args[6]->NumberValue();
        double dw = args[7]->NumberValue();
        double dh = args[8]->NumberValue();
        Paint2D* paint = ObjectWrap::Unwrap<NODE_Paint2D>(args[9]->ToObject())->paint;
        self->context->drawSurface(surface, allofw::Rectangle2d(sx, sy, sw, sh), allofw::Rectangle2d(dx, dy, dw, dh), paint);
    }
    NanReturnUndefined();
}

NAN_METHOD(NODE_GraphicalContext2D::NODE_rotate) {
    NODE_GraphicalContext2D* self = ObjectWrap::Unwrap<NODE_GraphicalContext2D>(args.This());
    self->context->rotate(args[0]->NumberValue());
    NanReturnUndefined();
}

NAN_METHOD(NODE_GraphicalContext2D::NODE_translate) {
    NODE_GraphicalContext2D* self = ObjectWrap::Unwrap<NODE_GraphicalContext2D>(args.This());
    self->context->translate(args[0]->NumberValue(), args[1]->NumberValue());
    NanReturnUndefined();
}

NAN_METHOD(NODE_GraphicalContext2D::NODE_scale) {
    NODE_GraphicalContext2D* self = ObjectWrap::Unwrap<NODE_GraphicalContext2D>(args.This());
    self->context->scale(args[0]->NumberValue(), args[1]->NumberValue());
    NanReturnUndefined();
}

NAN_METHOD(NODE_GraphicalContext2D::NODE_concatTransform) {
    allofw::Matrix3 m;
    double a = args[0]->NumberValue();
    double b = args[1]->NumberValue();
    double c = args[2]->NumberValue();
    double d = args[3]->NumberValue();
    double e = args[4]->NumberValue();
    double f = args[5]->NumberValue();
    m.a11 = a; m.a12 = c; m.a13 = e;
    m.a21 = b; m.a22 = d; m.a23 = f;
    m.a31 = 0; m.a32 = 0; m.a33 = 1;
    NODE_GraphicalContext2D* self = ObjectWrap::Unwrap<NODE_GraphicalContext2D>(args.This());
    self->context->concatTransform(m);
    NanReturnUndefined();
}

NAN_METHOD(NODE_GraphicalContext2D::NODE_setTransform) {
    allofw::Matrix3 m;
    double a = args[0]->NumberValue();
    double b = args[1]->NumberValue();
    double c = args[2]->NumberValue();
    double d = args[3]->NumberValue();
    double e = args[4]->NumberValue();
    double f = args[5]->NumberValue();
    m.a11 = a; m.a12 = c; m.a13 = e;
    m.a21 = b; m.a22 = d; m.a23 = f;
    m.a31 = 0; m.a32 = 0; m.a33 = 1;
    NODE_GraphicalContext2D* self = ObjectWrap::Unwrap<NODE_GraphicalContext2D>(args.This());
    self->context->setTransform(m);
    NanReturnUndefined();
}

NAN_METHOD(NODE_GraphicalContext2D::NODE_getTransform) {
    NanScope();
    NODE_GraphicalContext2D* self = ObjectWrap::Unwrap<NODE_GraphicalContext2D>(args.This());
    allofw::Matrix3 m = self->context->getTransform();
    Handle<Array> array = NanNew<Array>(6);
    array->Set(0, NanNew<Number>(m.a11));
    array->Set(1, NanNew<Number>(m.a21));
    array->Set(2, NanNew<Number>(m.a12));
    array->Set(3, NanNew<Number>(m.a22));
    array->Set(4, NanNew<Number>(m.a13));
    array->Set(5, NanNew<Number>(m.a23));
    NanReturnValue(array);
}

NAN_METHOD(NODE_GraphicalContext2D::NODE_clear) {
    double r = args[0]->NumberValue() / 255.0;
    double g = args[1]->NumberValue() / 255.0;
    double b = args[2]->NumberValue() / 255.0;
    double a = args.Length() < 4 ? 1 : args[3]->NumberValue();
    NODE_GraphicalContext2D* self = ObjectWrap::Unwrap<NODE_GraphicalContext2D>(args.This());
    self->context->clear(allofw::Color(r, g, b, a));
    NanReturnUndefined();
}

NAN_METHOD(NODE_GraphicalContext2D::NODE_reset) {
    NODE_GraphicalContext2D* self = ObjectWrap::Unwrap<NODE_GraphicalContext2D>(args.This());
    self->context->reset();
    NanReturnUndefined();
}

// NAN_METHOD(NODE_GraphicalContext2D::NODE_save) {
//     NanReturnUndefined();
// }

// NAN_METHOD(NODE_GraphicalContext2D::NODE_load) {
//     NanReturnUndefined();
// }

NAN_METHOD(NODE_GraphicalContext2D::NODE_save) {
    NODE_GraphicalContext2D* self = ObjectWrap::Unwrap<NODE_GraphicalContext2D>(args.This());
    self->context->save();
    NanReturnUndefined();
}

NAN_METHOD(NODE_GraphicalContext2D::NODE_restore) {
    NODE_GraphicalContext2D* self = ObjectWrap::Unwrap<NODE_GraphicalContext2D>(args.This());
    self->context->restore();
    NanReturnUndefined();
}

NAN_METHOD(NODE_GraphicalContext2D::NODE_flush) {
    NODE_GraphicalContext2D* self = ObjectWrap::Unwrap<NODE_GraphicalContext2D>(args.This());
    self->context->flush();
    NanReturnUndefined();
}

void NODE_Path2D::Init(Handle<Object> exports) {
    // Prepare constructor template
    Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
    tpl->SetClassName(NanNew<String>("Path2D"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    // Prototype
    NODE_SET_PROTOTYPE_METHOD(tpl, "moveTo", NODE_moveTo);
    NODE_SET_PROTOTYPE_METHOD(tpl, "lineTo", NODE_lineTo);
    NODE_SET_PROTOTYPE_METHOD(tpl, "bezierCurveTo", NODE_bezierCurveTo);
    NODE_SET_PROTOTYPE_METHOD(tpl, "circle", NODE_circle);
    NODE_SET_PROTOTYPE_METHOD(tpl, "arc", NODE_arc);
    NODE_SET_PROTOTYPE_METHOD(tpl, "close", NODE_close);

    NanAssignPersistent(constructor, tpl->GetFunction());

    // Export constructor.
    exports->Set(NanNew<String>("Path2D"), tpl->GetFunction());
}

NODE_Path2D::NODE_Path2D(NODE_GraphicalContext2D* context) {
    path = context->context->path();
}

NODE_Path2D::~NODE_Path2D() {
    delete path;
}

NAN_METHOD(NODE_Path2D::New) {
    NanScope();

    if(args.IsConstructCall()) {
        // Invoked as constructor: `new MyObject(...)`
        NODE_GraphicalContext2D* context = node::ObjectWrap::Unwrap<NODE_GraphicalContext2D>(args[0]->ToObject());
        NODE_Path2D* obj = new NODE_Path2D(context);
        obj->Wrap(args.This());
        NanReturnValue(args.This());
    } else {
        // Invoked as plain function `MyObject(...)`, turn into construct call.
        const int argc = 1;
        Local<Value> argv[argc] = { args[0] };
        NanReturnValue(NanNew(constructor)->NewInstance(argc, argv));
    }
}

NAN_METHOD(NODE_Path2D::NODE_moveTo) {
    NODE_Path2D* self = ObjectWrap::Unwrap<NODE_Path2D>(args.This());
    self->path->moveTo(args[0]->NumberValue(), args[1]->NumberValue());
    NanReturnThis();
}

NAN_METHOD(NODE_Path2D::NODE_lineTo) {
    NODE_Path2D* self = ObjectWrap::Unwrap<NODE_Path2D>(args.This());
    self->path->lineTo(args[0]->NumberValue(), args[1]->NumberValue());
    NanReturnThis();
}

NAN_METHOD(NODE_Path2D::NODE_bezierCurveTo) {
    NODE_Path2D* self = ObjectWrap::Unwrap<NODE_Path2D>(args.This());
    self->path->bezierCurveTo(args[0]->NumberValue(), args[1]->NumberValue(), // c1
                              args[2]->NumberValue(), args[3]->NumberValue(), // c2
                              args[4]->NumberValue(), args[5]->NumberValue()); // p
    NanReturnThis();
}

NAN_METHOD(NODE_Path2D::NODE_circle) {
    NODE_Path2D* self = ObjectWrap::Unwrap<NODE_Path2D>(args.This());
    self->path->circle(args[0]->NumberValue(), args[1]->NumberValue(), // center
                       args[2]->NumberValue()); // normal
    NanReturnThis();
}

NAN_METHOD(NODE_Path2D::NODE_arc) {
    NODE_Path2D* self = ObjectWrap::Unwrap<NODE_Path2D>(args.This());
    self->path->arc(args[0]->NumberValue(), args[1]->NumberValue(), // center
                    args[2]->NumberValue(), // radius
                    args[3]->NumberValue(), args[4]->NumberValue()); // angle1, angle2
    NanReturnThis();
}

NAN_METHOD(NODE_Path2D::NODE_close) {
    NODE_Path2D* self = ObjectWrap::Unwrap<NODE_Path2D>(args.This());
    self->path->close();
    NanReturnThis();
}

void NODE_Paint2D::Init(Handle<Object> exports) {
    // Prepare constructor template
    Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
    tpl->SetClassName(NanNew<String>("Paint2D"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    NODE_SET_PROTOTYPE_METHOD(tpl, "setMode", NODE_setMode);
    NODE_SET_PROTOTYPE_METHOD(tpl, "setColor", NODE_setColor);
    NODE_SET_PROTOTYPE_METHOD(tpl, "setStrokeWidth", NODE_setStrokeWidth);
    NODE_SET_PROTOTYPE_METHOD(tpl, "setLineCap", NODE_setLineCap);
    NODE_SET_PROTOTYPE_METHOD(tpl, "setLineJoin", NODE_setLineJoin);
    NODE_SET_PROTOTYPE_METHOD(tpl, "setTextSize", NODE_setTextSize);
    NODE_SET_PROTOTYPE_METHOD(tpl, "setTextAlign", NODE_setTextAlign);
    NODE_SET_PROTOTYPE_METHOD(tpl, "setTypeface", NODE_setTypeface);
    NODE_SET_PROTOTYPE_METHOD(tpl, "measureText", NODE_measureText);
    NODE_SET_PROTOTYPE_METHOD(tpl, "setColorMatrix", NODE_setColorMatrix);
    NODE_SET_PROTOTYPE_METHOD(tpl, "setColorMatrixScale", NODE_setColorMatrixScale);
    NODE_SET_PROTOTYPE_METHOD(tpl, "setColorMatrixScaleAlpha", NODE_setColorMatrixScaleAlpha);
    NODE_SET_PROTOTYPE_METHOD(tpl, "setTransferMode", NODE_setTransferMode);
    NODE_SET_PROTOTYPE_METHOD(tpl, "clone", NODE_clone);

    NanAssignPersistent(constructor, tpl->GetFunction());

    // Export constructor.
    exports->Set(NanNew<String>("Paint2D"), tpl->GetFunction());
}

NODE_Paint2D::NODE_Paint2D(NODE_GraphicalContext2D* context) {
    paint = context->context->paint();
}
NODE_Paint2D::NODE_Paint2D(NODE_Paint2D* paint_) {
    paint = paint_->paint->clone();
}

NODE_Paint2D::~NODE_Paint2D() {
    delete paint;
}

NAN_METHOD(NODE_Paint2D::New) {
    NanScope();
    if(args.IsConstructCall()) {
        // Invoked as constructor: `new MyObject(...)`
        if(args.Length() == 1) {
            NODE_GraphicalContext2D* context = node::ObjectWrap::Unwrap<NODE_GraphicalContext2D>(args[0]->ToObject());
            NODE_Paint2D* obj = new NODE_Paint2D(context);
            obj->Wrap(args.This());
            NanReturnThis();
        } else {
            NODE_Paint2D* paint = node::ObjectWrap::Unwrap<NODE_Paint2D>(args[0]->ToObject());
            NODE_Paint2D* obj = new NODE_Paint2D(paint);
            obj->Wrap(args.This());
            NanReturnThis();
        }
    } else {
        // Invoked as plain function `MyObject(...)`, turn into construct call.
        const int argc = 1;
        Local<Value> argv[argc] = { args[0] };
        NanReturnValue(NanNew(constructor)->NewInstance(argc, argv));
    }
}

NAN_METHOD(NODE_Paint2D::NODE_setMode) {
    NODE_Paint2D* self = ObjectWrap::Unwrap<NODE_Paint2D>(args.This());
    self->paint->setMode((PaintMode)args[0]->IntegerValue());
    NanReturnThis();
}

NAN_METHOD(NODE_Paint2D::NODE_setColor) {
    double r = args[0]->NumberValue() / 255.0;
    double g = args[1]->NumberValue() / 255.0;
    double b = args[2]->NumberValue() / 255.0;
    double a = args.Length() < 4 ? 1 : args[3]->NumberValue();
    NODE_Paint2D* self = ObjectWrap::Unwrap<NODE_Paint2D>(args.This());
    self->paint->setColor(allofw::Color(r, g, b, a));
    NanReturnThis();
}

NAN_METHOD(NODE_Paint2D::NODE_setStrokeWidth) {
    NODE_Paint2D* self = ObjectWrap::Unwrap<NODE_Paint2D>(args.This());
    self->paint->setStrokeWidth(args[0]->NumberValue());
    NanReturnThis();
}

NAN_METHOD(NODE_Paint2D::NODE_setLineCap) {
    NODE_Paint2D* self = ObjectWrap::Unwrap<NODE_Paint2D>(args.This());
    self->paint->setLineCap((LineCap)args[0]->IntegerValue());
    NanReturnThis();
}

NAN_METHOD(NODE_Paint2D::NODE_setLineJoin) {
    NODE_Paint2D* self = ObjectWrap::Unwrap<NODE_Paint2D>(args.This());
    self->paint->setLineJoin((LineJoin)args[0]->IntegerValue());
    NanReturnThis();
}

NAN_METHOD(NODE_Paint2D::NODE_setTextSize) {
    NODE_Paint2D* self = ObjectWrap::Unwrap<NODE_Paint2D>(args.This());
    self->paint->setTextSize(args[0]->NumberValue());
    NanReturnThis();
}

NAN_METHOD(NODE_Paint2D::NODE_setTextAlign) {
    NODE_Paint2D* self = ObjectWrap::Unwrap<NODE_Paint2D>(args.This());
    self->paint->setTextAlign((TextAlign)args[0]->IntegerValue());
    NanReturnThis();
}

NAN_METHOD(NODE_Paint2D::NODE_setTypeface) {
    NanUtf8String str(args[0]);
    NODE_Paint2D* self = ObjectWrap::Unwrap<NODE_Paint2D>(args.This());
    FontStyle style = FontStyle::NORMAL;
    if(args.Length() >= 2) {
        style = (FontStyle)args[1]->IntegerValue();
    }
    self->paint->setTypeface(*str, style);
    NanReturnThis();
}

NAN_METHOD(NODE_Paint2D::NODE_measureText) {
    NanScope();
    NanUtf8String str(args[0]);
    NODE_Paint2D* self = ObjectWrap::Unwrap<NODE_Paint2D>(args.This());
    double width = self->paint->measureText(*str);
    NanReturnValue(NanNew<Number>(width));
}

NAN_METHOD(NODE_Paint2D::NODE_setColorMatrix) {
    double m[20];
    for(int i = 0; i < 20; i++) m[i] = args[i]->NumberValue();
    NODE_Paint2D* self = ObjectWrap::Unwrap<NODE_Paint2D>(args.This());
    self->paint->setColorMatrix(m);
    NanReturnThis();
}

NAN_METHOD(NODE_Paint2D::NODE_setColorMatrixScale) {
    double r = args[0]->NumberValue();
    double g = args[1]->NumberValue();
    double b = args[2]->NumberValue();
    double a = args[3]->NumberValue();
    NODE_Paint2D* self = ObjectWrap::Unwrap<NODE_Paint2D>(args.This());
    self->paint->setColorMatrixScale(r, g, b, a);
    NanReturnThis();
}

NAN_METHOD(NODE_Paint2D::NODE_setColorMatrixScaleAlpha) {
    double a = args[0]->NumberValue();
    NODE_Paint2D* self = ObjectWrap::Unwrap<NODE_Paint2D>(args.This());
    self->paint->setColorMatrixScaleAlpha(a);
    NanReturnThis();
}

NAN_METHOD(NODE_Paint2D::NODE_setTransferMode) {
    NODE_Paint2D* self = ObjectWrap::Unwrap<NODE_Paint2D>(args.This());
    self->paint->setTransferMode((TransferMode)args[0]->IntegerValue());
    NanReturnThis();
}


NAN_METHOD(NODE_Paint2D::NODE_clone) {
    NanScope();
    const int argc = 2;
    Local<Value> argv[argc] = { args.This(), args.This() };
    NanReturnValue(NanNew(NODE_Paint2D::constructor)->NewInstance(argc, argv));
}

Persistent<Function> NODE_Surface2D::constructor;
Persistent<Function> NODE_VideoSurface2D::constructor;
Persistent<Function> NODE_GraphicalContext2D::constructor;
Persistent<Function> NODE_Path2D::constructor;
Persistent<Function> NODE_Paint2D::constructor;

NAN_METHOD(NODE_loadImageData) {
    NanScope();
    const int argc = 1;
    Local<Value> argv[argc] = { args[0] };
    NanReturnValue(NanNew(NODE_Surface2D::constructor)->NewInstance(argc, argv));
}

void NODE_Graphics_init(Handle<Object> exports) {
    backend = GraphicalBackend::CreateSkia();

    // Classes.
    NODE_Surface2D::Init(exports);
    NODE_VideoSurface2D::Init(exports);
    NODE_GraphicalContext2D::Init(exports);
    NODE_Path2D::Init(exports);
    NODE_Paint2D::Init(exports);

    exports->Set(NanNew<String>("loadImageData"), NanNew<FunctionTemplate>(NODE_loadImageData)->GetFunction());

    // Constant values.
    exports->Set(NanNew<String>("SURFACETYPE_PDF"), NanNew<Uint32>((int32_t)NODE_Surface2D::SURFACETYPE_PDF));
    exports->Set(NanNew<String>("SURFACETYPE_RASTER"), NanNew<Uint32>((int32_t)NODE_Surface2D::SURFACETYPE_RASTER));

    exports->Set(NanNew<String>("LINECAP_BUTT"), NanNew<Uint32>((int32_t)LineCap::BUTT));
    exports->Set(NanNew<String>("LINECAP_ROUND"), NanNew<Uint32>((int32_t)LineCap::ROUND));
    exports->Set(NanNew<String>("LINECAP_SQUARE"), NanNew<Uint32>((int32_t)LineCap::SQUARE));

    exports->Set(NanNew<String>("LINEJOIN_BEVEL"), NanNew<Uint32>((int32_t)LineJoin::BEVEL));
    exports->Set(NanNew<String>("LINEJOIN_MITER"), NanNew<Uint32>((int32_t)LineJoin::MITER));
    exports->Set(NanNew<String>("LINEJOIN_ROUND"), NanNew<Uint32>((int32_t)LineJoin::ROUND));

    exports->Set(NanNew<String>("FONTSTYLE_NORMAL"), NanNew<Uint32>((int32_t)FontStyle::NORMAL));
    exports->Set(NanNew<String>("FONTSTYLE_BOLD"), NanNew<Uint32>((int32_t)FontStyle::BOLD));
    exports->Set(NanNew<String>("FONTSTYLE_ITALIC"), NanNew<Uint32>((int32_t)FontStyle::ITALIC));
    exports->Set(NanNew<String>("FONTSTYLE_BOLDITALIC"), NanNew<Uint32>((int32_t)FontStyle::BOLDITALIC));

    exports->Set(NanNew<String>("TEXTALIGN_LEFT"), NanNew<Uint32>((int32_t)TextAlign::LEFT));
    exports->Set(NanNew<String>("TEXTALIGN_CENTER"), NanNew<Uint32>((int32_t)TextAlign::CENTER));
    exports->Set(NanNew<String>("TEXTALIGN_RIGHT"), NanNew<Uint32>((int32_t)TextAlign::RIGHT));

    exports->Set(NanNew<String>("PAINTMODE_STROKE"), NanNew<Uint32>((int32_t)PaintMode::STROKE));
    exports->Set(NanNew<String>("PAINTMODE_FILL"), NanNew<Uint32>((int32_t)PaintMode::FILL));
    exports->Set(NanNew<String>("PAINTMODE_STROKEFILL"), NanNew<Uint32>((int32_t)PaintMode::STROKEFILL));

    exports->Set(NanNew<String>("TRANSFERMODE_SRC_OVER"), NanNew<Uint32>((int32_t)TransferMode::SRC_OVER));
}
