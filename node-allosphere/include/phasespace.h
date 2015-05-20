#include <allofw/math/quaternion.h>

namespace allofw {

class Phasespace {
public:

    // Get marker positions.
    virtual void getMarkers(Vector3* positions, int* last_seens, int start, int count) = 0;
    virtual void start() = 0;

    virtual ~Phasespace() { }

    static Phasespace* Create();
};

}
