#include "allofw/graphics.h"
#include "allofw/opengl.h"

// Use release mode skia, even in debug builds.
#define SK_RELEASE

#include <SkStream.h>
#include <SkData.h>
#include <SkDevice.h>
#include <SkDocument.h>
#include <SkImage.h>
#include <SkMatrix.h>
#include <SkTypeface.h>
#include <SkColorPriv.h>
#include <SkGraphics.h>
#include <SkSurface.h>
#include <SkPath.h>
#include <SkMallocPixelRef.h>
#include <SkForceLinking.h>
#include <SkColorFilter.h>
#include <SkColorMatrixFilter.h>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

namespace allofw {

    // Functions from graphics_ffmpeg.
    VideoSurface2D* FFMPEG_VideoSurface2D_FromFile(const char* path);
    void FFMPEG_VideoSurface2D_Destroy(VideoSurface2D* surface);

namespace {

    inline double to_degree(double rad) {
        return rad / PI * 180.0;
    }

    SkColor convert_color(const Color& c) {
        int r = c.r * 255; if(r < 0) r = 0; if(r > 255) r = 255;
        int g = c.g * 255; if(g < 0) g = 0; if(g > 255) g = 255;
        int b = c.b * 255; if(b < 0) b = 0; if(b > 255) b = 255;
        int a = c.a * 255; if(a < 0) a = 0; if(a > 255) a = 255;
        return SkColorSetARGBMacro(a, r, g, b);
    }

    SkMatrix convert_matrix(const Matrix3d& mat) {
        SkMatrix r;
        r.setAll(mat.a11, mat.a12, mat.a13,
                 mat.a21, mat.a22, mat.a23,
                 mat.a31, mat.a32, mat.a33);
        return r;
    }

    SkRect convert_rect(const Rectangle2d& rect) {
        return SkRect::MakeXYWH(rect.position.x, rect.position.y, rect.size.x, rect.size.y);
    }

    class Path_Impl : public Path2D {
    public:

        struct PathAction {
            enum ActionType {
                MOVE_TO, LINE_TO, BEIZER_CURVE_TO,
                CIRCLE,
            };
            ActionType type;
            union { // First parameter
                Vector3d p;
                Vector3d center;
            };
            union { // second parameter
                Vector3d c1;
                Vector3d normal;
            };
            union {
                Vector3d c2;
                double radius;
            };
        };

        // Like most graphical contexts, we can construct a path with commands:
        // Move the pen position to p.
        virtual void moveTo(double x, double y) {
            skpath.moveTo(x, y);
        }
        // Add a line from the pen position to p, and move the pen to p.
        virtual void lineTo(double x, double y) {
            skpath.lineTo(x, y);
        }
        // Add a bezier curve, with four points: the pen, c1, c2, p, and move the pen to p.
        virtual void bezierCurveTo(double c1x, double c1y, double c2x, double c2y, double x, double y) {
            skpath.cubicTo(c1x, c1y, c2x, c2y, x, y);
        }
        // Draw circle centered at center, with normal, and radius.
        virtual void circle(double x, double y, double radius) {
            skpath.addCircle(x, y, radius);
        }

        virtual void arc(double x, double y, double radius, double angle1, double angle2) {
            angle1 = to_degree(angle1);
            angle2 = to_degree(angle2);
            double sweep = angle2 - angle1;
            if(sweep < 0) {
                sweep = fmod(sweep, 360);
                if(sweep < 0) sweep += 360;
            }
            skpath.addArc(SkRect::MakeXYWH(x - radius, y - radius, radius * 2, radius * 2), angle1, sweep);
        }

        virtual void close() {
            skpath.close();
        }

        SkPath skpath;

    };

    class Paint_Impl : public Paint2D {
    public:

        Paint_Impl() {
            paint.setAntiAlias(true);
        }

        Paint_Impl(const SkPaint& paint_) : paint(paint_) { }

