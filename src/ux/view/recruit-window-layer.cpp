#include "recruit-window-layer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "world/world.hpp"

using namespace kekw::ux::view;

recruit_window_layer::recruit_window_layer(
    std::shared_ptr<kekw::mod::recruit_env> recruit_env)
    : shader_(new shader()), recruit_env_(recruit_env) {}

recruit_window_layer::~recruit_window_layer() {
    glDeleteVertexArrays(1, &(this->vao_));
    glDeleteBuffers(1, &(this->vbo_));
    glDeleteBuffers(1, &(this->ebo_));
}

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

    auto scale_factor = (float)info->window_width() / (float)info->window_height();
    auto cam = fixed_camera(scale_factor);
    cam.set_clip_plane(glm::vec2(10.f, 20.f));
    cam.viewport() = glm::vec4(0, 0, info->window_width(), info->window_height());

    auto projection = cam.get_projection();
    auto view = cam.get_view();
    this->shader_->set("projection", cam.get_projection());
    this->shader_->set("view", cam.get_view());

    glm::vec4 bounds = glm::vec4(0, 0, info->window_width(), info->window_height());

    auto av = this->recruit_env_->available_view();
    float left = -(av->size() * (card_width + margin) - margin) / 2.0f;

    float mouse_x = (2.f * info->mouse_x()) / info->window_width() - 1.f;
    float mouse_y = 1.f - (2.f * info->mouse_y()) / info->window_height();
    float mouse_z = 1.f;
    auto ray_nds = glm::vec3(mouse_x, mouse_y, mouse_z);
    auto ray_clip = glm::vec4(ray_nds.xy(), -1.0, 1.0);
    glm::vec4 ray_eye = glm::inverse(cam.get_projection()) * ray_clip;
    ray_eye = glm::vec4(ray_eye.xy(), -1.f, 0.f);
    glm::vec3 ray_world = (glm::inverse(cam.get_view()) * ray_eye).xyz();
    ray_world = glm::normalize(ray_world);

    float distance;
    for (auto it = av->begin(); it != av->end(); ++it, left += card_width + margin) {
        auto card = card_instance(it->get(), this->card_body_.get());
        card.shift(glm::vec3(left, 0.f, -11.0f));

        if (card.hit_test(cam.get_position(), ray_world, distance)) {
            this->shader_->set("highlight", false);
        } else {
            this->shader_->set("highlight", true);
        }

        card.render();
    }

    float win_x = info->mouse_x();
    float win_y = info->window_height() - info->mouse_y();
    float win_z;
    glReadPixels(win_x, win_y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &win_z);

    auto mouse = glm::vec3(win_x, win_y, win_z);
    auto unproj = cam.to_world_coords(mouse);
    info->debug_1 = fmt::format(
        "({0:.3f}, {1:.3f}), depth: {2:.3f}", unproj.x, unproj.y, 1.0f - win_z);
    info->debug_1 += fmt::format("\nray world: {0}", glm::to_string(ray_world));
}