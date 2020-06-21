#pragma once

#include <src/world/camera.h>
#include <src/world/entity.h>
#include <src/world/scene.h>

#include <memory>

namespace kekw {
namespace ux {
namespace scenes {

class recruit_scene : public kekw::world::scene {
   public:
    recruit_scene(
        std::unique_ptr<kekw::world::camera> cam,
        std::unique_ptr<kekw::world::entity> root)
        : scene(std::move(cam), std::move(root)) {}

   private:
    std::unique_ptr<kekw::world::scene> scene_;
};

}  // namespace scenes
}  // namespace ux
}  // namespace kekw