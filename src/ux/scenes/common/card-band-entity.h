#pragma once

#include "card-entity.h"

#include <src/world/entity.h>

#include <vector>
#include <memory>

namespace kekw {

class card_band_entity : public entity {
   public:
    card_band_entity() : entity(), cards_() {}

    void on_initialize(initialize_context* context) override {
        this->set_position(vec3(0, 0.f, -10.0f));

        for (int i = 0; i != 6; i++) {
            this->cards_.push_back(std::make_unique<card_entity>());
        }

        for (auto it = this->cards_.begin(); it != this->cards_.end(); ++it) {
            auto card = it->get();
            card->set_parent(this);
            card->on_initialize(context);
        }
    }

    void on_update(update_context* context) override {
        const real_t width = 1.25;
        real_t idx = 1.f - this->cards_.size();
        real_t centre = idx * (width / 2);
        for (auto it = this->cards_.begin(); it != this->cards_.end(); ++it) {
            (*it)->set_position(vec3(centre, 0, 0));
            (*it)->on_update(context);
            centre += width;
        }
    }

    void on_render(render_context* context) override {
        for (auto it = this->cards_.begin(); it != this->cards_.end(); ++it) {
            (*it)->on_render(context);
        }
    }

   private:
    std::vector<std::unique_ptr<card_entity>> cards_;
};

}  // namespace kekw
