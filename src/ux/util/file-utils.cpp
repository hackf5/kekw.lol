#include "file-utils.h"

#include <fmt/format.h>
#include <iterator>
#include <stdexcept>

namespace kekw {
namespace util {
class app_filepaths {
   public:
    app_filepaths(std::string const& exe_path);
    fs::path get_absolute_path(std::string const& relative_path) const;

   private:
    std::unique_ptr<fs::path> exe_path_;
};
}  // namespace util
}  // namespace kekw

kekw::util::app_filepaths::app_filepaths(std::string const& exe_path)
    : exe_path_(new fs::path(exe_path)) {}

fs::path kekw::util::app_filepaths::get_absolute_path(
    std::string const& relative_path) const {
    return this->exe_path_->parent_path() / relative_path;
}

std::string kekw::util::read_fstream(std::ifstream& stream) {
    return std::string(std::istreambuf_iterator<char>{stream}, {});
}

std::string kekw::util::read_file(std::string const& path, bool is_binary) {
    if (!fs::exists(path)) {
        throw std::invalid_argument(fmt::format("path ({0}) does not exist"));
    }

    auto openmode = std::ios::in | std::ios::ate;
    if (is_binary) {
        openmode |= std::ios::binary;
    }

    std::ifstream stream(path, openmode);
    stream.seekg(0, std::ios::beg);
    return read_fstream(stream);
}

static kekw::util::app_filepaths* instance;

void kekw::util::set_exe_path(std::string const& exe_path) {
    instance = new app_filepaths(exe_path);
}

fs::path kekw::util::get_absolute_path(std::string const& relative_path) {
    return instance->get_absolute_path(relative_path);
}