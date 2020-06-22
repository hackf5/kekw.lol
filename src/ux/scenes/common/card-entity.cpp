#include "card-entity.h"

#include <glm/glm.hpp>

using namespace kekw::ux::scenes;

const box_mesh_2d card_entity::MESH = box_mesh_2d(1, 88.0f / 62.0f);

box_mesh_2d::box_mesh_2d(real_t width, real_t height)
    : width_(width), height_(height), vertexes_() {
    mat4 scale = glm::scale(glm::identity<mat4>(), vec3(width_, height_, 1.0f));

    auto make_vertex = [&scale](unsigned int index) {
        return scale * vec4(
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