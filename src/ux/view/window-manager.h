#pragma once

#include <inc/glad/glad.h>
#include <GLFW/glfw3.h>

#include <memory>
#include <vector>
#include <string>

namespace kekw {
namespace ux {
namespace view {

class window_info;

class window_layer {
   public:
    virtual ~window_layer() = 0;

    virtual void initialize(window_info *info) = 0;
    virtual void render(window_info *info) = 0;
};

class window_manager {
   public:
    window_manager();
    ~window_manager();

    void add_layer(std::unique_ptr<window_layer> layer);
    void Start();

   private:
    GLFWwindow *window_;

    std::vector<std::unique_ptr<window_layer>> layers_;

    std::unique_ptr<window_info> window_info_;

    void framebuffer_size_callback(GLFWwindow *window, int width, int height);

    void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
};

class mouse_button_state {
    friend class window_manager;
    friend class window_info;

   private:
    bool is_down_;
    bool is_click_release_;
    bool is_click_;

   public:
    bool is_down() { return this->is_down_; }
    bool is_click() { return this->is_click_; }
    bool is_click_release() { return this->is_click_release_; }

   private:
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
};

class window_info {
    friend class window_manager;

   public:
    window_info(GLFWwindow *window);

    GLFWwindow *get_window() const;

    double mouse_x() const { return this->mouse_x_; }
    double mouse_y() const { return this->mouse_y_; }

    mouse_button_state const *left_mouse_button() const {
        return this->left_mouse_button_state_.get();
    }

    mouse_button_state const *right_mouse_button() const {
        return this->right_mouse_button_state_.get();
    }

    bool focused() const { return this->focused_; }
    int window_width() const { return this->window_width_; }
    int window_height() const { return this->window_height_; }

    std::string debug_1;

   private:
    void before_poll_events() {
        this->left_mouse_button_state_->before_poll_events();
        this->right_mouse_button_state_->before_poll_events();
    }

    void after_poll_events() {
        this->focused_ = glfwGetWindowAttrib(this->window_, GLFW_FOCUSED) != 0;
        glfwGetCursorPos(this->window_, &this->mouse_x_, &this->mouse_y_);
    }

    GLFWwindow *window_;

    const std::unique_ptr<mouse_button_state> left_mouse_button_state_;
    const std::unique_ptr<mouse_button_state> right_mouse_button_state_;
    int window_width_;
    int window_height_;
    bool focused_;
    double mouse_x_;
    double mouse_y_;
};

}  // namespace view
}  // namespace ux
}  // namespace kekw
