#ifndef IV_MATH_GEOMETRY_H
#define IV_MATH_GEOMETRY_H

#include "vector.h"

ALLOFW_NS_BEGIN

typedef Vector2_<int> Size2i;
typedef Vector3_<int> Size3i;
typedef Vector2_<float> Size2f;
typedef Vector3_<float> Size3f;
typedef Vector2_<double> Size2d;
typedef Vector3_<double> Size3d;

template<typename number_t>
struct Rectangle2_ {
    Vector2_<number_t> position;
    Vector2_<number_t> size;
    Rectangle2_() = default;
    Rectangle2_(const Vector2_<number_t>& position_, const Vector2_<number_t>& size_)
      : position(position_), size(size_) { }
    Rectangle2_(number_t w, number_t h)
      : position(0, 0), size(w, h) { }
    Rectangle2_(number_t x, number_t y, number_t w, number_t h)
      : position(x, y), size(w, h) { }
};

typedef Rectangle2_<float> Rectangle2f;
typedef Rectangle2_<double> Rectangle2d;
typedef Rectangle2d Rectangle2;

template<typename number_t>
struct Rectangle3_ {
    Vector3_<number_t> position;
    Vector3_<number_t> size;
    Rectangle3_() = default;
    Rectangle3_(const Vector3_<number_t>& position_, const Vector3_<number_t>& size_)
      : position(position_), size(size_) { }
    Rectangle3_(number_t w, number_t h, number_t d)
      : position(0, 0, 0), size(w, h, d) { }
    Rectangle3_(number_t x, number_t y, number_t z, number_t w, number_t h, number_t d)
      : position(x, y, z), size(w, h, d) { }
};

typedef Rectangle3_<float> Rectangle3f;
typedef Rectangle3_<double> Rectangle3d;
typedef Rectangle3d Rectangle3;

ALLOFW_NS_END

#endif
