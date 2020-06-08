#include "card.h"

kekw::mod::card::card(
    card_id_param_t id, card_defn_id_param_t defn_id, std::string const& name)
    : id_(id), defn_id_(defn_id), name_(name) {}

kekw::mod::card::~card() {}

kekw::mod::card_id_ret_t kekw::mod::card::id() const { return this->id_; }

kekw::mod::card_defn_id_ret_t kekw::mod::card::defn_id() const { return this->defn_id_; }

const std::string& kekw::mod::card::name() const { return this->name_; }