#include "debug-overlay-widget.h"

#include "imgui/extensions.h"

using namespace kekw;

void debug_overlay_widget::update(window_context* context) {
    // FIXME-VIEWPORT: Select a default viewport
    const float DISTANCE = 10.0f;
    static int corner = 0;
    ImGuiIO& io = ImGui::GetIO();
    if (corner != -1) {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImVec2 work_area_pos =
            viewport->GetWorkPos();  // Instead of using viewport->Pos we use GetWorkPos()
                                     // to avoid menu bars, if any!
        ImVec2 work_area_size = viewport->GetWorkSize();
        ImVec2 window_pos = ImVec2(
            (corner & 1) ? (work_area_pos.x + work_area_size.x - DISTANCE)
                         : (work_area_pos.x + DISTANCE),
            (corner & 2) ? (work_area_pos.y + work_area_size.y - DISTANCE)
                         : (work_area_pos.y + DISTANCE));
        ImVec2 window_pos_pivot =
            ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
        ImGui::SetNextWindowViewport(viewport->ID);
    }
    ImGui::SetNextWindowBgAlpha(0.35f);  // Transparent background
    ImGuiWindowFlags window_flags =
        ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking |
        ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    if (corner != -1) window_flags |= ImGuiWindowFlags_NoMove;

    bool p_open;
    if (ImGui::Begin("debug overlay", &p_open, window_flags)) {
        ImGui::Text("Debug: (right-click to change position)");

        ImGui::Separator();
        if (context->has_focus())
            ImGui::Text(
                "Mouse Position: (%.1f,%.1f)",
                context->mouse_coords().x,
                context->mouse_coords().y);
        else
            ImGui::Text("Mouse Position: <invalid>");

        // ImGui::Text("Mouse Position World: %s", context->debug_1.c_str());

        if (ImGui::BeginPopupContextWindow()) {
            if (ImGui::MenuItem("Custom", NULL, corner == -1)) corner = -1;
            if (ImGui::MenuItem("Top-left", NULL, corner == 0)) corner = 0;
            if (ImGui::MenuItem("Top-right", NULL, corner == 1)) corner = 1;
            if (ImGui::MenuItem("Bottom-left", NULL, corner == 2)) corner = 2;
            if (ImGui::MenuItem("Bottom-right", NULL, corner == 3)) corner = 3;
            if (p_open && ImGui::MenuItem("Close")) p_open = false;
            ImGui::EndPopup();
        }
    }
    ImGui::End();
}