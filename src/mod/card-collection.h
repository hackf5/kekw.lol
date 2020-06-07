#pragma once

#include "mod_types.h"

#include <memory>
#include <string>
#include <vector>

namespace kekw {
namespace mod {

class card;

class card_collection {
   private:
    typedef std::unique_ptr<card> card_ptr_t;

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

   public:
    virtual ~card_collection() = 0;
    size_t size() const;
    size_t max_size() const;

   protected:
    card_collection(size_t max_size);

    void add_card(std::unique_ptr<card> card);
    void insert_card(std::unique_ptr<card> card, size_t index);
    void remove_card(card_id_param_t id);
    void move_card(card_id_param_t id, size_t index);
    cards_iterator_t find_card(card_id_param_t id);
    const_cards_iterator_t find_card(card_id_param_t id) const;

   private:
    cards_t cards_;
    size_t max_size_;
};

}  // namespace mod
}  // namespace kekw