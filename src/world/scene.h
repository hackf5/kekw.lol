#pragma once

#include <src/world/camera.h>
#include <src/world/entity.h>

#include <memory>

namespace kekw {
namespace world {

class scene {
   public:
    scene(std::unique_ptr<camera> cam, std::unique_ptr<entity> root)
        : cam_(std::move(cam)), root_(std::move(root)) {}

    virtual ~scene() {}

    inline camera* cam() { return this->cam_.get(); }
    inline entity* root() { return this->root_.get(); }

   private:
    std::unique_ptr<camera> cam_;
    std::unique_ptr<entity> root_;
};

}  // namespace world
}  // namespace kekw