        virtual void setMode(const PaintMode& mode) {
            switch(mode) {
                case PaintMode::STROKE: {
                    paint.setStyle(SkPaint::kStroke_Style);
                } break;
                case PaintMode::FILL: {
                    paint.setStyle(SkPaint::kFill_Style);
                } break;
                case PaintMode::STROKEFILL: {
                    paint.setStyle(SkPaint::kStrokeAndFill_Style);
                } break;
            }
        }

        // Set stroke/fill styles.
        virtual void setColor(const Color& color) {
            paint.setColor(convert_color(color));
        }

        virtual void setColorMatrix(double matrix[20]) {
            SkScalar m[20];
            for(int i = 0; i < 20; i++) m[i] = matrix[i];
            paint.setColorFilter(SkColorMatrixFilter::Create(m));
        }

        virtual void setColorMatrixScaleAlpha(double a) {
            SkColorMatrix matrix;
            matrix.setScale(1, 1, 1, a);
            paint.setColorFilter(SkColorMatrixFilter::Create(matrix));
        }

        virtual void setColorMatrixScale(double r, double g, double b, double a) {
            SkColorMatrix matrix;
            matrix.setScale(r, g, b, a);
            paint.setColorFilter(SkColorMatrixFilter::Create(matrix));
        }

        virtual void setTransferMode(TransferMode mode) {
        }

        virtual void setStrokeWidth(double value) {
            paint.setStrokeWidth(value);
        }

        virtual void setLineCap(LineCap value) {
            switch(value) {
                case LineCap::BUTT: {
                    paint.setStrokeCap(SkPaint::kButt_Cap);
                } break;
                case LineCap::ROUND: {
                    paint.setStrokeCap(SkPaint::kRound_Cap);
                } break;
                case LineCap::SQUARE: {
                    paint.setStrokeCap(SkPaint::kSquare_Cap);
                } break;
                default: {
                    throw invalid_argument("linecap");
                } break;
            }
        }

        virtual void setLineJoin(LineJoin value) {
            switch(value) {
                case LineJoin::MITER: {
                    paint.setStrokeJoin(SkPaint::kMiter_Join);
                } break;
                case LineJoin::ROUND: {
                    paint.setStrokeJoin(SkPaint::kRound_Join);
                } break;
                case LineJoin::BEVEL: {
                    paint.setStrokeJoin(SkPaint::kBevel_Join);
                } break;
                default: {
                    throw invalid_argument("linejoin");
                } break;
            }

        }

        // Font styles.
        virtual void setTextSize(double value) {
            paint.setTextSize(value);
        }

        virtual void setTextAlign(TextAlign align) {
            switch(align) {
                case TextAlign::LEFT: {
                    paint.setTextAlign(SkPaint::kLeft_Align);
                } break;
                case TextAlign::CENTER: {
                    paint.setTextAlign(SkPaint::kCenter_Align);
                } break;
                case TextAlign::RIGHT: {
                    paint.setTextAlign(SkPaint::kRight_Align);
                } break;
                default: {
                    throw invalid_argument("textalign");
                } break;
            }
        }

        virtual void setTypeface(const char* name, FontStyle style) {
            SkTypeface::Style sty;
            switch(style) {
                case FontStyle::NORMAL: {
                    sty = SkTypeface::kNormal;
                } break;
                case FontStyle::ITALIC: {
                    sty = SkTypeface::kItalic;
                } break;
                case FontStyle::BOLD: {
                    sty = SkTypeface::kBold;
                } break;
                case FontStyle::BOLDITALIC: {
                    sty = SkTypeface::kBoldItalic;
                } break;
                default: {
                    throw invalid_argument("fontstyle");
                } break;
            }
            SkTypeface* typeface = SkTypeface::CreateFromName(name, sty);
            if(!typeface) {
                typeface = SkTypeface::CreateFromName(NULL, sty);
            }
            paint.setTypeface(typeface);
            typeface->unref();
        }

        virtual Paint2D* clone() {
            return new Paint_Impl(paint);
        }

        // Measure text width.
        virtual double measureText(const char* text) {
            return paint.measureText(text, strlen(text));
        }

        SkPaint paint;

    };

    // A SkWStream that wrapps ByteStreams.
    class ByteStreamWrapper : public SkWStream {
    public:

