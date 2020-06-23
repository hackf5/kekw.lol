#pragma once

#include "window-manager.h"

#include <memory>
#include <vector>

namespace kekw {

class ux_window_widget {
   public:
    virtual ~ux_window_widget() = 0;

    virtual void initialize(window_context *info);
    virtual void update(window_context *info);
};

class ux_window_layer : public window_layer {
   public:
    ~ux_window_layer();

    void add_widget(std::unique_ptr<ux_window_widget> widget);

    void initialize(window_context *info) override;
    void update(window_context *info) override;
    void render(window_context *info) override;

   private:
    std::vector<std::unique_ptr<ux_window_widget>> widgets_;
};

}  // namespace kekw