#ifndef IV_MATH_VECTOR_H
#define IV_MATH_VECTOR_H

#include "basic.h"

namespace allofw {

    template <typename T>
    struct Vector2_ {
        typedef T number_t;
        static const size_t size = 2;

        Vector2_() = default;
        template <typename number_from_t>
        inline Vector2_(const Vector2_<number_from_t>& v) : x(v.x), y(v.y) { }
        inline Vector2_(number_t v) { x = y = v; }
        inline Vector2_(number_t x_, number_t y_) : x(x_), y(y_) { }

        template<typename OtherT>
        inline Vector2_(const OtherT& other)
        : x(other.x), y(other.y) { }

        inline Vector2_& operator += (const Vector2_& v) { x += v.x; y += v.y; return *this; }
        inline Vector2_& operator -= (const Vector2_& v) { x -= v.x; y -= v.y; return *this; }
        inline Vector2_& operator *= (number_t s) { x *= s; y *= s; return *this; }
        inline Vector2_& operator /= (number_t s) { x /= s; y /= s; return *this; }
        inline Vector2_ operator + (const Vector2_& v) const { return Vector2_(x + v.x, y + v.y); }
        inline Vector2_ operator - (const Vector2_& v) const { return Vector2_(x - v.x, y - v.y); }
        inline Vector2_ operator - () const { return Vector2_(-x, -y); }
        inline Vector2_ operator * (number_t s) const { return Vector2_(x * s, y * s); }
        inline Vector2_ operator / (number_t s) const { return Vector2_(x / s, y / s); }
        inline number_t dot(const Vector2_& v) const { return x * v.x + y * v.y; }
        inline number_t len2() const { return x * x + y * y; }
        inline number_t len() const { return std::sqrt(x * x + y * y); }
        inline Vector2_ unit() const { return *this / len(); }
        inline number_t cross(const Vector2_& v) const {
            return x * v.y - y * v.x;
        }
        inline number_t operator * (const Vector2_& v) const { return this->cross(v); }

        friend inline Vector2_ operator * (number_t s, const Vector2_& v) { return v * s; }

        inline Vector2_ rotate90deg() const { return Vector2_(-y, x); }
        inline double angle() const { return std::atan2(y, x); }

        number_t* data() { return &x; }
        const number_t* data() const { return &x; }
        int shape() const { return 2; }

        static Vector2_ NaN() { return Vector2_(NAN, NAN); }

        union {
            number_t x;
            number_t w;
        };
        union {
            number_t y;
            number_t h;
        };
    };

    template <typename T>
    struct Vector3_ {
        typedef T number_t;
        static const size_t size = 3;

        Vector3_() = default;
        template <typename number_from_t>
        inline Vector3_(const Vector2_<number_from_t>& v) : x(v.x), y(v.y), z(0) { }
        template <typename number_from_t>
        inline Vector3_(const Vector3_<number_from_t>& v) : x(v.x), y(v.y), z(v.z) { }
        inline Vector3_(number_t v) { x = y = z = v; }
        inline Vector3_(number_t x_, number_t y_, number_t z_) : x(x_), y(y_), z(z_) { }

        template<typename OtherT>
        inline Vector3_(const OtherT& other)
        : x(other.x), y(other.y), z(other.z) { }

        inline Vector3_& operator += (const Vector3_& v) { x += v.x; y += v.y; z += v.z; return *this; }
        inline Vector3_& operator -= (const Vector3_& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
        inline Vector3_& operator *= (number_t s) { x *= s; y *= s; z *= s; return *this; }
        inline Vector3_& operator /= (number_t s) { x /= s; y /= s; z /= s; return *this; }
        inline Vector3_ operator + (const Vector3_& v) const { return Vector3_(x + v.x, y + v.y, z + v.z); }
        inline Vector3_ operator - (const Vector3_& v) const { return Vector3_(x - v.x, y - v.y, z - v.z); }
        inline Vector3_ operator - () const { return Vector3_(-x, -y, -z); }
        inline Vector3_ operator * (number_t s) const { return Vector3_(x * s, y * s, z * s); }
        inline Vector3_ operator / (number_t s) const { return Vector3_(x / s, y / s, z / s); }
        inline number_t dot(const Vector3_& v) const { return x * v.x + y * v.y + z * v.z; }
        inline number_t len2() const { return x * x + y * y + z * z; }
        inline number_t len() const { return std::sqrt(x * x + y * y + z * z); }
        inline Vector3_ unit() const { return *this / len(); }
        inline Vector3_ cross(const Vector3_& v) const {
            return Vector3_(
                y * v.z - z * v.y,
                z * v.x - x * v.z,
                x * v.y - y * v.x
            );
        }

        friend inline Vector3_ operator * (number_t s, const Vector3_& v) { return v * s; }

        inline Vector2_<number_t> xy() const { return Vector2_<number_t>(x, y); }

        number_t* data() { return &x; }
        const number_t* data() const { return &x; }
        int shape() const { return 3; }

        static Vector3_ NaN() { return Vector3_(NAN, NAN, NAN); }

        number_t x, y, z;
    };

