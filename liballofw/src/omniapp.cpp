#include "allofw/logger.h"
#include "allofw/utils.h"
#include "allofw/omniapp.h"
#include <unistd.h>
#include <string.h>
#include <zmq.h>
#include <pthread.h>

ALLOFW_NS_BEGIN

namespace details {

    void* global_zmq_context = nullptr;
    void* get_global_zmq_context() {
        if(!global_zmq_context) {
            global_zmq_context = zmq_ctx_new();
        }
        return global_zmq_context;
    }

    struct FrameHeader {
        int frame_id;
        double dt;
    };

    struct FPSControl {
        FPSControl(double fps) {
            min_dt = 1.0 / fps;
            t0 = 0;
        }
        void tick() {
            while(1) {
                double t = get_time_seconds();
                double dt = t - t0;
                if(dt >= min_dt) break;
                usleep(1);
            }
            t0 = get_time_seconds();
        }
        double min_dt;
        double t0;
    };

    struct FeedbackMessage {
        enum Type {
            kRegister = 0,
            kKeepalive = 1
        };
        int type;
        int client_id;
        int frame_id;
    };

    template<typename T>
    void zmq_send_struct(void* socket, const T& data) {
        zmq_msg_t msg;
        zmq_msg_init_size(&msg, sizeof(T));
        memcpy(zmq_msg_data(&msg), &data, sizeof(T));
        zmq_msg_send(&msg, socket, 0);
    }
    template<typename T>
    void zmq_recv_struct(void* socket, T& data) {
        zmq_msg_t msg;
        zmq_msg_init(&msg);
        zmq_msg_recv(&msg, socket, 0);
        memcpy(&data, zmq_msg_data(&msg), sizeof(T));
        zmq_msg_close(&msg);
    }

    class OmniAppRendererBase::Details : public OmniStereo::Delegate {
    public:
        Details(OmniAppRendererBase* self, Configuration* config) {
            self_ = self;
            config_ = config;
            initialized_ = false;
        }
        ~Details() {
            if(initialized_) {
                OpenGLWindow::Destroy(window_);
                zmq_close(socket_broadcast);
            }
        }
        void doInitialize() {
            socket_broadcast = zmq_socket(get_global_zmq_context(), ZMQ_SUB);
            zmq_connect(socket_broadcast, config_->getString("omniapp.broadcast", "inproc://omniapp_broadcast").c_str());
            zmq_setsockopt(socket_broadcast, ZMQ_SUBSCRIBE, "", 0);

            window_ = OpenGLWindow::Create(config_);
            window_->makeContextCurrent();
            omni_ = OmniStereo::Create(config_);
            omni_->setDelegate(this);

            t_last = -1;

            self_->onInitialize();

            initialized_ = true;
        }
        void doTick() {
            FrameHeader header;
            { // Receive state.
                zmq_msg_t msg;
                zmq_msg_init(&msg);
                zmq_msg_recv(&msg, socket_broadcast, 0);
                memcpy(&header, zmq_msg_data(&msg), sizeof(FrameHeader));
                memcpy(self_->getState(), ((FrameHeader*)zmq_msg_data(&msg)) + 1, self_->getStateSize());
                zmq_msg_close(&msg);
            }

            { // Update, capture, and composite scene.
                self_->onFrame(header.dt);

                omni_->capture();

                Size2i size = window_->getFramebufferSize();
                omni_->composite(Rectangle2i(0, 0, size.w, size.h));

                glFlush();
            }

            { // Swap buffers.
                window_->swapBuffers();
                window_->pollEvents();
            }
        }
        bool shouldStop() {
            return false;
        }

        void onCaptureViewport(const CaptureInfo& info) {
            self_->onCaptureViewport(info);
        }

        Configuration* config_;
        OpenGLWindow* window_;
        OmniStereo* omni_;
        OmniAppRendererBase* self_;
        double t_last;

        void* socket_broadcast;

        bool initialized_;
    };

    OmniAppRendererBase::OmniAppRendererBase(Configuration* config) {
        details_ = new Details(this, config);
    }

    void OmniAppRendererBase::onInitialize() { }
    void OmniAppRendererBase::onFrame(double dt) { }
    void OmniAppRendererBase::onCaptureViewport(const OmniStereo::Delegate::CaptureInfo& info) { }
    void OmniAppRendererBase::onPresent() { }
    void OmniAppRendererBase::doInitialize() { details_->doInitialize(); }
    void OmniAppRendererBase::doTick() { details_->doTick(); }
    bool OmniAppRendererBase::shouldStop() { return details_->shouldStop(); }

