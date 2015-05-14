#ifndef ALLOFW_GRAPHICS_H
#define ALLOFW_GRAPHICS_H

#include "common.h"
#include "stream.h"
#include "math/math.h"

namespace ALLOFW_NS {

    // Graphics API based on Skia's API.

    class Path2D;
    class Paint2D;
    class Surface2D;
    class GraphicalBackend;

    enum class LineCap : short {
        BUTT   = 0,
        ROUND  = 1,
        SQUARE = 2
    };

    enum class LineJoin : short {
        BEVEL = 0,
        MITER = 1,
        ROUND = 2
    };

    enum class FontStyle : short {
        NORMAL     = 0,
        BOLD       = 1,
        ITALIC     = 2,
        BOLDITALIC = 3
    };

    enum class TextAlign : short {
        LEFT   = 0,
        CENTER = 1,
        RIGHT  = 2
    };

    enum class PaintMode : short {
        STROKE     = 0,
        FILL       = 1,
        STROKEFILL = 2
    };

    enum class TransferMode : short {
        SRC_OVER = 0, DST_OVER = 1,
        SRC      = 2, DST      = 3,
        SRC_IN   = 4, DST_IN   = 5,
        SRC_OUT  = 6, DST_OUT  = 7,
        SRC_ATOP = 8, DST_ATOP = 9,
        XOR = 10, PLUS = 11, CLEAR = 12
    };

    // Abstract class for 2D graphical contexts.
    class GraphicalContext2D {
    public:

        // The initial origin is defined at the center of the canvas.
        // X: left to right, Y: bottom to top.

        struct State {
            Matrix3d transform;
        };

        // Begin a path, delete the path after use.
        virtual Path2D* path() = 0;
        virtual Paint2D* paint() = 0;

        virtual void destroyPath(Path2D* path) = 0;
        virtual void destroyPaint(Paint2D* paint) = 0;

        // Draw a path.
        virtual void drawPath(Path2D* path, Paint2D* paint) = 0;
        // Draw text.
        virtual void drawText(const char* text, double x, double y, Paint2D* paint) = 0;
        // Draw line.
        virtual void drawLine(double x1, double y1, double x2, double y2, Paint2D* paint) = 0;
        // Draw line.
        virtual void drawCircle(double x, double y, double radius, Paint2D* paint) = 0;

        virtual void drawRectangle(const Rectangle2d& rect, Paint2D* paint) = 0;

        // Simple drawSurface
        virtual void drawSurface(Surface2D* surface, double x, double y, Paint2D* paint) = 0;
        virtual void drawSurface(Surface2D* surface, const Rectangle2d& src, const Rectangle2d& dest, Paint2D* paint) = 0;

        virtual void rotate(double radius) = 0;
        virtual void translate(double tx, double ty) = 0;
        virtual void scale(double tx, double ty) = 0;
        virtual void concatTransform(const Matrix3d& matrix) = 0;
        virtual void setTransform(const Matrix3d& matrix) = 0;
        virtual Matrix3d getTransform() const = 0;

        // Fill the entire canvas with color.
        virtual void clear(const Color& color) = 0;

        // Reset the graphical state.
        virtual void reset() = 0;

        // Process pending operations.
        virtual void flush() = 0;

        // Save/load the current graphical state.
        virtual State getState() const = 0;
        virtual void setState(const State& state) = 0;

        // Push and pop the graphical state.
        virtual void save() = 0;
        virtual void restore() = 0;

    protected:
        virtual ~GraphicalContext2D();
    };

    class GraphicalContext3D {
    };

    // This can be used for both 2D and 3D contexts,
    // for 2D drawing, only x, y components will be considered.
    class Path2D {
    public:

        // Like most graphical contexts, we can construct a path with commands:
        // Move the pen position to p.
        virtual void moveTo(double x, double y) = 0;
        // Add a line from the pen position to p, and move the pen to p.
        virtual void lineTo(double x, double y) = 0;
        // Add a bezier curve, with four points: the pen, c1, c2, p, and move the pen to p.
        virtual void bezierCurveTo(double c1x, double c1y, double c2x, double c2y, double x, double y) = 0;
        // Draw circle centered at center, with normal, and radius.
        virtual void circle(double x, double y, double radius) = 0;

