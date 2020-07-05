#pragma once

#include "transform.h"

#include <memory>

namespace kekw {

class spatial : public transform {
   public:
    spatial();
    spatial(spatial* parent);

    spatial* root() const;

    inline spatial* parent() const { return this->parent_; }

    inline void set_parent(spatial* parent) { this->parent_ = parent; }

    mat4 abs_matrix() const;

    void push_scale_all(vec3_param_t value);

    void pop_scale_all();

   private:
    spatial* parent_;
};

}  // namespace kekw