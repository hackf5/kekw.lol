#include "recruit-window-layer.h"

#include <inc/glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "world/world.hpp"
#include <src/world/camera.h>

using namespace kekw::ux::view;

recruit_window_layer::recruit_window_layer(
    std::shared_ptr<kekw::mod::recruit_env> recruit_env, kekw::world::camera *camera)
    : shader_(new shader()), recruit_env_(recruit_env), camera_(camera) {}

recruit_window_layer::~recruit_window_layer() {}

void recruit_window_layer::initialize(window_info *info) {
    // compile shader
    this->shader_->add_stage_file(GL_VERTEX_SHADER, "glsl/card.vert");
    this->shader_->add_stage_file(GL_FRAGMENT_SHADER, "glsl/card.frag");
    this->shader_->compile();

    this->card_body_ = std::shared_ptr<card_body>(new card_body(this->shader_));
    this->card_body_->initialize();
}

void recruit_window_layer::render(window_info *info) {
    this->shader_->use();

    this->camera_->set_aspect_ratio(info->window_width() / info->window_height());
    this->camera_->set_viewport(
        glm::vec4(0.f, 0.f, info->window_width(), info->window_height()));

    this->camera_->look_at(glm::vec4(0.f, 0.f, -10.0f, 1.f));

    this->shader_->set("projection", this->camera_->get_projection());
    this->shader_->set("view", this->camera_->get_view());

    auto mouse_x = info->mouse_x();
    auto mouse_y = this->camera_->get_viewport().w - info->mouse_y();
    auto mouse_screen = glm::vec2(mouse_x, mouse_y);
    auto v0 = this->camera_->to_world_coords(glm::vec3(mouse_screen, 0.f));
    auto v1 = this->camera_->to_world_coords(glm::vec3(mouse_screen, 1.f));

    auto direction = glm::normalize(v1 - v0);

    auto av = this->recruit_env_->available_view();
    float left = -(av->size() * (card_width + margin) - margin) / 2.0f;
    float distance;
    for (auto it = av->begin(); it != av->end(); ++it, left += card_width + margin) {
        auto card = card_instance(it->get(), this->card_body_.get());
        card.shift(glm::vec3(left, 0.f, -10.0f));

        if (card.hit_test(this->camera_->position(), direction, distance)) {
            this->shader_->set("highlight", false);
        } else {
            this->shader_->set("highlight", true);
        }

        card.render();
    }
}