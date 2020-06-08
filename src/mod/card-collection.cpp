#include "card-collection.h"
#include "card.h"

#include <fmt/format.h>
#include <src/util/tools.h>
#include <cassert>
#include <algorithm>
#include <iterator>

kekw::mod::card_collection::card_collection(size_t max_size)
    : cards_(), max_size_(max_size), card_ids_() {}

kekw::mod::card_collection::~card_collection() {}

size_t kekw::mod::card_collection::size() const { return this->cards_.size(); }

size_t kekw::mod::card_collection::max_size() const { return this->max_size_; }

std::vector<kekw::mod::card_id_t> kekw::mod::card_collection::get_card_ids() const {
    std::vector<card_id_t> card_ids;
    std::for_each(this->begin(), this->end(), [&card_ids](card_ptr_t const& card_ptr) {
        card_ids.push_back(card_ptr->id());
    });
    return card_ids;
}

void kekw::mod::card_collection::add_card(card_ptr_t card) {
    this->insert_card(std::move(card), this->size());
}

void kekw::mod::card_collection::insert_card(card_ptr_t card, size_t index) {
    if (this->size() >= this->max_size()) {
        throw std::out_of_range(
            fmt::format("collection is at max size ({0})", this->max_size()));
    }

    // note that size_t is unsigned, so no need to check index < 0
    if (this->size() < index) {
        throw std::invalid_argument(
            fmt::format("index ({0}) is larger than size ({1})", index, this->size()));
    }

    auto insert = this->card_ids_.insert(card->id());
    if (!insert.second) {
        throw std::invalid_argument(
            fmt::format("collection already contains card ({0})", card->id()));
    }

    this->cards_.insert(this->cards_.begin() + index, std::move(card));
}

void kekw::mod::card_collection::remove_card(card_id_param_t id) {
    if (!this->card_ids_.erase(id)) {
        throw std::invalid_argument(
            fmt::format("collection does not contain card ({0})", id));
    }

    auto it = this->find_card(id);
    assert(it != this->cards_.end());
    this->cards_.erase(it);
}

void kekw::mod::card_collection::move_card(card_id_param_t id, size_t index) {
    if (this->size() <= index) {
        throw std::invalid_argument(fmt::format(
            "index ({0}) must be strictly less than size ({1})", index, this->size()));
    }

    auto index_it = this->cards_.begin() + index;

    auto id_it = this->find_card(id);
    if (id_it == this->cards_.end()) {
        throw std::invalid_argument(
            fmt::format("collection does not contain card ({0})", id));
    }

    if (id_it == index_it) {
        // card is already in position.
        return;
    }

    if (index_it < id_it) {
        auto id_rit = std::make_reverse_iterator(id_it);
        auto index_rit = std::make_reverse_iterator(index_it);
        std::rotate(id_rit - 1, id_rit, index_rit);
    } else {
        std::rotate(id_it, id_it + 1, index_it + 1);
    }
}

kekw::mod::card_collection::cards_iterator_t kekw::mod::card_collection::find_card(
    card_id_param_t id) {
    // duplicating code for performance.
    return std::find_if(
        this->cards_.begin(), this->cards_.end(), [=](card_ptr_t const& card_ptr) {
            return card_ptr->id() == id;
        });
}

kekw::mod::card_collection::const_cards_iterator_t kekw::mod::card_collection::find_card(
    card_id_param_t id) const {
    return std::find_if(
        this->cards_.begin(), this->cards_.end(), [=](card_ptr_t const& card_ptr) {
            return card_ptr->id() == id;
        });
}

bool kekw::mod::card_collection::contains_card(card_id_param_t id) const {
    return this->card_ids_.find(id) != this->card_ids_.end();
}

kekw::mod::card_collection::cards_iterator_t kekw::mod::card_collection::begin() {
    return this->cards_.begin();
}

kekw::mod::card_collection::cards_iterator_t kekw::mod::card_collection::end() {
    return this->cards_.end();
}

kekw::mod::card_collection::const_cards_iterator_t kekw::mod::card_collection::begin()
    const {
    return this->cards_.begin();
}

kekw::mod::card_collection::cards_t::const_iterator kekw::mod::card_collection::end()
    const {
    return this->cards_.end();
}

kekw::mod::card_collection::cards_reverse_iterator_t
kekw::mod::card_collection::rbegin() {
    return this->cards_.rbegin();
}

kekw::mod::card_collection::cards_reverse_iterator_t kekw::mod::card_collection::rend() {
    return this->cards_.rend();
}

kekw::mod::card_collection::cards_t::const_reverse_iterator
kekw::mod::card_collection::rbegin() const {
    return this->cards_.rbegin();
}

kekw::mod::card_collection::cards_t::const_reverse_iterator
kekw::mod::card_collection::rend() const {
    return this->cards_.rend();
}
