#include "card-collection.h"
#include "card.h"

#include <src/util/tools.h>
#include <cassert>
#include <algorithm>

kekw::mod::card_collection::card_collection(size_t max_size) : cards_(), max_size_(max_size) {}

kekw::mod::card_collection::~card_collection() {}

size_t kekw::mod::card_collection::size() const { return this->cards_.size(); }

size_t kekw::mod::card_collection::max_size() const { return this->max_size_; }

void kekw::mod::card_collection::add_card(std::unique_ptr<card> card) {
    assert(this->size() < this->max_size());

    this->cards_.push_back(std::move(card));
}

void kekw::mod::card_collection::insert_card(std::unique_ptr<card> card, size_t index) {
    assert(this->size() < this->max_size());

    this->cards_.insert(this->cards_.begin() + index, std::move(card));
}

void kekw::mod::card_collection::remove_card(card_id_param_t id) {
    auto it = this->find_card(id);
    if (it == this->cards_.end()) {
        return;
    }

    this->cards_.erase(it);
}

void kekw::mod::card_collection::move_card(card_id_param_t id, size_t index) {
    assert(index < this->size());
    auto index_it = this->cards_.begin() + index;

    auto id_it = this->find_card(id);
    assert(id_it != this->cards_.end());

    if (id_it == index_it) {
        return;
    }

    if (id_it < index_it) {
        std::rotate(id_it, id_it + 1, index_it);
    } else {
        std::rotate(index_it, index_it + 1, id_it);
    }
}

kekw::mod::card_collection::cards_iterator_t kekw::mod::card_collection::find_card(card_id_param_t id) {
    // duplicating code for readability
    return std::find_if(
        this->cards_.begin(), this->cards_.end(), [=](card_ptr_t const& card_ptr) { return card_ptr->id() == id; });
}

kekw::mod::card_collection::const_cards_iterator_t kekw::mod::card_collection::find_card(card_id_param_t id) const {
    return std::find_if(
        this->cards_.begin(), this->cards_.end(), [=](card_ptr_t const& card_ptr) { return card_ptr->id() == id; });
}