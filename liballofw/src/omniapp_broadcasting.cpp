#include "allofw/logger.h"
#include "allofw/utils.h"
#include "allofw/omniapp_broadcasting.h"
#include <unistd.h>
#include <string.h>

ALLOFW_NS_BEGIN

namespace details {
    struct SimulatorLoop::Details {
        Configuration* config;
        double t_previous;
        bool closing;
        Details() {
            config = nullptr;
            closing = false;
        }
    };

    void SimulatorLoop::initialize(Configuration* config) {
        details_->config = config;
        onInitialize();
        details_->t_previous = get_time_seconds();
    }

    void SimulatorLoop::main() {
        while(!details_->closing) {
            tick();
        }
    }

    void SimulatorLoop::tick() {
        double t = get_time_seconds();
        double dt = t - details_->t_previous;
        details_->t_previous = t;
        onFrame(dt);
        onFrameAfter();
    }

    Configuration* SimulatorLoop::config() {
        return details_->config;
    }

    SimulatorLoop::SimulatorLoop() {
        details_ = new SimulatorLoop::Details();
    }

    SimulatorLoop::~SimulatorLoop() {
        delete details_;
    }

    struct BroadcastingAppSocket::Details {
        struct MessageHeader {
            enum MessageType {
                kSync,
                kBarrier,
                kBarrierClear,
                kBarrierResponse
            };
            MessageType type;
            int renderer_id;
        };
        BroadcastingSocket* socket;
        unsigned char* buffer;
        size_t buffer_size;
        MessageHeader* message;
        void* state;
        size_t state_size;
    };

    BroadcastingAppSocket::BroadcastingAppSocket(Configuration* config, size_t state_size) {
        details_ = new Details();
        details_->socket = BroadcastingSocket::Create(config);
        details_->state_size = state_size;
        details_->buffer_size = state_size + sizeof(Details::MessageHeader);
        details_->buffer = new unsigned char[details_->buffer_size];
        details_->message = (Details::MessageHeader*)details_->buffer;
        details_->state = details_->buffer + sizeof(Details::MessageHeader);
    }
    BroadcastingAppSocket::~BroadcastingAppSocket() {
        delete details_->socket;
        delete [] details_->buffer;
        delete details_;
    }
    BroadcastingSocket* BroadcastingAppSocket::socket() const { return details_->socket; }
    void* BroadcastingAppSocket::state() const { return details_->state; }
    size_t BroadcastingAppSocket::state_size() const { return details_->state_size; }

    void BroadcastingAppSocket::handshake() {
        details_->socket->handshake();
    }
    // Synchronize states.
    void BroadcastingAppSocket::sync() {
        if(details_->socket->role() == BroadcastingSocket::kSimulator) {
            details_->message->type = Details::MessageHeader::kSync;
            details_->socket->broadcast(BroadcastingSocket::Message(details_->message, details_->buffer_size));
        } else {
            BroadcastingSocket::Message msg = details_->socket->receive();
            if(msg.as<Details::MessageHeader>().type == Details::MessageHeader::kSync) {
                memcpy(details_->message, msg.data, min(msg.size, details_->buffer_size));
            } else {
                throw exception("BroadcastingAppSocket: sync failed.");
            }
        }
    }
    // Barrier before present.
    void BroadcastingAppSocket::barrier() {
        if(details_->socket->role() == BroadcastingSocket::kSimulator) {
            Details::MessageHeader header;
            header.type = Details::MessageHeader::kBarrier;
            details_->socket->broadcast(header);
            BroadcastingSocket::Messages responses = details_->socket->collect();
            for(int i = 0; i < responses.size; i++) {
                if(responses.messages[i].as<Details::MessageHeader>().type != Details::MessageHeader::kBarrierResponse) {
                    throw exception("BroadcastingAppSocket: barrier failed.");
                }
            }
            header.type = Details::MessageHeader::kBarrierClear;
            details_->socket->broadcast(header);
        } else {
            {
                BroadcastingSocket::Message msg = details_->socket->receive();
                const Details::MessageHeader& header = msg.as<Details::MessageHeader>();
                if(header.type == Details::MessageHeader::kBarrier) {
                    Details::MessageHeader response;
                    response.type = Details::MessageHeader::kBarrierResponse;
                    response.renderer_id = details_->socket->rendererID();
                    details_->socket->feedback(response);
                } else {
                    throw exception("BroadcastingAppSocket: barrier failed.");
                }
            }
            {
                BroadcastingSocket::Message msg = details_->socket->receive();
                const Details::MessageHeader& header = msg.as<Details::MessageHeader>();
                if(header.type == Details::MessageHeader::kBarrierClear) {
                    return;
                } else {
                    throw exception("BroadcastingAppSocket: barrier failed.");
                }
            }
        }
    }
}

ALLOFW_NS_END
