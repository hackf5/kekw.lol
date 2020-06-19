#include "transform.h"

#include <glm/gtx/norm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace kekw::world;

const transform transform::identity = transform();

const vec3 transform::origin = glm::zero<vec3>();
const vec3 transform::right = vec3(1.f, 0.f, 0.f);
const vec3 transform::up = vec3(0.f, 1.f, 0.f);
const vec3 transform::forward = vec3(0.f, 0.f, -1.f);

transform::transform()
    : position_(transform::origin),
      rotation_(glm::identity<quat>()),
      scale_(glm::one<vec3>()),
      matrix_(glm::identity<mat4>()),
      dirty_(false),
      is_mirror_(false) {}

transform::transform(transform const& other)
    : position_(other.position_),
      rotation_(other.rotation_),
      scale_(other.scale_),
      matrix_(other.matrix_),
      dirty_(other.dirty_),
      is_mirror_(other.is_mirror_) {}

mat4_ret_t transform::matrix() const {
    as_non_const(this)->recalculate_if_dirty();
    return this->matrix_;
}

void transform::recalculate_if_dirty() {
    // if (!this->dirty_) {
    //     return;
    // }

    this->matrix_ = glm::translate(
        glm::scale(glm::mat4_cast(this->rotation()), this->scale()),
        this->is_mirror_ ? -this->position() : this->position());

    this->on_recalculate();

    this->dirty_ = false;
}
