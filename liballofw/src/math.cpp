#include <allofw/math/matrix.hpp>

#include <iostream>

#include <armadillo>

using namespace std;
using namespace arma;

IVNS_BEGIN

// Armadillo uses column-major order for storage.
// We are also using column-major order.

template<typename number_t, int size>
void internal::matrix_inversion(const number_t* data, number_t* output) {
    Mat<number_t> A(data, size, size);
    Mat<number_t> B(output, size, size, false, true);
    inv(B, A, "std");
}

template void internal::matrix_inversion<double, 3>(const double*, double*);
template void internal::matrix_inversion<float, 3>(const float*, float*);
template void internal::matrix_inversion<double, 4>(const double*, double*);
template void internal::matrix_inversion<float, 4>(const float*, float*);

template<typename number_t, int size>
void internal::matrix_svd(const number_t* data, number_t* s, number_t* U, number_t* V) {
    if(U && V) {
        Mat<number_t> A(data, size, size);
        Col<number_t> mS(s, size, false, true);
        Mat<number_t> mU(U, size, size, false, true);
        Mat<number_t> mV(V, size, size, false, true);
        svd(mU, mS, mV, A);
    } else {
        Mat<number_t> A(data, size, size);
        Col<number_t> mS(s, size, false, true);
        svd(mS, A);
    }
}

template void internal::matrix_svd<double, 3>(const double*, double*, double*, double*);
template void internal::matrix_svd<float, 3>(const float*, float*, float*, float*);
template void internal::matrix_svd<double, 4>(const double*, double*, double*, double*);
template void internal::matrix_svd<float, 4>(const float*, float*, float*, float*);

IVNS_END
