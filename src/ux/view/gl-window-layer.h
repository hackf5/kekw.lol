#pragma once

#include "window-manager.h"

#include <spdlog/spdlog.h>
#include <src/util/file-utils.h>
#include <src/ux/shader/shader.h>

namespace kekw {
namespace ux {
namespace view {

class gl_window_layer : public window_layer {
   public:
    ~gl_window_layer();
    void initialize(window_info *manager) override;
    void render(window_info *manager) override;

   private:
    kekw::ux::shader shader_;
    GLuint vbo_, vao_, ebo_, texture_;
};

}  // namespace view
}  // namespace ux
}  // namespace kekw