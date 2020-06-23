#include "spatial.h"

using namespace kekw;

spatial::spatial() : transform(), parent_(nullptr) {}

spatial* spatial::root() const {
    if (this->parent_) {
        return this->parent_->root();
    }

    return as_non_const(this);
}

mat4 spatial::abs_matrix() const {
    auto result = this->matrix();
    if (this->parent() != nullptr) {
        result = this->parent()->abs_matrix() * result;
    }

    return result;
}