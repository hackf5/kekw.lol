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

    glGenVertexArrays(1, &(this->vao_));
    glBindVertexArray(this->vao_);

    glGenBuffers(1, &(this->vbo_));
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo_);

    glVertexAttribPointer(
        0,  // attribute. No particular reason for 0, but must match the layout in the
            // shader.
        3,         // size
        GL_FLOAT,  // type
        GL_FALSE,  // normalized?
        0,         // stride
        (void *)0  // array buffer offset
    );
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void recruit_window_layer::render(window_info *info) {
    this->shader_->use();

    auto aspect_ratio = (float)info->window_width() / (float)info->window_height();
    auto cam = kekw::world::camera();
    cam.set_position(glm::vec3(6.f, 6.f, -1.f));
    cam.set_field_of_view(glm::radians(45.f));
    cam.set_aspect_ratio(aspect_ratio);
    cam.set_clip_plane(glm::vec2(0.1, 100.f));
    cam.set_viewport(glm::vec4(0, 0, info->window_width(), info->window_height()));
    cam.look_at(glm::vec3(2.f, 2.f, -10.0f));

    this->shader_->set("projection", cam.get_projection());
    this->shader_->set("view", cam.get_view());

    auto mouse_x = info->mouse_x();
    auto mouse_y = cam.get_viewport().w - info->mouse_y();
    auto mouse_screen = glm::vec2(mouse_x, mouse_y);
    auto v0 = cam.to_world_coords(glm::vec3(mouse_screen, 0.f));
    auto v1 = cam.to_world_coords(glm::vec3(mouse_screen, 1.f));

    auto v0_inv = cam.to_screen_coords(v0);
    auto v0_inv_inv = cam.to_world_coords(v0_inv);
    auto direction = glm::normalize(v1 - v0);

    // this is wrong, the origin should be the camera position, but it is flipped.
    auto origin_w = -cam.position();

    auto av = this->recruit_env_->available_view();
    float left = -(av->size() * (card_width + margin) - margin) / 2.0f;
    float distance;
    for (auto it = av->begin(); it != av->end(); ++it, left += card_width + margin) {
        auto card = card_instance(it->get(), this->card_body_.get());
        card.shift(glm::vec3(left, 0.f, -10.0f));

        if (card.hit_test(origin_w, direction, distance)) {
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
    info->debug_1 += fmt::format("\nray world: {0}", glm::to_string(direction));
    info->debug_1 +=
        fmt::format("\nray: {0}, {1}", glm::to_string(v0), glm::to_string(v1));
    info->debug_1 += fmt::format(
        "\nv0, v0_inv_inv: {0}, {1}", glm::to_string(v0), glm::to_string(v0_inv_inv));
}