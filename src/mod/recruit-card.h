#pragma once

#include "card.h"

namespace kekw {
namespace mod {

class recruit_card : public card {
   public:
    recruit_card() = delete;
    recruit_card(recruit_card const&) = delete;

    recruit_card(
        card_id_param_t id, card_defn_id_param_t defn_id, std::string const& name);
};

}  // namespace mod
}  // namespace kekw