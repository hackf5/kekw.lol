#include "card-entity.h"

#include <src/ux/shader/shader.h>
#include <src/world/plane.h>

using namespace kekw;

const box_mesh_2d card_entity::MESH = box_mesh_2d(1, 88.0f / 62.0f);

void card_entity::on_initialize(initialize_context* context) {
    this->renderer_ = context->locate_service("card_renderer");
}

void card_entity::on_update(update_context* context) {
    real_t distance;
    auto hit = this->collider_->hit_test(
        context->scene()->cam()->position(), context->get_mouse_ray(), distance);

    if (hit) {
        context->register_hit("", this->id(), distance);
    }
}

void card_entity::on_late_update(update_context* context) {
    if (context->window_ctx()->left_mouse_button()->is_click() &&
        this->id() == context->get_hit_id("")) {
        auto centre = (this->abs_matrix() * vec4(transform::origin, 1.f)).xyz();

        real_t distance;
        this->collider_->hit_test(
            context->scene()->cam()->position(), context->get_mouse_ray(), distance);

        auto intersect =
            context->scene()->cam()->position() + context->get_mouse_ray() * distance;

        context->window_ctx()->left_mouse_button()->begin_drag(this->id(), intersect);
    }
}

void card_entity::on_render(render_context* context) {
    if (context->window_ctx()->left_mouse_button()->is_dragging(this->id())) {
        this->render_drag(context);
    } else {
        this->render(context);
    }
}

void card_entity::render_drag(render_context* context) {
    if (context->pass() != 1) {
        return;
    }

    auto r = this->get_renderer();
    auto shader = r->get_shader();
    shader->use();

    auto t = glm::translate(
        this->abs_matrix(),
        context->update_ctx()->get_drag_plane_intercept() -
            context->window_ctx()->left_mouse_button()->get_drag_intersect());

    shader->set("model", t);
    shader->set("color", vec4(vivid::Color("#B0F2B4").value(), .8f));

    r->render(this);
}

void card_entity::render(render_context* context) {
    if (context->pass() != 0) {
        return;
    }

    auto r = this->get_renderer();
    auto shader = r->get_shader();
    shader->use();

    shader->set("model", this->abs_matrix());
    if (!context->window_ctx()->left_mouse_button()->is_dragging() &&
        this->id() == context->update_ctx()->get_hit_id("")) {
        shader->set("color", vec4(vivid::Color("#B20D30").value(), 1.0));
    } else {
        shader->set("color", vec4(this->color_.value(), 1.0));
    }

    r->render(this);
}