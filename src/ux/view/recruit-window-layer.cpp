#include "recruit-window-layer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace kekw::ux::view;

recruit_window_layer::recruit_window_layer(
    std::shared_ptr<kekw::mod::recruit_env> recruit_env) {
    this->recruit_env_ = recruit_env;
}

recruit_window_layer::~recruit_window_layer() {
    glDeleteVertexArrays(1, &(this->vao_));
    glDeleteBuffers(1, &(this->vbo_));
    glDeleteBuffers(1, &(this->ebo_));
}

void recruit_window_layer::initialize(window_info *info) {
    // compile shader
    this->shader_.add_stage_file(GL_VERTEX_SHADER, "glsl/card.vert");
    this->shader_.add_stage_file(GL_FRAGMENT_SHADER, "glsl/card.frag");
    this->shader_.compile();

    // allocate + bind gpu memory
    glGenVertexArrays(1, &(this->vao_));
    glBindVertexArray(this->vao_);

    glGenBuffers(1, &(this->vbo_));
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo_);

    glGenBuffers(1, &this->ebo_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo_);

    // populate buffers
    // clang-format off
    float vertices[] = {
        // positions
        card_width,    card_height, 0.0f,   // top right
        card_width,    0.0f,        0.0f,   // bottom right
        0.0f,          0.0f,        0.0f,   // bottom left
        0.0f,          card_height, 0.0f    // top left 
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // clang-format on

    // clang-format off
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    // clang-format on

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // set attributes
    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // unbind gpu memory
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void recruit_window_layer::render(window_info *info) {
    this->shader_.use();

    glBindVertexArray(this->vao_);
    auto scale_factor = (float)info->window_width() / (float)info->window_height();

    auto projection = glm::perspective(glm::radians(45.0f), scale_factor, 0.1f, 10.0f);
    this->shader_.set("projection", projection);

    auto view = glm::mat4(1.0f);
    this->shader_.set("view", view);

    glm::vec4 bounds = glm::vec4(0, 0, info->window_width(), info->window_height());

    auto av = this->recruit_env_->available_view();
    float left = -(av->size() * (card_width + margin) - margin) / 2.0f;

    std::vector<glm::vec3> card_pos;
    for (auto it = av->begin(); it != av->end(); ++it, left += card_width + margin) {
        auto model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(left, -card_width / 2.0f, -8.0f));
        this->shader_.set("model", model);

        // card_pos.push_back(glm::project())

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    float win_x = info->mouse_x();
    float win_y = info->window_height() - info->mouse_y() - 1;
    float win_z;
    glReadPixels(win_x, win_y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &win_z);

    glm::vec3 mouse = glm::vec3(win_x, win_y, win_z);
    auto unproj = glm::unProject(mouse, view, projection, bounds);
    info->debug_1 =
        fmt::format("({0:.3f}, {1:.3f}), depth: {2:.3f}", unproj.x, unproj.y, win_z);

    glBindVertexArray(0);
}