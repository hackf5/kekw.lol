#include "window-manager.h"

#include <spdlog/spdlog.h>
#include <inc/stb/stb_image.h>

#include <src/util/file-utils.h>

#include <stdexcept>

kekw::ux::view::WindowInfo::WindowInfo(GLFWwindow *window) : window_(window) {}

GLFWwindow *kekw::ux::view::WindowInfo::getWindow() const {
    return this->window_;
}

kekw::ux::view::WindowLayer::~WindowLayer() {}

kekw::ux::view::WindowManager::WindowManager() : window_(0), layers_() {
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW.");
    }

    spdlog::debug("GLFW initialized.");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);

    // required for Mac OS X
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    const auto window_width = 1280;
    const auto window_height = 720;
    this->window_ =
        glfwCreateWindow(window_width, window_height, "kekw", NULL, NULL);
    if (this->window_ == NULL) {
        throw std::runtime_error("Failed to create GLFW window.");
    }

    spdlog::debug("GLFW window created.");

    glfwSetWindowUserPointer(this->window_, this);
    glfwMakeContextCurrent(this->window_);

    glfwSetFramebufferSizeCallback(
        this->window_, +[](GLFWwindow *window, int width, int height) {
            static_cast<WindowManager *>(glfwGetWindowUserPointer(window))
                ->FramebufferSizeCallback(window, width, height);
        });

    glfwSwapInterval(1);  // Enable vsync
    spdlog::debug("GLFW window initialized.");

    // TODO: this fails for non-ascii paths, converting from wide string to
    // string seems like a total ball ache. needs <windows.h> to be included and
    // the WideCharToMultiByte should be called.
    auto icon_path = kekw::util::get_absolute_path("res/img/app-icon.png");

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

    spdlog::debug("GL loaded.");
}

kekw::ux::view::WindowManager::~WindowManager() {
    glfwDestroyWindow(this->window_);
    glfwTerminate();
}

void kekw::ux::view::WindowManager::FramebufferSizeCallback(
    GLFWwindow *window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width
    // and height will be significantly larger than specified on retina
    // displays.
    glViewport(0, 0, width, height);
}

void kekw::ux::view::WindowManager::AddLayer(
    std::unique_ptr<WindowLayer> layer) {
    this->layers_.push_back(std::move(layer));
}

void kekw::ux::view::WindowManager::Start() {
    WindowInfo info(this->window_);

    for (auto it = this->layers_.begin(); it != this->layers_.end(); ++it) {
        (**it).Initialize(&info);
    }

    while (!glfwWindowShouldClose(this->window_)) {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);

        for (auto it = this->layers_.begin(); it != this->layers_.end(); ++it) {
            (**it).Render(&info);
        }

        glfwSwapBuffers(this->window_);
    }
}