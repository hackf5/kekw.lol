#pragma once

#include <src/world/types.h>
#include <src/world/entity.h>
#include <src/ux/contexts.h>
#include <src/ux/scenes/common/card-band-entity.h>

#include <memory>

namespace kekw {

class recruit_scene_entity : public entity {
   public:
    recruit_scene_entity() : entity() {}

    void on_initialize(initialize_context* context) override {
        this->set_position(vec3(0, 0.f, -10.0f));

        this->available_ = std::make_unique<card_band_entity>();
        this->available_->set_parent(this);
        this->available_->set_position(vec3(0, 1, 0));
        this->available_->on_initialize(context);

        this->owned_ = std::make_unique<card_band_entity>();
        this->owned_->set_parent(this);
        this->owned_->set_position(vec3(0, -1, 0));
        this->owned_->set_scale(vec3(0.75, 0.75, 0.75));
        this->owned_->on_initialize(context);
    }

    void on_update(update_context* context) override {
        this->available_->on_update(context);
        this->owned_->on_update(context);
    }

    void on_late_update(update_context* context) override {
        this->available_->on_late_update(context);
        this->owned_->on_late_update(context);
    }

    void on_render(render_context* context) override {
        this->available_->on_render(context);
        this->owned_->on_render(context);
    }

   private:
    std::unique_ptr<card_band_entity> available_;
    std::unique_ptr<card_band_entity> owned_;
};

}  // namespace kekw