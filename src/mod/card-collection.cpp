#include "card-collection.h"
#include "card.h"

#include <fmt/format.h>
#include <src/util/tools.h>
#include <cassert>
#include <algorithm>

kekw::mod::card_collection::card_collection(size_t max_size) : cards_(), max_size_(max_size), card_ids_() {}

kekw::mod::card_collection::~card_collection() {}

size_t kekw::mod::card_collection::size() const { return this->cards_.size(); }

size_t kekw::mod::card_collection::max_size() const { return this->max_size_; }

void kekw::mod::card_collection::add_card(card_ptr_t card) { this->insert_card(std::move(card), this->size()); }

void kekw::mod::card_collection::insert_card(card_ptr_t card, size_t index) {
    if (this->size() >= this->max_size()) {
        throw std::out_of_range(fmt::format("collection is at max size ({0})", this->max_size()));
    }

    if (this->size() < index) {
        throw std::out_of_range(fmt::format("index ({0}) is larger than size ({1})", index, this->size()));
    }

    if (index < 0) {
        throw std::out_of_range(fmt::format("index ({0}) cannot be less than zero", index, this->size()));
    }

    auto insert = this->card_ids_.insert(card->id());
    if (!insert.second) {
        throw std::runtime_error(fmt::format("collection already contains card ({0})", card->id()));
    }

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