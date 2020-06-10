#pragma once

#include "mod-types.h"
#include <string>

namespace kekw {
namespace mod {

class hero {
   public:
    hero() = delete;
    hero(hero_id_param_t id, hero_defn_id_param_t defn_id, std::string const& name);
    virtual ~hero() = 0;

    hero_id_ret_t id() const;
    hero_defn_id_ret_t defn_id() const;
    const std::string& name() const;

   private:
    const hero_id_t id_;
    const hero_defn_id_t defn_id_;
    const std::string name_;
};

}  // namespace mod
}  // namespace kekw