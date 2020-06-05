#include "ux-window-layer.h"

kekw::ux::view::UxWindowLayer::~UxWindowLayer() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void kekw::ux::view::UxWindowLayer::Initialize(WindowInfo *info) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();

    auto SCALE = 2.0f;
    ImFontConfig cfg;
    cfg.SizePixels = 13 * SCALE;
    ImGui::GetIO().Fonts->AddFontDefault(&cfg)->DisplayOffset.y = SCALE;
    ImGui::GetStyle().ScaleAllSizes(SCALE);

    ImGui::GetIO().FontGlobalScale = SCALE / 2;

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(info->getWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
}

void kekw::ux::view::UxWindowLayer::Render(WindowInfo *info) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    bool show_demo_window = true;
    if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}