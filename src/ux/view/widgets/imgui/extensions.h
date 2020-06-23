#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

namespace kekw {

void imgui_begin_disable_item(bool enabled);

void imgui_end_disable_item(bool enabled, char const* reason = nullptr);

}  // namespace kekw