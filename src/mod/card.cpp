#include "card.h"

kekw::mod::card::card(card_id_param_t id, card_type_id_param_t type_id, std::string const& name)
    : id_(id), type_id_(type_id), name_(name) {}

kekw::mod::card::~card() {}

kekw::mod::card_id_ret_t kekw::mod::card::id() const { return this->id_; }

kekw::mod::card_type_id_ret_t kekw::mod::card::type_id() const { return this->type_id_; }

const std::string& kekw::mod::card::name() const { return this->name_; }