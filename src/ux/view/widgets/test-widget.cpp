#include "test-widget.h"

#include <inc/imgui/imgui.h>

using namespace kekw::ux::view::widgets;

void test_widget::render(ux_window_layer *layer) {
    if (ImGui::Begin("Test 1")) {
        ImGui::Button("Clear");
        ImGui::SameLine();
        ImGui::Button("Copy");
    }
    ImGui::End();  // Test 1

    if (ImGui::Begin("Test 2")) {
        ImGui::Button("Clear");
        ImGui::SameLine();
        ImGui::Button("Copy");
    }
    ImGui::End();  // Test 2
}