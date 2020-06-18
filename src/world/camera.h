#pragma once

#include "spatial.h"

namespace kekw {
namespace world {

class camera : public spatial {
   public:
    camera();
    virtual ~camera(){};

    mat4_ret_t get_projection() const;
    mat4_ret_t get_view() const;

    inline vec4_ret_t get_viewport() const { return this->viewport_; }
    inline void set_viewport(vec4_param_t value) {
        this->viewport_ = value;
        this->make_dirty();
    }

    inline float get_field_of_view() const { return this->field_of_view_; }
    inline void set_field_of_view(real_t value) {
        this->field_of_view_ = value;
        this->make_dirty();
    }

    inline float get_aspect_ratio() const { return this->aspect_ratio_; }
    inline void set_aspect_ratio(real_t value) {
        this->aspect_ratio_ = value;
        this->make_dirty();
    }

    inline vec2_ret_t get_clip_plane() const { return this->clip_plane_; }
    inline void set_clip_plane(vec2_param_t value) {
        this->clip_plane_ = value;
        this->make_dirty();
    }

    inline vec3 to_screen_coords(vec3_param_t world) const {
        return glm::project(
            world, this->get_view(), this->get_projection(), this->get_viewport());
    }

    inline vec3 to_world_coords(vec3_param_t screen) const {
        return glm::unProject(
            screen, this->get_view(), this->get_projection(), this->get_viewport());
    }

    void look_at(vec3_param_t target);

   protected:
    void on_recalculate() override;

   private:
    real_t field_of_view_;
    real_t aspect_ratio_;
    vec2 clip_plane_;
    vec4 viewport_;
    mat4 projection_;
};

}  // namespace world
}  // namespace kekw