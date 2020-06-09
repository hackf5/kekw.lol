#include "owned-card-collection.h"

using namespace kekw::mod;
typedef owned_card_collection::card_ptr_t card_ptr_t;

owned_card_collection::owned_card_collection() : card_collection(10) {}

owned_card_collection::owned_card_collection(card_ptr_t *begin, card_ptr_t *end)
    : card_collection(begin, end, 10) {}

owned_card_collection::~owned_card_collection() {}

void owned_card_collection::add_card(card_ptr_t card) {
    card_collection::add_card(std::move(card));
}

card_ptr_t owned_card_collection::remove_card(card_id_param_t id) {
    return card_collection::remove_card(id);
}