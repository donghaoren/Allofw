#include <node.h>
#include <v8.h>

#include <allofw/graphics.h>

class NODE_Surface2D : public node::ObjectWrap {
public:

    static const int SURFACETYPE_RASTER = 0;
    static const int SURFACETYPE_PDF    = 1;

    static void Init(v8::Handle<v8::Object> exports);

    allofw::Surface2D* surface;

    friend v8::Handle<v8::Value> NODE_loadImageData(const v8::Arguments& args);

private:
    explicit NODE_Surface2D(int width, int height, int type);
    explicit NODE_Surface2D(int width, int height, void* pixels);
    explicit NODE_Surface2D(const void* data, size_t length);
    ~NODE_Surface2D();

    static v8::Handle<v8::Value> New(const v8::Arguments& args);

    static v8::Handle<v8::Value> NODE_width(const v8::Arguments& args);
    static v8::Handle<v8::Value> NODE_height(const v8::Arguments& args);
    static v8::Handle<v8::Value> NODE_pixels(const v8::Arguments& args);
    static v8::Handle<v8::Value> NODE_bindTexture(const v8::Arguments& args);
    static v8::Handle<v8::Value> NODE_uploadTexture(const v8::Arguments& args);
    static v8::Handle<v8::Value> NODE_unbindTexture(const v8::Arguments& args);
    static v8::Handle<v8::Value> NODE_save(const v8::Arguments& args);

    static v8::Persistent<v8::Function> constructor;
};

class NODE_VideoSurface2D : public node::ObjectWrap {
public:

    static void Init(v8::Handle<v8::Object> exports);

    allofw::VideoSurface2D* video;

private:
    explicit NODE_VideoSurface2D(const char* filename);
    ~NODE_VideoSurface2D();

    static v8::Handle<v8::Value> New(const v8::Arguments& args);

    static v8::Handle<v8::Value> NODE_width(const v8::Arguments& args);
    static v8::Handle<v8::Value> NODE_height(const v8::Arguments& args);
    static v8::Handle<v8::Value> NODE_fps(const v8::Arguments& args);
    static v8::Handle<v8::Value> NODE_duration(const v8::Arguments& args);
    static v8::Handle<v8::Value> NODE_nextFrame(const v8::Arguments& args);
    static v8::Handle<v8::Value> NODE_seek(const v8::Arguments& args);
    static v8::Handle<v8::Value> NODE_pixels(const v8::Arguments& args);

    static v8::Persistent<v8::Function> constructor;
};

class NODE_GraphicalContext2D : public node::ObjectWrap {
public:
    static void Init(v8::Handle<v8::Object> exports);

    allofw::GraphicalContext2D* context;

private:
    explicit NODE_GraphicalContext2D(NODE_Surface2D* surface);
    ~NODE_GraphicalContext2D();

    static v8::Handle<v8::Value> New(const v8::Arguments& args);

    static v8::Handle<v8::Value> NODE_path(const v8::Arguments& args);
    static v8::Handle<v8::Value> NODE_paint(const v8::Arguments& args);

    static v8::Handle<v8::Value> NODE_drawPath(const v8::Arguments& args);
    static v8::Handle<v8::Value> NODE_drawText(const v8::Arguments& args);
    static v8::Handle<v8::Value> NODE_drawLine(const v8::Arguments& args);
    static v8::Handle<v8::Value> NODE_drawCircle(const v8::Arguments& args);
    static v8::Handle<v8::Value> NODE_drawRectangle(const v8::Arguments& args);
    static v8::Handle<v8::Value> NODE_drawSurface(const v8::Arguments& args);

    static v8::Handle<v8::Value> NODE_rotate(const v8::Arguments& args);
    static v8::Handle<v8::Value> NODE_translate(const v8::Arguments& args);
    static v8::Handle<v8::Value> NODE_scale(const v8::Arguments& args);
    static v8::Handle<v8::Value> NODE_concatTransform(const v8::Arguments& args);
    static v8::Handle<v8::Value> NODE_setTransform(const v8::Arguments& args);
    static v8::Handle<v8::Value> NODE_getTransform(const v8::Arguments& args);

