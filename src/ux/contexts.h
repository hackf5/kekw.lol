#pragma once

#include <src/world/scene.h>

namespace kekw {

class mouse_button_state {
   public:
    virtual ~mouse_button_state() {}

    virtual bool is_down() = 0;
    virtual bool is_click() = 0;
    virtual bool is_click_release() = 0;
};

class window_context {
   public:
    virtual ~window_context() {}

    virtual void *get_window() const = 0;

    virtual bool has_focus() const = 0;
    virtual double window_width() const = 0;
    virtual double window_height() const = 0;

    virtual double mouse_x() const = 0;
    virtual double mouse_y() const = 0;
    virtual mouse_button_state const *left_mouse_button() const = 0;
    virtual mouse_button_state const *right_mouse_button() const = 0;
};

class context_base {
   public:
    context_base(const window_context *window_ctx, kekw::world::scene *scene)
        : window_ctx_(window_ctx), scene_(scene) {}
    virtual ~context_base() {}

    inline const window_context *window_ctx() const { return this->window_ctx_; };

    inline kekw::world::scene *scene() const { return this->scene_; }

   private:
    const window_context *const window_ctx_;
    kekw::world::scene *const scene_;
};

class initialize_context : public context_base {
   public:
    initialize_context(const window_context *window_ctx, kekw::world::scene *scene)
        : context_base(window_ctx, scene) {}

    virtual void register_service(
        const std::string &name, std::shared_ptr<void> service) = 0;

    virtual std::shared_ptr<void> locate_service(const std::string &name) = 0;
};

class update_context : public context_base {
   public:
    update_context(const window_context *window_ctx, kekw::world::scene *scene)
        : context_base(window_ctx, scene) {}

    virtual const update_context *previous_context() = 0;
};

class render_context : public context_base {
   public:
    render_context(const window_context *window_ctx, kekw::world::scene *scene)
        : context_base(window_ctx, scene) {}
};

}  // namespace kekw