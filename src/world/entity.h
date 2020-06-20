#pragma once

#include "spatial.h"
#include "types.h"

#include <vector>
#include <memory>

namespace kekw {
namespace world {

class update_context;
class render_context;

class entity : public spatial {
   public:
    virtual ~entity() {}
    virtual void on_update(update_context* context) {}
    virtual void on_render(render_context* context) {}
};

}  // namespace world
}  // namespace kekw