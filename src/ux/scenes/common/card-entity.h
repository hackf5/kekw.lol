#pragma once

#include <src/world/entity.h>
#include <src/world/collider.h>

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
    card_entity() : collider_(std::make_unique<mesh_collider>(this, &MESH)) {}

   private:
    std::unique_ptr<collider> collider_;
    static const box_mesh_2d MESH;
};

}  // namespace scenes
}  // namespace ux
}  // namespace kekw