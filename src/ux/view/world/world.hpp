#pragma once

#include <src/ux/shader/shader.h>
#include <src/mod/card.h>

#include <inc/glad/glad.h>
#include <GLFW/glfw3.h>

#include <fmt/format.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/intersect.hpp>
#include <glm/gtx/string_cast.hpp>

#include <boost/call_traits.hpp>

#include <vector>
#include <memory>
#include <tuple>
#include <limits>
#include <map>

typedef boost::call_traits<glm::vec3>::param_type vec3_param_t;
typedef boost::call_traits<glm::vec3>::const_reference vec3_ret_t;

typedef boost::call_traits<glm::mat4>::param_type mat4_param_t;
typedef boost::call_traits<glm::mat4>::const_reference mat4_ret_t;

typedef std::tuple<glm::vec3, glm::vec3, glm::vec3> triangle_t;

class camera {
   public:
    camera(float aspect_ratio)
        : field_of_view_(glm::radians(45.f)),
          aspect_ratio_(aspect_ratio),
          clip_plane_(0.1f, 100.9f),
          viewport_(),
          projection_(glm::identity<glm::mat4>()),
          dirty_(true) {}

    virtual ~camera() {}

    virtual mat4_ret_t get_view() = 0;

    mat4_ret_t get_projection() {
        if (this->dirty_) {
            this->dirty_ = false;
            this->projection_ = glm::perspective(
                this->field_of_view_,
                this->aspect_ratio_,
                this->clip_plane_.x,
                this->clip_plane_.y);
        }

        return this->projection_;
    }

    glm::vec3 to_screen_coords(vec3_param_t object, mat4_param_t model) {
        return glm::project(
            object, model * this->get_view(), this->get_projection(), this->viewport_);
    }

    glm::vec3 to_world_coords(
        vec3_param_t screen, mat4_param_t model = glm::identity<glm::mat4>()) {
        return glm::unProject(
            screen, model * this->get_view(), this->get_projection(), this->viewport_);
    }

    // needs setting, yuk!
    glm::vec4& viewport() { return this->viewport_; }

    float get_field_of_view() { return this->field_of_view_; }
    void set_field_of_view(float value) {
        this->field_of_view_ = value;
        this->dirty_ = true;
    }

    float get_aspect_ratio() { return this->aspect_ratio_; }
    void set_aspect_ratio(float value) {
        this->aspect_ratio_ = value;
        this->dirty_ = true;
    }

    glm::vec2 get_clip_plane() { return this->clip_plane_; }
    void set_clip_plane(glm::vec2 value) {
        this->clip_plane_ = value;
        this->dirty_ = true;
    }

    virtual std::string to_string() {
        return fmt::format(
            "proj:\n{0}\nview:\n{1}",
            glm::to_string(this->get_projection()),
            glm::to_string(this->get_view()));
    }

   private:
    float field_of_view_;
    float aspect_ratio_;
    glm::vec2 clip_plane_;
    glm::vec4 viewport_;
    glm::mat4 projection_;

    bool dirty_;
};

class body {
   public:
    virtual ~body() {}
    virtual void initialize() = 0;
    virtual std::vector<triangle_t> const& triangles() const = 0;
};

class body_instance {
   public:
    body_instance(body* body)
        : body_(body), translation_(glm::identity<glm::mat4>()), dirty_(true) {}

    ~body_instance() {}

    void set_position(vec3_param_t position) {
        this->translation_ = glm::translate(glm::identity<glm::mat4>(), position);
        this->dirty_ = true;
    }

    void translate(vec3_param_t value) {
        this->translation_ = glm::translate(this->translation_, value);
        this->dirty_ = true;
    }

    std::vector<triangle_t> triangles() {
        if (!this->dirty_) {
            return this->triangles_;
        }
        auto get_vertex = [](mat4_param_t model, vec3_param_t vertex) {
            return (model * glm::vec4(vertex, 1.f)).xyz();
        };

        auto local_triangles = this->body_->triangles();
        this->triangles_.clear();
        for (auto it = local_triangles.begin(); it != local_triangles.end(); ++it) {
            this->triangles_.push_back(std::make_tuple(
                get_vertex(this->translation_, std::get<0>(*it)),
                get_vertex(this->translation_, std::get<1>(*it)),
                get_vertex(this->translation_, std::get<2>(*it))));
        }

        return this->triangles_;
    }

   private:
    body* body_;
    glm::mat4 translation_;
    std::vector<triangle_t> triangles_;
    bool dirty_;
};

class renderer {
    virtual ~renderer() {}
    virtual void initialize() = 0;
    virtual void render(
        kekw::ux::shader const& shader,
        mat4_param_t model = glm::identity<glm::mat4>()) = 0;
};

class scene_object {
   public:
    virtual ~scene_object(){};
    virtual void initialize() = 0;
    virtual void render(kekw::ux::shader const& shader);
};

class scene {
   public:
    virtual ~scene(){};

    virtual void render() {
        // init camera from windo_info.

        // layout scene.

        // user interaction.

        // render the scene.
        this->shader_->use();
        this->shader_->set("projection", this->camera_->get_projection());
        this->shader_->set("view", this->camera_->get_view());
    }

