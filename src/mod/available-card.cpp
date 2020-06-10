#include "available-card.h"

using namespace kekw::mod;

available_card::available_card(
    card_id_param_t id, card_defn_id_param_t defn_id, std::string const& name)
    : card(id, defn_id, name) {}