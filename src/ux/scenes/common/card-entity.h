#pragma once

#include "box-mesh-2d.h"

#include <src/ux/contexts.h>
#include <src/world/types.h>
#include <src/world/entity.h>
#include <src/world/collider.h>
#include <src/world/renderer.h>

#include <vivid/vivid.h>

#include <memory>

namespace kekw {

class card_entity : public entity {
   public:
    card_entity() : entity(), collider_(std::make_unique<mesh_collider>(this, &MESH)) {}

    void on_initialize(initialize_context* context) override;
    void on_update(update_context* context) override;
    void on_late_update(update_context* context) override;
    void on_render(render_context* context) override;

    inline const vivid::Color& get_color() const { return this->color_; }
    inline void set_color(const vivid::Color& value) { this->color_ = value; }

   private:
    static const box_mesh_2d MESH;

    std::unique_ptr<collider> collider_;
    std::shared_ptr<void> renderer_;
    vivid::Color color_;

    inline renderer* get_renderer() {
        return static_cast<renderer*>(this->renderer_.get());
    }

    void render_drag(render_context* context);
    void render(render_context* context);
};

}  // namespace kekw