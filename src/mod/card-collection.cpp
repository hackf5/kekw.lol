#include "card-collection.h"
#include "card.h"

kekw::mod::card_collection::card_collection(int max_size) : cards_(), max_size_(max_size) {}

kekw::mod::card_collection::~card_collection() {}

int kekw::mod::card_collection::size() const { return this->cards_.size(); }

int kekw::mod::card_collection::max_size() const { return this->max_size_; }

void kekw::mod::card_collection::add_card(std::unique_ptr<card> card) {}

void kekw::mod::card_collection::insert_card(int index, std::unique_ptr<card> card) {}

void kekw::mod::card_collection::remove_card(std::string id) {}