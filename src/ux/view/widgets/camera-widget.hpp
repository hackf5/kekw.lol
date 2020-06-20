#pragma once

#include "imgui/extensions.h"

#include <src/ux/view/ux-window-layer.h>
#include <src/world/camera.h>

#include <src/world/camera.h>

#include <glm/gtc/type_ptr.hpp>

namespace kekw {
namespace ux {
namespace view {
namespace widgets {

using namespace kekw::world;

class camera_widget : public ux_window_widget {
   public:
    camera_widget(world::camera *camera) : camera_(camera) {}

    void initialize(window_context *info) override {
        this->camera_->set_clip_plane(vec2(0.1f, 100.f));
        this->camera_->set_field_of_view(glm::radians(45.f));
        this->camera_->set_aspect_ratio(info->window_width() / info->window_height());
        this->camera_->set_viewport(
            vec4(0.f, 0.f, info->window_width(), info->window_height()));
        this->camera_->set_position(glm::zero<vec3>());
    }

    void update(window_context *info) override {
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
    world::camera *camera_;
};

}  // namespace widgets
}  // namespace view
}  // namespace ux
}  // namespace kekw