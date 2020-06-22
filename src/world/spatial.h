#pragma once

#include "transform.h"

#include <memory>

namespace kekw {
namespace world {

class spatial : public transform {
   public:
    spatial();
    spatial(spatial* parent);

    inline mat4 abs_mat() const {
        auto result = this->mat();
        if (this->parent() != nullptr) {
            result = this->parent()->abs_mat() * result;
        }

        return result;
    }

    spatial* root() const;

    inline spatial* parent() const { return this->parent_; }

    inline void set_parent(spatial* parent) { this->parent_ = parent; }

   private:
    spatial* parent_;
};

}  // namespace world
}  // namespace kekw