#include "camera.h"

#include <glm/gtx/norm.hpp>

using namespace kekw::world;

mat4_ret_t camera::get_projection() const {
    as_non_const(this)->recalculate_if_dirty();
    return this->projection_;
}

mat4_ret_t camera::get_view() const { return this->matrix(); }

void camera::on_recalculate() {
    this->projection_ = glm::perspective(
        this->field_of_view_,
        this->aspect_ratio_,
        this->clip_plane_.x,
        this->clip_plane_.y);
}

void camera::lool_at(vec3_param_t target) {
    // https://stackoverflow.com/questions/12435671/quaternion-lookat-function

    static const vec3 forward = vec3(0.f, 0.f, -1.f);
    static const vec3 up = vec3(0.f, 1.f, 0.f);

    vec3 direction = glm::normalize(target - this->position());
    vec3 axis = glm::normalize(glm::cross(forward, direction));
    if (glm::l2Norm(axis) <= 0.000001f) {
        axis = up;
    }

    float dot = glm::dot(forward, target);
    float angle = glm::acos(dot);

    auto s = glm::sin(angle / 2);
    this->set_rotation(quat(glm::cos(angle / 2), s * axis));
    this->set_scale(vec3(1.f, 1.f, 1.f));
}