#include "card-band-entity.h"

#include <set>

using namespace kekw;

const box_mesh_2d card_band_entity::L_MESH = box_mesh_2d(1, 88.0f / 62.0f, {-0.5f, 0});
const box_mesh_2d card_band_entity::R_MESH = box_mesh_2d(1, 88.0f / 62.0f, {0.5f, 0});

void card_band_entity::on_late_update(update_context* context) {
    for (auto it = this->cards_.begin(); it != this->cards_.end(); ++it) {
        (*it)->on_late_update(context);
    }

    for (auto it = this->drops_.begin(); it != this->drops_.end(); ++it) {
        (*it)->on_late_update(context);
    }

    if (!context->window_ctx()->left_mouse_button()->is_dragging()) {
        return;
    }

    if (context->get_hit_id("d") == this->l_id_) {
        this->drops_.front()->force_hit();
    }

    if (context->get_hit_id("d") == this->r_id_) {
        this->drops_.back()->force_hit();
    }
}

void card_band_entity::on_render(render_context* context) {
    std::set<decltype(this->drops_)::iterator> excluded_drops;

    if (context->window_ctx()->left_mouse_button()->is_dragging()) {
        auto drag_id = context->window_ctx()->left_mouse_button()->drag_id();
        int card_index;
        if (this->find_index_by_id(this->cards_, drag_id, card_index)) {
            excluded_drops.insert(this->drops_.begin() + card_index);
            excluded_drops.insert(this->drops_.begin() + card_index + 1);
        }
    }

    for (auto it = this->cards_.begin(); it != this->cards_.end(); ++it) {
        (*it)->on_render(context);
    }

    for (auto it = this->drops_.begin(); it != this->drops_.end(); ++it) {
        if (excluded_drops.count(it) == 0) {
            (*it)->on_render(context);
        }
    }
}

template <class T>
bool card_band_entity::find_index_by_id(
    const std::vector<std::unique_ptr<T>>& entities, entity_id_t id, int& index) const {
    auto it = std::find_if(
        entities.begin(), entities.end(), [id](const std::unique_ptr<T>& ptr) {
            return ptr->id() == id;
        });

    if (it != entities.end()) {
        index = std::distance(entities.begin(), it);
        return true;
    }

    return false;
}