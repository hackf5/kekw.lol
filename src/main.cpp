#include <inc/glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/basic_file_sink.h>

#include <src/mod/recruit-env.h>

#include <src/ux/util/file-utils.h>

#include <src/ux/view/window-manager.h>
#include <src/ux/view/ux-window-layer.h>
#include <src/ux/view/scene-window-layer.h>

#include <src/ux/view/widgets/debug-overlay-widget.h>
#include <src/ux/view/widgets/camera-widget.hpp>

#include <src/ux/scenes/recruit/recruit-scene.h>

#include <src/world/camera.h>

#include <src/ux/scenes/common/card-entity.h>

#include <memory>

using namespace kekw;

namespace md = kekw::mod;

std::shared_ptr<md::recruit_env> create_recruit_env(md::index_t max_size) {
    return std::shared_ptr<md::recruit_env>(new md::recruit_env(
        std::unique_ptr<md::recruit_hero>(
            new md::recruit_hero(1, "dummy-hero", "cleefster")),
        std::unique_ptr<md::available_card_collection>(
            new md::available_card_collection(max_size)),
        std::unique_ptr<md::owned_card_collection>(
            new md::owned_card_collection(max_size)),
        std::unique_ptr<md::recruit_card_collection>(
            new md::recruit_card_collection(max_size)),
        std::unique_ptr<md::recruit_card_factory>(new md::recruit_card_factory())));
}

int main(int argc, char *argv[]) {
    set_exe_path(std::string(argv[0]));

    // TODO: there may be some perf issues here, see
    // https://github.com/gabime/spdlog/wiki
    auto file_logger = spdlog::basic_logger_mt<spdlog::async_factory>(
        "basic_logger", get_absolute_path("logs/log.txt").string());
    file_logger->flush_on(spdlog::level::debug);
    spdlog::set_default_logger(file_logger);

    spdlog::set_level(spdlog::level::debug);
    spdlog::set_pattern("[%H:%M:%S %z] [%n] [%^---%L---%$] [thread %t] %v");
    spdlog::info("Loading KEKW.lol... {}", "test");

    auto recruit_env = create_recruit_env(4);
    recruit_env->refresh();

    auto scene = std::make_shared<recruit_scene>(
        std::make_unique<camera>(), std::make_unique<card_entity>());

    window_manager manager;
    manager.add_layer(std::make_unique<scene_window_layer>(scene));

    auto ux_layer = std::unique_ptr<ux_window_layer>(new ux_window_layer());
    ux_layer->add_widget(std::unique_ptr<ux_window_widget>(new debug_overlay_widget()));
    ux_layer->add_widget(
        std::unique_ptr<ux_window_widget>(new camera_widget(scene->cam())));
    manager.add_layer(std::move(ux_layer));

    manager.start();

    return 0;
}