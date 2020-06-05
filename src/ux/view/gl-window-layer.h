#pragma once

#include "window-manager.h"

#include <spdlog/spdlog.h>
#include <src/util/file-utils.h>
#include <src/ux/shader/shader.h>

namespace kekw {
namespace ux {
namespace view {

class GlWindowLayer : public WindowLayer {
   public:
    ~GlWindowLayer();
    void Initialize(WindowInfo *manager) override;
    void Render(WindowInfo *manager) override;

   private:
    kekw::ux::Shader shader_;
    GLuint VBO, VAO;
};

}  // namespace view
}  // namespace ux
}  // namespace kekw