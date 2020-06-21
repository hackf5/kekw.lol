#pragma once

#include "spatial.h"

#include <vector>
#include <memory>

namespace kekw {
namespace world {

class update_context;
class render_context;

class entity : public spatial {
   public:
    virtual void on_update(update_context* context) = 0;
    virtual void on_render(render_context* context) = 0;
};

}  // namespace world
}  // namespace kekw