#pragma once

#include "types.h"

#include <stack>

namespace kekw {

class transform {
   public:
    static const transform identity;

    static const vec3 origin;
    static const vec3 right;
    static const vec3 up;
    static const vec3 forward;

    transform();
    transform(transform const& other);

    inline vec3_ret_t position() const { return this->position_; }
    inline quat_ret_t rotation() const { return this->rotation_; }
    inline vec3_ret_t scale() const { return this->scale_; }

    mat4_ret_t matrix() const;

    inline void set_position(vec3_param_t value) {
        this->position_ = value;
        this->make_dirty();
    }

    inline void set_rotation(quat_param_t value) {
        this->rotation_ = value;
        this->make_dirty();
    }

    inline void set_scale(vec3_param_t value) {
        this->scale_ = value;
        this->make_dirty();
    }

    inline void push_scale(vec3_param_t value) {
        this->scale_stack_.push(this->scale());
        this->set_scale(value);
    }

    inline void pop_scale() {
        this->set_scale(this->scale_stack_.top());
        this->scale_stack_.pop();
    }

   protected:
    virtual void on_recalculate() {}
    void recalculate_if_dirty();
    inline void make_dirty() { this->dirty_ = true; }

   private:
    vec3 position_;
    quat rotation_;
    vec3 scale_;
    mat4 matrix_;

    std::stack<vec3> scale_stack_;

    bool dirty_;

   protected:
    bool is_mirror_;
};

}  // namespace kekw