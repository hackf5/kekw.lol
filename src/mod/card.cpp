#include "card.h"

using namespace kekw::mod;

card::card(card_id_param_t id, card_defn_id_param_t defn_id, std::string const& name)
    : id_(id), defn_id_(defn_id), name_(name) {}

card::~card() {}

card_id_ret_t card::id() const { return this->id_; }

card_defn_id_ret_t card::defn_id() const { return this->defn_id_; }

const std::string& card::name() const { return this->name_; }