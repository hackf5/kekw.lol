#pragma once

#include <filesystem>
#include <fstream>
#include <memory>
#include <string>

namespace fs = std::filesystem;

namespace kekw {
namespace util {

void set_exe_path(std::string const& exe_path);
fs::path get_absolute_path(std::string const& relative_path);
std::string read_fstream(std::ifstream& stream);
std::string read_file(std::string const& path, bool is_binary = false);

}  // namespace util
}  // namespace kekw