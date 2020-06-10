#pragma once

#include "card.h"

namespace kekw {
namespace mod {

class available_card : public card {
   public:
    available_card() = delete;
    available_card(available_card const&) = delete;

    available_card(
        card_id_param_t id, card_defn_id_param_t defn_id, std::string const& name);
};

}  // namespace mod
}  // namespace kekw