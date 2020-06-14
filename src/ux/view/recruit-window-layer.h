#pragma once

#include "window-manager.h"

#include <inc/glad/glad.h>
#include <GLFW/glfw3.h>

#include <src/mod/recruit-env.h>
#include <src/ux/shader/shader.h>

#include "world/world.hpp"

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
    std::shared_ptr<kekw::ux::shader> shader_;
    GLuint vbo_, vao_, ebo_;
    std::shared_ptr<kekw::mod::recruit_env> recruit_env_;
    std::shared_ptr<card_body> card_body_;

    const float card_width = 1.0f;
    const float card_height = 88.0f / 62.0f;
    const float margin = 0.25f;
};

}  // namespace view
}  // namespace ux
}  // namespace kekw