#pragma once

#include <src/world/camera.h>
#include <memory>

namespace kekw {
namespace world {

class scene {
   public:
    camera* cam() { return this->camera_.get(); }

   private:
    std::unique_ptr<camera> camera_;
};

}  // namespace world
}  // namespace kekw
