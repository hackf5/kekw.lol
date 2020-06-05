#pragma once

#include "window-manager.h"

#include <inc/imgui/imgui.h>
#include <inc/imgui/imgui_impl_glfw.h>
#include <inc/imgui/imgui_impl_opengl3.h>

namespace kekw {
namespace ux {
namespace view {

class UxWindowLayer : public WindowLayer {
   public:
    ~UxWindowLayer();
    void Initialize(WindowInfo *info) override;
    void Render(WindowInfo *info) override;
};

}  // namespace view
}  // namespace ux
}  // namespace kekw