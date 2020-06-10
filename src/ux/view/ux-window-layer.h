#pragma once

#include "window-manager.h"

#include <inc/imgui/imgui.h>
#include <inc/imgui/imgui_impl_glfw.h>
#include <inc/imgui/imgui_impl_opengl3.h>

namespace kekw {
namespace ux {
namespace view {

class ux_window_layer : public window_layer {
   public:
    ~ux_window_layer();
    void initialize(window_info *info) override;
    void render(window_info *info) override;
};

}  // namespace view
}  // namespace ux
}  // namespace kekw