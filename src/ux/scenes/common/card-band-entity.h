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
    card_band_entity() : entity(), cards_(), drops_() {}

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

    std::vector<std::unique_ptr<card_entity>> cards_;
    std::vector<std::unique_ptr<card_drop_entity>> drops_;
};

}  // namespace kekw
