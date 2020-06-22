#pragma once

#include <src/ux/shader/shader.h>
#include <src/ux/contexts.h>
#include <src/world/entity.h>
#include <src/world/collider.h>
#include <src/world/renderer.h>

#include <memory>
#include <vector>

using namespace kekw::world;

namespace kekw {
namespace ux {
namespace scenes {

class box_mesh_2d : public mesh {
   public:
    box_mesh_2d(real_t width, real_t height);

    inline real_t width() const { return this->width_; }
    inline real_t height() const { return this->height_; }

    inline const triangle* begin() const override { return this->vertexes_.data(); }
    inline const triangle* end() const override {
        return this->begin() + this->vertexes_.size();
    }

   private:
    // clang-format off
    static const constexpr real_t UNIFORM_VERTEXES[] = {
        0.5,   0.5, 0,
        0.5,  -0.5, 0,
        -0.5, -0.5, 0,
        -0.5,  0.5, 0,
    };

    static const constexpr unsigned int UNIFORM_INDEXES[] {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    // clang-format on

    real_t width_, height_;
    std::vector<triangle> vertexes_;
};

class card_entity : public entity {
   public:
    card_entity()
        : entity(),
          collider_(std::make_unique<mesh_collider>(this, &MESH)),
          is_hit_(false) {}

    void on_initialize(initialize_context* context) override {
        this->renderer_ = context->locate_service("card_renderer");
        this->set_position(glm::vec3(0, 0.f, -10.0f));
    }

    void on_update(update_context* context) override {
        real_t distance;
        this->is_hit_ = this->collider_->hit_test(
            context->scene()->cam()->position(), context->get_mouse_ray(), distance);
    }

    void on_render(render_context* context) override {
        auto r = this->get_renderer();
        auto shader = r->get_shader();
        shader->use();
        shader->set("highlight", this->is_hit_);
        r->render(this);
    }

   private:
    static const box_mesh_2d MESH;

    inline kekw::world::renderer* get_renderer() {
        return static_cast<kekw::world::renderer*>(this->renderer_.get());
    }

    std::unique_ptr<collider> collider_;
    std::shared_ptr<void> renderer_;
    bool is_hit_;
};

}  // namespace scenes
}  // namespace ux
}  // namespace kekw