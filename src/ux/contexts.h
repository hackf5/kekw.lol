#pragma once

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

class update_context {
   public:
    virtual ~update_context() {}

    virtual const window_context *window_ctx() const = 0;
};

class render_context {
   public:
    virtual ~render_context() {}

    virtual const window_context *window_ctx() const = 0;
};

}  // namespace kekw