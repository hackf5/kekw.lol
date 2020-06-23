#pragma once

#include "../ux-window-layer.h"

#include <memory>

namespace kekw {

class debug_overlay_widget : public ux_window_widget {
   public:
    void update(window_context *info) override;

   private:
};

}  // namespace kekw
