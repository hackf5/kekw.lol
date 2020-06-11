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
    typedef typename available_card_collection::view_t available_view_t;
    typedef typename owned_card_collection::view_t owned_view_t;
    typedef typename recruit_card_collection::view_t recruit_view_t;

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
    owned_card const* own(card_id_param_t card_id);

    // moves a card from owned to recruit
    recruit_card const* recruit(card_id_param_t card_id, index_t index);

    // dismisses a recruited card
    std::unique_ptr<recruit_card> dismiss(card_id_param_t card_id);

    available_view_t const* available_view() const;
    owned_view_t const* owned_view() const;
    recruit_view_t const* recruit_view() const;

   private:
    std::unique_ptr<recruit_hero> hero_;
    std::unique_ptr<available_card_collection> available_cards_;
    std::unique_ptr<owned_card_collection> owned_cards_;
    std::unique_ptr<recruit_card_collection> recruit_cards_;
    std::unique_ptr<recruit_card_factory> card_factory_;
};

}  // namespace mod
}  // namespace kekw
