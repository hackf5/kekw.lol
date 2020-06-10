#pragma once

#include "mod-types.h"
#include <string>

namespace kekw {
namespace mod {

class card {
   public:
    card() = delete;
    card(card_id_param_t id, card_defn_id_param_t defn_id, std::string const& name);
    virtual ~card() = 0;

    card_id_ret_t id() const;
    card_defn_id_ret_t defn_id() const;
    const std::string& name() const;

   private:
    const card_id_t id_;
    const card_defn_id_t defn_id_;
    const std::string name_;
};

}  // namespace mod
}  // namespace kekw