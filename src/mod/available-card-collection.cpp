#include "available-card-collection.h"

using namespace kekw::mod;
typedef available_card_collection::card_ptr_t card_ptr_t;

available_card_collection::available_card_collection(index_t max_size)
    : card_collection(max_size) {}

available_card_collection::available_card_collection(
    card_ptr_t *begin, card_ptr_t *end, index_t max_size)
    : card_collection(begin, end, max_size) {}

available_card_collection::~available_card_collection() {}

void available_card_collection::add_card(card_ptr_t card) {
    card_collection::add_card(std::move(card));
}

card_ptr_t available_card_collection::remove_card(card_id_param_t id) {
    return card_collection::remove_card(id);
}

card_ptr_t available_card_collection::replace_card(
    card_id_param_t id, card_ptr_t new_card) {
    return card_collection::replace_card(id, std::move(new_card));
}

void available_card_collection::clear() { card_collection::clear(); }