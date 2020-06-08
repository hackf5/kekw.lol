#pragma once

#include "card-collection.h"
#include "available-card.h"

namespace kekw {
namespace mod {

class available_card_collection : public card_collection {
    available_card_collection() = delete;
    available_card_collection(available_card_collection const&) = delete;
    ~available_card_collection();

    void add_card(card_ptr_t card);
    card_ptr_t remove_card(card_id_param_t id);
    card_ptr_t replace_card(card_id_param_t id, card_ptr_t new_card);

    const_cards_iterator_t begin() const;
    const_cards_iterator_t end() const;
};

}  // namespace mod
}  // namespace kekw