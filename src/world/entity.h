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
    entity() : spatial(), id_(entity_id::next()) {}

    virtual void on_initialize(initialize_context* context){};
    virtual void on_update(update_context* context){};
    virtual void on_late_update(update_context* context){};
    virtual void on_render(render_context* context){};

    inline entity_id_t id() const { return this->id_; }

   private:
    entity_id_t id_;
};

}  // namespace kekw