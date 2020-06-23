#pragma once

#include "imgui/extensions.h"

#include <src/ux/view/ux-window-layer.h>
#include <src/world/camera.h>

#include <src/world/camera.h>

#include <glm/gtc/type_ptr.hpp>

namespace kekw {

class camera_widget : public ux_window_widget {
   public:
    camera_widget(camera *camera) : camera_(camera) {}

    void initialize(window_context *context) override {}

    void update(window_context *context) override {
        if (!ImGui::Begin("Camera")) {
            ImGui::End();
            return;
        }

        auto p = vec3(this->camera_->position());
        auto position = static_cast<float *>(glm::value_ptr(p));
        if (ImGui::SliderFloat3("Position", position, -10, 10)) {
            this->camera_->set_position(p);
        }

        ImGui::End();
    }

   private:
    camera *camera_;
};

}  // namespace kekw