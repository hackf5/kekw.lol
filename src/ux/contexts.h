#pragma once

#include <src/world/types.h>
#include <src/world/scene.h>

#include <string>

namespace kekw {

class mouse_button_state {
   public:
    virtual ~mouse_button_state() {}

    virtual bool is_down() const = 0;
    virtual bool is_click() const = 0;
    virtual bool is_click_release() const = 0;
    virtual bool is_dragging() const = 0;

    virtual void begin_drag(unsigned long id, vec3_param_t intersect) = 0;
    virtual bool is_dragging(unsigned long id) const = 0;

    virtual vec3_ret_t get_drag_intersect() const = 0;
};

class window_context {
   public:
    virtual ~window_context() {}

    virtual void *get_window() const = 0;

    virtual bool has_focus() const = 0;
    virtual vec2 window_dims() const = 0;
    virtual vec2 content_scale() const = 0;
    virtual vec2 screen_resolution() const = 0;
    virtual vec2 mouse_coords() const = 0;

    virtual mouse_button_state *left_mouse_button() const = 0;
    virtual mouse_button_state *right_mouse_button() const = 0;
};

class context_base {
   public:
    context_base(const window_context *window_ctx, kekw::scene *scene)
        : window_ctx_(window_ctx), scene_(scene) {}
    virtual ~context_base() {}

    inline const window_context *window_ctx() const { return this->window_ctx_; };

    inline kekw::scene *scene() const { return this->scene_; }

   private:
    const window_context *const window_ctx_;
    kekw::scene *const scene_;
};

class initialize_context : public context_base {
   public:
    initialize_context(const window_context *window_ctx, kekw::scene *scene)
        : context_base(window_ctx, scene) {}

    virtual void register_service(
        const std::string &name, std::shared_ptr<void> service) = 0;

    virtual std::shared_ptr<void> locate_service(const std::string &name) = 0;
};

class update_context : public context_base {
   public:
    update_context(const window_context *window_ctx, kekw::scene *scene)
        : context_base(window_ctx, scene) {}

    virtual vec3_ret_t get_mouse_ray() const = 0;

    virtual void set_mouse_ray(vec3_param_t ray) = 0;

    virtual void register_hit(
        const std::string &category, unsigned long id, real_t distance) = 0;

    virtual unsigned long get_hit_id(const std::string &category) const = 0;

    virtual vec3 get_drag_plane_intercept() const = 0;
};

class render_context : public context_base {
   public:
    render_context(const window_context *window_ctx, kekw::scene *scene)
        : context_base(window_ctx, scene) {}

    virtual const update_context *update_ctx() = 0;

    virtual unsigned int pass() const = 0;
};

}  // namespace kekw