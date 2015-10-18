#ifndef IV_MATH_MATRIX_H
#define IV_MATH_MATRIX_H

#include "vector.h"
#include <algorithm>

namespace allofw {

    namespace internal {
        template<typename number_t, int size>
        void matrix_inversion(const number_t* data, number_t* output);

        extern template void matrix_inversion<double, 3>(const double*, double*);
        extern template void matrix_inversion<float, 3>(const float*, float*);
        extern template void matrix_inversion<double, 4>(const double*, double*);
        extern template void matrix_inversion<float, 4>(const float*, float*);

        template<typename number_t, int size>
        void matrix_svd(const number_t* data, number_t* s, number_t* U, number_t* V);

        extern template void matrix_svd<double, 3>(const double*, double*, double*, double*);
        extern template void matrix_svd<float, 3>(const float*, float*, float*, float*);
        extern template void matrix_svd<double, 4>(const double*, double*, double*, double*);
        extern template void matrix_svd<float, 4>(const float*, float*, float*, float*);
    }

    template <typename T>
    struct Matrix3_ {

        typedef T number_t;
        typedef Vector3_<T> vector_t;
        static const size_t size = 9;

        // Data stored in column-major order.
        number_t a11, a21, a31;
        number_t a12, a22, a32;
        number_t a13, a23, a33;


        number_t* data() { return &a11; }
        const number_t* data() const { return &a11; }
        std::pair<int, int> shape() const { return std::make_pair(3, 3); }

        Matrix3_() = default;

        static Matrix3_ zero() {
            Matrix3_ r;
            r.a11 = r.a12 = r.a13 = 0;
            r.a21 = r.a22 = r.a23 = 0;
            r.a31 = r.a32 = r.a33 = 0;
            return r;
        }

        static Matrix3_ eye() {
            Matrix3_ r;
            r.a11 = r.a22 = r.a33 = 1;
            r.a12 = r.a21 = r.a13 = 0;
            r.a31 = r.a32 = r.a23 = 0;
            return r;
        }

        template<typename Generator>
        static Matrix3_ random(Generator generator) {
            Matrix3_ r;
            for(int i = 0; i < size; i++) r.data()[i] = generator();
            return r;
        }

        static Matrix3_ fromDiagonal(const vector_t& diag) {
            Matrix3_ r;
            r.a11 = diag.x;
            r.a22 = diag.y;
            r.a33 = diag.z;
            r.a12 = r.a21 = r.a13 = 0;
            r.a31 = r.a32 = r.a23 = 0;
            return r;
        }

        static Matrix3_ fromColumns(const vector_t& e1, const vector_t& e2, const vector_t& e3) {
            Matrix3_ r;
            r.a11 = e1.x; r.a12 = e2.x; r.a13 = e3.x;
            r.a21 = e1.y; r.a22 = e2.y; r.a23 = e3.y;
            r.a31 = e1.z; r.a32 = e2.z; r.a33 = e3.z;
            return r;
        }

        static Matrix3_ fromRows(const vector_t& e1, const vector_t& e2, const vector_t& e3) {
            Matrix3_ r;
            r.a11 = e1.x; r.a12 = e1.y; r.a13 = e1.z;
            r.a21 = e2.x; r.a22 = e2.y; r.a23 = e2.z;
            r.a31 = e3.x; r.a32 = e3.y; r.a33 = e3.z;
            return r;
        }

        vector_t operator * (const vector_t& v) const {
            return vector_t(a11 * v.x + a12 * v.y + a13 * v.z,
                                a21 * v.x + a22 * v.y + a23 * v.z,
                                a31 * v.x + a32 * v.y + a33 * v.z);
        }

