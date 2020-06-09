#pragma once

#include "card-collection.h"
#include "available-card.h"

namespace kekw {
namespace mod {

class available_card_collection : public card_collection<available_card> {
   public:
    available_card_collection() = delete;
    available_card_collection(available_card_collection const &) = delete;
    available_card_collection(size_t max_size);
    available_card_collection(card_ptr_t *begin, card_ptr_t *end, size_t max_size);

    virtual ~available_card_collection();

    void add_card(card_ptr_t card);
    card_ptr_t remove_card(card_id_param_t id);
    card_ptr_t replace_card(card_id_param_t id, card_ptr_t new_card);
};

}  // namespace mod
}  // namespace kekw