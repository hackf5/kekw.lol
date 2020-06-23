#pragma once

#include <inc/glad/glad.h>
#include <src/world/types.h>

#include <memory>
#include <string>
#include <vector>

namespace kekw {

class shader {
   public:
    shader();
    virtual ~shader();

    GLuint program_id() const;
    void add_stage(GLenum stage, std::string const &source);
    void add_stage_file(GLenum stage, std::string const &path);
    void compile();
    void use() const;

    void set(std::string const &name, GLboolean value) const;
    void set(std::string const &name, GLint value) const;
    void set(std::string const &name, GLfloat value) const;
    void set(std::string const &name, mat4_param_t value) const;

   private:
    typedef std::tuple<GLenum, GLuint> stage_t;

    GLuint id_;
    std::vector<stage_t> shaders_;

    void delete_shaders();
};

shader *register_shader(std::string const &name);
const shader *get_shader(std::string const &name);

}  // namespace kekw
