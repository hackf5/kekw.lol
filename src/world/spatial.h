#pragma once

#include "transform.h"

#include <memory>

namespace kekw {

class spatial : public transform {
   public:
    spatial();

    spatial* root() const;

    inline spatial* parent() const { return this->parent_; }

    inline void set_parent(spatial* parent) { this->parent_ = parent; }

    mat4 abs_matrix() const;

   private:
    spatial* parent_;
};

}  // namespace kekw