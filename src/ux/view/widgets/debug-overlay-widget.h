#pragma once

#include "../ux-window-layer.h"

#include <memory>

namespace kekw {
namespace ux {
namespace view {
namespace widgets {

class debug_overlay_widget : public ux_window_widget {
   public:
    void update(window_info *info) override;

   private:
};

}  // namespace widgets
}  // namespace view
}  // namespace ux
}  // namespace kekw
