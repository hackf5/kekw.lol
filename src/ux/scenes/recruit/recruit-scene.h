#pragma once

#include <inc/glad/glad.h>
#include <GLFW/glfw3.h>

#include <src/ux/shader/shader.h>
#include <src/ux/contexts.h>

#include <src/world/camera.h>
#include <src/world/entity.h>
#include <src/world/scene.h>
#include <src/world/renderer.h>

#include <memory>

namespace kekw {

class card_renderer : public renderer {
   public:
    card_renderer(real_t width, real_t height) : width_(width), height_(height) {}

    ~card_renderer() {
        glDeleteVertexArrays(1, &(this->vao_));
        glDeleteBuffers(1, &(this->vbo_));
        glDeleteBuffers(1, &(this->ebo_));
    }

    void initialize() {
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

    real_t width() const { return this->width_; }

    real_t height() const { return this->height_; }

    inline const shader* get_shader() const override {
        return kekw::get_shader("default");
    }

    void render(const entity* entity) override {
        auto shader = this->get_shader();
        shader->use();
        shader->set("model", entity->matrix());

        glBindVertexArray(this->vao_);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

   private:
    GLuint vbo_, vao_, ebo_;
    real_t width_, height_;
};

class recruit_scene : public scene {
   public:
    recruit_scene(std::unique_ptr<camera> cam, std::unique_ptr<entity> root)
        : scene(std::move(cam), std::move(root)) {}

    void on_initialize(initialize_context* context) override {
        auto shader = register_shader("default");
        shader->add_stage_file(GL_VERTEX_SHADER, "glsl/default.vert");
        shader->add_stage_file(GL_FRAGMENT_SHADER, "glsl/card.frag");
        shader->compile();
        shader->use();

        auto cr = std::make_shared<card_renderer>(1, 88.0f / 62.0f);
        cr->initialize();
        context->register_service("card_renderer", cr);

        this->root()->on_initialize(context);
    };

    void on_update(update_context* context) override {
        auto window_ctx = context->window_ctx();
        this->cam()->set_aspect_ratio(
            window_ctx->window_width() / window_ctx->window_height());
        this->cam()->set_viewport(
            glm::vec4(0.f, 0.f, window_ctx->window_width(), window_ctx->window_height()));
        this->cam()->look_at(glm::vec4(0.f, 0.f, -10.0f, 1.f));

        // the camera transforms need to be stored immediately after look_at as the camera
        // could move during update.
        this->projection_ = this->cam()->get_projection();
        this->view_ = this->cam()->get_view();

        auto mouse_x = window_ctx->mouse_x();
        auto mouse_y = this->cam()->get_viewport().w - window_ctx->mouse_y();
        auto mouse_screen = glm::vec2(mouse_x, mouse_y);
        auto v0 = this->cam()->to_world_coords(glm::vec3(mouse_screen, 0.f));
        auto v1 = this->cam()->to_world_coords(glm::vec3(mouse_screen, 1.f));
        context->set_mouse_ray(glm::normalize(v1 - v0));

        this->root()->on_update(context);
    };

    void on_render(render_context* context) override {
        auto shader = get_shader("default");
        shader->use();
        shader->set("projection", this->projection_);
        shader->set("view", this->view_);

        this->root()->on_render(context);
    };

   private:
    std::unique_ptr<scene> scene_;
    mat4 projection_;
    mat4 view_;
};

}  // namespace kekw