#pragma once

#include "../ux-window-layer.h"

#include <src/mod/recruit-env.h>

#include <memory>

namespace kekw {
namespace ux {
namespace view {
namespace widgets {

class test_widget : public ux_window_widget {
   public:
    test_widget(std::shared_ptr<kekw::mod::recruit_env> recruit_env);

    void update(window_context *info) override;

   private:
    std::shared_ptr<kekw::mod::recruit_env> recruit_env_;
};

}  // namespace widgets
}  // namespace view
}  // namespace ux
}  // namespace kekw
