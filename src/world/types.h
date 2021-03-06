#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <boost/call_traits.hpp>

#include <type_traits>
#include <tuple>

namespace kekw {

typedef float real_t;
typedef unsigned int entity_id_t;

typedef glm::vec2 vec2;
typedef boost::call_traits<vec2>::param_type vec2_param_t;
typedef boost::call_traits<vec2>::const_reference vec2_ret_t;

typedef glm::vec3 vec3;
typedef boost::call_traits<vec3>::param_type vec3_param_t;
typedef boost::call_traits<vec3>::const_reference vec3_ret_t;

typedef glm::vec4 vec4;
typedef boost::call_traits<vec4>::param_type vec4_param_t;
typedef boost::call_traits<vec4>::const_reference vec4_ret_t;

typedef glm::mat4 mat4;
typedef boost::call_traits<mat4>::param_type mat4_param_t;
typedef boost::call_traits<mat4>::const_reference mat4_ret_t;

typedef glm::quat quat;
typedef boost::call_traits<quat>::param_type quat_param_t;
typedef boost::call_traits<quat>::const_reference quat_ret_t;

struct triangle {
    triangle() : v0(), v1(), v2() {}

    triangle(vec3_param_t v0, vec3_param_t v1, vec3_param_t v2)
        : v0(v0), v1(v1), v2(v2) {}

    const vec3 v0;
    const vec3 v1;
    const vec3 v2;

    triangle transform(mat4_param_t model) const {
        auto tv = [](mat4_param_t m, vec3_param_t v) {
            return (m * glm::vec4(v, 1.f)).xyz();
        };

        return triangle(tv(model, v0), tv(model, v1), tv(model, v2));
    }
};

struct entity_id {
    static entity_id_t next() {
        static entity_id_t id = 0;
        return ++id;
    }
};

template <typename T>
inline typename std::remove_const<T>::type *as_non_const(T *p) {
    return const_cast<typename std::remove_const<T>::type *>(p);
}

}  // namespace kekw
