#ifndef ALLOFW_OMNIAPP_BROADCASTING_H
#define ALLOFW_OMNIAPP_BROADCASTING_H

// Basic application with OmniStereo.
// This is designed to be a header only class.

#include "config.h"
#include "opengl.h"
#include "omnistereo.h"

#include <vector>

namespace ALLOFW_NS {

    namespace details {

        class OmniAppRendererBase;
        class OmniAppSimulatorBase;
        class OmniAppRunnerBase;

        // Each renderer manages a window with multiple viewports.
        class OmniAppRendererBase {
        public:
            OmniAppRendererBase(Configuration* config);

            virtual void onInitialize();
            virtual void onFrame(double dt);
            virtual void onCaptureViewport(const OmniStereo::Delegate::CaptureInfo& info);
            virtual void onPresent();

            virtual ~OmniAppRendererBase();

            OmniStereo* omni();
        protected:
            class Details;

            friend class OmniAppSimulatorBase;
            friend class OmniAppRunnerBase;

            // This is overriden by subclasses.
            virtual size_t getStateSize() = 0;
            virtual void* getState() = 0;

            void doInitialize();
            void doTick();
            bool shouldStop();
            Details* details() { return details_; }
        private:
            Details* details_;
        };

        class OmniAppSimulatorBase {
        public:
            OmniAppSimulatorBase(Configuration* config);

            virtual void onInitialize();
            virtual void onFrame(double dt);

            virtual ~OmniAppSimulatorBase();

        protected:
            class Details;

            friend class OmniAppRendererBase;
            friend class OmniAppRunnerBase;

            virtual size_t getStateSize() = 0;
            virtual void* getState() = 0;

            void doInitialize();
            void doTick();
            bool shouldStop();
            Details* details() { return details_; }
        private:
            Details* details_;
        };

        class OmniAppRunnerBase {
        public:
            OmniAppRunnerBase(int argc, char* argv[]);
            ~OmniAppRunnerBase();

            void run();
        protected:
            virtual details::OmniAppRendererBase* createRenderer(Configuration* config) = 0;
            virtual details::OmniAppSimulatorBase* createSimulator(Configuration* config) = 0;
            virtual void destroyRenderer(details::OmniAppRendererBase* renderer) = 0;
            virtual void destroySimulator(details::OmniAppSimulatorBase* simulator) = 0;
        private:
            class Details;
            Details* details_;
        };
    }

    // The renderer base class.
    template<typename State>
    class OmniAppRenderer : public details::OmniAppRendererBase {
    public:
        OmniAppRenderer(Configuration* config) : OmniAppRendererBase(config) { }

        typedef State state_t;

        state_t& state() { return state_; }
        const state_t& state() const { return state_; }

    protected:
        virtual size_t getStateSize() override { return sizeof(State); }
        virtual void* getState() override { return &state_; }
    private:
        state_t state_;
    };

    // The simulator base class.
    template<typename State>
    class OmniAppSimulator : public details::OmniAppSimulatorBase {
    public:
        OmniAppSimulator(Configuration* config) : OmniAppSimulatorBase(config) { }

        typedef State state_t;

        state_t& state() { return state_; }
        const state_t& state() const { return state_; }

    protected:
        virtual size_t getStateSize() override { return sizeof(State); }
        virtual void* getState() override { return &state_; }
    private:
        state_t state_;
    };

    template<typename RendererClass, typename SimulatorClass>
    class OmniAppRunner : public details::OmniAppRunnerBase {
    public:
        OmniAppRunner(int argc, char* argv[])
        : OmniAppRunnerBase(argc, argv) { }

    protected:
        virtual details::OmniAppRendererBase* createRenderer(Configuration* config) override {
            return new RendererClass(config);
        }
        virtual details::OmniAppSimulatorBase* createSimulator(Configuration* config) override {
            return new SimulatorClass(config);
        }
        virtual void destroyRenderer(details::OmniAppRendererBase* renderer) override {
            delete renderer;
        }
        virtual void destroySimulator(details::OmniAppSimulatorBase* simulator) override {
            delete simulator;
        }
    };
}

#endif


// struct State {
// };
// // User code:
// class MyRenderer : public OmniAppRenderer<State> {
// public:
//     virtual void onInitialize() {
//     }
//     virtual void onFrame(double dt) {
//     }
//     virtual void onCapture() {
//     }
// };

// class MySimulator : public OmniAppSimulator<State> {
// public:
//     virtual void onInitialize() {
//     }
//     virtual void onFrame(double dt) {
//     }
// };

// OmniAppRunner<MyRenderer, MySimulator> runner("allofw.yaml");
// runner.run();
