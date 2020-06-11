#include "recruit-window-layer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

    const float w = 1;
    const float h = 88.0f / 62.0f;

    // populate buffers
    // clang-format off
    float vertices[] = {
        // positions
         w,  h, 0.0f,  // top right
         w, -h, 0.0f,   // bottom right
        -w, -h, 0.0f,   // bottom left
        -w,  h, 0.0f  // top left 
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

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    auto width = (GLfloat)viewport[2];
    auto height = (GLfloat)viewport[3];
    auto scale_factor = width / height;

    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 projection =
        glm::perspective(glm::radians(45.0f), width / height, 0.1f, 100.0f);

    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -20.0f));

    this->shader_.set("projection", projection);
    this->shader_.set("view", view);
    this->shader_.set("model", model);

    glBindVertexArray(this->vao_);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}