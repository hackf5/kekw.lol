#include <inc/glad/glad.h>
#include <GLFW/glfw3.h>

#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/basic_file_sink.h>

#include <src/mod/recruit-env.h>

#include <src/ux/util/file-utils.h>
#include <src/ux/view/window-manager.h>
#include <src/ux/view/ux-window-layer.h>
#include <src/ux/view/gl-window-layer.h>
#include <src/ux/view/recruit-window-layer.h>
#include <src/ux/view/widgets/test-widget.h>

#include <memory>

namespace vw = kekw::ux::view;
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
    kekw::util::set_exe_path(std::string(argv[0]));

    // TODO: there may be some perf issues here, see
    // https://github.com/gabime/spdlog/wiki
    auto file_logger = spdlog::basic_logger_mt<spdlog::async_factory>(
        "basic_logger", kekw::util::get_absolute_path("logs/log.txt").string());
    file_logger->flush_on(spdlog::level::debug);
    spdlog::set_default_logger(file_logger);

    spdlog::set_level(spdlog::level::debug);
    spdlog::set_pattern("[%H:%M:%S %z] [%n] [%^---%L---%$] [thread %t] %v");
    spdlog::info("Loading KEKW.lol... {}", "test");

    auto recruit_env = create_recruit_env(4);
    recruit_env->refresh();

    vw::window_manager manager;
    manager.add_layer(
        std::unique_ptr<vw::window_layer>(new vw::recruit_window_layer(recruit_env)));

    auto ux_layer = std::unique_ptr<vw::ux_window_layer>(new vw::ux_window_layer());
    ux_layer->add_widget(
        std::unique_ptr<vw::ux_window_widget>(new vw::widgets::test_widget(recruit_env)));
    manager.add_layer(std::move(ux_layer));

    manager.Start();

    return 0;
}