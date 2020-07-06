#include "drag-data.h"

#include <inc/glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/type_ptr.hpp>

#include <cmath>

using namespace kekw;

drag_data::~drag_data() {}

tween_drag_data::tween_drag_data(tweeny::tween<float, float, float> tween)
    : last_offset_time_(glfwGetTime()), tween_(tween) {}

vec3 tween_drag_data::offset() {
    auto time = glfwGetTime();
    float delta_prog = glm::clamp(
        0.0, 1.0, (time - this->last_offset_time_) * 1000.0 / this->tween_.duration());
    this->last_offset_time_ = time;

    return glm::make_vec3(this->tween_.step(delta_prog, false).data());
}