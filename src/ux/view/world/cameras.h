#pragma once

#include "scene.h"

namespace kekw {
namespace ux {
namespace view {
namespace world {

class camera_base : public camera {
    vec3_ret_t get_position() const override;
    void set_position(vec3_param_t value) const override;

    mat4_ret_t get_projection() const override;

    vec4_ret_t get_viewport() const override;
    void set_viewport(vec4_param_t value) override;

    float get_field_of_view() const override;
    void set_field_of_view(float value) override;

    float get_aspect_ratio() const override;
    void set_aspect_ratio(float value) override;

    vec2_ret_t get_clip_plane() const override;
    void set_clip_plane(vec2_param_t value) override;

    vec3 to_screen_coords(vec3_param_t world) const override;
    vec3 to_world_coords(vec3_param_t screen) const override;

    std::string to_string() override;

   protected:
    virtual void on_recalculate();
    void recalculate_if_dirty();
    void make_dirty();

   private:
    float field_of_view_;
    float aspect_ratio_;
    vec2 clip_plane_;
    vec4 viewport_;

    vec3 position_;
    mat4 projection_;
    bool dirty_;
};

class fixed_camera : public camera_base {};

}  // namespace world
}  // namespace view
}  // namespace ux
}  // namespace kekw