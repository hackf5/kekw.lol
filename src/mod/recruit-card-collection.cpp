#include "recruit-card-collection.h"

using namespace kekw::mod;
typedef recruit_card_collection::card_ptr_t card_ptr_t;

recruit_card_collection::recruit_card_collection(index_t max_size)
    : card_collection(max_size) {}

recruit_card_collection::recruit_card_collection(
    card_ptr_t *begin, card_ptr_t *end, index_t max_size)
    : card_collection(begin, end, max_size) {}

recruit_card_collection::~recruit_card_collection() {}

void recruit_card_collection::insert_card(card_ptr_t card, index_t index) {
    card_collection::insert_card(std::move(card), index);
}

card_ptr_t recruit_card_collection::remove_card(card_id_param_t id) {
    return card_collection::remove_card(id);
}

card_ptr_t recruit_card_collection::replace_card(
    card_id_param_t id, card_ptr_t new_card) {
    return card_collection::replace_card(id, std::move(new_card));
}

void recruit_card_collection::move_card(card_id_param_t id, index_t index) {
    card_collection::move_card(id, index);
}