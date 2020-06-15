#pragma once

#include <glm/glm.hpp>

#include <boost/call_traits.hpp>

#include <string>
#include <vector>
#include <memory>

namespace kekw {
namespace ux {
namespace view {

// forward declaration
class window_info;

namespace world {

class camera;
class body;
class body_instance;
class scene_object;
class scene;

typedef glm::vec2 vec2;
typedef boost::call_traits<vec2>::param_type vec2_param_t;
typedef boost::call_traits<vec2>::const_reference vec2_ret_t;

typedef glm::vec3 vec3;
typedef boost::call_traits<vec3>::param_type vec3_param_t;
typedef boost::call_traits<vec3>::const_reference vec3_ret_t;

typedef glm::vec4 vec4;
typedef boost::call_traits<vec4>::param_type vec4_param_t;
typedef boost::call_traits<vec4>::const_reference vec4_ret_t;

typedef glm::mat4 mat4;
typedef boost::call_traits<mat4>::param_type mat4_param_t;
typedef boost::call_traits<mat4>::const_reference mat4_ret_t;

typedef std::tuple<vec3, vec3, vec3> triangle_t;
typedef std::vector<triangle_t> mesh_t;

typedef kekw::ux::view::window_info window_info_t;

class camera {
   public:
    virtual ~camera() = 0;

    virtual vec3_ret_t get_position() const = 0;
    virtual void set_position(vec3_param_t value) const = 0;

    virtual mat4_ret_t get_projection() const = 0;
    virtual mat4_ret_t get_view() const = 0;

    virtual vec4_ret_t get_viewport() const = 0;
    virtual void set_viewport(vec4_param_t value) = 0;

    virtual float get_field_of_view() const = 0;
    virtual void set_field_of_view(float value) = 0;

    virtual float get_aspect_ratio() const = 0;
    virtual void set_aspect_ratio(float value) = 0;

    virtual vec2_ret_t get_clip_plane() const = 0;
    virtual void set_clip_plane(vec2_param_t value) = 0;

    virtual vec3 to_screen_coords(vec3_param_t world) const = 0;
    virtual vec3 to_world_coords(vec3_param_t screen) const = 0;

    virtual std::string to_string() = 0;
};

class body {
   public:
    virtual ~body() = 0;
    virtual mesh_t const& get_triangles() const = 0;
};

class body_instance {
   public:
    body_instance(std::shared_ptr<body> body);

    virtual ~body_instance() = 0;

    virtual mat4_ret_t get_model() const;

    void set_position(vec3_param_t position);

    void translate(vec3_param_t value);

    mesh_t const& get_triangles();

   private:
    std::shared_ptr<body> body_;
    mat4 translation_;
    mesh_t triangles_;
    bool dirty_;
};

class scene_object {
   public:
    virtual ~scene_object() = 0;

    virtual void initialize(scene* scene) = 0;
    virtual void render();
};

class scene {
   public:
    virtual ~scene() = 0;

    virtual camera* get_camera();

    virtual void initialize(window_info_t* window_info) = 0;
    virtual void begin_frame(window_info_t* window_info) = 0;
    virtual void do_user_interaction() = 0;
    virtual void render();
    virtual void end_frame() = 0;
};

}  // namespace world
}  // namespace view
}  // namespace ux
}  // namespace kekw