    static v8::Handle<v8::Value> NODE_clear(const v8::Arguments& args);
    static v8::Handle<v8::Value> NODE_reset(const v8::Arguments& args);

    // static v8::Handle<v8::Value> NODE_getState(const v8::Arguments& args);
    // static v8::Handle<v8::Value> NODE_setState(const v8::Arguments& args);

    static v8::Handle<v8::Value> NODE_save(const v8::Arguments& args);
    static v8::Handle<v8::Value> NODE_restore(const v8::Arguments& args);

    static v8::Handle<v8::Value> NODE_flush(const v8::Arguments& args);

    static v8::Persistent<v8::Function> constructor;
};

class NODE_Path2D : public node::ObjectWrap {
public:
    static void Init(v8::Handle<v8::Object> exports);

    static v8::Handle<v8::Value> NewInstance(const v8::Arguments& args);

    allofw::Path2D* path;

    friend class NODE_GraphicalContext2D;

private:
    explicit NODE_Path2D(NODE_GraphicalContext2D* context);
    ~NODE_Path2D();

    static v8::Handle<v8::Value> New(const v8::Arguments& args);

    static v8::Handle<v8::Value> NODE_moveTo(const v8::Arguments& args);
    static v8::Handle<v8::Value> NODE_lineTo(const v8::Arguments& args);
    static v8::Handle<v8::Value> NODE_bezierCurveTo(const v8::Arguments& args);
    static v8::Handle<v8::Value> NODE_circle(const v8::Arguments& args);
    static v8::Handle<v8::Value> NODE_arc(const v8::Arguments& args);
    static v8::Handle<v8::Value> NODE_close(const v8::Arguments& args);

    static v8::Persistent<v8::Function> constructor;
};

class NODE_Paint2D : public node::ObjectWrap {
public:
    static void Init(v8::Handle<v8::Object> exports);

    static v8::Handle<v8::Value> NewInstance(const v8::Arguments& args);

    allofw::Paint2D* paint;

    friend class NODE_GraphicalContext2D;

private:
    explicit NODE_Paint2D(NODE_GraphicalContext2D* context);
    explicit NODE_Paint2D(NODE_Paint2D* paint);
    ~NODE_Paint2D();

    static v8::Handle<v8::Value> New(const v8::Arguments& args);

    static v8::Handle<v8::Value> NODE_setMode(const v8::Arguments& args);
    static v8::Handle<v8::Value> NODE_setColor(const v8::Arguments& args);
    static v8::Handle<v8::Value> NODE_setStrokeWidth(const v8::Arguments& args);
    static v8::Handle<v8::Value> NODE_setLineCap(const v8::Arguments& args);
    static v8::Handle<v8::Value> NODE_setLineJoin(const v8::Arguments& args);

    static v8::Handle<v8::Value> NODE_setTextSize(const v8::Arguments& args);
    static v8::Handle<v8::Value> NODE_setTextAlign(const v8::Arguments& args);
    static v8::Handle<v8::Value> NODE_setTypeface(const v8::Arguments& args);

    static v8::Handle<v8::Value> NODE_measureText(const v8::Arguments& args);

    static v8::Handle<v8::Value> NODE_setColorMatrix(const v8::Arguments& args);
    static v8::Handle<v8::Value> NODE_setColorMatrixScale(const v8::Arguments& args);
    static v8::Handle<v8::Value> NODE_setColorMatrixScaleAlpha(const v8::Arguments& args);
    static v8::Handle<v8::Value> NODE_setTransferMode(const v8::Arguments& args);

    static v8::Handle<v8::Value> NODE_clone(const v8::Arguments& args);

    static v8::Persistent<v8::Function> constructor;
};

v8::Handle<v8::Value> NODE_loadImageData(const v8::Arguments& args);

void NODE_Graphics_init(v8::Handle<v8::Object> exports);
