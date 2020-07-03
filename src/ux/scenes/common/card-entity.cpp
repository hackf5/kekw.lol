#include "card-entity.h"

#include <src/ux/shader/shader.h>

using namespace kekw;

const box_mesh_2d card_entity::MESH = box_mesh_2d(1, 88.0f / 62.0f);
unsigned long card_entity::ID = 1;

void card_entity::on_initialize(initialize_context* context) {
    this->renderer_ = context->locate_service("card_renderer");
}

void card_entity::on_update(update_context* context) {
    real_t distance;
    auto hit = this->collider_->hit_test(
        context->scene()->cam()->position(), context->get_mouse_ray(), distance);

    if (hit) {
        context->register_hit(this->id_, distance);
    }
}

void card_entity::on_render(render_context* context) {
    if (context->window_ctx()->left_mouse_button()->is_click()) {
        if (this->id_ == context->update_ctx()->get_hit_id()) {
            context->window_ctx()->left_mouse_button()->begin_drag(
                this->id_, this->abs_matrix());
        }
    }

    if (context->window_ctx()->left_mouse_button()->is_dragging(this->id_)) {
        return;
    }

    auto r = this->get_renderer();
    auto shader = r->get_shader();
    shader->use();
    shader->set("model", this->abs_matrix());

    if (this->id_ == context->update_ctx()->get_hit_id()) {
        shader->set("color", vec4(vivid::Color("#B20D30").value(), 1.0));
    } else {
        shader->set("color", vec4(this->color_.value(), 1.0));
    }

    r->render(this);
}