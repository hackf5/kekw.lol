#pragma once

#include <glad/glad.h>

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
    void AddStage(GLenum stage, const char *source);
    void Compile();
    void Use() const;

    void Set(const std::string &name, bool value) const;
    void Set(const std::string &name, int value) const;
    void Set(const std::string &name, float value) const;

   private:
    typedef std::tuple<GLenum, GLuint> stage_t;

    GLuint id_;
    const std::unique_ptr<std::vector<stage_t>> shaders_;

    void DeleteShaders();
};
}  // namespace ux
}  // namespace kekw