#include "allofw/math/matrix.h"

#include <iostream>

using namespace std;

ALLOFW_NS_BEGIN

#define M3(x, y) ((y) * 3 + (x)) // Column-major order.
#define M4(x, y) ((y) * 4 + (x)) // Column-major order.

namespace internal {
    template<typename number_t>
    inline void matrix_inversion_3x3(const number_t* data, number_t* output) {
        // Computes the inverse of a matrix m
        number_t det = data[M3(0, 0)] * (data[M3(1, 1)] * data[M3(2, 2)] - data[M3(2, 1)] * data[M3(1, 2)]) -
                       data[M3(0, 1)] * (data[M3(1, 0)] * data[M3(2, 2)] - data[M3(1, 2)] * data[M3(2, 0)]) +
                       data[M3(0, 2)] * (data[M3(1, 0)] * data[M3(2, 1)] - data[M3(1, 1)] * data[M3(2, 0)]);
        number_t invdet = (number_t)(1) / det;
        output[M3(0, 0)] = (data[M3(1, 1)] * data[M3(2, 2)] - data[M3(2, 1)] * data[M3(1, 2)]) * invdet;
        output[M3(0, 1)] = (data[M3(0, 2)] * data[M3(2, 1)] - data[M3(0, 1)] * data[M3(2, 2)]) * invdet;
        output[M3(0, 2)] = (data[M3(0, 1)] * data[M3(1, 2)] - data[M3(0, 2)] * data[M3(1, 1)]) * invdet;
        output[M3(1, 0)] = (data[M3(1, 2)] * data[M3(2, 0)] - data[M3(1, 0)] * data[M3(2, 2)]) * invdet;
        output[M3(1, 1)] = (data[M3(0, 0)] * data[M3(2, 2)] - data[M3(0, 2)] * data[M3(2, 0)]) * invdet;
        output[M3(1, 2)] = (data[M3(1, 0)] * data[M3(0, 2)] - data[M3(0, 0)] * data[M3(1, 2)]) * invdet;
        output[M3(2, 0)] = (data[M3(1, 0)] * data[M3(2, 1)] - data[M3(2, 0)] * data[M3(1, 1)]) * invdet;
        output[M3(2, 1)] = (data[M3(2, 0)] * data[M3(0, 1)] - data[M3(0, 0)] * data[M3(2, 1)]) * invdet;
        output[M3(2, 2)] = (data[M3(0, 0)] * data[M3(1, 1)] - data[M3(1, 0)] * data[M3(0, 1)]) * invdet;
    }

