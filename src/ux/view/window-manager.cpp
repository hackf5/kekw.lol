#include "window-manager.h"

#include <spdlog/spdlog.h>
#include <stb/stb_image.h>

#include <src/world/transform.h>
#include <src/ux/util/file-utils.h>

#include <string>
#include <stdexcept>

using namespace kekw;

namespace kekw {

class mouse_button_state_impl : public mouse_button_state {
   public:
    inline bool is_down() const override { return this->is_down_; }
    inline bool is_click() const override { return this->is_click_; }
    inline bool is_click_release() const override { return this->is_click_release_; }

    inline void begin_drag(unsigned long id, mat4_param_t transform) override {
        if (id == 0) {
            throw std::invalid_argument("id cannot be 0");
        }

        if (!this->is_click()) {
            throw std::runtime_error("can only begin drag on click");
        }

        this->drag_id_ = id;
        this->drag_transform_ = transform;
    }

    inline bool is_dragging(unsigned long id) const override {
        return this->drag_id_ == id;
    }

    inline mat4 get_drag_transform() const override { return this->drag_transform_; }

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
            this->drag_id_ = 0;
            this->drag_transform_ = glm::identity<mat4>();
            this->is_down_ = false;
        } else {
            this->is_click_ = true;
            this->is_down_ = true;
        }
    }

   private:
    bool is_down_;
    bool is_click_release_;
    bool is_click_;

    unsigned long drag_id_;
    mat4 drag_transform_;
};

}  // namespace kekw

window_context_impl::window_context_impl(GLFWwindow *window)
    : window_(window),
      left_mouse_button_state_(new mouse_button_state_impl()),
      right_mouse_button_state_(new mouse_button_state_impl()) {
    // macos high dpi oddness...
    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    glfwGetMonitorContentScale(monitor, &this->content_scale_.x, &this->content_scale_.y);
    int scr_x, scr_y;
    glfwGetMonitorPhysicalSize(monitor, &scr_x, &scr_y);
    this->screen_resolution_ = vec2(scr_x, scr_y);
}

mouse_button_state *window_context_impl::left_mouse_button() const {
    return this->left_mouse_button_state_.get();
}

mouse_button_state *window_context_impl::right_mouse_button() const {
    return this->right_mouse_button_state_.get();
}

void window_context_impl::before_poll_events() {
    this->left_mouse_button_state_->before_poll_events();
    this->right_mouse_button_state_->before_poll_events();
}

void window_context_impl::after_poll_events() {
    this->has_focus_ = glfwGetWindowAttrib(this->window_, GLFW_FOCUSED) != 0;
    double mouse_x, mouse_y;
    glfwGetCursorPos(this->window_, &mouse_x, &mouse_y);
    this->mouse_coords_ = vec2(mouse_x, mouse_y) * this->content_scale_;
}

window_layer::~window_layer() {}

window_manager::window_manager() : window_(0), layers_(), window_context_() {
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW.");
    }

    spdlog::debug("GLFW initialized.");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);

    // required for Mac OS X
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    const auto window_width = 1280;
    const auto window_height = 720;
    this->window_ = glfwCreateWindow(window_width, window_height, "kekw", NULL, NULL);
    if (this->window_ == NULL) {
        throw std::runtime_error("Failed to create GLFW window.");
    }

    this->window_context_ =
        std::unique_ptr<window_context_impl>(new window_context_impl(this->window_));
    spdlog::debug("GLFW window created.");

    glfwSetWindowUserPointer(this->window_, this);
    glfwMakeContextCurrent(this->window_);

    glfwSetFramebufferSizeCallback(
        this->window_, +[](GLFWwindow *window, int width, int height) {
            static_cast<window_manager *>(glfwGetWindowUserPointer(window))
                ->framebuffer_size_callback(window, width, height);
        });

    glfwSetMouseButtonCallback(
        this->window_, +[](GLFWwindow *window, int button, int action, int mods) {
            static_cast<window_manager *>(glfwGetWindowUserPointer(window))
                ->mouse_button_callback(window, button, action, mods);
        });

    glfwSwapInterval(1);  // Enable vsync
    spdlog::debug("GLFW window initialized.");

    // TODO: this fails for non-ascii paths, converting from wide string to
    // string seems like a total ball ache. needs <windows.h> to be included and
    // the WideCharToMultiByte should be called.
    auto icon_path = get_absolute_path("res/img/app-icon.png");

    GLFWimage images[1];
    images[0].pixels = stbi_load(
        icon_path.string().c_str(),
        &images[0].width,
        &images[0].height,
        0,
        4);  // rgba channels

    glfwSetWindowIcon(this->window_, 1, images);
    stbi_image_free(images[0].pixels);

    spdlog::debug("GLFW window icon set.");

    if (!gladLoadGL()) {
        throw std::runtime_error("Failed to load GL.");
    }

    glEnable(GL_DEPTH_TEST);
    spdlog::debug("GL loaded.");
}

window_manager::~window_manager() {
    glfwDestroyWindow(this->window_);
    glfwTerminate();
}

void window_manager::framebuffer_size_callback(
    GLFWwindow *window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width
    // and height will be significantly larger than specified on retina
    // displays.
    glViewport(0, 0, width, height);
    this->window_context_->update_dimensions(width, height);
}

void window_manager::mouse_button_callback(
    GLFWwindow *window, int button, int action, int mods) {
    mouse_button_state_impl *m_button;
    switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT:
            m_button = this->window_context_->left_mouse_button();
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            m_button = this->window_context_->right_mouse_button();
            break;
        default:
            break;
    }

    if (action == GLFW_PRESS) {
        m_button->update(true);
    } else if (action == GLFW_RELEASE) {
        m_button->update(false);
    }
}

void window_manager::add_layer(std::unique_ptr<window_layer> layer) {
    this->layers_.push_back(std::move(layer));
}

void window_manager::start() {
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    this->window_context_->update_dimensions(viewport[2], viewport[3]);

    for (auto it = this->layers_.begin(); it != this->layers_.end(); ++it) {
        (**it).initialize(this->window_context_.get());
    }

    while (!glfwWindowShouldClose(this->window_)) {
        this->window_context_->before_poll_events();
        glfwPollEvents();
        this->window_context_->after_poll_events();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (auto it = this->layers_.begin(); it != this->layers_.end(); ++it) {
            (**it).update(this->window_context_.get());
        }

        for (auto it = this->layers_.begin(); it != this->layers_.end(); ++it) {
            (**it).render(this->window_context_.get());
        }

        glfwSwapBuffers(this->window_);
    }
}