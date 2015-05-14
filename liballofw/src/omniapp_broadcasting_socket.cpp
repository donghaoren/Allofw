#include "allofw/logger.h"
#include "allofw/utils.h"
#include "allofw/omniapp_broadcasting.h"
#include <stdlib.h>
#include <zmq.h>
#include <unistd.h>
#include <string.h>
#include <map>

ALLOFW_NS_BEGIN

namespace {
    template<typename T>
    void zmq_setsockopt_ez(void* socket, int name, T value) {
        zmq_setsockopt(socket, name, &value, sizeof(T));
    }

    class BroadcastingSocketZeroMQ : public BroadcastingSocket {
    public:
        BroadcastingSocketZeroMQ(Configuration* config) {
            std::string role = config->getSTLString("broadcasting.role", "renderer");
            role_ = kSimulator;
            if(role == "simulator") role_ = kSimulator;
            if(role == "renderer") role_ = kRenderer;

            zmq_context_ = zmq_ctx_new();

            switch(role_) {
                case kSimulator: {
                    int n_renderers = config->getInt32("broadcasting.renderer_count", 1);
                    renderer_ids_.resize(n_renderers);
                    for(int i = 0; i < n_renderers; i++) renderer_ids_[i] = i;

                    std::string ep_broadcast = config->getSTLString("broadcasting.zeromq.broadcast", "tcp://127.0.0.1:55555");
                    std::string ep_feedback = config->getSTLString("broadcasting.zeromq.feedback", "tcp://127.0.0.1:55556");

                    int r;
                    socket_pub_ = zmq_socket(zmq_context_, ZMQ_PUB);
                    zmq_setsockopt_ez(socket_pub_, ZMQ_SNDHWM, config->getInt32("broadcasting.zeromq.sndhwm", 10000));
                    zmq_setsockopt_ez(socket_pub_, ZMQ_SNDBUF, config->getInt32("broadcasting.zeromq.sndbuf", 0));
                    zmq_setsockopt_ez(socket_pub_, ZMQ_RATE, config->getInt32("broadcasting.zeromq.rate", 10000000));
                    r = zmq_bind(socket_pub_, ep_broadcast.c_str());
                    if(r < 0) throw exception("zmq_bind(socket_pub_)");

                    socket_feedback_ = zmq_socket(zmq_context_, ZMQ_PULL);
                    r = zmq_bind(socket_feedback_, ep_feedback.c_str());
                    if(r < 0) throw exception("zmq_bind(socket_feedback_)");

                } break;
                case kRenderer: {
                    renderer_id_ = config->getInt32("broadcasting.renderer_id", 0);

                    std::string ep_broadcast = config->getSTLString("broadcasting.zeromq.broadcast", "tcp://127.0.0.1:55555");
                    std::string ep_feedback = config->getSTLString("broadcasting.zeromq.feedback", "tcp://127.0.0.1:55556");

                    int r;
                    socket_sub_ = zmq_socket(zmq_context_, ZMQ_SUB);
                    zmq_setsockopt_ez(socket_sub_, ZMQ_RCVHWM, config->getInt32("broadcasting.zeromq.rcvhwm", 10000));
                    zmq_setsockopt_ez(socket_sub_, ZMQ_RCVBUF, config->getInt32("broadcasting.zeromq.rcvbuf", 0));
                    zmq_setsockopt_ez(socket_sub_, ZMQ_RATE, config->getInt32("broadcasting.zeromq.rate", 10000000));
                    r = zmq_setsockopt(socket_sub_, ZMQ_SUBSCRIBE, NULL, 0);
                    if(r < 0) throw exception("zmq_setsockopt(socket_sub_)");
                    r = zmq_connect(socket_sub_, ep_broadcast.c_str());
                    if(r < 0) throw exception("zmq_connect(socket_sub_)");

                    socket_feedback_ = zmq_socket(zmq_context_, ZMQ_PUSH);
                    r = zmq_connect(socket_feedback_, ep_feedback.c_str());
                    if(r < 0) throw exception("zmq_connect(socket_feedback_)");
                    zmq_msg_init(&msg_);
                } break;
            }
        }

