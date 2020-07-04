#include "recruit-scene.h"

#include <src/ux/scenes/common/card-renderer.h>
#include <src/ux/shader/shader.h>

using namespace kekw;

void recruit_scene::on_initialize(initialize_context* context) {
    auto shader = register_shader("default");
    shader->add_stage_file(GL_VERTEX_SHADER, "glsl/default.vert");
    shader->add_stage_file(GL_FRAGMENT_SHADER, "glsl/card.frag");
    shader->compile();
    shader->use();

    auto cr = std::make_shared<card_renderer>(1, 88.0f / 62.0f);
    cr->initialize();
    context->register_service("card_renderer", cr);

    auto dr = std::make_shared<card_renderer>(0.25f, 88.0f / 62.0f);
    dr->initialize();
    context->register_service("drop_renderer", dr);

    this->cam()->set_clip_plane(vec2(0.1f, 100.f));
    this->cam()->set_field_of_view(glm::radians(45.f));

    auto window_ctx = context->window_ctx();
    this->cam()->set_aspect_ratio(
        window_ctx->window_dims().x / window_ctx->window_dims().y);
    this->cam()->set_viewport(vec4(0.f, 0.f, window_ctx->window_dims()));
    this->cam()->set_position(glm::zero<vec3>());

    this->root()->on_initialize(context);
}

void recruit_scene::on_update(update_context* context) {
    auto window_ctx = context->window_ctx();
    this->cam()->set_aspect_ratio(
        window_ctx->window_dims().x / window_ctx->window_dims().y);
    this->cam()->set_viewport(glm::vec4(0.f, 0.f, window_ctx->window_dims()));
    this->cam()->look_at(glm::vec4(0.f, 0.f, -10.0f, 1.f));

    // the camera transforms need to be stored immediately after look_at as the camera
    // could move during update.
    this->projection_ = this->cam()->get_projection();
    this->view_ = this->cam()->get_view();

    auto mouse_x = window_ctx->mouse_coords().x;
    auto mouse_y = this->cam()->get_viewport().w - window_ctx->mouse_coords().y;
    auto mouse_screen = vec2(mouse_x, mouse_y);
    auto v0 = this->cam()->to_world_coords(vec3(mouse_screen, 0.f));
    auto v1 = this->cam()->to_world_coords(vec3(mouse_screen, 1.f));
    context->set_mouse_ray(glm::normalize(v1 - v0));

    this->root()->on_update(context);
}

void recruit_scene::on_late_update(update_context* context) {
    this->root()->on_late_update(context);
}

void recruit_scene::on_render(render_context* context) {
    auto shader = get_shader("default");
    shader->use();
    shader->set("projection", this->projection_);
    shader->set("view", this->view_);

    this->root()->on_render(context);
}