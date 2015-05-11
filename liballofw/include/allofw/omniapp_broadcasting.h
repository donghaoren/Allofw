#ifndef ALLOFW_OMNIAPP_H
#define ALLOFW_OMNIAPP_H

// Basic application with OmniStereo.
// This is designed to be a header only class.

#include "opengl.h"
#include "omniapp.h"

#include <vector>

namespace ALLOFW_NS {

    // The broadcasting simulator.
    class BroadcastingSimulator : non_copyable {
    public:
        BroadcastingSimulator(Configuration* config_);

        virtual void onInitialize();
        virtual void onFrame();

        void main();
        void tick();

        // Close this app?
        bool closing();
        void setClosing();

        ~BroadcastingSimulator();
    private:
        struct Details;
        Details* details_;
    };

    // The broadcasting renderer.
    class BroadcastingRenderer : public virtual OmniAppMixin {
    public:
        OmniAppMixin_BroadcastingRenderer();

        // Holds on and wait for the next frame.
        virtual void onFrame(double dt) override;

        ~OmniAppMixin_BroadcastingRenderer();
    private:
        struct Details;
        Details* details_;
    };

}

#endif
