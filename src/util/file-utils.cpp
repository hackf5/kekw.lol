#include "file-utils.h"

#include <iterator>
#include <stdexcept>

std::string kekw::util::read_fstream(std::ifstream& stream) {
    return std::string(std::istreambuf_iterator<char>{stream}, {});
}

std::string kekw::util::read_file(std::string const& path, bool is_binary) {
    auto openmode = std::ios::in | std::ios::ate;
    if (is_binary) {
        openmode |= std::ios::binary;
    }

    std::ifstream stream(path, openmode);
    stream.seekg(0, std::ios::beg);
    return read_fstream(stream);
}

kekw::util::AppFilepaths::AppFilepaths(std::string const& exe_path) : exe_path_(new fs::path(exe_path)) {
}

fs::path kekw::util::AppFilepaths::GetAbsolutePath(std::string const& relative_path) const{
    return this->exe_path_->parent_path() / relative_path;
}