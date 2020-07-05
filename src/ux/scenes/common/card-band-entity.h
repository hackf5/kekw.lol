#pragma once

#include "card-entity.h"
#include "card-drop-entity.h"

#include <src/world/entity.h>
#include <vivid/vivid.h>

#include <vector>
#include <memory>
#include <algorithm>

namespace kekw {

class card_band_entity : public entity {
   public:
    card_band_entity()
        : entity(),
          cards_(),
          drops_(),
          l_spatial_(std::make_unique<spatial>(this)),
          r_spatial_(std::make_unique<spatial>(this)),
          l_collider_(std::make_unique<mesh_collider>(this->l_spatial_.get(), &L_MESH)),
          r_collider_(std::make_unique<mesh_collider>(this->r_spatial_.get(), &R_MESH)) {
        // put the left and right colliders slightly behind the drop layer so the drop
        // layer always gets hit first.
        this->l_spatial_->set_scale({1000, 1.f, 1.f});
        this->l_spatial_->set_position({0, 0, -0.01f});

        this->r_spatial_->set_scale({1000, 1.f, 1.f});
        this->r_spatial_->set_position({0, 0, -0.01f});
    }

    void on_initialize(initialize_context* context) override {
        for (int i = 0; i != 6; i++) {
            this->cards_.push_back(std::make_unique<card_entity>());
        }

        const real_t width = 1.25;
        real_t idx = 1.f - this->cards_.size();
        real_t centre = idx * (width / 2);
        auto start_color = vivid::Color("#3F84E5");
        auto end_color = vivid::Color("#BAF2E9");
        int index = 0;
        for (auto it = this->cards_.begin(); it != this->cards_.end(); ++it, ++index) {
            auto card = it->get();
            card->set_parent(this);
            card->on_initialize(context);
            card->set_color(vivid::lerpLch(
                start_color, end_color, (float)index / this->cards_.size()));
            card->set_position(vec3(centre, 0, 0));
            centre += width;
        }

        for (int i = 0; i != this->cards_.size() + 1; i++) {
            this->drops_.push_back(std::make_unique<card_drop_entity>());
        }

        idx = 1.f - this->drops_.size();
        centre = idx * (width / 2);
        for (auto it = this->drops_.begin(); it != this->drops_.end(); ++it) {
            auto drop = it->get();
            drop->set_parent(this);
            drop->on_initialize(context);
            drop->set_position(vec3(centre, 0, 0));
            centre += width;
        }
    }

    void on_update(update_context* context) override {
        real_t distance;
        if (this->l_collider_->hit_test(
                context->scene()->cam()->position(),
                context->get_mouse_ray(),
                distance)) {
            context->register_hit("d", this->l_id_, distance);
        }

        if (this->r_collider_->hit_test(
                context->scene()->cam()->position(),
                context->get_mouse_ray(),
                distance)) {
            context->register_hit("d", this->r_id_, distance);
        }

        for (auto it = this->cards_.begin(); it != this->cards_.end(); ++it) {
            (*it)->on_update(context);
        }

        for (auto it = this->drops_.begin(); it != this->drops_.end(); ++it) {
            (*it)->on_update(context);
        }
    }

    void on_late_update(update_context* context) override;

    void on_render(render_context* context) override;

   private:
    template <class T>
    bool find_index_by_id(
        const std::vector<std::unique_ptr<T>>& entities,
        entity_id_t id,
        int& index) const;

    static const box_mesh_2d L_MESH;
    static const box_mesh_2d R_MESH;

    std::vector<std::unique_ptr<card_entity>> cards_;
    std::vector<std::unique_ptr<card_drop_entity>> drops_;

    entity_id_t l_id_ = entity_id::next();
    entity_id_t r_id_ = entity_id::next();
    std::unique_ptr<spatial> l_spatial_;
    std::unique_ptr<spatial> r_spatial_;
    std::unique_ptr<collider> l_collider_;
    std::unique_ptr<collider> r_collider_;
};

}  // namespace kekw
