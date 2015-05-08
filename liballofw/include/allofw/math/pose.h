#ifndef IV_MATH_POSE_H
#define IV_MATH_POSE_H

#include "vector.h"
#include "quaternion.h"

ALLOFW_NS_BEGIN

struct Pose {
    Vector3 position;
    Quaternion rotation;

    Pose() : position(0, 0, 0), rotation(1, 0, 0, 0) { }

    static Pose lookAt(Vector3 eye, Vector3 at, Vector3 up);
};

ALLOFW_NS_END

#endif
