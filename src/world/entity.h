#pragma once

#include "spatial.h"

#include <vector>
#include <memory>

namespace kekw {

class initialize_context;
class update_context;
class render_context;

class entity : public spatial {
   public:
    entity() : spatial(), id_(++ID) {}

    virtual void on_initialize(initialize_context* context){};
    virtual void on_update(update_context* context){};
    virtual void on_late_update(update_context* context){};
    virtual void on_render(render_context* context){};

    inline unsigned long id() { return this->id_; }

   private:
    static unsigned long ID;
    unsigned long id_;
};

}  // namespace kekw