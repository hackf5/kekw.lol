#pragma once

#include "window-manager.h"

#include <src/mod/recruit-env.h>
#include <src/ux/shader/shader.h>

#include <memory>

namespace kekw {
namespace ux {
namespace view {

class recruit_window_layer : public window_layer {
   public:
    recruit_window_layer(std::shared_ptr<kekw::mod::recruit_env> recruit_env);
    ~recruit_window_layer();

    void initialize(window_info *manager) override;
    void render(window_info *manager) override;

   private:
    kekw::ux::shader shader_;
    GLuint vbo_, vao_, ebo_;
    std::shared_ptr<kekw::mod::recruit_env> recruit_env_;
};

}  // namespace view
}  // namespace ux
}  // namespace kekw