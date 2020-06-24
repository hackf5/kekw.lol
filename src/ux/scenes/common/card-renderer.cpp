#include "card-renderer.h"

#include <inc/glad/glad.h>
#include <GLFW/glfw3.h>

using namespace kekw;

card_renderer::~card_renderer() {
    glDeleteVertexArrays(1, &(this->vao_));
    glDeleteBuffers(1, &(this->vbo_));
    glDeleteBuffers(1, &(this->ebo_));
}

void card_renderer::initialize() {
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
            this->width() / 2.0f,   this->height() / 2.0f, 0.0f,   // top right
            this->width() / 2.0f,  -this->height() / 2.0f, 0.0f,   // bottom right
            -this->width() / 2.0f, -this->height() / 2.0f, 0.0f,   // bottom left
            -this->width() / 2.0f,  this->height() / 2.0f, 0.0f    // top left 
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // unbind gpu memory
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void card_renderer::render(const entity* entity) {
    auto shader = this->get_shader();
    shader->use();
    shader->set("model", entity->abs_matrix());

    glBindVertexArray(this->vao_);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}