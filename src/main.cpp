#include <inc/glad/glad.h>
#include <GLFW/glfw3.h>

#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/basic_file_sink.h>

#define STB_IMAGE_IMPLEMENTATION
#include <inc/stb/stb_image.h>

#include <src/util/file-utils.h>

#include <src/ux/view/window-manager.h>
#include <src/ux/view/ux-window-layer.h>
#include <src/ux/view/gl-window-layer.h>

#include <memory>

namespace vw = kekw::ux::view;

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

    vw::WindowManager manager;
    manager.AddLayer(std::unique_ptr<vw::WindowLayer>(new vw::GlWindowLayer()));
    manager.AddLayer(std::unique_ptr<vw::WindowLayer>(new vw::UxWindowLayer()));
    manager.Start();
    
    return 0;
}