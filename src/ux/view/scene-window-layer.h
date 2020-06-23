#pragma once

#include "window-manager.h"

#include <src/ux/contexts.h>
#include <src/world/scene.h>

#include <memory>

namespace kekw {

class scene_window_layer : public window_layer {
   public:
    scene_window_layer(std::shared_ptr<scene> scene) : scene_(scene) {}

    void initialize(window_context *info) override;
    void update(window_context *info) override;
    void render(window_context *info) override;

   private:
    std::shared_ptr<scene> scene_;
    std::unique_ptr<update_context> previous_context_;
};

}  // namespace kekw