        Matrix3_ operator + (const Matrix3_& m) const {
            Matrix3_ r;
            r.a11 = a11 + m.a11; r.a12 = a12 + m.a12; r.a13 = a13 + m.a13;
            r.a21 = a21 + m.a21; r.a22 = a22 + m.a22; r.a23 = a23 + m.a23;
            r.a31 = a31 + m.a31; r.a32 = a32 + m.a32; r.a33 = a33 + m.a33;
            return r;
        }
        Matrix3_ operator - (const Matrix3_& m) const {
            Matrix3_ r;
            r.a11 = a11 - m.a11; r.a12 = a12 - m.a12; r.a13 = a13 - m.a13;
            r.a21 = a21 - m.a21; r.a22 = a22 - m.a22; r.a23 = a23 - m.a23;
            r.a31 = a31 - m.a31; r.a32 = a32 - m.a32; r.a33 = a33 - m.a33;
            return r;
        }
        Matrix3_ operator * (number_t s) const {
            Matrix3_ r;
            r.a11 = a11 * s; r.a12 = a12 * s; r.a13 = a13 * s;
            r.a21 = a21 * s; r.a22 = a22 * s; r.a23 = a23 * s;
            r.a31 = a31 * s; r.a32 = a32 * s; r.a33 = a33 * s;
            return r;
        }
        Matrix3_ operator * (const Matrix3_& m) const {
            Matrix3_ r;
            r.a11 = a11 * m.a11 + a12 * m.a21 + a13 * m.a31;
            r.a12 = a11 * m.a12 + a12 * m.a22 + a13 * m.a32;
            r.a13 = a11 * m.a13 + a12 * m.a23 + a13 * m.a33;
            r.a21 = a21 * m.a11 + a22 * m.a21 + a23 * m.a31;
            r.a22 = a21 * m.a12 + a22 * m.a22 + a23 * m.a32;
            r.a23 = a21 * m.a13 + a22 * m.a23 + a23 * m.a33;
            r.a31 = a31 * m.a11 + a32 * m.a21 + a33 * m.a31;
            r.a32 = a31 * m.a12 + a32 * m.a22 + a33 * m.a32;
            r.a33 = a31 * m.a13 + a32 * m.a23 + a33 * m.a33;
            return r;
        }

        number_t determinant() const {
            return a11 * (a22 * a33 - a32 * a23) +
                   a12 * (a23 * a31 - a21 * a33) +
                   a13 * (a21 * a32 - a22 * a31);
        }

        Matrix3_ inversion() const {
            Matrix3_ r;
            internal::matrix_inversion<number_t, 3>(data(), r.data());
            return r;
        }
        inline Matrix3_ i() const { return inversion(); }

        vector_t svd() const {
            vector_t r;
            internal::matrix_svd<number_t, 3>(data(), r.data(), nullptr, nullptr);
            return r;
        }

        vector_t svd(Matrix3_& U, Matrix3_& V) const {
            vector_t r;
            internal::matrix_svd<number_t, 3>(data(), r.data(), U.data(), V.data());
            return r;
        }

        Matrix3_ transpose() const {
            Matrix3_ r;
            r.a11 = a11; r.a21 = a12; r.a31 = a13;
            r.a12 = a21; r.a22 = a22; r.a32 = a23;
            r.a13 = a31; r.a23 = a32; r.a33 = a33;
            return r;
        }
        inline Matrix3_ t() const { return transpose(); }

        static Matrix3_ skew(const vector_t& v) {
            Matrix3_ r;
            r.a11 = 0; r.a22 = 0; r.a33 = 0;
            r.a12 = -v.z; r.a21 = v.z;
            r.a23 = -v.x; r.a32 = v.x;
            r.a31 = -v.y; r.a13 = v.y;
            return r;
        }

        number_t max() const { return *std::max_element(data(), data() + size); }
        number_t min() const { return *std::min_element(data(), data() + size); }

        friend number_t max(const Matrix3_& m) {
            return m.max();
        }

        friend number_t min(const Matrix3_& m) {
            return m.min();
        }

        Matrix3_ abs() const {
            Matrix3_ r;
            for(int i = 0; i < size; i++) { r.data()[i] = std::abs(data()[i]); }
            return r;
        }
        friend Matrix3_ abs(const Matrix3_& m) { return m.abs(); }
    };

    typedef Matrix3_<float> Matrix3f;
    typedef Matrix3_<double> Matrix3d;
    typedef Matrix3d Matrix3;

    static_assert(sizeof(Matrix3d) == sizeof(double) * 9, "Matrix3d alignment error.");
    static_assert(sizeof(Matrix3f) == sizeof(float) * 9, "Matrix3f alignment error.");

    template <typename T>
    struct Matrix4_ {

        typedef T number_t;
        typedef Vector4_<T> vector_t;
        static const size_t size = 16;

        number_t a11, a21, a31, a41;
        number_t a12, a22, a32, a42;
        number_t a13, a23, a33, a43;
        number_t a14, a24, a34, a44;

        number_t* data() { return &a11; }
        const number_t* data() const { return &a11; }
        std::pair<int, int> shape() const { return std::make_pair(4, 4); }

        Matrix4_() = default;

        static Matrix4_ zero() {
            Matrix4_ r;
            r.a11 = r.a12 = r.a13 = r.a14 = 0;
            r.a21 = r.a22 = r.a23 = r.a24 = 0;
            r.a31 = r.a32 = r.a33 = r.a34 = 0;
            r.a41 = r.a42 = r.a43 = r.a44 = 0;
            return r;
        }

        static Matrix4_ eye() {
            Matrix4_ r;
            r.a11 = r.a22 = r.a33 = r.a44 = 1;
            r.a21 = r.a31 = r.a41 = 0;
            r.a12 = r.a32 = r.a42 = 0;
            r.a13 = r.a23 = r.a43 = 0;
            r.a14 = r.a24 = r.a34 = 0;
            return r;
        }

