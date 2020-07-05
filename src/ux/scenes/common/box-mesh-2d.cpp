#include "box-mesh-2d.h"

using namespace kekw;

box_mesh_2d::box_mesh_2d(real_t width, real_t height, vec2 offset)
    : width_(width), height_(height), offset_(vec3(offset, 0.f)), vertexes_() {
    mat4 transform = glm::translate(glm::identity<mat4>(), this->offset_);
    transform = glm::scale(transform, vec3(this->width_, this->height_, 1.0f));

    auto make_vertex = [&transform](unsigned int index) {
        return transform * vec4(
                               UNIFORM_VERTEXES[index],
                               UNIFORM_VERTEXES[index + 1],
                               UNIFORM_VERTEXES[index + 2],
                               1.f);
    };

    unsigned int triangle_index = 0;
    for (auto i = UNIFORM_INDEXES;
         i < UNIFORM_INDEXES + (sizeof(UNIFORM_INDEXES) / sizeof(unsigned int));
         i += 3) {
        auto i0 = *(i + 0);
        auto i1 = *(i + 1);
        auto i2 = *(i + 2);

        this->vertexes_.emplace_back(
            make_vertex(i0 * 3), make_vertex(i1 * 3), make_vertex(i2 * 3));
    }
}