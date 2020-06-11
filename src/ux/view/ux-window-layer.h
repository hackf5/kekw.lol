#pragma once

#include "window-manager.h"

#include <memory>
#include <vector>

namespace kekw {
namespace ux {
namespace view {

class ux_window_layer;

class ux_window_widget {
   public:
    virtual ~ux_window_widget() = 0;

    virtual void initialize(ux_window_layer *layer);
    virtual void render(ux_window_layer *layer);
};

class ux_window_layer : public window_layer {
   public:
    ~ux_window_layer();

    void add_widget(std::unique_ptr<ux_window_widget> widget);
    void initialize(window_info *info) override;
    void render(window_info *info) override;

   private:
    std::vector<std::unique_ptr<ux_window_widget>> widgets_;
};

}  // namespace view
}  // namespace ux
}  // namespace kekw