#include "spatial.h"

using namespace kekw;

spatial::spatial() : transform(), parent_(nullptr) {}
spatial::spatial(spatial* parent) : transform(), parent_(parent) {}

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

void spatial::push_scale_all(vec3_param_t value) {
    this->push_scale(value);
    if (this->parent() != nullptr) {
        this->parent()->push_scale_all(value);
    }
}

void spatial::pop_scale_all() {
    this->pop_scale();
    if (this->parent() != nullptr) {
        this->parent()->pop_scale();
    }
}