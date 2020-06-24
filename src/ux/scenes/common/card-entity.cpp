#include "card-entity.h"

#include <src/ux/shader/shader.h>

using namespace kekw;

const box_mesh_2d card_entity::MESH = box_mesh_2d(1, 88.0f / 62.0f);

void card_entity::on_initialize(initialize_context* context) {
    this->renderer_ = context->locate_service("card_renderer");
}

void card_entity::on_update(update_context* context) {
    real_t distance;
    this->is_hit_ = this->collider_->hit_test(
        context->scene()->cam()->position(), context->get_mouse_ray(), distance);
}

void card_entity::on_render(render_context* context) {
    auto r = this->get_renderer();
    auto shader = r->get_shader();
    shader->use();
    shader->set("highlight", this->is_hit_);
    r->render(this);
}