        template<typename Generator>
        static Matrix4_ random(Generator generator) {
            Matrix4_ r;
            for(int i = 0; i < size; i++) r.data()[i] = generator();
            return r;
        }

        static Matrix4_ fromDiagonal(const vector_t& diag) {
            Matrix4_ r;
            r.a11 = diag.x;
            r.a22 = diag.y;
            r.a33 = diag.z;
            r.a44 = diag.w;
            r.a21 = r.a31 = r.a41 = 0;
            r.a12 = r.a32 = r.a42 = 0;
            r.a13 = r.a23 = r.a43 = 0;
            r.a14 = r.a24 = r.a34 = 0;
            return r;
        }

        static Matrix4_ fromColumns(const vector_t& e1, const vector_t& e2, const vector_t& e3, const vector_t& e4) {
            Matrix4_ r;
            r.a11 = e1.x; r.a12 = e2.x; r.a13 = e3.x; r.a14 = e4.x;
            r.a21 = e1.y; r.a22 = e2.y; r.a23 = e3.y; r.a24 = e4.y;
            r.a31 = e1.z; r.a32 = e2.z; r.a33 = e3.z; r.a34 = e4.z;
            r.a41 = e1.w; r.a42 = e2.w; r.a43 = e3.w; r.a44 = e4.w;
            return r;
        }
        static Matrix4_ fromRows(const vector_t& e1, const vector_t& e2, const vector_t& e3, const vector_t& e4) {
            Matrix4_ r;
            r.a11 = e1.x; r.a12 = e1.y; r.a13 = e1.z; r.a14 = e1.w;
            r.a21 = e2.x; r.a22 = e2.y; r.a23 = e2.z; r.a24 = e2.w;
            r.a31 = e3.x; r.a32 = e3.y; r.a33 = e3.z; r.a34 = e3.w;
            r.a41 = e4.x; r.a42 = e4.y; r.a43 = e4.z; r.a44 = e4.w;
            return r;
        }
        template<typename T1>
        vector_t operator * (const vector_t& v) const {
            return vector_t(a11 * v.x + a12 * v.y + a13 * v.z + a14 * v.w,
                            a21 * v.x + a22 * v.y + a23 * v.z + a24 * v.w,
                            a31 * v.x + a32 * v.y + a33 * v.z + a34 * v.w,
                            a41 * v.x + a42 * v.y + a43 * v.z + a44 * v.w);
        }
        Matrix4_ operator + (const Matrix4_& m) const {
            Matrix4_ r;
            r.a11 = a11 + m.a11; r.a12 = a12 + m.a12; r.a13 = a13 + m.a13; r.a14 = a14 + m.a14;
            r.a21 = a21 + m.a21; r.a22 = a22 + m.a22; r.a23 = a23 + m.a23; r.a24 = a24 + m.a24;
            r.a31 = a31 + m.a31; r.a32 = a32 + m.a32; r.a33 = a33 + m.a33; r.a34 = a34 + m.a34;
            r.a41 = a41 + m.a41; r.a42 = a42 + m.a42; r.a43 = a43 + m.a43; r.a44 = a44 + m.a44;
            return r;
        }
        Matrix4_ operator - (const Matrix4_& m) const {
            Matrix4_ r;
            r.a11 = a11 - m.a11; r.a12 = a12 - m.a12; r.a13 = a13 - m.a13; r.a14 = a14 - m.a14;
            r.a21 = a21 - m.a21; r.a22 = a22 - m.a22; r.a23 = a23 - m.a23; r.a24 = a24 - m.a24;
            r.a31 = a31 - m.a31; r.a32 = a32 - m.a32; r.a33 = a33 - m.a33; r.a34 = a34 - m.a34;
            r.a41 = a41 - m.a41; r.a42 = a42 - m.a42; r.a43 = a43 - m.a43; r.a44 = a44 - m.a44;
            return r;
        }
        Matrix4_ operator * (number_t s) const {
            Matrix4_ r;
            r.a11 = a11 * s; r.a12 = a12 * s; r.a13 = a13 * s; r.a14 = a14 * s;
            r.a21 = a21 * s; r.a22 = a22 * s; r.a23 = a23 * s; r.a24 = a24 * s;
            r.a31 = a31 * s; r.a32 = a32 * s; r.a33 = a33 * s; r.a34 = a34 * s;
            r.a41 = a41 * s; r.a42 = a42 * s; r.a43 = a43 * s; r.a44 = a44 * s;
            return r;
        }
        Matrix4_ operator * (const Matrix4_& m) const {
            Matrix4_ r;
            r.a11 = a11 * m.a11 + a12 * m.a21 + a13 * m.a31 + a14 * m.a41;
            r.a12 = a11 * m.a12 + a12 * m.a22 + a13 * m.a32 + a14 * m.a42;
            r.a13 = a11 * m.a13 + a12 * m.a23 + a13 * m.a33 + a14 * m.a43;
            r.a14 = a11 * m.a14 + a12 * m.a24 + a13 * m.a34 + a14 * m.a44;
            r.a21 = a21 * m.a11 + a22 * m.a21 + a23 * m.a31 + a24 * m.a41;
            r.a22 = a21 * m.a12 + a22 * m.a22 + a23 * m.a32 + a24 * m.a42;
            r.a23 = a21 * m.a13 + a22 * m.a23 + a23 * m.a33 + a24 * m.a43;
            r.a24 = a21 * m.a14 + a22 * m.a24 + a23 * m.a34 + a24 * m.a44;
            r.a31 = a31 * m.a11 + a32 * m.a21 + a33 * m.a31 + a34 * m.a41;
            r.a32 = a31 * m.a12 + a32 * m.a22 + a33 * m.a32 + a34 * m.a42;
            r.a33 = a31 * m.a13 + a32 * m.a23 + a33 * m.a33 + a34 * m.a43;
            r.a34 = a31 * m.a14 + a32 * m.a24 + a33 * m.a34 + a34 * m.a44;
            r.a41 = a41 * m.a11 + a42 * m.a21 + a43 * m.a31 + a44 * m.a41;
            r.a42 = a41 * m.a12 + a42 * m.a22 + a43 * m.a32 + a44 * m.a42;
            r.a43 = a41 * m.a13 + a42 * m.a23 + a43 * m.a33 + a44 * m.a43;
            r.a44 = a41 * m.a14 + a42 * m.a24 + a43 * m.a34 + a44 * m.a44;
            return r;
        }

