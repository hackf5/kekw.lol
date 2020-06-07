#pragma once

#include "mod_types.h"
#include <string>

namespace kekw {
namespace mod {

class card {
   public:
    card(card_id_param_t id, card_type_id_param_t type_id, std::string const& name);
    virtual ~card() = 0;

    card_id_ret_t id() const;
    card_type_id_ret_t type_id() const;
    const std::string& name() const;

   private:
    const card_id_t id_;
    const card_type_id_t type_id_;
    const std::string name_;
};

class card_foo {};

}  // namespace mod
}  // namespace kekw