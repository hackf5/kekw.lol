#pragma once

#include <inc/glad/glad.h>

#include <fstream>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

namespace kekw {
namespace ux {

class Shader {
   public:
    Shader();
    virtual ~Shader();

    GLuint ProgramId() const;
    void AddStage(GLenum stage, std::string const &source);
    void Compile();
    void Use() const;

    void Set(std::string const &name, bool value) const;
    void Set(std::string const &name, int value) const;
    void Set(std::string const &name, float value) const;

   private:
    typedef std::tuple<GLenum, GLuint> stage_t;

    GLuint id_;
    const std::unique_ptr<std::vector<stage_t>> shaders_;

    void DeleteShaders();
};
}  // namespace ux
}  // namespace kekw
