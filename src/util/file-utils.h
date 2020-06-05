#pragma once

#include <filesystem>
#include <fstream>
#include <memory>
#include <string>

namespace fs = std::filesystem;

namespace kekw {
namespace util {

std::string read_fstream(std::ifstream& stream);
std::string read_file(std::string const& path, bool is_binary = false);

class AppFilepaths {
   public:
    AppFilepaths(std::string const& exe_path);
    fs::path GetAbsolutePath(std::string const& relative_path) const;

   private:
    std::unique_ptr<fs::path> exe_path_;
};

}  // namespace util
}  // namespace kekw