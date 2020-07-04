#pragma once

#include <src/ux/contexts.h>

#include <src/world/types.h>
#include <src/world/camera.h>
#include <src/world/entity.h>
#include <src/world/scene.h>

#include <memory>

namespace kekw {

class recruit_scene : public scene {
   public:
    recruit_scene(std::unique_ptr<camera> cam, std::unique_ptr<entity> root)
        : scene(std::move(cam), std::move(root)) {}

    void on_initialize(initialize_context* context) override;

    void on_update(update_context* context) override;

    void on_late_update(update_context* context) override;

    void on_render(render_context* context) override;

   private:
    std::unique_ptr<scene> scene_;
    mat4 projection_;
    mat4 view_;
};

}  // namespace kekw