   private:
    std::unique_ptr<camera> camera_;
    std::shared_ptr<kekw::ux::shader> shader_;
    std::map<int, std::unique_ptr<body>> bodies_;
    std::vector<std::unique_ptr<scene_object>> objects_;
};

class fixed_camera : public camera {
   public:
    fixed_camera(
        float aspect_ratio,
        vec3_param_t position = glm::vec3(0.0f, 0.0f, 0.0f),
        vec3_param_t target = glm::vec3(0.0f, 0.0f, -1.0f),
        vec3_param_t up = glm::vec3(0.0f, 1.0f, 0.0f))
        : camera(aspect_ratio),
          position_(position),
          target_(target),
          up_(up),
          dirty_(true) {}

    vec3_ret_t get_position() const { return this->position_; }
    void set_position(vec3_param_t value) {
        this->position_ = value;
        this->dirty_ = true;
    }
    vec3_ret_t get_target() const { return this->target_; }
    void set_target(vec3_param_t value) {
        this->target_ = value;
        this->dirty_ = true;
    }

    vec3_ret_t get_up() const { return this->up_; }

    mat4_ret_t get_view() override {
        if (this->dirty_) {
            this->dirty_ = false;
            this->view_ = glm::lookAt(this->position_, this->target_, this->up_);
        }

        return this->view_;
    }

    glm::vec3 get_direction() const {
        return glm::normalize(this->position_ - this->target_);
    }

   private:
    glm::vec3 position_;
    glm::vec3 target_;
    glm::vec3 up_;
    glm::mat4 view_;

    bool dirty_;
};

class card_body {
   public:
    card_body(std::shared_ptr<kekw::ux::shader> shader) : shader_(shader) {}

    ~card_body() {
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

        auto make_vertex = [&vertices](unsigned int index) {
            return glm::vec3(vertices[index], vertices[index + 1], vertices[index + 2]);
        };

        for (auto i = indices; i < indices + (sizeof(indices) / sizeof(unsigned int));
             i += 3) {
            auto i0 = *(i + 0);
            auto i1 = *(i + 1);
            auto i2 = *(i + 2);

            this->triangles_.push_back(
                {make_vertex(i0 * 3), make_vertex(i1 * 3), make_vertex(i2 * 3)});
        }

        // set attributes
        // position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // unbind gpu memory
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    float width() const { return 1.0f; }

    float height() const { return 88.0f / 62.0f; }

    std::vector<triangle_t> const& triangles() const { return this->triangles_; }

    void render(mat4_param_t model) {
        this->shader_->use();
        this->shader_->set("model", model);

        glBindVertexArray(this->vao_);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

   private:
    std::shared_ptr<kekw::ux::shader> shader_;
    GLuint vbo_, vao_, ebo_;
    std::vector<triangle_t> triangles_;
};

class card_instance {
   public:
    card_instance(kekw::mod::card* card, card_body* card_body)
        : card_(card), card_body_(card_body), translation_(glm::identity<glm::mat4>()) {}

    void set_position(vec3_param_t position) {
        this->translation_ = glm::translate(this->translation_, position);
    }

    void shift(vec3_param_t value) {
        this->translation_ = glm::translate(this->translation_, value);
    }

    void shift_x(float x_delta) {
        this->translation_ =
            glm::translate(this->translation_, glm::vec3(x_delta, 0.f, 0.f));
    }

    void shift_y(float y_delta) {
        this->translation_ =
            glm::translate(this->translation_, glm::vec3(0.f, y_delta, 0.f));
    }

    void shift_z(float z_delta) {
        this->translation_ =
            glm::translate(this->translation_, glm::vec3(0.f, 0.f, z_delta));
    }

    void render() { this->card_body_->render(this->translation_); }

    std::vector<triangle_t> triangles() {
        auto get_vertex = [](mat4_param_t model, vec3_param_t vertex) {
            return (model * glm::vec4(vertex, 1.f)).xyz();
        };

        auto local_triangles = this->card_body_->triangles();
        std::vector<triangle_t> result;
        for (auto it = local_triangles.begin(); it != local_triangles.end(); ++it) {
            result.push_back(std::make_tuple(
                get_vertex(this->translation_, std::get<0>(*it)),
                get_vertex(this->translation_, std::get<1>(*it)),
                get_vertex(this->translation_, std::get<2>(*it))));
        }

        return result;
    }

    bool hit_test(vec3_param_t origin_w, vec3_param_t direction_w, float& distance) {
        distance = std::numeric_limits<float>::infinity();
        auto distance_t = std::numeric_limits<float>::infinity();

        auto tr = this->triangles();
        bool hit_return = false;
        glm::vec2 intersect;
        glm::vec3 p0, p1, p2;
        for (auto it = tr.begin(); it != tr.end(); ++it) {
            std::tie(p0, p1, p2) = *it;

            bool hit = glm::intersectRayTriangle(
                origin_w, direction_w, p0, p1, p2, intersect, distance_t);

            if (!hit) {
                continue;
            }

            hit_return = true;
            if (distance_t < distance) {
                distance = distance_t;
            }
        }

        return hit_return;
    }

   private:
    kekw::mod::card* card_;
    card_body* card_body_;
    glm::mat4 translation_;
};
