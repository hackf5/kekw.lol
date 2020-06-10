#include "recruit-card.h"

using namespace kekw::mod;

recruit_card::recruit_card(
    card_id_param_t id, card_defn_id_param_t defn_id, std::string const& name)
    : card(id, defn_id, name) {}