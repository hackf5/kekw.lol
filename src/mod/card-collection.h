#pragma once

#include "mod_types.h"

#include <memory>
#include <string>
#include <vector>
#include <unordered_set>

namespace kekw {
namespace mod {

class card;

class card_collection {
   public:
    typedef std::unique_ptr<card> card_ptr_t;

   private:
    // prefer vector over list as although there are going to be a lot of inserts
    // because sizeof(unique_ptr<T>) == sizeof(T*) which is 8 bytes and on small
    // objects vector always performs better than list for small data types even
    // on random inserts.
    // https://stackoverflow.com/questions/13460395/how-can-stdunique-ptr-have-no-size-overhead
    // https://baptiste-wicht.com/posts/2012/12/cpp-benchmark-vector-list-deque.html
    typedef std::vector<card_ptr_t> cards_t;

   protected:
    typedef cards_t::iterator cards_iterator_t;
    typedef cards_t::const_iterator const_cards_iterator_t;
    typedef cards_t::reverse_iterator cards_reverse_iterator_t;
    typedef cards_t::const_reverse_iterator const_cards_reverse_iterator_t;

   public:
    card_collection() = delete;
    card_collection(card_collection const&) = delete;
    virtual ~card_collection() = 0;

    size_t size() const;
    size_t max_size() const;
    bool contains_card(card_id_param_t id) const;
    std::vector<card_id_t> get_card_ids() const;

   protected:
    card_collection(size_t max_size);

    void add_card(card_ptr_t card);
    void insert_card(card_ptr_t card, size_t index);
    void remove_card(card_id_param_t id);
    void move_card(card_id_param_t id, size_t index);
    cards_iterator_t find_card(card_id_param_t id);
    const_cards_iterator_t find_card(card_id_param_t id) const;

    cards_iterator_t begin();
    cards_iterator_t end();

    const_cards_iterator_t begin() const;
    const_cards_iterator_t end() const;

    cards_reverse_iterator_t rbegin();
    cards_reverse_iterator_t rend();

    const_cards_reverse_iterator_t rbegin() const;
    const_cards_reverse_iterator_t rend() const;

   private:
    // cards contained in the collection in order.
    cards_t cards_;

    // the maximum number of elements that can be stored in the collection.
    size_t max_size_;

    //
    std::unordered_set<card_id_t> card_ids_;
};

}  // namespace mod
}  // namespace kekw