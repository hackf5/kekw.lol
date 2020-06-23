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

    virtual void initialize(window_context *info) = 0;

    virtual void update(window_context *info) {}

    virtual void render(window_context *info) = 0;
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

    std::unique_ptr<window_context_impl> window_info_;

    void framebuffer_size_callback(GLFWwindow *window, int width, int height);

    void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
};

class mouse_button_state_impl : public mouse_button_state {
   public:
    inline bool is_down() override { return this->is_down_; }
    inline bool is_click() override { return this->is_click_; }
    inline bool is_click_release() override { return this->is_click_release_; }

    void before_poll_events() {
        this->is_click_ = false;
        this->is_click_release_ = false;
    }

    void update(bool is_down) {
        if (this->is_down_ == is_down) {
            // no change in state;
            return;
        }

        if (this->is_down_) {
            // then !is_down;
            this->is_click_release_ = true;
        } else {
            this->is_click_ = true;
        }
    }

   private:
    bool is_down_;
    bool is_click_release_;
    bool is_click_;
};

class window_context_impl : public window_context {
   public:
    window_context_impl(GLFWwindow *window);

    inline void *get_window() const override { return this->window_; }

    inline bool has_focus() const { return this->has_focus_; }
    inline double window_width() const override { return this->window_width_; }
    inline double window_height() const override { return this->window_height_; }

    inline double mouse_x() const override { return this->mouse_x_; }
    inline double mouse_y() const override { return this->mouse_y_; }

    inline mouse_button_state const *left_mouse_button() const override {
        return this->left_mouse_button_state_.get();
    }

    inline mouse_button_state const *right_mouse_button() const override {
        return this->right_mouse_button_state_.get();
    }

    inline void before_poll_events() {
        this->left_mouse_button_state_->before_poll_events();
        this->right_mouse_button_state_->before_poll_events();
    }

    inline void after_poll_events() {
        this->has_focus_ = glfwGetWindowAttrib(this->window_, GLFW_FOCUSED) != 0;
        glfwGetCursorPos(this->window_, &this->mouse_x_, &this->mouse_y_);
    }

    inline void update_dimensions(double width, double height) {
        this->window_width_ = width;
        this->window_height_ = height;
    }

    inline mouse_button_state_impl *left_mouse_button() {
        return this->left_mouse_button_state_.get();
    }

    inline mouse_button_state_impl *right_mouse_button() {
        return this->right_mouse_button_state_.get();
    }

   private:
    GLFWwindow *window_;

    int window_width_;
    int window_height_;
    bool has_focus_;
    double mouse_x_;
    double mouse_y_;

    const std::unique_ptr<mouse_button_state_impl> left_mouse_button_state_;
    const std::unique_ptr<mouse_button_state_impl> right_mouse_button_state_;
};

}  // namespace kekw
