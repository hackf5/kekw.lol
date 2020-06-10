#include "recruit_hero.h"

using namespace kekw::mod;

recruit_hero::recruit_hero(
    hero_id_param_t id, hero_defn_id_param_t defn_id, std::string const& name)
    : hero(id, defn_id, name) {}