#ifndef IV_MATH_QUATERNION_H
#define IV_MATH_QUATERNION_H

#include "matrix.h"

ALLOFW_NS_BEGIN

template<typename T>
struct Quaternion_ {
    T w;
    Vector3_<T> v;

    Quaternion_() = default;
    Quaternion_(T w_, T x_, T y_, T z_) : w(w_), v(x_, y_, z_) {}
    template<typename T1>
    Quaternion_(T w_, const Vector3_<T1>& v_) : w(w_), v(v_) {}

    static Quaternion_ rotation(const Vector3_<T>& v, double alpha) {
        return Quaternion_(std::cos(alpha / 2), v.normalize() * std::sin(alpha / 2));
    }
    T len() const{
        return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z + w * w);
    }
    Quaternion_ operator + (const Quaternion_& q) const {
        return Quaternion_(w + q.w, v + q.v);
    }
    Quaternion_ operator - (const Quaternion_& q) const {
        return Quaternion_(w - q.w, v - q.v);
    }
    Quaternion_ operator - () const {
        return Quaternion_(-w, -v);
    }
    Quaternion_ operator * (T s) const {
        return Quaternion_(w * s, v * s);
    }
    Quaternion_ operator / (T s) const {
        return Quaternion_(w / s, v / s);
    }
    Quaternion_ normalize() const {
        T l = len();
        return *this * (1.0 / l);
    }
    Quaternion_ operator * (const Quaternion_& q) const {
        return Quaternion_(w * q.w - v.dot(q.v), v * q.v + q.v * w + v * q.w);
    }
    template<typename T1>
    Quaternion_ operator * (const Vector3_<T1>& v) const {
        return *this * Quaternion_(0, v);
    }
    Quaternion_ inversion() const {
        return Quaternion_(w, -v);
    }
    template<typename T1>
    Vector3_<T1> rotate(const Vector3_<T1>& v) const {
        return ((*this) * v * inversion()).v;
    }

    Matrix3_<T> matrix() const {
        Vector3_<T> e1 = rotate(Vector3_<T>(1, 0, 0));
        Vector3_<T> e2 = rotate(Vector3_<T>(0, 1, 0));
        Vector3_<T> e3 = rotate(Vector3_<T>(0, 0, 1));
        return Matrix3_<T>::fromColumns(e1, e2, e3);
    }
};

template<typename T1, typename T2>
Vector3_<T1> operator * (const Vector3_<T1>& v, const Matrix3_<T2>& m) {
    return Vector3_<T1>(m.a11 * v.x + m.a21 * v.y + m.a31 * v.z,
                        m.a12 * v.x + m.a22 * v.y + m.a32 * v.z,
                        m.a13 * v.x + m.a23 * v.y + m.a33 * v.z);
}

template<typename T1, typename T2>
Quaternion_<T2> operator * (const Vector3_<T1>& v, const Quaternion_<T2>& q) {
    return Quaternion_<T2>(0, v) * q;
}

typedef Quaternion_<float> Quaternionf;
typedef Quaternion_<double> Quaterniond;
typedef Quaterniond Quaternion;

ALLOFW_NS_END

#endif