    OmniStereo* OmniAppRendererBase::omni() { return details_->omni_; }
    OmniAppRendererBase::~OmniAppRendererBase() {
        delete details_;
    }

    class OmniAppSimulatorBase::Details {
    public:
        Details(OmniAppSimulatorBase* self, Configuration* config) : fps(60) {
            self_ = self;
            config_ = config;
            initialized_ = false;
        }
        void doInitialize() {
            socket_broadcast = zmq_socket(get_global_zmq_context(), ZMQ_PUB);
            zmq_bind(socket_broadcast, config_->getString("omniapp.broadcast", "inproc://omniapp_broadcast").c_str());

            t_last = -1;
            frame_id = 0;
            self_->onInitialize();
            initialized_ = true;
        }
        void doTick() {
            // Call onFrame.
            double t = get_time_seconds();
            if(t_last == -1) t_last = t - 1.0 / 60;
            double dt = t - t_last;
            self_->onFrame(dt);
            t_last = t;

            // Send control message.
            FrameHeader header;
            header.frame_id = frame_id;
            header.dt = dt;

            zmq_msg_t msg;
            zmq_msg_init_size(&msg, self_->getStateSize() + sizeof(FrameHeader));
            memcpy(zmq_msg_data(&msg), &header, sizeof(FrameHeader));
            memcpy(((FrameHeader*)zmq_msg_data(&msg)) + 1, self_->getState(), self_->getStateSize());
            zmq_msg_send(&msg, socket_broadcast, 0);

            frame_id += 1;
            fps.tick();
        }
        bool shouldStop() {
            return false;
        }
        Configuration* config_;
        OmniAppSimulatorBase* self_;
        double t_last;
        int frame_id;

        void* socket_broadcast;

        FPSControl fps;

        bool initialized_;
    };

    OmniAppSimulatorBase::OmniAppSimulatorBase(Configuration* config) {
        details_ = new Details(this, config);
    }
    void OmniAppSimulatorBase::onInitialize() { }
    void OmniAppSimulatorBase::onFrame(double dt) { }
    void OmniAppSimulatorBase::doInitialize() { details_->doInitialize(); }
    void OmniAppSimulatorBase::doTick() { details_->doTick(); }
    bool OmniAppSimulatorBase::shouldStop() { return details_->shouldStop(); }
    OmniAppSimulatorBase::~OmniAppSimulatorBase() {
        delete details_;
    }

    class OmniAppRunnerBase::Details {
    public:
        Details(OmniAppRunnerBase* self, int argc, char* argv[]) {
            self_ = self;
            config_ = Configuration::ParseMainArgs(argc, argv);
            simulator_ = nullptr;
            renderer_ = nullptr;
        }
        ~Details() {
            if(simulator_) {
                self_->destroySimulator(simulator_);
            }
            if(renderer_) {
                self_->destroyRenderer(renderer_);
            }
        }

        static void* simulator_thread(void* ptr) {
            Details* self = (Details*)ptr;
            self->simulator_ = self->self_->createSimulator(self->config_);
            self->simulator_->doInitialize();
            while(1) {
                self->simulator_->doTick();
                if(self->simulator_->shouldStop()) {
                    break;
                }
            }
            return nullptr;
        }

        static void* renderer_thread(void* ptr) {
            Details* self = (Details*)ptr;
            self->renderer_ = self->self_->createRenderer(self->config_);
            self->renderer_->doInitialize();
            while(1) {
                self->renderer_->doTick();
                if(self->renderer_->shouldStop()) {
                    break;
                }
            }
            return nullptr;
        }

        void run() {
            get_global_zmq_context();
            std::string role = config_->getString("omniapp.role", "both");
            pthread_t thread_s;
            bool run_simulator = (role == "simulator" || role == "both");
            bool run_renderer = (role == "renderer" || role == "both");
            if(run_simulator) {
                pthread_create(&thread_s, 0, simulator_thread, this);
            }
            if(run_renderer) {
                renderer_thread(this);
            }
            if(run_simulator) {
                pthread_join(thread_s, 0);
            }
        }

        OmniAppRendererBase* renderer_;
        OmniAppSimulatorBase* simulator_;
        Configuration* config_;
        OmniAppRunnerBase* self_;
    };

    OmniAppRunnerBase::OmniAppRunnerBase(int argc, char* argv[]) {
        details_ = new Details(this, argc, argv);
    }

    OmniAppRunnerBase::~OmniAppRunnerBase() {
        delete details_;
    }

    void OmniAppRunnerBase::run() {
        details_->run();
    }

}

ALLOFW_NS_END
