#include <node.h>
#include <v8.h>
#include <nan.h>

#include <allofw/graphics.h>

class NODE_Surface2D : public Nan::ObjectWrap {
public:

    static const int SURFACETYPE_RASTER = 0;
    static const int SURFACETYPE_PDF    = 1;

    static void Init(v8::Handle<v8::Object> exports);

    allofw::Surface2D* surface;

    friend NAN_METHOD(NODE_loadImageData);

private:
    explicit NODE_Surface2D(int width, int height, int type);
    explicit NODE_Surface2D(int width, int height, void* pixels);
    explicit NODE_Surface2D(const void* data, size_t length);
    ~NODE_Surface2D();

    static NAN_METHOD(New);

    static NAN_METHOD(NODE_width);
    static NAN_METHOD(NODE_height);
    static NAN_METHOD(NODE_pixels);
    static NAN_METHOD(NODE_bindTexture);
    static NAN_METHOD(NODE_uploadTexture);
    static NAN_METHOD(NODE_unbindTexture);
    static NAN_METHOD(NODE_save);

    static Nan::Persistent<v8::Function> constructor;
};

class NODE_VideoSurface2D : public Nan::ObjectWrap {
public:

    static void Init(v8::Handle<v8::Object> exports);

    allofw::VideoSurface2D* video;

private:
    explicit NODE_VideoSurface2D(const char* filename);
    ~NODE_VideoSurface2D();

    static NAN_METHOD(New);

    static NAN_METHOD(NODE_width);
    static NAN_METHOD(NODE_height);
    static NAN_METHOD(NODE_fps);
    static NAN_METHOD(NODE_duration);
    static NAN_METHOD(NODE_nextFrame);
    static NAN_METHOD(NODE_seek);
    static NAN_METHOD(NODE_pixels);
    static NAN_METHOD(NODE_setPixelBuffer);

    static Nan::Persistent<v8::Function> constructor;
};

class NODE_GraphicalContext2D : public Nan::ObjectWrap {
public:
    static void Init(v8::Handle<v8::Object> exports);

    allofw::GraphicalContext2D* context;

    friend class NODE_Path2D;
    friend class NODE_Paint2D;

private:
    explicit NODE_GraphicalContext2D(NODE_Surface2D* surface);
    ~NODE_GraphicalContext2D();

    static NAN_METHOD(New);

    static NAN_METHOD(NODE_path);
    static NAN_METHOD(NODE_paint);

    static NAN_METHOD(NODE_drawPath);
    static NAN_METHOD(NODE_drawText);
    static NAN_METHOD(NODE_drawLine);
    static NAN_METHOD(NODE_drawCircle);
    static NAN_METHOD(NODE_drawRectangle);
    static NAN_METHOD(NODE_drawSurface);

    static NAN_METHOD(NODE_rotate);
    static NAN_METHOD(NODE_translate);
    static NAN_METHOD(NODE_scale);
    static NAN_METHOD(NODE_concatTransform);
    static NAN_METHOD(NODE_setTransform);
    static NAN_METHOD(NODE_getTransform);

    static NAN_METHOD(NODE_clear);
    static NAN_METHOD(NODE_reset);

    // static v8::Handle<v8::Value> NODE_getState(const v8::Arguments& args);
    // static v8::Handle<v8::Value> NODE_setState(const v8::Arguments& args);

    static NAN_METHOD(NODE_save);
    static NAN_METHOD(NODE_restore);

    static NAN_METHOD(NODE_flush);

    static Nan::Persistent<v8::Function> constructor;
};

class NODE_Path2D : public Nan::ObjectWrap {
public:
    static void Init(v8::Handle<v8::Object> exports);

    static NAN_METHOD(NewInstance);

    allofw::Path2D* path;
    NODE_GraphicalContext2D* context;

    friend class NODE_GraphicalContext2D;

private:
    explicit NODE_Path2D(NODE_GraphicalContext2D* context);
    ~NODE_Path2D();

    static NAN_METHOD(New);

    static NAN_METHOD(NODE_moveTo);
    static NAN_METHOD(NODE_lineTo);
    static NAN_METHOD(NODE_bezierCurveTo);
    static NAN_METHOD(NODE_circle);
    static NAN_METHOD(NODE_arc);
    static NAN_METHOD(NODE_close);

    static Nan::Persistent<v8::Function> constructor;
};

class NODE_Paint2D : public Nan::ObjectWrap {
public:
    static void Init(v8::Handle<v8::Object> exports);

    static NAN_METHOD(NewInstance);

    allofw::Paint2D* paint;
    NODE_GraphicalContext2D* context;

    friend class NODE_GraphicalContext2D;

private:
    explicit NODE_Paint2D(NODE_GraphicalContext2D* context);
    explicit NODE_Paint2D(NODE_Paint2D* paint);
    ~NODE_Paint2D();

    static NAN_METHOD(New);

    static NAN_METHOD(NODE_setMode);
    static NAN_METHOD(NODE_setColor);
    static NAN_METHOD(NODE_setStrokeWidth);
    static NAN_METHOD(NODE_setLineCap);
    static NAN_METHOD(NODE_setLineJoin);

    static NAN_METHOD(NODE_setTextSize);
    static NAN_METHOD(NODE_setTextAlign);
    static NAN_METHOD(NODE_setTypeface);

    static NAN_METHOD(NODE_measureText);

    static NAN_METHOD(NODE_setColorMatrix);
    static NAN_METHOD(NODE_setColorMatrixScale);
    static NAN_METHOD(NODE_setColorMatrixScaleAlpha);
    static NAN_METHOD(NODE_setTransferMode);

    static NAN_METHOD(NODE_clone);

    static Nan::Persistent<v8::Function> constructor;
};

NAN_METHOD(NODE_loadImageData);

void NODE_Graphics_init(v8::Handle<v8::Object> exports);
