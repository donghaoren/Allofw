#ifndef ALLOFW_OMNISTEREO_H
#define ALLOFW_OMNISTEREO_H

// Omnistereo Rendering.
#include <memory>

#include "common.h"
#include "config.h"
#include "math/math.h"

namespace ALLOFW_NS {
    class OmniStereo;
    typedef shared_ptr<OmniStereo> POmniStereo;

    class WrapBlend;
    typedef shared_ptr<WrapBlend> PWrapBlend;

    class OmniStereo {
    public:

        virtual ~OmniStereo() { }

        static OmniStereo* Create_(PConfiguration conf);
        static POmniStereo Create(PConfiguration conf) {
            return POmniStereo(Create_(conf));
        }
    };

    class WrapBlend {
    public:

        static WrapBlend* LoadAllosphereCalibration_(const char* calibration_path, const char* hostname);
        static PWrapBlend LoadAllosphereCalibration(const std::string& calibration_path, const std::string& hostname) {
            return PWrapBlend(LoadAllosphereCalibration_(calibration_path.c_str(), hostname.c_str()));
        }
    };
}

#endif
