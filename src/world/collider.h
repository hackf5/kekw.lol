#pragma once

#include "types.h"
#include "spatial.h"

#include <vector>

namespace kekw {
namespace world {

class collider {
   public:
    collider(spatial* spatial) : spatial_(spatial) {}
    virtual ~collider() {}

    inline spatial* spa() { return this->spatial_; }

    virtual bool hit_test(
        vec3_param_t origin_w, vec3_param_t direction_w, real_t& distance) = 0;

   private:
    spatial* spatial_;
};

class mesh {
   public:
    typedef std::vector<triangle_t>::const_iterator iterator;

    mesh(triangle_t const* triangles, size_t size) : triangles_() {
        for (auto it = triangles; it != triangles + size; ++it) {
            this->triangles_.push_back(*it);
        }
    }

    virtual ~mesh() {}

    inline iterator begin() const { return this->triangles_.begin(); }
    inline iterator end() const { return this->triangles_.end(); }

   private:
    std::vector<triangle_t> triangles_;
};

class mesh_collider : public collider {
   public:
    mesh_collider(spatial* spatial, mesh* mesh) : collider(spatial), mesh_(mesh) {}

    inline mesh* msh() { return this->mesh_; }

    bool hit_test(
        vec3_param_t origin_w, vec3_param_t direction_w, real_t& distance) override;

   private:
    mesh* mesh_;
};

}  // namespace world
}  // namespace kekw