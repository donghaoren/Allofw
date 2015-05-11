#ifndef IV_MATH_QUATERNION_H
#define IV_MATH_QUATERNION_H

#include "matrix.h"

ALLOFW_NS_BEGIN

template<typename T>
struct Quaternion_ {
    union {
        Vector3_<T> v;
        struct {
            T x, y, z;
        };
    };
    T w;

    inline Quaternion_() = default;

    inline Quaternion_(T w_, T x_, T y_, T z_)
    : v(x_, y_, z_), w(w_) { }

    template<typename T1>
    inline Quaternion_(T w_, const Vector3_<T1>& v_)
    : v(v_), w(w_) { }

    template<typename OtherT>
    inline Quaternion_(const OtherT& other)
    : w(other.w), v(other.x, other.y, other.z) { }

    static inline Quaternion_ rotation(const Vector3_<T>& v, double alpha) {
        return Quaternion_(std::cos(alpha / 2), v.unit() * std::sin(alpha / 2));
    }
    inline T len() const{
        return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z + w * w);
    }
    inline Quaternion_ operator + (const Quaternion_& q) const {
        return Quaternion_(w + q.w, v + q.v);
    }
    inline Quaternion_ operator - (const Quaternion_& q) const {
        return Quaternion_(w - q.w, v - q.v);
    }
    inline Quaternion_ operator - () const {
        return Quaternion_(-w, -v);
    }
    inline Quaternion_ operator * (T s) const {
        return Quaternion_(w * s, v * s);
    }
    inline Quaternion_ operator / (T s) const {
        return Quaternion_(w / s, v / s);
    }
    inline Quaternion_ unit() const {
        T l = len();
        return *this * (1.0 / l);
    }
    inline Quaternion_ operator * (const Quaternion_& q) const {
        return Quaternion_(w * q.w - v.dot(q.v), v * q.v + q.v * w + v * q.w);
    }
    template<typename T1>
    inline Quaternion_ operator * (const Vector3_<T1>& v) const {
        return *this * Quaternion_(0, v);
    }
    inline Quaternion_ inversion() const {
        return Quaternion_(w, -v);
    }
    template<typename T1>
    inline Vector3_<T1> rotate(const Vector3_<T1>& v) const {
        return ((*this) * v * inversion()).v;
    }

    inline Matrix3_<T> matrix() const {
        Vector3_<T> e1 = rotate(Vector3_<T>(1, 0, 0));
        Vector3_<T> e2 = rotate(Vector3_<T>(0, 1, 0));
        Vector3_<T> e3 = rotate(Vector3_<T>(0, 0, 1));
        return Matrix3_<T>::fromColumns(e1, e2, e3);
    }
};

template<typename T1, typename T2>
inline Vector3_<T1> operator * (const Vector3_<T1>& v, const Matrix3_<T2>& m) {
    return Vector3_<T1>(m.a11 * v.x + m.a21 * v.y + m.a31 * v.z,
                        m.a12 * v.x + m.a22 * v.y + m.a32 * v.z,
                        m.a13 * v.x + m.a23 * v.y + m.a33 * v.z);
}

template<typename T1, typename T2>
inline Quaternion_<T2> operator * (const Vector3_<T1>& v, const Quaternion_<T2>& q) {
    return Quaternion_<T2>(0, v) * q;
}

typedef Quaternion_<float> Quaternionf;
typedef Quaternion_<double> Quaterniond;
typedef Quaterniond Quaternion;

ALLOFW_NS_END

#endif
