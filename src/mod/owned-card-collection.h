#pragma once

#include "card-collection.h"
#include "owned-card.h"

namespace kekw {
namespace mod {

class owned_card_collection : public card_collection<owned_card> {
   public:
    owned_card_collection(owned_card_collection const &) = delete;

    owned_card_collection();
    owned_card_collection(card_ptr_t *begin, card_ptr_t *end);

    virtual ~owned_card_collection();

    void add_card(card_ptr_t card);
    card_ptr_t remove_card(card_id_param_t id);
};

}  // namespace mod
}  // namespace kekw