        ByteStreamWrapper(ByteStream* s) : stream(s) { }
        virtual bool write(const void *buffer, size_t size) {
            return stream->write(buffer, size) == size;
        }
        virtual size_t bytesWritten() const {
            return stream->position();
        }
        virtual void flush() {
            stream->flush();
        }

        ByteStream* stream;

    };

    class Surface2D_Impl : public Surface2D {
    public:
        virtual ~Surface2D_Impl() { }
    };

    class Surface2D_Bitmap : public Surface2D_Impl {
    public:
        Surface2D_Bitmap() {
            texture = 0;
        }

        Surface2D_Bitmap(int width, int height) {
            bool r = bitmap.tryAllocPixels(SkImageInfo::Make(width, height, kRGBA_8888_SkColorType, kPremul_SkAlphaType));
            if(!r) {
                throw invalid_argument("cannot create 2D bitmap.");
            }
            texture = 0;
        }

        Surface2D_Bitmap(int width, int height, void* buffer) {
            bool r = bitmap.installPixels(SkImageInfo::Make(width, height, kRGBA_8888_SkColorType, kPremul_SkAlphaType), buffer, width * 4);
            if(!r) {
                throw invalid_argument("cannot create 2D bitmap.");
            }
            texture = 0;
        }

        // Width, height, stride.
        virtual int width() const {
            return bitmap.width();
        }

        virtual int height() const {
            return bitmap.height();
        }

        virtual void bindTexture(unsigned int unit) {
            if(!texture) {
                glGenTextures(1, &texture);
            }
            glActiveTexture(GL_TEXTURE0 + unit);
            glBindTexture(GL_TEXTURE_2D, texture);
        }

        virtual const void* pixels() const {
            return bitmap.getPixels();
        }

        virtual void uploadTexture() {
            bindTexture(0);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

            // Strange problem in skia, need RGBA format in Mac, but BGRA in linux.
            // In Linux, comment out SK_SAMPLES_FOR_X in SkUserConfig.h to solve the RGB ordering problem.

            unsigned char* bmp = (unsigned char*)bitmap.getPixels();
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width(), height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, bmp);

            glGenerateMipmap(GL_TEXTURE_2D);

            unbindTexture(0);
        }

        virtual void unbindTexture(unsigned int unit) {
            glActiveTexture(GL_TEXTURE0 + unit);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        virtual void save(ByteStream* stream) {
            SkData* data = SkImageEncoder::EncodeData(bitmap, SkImageEncoder::kPNG_Type, SkImageEncoder::kDefaultQuality);
            if(data) {
                stream->write(data->bytes(), data->size());
                data->unref();
            } else {
                cout << bitmap.width() << ", " << bitmap.height() << ", " << bitmap.rowBytes() << endl;
                cout << "Warning: unable to save." << endl;
            }
        }

        virtual ~Surface2D_Bitmap() {
            if(texture) {
                glDeleteTextures(1, &texture);
            }
        }

        SkBitmap bitmap;
        GLuint texture;
    };

    class Surface2D_Surface : public Surface2D_Impl {
    public:

        Surface2D_Surface(int width, int height) {
            // Seems the NewRaster will call SkMallocPixelRef::NewAllocate, which only accepts kN32_SkColorType,
            // so we need to ensure that kRGBA_8888_SkColorType == kN32_SkColorType.
            SkImageInfo info = SkImageInfo::Make(width, height, kRGBA_8888_SkColorType, kPremul_SkAlphaType);
            surface = SkSurface::NewRaster(info);
            if(!surface) {
                throw invalid_argument("cannot create 2D surface.");
            }
            texture = 0;
        }

        // Width, height, stride.
        virtual int width() const {
            return surface->width();
        }

        virtual int height() const {
            return surface->height();
        }

        virtual const void* pixels() const {
            SkImageInfo info;
            size_t row_bytes;
            return surface->peekPixels(&info, &row_bytes);
        }

