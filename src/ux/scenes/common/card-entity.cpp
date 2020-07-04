#include "card-entity.h"

#include <src/ux/shader/shader.h>
#include <src/world/plane.h>

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
    if (context->window_ctx()->left_mouse_button()->is_click() &&
        this->id_ == context->update_ctx()->get_hit_id()) {
        auto position = (this->abs_matrix() * vec4(transform::origin, 1.f)).xyz();
        auto t = glm::translate(
            this->abs_matrix(),
            context->update_ctx()->get_drag_plane_intercept() - position);

        context->window_ctx()->left_mouse_button()->begin_drag(this->id_, t);
    }

    auto r = this->get_renderer();
    auto shader = r->get_shader();
    shader->use();

    if (context->window_ctx()->left_mouse_button()->is_dragging(this->id_)) {
        auto drag_transform =
            context->window_ctx()->left_mouse_button()->get_drag_transform();
        auto position = (drag_transform * vec4(transform::origin, 1.f)).xyz();
        auto t = glm::translate(
            drag_transform, context->update_ctx()->get_drag_plane_intercept() - position);
        shader->set("model", t);
        shader->set("color", vec4(vivid::Color("#B0F2B4").value(), 1.0));
    } else {
        shader->set("model", this->abs_matrix());
        if (this->id_ == context->update_ctx()->get_hit_id()) {
            shader->set("color", vec4(vivid::Color("#B20D30").value(), 1.0));
        } else {
            shader->set("color", vec4(this->color_.value(), 1.0));
        }
    }

    r->render(this);
}