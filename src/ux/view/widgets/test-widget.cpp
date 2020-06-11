#include "test-widget.h"

#include "imgui/extensions.h"

using namespace kekw::ux::view::widgets;

test_widget::test_widget(std::shared_ptr<kekw::mod::recruit_env> recruit_env) {
    this->recruit_env_ = recruit_env;
}

void test_widget::render(ux_window_layer *layer) {
    if (!ImGui::Begin("Add Available")) {
        ImGui::End();
        return;
    }

    bool enabled = this->recruit_env_->available_view()->is_empty();

    imgui::begin_disable_item(enabled);
    if (ImGui::Button("Add Card")) {
        this->recruit_env_->refresh();
    }

    imgui::end_disable_item(enabled);

    ImGui::End();  // Add Available
}