#pragma once

#include <inc/imgui/imgui.h>
#include <inc/imgui/imgui_internal.h>

namespace kekw {
namespace ux {
namespace view {
namespace widgets {
namespace imgui {

void begin_disable_item(bool enabled);

void end_disable_item(bool enabled, char const* reason = nullptr);

}  // namespace imgui
}  // namespace widgets
}  // namespace view
}  // namespace ux
}  // namespace kekw