#pragma once

#include "spatial.h"

namespace kekw {
namespace world {

class camera : public spatial {
   public:
    virtual ~camera(){};

    void lool_at(vec3_param_t target);

    mat4_ret_t get_projection() const;
    mat4_ret_t get_view() const;

    inline vec4_ret_t get_viewport() const { return this->viewport_; }
    inline void set_viewport(vec4_param_t value) {
        this->viewport_ = value;
        this->make_dirty();
    }

    inline float get_field_of_view() const { return this->field_of_view_; }
    inline void set_field_of_view(float value) {
        this->field_of_view_ = value;
        this->make_dirty();
    }

    inline float get_aspect_ratio() const { return this->aspect_ratio_; }
    inline void set_aspect_ratio(float value) {
        this->aspect_ratio_ = value;
        this->make_dirty();
    }

    inline vec2_ret_t get_clip_plane() const { return this->clip_plane_; }
    inline void set_clip_plane(vec2_param_t value) {
        this->clip_plane_ = value;
        this->make_dirty();
    }

    vec3 to_screen_coords(vec3_param_t world) const;
    vec3 to_world_coords(vec3_param_t screen) const;

   protected:
    void on_recalculate() override;

   private:
    float field_of_view_;
    float aspect_ratio_;
    vec2 clip_plane_;
    vec4 viewport_;
    mat4 projection_;
};

}  // namespace world
}  // namespace kekw