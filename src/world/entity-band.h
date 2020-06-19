#pragma once

#include "spatial.h"
#include "types.h"

#include <vector>
#include <memory>

namespace kekw {
namespace world {

class entity : public spatial {};

class entity_band : public entity {
   public:
    void add_entity(std::unique_ptr<entity> entity) {
        this->entities_.push_back(std::move(entity));
        entity->set_parent(this);
    }

    void on_update() {
        auto n = this->entities_.size();
        auto left = -this->r_ * ((n + 1) / 2);
        auto shift = vec3(left, 0.f, 0.f);
        for (auto it = this->entities_.begin(); it != this->entities_.end(); ++it) {
            (*it)->set_position(shift);
            shift.x += this->r_;
        }
    }

    real_t r_;

    std::vector<std::unique_ptr<entity>> entities_;
};

}  // namespace world
}  // namespace kekw