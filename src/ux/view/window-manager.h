#pragma once

#include <inc/glad/glad.h>
#include <GLFW/glfw3.h>

#include <src/ux/contexts.h>

#include <memory>
#include <vector>
#include <string>

namespace kekw {

class window_layer;
class window_manager;
class window_context_impl;
class mouse_button_state_impl;

class window_layer {
   public:
    virtual ~window_layer() = 0;

    virtual void initialize(window_context *context) = 0;

    virtual void update(window_context *context) = 0;

    virtual void render(window_context *context) = 0;
};

class window_manager {
   public:
    window_manager();
    ~window_manager();

    void add_layer(std::unique_ptr<window_layer> layer);
    void start();

   private:
    GLFWwindow *window_;

    std::vector<std::unique_ptr<window_layer>> layers_;

    std::unique_ptr<window_context_impl> window_context_;

    void framebuffer_size_callback(GLFWwindow *window, int width, int height);

    void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
};

class window_context_impl : public window_context {
   public:
    window_context_impl(GLFWwindow *window);

    inline void *get_window() const override { return this->window_; }

    inline bool has_focus() const override { return this->has_focus_; }
    inline vec2 window_dims() const override { return this->window_dims_; }
    inline vec2 content_scale() const override { return this->content_scale_; }
    inline vec2 screen_resolution() const override { return this->screen_resolution_; }
    inline vec2 mouse_coords() const override { return this->mouse_coords_; }

    mouse_button_state *left_mouse_button() const override;

    mouse_button_state *right_mouse_button() const override;

    void before_poll_events();

    inline void after_poll_events();

    inline void update_dimensions(float width, float height) {
        this->window_dims_ = vec2(width, height);
    }

    inline mouse_button_state_impl *left_mouse_button() {
        return this->left_mouse_button_state_.get();
    }

    inline mouse_button_state_impl *right_mouse_button() {
        return this->right_mouse_button_state_.get();
    }

   private:
    GLFWwindow *window_;

    bool has_focus_;
    vec2 window_dims_;
    vec2 mouse_coords_;
    vec2 content_scale_;
    vec2 screen_resolution_;

    const std::unique_ptr<mouse_button_state_impl> left_mouse_button_state_;
    const std::unique_ptr<mouse_button_state_impl> right_mouse_button_state_;
};

}  // namespace kekw
