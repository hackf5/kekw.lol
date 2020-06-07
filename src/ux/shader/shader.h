#pragma once

#include <inc/glad/glad.h>

#include <fstream>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

namespace kekw {
namespace ux {

class shader {
   public:
    shader();
    virtual ~shader();

    GLuint program_id() const;
    void add_stage(GLenum stage, std::string const &source);
    void add_stage_file(GLenum stage, std::string const &path);
    void compile();
    void use() const;

    void set(std::string const &name, bool value) const;
    void set(std::string const &name, int value) const;
    void set(std::string const &name, float value) const;

   private:
    typedef std::tuple<GLenum, GLuint> stage_t;

    GLuint id_;
    std::vector<stage_t> shaders_;

    void delete_shaders();
};
}  // namespace ux
}  // namespace kekw