        virtual void bindTexture(unsigned int unit) {
            if(!texture) {
                glGenTextures(1, &texture);
            }
            glActiveTexture(GL_TEXTURE0 + unit);
            glBindTexture(GL_TEXTURE_2D, texture);
        }

        virtual void uploadTexture() {
            SkImageInfo info;
            size_t row_bytes;
            unsigned char* bmp = (unsigned char*)surface->peekPixels(&info, &row_bytes);
            if(!bmp) {
                return;
            }

            bindTexture(0);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width(), height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, bmp);

            glGenerateMipmap(GL_TEXTURE_2D);

            unbindTexture(0);
        }

        virtual void unbindTexture(unsigned int unit) {
            glActiveTexture(GL_TEXTURE0 + unit);
            glBindTexture(GL_TEXTURE_2D, 0);
            glDisable(GL_TEXTURE_2D);
        }

        virtual void save(ByteStream* stream) {
            SkImage* img = surface->newImageSnapshot();
            if(!img) return;
            SkData* data = img->encode(SkImageEncoder::kPNG_Type, 0);
            if(!data) {
                img->unref();
                return;
            }
            stream->write(data->bytes(), data->size());
            data->unref();
            img->unref();
        }

        virtual ~Surface2D_Surface() {
            if(surface) {
                surface->unref();
            }
        }

        SkSurface* surface;
        GLuint texture;
    };

    class Surface2D_PDF : public Surface2D_Impl {
    public:

        Surface2D_PDF(int width_, int height_) {
            pdf_width = width_; pdf_height = height_;
            document = SkDocument::CreatePDF(&datastream);
        }

        // Width, height, stride.
        virtual int width() const {
            return pdf_width;
        }

        virtual int height() const {
            return pdf_height;
        }

        virtual const void* pixels() const {
            return NULL;
        }

        virtual void bindTexture(unsigned int unit) {
            throw not_supported();
        }

        virtual void uploadTexture() {
            throw not_supported();
        }

        virtual void unbindTexture(unsigned int unit) {
            throw not_supported();
        }

        virtual void save(ByteStream* stream) {
            document->endPage();
            document->close();
            SkData* data = datastream.copyToData();
            stream->write(data->bytes(), data->size());
            data->unref();
        }

        virtual ~Surface2D_PDF() {
            document->unref();
        }

        SkDocument* document;
        SkDynamicMemoryWStream datastream;
        int pdf_width, pdf_height;

    };

    class GraphicalContext2D_Impl : public GraphicalContext2D {
    public:
        // Initialize with a bitmap.
        GraphicalContext2D_Impl(SkBitmap& bitmap)
          : canvas_ptr(new SkCanvas(bitmap)), canvas(*canvas_ptr) {
        }
        // Initialize with a canvas pointer, add a reference to it.
        GraphicalContext2D_Impl(SkCanvas* canvas_ptr_)
          : canvas_ptr(canvas_ptr_), canvas(*canvas_ptr) {
            canvas.ref();
        }
        // Create a new path.
        virtual Path2D* path() {
            return new Path_Impl();
        }
        // Create a new paint.
        virtual Paint2D* paint() {
            return new Paint_Impl();
        }
        virtual void destroyPath(Path2D* path) {
            delete dynamic_cast<Path_Impl*>(path);
        }
        virtual void destroyPaint(Paint2D* paint) {
            delete dynamic_cast<Paint_Impl*>(paint);
        }
        // Draw a path.
        virtual void drawPath(Path2D* path, Paint2D* paint_) {
            SkPaint& paint = dynamic_cast<Paint_Impl*>(paint_)->paint;
            canvas.drawPath(dynamic_cast<Path_Impl*>(path)->skpath, paint);
        }
        // Draw text.
        virtual void drawText(const char* text, double x, double y, Paint2D* paint_) {
            SkPaint& paint = dynamic_cast<Paint_Impl*>(paint_)->paint;
            canvas.drawText(text, strlen(text), x, y, paint);
        }
        // Draw line.
        virtual void drawLine(double x1, double y1, double x2, double y2, Paint2D* paint_) {
            SkPaint& paint = dynamic_cast<Paint_Impl*>(paint_)->paint;
            canvas.drawLine(x1, y1, x2, y2, paint);
        }
        // Draw line.
        virtual void drawCircle(double x, double y, double radius, Paint2D* paint_) {
            SkPaint& paint = dynamic_cast<Paint_Impl*>(paint_)->paint;
            canvas.drawCircle(x, y, radius, paint);
        }
        virtual void drawRectangle(const Rectangle2d& rect, Paint2D* paint_) {
            SkPaint& paint = dynamic_cast<Paint_Impl*>(paint_)->paint;
            canvas.drawRect(convert_rect(rect), paint);
        }

