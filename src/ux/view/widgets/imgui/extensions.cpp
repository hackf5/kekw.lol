#include "extensions.h"

namespace imgui_ex = kekw::ux::view::widgets::imgui;

void imgui_ex::begin_disable_item(bool enabled) {
    if (!enabled) {
        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
    }
}

void imgui_ex::end_disable_item(bool enabled, char const* reason) {
    if (!enabled) {
        ImGui::PopItemFlag();
        ImGui::PopStyleVar();

        if (reason != nullptr && reason[0] && ImGui::IsItemHovered()) {
            ImGui::BeginTooltip();
            ImGui::TextUnformatted(reason);
            ImGui::EndTooltip();
        }
    }
}