#pragma once

#include "recruit_hero.h"
#include "available-card-collection.h"
#include "owned-card-collection.h"
#include "recruit-card-collection.h"
#include "recruit-card-factory.h"

#include <memory>

namespace kekw {
namespace mod {

class recruit_env {
   public:
    recruit_env(
        std::unique_ptr<recruit_hero> hero,
        std::unique_ptr<available_card_collection> available_cards,
        std::unique_ptr<owned_card_collection> owned_cards,
        std::unique_ptr<recruit_card_collection> recruit_cards,
        std::unique_ptr<recruit_card_factory> card_factory);

    virtual ~recruit_env() {}

    // refreshes available cards
    void refresh();

    // moves a card from available to owned
    void own(card_id_param_t card_id);

    // moves a card from owned to recruit
    void recruit(card_id_param_t card_id, index_t index);

    // dismisses a recruited card
    void dismiss(card_id_param_t card_id);

   private:
    std::unique_ptr<recruit_hero> hero_;
    std::unique_ptr<available_card_collection> available_cards_;
    std::unique_ptr<owned_card_collection> owned_cards_;
    std::unique_ptr<recruit_card_collection> recruit_cards_;
    std::unique_ptr<recruit_card_factory> card_factory_;
};

}  // namespace mod
}  // namespace kekw
