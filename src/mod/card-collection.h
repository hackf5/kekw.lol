#pragma once

#include "mod-types.h"
#include "card.h"

#include <spdlog/fmt/bundled/format.h>

#include <memory>
#include <string>
#include <vector>
#include <unordered_set>
#include <cassert>
#include <algorithm>
#include <iterator>

namespace kekw {
namespace mod {

template <typename TCard>
class card_collection {
   public:
    typedef std::unique_ptr<TCard> card_ptr_t;
    typedef card_collection<TCard> view_t;

   private:
    // prefer vector over list as although there are going to be a lot of inserts
    // because sizeof(unique_ptr<T>) == sizeof(T*) which is 8 bytes and on small
    // objects vector always performs better than list for small data types even
    // on random inserts.
    // https://stackoverflow.com/questions/13460395/how-can-stdunique-ptr-have-no-size-overhead
    // https://baptiste-wicht.com/posts/2012/12/cpp-benchmark-vector-list-deque.html
    typedef std::vector<card_ptr_t> cards_t;

   public:
    typedef typename cards_t::iterator cards_iterator_t;
    typedef typename cards_t::const_iterator const_cards_iterator_t;
    typedef typename cards_t::reverse_iterator cards_reverse_iterator_t;
    typedef typename cards_t::const_reverse_iterator const_cards_reverse_iterator_t;

   public:
    card_collection() = delete;
    card_collection(card_collection const&) = delete;
    virtual ~card_collection() = 0;

    index_t size() const;
    index_t max_size() const;
    bool is_empty() const;
    bool is_full() const;

    bool contains_card(card_id_param_t id) const;
    std::vector<card_id_t> get_card_ids() const;

    TCard const* at(index_t) const;

    const_cards_iterator_t begin() const;
    const_cards_iterator_t end() const;

    const_cards_reverse_iterator_t rbegin() const;
    const_cards_reverse_iterator_t rend() const;

    const_cards_iterator_t find_card(card_id_param_t id) const;

   protected:
    card_collection(index_t max_size);
    card_collection(card_ptr_t* begin, card_ptr_t* end, index_t max_size);

    // adds the card to the end of the collection
    void add_card(card_ptr_t card);

    // inserts the card into the collection at position index.
    void insert_card(card_ptr_t card, index_t index);

    // removes card with id and returns a pointer to it.
    card_ptr_t remove_card(card_id_param_t id);

    // moves card with id to position index.
    void move_card(card_id_param_t id, index_t index);

    // replaces the card with id with new_card and returns a pointer to the replaced card.
    card_ptr_t replace_card(card_id_param_t id, card_ptr_t new_card);

    // clears the collection.
    void clear();

    cards_iterator_t find_card(card_id_param_t id);

    cards_iterator_t begin();
    cards_iterator_t end();

    cards_reverse_iterator_t rbegin();
    cards_reverse_iterator_t rend();

   private:
    // cards contained in the collection in order.
    cards_t cards_;

    // the maximum number of elements that can be stored in the collection.
    index_t max_size_;

