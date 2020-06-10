#pragma once

#include "hero.h"

namespace kekw {
namespace mod {

class recruit_hero : public hero {
   public:
    recruit_hero() = delete;
    recruit_hero(
        hero_id_param_t id, hero_defn_id_param_t defn_id, std::string const& name);
    virtual ~recruit_hero() {}
};

}  // namespace mod
}  // namespace kekw