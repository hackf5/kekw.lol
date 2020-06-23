#pragma once

#include "types.h"
#include <src/ux/shader/shader.h>

namespace kekw {

class entity;

class renderer {
   public:
    virtual ~renderer() = 0;

    virtual const shader* get_shader() const = 0;
    virtual void render(const entity* entity) = 0;
};

}  // namespace kekw