#pragma once

#include "box-mesh-2d.h"

#include <src/ux/contexts.h>

#include <src/world/types.h>
#include <src/world/entity.h>
#include <src/world/collider.h>
#include <src/world/renderer.h>

#include <memory>

namespace kekw {

class card_entity : public entity {
   public:
    card_entity()
        : entity(), collider_(std::make_unique<mesh_collider>(this, &MESH)), id_(ID++) {}

    void on_initialize(initialize_context* context) override;
    void on_update(update_context* context) override;
    void on_render(render_context* context) override;

   private:
    static const box_mesh_2d MESH;
    static unsigned long ID;

    inline renderer* get_renderer() {
        return static_cast<renderer*>(this->renderer_.get());
    }

    std::unique_ptr<collider> collider_;
    std::shared_ptr<void> renderer_;

    unsigned long id_;
};

}  // namespace kekw