        // Simple drawSurface
        virtual void drawSurface(Surface2D* surface_, double x, double y, Paint2D* paint_) {
            SkPaint& paint = dynamic_cast<Paint_Impl*>(paint_)->paint;
            if(typeid(*surface_) == typeid(Surface2D_Bitmap)) {
                Surface2D_Bitmap* surface = dynamic_cast<Surface2D_Bitmap*>(surface_);
                canvas.drawBitmap(surface->bitmap, x, y, &paint);
            } else {
                throw invalid_argument("drawSurface() can only take bitmaps.");
            }
        }
        virtual void drawSurface(Surface2D* surface_, const Rectangle2d& src, const Rectangle2d& dest, Paint2D* paint_) {
            SkPaint& paint = dynamic_cast<Paint_Impl*>(paint_)->paint;
            if(typeid(*surface_) == typeid(Surface2D_Bitmap)) {
                Surface2D_Bitmap* surface = dynamic_cast<Surface2D_Bitmap*>(surface_);
                canvas.drawBitmapRect(surface->bitmap, convert_rect(src), convert_rect(dest), &paint);
            } else {
                throw invalid_argument("drawSurface() can only take bitmaps.");
            }
        }
        // Rotate.
        virtual void rotate(double radius) {
            canvas.rotate(radius / PI * 180.0);
        }
        // Translate.
        virtual void translate(double tx, double ty) {
            canvas.translate(tx, ty);
        }
        // Scale.
        virtual void scale(double tx, double ty) {
            canvas.scale(tx, ty);
        }
        // Concat a transformation matrix.
        virtual void concatTransform(const Matrix3d& matrix) {
            canvas.concat(convert_matrix(matrix));
        }
        // Set the transformation matrix.
        virtual void setTransform(const Matrix3d& matrix) {
            canvas.setMatrix(convert_matrix(matrix));
        }
        // Get the current transformation matrix.
        virtual Matrix3d getTransform() const {
            Matrix3d mat;
            const SkMatrix& skm = canvas.getTotalMatrix();
            mat.a11 = skm.getScaleX();
            mat.a12 = skm.getSkewX();
            mat.a22 = skm.getScaleY();
            mat.a21 = skm.getSkewY();
            mat.a13 = skm.getTranslateX();
            mat.a23 = skm.getTranslateY();
            mat.a31 = skm.getPerspX();
            mat.a32 = skm.getPerspY();
            mat.a33 = 1.0;
            return mat;
        }
        // Clear the canvas.
        virtual void clear(const Color& color) {
            canvas.clear(convert_color(color));
        }
        // Reset the graphical state.
        virtual void reset() {
            canvas.resetMatrix();
        }
        // Save/load the current graphical state.
        virtual State getState() const {
            State s;
            s.transform = getTransform();
            return s;
        }
        // Load a saved state.
        virtual void setState(const State& state) {
            setTransform(state.transform);
        }
        // Push the graphical state.
        virtual void save() {
            canvas.save();
        }
        // Pop the graphical state.
        virtual void restore() {
            canvas.restore();
        }

        // Flush pending operations.
        virtual void flush() {
            canvas.flush();
        }

        // Destructor, unref the canvas.
        virtual ~GraphicalContext2D_Impl() {
            canvas.unref();
        }

        SkCanvas* canvas_ptr;
        SkCanvas& canvas;
        Matrix3d matrix0;
    };

