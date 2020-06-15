#include "transform.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace kekw::world;

const transform transform::identity = transform();

transform::transform()
    : position_(vec3(0.f, 0.f, 0.f)),
      rotation_(glm::identity<quat>()),
      scale_(vec3(1.f, 1.f, 1.f)),
      matrix_(glm::identity<mat4>()),
      dirty_(false) {}

transform::transform(transform const& other)
    : position_(other.position_),
      rotation_(other.rotation_),
      scale_(other.scale_),
      matrix_(other.matrix_),
      dirty_(other.dirty_) {}

mat4_ret_t transform::matrix() const {
    as_non_const(this)->recalculate_if_dirty();
    return this->matrix_;
}

void transform::recalculate_if_dirty() {
    if (!this->dirty_) {
        return;
    }

    this->matrix_ = glm::translate(
        glm::scale(glm::mat4_cast(this->rotation()), this->scale()), this->position());

    this->on_recalculate();

    this->dirty_ = false;
}