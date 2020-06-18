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

void transform::look_at(vec3_param_t target) {
    // https://stackoverflow.com/questions/12435671/quaternion-lookat-function

    vec3 direction = glm::normalize(target - this->position());
    vec3 axis = glm::cross(transform::forward, direction);
    if (glm::l2Norm(axis) <= 0.0001f) {
        // the direction is parallel to the forward axis.
        axis = transform::up;
    }

    auto dot = glm::dot(transform::forward, direction);
    auto angle = glm::acos(dot);

    this->set_rotation(glm::angleAxis(angle, glm::normalize(axis)));
    this->set_scale(vec3(1.f, 1.f, 1.f));
}