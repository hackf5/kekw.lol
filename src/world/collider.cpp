#include "collider.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/intersect.hpp>

#include <limits>

using namespace kekw::world;

bool mesh_collider::hit_test(
    vec3_param_t origin_w, vec3_param_t direction_w, real_t& distance) {
    distance = std::numeric_limits<real_t>::infinity();
    auto distance_t = std::numeric_limits<real_t>::infinity();

    bool hit_return = false;
    for (auto it = this->mesh()->begin(); it != this->mesh()->end(); ++it) {
        // it would obviously be better to cache the transforms, but since
        // there will be such a small number of entities, recalculating
        // on every frame is fine.
        const auto [p0, p1, p2] = it->transform(this->spatial()->abs_mat());

        glm::vec2 _;
        bool hit =
            glm::intersectRayTriangle(origin_w, direction_w, p0, p1, p2, _, distance_t);

        if (!hit) {
            continue;
        }

        hit_return = true;
        if (distance_t < distance) {
            distance = distance_t;
        }
    }

    return hit_return;
}