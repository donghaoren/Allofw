#include "phasespace/phasespace/Phasespace.hpp"
#include "phasespace.h"

namespace allofw {

class PhasespaceImpl : public Phasespace {
public:

    PhasespaceImpl() {
        tracker = ::Phasespace::master();
    }

    // Get marker positions.
    virtual void getMarkers(Vector3* positions, int* last_seens, int start, int count) {
        vector<Vec3f> pos(count);
        tracker->getMarkers(&pos[0], last_seens, start, count);
        for(int i = 0; i < count; i++) {
            positions[i].x = pos[i].x;
            positions[i].y = pos[i].y;
            positions[i].z = pos[i].z;
        }
    }

    virtual void start() {
        tracker->start();
    }

    virtual ~PhasespaceImpl() {
        tracker->stop();
        delete tracker;
    }

    ::Phasespace* tracker;
};

Phasespace* Phasespace::Create() {
    return new PhasespaceImpl();
}

}
