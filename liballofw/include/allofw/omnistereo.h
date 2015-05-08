#ifndef ALLOFW_OMNISTEREO_H
#define ALLOFW_OMNISTEREO_H

// Omnistereo Rendering.
#include <memory>

#include "common.h"
#include "config.h"
#include "math/math.h"

namespace ALLOFW_NS {

    class OmniStereo {
    public:

        virtual ~OmniStereo() { }

        static OmniStereo* Create(Configuration* conf);
    };

    class WrapBlend {
    public:

        static WrapBlend* LoadAllosphereCalibration_(const char* calibration_path, const char* hostname);
    };
}

#endif
