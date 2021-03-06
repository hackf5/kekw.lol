#include "shader.h"

#include <src/ux/util/file-utils.h>

#include <glm/ext.hpp>
#include <spdlog/fmt/bundled/format.h>

#include <stdexcept>
#include <unordered_map>

using namespace kekw;

shader::shader() : id_(0), shaders_() {}

shader::~shader() {
    this->delete_shaders();
    glDeleteProgram(this->id_);
}

GLuint shader::program_id() const { return this->id_; }

void shader::add_stage_file(GLenum stage, std::string const &path) {
    if (this->id_) {
        throw std::runtime_error("Cannot add stages to a compiled program.");
    }

    this->add_stage(stage, read_file(get_absolute_path(path).string()));
}

void shader::add_stage(GLenum stage, std::string const &source) {
    if (this->id_) {
        throw std::runtime_error("Cannot add stages to a compiled program.");
    }

    auto shader = glCreateShader(stage);
    this->shaders_.push_back({stage, shader});

    auto source_cstr = source.c_str();
    glShaderSource(shader, 1, &source_cstr, NULL);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        auto message = "Failed to compile shader. Error: " + std::string(infoLog);
        throw std::runtime_error(message);
    };
}

void shader::compile() {
    if (this->id_) {
        throw std::runtime_error("Program is already compiled.");
    }

    if (!this->shaders_.size()) {
        throw std::runtime_error("Cannot compile program with no stages.");
    }

    this->id_ = glCreateProgram();

    for (auto it = this->shaders_.begin(); it != this->shaders_.end(); ++it) {
        glAttachShader(this->id_, std::get<1>(*it));
    }

    int success;
    glLinkProgram(this->id_);
    this->delete_shaders();

    glGetProgramiv(this->id_, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(this->id_, 512, NULL, infoLog);
        auto message = "Failed to compile program. Error: " + std::string(infoLog);
        throw std::runtime_error(message);
    };
}

void shader::use() const {
    if (!this->id_) {
        throw std::runtime_error("Program is not compiled.");
    }

    glUseProgram(this->id_);
}

void shader::set(const std::string &name, GLboolean value) const {
    glUniform1i(glGetUniformLocation(this->id_, name.c_str()), (int)value);
}

void shader::set(const std::string &name, GLint value) const {
    glUniform1i(glGetUniformLocation(this->id_, name.c_str()), (int)value);
}

void shader::set(const std::string &name, GLfloat value) const {
    glUniform1f(glGetUniformLocation(this->id_, name.c_str()), value);
}

void shader::set(const std::string &name, mat4_param_t value) const {
    glUniformMatrix4fv(
        glGetUniformLocation(this->id_, name.c_str()),
        1,
        GL_FALSE,
        glm::value_ptr(value));
}

void shader::set(const std::string &name, vec4_param_t value) const {
    glUniform4fv(glGetUniformLocation(this->id_, name.c_str()), 1, glm::value_ptr(value));
}

void shader::delete_shaders() {
    for (auto it = this->shaders_.begin(); it != this->shaders_.end(); ++it) {
        glDeleteShader(std::get<1>(*it));
    }
}

static auto map_instance = std::unordered_map<std::string, std::unique_ptr<shader>>();

shader *kekw::register_shader(std::string const &name) {
    if (map_instance.count(name)) {
        auto message = fmt::format("shader '{0}' is already registered", name);
        throw std::invalid_argument(message);
    }

    map_instance.insert(std::make_pair(name, std::make_unique<shader>()));
    return map_instance.find(name)->second.get();
}

const shader *kekw::get_shader(std::string const &name) {
    return map_instance.find(name)->second.get();
}