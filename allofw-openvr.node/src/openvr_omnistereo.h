#include <allofw/omnistereo.h>
#include <allofw/opengl_utils.h>

ALLOFW_NS_BEGIN

class OpenVROmniStereo : public OmniStereo {
public:
    virtual Pose getHeadPose() = 0;

protected:
    virtual ~OpenVROmniStereo();
};

OmniStereo* CreateOpenVROmniStereo();
void DestroyOpenVROmniStereo(OmniStereo* omnistereo);

ALLOFW_NS_END