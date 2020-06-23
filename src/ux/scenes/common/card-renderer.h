#pragma once

#include <src/world/types.h>
#include <src/ux/shader/shader.h>
#include <src/world/entity.h>
#include <src/world/renderer.h>

namespace kekw {

class card_renderer : public renderer {
   public:
    card_renderer(real_t width, real_t height) : width_(width), height_(height) {}

    ~card_renderer();

    void initialize();

    real_t width() const { return this->width_; }

    real_t height() const { return this->height_; }

    inline const shader* get_shader() const override {
        return kekw::get_shader("default");
    }

    void render(const entity* entity) override;

   private:
    GLuint vbo_, vao_, ebo_;
    real_t width_, height_;
};

}  // namespace kekw