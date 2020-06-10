#pragma once

#include "card-collection.h"
#include "recruit-card.h"

namespace kekw {
namespace mod {

class recruit_card_collection : public card_collection<recruit_card> {
   public:
    recruit_card_collection() = delete;
    recruit_card_collection(recruit_card_collection const &) = delete;
    recruit_card_collection(index_t max_size);
    recruit_card_collection(card_ptr_t *begin, card_ptr_t *end, index_t max_size);

    virtual ~recruit_card_collection();

    void insert_card(card_ptr_t card, index_t index);
    card_ptr_t remove_card(card_id_param_t id);
    card_ptr_t replace_card(card_id_param_t id, card_ptr_t new_card);
    void move_card(card_id_param_t id, index_t index);
};

}  // namespace mod
}  // namespace kekw
