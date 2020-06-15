#include "cameras.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/intersect.hpp>
#include <glm/gtx/string_cast.hpp>

#include <type_traits>

using namespace kekw::ux::view::world;

template <typename T>
typename std::remove_const<T>::type *remove_const(T *p) {
    return const_cast<typename std::remove_const<T>::type *>(p);
}

camera::~camera() {}

mat4_ret_t camera_base::get_projection() const {
    remove_const(this)->recalculate_if_dirty();
    return this->projection_;
}

vec4_ret_t camera_base::get_viewport() const { return this->viewport_; }
void camera_base::set_viewport(vec4_param_t value) {
    this->make_dirty();
    this->viewport_ = value;
}

float camera_base::get_field_of_view() const { return this->field_of_view_; }
void camera_base::set_field_of_view(float value) {
    this->make_dirty();
    this->field_of_view_ = value;
}

float camera_base::get_aspect_ratio() const { return this->aspect_ratio_; }
void camera_base::set_aspect_ratio(float value) {
    this->make_dirty();
    this->aspect_ratio_ = value;
}

vec2_ret_t camera_base::get_clip_plane() const { return this->clip_plane_; }
void camera_base::set_clip_plane(vec2_param_t value) {
    this->make_dirty();
    this->clip_plane_ = value;
}

vec3 camera_base::to_screen_coords(vec3_param_t world) const {
    return glm::project(
        world, this->get_view(), this->get_projection(), this->get_viewport());
}

vec3 camera_base::to_world_coords(vec3_param_t screen) const {
    return glm::unProject(
        screen, this->get_view(), this->get_projection(), this->get_viewport());
}

std::string camera_base::to_string() { return "{camera}"; }

void camera_base::recalculate_if_dirty() {
    this->on_recalculate();
    this->dirty_ = false;
}

void camera_base::on_recalculate() {
    this->projection_ = glm::perspective(
        this->field_of_view_,
        this->aspect_ratio_,
        this->clip_plane_.x,
        this->clip_plane_.y);
}

void camera_base::make_dirty() { this->dirty_ = true; }
