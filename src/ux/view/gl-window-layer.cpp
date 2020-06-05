#include "gl-window-layer.h"

kekw::ux::view::GlWindowLayer::~GlWindowLayer() {
    glDeleteVertexArrays(1, &(this->VAO));
    glDeleteBuffers(1, &(this->VBO));
}

void kekw::ux::view::GlWindowLayer::Initialize(WindowInfo *info) {
    float vertices[] = {
        -0.5f,
        -0.5f,
        0.0f,  // left
        0.5f,
        -0.5f,
        0.0f,  // right
        0.0f,
        0.5f,
        0.0f  // top
    };

    glGenVertexArrays(1, &(this->VAO));
    glGenBuffers(1, &(this->VBO));
    // bind the Vertex Array Object first, then bind and set vertex buffer(s),
    // and then configure vertex attributes(s).
    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered
    // VBO as the vertex attribute's bound vertex buffer object so afterwards we
    // can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally
    // modify this VAO, but this rarely happens. Modifying other VAOs requires a
    // call to glBindVertexArray anyways so we generally don't unbind VAOs (nor
    // VBOs) when it's not directly necessary.
    glBindVertexArray(0);
    this->shader_.AddStage(
        GL_VERTEX_SHADER,
        kekw::util::read_file(
            kekw::util::get_absolute_path("glsl/simple.vert").string()));
    this->shader_.AddStage(
        GL_FRAGMENT_SHADER,
        kekw::util::read_file(
            kekw::util::get_absolute_path("glsl/simple.frag").string()));
    this->shader_.Compile();
}

void kekw::ux::view::GlWindowLayer::Render(WindowInfo *info) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    this->shader_.Use();
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}