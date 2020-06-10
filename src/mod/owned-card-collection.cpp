#include "owned-card-collection.h"

using namespace kekw::mod;
typedef owned_card_collection::card_ptr_t card_ptr_t;

owned_card_collection::owned_card_collection(index_t max_size)
    : card_collection(max_size) {}

owned_card_collection::owned_card_collection(
    card_ptr_t *begin, card_ptr_t *end, index_t max_size)
    : card_collection(begin, end, max_size) {}

owned_card_collection::~owned_card_collection() {}

void owned_card_collection::add_card(card_ptr_t card) {
    card_collection::add_card(std::move(card));
}

card_ptr_t owned_card_collection::remove_card(card_id_param_t id) {
    return card_collection::remove_card(id);
}