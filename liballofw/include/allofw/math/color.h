#ifndef IV_MATH_COLOR_H
#define IV_MATH_COLOR_H

#include "vector.h"

namespace allofw {

    //! Color struct.
    struct Color {
        //! RGBA.
        float r, g, b, a;
        //! Default constructor, uninitialized.
        Color() = default;
        //! Initialize with RGBA.
        Color(float r_, float g_, float b_, float a_ = 1) : r(r_), g(g_), b(b_), a(a_) { }
    };

}

#endif
