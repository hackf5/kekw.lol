#pragma once

#include <src/world/types.h>
#include <tweeny.h>

namespace kekw {

class drag_data {
   public:
    virtual ~drag_data() = 0;
    virtual vec3 offset() = 0;
};

class fixed_drag_data : public drag_data {
   public:
    fixed_drag_data(vec3 offset) : offset_(offset) {}

    inline vec3 offset() override { return this->offset_; }

   private:
    vec3 offset_;
};

class tween_drag_data : public drag_data {
   public:
    tween_drag_data(tweeny::tween<float, float, float> tween);

    vec3 offset() override;

   private:
    double last_offset_time_;
    tweeny::tween<float, float, float> tween_;
};

}  // namespace kekw