        // Draw 2D arc centered at center, radius, and angle1 to angle2.
        virtual void arc(double x, double y, double radius, double angle1, double angle2) = 0;

        // Close the current contour.
        virtual void close() = 0;

    protected:
        virtual ~Path2D();
    };

    class Paint2D {
    public:

        virtual void setMode(const PaintMode& mode) = 0;

        // Set stroke/fill styles.
        virtual void setColor(const Color& color) = 0;
        virtual void setStrokeWidth(double value) = 0;
        virtual void setLineCap(LineCap value) = 0;
        virtual void setLineJoin(LineJoin value) = 0;

        // Font styles.
        virtual void setTextSize(double value) = 0;
        virtual void setTextAlign(TextAlign align) = 0;
        virtual void setTypeface(const char* name, FontStyle style = FontStyle::NORMAL) = 0;

        // Measure text width.
        virtual double measureText(const char* text) = 0;

        // 5x4 matrix
        virtual void setColorMatrix(double matrix[20]) = 0;
        virtual void setColorMatrixScaleAlpha(double a) = 0;
        virtual void setColorMatrixScale(double r, double g, double b, double a) = 0;
        virtual void setTransferMode(TransferMode mode) = 0;

        virtual Paint2D* clone() = 0;

    protected:
        virtual ~Paint2D();
    };

    // 2D surface.
    class Surface2D {
    public:

        // Width, height, stride.
        virtual int width() const = 0;
        virtual int height() const = 0;
        virtual const void* pixels() const = 0;

        // Create a OpenGL texture for the surface.
        // Important: We are using premultiplied alpha in the textures.
        // Please use glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA) for the texture be correctly blended.
        virtual void bindTexture(unsigned int unit) = 0;
        virtual void uploadTexture() = 0;
        virtual void unbindTexture(unsigned int unit) = 0;

        virtual void save(ByteStream* stream) = 0;

    protected:
        virtual ~Surface2D();
    };

    class VideoSurface2D {
    public:

        virtual int width() const = 0;
        virtual int height() const = 0;

        virtual bool nextFrame() = 0;

        virtual void seek(double time) = 0;
        virtual double fps() = 0;
        virtual double duration() = 0;

        virtual const void* pixels() const = 0;

    protected:
        virtual ~VideoSurface2D();
    };

    class GraphicalBackend {
    public:

        // Create new 2D surface. We use RGBA8888, Premultiplied alpha format.
        virtual Surface2D* createSurface2D(int width, int height) = 0;
        // "Use" the pixels, not copy them.
        virtual Surface2D* createSurface2DWithPixels(int width, int height, void* pixels) = 0;
        // Render to PDF.
        virtual Surface2D* createPDFSurface2D(int width, int height) = 0;

        // Load image from stream or buffer.
        virtual Surface2D* createSurface2DFromImage(ByteStream* stream) = 0;
        virtual Surface2D* createSurface2DFromImage(const void* data, size_t length) = 0;

        // Load video from stream or file.
        virtual VideoSurface2D* createVideoSurface2DFromStream(ByteStream* stream) = 0;
        virtual VideoSurface2D* createVideoSurface2DFromFile(const char* path) = 0;

        // Create 2D graphical context for a surface.
        virtual GraphicalContext2D* createGraphicalContext2D(Surface2D* surface) = 0;

        virtual void destroySurface2D(Surface2D* surface) = 0;
        virtual void destroyVideoSurface2D(VideoSurface2D* video) = 0;
        virtual void destroyGraphicalContext2D(GraphicalContext2D* context) = 0;

        // Create different backends.
        static GraphicalBackend* CreateSkia();
        // Cairo binding is not unavailable yet.
        // static GraphicalBackend* CreateCairo();
        static void Destroy(GraphicalBackend* backend);

    protected:
        virtual ~GraphicalBackend();
    };
}

#endif
