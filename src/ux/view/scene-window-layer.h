#pragma once

#include "window-manager.h"

#include <src/world/scene.h>

#include <memory>

namespace kekw {
namespace ux {
namespace view {

class scene_window_layer : public window_layer {
   public:
    scene_window_layer(std::shared_ptr<kekw::world::scene> scene) : scene_(scene) {}

    void initialize(window_context *info) override;
    void update(window_context *info) override;
    void render(window_context *info) override;

   private:
    std::shared_ptr<kekw::world::scene> scene_;
};

}  // namespace view
}  // namespace ux
}  // namespace kekw