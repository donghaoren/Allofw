// liballofw: A lightweight framework for graphics.
// See LICENSE.md for license information.
//
// omnistereo.cpp
//
// Implements Omnistereo rendering class.
//
// Author(s):
//   Donghao Ren (donghaoren@cs.ucsb.edu)
//

#include "allofw/omniapp.h"
#include "allofw/logger.h"
#include "allofw/utils.h"
#include <unistd.h>

ALLOFW_NS_BEGIN

OmniAppBase::OmniAppBase() { }

void OmniAppBase::onInitialize() { }

void OmniAppBase::onFrame(double dt) { }

void OmniAppBase::onPresent() { }

void OmniAppBase::render() {
    // Compute dt.
    double t = get_time_seconds();
    double dt = t - t_previous_frame_;
    t_previous_frame_ = t;
    onFrame(dt);
    Size2i viewport_size = window_->getFramebufferSize();
    OmniStereo::CompositeInfo info;
    Rectangle2i viewport(0, 0, viewport_size.w, viewport_size.h);
    omni_->setPose(pose_);
    omni_->capture();
    omni_->composite(viewport, info);
    onPresent();
    window_->swapBuffers();
}

void OmniAppBase::onCaptureFace(const CaptureInfo& info) { }

// Window handlers.
void OmniAppBase::onMove(int x, int y) { }

void OmniAppBase::onResize(int width, int height) { }

void OmniAppBase::onClose() {
    closing_ = true;
}
void OmniAppBase::onRefresh() {
    render();
}
void OmniAppBase::onFocus(int focused) { }

void OmniAppBase::onIconify(int iconified) { }

void OmniAppBase::onFramebufferSize(int width, int height) {
    render();
}

// Input events.
void OmniAppBase::onKeyboard(const char* c_key, const char* c_action, const char* c_modifiers, int scancode) { }

void OmniAppBase::initialize(Configuration* config) {
    config_ = config;
    window_ = OpenGLWindow::Create(config_);
    window_->makeContextCurrent();
    omni_ = OmniStereo::Create(config_);
    omni_->setDelegate(this);
    window_->setDelegate(this);
    closing_ = false;
    window_->enableKeyboardInput();

    onInitialize();
    t_previous_frame_ = get_time_seconds();
}

// Start the main loop.
void OmniAppBase::main() {
    while(!closing_) {
        tick();
    }
}
void OmniAppBase::tick() {
    if(!closing_) {
        render();
        window_->pollEvents();
    }
}

OmniAppBase::~OmniAppBase() {
    delete omni_;
    delete window_;
}

OmniAppMixin_Navigation::OmniAppMixin_Navigation() {
    translation_speeds_.resize(6);
    translation_speeds_[0] = Vector4(+1, 0, 0, 0);
    translation_speeds_[1] = Vector4(-1, 0, 0, 0);
    translation_speeds_[2] = Vector4(0, +1, 0, 0);
    translation_speeds_[3] = Vector4(0, -1, 0, 0);
    translation_speeds_[4] = Vector4(0, 0, +1, 0);
    translation_speeds_[5] = Vector4(0, 0, -1, 0);
    rotation_speed_ = Vector4(0, 0, 0, 0);
}

void OmniAppMixin_Navigation::onKeyboard(const char* c_key, const char* c_action, const char* c_modifiers, int scancode) {
    std::string key(c_key), action(c_action), modifiers(c_modifiers);

    double* target = nullptr;;
    if(key == "UP") target = &translation_speeds_[5].w;
    if(key == "DOWN") target = &translation_speeds_[4].w;
    if(key == "LEFT") target = &translation_speeds_[1].w;
    if(key == "RIGHT") target = &translation_speeds_[0].w;
    if(key == "Q") target = &translation_speeds_[2].w;
    if(key == "Z") target = &translation_speeds_[3].w;
    if(key == "A") target = &rotation_speed_.x;
    if(key == "D") target = &rotation_speed_.y;
    if(key == "W") target = &rotation_speed_.z;
    if(key == "X") target = &rotation_speed_.w;
    if(target) {
        if(action == "PRESS") *target = 1;
        if(action == "RELEASE") *target = 0;
    }
    if(action == "PRESS") {
        if(key == "GRAVE_ACCENT") {
            pose() = Pose();
        }
    }
}
void OmniAppMixin_Navigation::onFrame(double dt) {
    Vector3 translation_speed(0, 0, 0);
    for(Vector4 v : translation_speeds_) {
        translation_speed += v.xyz() * v.w;
    }
    pose().position += pose().rotation.rotate(translation_speed * dt);

    Quaternion r1 = Quaternion::Rotation(Vector3(0, 1, 0), dt * (rotation_speed_.x - rotation_speed_.y));
    Quaternion r2 = Quaternion::Rotation(Vector3(1, 0, 0), dt * (rotation_speed_.z - rotation_speed_.w));
    pose().rotation = r1 * pose().rotation * r2;
    pose().rotation = pose().rotation.unit();
}

ALLOFW_NS_END
