#pragma once

#include "window-manager.h"

#include <src/world/scene.h>

#include <memory>

namespace kekw {
namespace ux {
namespace view {

class scene_window_layer : public window_layer {
   public:
    void initialize(window_info *info) override;
    void update(window_info *info) override;
    void render(window_info *info) override;

   private:
    std::unique_ptr<kekw::world::scene> scene_;
};

}  // namespace view
}  // namespace ux
}  // namespace kekw