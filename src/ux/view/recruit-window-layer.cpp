#include "recruit-window-layer.h"

#include <algorithm>

using namespace kekw::ux::view;

recruit_window_layer::~recruit_window_layer() {
    glDeleteVertexArrays(1, &(this->vao_));
    glDeleteBuffers(1, &(this->vbo_));
    glDeleteBuffers(1, &(this->ebo_));
    // glDeleteTextures(1, &(this->texture_));
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

    const float golden = 1.61803398875f;

    // populate buffers
    // clang-format off
    float vertices[] = {
        // positions
        1.0f,  golden,    // top right
        1.0f,  -golden,   // bottom right
        -1.0f, -golden,   // bottom left
        -1.0f,  golden,   // top left 
    };
    std::for_each(
        vertices, 
        vertices + sizeof(vertices), 
        [golden](float& value) { value /= 2.0f * golden; });
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
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // unbind gpu memory
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void recruit_window_layer::render(window_info *info) {
    this->shader_.use();
    info->get_window();

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    glBindVertexArray(this->vao_);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}