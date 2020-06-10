#pragma once

#include "card.h"

namespace kekw {
namespace mod {

class owned_card : public card {
   public:
    owned_card() = delete;
    owned_card(owned_card const&) = delete;

    owned_card(card_id_param_t id, card_defn_id_param_t defn_id, std::string const& name);
};

}  // namespace mod
}  // namespace kekw