#pragma once

#include "types.h"

#include <glm/gtx/intersect.hpp>

namespace kekw {
class plane {
   public:
    plane(vec3_param_t p0, vec3_param_t p1, vec3_param_t p2)
        : origin_(p0), normal_(glm::normalize(glm::cross(p1 - p0, p2 - p0))) {}

    inline vec3_ret_t origin() const { return this->origin_; }
    inline vec3_ret_t normal() const { return this->normal_; }

    bool hit_test(vec3_param_t origin, vec3_param_t direction, real_t& distance) const {
        return glm::intersectRayPlane(
            origin, direction, this->origin_, this->normal_, distance);
    }

   private:
    const vec3 origin_;
    const vec3 normal_;
};
}  // namespace kekw