        number_t determinant() const {
            return
              + a44 * (a11 * (a22 * a33 - a32 * a23) + a12 * (a23 * a31 - a21 * a33) + a13 * (a21 * a32 - a22 * a31))
              - a34 * (a11 * (a22 * a43 - a42 * a23) + a12 * (a23 * a41 - a21 * a43) + a13 * (a21 * a42 - a22 * a41))
              + a24 * (a11 * (a32 * a43 - a42 * a33) + a12 * (a33 * a41 - a31 * a43) + a13 * (a31 * a42 - a32 * a41))
              - a14 * (a21 * (a32 * a43 - a42 * a33) + a22 * (a33 * a41 - a31 * a43) + a23 * (a31 * a42 - a32 * a41));
        }

        Matrix4_ inversion() const {
            Matrix4_ r;
            internal::matrix_inversion<number_t, 4>(data(), r.data());
            return r;
        }
        inline Matrix4_ i() const { return inversion(); }

        vector_t svd() const {
            vector_t r;
            internal::matrix_svd<number_t, 4>(data(), r.data(), nullptr, nullptr);
            return r;
        }

        vector_t svd(Matrix4_& U, Matrix4_& V) const {
            vector_t r;
            internal::matrix_svd<number_t, 4>(data(), r.data(), U.data(), V.data());
            return r;
        }

        Matrix4_ transpose() const {
            Matrix4_ r;
            r.a11 = a11; r.a21 = a12; r.a31 = a13; r.a41 = a14;
            r.a12 = a21; r.a22 = a22; r.a32 = a23; r.a42 = a24;
            r.a13 = a31; r.a23 = a32; r.a33 = a33; r.a43 = a34;
            r.a14 = a41; r.a24 = a42; r.a34 = a43; r.a44 = a44;
            return r;
        }
        inline Matrix4_ t() const { return transpose(); }

        number_t max() const { return *std::max_element(data(), data() + size); }
        number_t min() const { return *std::min_element(data(), data() + size); }

        friend number_t max(const Matrix4_& m) {
            return m.max();
        }

        friend number_t min(const Matrix4_& m) {
            return m.min();
        }

        Matrix4_ abs() const {
            Matrix4_ r;
            for(int i = 0; i < size; i++) { r.data()[i] = std::abs(data()[i]); }
            return r;
        }
        friend Matrix4_ abs(const Matrix4_& m) { return m.abs(); }
    };

    typedef Matrix4_<float> Matrix4f;
    typedef Matrix4_<double> Matrix4d;
    typedef Matrix4d Matrix4;

    static_assert(sizeof(Matrix4d) == sizeof(double) * 16, "Matrix4d alignment error.");
    static_assert(sizeof(Matrix4f) == sizeof(float) * 16, "Matrixf alignment error.");

}

#endif
