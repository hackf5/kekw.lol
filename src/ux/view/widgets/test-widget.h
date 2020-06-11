#pragma once

#include "../ux-window-layer.h"

namespace kekw {
namespace ux {
namespace view {
namespace widgets {

class test_widget : public ux_window_widget {
   public:
    void render(ux_window_layer *layer) override;
};

}  // namespace widgets
}  // namespace view
}  // namespace ux
}  // namespace kekw
