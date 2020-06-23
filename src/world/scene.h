#pragma once

#include <src/world/camera.h>
#include <src/world/entity.h>

#include <memory>

namespace kekw {

class scene {
   public:
    scene(std::unique_ptr<camera> cam, std::unique_ptr<entity> root)
        : cam_(std::move(cam)), root_(std::move(root)) {}

    virtual ~scene() {}

    inline camera* cam() { return this->cam_.get(); }
    inline entity* root() { return this->root_.get(); }

    virtual void on_initialize(initialize_context* context) = 0;
    virtual void on_update(update_context* context) = 0;
    virtual void on_render(render_context* context) = 0;

   private:
    std::unique_ptr<camera> cam_;
    std::unique_ptr<entity> root_;
};

}  // namespace kekw