    // the ids of the cards in the collection.
    std::unordered_set<card_id_t> card_ids_;
};

template <typename TCard>
card_collection<TCard>::card_collection(index_t max_size)
    : cards_(), max_size_(max_size), card_ids_() {}

template <typename TCard>
card_collection<TCard>::card_collection(
    card_ptr_t* begin, card_ptr_t* end, index_t max_size)
    : cards_(), max_size_(max_size), card_ids_() {
    for (auto it = begin; it != end; ++it) {
        bool inserted;
        std::tie(std::ignore, inserted) = this->card_ids_.insert((*it)->id());
        if (!inserted) {
            throw std::invalid_argument(
                fmt::format("duplicate card found ({0})", (*it)->id()));
        }

        this->cards_.push_back(std::move(*it));
    }

    if (this->size() > this->max_size()) {
        throw std::invalid_argument(fmt::format(
            "too many cards given; size={0}, max_size={1}",
            this->size(),
            this->max_size()));
    }
}

template <typename TCard>
card_collection<TCard>::~card_collection() {}

template <typename TCard>
index_t card_collection<TCard>::size() const {
    return this->cards_.size();
}

template <typename TCard>
index_t card_collection<TCard>::max_size() const {
    return this->max_size_;
}

template <typename TCard>
std::vector<card_id_t> card_collection<TCard>::get_card_ids() const {
    std::vector<card_id_t> card_ids;
    std::for_each(this->begin(), this->end(), [&card_ids](card_ptr_t const& card_ptr) {
        card_ids.push_back(card_ptr->id());
    });
    return card_ids;
}

template <typename TCard>
void card_collection<TCard>::add_card(card_ptr_t card) {
    this->insert_card(std::move(card), this->size());
}

template <typename TCard>
void card_collection<TCard>::insert_card(card_ptr_t card, index_t index) {
    if (this->size() >= this->max_size()) {
        throw std::out_of_range(
            fmt::format("collection is at max size ({0})", this->max_size()));
    }

    // note that index_t is unsigned, so no need to check index < 0
    if (this->size() < index) {
        throw std::invalid_argument(
            fmt::format("index ({0}) is larger than size ({1})", index, this->size()));
    }

    bool inserted;
    std::tie(std::ignore, inserted) = this->card_ids_.insert(card->id());
    if (!inserted) {
        throw std::invalid_argument(
            fmt::format("collection already contains card ({0})", card->id()));
    }

    this->cards_.insert(this->cards_.begin() + index, std::move(card));
}

template <typename TCard>
typename card_collection<TCard>::card_ptr_t card_collection<TCard>::remove_card(
    card_id_param_t id) {
    if (!this->card_ids_.erase(id)) {
        throw std::invalid_argument(
            fmt::format("collection does not contain card ({0})", id));
    }

    auto it = this->find_card(id);
    assert(it != this->cards_.end());

    card_ptr_t removed;
    it->swap(removed);

    this->cards_.erase(it);

    return removed;
}

template <typename TCard>
void card_collection<TCard>::move_card(card_id_param_t id, index_t index) {
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

template <typename TCard>
typename card_collection<TCard>::card_ptr_t card_collection<TCard>::replace_card(
    card_id_param_t id, card_ptr_t new_card) {
    auto card = this->find_card(id);
    if (card == this->end()) {
        throw std::invalid_argument(
            fmt::format("collection does not contain card ({0})", id));
    }

    bool inserted;
    std::tie(std::ignore, inserted) = this->card_ids_.insert(new_card->id());
    if (!inserted) {
        throw std::invalid_argument(
            fmt::format("collection already contains card ({0})", new_card->id()));
    }

    this->card_ids_.erase((*card)->id());

    new_card.swap(*card);
    return new_card;
}

template <typename TCard>
typename card_collection<TCard>::cards_iterator_t card_collection<TCard>::find_card(
    card_id_param_t id) {
    // duplicating code for performance.
    return std::find_if(
        this->cards_.begin(), this->cards_.end(), [=](card_ptr_t const& card_ptr) {
            return card_ptr->id() == id;
        });
}

template <typename TCard>
typename card_collection<TCard>::const_cards_iterator_t card_collection<TCard>::find_card(
    card_id_param_t id) const {
    return std::find_if(
        this->cards_.begin(), this->cards_.end(), [=](card_ptr_t const& card_ptr) {
            return card_ptr->id() == id;
        });
}

template <typename TCard>
bool card_collection<TCard>::contains_card(card_id_param_t id) const {
    return this->card_ids_.find(id) != this->card_ids_.end();
}

template <typename TCard>
typename card_collection<TCard>::cards_iterator_t card_collection<TCard>::begin() {
    return this->cards_.begin();
}

template <typename TCard>
typename card_collection<TCard>::cards_iterator_t card_collection<TCard>::end() {
    return this->cards_.end();
}

template <typename TCard>
typename card_collection<TCard>::const_cards_iterator_t card_collection<TCard>::begin()
    const {
    return this->cards_.begin();
}

template <typename TCard>
typename card_collection<TCard>::const_cards_iterator_t card_collection<TCard>::end()
    const {
    return this->cards_.end();
}

template <typename TCard>
typename card_collection<TCard>::cards_reverse_iterator_t
card_collection<TCard>::rbegin() {
    return this->cards_.rbegin();
}

template <typename TCard>
typename card_collection<TCard>::cards_reverse_iterator_t card_collection<TCard>::rend() {
    return this->cards_.rend();
}

template <typename TCard>
typename card_collection<TCard>::const_cards_reverse_iterator_t
card_collection<TCard>::rbegin() const {
    return this->cards_.rbegin();
}

template <typename TCard>
typename card_collection<TCard>::const_cards_reverse_iterator_t
card_collection<TCard>::rend() const {
    return this->cards_.rend();
}

template <typename TCard>
void card_collection<TCard>::clear() {
    this->card_ids_.clear();
    this->cards_.clear();
}

template <typename TCard>
bool card_collection<TCard>::is_empty() const {
    return this->cards_.empty();
}

template <typename TCard>
bool card_collection<TCard>::is_full() const {
    return this->size() == this->max_size();
}

template <typename TCard>
TCard const* card_collection<TCard>::at(index_t index) const {
    if (this->size() <= index) {
        throw std::invalid_argument(fmt::format(
            "index ({0}) must be strictly less than size ({1})", index, this->size()));
    }

    auto& ptr = this->cards_.at(index);
    return ptr.get();
}

}  // namespace mod
}  // namespace kekw