#pragma once

#include <filesystem>
#include <fstream>
#include <memory>
#include <string>

namespace kekw {

void set_exe_path(std::string const& exe_path);

std::filesystem::path get_absolute_path(std::string const& relative_path);

std::string read_fstream(std::ifstream& stream);

std::string read_file(std::string const& path, bool is_binary = false);

}  // namespace kekw