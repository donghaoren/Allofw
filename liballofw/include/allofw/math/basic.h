#ifndef IV_MATH_BASIC_H
#define IV_MATH_BASIC_H

#include "../common.h"
#include <cmath>
#include <limits>

namespace allofw {

    const double PI = 3.1415926535897932384626433832795028841971693993751;
    const double E = 2.7182818284590452353602874713526624977572470937000;
    const double NaN = std::numeric_limits<double>::quiet_NaN();
    const double Infinity = std::numeric_limits<double>::infinity();

    using std::isnan;

    template<typename number_t>
    inline number_t min(number_t a, number_t b) { return a < b ? a : b; }

    template<typename number_t>
    inline number_t max(number_t a, number_t b) { return a > b ? a : b; }

    template<typename number_t>
    inline number_t abs(number_t a) { return a > 0 ? a : -a; }

    template<typename number_t>
    inline number_t sign(number_t a) { return a > 0 ? 1 : (a < 0 ? -1 : 0); }

}

#endif
