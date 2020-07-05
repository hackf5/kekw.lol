#pragma once

#include <src/world/types.h>
#include <src/world/collider.h>

#include <vector>

namespace kekw {

class box_mesh_2d : public mesh {
   public:
    box_mesh_2d(real_t width, real_t height, vec2 offset);

    inline real_t width() const { return this->width_; }
    inline real_t height() const { return this->height_; }
    inline vec2 offset() const { return this->offset_.xy(); }

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
    vec3 offset_;
    std::vector<triangle> vertexes_;
};
}  // namespace kekw