    template<typename number_t>
    inline void matrix_inversion_4x4(const number_t* m, number_t* invOut) {
        // http://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
        number_t inv[16], det;
        int i;

        inv[0] = m[5]  * m[10] * m[15] -
                 m[5]  * m[11] * m[14] -
                 m[9]  * m[6]  * m[15] +
                 m[9]  * m[7]  * m[14] +
                 m[13] * m[6]  * m[11] -
                 m[13] * m[7]  * m[10];

        inv[4] = -m[4]  * m[10] * m[15] +
                  m[4]  * m[11] * m[14] +
                  m[8]  * m[6]  * m[15] -
                  m[8]  * m[7]  * m[14] -
                  m[12] * m[6]  * m[11] +
                  m[12] * m[7]  * m[10];

        inv[8] = m[4]  * m[9] * m[15] -
                 m[4]  * m[11] * m[13] -
                 m[8]  * m[5] * m[15] +
                 m[8]  * m[7] * m[13] +
                 m[12] * m[5] * m[11] -
                 m[12] * m[7] * m[9];

        inv[12] = -m[4]  * m[9] * m[14] +
                   m[4]  * m[10] * m[13] +
                   m[8]  * m[5] * m[14] -
                   m[8]  * m[6] * m[13] -
                   m[12] * m[5] * m[10] +
                   m[12] * m[6] * m[9];

        inv[1] = -m[1]  * m[10] * m[15] +
                  m[1]  * m[11] * m[14] +
                  m[9]  * m[2] * m[15] -
                  m[9]  * m[3] * m[14] -
                  m[13] * m[2] * m[11] +
                  m[13] * m[3] * m[10];

        inv[5] = m[0]  * m[10] * m[15] -
                 m[0]  * m[11] * m[14] -
                 m[8]  * m[2] * m[15] +
                 m[8]  * m[3] * m[14] +
                 m[12] * m[2] * m[11] -
                 m[12] * m[3] * m[10];

        inv[9] = -m[0]  * m[9] * m[15] +
                  m[0]  * m[11] * m[13] +
                  m[8]  * m[1] * m[15] -
                  m[8]  * m[3] * m[13] -
                  m[12] * m[1] * m[11] +
                  m[12] * m[3] * m[9];

        inv[13] = m[0]  * m[9] * m[14] -
                  m[0]  * m[10] * m[13] -
                  m[8]  * m[1] * m[14] +
                  m[8]  * m[2] * m[13] +
                  m[12] * m[1] * m[10] -
                  m[12] * m[2] * m[9];

        inv[2] = m[1]  * m[6] * m[15] -
                 m[1]  * m[7] * m[14] -
                 m[5]  * m[2] * m[15] +
                 m[5]  * m[3] * m[14] +
                 m[13] * m[2] * m[7] -
                 m[13] * m[3] * m[6];

        inv[6] = -m[0]  * m[6] * m[15] +
                  m[0]  * m[7] * m[14] +
                  m[4]  * m[2] * m[15] -
                  m[4]  * m[3] * m[14] -
                  m[12] * m[2] * m[7] +
                  m[12] * m[3] * m[6];

        inv[10] = m[0]  * m[5] * m[15] -
                  m[0]  * m[7] * m[13] -
                  m[4]  * m[1] * m[15] +
                  m[4]  * m[3] * m[13] +
                  m[12] * m[1] * m[7] -
                  m[12] * m[3] * m[5];

        inv[14] = -m[0]  * m[5] * m[14] +
                   m[0]  * m[6] * m[13] +
                   m[4]  * m[1] * m[14] -
                   m[4]  * m[2] * m[13] -
                   m[12] * m[1] * m[6] +
                   m[12] * m[2] * m[5];

        inv[3] = -m[1] * m[6] * m[11] +
                  m[1] * m[7] * m[10] +
                  m[5] * m[2] * m[11] -
                  m[5] * m[3] * m[10] -
                  m[9] * m[2] * m[7] +
                  m[9] * m[3] * m[6];

        inv[7] = m[0] * m[6] * m[11] -
                 m[0] * m[7] * m[10] -
                 m[4] * m[2] * m[11] +
                 m[4] * m[3] * m[10] +
                 m[8] * m[2] * m[7] -
                 m[8] * m[3] * m[6];

        inv[11] = -m[0] * m[5] * m[11] +
                   m[0] * m[7] * m[9] +
                   m[4] * m[1] * m[11] -
                   m[4] * m[3] * m[9] -
                   m[8] * m[1] * m[7] +
                   m[8] * m[3] * m[5];

        inv[15] = m[0] * m[5] * m[10] -
                  m[0] * m[6] * m[9] -
                  m[4] * m[1] * m[10] +
                  m[4] * m[2] * m[9] +
                  m[8] * m[1] * m[6] -
                  m[8] * m[2] * m[5];

        det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

        det = (number_t)(1) / det;

        for (i = 0; i < 16; i++) {
            invOut[i] = inv[i] * det;
        }
    }
}

template<typename number_t, int size>
void internal::matrix_inversion(const number_t* data, number_t* output) {
    if(size == 3) matrix_inversion_3x3(data, output);
    if(size == 4) matrix_inversion_4x4(data, output);
}

template void internal::matrix_inversion<double, 3>(const double* data, double* output);
template void internal::matrix_inversion<float, 3>(const float*, float*);
template void internal::matrix_inversion<double, 4>(const double*, double*);
template void internal::matrix_inversion<float, 4>(const float*, float*);

ALLOFW_NS_END
