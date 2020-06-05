#pragma once

#include <inc/glad/glad.h>
#include <GLFW/glfw3.h>

#include <memory>
#include <vector>

namespace kekw {
namespace ux {
namespace view {

class WindowInfo {
   public:
    WindowInfo(GLFWwindow *window);

    GLFWwindow *getWindow() const;

   private:
    GLFWwindow *window_;
};

class WindowLayer {
   public:
    virtual ~WindowLayer() = 0;

    virtual void Initialize(WindowInfo *info) = 0;
    virtual void Render(WindowInfo *info) = 0;
};

class WindowManager {
   public:
    WindowManager();
    ~WindowManager();

    void AddLayer(std::unique_ptr<WindowLayer> layer);
    void Start();

   private:
    GLFWwindow *window_;

    std::vector<std::unique_ptr<WindowLayer>> layers_;

    void FramebufferSizeCallback(GLFWwindow *window, int width, int height);
};

}  // namespace view
}  // namespace ux
}  // namespace kekw
