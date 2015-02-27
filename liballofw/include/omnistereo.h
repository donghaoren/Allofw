// Omnistereo Rendering.
#include <memory>

#include "common.h"
#include "config.h"

namespace ALLOFW_NS {
    class OmniStereo;
    typedef std::shared_ptr<OmniStereo> POmniStereo;

    class OmniStereo {
    public:

        virtual ~OmniStereo() { }

        static OmniStereo* Create_(PConfiguration conf);
        static POmniStereo Create(PConfiguration conf) {
            return POmniStereo(Create_(conf));
        }
    };
}
