#include "camera.h"

using namespace kekw::world;

camera::camera() : spatial() {}

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