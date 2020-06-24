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
    auto r = this->get_renderer();
    auto shader = r->get_shader();
    shader->use();

    if (this->id_ == context->update_ctx()->get_hit_id()) {
        shader->set("state", 1);
    } else {
        shader->set("state", 0);
    }

    r->render(this);
}