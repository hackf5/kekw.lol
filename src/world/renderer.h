#pragma once

#include "types.h"
#include <src/ux/shader/shader.h>

namespace kekw::world {

class entity;

class renderer {
   public:
    virtual ~renderer() {}

    virtual const kekw::ux::shader* get_shader() const = 0;

    virtual void render(const entity* entity) = 0;
};

}  // namespace kekw::world