    template <typename T>
    struct Vector4_ {
        typedef T number_t;
        static const size_t size = 4;

        Vector4_() = default;
        template <typename number_from_t>
        inline Vector4_(const Vector2_<number_from_t>& v) : x(v.x), y(v.y), z(0), w(0) { }
        template <typename number_from_t>
        inline Vector4_(const Vector3_<number_from_t>& v) : x(v.x), y(v.y), z(v.z), w(0) { }
        template <typename number_from_t>
        inline Vector4_(const Vector4_<number_from_t>& v) : x(v.x), y(v.y), z(v.z), w(v.w) { }
        inline Vector4_(number_t v) { x = y = z = w = v; }
        inline Vector4_(number_t x_, number_t y_, number_t z_, number_t w_) : x(x_), y(y_), z(z_), w(w_) { }

        template<typename OtherT>
        inline Vector4_(const OtherT& other)
        : x(other.x), y(other.y), z(other.z), w(other.w) { }

        inline Vector4_& operator += (const Vector4_& v) { x += v.x; y += v.y; z += v.z; w += v.w; return *this; }
        inline Vector4_& operator -= (const Vector4_& v) { x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this; }
        inline Vector4_& operator *= (number_t s) { x *= s; y *= s; z *= s; w *= s; return *this; }
        inline Vector4_& operator /= (number_t s) { x /= s; y /= s; z /= s; w /= s; return *this; }
        inline Vector4_ operator + (const Vector4_& v) const { return Vector4_(x + v.x, y + v.y, z + v.z, w + v.w); }
        inline Vector4_ operator - (const Vector4_& v) const { return Vector4_(x - v.x, y - v.y, z - v.z, w - v.w); }
        inline Vector4_ operator - () const { return Vector4_(-x, -y, -z, -w); }
        inline Vector4_ operator * (number_t s) const { return Vector4_(x * s, y * s, z * s, w * s); }
        inline Vector4_ operator / (number_t s) const { return Vector4_(x / s, y / s, z / s, w / s); }
        inline number_t dot(const Vector4_& v) const { return x * v.x + y * v.y + z * v.z + w * v.w; }
        inline number_t len2() const { return x * x + y * y + z * z + w * w; }
        inline number_t len() const { return std::sqrt(x * x + y * y + z * z + w * w); }
        inline Vector4_ unit() const { return *this / len(); }

        friend inline Vector4_ operator * (number_t s, const Vector4_& v) { return v * s; }

        inline Vector2_<number_t> xy() const { return Vector2_<number_t>(x, y); }
        inline Vector3_<number_t> xyz() const { return Vector3_<number_t>(x, y, z); }

        number_t* data() { return &x; }
        const number_t* data() const { return &x; }
        int shape() const { return 4; }

        static Vector4_ NaN() { return Vector4_(NAN, NAN, NAN, NAN); }

        number_t x, y, z, w;
    };

    typedef Vector2_<double> Vector2d;
    typedef Vector2_<float> Vector2f;
    typedef Vector2d Vector2;

    static_assert(sizeof(Vector2d) == sizeof(double) * 2, "Vector2d alignment error.");
    static_assert(sizeof(Vector2f) == sizeof(float) * 2, "Vector2f alignment error.");

    typedef Vector3_<double> Vector3d;
    typedef Vector3_<float> Vector3f;
    typedef Vector3d Vector3;

    static_assert(sizeof(Vector3d) == sizeof(double) * 3, "Vector3d alignment error.");
    static_assert(sizeof(Vector3f) == sizeof(float) * 3, "Vector3f alignment error.");

    typedef Vector4_<double> Vector4d;
    typedef Vector4_<float> Vector4f;
    typedef Vector4d Vector4;

    static_assert(sizeof(Vector4d) == sizeof(double) * 4, "Vector4d alignment error.");
    static_assert(sizeof(Vector4f) == sizeof(float) * 4, "Vector4f alignment error.");

    template <typename T> bool isnan(const Vector2_<T>& v) { return std::isnan(v.x) || std::isnan(v.y); }
    template <typename T> bool isnan(const Vector3_<T>& v) { return std::isnan(v.x) || std::isnan(v.y) || std::isnan(v.z); }
    template <typename T> bool isnan(const Vector4_<T>& v) { return std::isnan(v.x) || std::isnan(v.y) || std::isnan(v.z) || std::isnan(v.w); }

}

#endif
