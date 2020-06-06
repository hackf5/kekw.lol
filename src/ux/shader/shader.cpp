#include "shader.h"

#include <src/util/file-utils.h>

#include <stdexcept>

kekw::ux::Shader::Shader() : id_(0), shaders_() {}

kekw::ux::Shader::~Shader() {
    this->DeleteShaders();
    glDeleteProgram(this->id_);
}

GLuint kekw::ux::Shader::ProgramId() const { return this->id_; }

void kekw::ux::Shader::AddStageFile(GLenum stage, std::string const &path) {
    if (this->id_) {
        throw std::runtime_error("Cannot add stages to a compiled program.");
    }

    this->AddStage(stage, kekw::util::read_file(kekw::util::get_absolute_path(path).string()));
}

void kekw::ux::Shader::AddStage(GLenum stage, std::string const &source) {
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

void kekw::ux::Shader::Compile() {
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
    this->DeleteShaders();

    glGetProgramiv(this->id_, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(this->id_, 512, NULL, infoLog);
        auto message = "Failed to compile program. Error: " + std::string(infoLog);
        throw std::runtime_error(message);
    };
}

void kekw::ux::Shader::Use() const {
    if (!this->id_) {
        throw std::runtime_error("Program is already compiled.");
    }

    glUseProgram(this->id_);
}

void kekw::ux::Shader::Set(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(this->id_, name.c_str()), (int)value);
}

void kekw::ux::Shader::Set(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(this->id_, name.c_str()), (int)value);
}

void kekw::ux::Shader::Set(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(this->id_, name.c_str()), value);
}

void kekw::ux::Shader::DeleteShaders() {
    for (auto it = this->shaders_.begin(); it != this->shaders_.end(); ++it) {
        glDeleteShader(std::get<1>(*it));
    }
}