#pragma once

#include <inc/glad/glad.h>
#include <glm/glm.hpp>

#include <fstream>
#include <memory>
#include <string>
#include <tuple>
#include <vector>
#include <string_view>

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

    void set(std::string const &name, GLboolean value) const;
    void set(std::string const &name, GLint value) const;
    void set(std::string const &name, GLfloat value) const;
    void set(std::string const &name, glm::mat4 const &value) const;

   private:
    typedef std::tuple<GLenum, GLuint> stage_t;

    GLuint id_;
    std::vector<stage_t> shaders_;

    void delete_shaders();
};

shader *register_shader(std::string const &name);
const shader *get_shader(std::string const &name);

}  // namespace ux
}  // namespace kekw