    class GraphicalBackend_Skia_Impl : public GraphicalBackend {
    public:
        GraphicalBackend_Skia_Impl() {
            SkForceLinking(false);
            SkGraphics::Init();
        }
        ~GraphicalBackend_Skia_Impl() {
            SkGraphics::Term();
        }

        // Create new 2D surface.
        virtual Surface2D* createSurface2D(int width, int height) {
            //return new Surface2D_Surface(width, height);
            return new Surface2D_Bitmap(width, height);
        }

        virtual Surface2D* createSurface2DWithPixels(int width, int height, void* pixels) {
            //return new Surface2D_Surface(width, height);
            return new Surface2D_Bitmap(width, height, pixels);
        }

        virtual Surface2D* createPDFSurface2D(int width, int height) {
            return new Surface2D_PDF(width, height);
        }

        // Create 2D graphical context for a surface.
        virtual GraphicalContext2D* createGraphicalContext2D(Surface2D* surface_) {
            if(typeid(*surface_) == typeid(Surface2D_Bitmap)) {
                Surface2D_Bitmap* surface = dynamic_cast<Surface2D_Bitmap*>(surface_);
                GraphicalContext2D_Impl* r = new GraphicalContext2D_Impl(surface->bitmap);
                return r;
            }
            if(typeid(*surface_) == typeid(Surface2D_Surface)) {
                Surface2D_Surface* surface = dynamic_cast<Surface2D_Surface*>(surface_);
                GraphicalContext2D_Impl* r = new GraphicalContext2D_Impl(surface->surface->getCanvas());
                return r;
            }
            if(typeid(*surface_) == typeid(Surface2D_PDF)) {
                Surface2D_PDF* surface = dynamic_cast<Surface2D_PDF*>(surface_);
                SkCanvas* canvas = surface->document->beginPage(surface->width(), surface->height());
                GraphicalContext2D_Impl* r = new GraphicalContext2D_Impl(canvas);
                return r;
            }
            throw invalid_argument("surface");
        }

        virtual Surface2D* createSurface2DFromImage(ByteStream* stream) {
            std::vector<unsigned char> buffer;
            std::vector<unsigned char> read_buffer(1024 * 1024);
            int length;
            while((length = stream->read(&read_buffer[0], read_buffer.size())) != 0) {
                buffer.insert(buffer.end(), read_buffer.begin(), read_buffer.begin() + length);
            }
            return createSurface2DFromImage(&buffer[0], buffer.size());
        }

        virtual Surface2D* createSurface2DFromImage(const void* data, size_t length) {
            Surface2D_Bitmap* surface = new Surface2D_Bitmap();
            if(SkImageDecoder::DecodeMemory(data, length, &surface->bitmap, kRGBA_8888_SkColorType, SkImageDecoder::kDecodePixels_Mode)) {
                return surface;
            } else {
                delete surface;
                throw invalid_argument("failed to decode image.");
            }
        }

        virtual VideoSurface2D* createVideoSurface2DFromStream(ByteStream* stream) {
            throw not_implemented_yet();
        }
        virtual VideoSurface2D* createVideoSurface2DFromFile(const char* path) {
            return FFMPEG_VideoSurface2D_FromFile(path);
        }

        virtual void destroySurface2D(Surface2D* surface) {
            delete (Surface2D_Impl*)surface;
        }
        virtual void destroyVideoSurface2D(VideoSurface2D* video) {
            FFMPEG_VideoSurface2D_Destroy(video);
        }
        virtual void destroyGraphicalContext2D(GraphicalContext2D* context) {
            delete (GraphicalContext2D_Impl*)context;
        }
    };
}

GraphicalBackend* GraphicalBackend::CreateSkia() {
    return new GraphicalBackend_Skia_Impl();
}

void GraphicalBackend::Destroy(GraphicalBackend* backend) {
    delete backend;
}

GraphicalContext2D::~GraphicalContext2D() { }
Path2D::~Path2D() { }
Paint2D::~Paint2D() { }
Surface2D::~Surface2D() { }
VideoSurface2D::~VideoSurface2D() { }
GraphicalBackend::~GraphicalBackend() { }

}
