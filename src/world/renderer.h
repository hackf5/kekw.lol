#pragma once

#include "types.h"

namespace kekw::world {

class renderer {
   public:
    virtual ~renderer() {}
    virtual void render(mat4_param_t model) = 0;
};

}  // namespace kekw::world