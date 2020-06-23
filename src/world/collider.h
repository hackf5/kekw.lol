#pragma once

#include "types.h"
#include "spatial.h"

namespace kekw {

class collider {
   public:
    collider(kekw::spatial* spatial) : spatial_(spatial) {}

    virtual ~collider() = 0;

    inline const spatial* spatial() const { return this->spatial_; }

    virtual bool hit_test(
        vec3_param_t origin_w, vec3_param_t direction_w, real_t& distance) = 0;

   private:
    kekw::spatial* const spatial_;
};

class mesh {
   public:
    virtual ~mesh() = 0;

    virtual const triangle* begin() const = 0;
    virtual const triangle* end() const = 0;
};

class mesh_collider : public collider {
   public:
    mesh_collider(kekw::spatial* spatial, const kekw::mesh* mesh)
        : collider(spatial), mesh_(mesh) {}

    inline const mesh* mesh() const { return this->mesh_; };

    bool hit_test(
        vec3_param_t origin_w, vec3_param_t direction_w, real_t& distance) override;

   private:
    const kekw::mesh* const mesh_;
};

}  // namespace kekw