        virtual Role role() const {
            return role_;
        }

        void clear_messages(void* socket) {
            while(1) {
                zmq_msg_t msg;
                zmq_msg_init(&msg);
                int r = zmq_msg_recv(&msg, socket_feedback_, ZMQ_DONTWAIT);
                zmq_msg_close(&msg);
                if(r < 0) {
                    return;
                }
            }
        }

        struct HandshakeMessage {
            unsigned int magic_numbes[4];
            int renderer_id;
            bool handshake_done;

            void magic() {
                magic_numbes[0] = rand();
                magic_numbes[1] = rand();
                magic_numbes[2] = rand();
                magic_numbes[3] = rand();
            }
            bool validate(const HandshakeMessage& m) const {
                return m.magic_numbes[0] == magic_numbes[0] &&
                       m.magic_numbes[1] == magic_numbes[1] &&
                       m.magic_numbes[2] == magic_numbes[2] &&
                       m.magic_numbes[3] == magic_numbes[3];
            }
        };

        virtual void handshake() {
            switch(role_) {
                case kSimulator: {
                    Logger::Default()->print(Logger::kInfo, "BroadcastingSocketZeroMQ::handshake (simulator)");
                    HandshakeMessage message;
                    message.magic();
                    message.handshake_done = false;

                    clear_messages(socket_feedback_);
                    std::vector<bool> renderer_status(renderer_ids_.size());
                    for(int i = 0; i < renderer_status.size(); i++) {
                        renderer_status[i] = false;
                    }
                    int renderer_ready_count = 0;
                    while(renderer_ready_count < renderer_ids_.size()) {
                        {
                            zmq_msg_t msg;
                            zmq_msg_init_size(&msg, sizeof(HandshakeMessage));
                            memcpy(zmq_msg_data(&msg), &message, sizeof(HandshakeMessage));
                            int r = zmq_msg_send(&msg, socket_pub_, 0);
                            if(r < 0) throw exception("handshake: zmq_msg_send(socket_pub_)");
                        }
                        {
                            zmq_msg_t msg;
                            zmq_msg_init(&msg);
                            int r = zmq_msg_recv(&msg, socket_feedback_, ZMQ_DONTWAIT);
                            if(r >= 0) {
                                if(zmq_msg_size(&msg) == sizeof(HandshakeMessage)) {
                                    const HandshakeMessage& m = *(const HandshakeMessage*)zmq_msg_data(&msg);
                                    if(m.validate(message)) {
                                        if(!renderer_status[m.renderer_id]) {
                                            renderer_status[m.renderer_id] = true;
                                            renderer_ready_count += 1;
                                        }
                                    }
                                }
                            }
                            zmq_msg_close(&msg);
                        }
                        usleep(10000);
                    }
                    {
                        message.handshake_done = true;
                        zmq_msg_t msg;
                        zmq_msg_init_size(&msg, sizeof(HandshakeMessage));
                        memcpy(zmq_msg_data(&msg), &message, sizeof(HandshakeMessage));
                        int r = zmq_msg_send(&msg, socket_pub_, 0);
                        if(r < 0) throw exception("handshake: zmq_msg_send(socket_pub_)");
                    }
                } break;
                case kRenderer: {
                    Logger::Default()->print(Logger::kInfo, "BroadcastingSocketZeroMQ::handshake (renderer)");
                    clear_messages(socket_sub_);
                    bool replied = false;
                    while(true) {
                        zmq_msg_t msg;
                        zmq_msg_init(&msg);
                        int r = zmq_msg_recv(&msg, socket_sub_, ZMQ_DONTWAIT);
                        if(r >= 0) {
                            if(zmq_msg_size(&msg) == sizeof(HandshakeMessage)) {
                                HandshakeMessage& m = *(HandshakeMessage*)zmq_msg_data(&msg);
                                if(m.handshake_done) {
                                    zmq_msg_close(&msg);
                                    break;
                                } else {
                                    if(!replied) {
                                        m.renderer_id = renderer_id_;
                                        zmq_msg_send(&msg, socket_feedback_, 0);
                                        replied = true;
                                    } else {
                                        zmq_msg_close(&msg);
                                    }
                                }
                            } else {
                                zmq_msg_close(&msg);
                            }
                        } else {
                            zmq_msg_close(&msg);
                        }
                    }
                } break;
            }
            Logger::Default()->print(Logger::kInfo, "BroadcastingSocketZeroMQ::handshake complete.");
        }

