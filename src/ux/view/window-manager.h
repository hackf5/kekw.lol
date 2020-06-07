#pragma once

#include <inc/glad/glad.h>
#include <GLFW/glfw3.h>

#include <memory>
#include <vector>

namespace kekw {
namespace ux {
namespace view {

class window_info {
   public:
    window_info(GLFWwindow *window);

    GLFWwindow *get_window() const;

   private:
    GLFWwindow *window_;
};

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

    void framebuffer_size_callback(GLFWwindow *window, int width, int height);
};

}  // namespace view
}  // namespace ux
}  // namespace kekw
