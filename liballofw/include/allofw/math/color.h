#ifndef IV_MATH_COLOR_H
#define IV_MATH_COLOR_H

#include "vector.h"

ALLOFW_NS_BEGIN

struct Color {
    float r, g, b, a;
    Color() = default;
    Color(float r_, float g_, float b_, float a_ = 1) : r(r_), g(g_), b(b_), a(a_) { }
};

ALLOFW_NS_END

#endif
