#include "camera.h"

#include <glm/gtx/norm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace kekw;

camera::camera() : spatial() { this->is_mirror_ = true; }

mat4_ret_t camera::get_projection() const {
    as_non_const(this)->recalculate_if_dirty();
    return this->projection_;
}

mat4_ret_t camera::get_view() const { return this->mat(); }

void camera::on_recalculate() {
    this->projection_ = glm::perspective(
        this->field_of_view_,
        this->aspect_ratio_,
        this->clip_plane_.x,
        this->clip_plane_.y);
}

void camera::look_at(vec3_param_t target) {
    // https://stackoverflow.com/questions/12435671/quaternion-lookat-function

    // for non-camera transforms x/y probably not flipped.
    vec3 direction = glm::normalize(
        target - glm::vec3(-this->position().x, -this->position().y, this->position().z));
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