        // ## Simulator functions:
        // Broadcast data to all slaves, corresponding to "receive".
        virtual void broadcast(Message message) {
            if(role_ != kSimulator) throw exception("invalid role");
            zmq_msg_t msg;
            zmq_msg_init_size(&msg, message.size);
            memcpy(zmq_msg_data(&msg), message.data, message.size);
            int r = zmq_msg_send(&msg, socket_pub_, 0);
            if(r < 0) throw exception("broadcast: zmq_msg_send()");
        }
        // Collect message from renderers, corresponding to "feedback".
        virtual Messages collect() {
            if(role_ != kSimulator) throw exception("invalid role");
            for(zmq_msg_t& msg : renderer_messages_) {
                zmq_msg_close(&msg);
            }
            renderer_messages_.resize(renderer_ids_.size());
            messages_.resize(renderer_ids_.size());
            for(size_t i = 0; i < renderer_ids_.size(); i++) {
                zmq_msg_t& msg = renderer_messages_[i];
                zmq_msg_init(&msg);
                int r = zmq_msg_recv(&msg, socket_feedback_, 0);
                if(r < 0) throw exception("collect: zmq_msg_recv()");
                messages_[i].data = zmq_msg_data(&msg);
                messages_[i].size = zmq_msg_size(&msg);
            }
            Messages result;
            result.messages = &messages_[0];
            result.size = messages_.size();
            return result;
        }
        // Get the number of renderers.
        virtual int rendererCount() {
            if(role_ != kSimulator) throw exception("invalid role");
            return renderer_ids_.size();
        }

        // ## Renderer functions:
        // Wait for the next message, return its size and contents.
        // Size and contents will be invalid upon the next call of nextMessage.
        virtual Message receive() {
            if(role_ != kRenderer) throw exception("invalid role");
            zmq_msg_close(&msg_);
            zmq_msg_init(&msg_);
            int r = zmq_msg_recv(&msg_, socket_sub_, 0);
            if(r < 0) throw exception("receive: zmq_msg_recv()");
            return Message(zmq_msg_data(&msg_), zmq_msg_size(&msg_));
        }
        // Send message to the simulator.
        virtual void feedback(Message message) {
            if(role_ != kRenderer) throw exception("invalid role");
            zmq_msg_t msg;
            zmq_msg_init_size(&msg, message.size);
            memcpy(zmq_msg_data(&msg), message.data, message.size);
            zmq_msg_send(&msg, socket_feedback_, 0);
        }
        virtual int rendererID() {
            if(role_ != kRenderer) throw exception("invalid role");
            return renderer_id_;
        }

        virtual ~BroadcastingSocketZeroMQ() {
            zmq_ctx_destroy(zmq_context_);
        }
    private:
        Role role_;
        void* zmq_context_;
        void* socket_feedback_;

        // Renderer specific:
        void* socket_sub_;
        zmq_msg_t msg_;
        int renderer_id_;

        // Simulator specific:
        void* socket_pub_;
        std::vector<int> renderer_ids_;
        std::vector<Message> messages_;
        std::vector<zmq_msg_t> renderer_messages_;
    };
}

BroadcastingSocket* BroadcastingSocket::Create(Configuration* config) {
    return new BroadcastingSocketZeroMQ(config);
}

ALLOFW_NS_END
