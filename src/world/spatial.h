#pragma once

#include "transform.h"

#include <memory>

namespace kekw {
namespace world {

class spatial : public transform {
   public:
    spatial();
    spatial(spatial* parent);

    inline mat4 absolute_matrix() const {
        auto result = this->matrix();
        if (this->parent()) {
            result = this->parent()->absolute_matrix() * result;
        }

        return result;
    }

    spatial* root() const;
    spatial* parent() const;

    void set_parent(spatial* parent);

   private:
    spatial* parent_;
};

}  // namespace world
}  // namespace kekw