#ifndef ALLOFW_OMNIAPP_BROADCASTING_H
#define ALLOFW_OMNIAPP_BROADCASTING_H

// Basic application with OmniStereo.
// This is designed to be a header only class.

#include "config.h"
#include "omniapp.h"

#include <vector>

namespace ALLOFW_NS {

    // Abstraction of the networking layer.
    class BroadcastingSocket {
    public:
        enum Role {
            kSimulator  = 0,
            kRenderer   = 1
        };

        struct Message {
            const void* data;
            size_t size;

            Message() : data(nullptr), size(0) { }

            template<typename DataType>
            const DataType& as(size_t offset = 0) const { return *(DataType*)((const unsigned char*)data + offset); }

            Message(void* data_, size_t size_) : data(data_), size(size_) { }

            template<typename T>
            Message(const T& data_) : data(&data_), size(sizeof(T)) { }
        };

        struct Messages {
            const Message* messages;
            size_t size;
        };

        virtual Role role() const = 0;

        // ## Simulator functions:
        // Broadcast data to all slaves, corresponding to "receive".
        virtual void broadcast(Message message) = 0;
        // Collect message from renderers, corresponding to "feedback".
        virtual Messages collect() = 0;
        // Get the number of renderers.
        virtual int rendererCount() = 0;

        // Handshake simulator and renderers.
        virtual void handshake() = 0;

        // ## Renderer functions:
        // Wait for the next message, return its size and contents.
        // Size and contents will be invalid upon the next call of nextMessage.
        virtual Message receive() = 0;
        // Send message to the simulator.
        virtual void feedback(Message msg) = 0;
        virtual int rendererID() = 0;

        // Note:
        //   These functions are all designed to be synchronous.
        //   If you choose to implement this interface,
        //   consider performing the networking in a separate thread,
        //   and use a queue to buffer the messages.

        virtual ~BroadcastingSocket() { }

        // Default implementation is based on ZeroMQ.
        static BroadcastingSocket* Create(Configuration* config);
        static BroadcastingSocket* CreateZeroMQ(Configuration* config);
    };

    namespace details {
        // Basic state broadcasting app.
        class BroadcastingAppSocket : non_copyable {
        public:
            BroadcastingAppSocket(Configuration* config, size_t state_size);

            BroadcastingSocket* socket() const;
            void* state() const;
            size_t state_size() const;

            // Handshake simulator and renderers.
            void handshake();
            // Synchronize states.
            void sync();
            // Barrier before present.
            void barrier();

            ~BroadcastingAppSocket();
        private:
            struct Details;
            Details* details_;
        };

        class SimulatorLoop {
        public:
            SimulatorLoop();
            virtual void onInitialize() { }
            virtual void onFrame(double dt) { }
            virtual void onFrameAfter() { }
            void initialize(Configuration* config);
            void main();
            void tick();
            virtual ~SimulatorLoop();
            Configuration* config();
        private:
            struct Details;
            Details* details_;
        };
    }

    template<typename State>
    class BroadcastingApp_Simulator : public virtual details::SimulatorLoop {
    public:
        BroadcastingApp_Simulator() { socket_ = nullptr; }

        virtual void onInitialize() override {
            socket_ = new details::BroadcastingAppSocket(config(), sizeof(State));
            socket_->handshake();
        }
        virtual void onFrame(double dt) override { }
        virtual void onFrameAfter() override { socket_->sync(); socket_->barrier(); }

        State& state() { return *(State*)socket_->state(); }

        virtual ~BroadcastingApp_Simulator() {
            if(socket_) delete socket_;
        }
    private:
        details::BroadcastingAppSocket* socket_;
    };

    template<typename State>
    class BroadcastingApp_Renderer : public virtual OmniAppMixin {
    public:
        BroadcastingApp_Renderer() { socket_ = nullptr; }

        virtual void onInitialize() override {
            socket_ = new details::BroadcastingAppSocket(config(), sizeof(State));
            socket_->handshake();
        }
        virtual void onFrame(double dt) override {
            socket_->sync();
        }
        virtual void onPresent() override {
            socket_->barrier();
        }

        State& state() { return *(State*)socket_->state(); }

        virtual ~BroadcastingApp_Renderer() {
            if(socket_) delete socket_;
        }
    private:
        details::BroadcastingAppSocket* socket_;
    };

}

#endif
