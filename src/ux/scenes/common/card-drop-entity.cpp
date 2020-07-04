#include "card-drop-entity.h"

#include <src/world/types.h>
#include <vivid/vivid.h>

using namespace kekw;

const box_mesh_2d card_drop_entity::MESH = box_mesh_2d(0.25f, 88.0f / 62.0f);

void card_drop_entity::on_initialize(initialize_context* context) {
    this->renderer_ = context->locate_service("drop_renderer");
}

void card_drop_entity::on_update(update_context* context) {}

void card_drop_entity::on_late_update(update_context* context) {}

void card_drop_entity::on_render(render_context* context) {
    if (context->pass() != 0) {
        return;
    }

    auto r = this->get_renderer();
    auto shader = r->get_shader();
    shader->use();
    shader->set("model", this->abs_matrix());
    shader->set("color", vec4(vivid::Color("#F2E2BA").value(), 1.0));

    r->render(this);
}