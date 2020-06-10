#include "hero.h"

using namespace kekw::mod;

hero::hero(hero_id_param_t id, hero_defn_id_param_t defn_id, std::string const& name)
    : id_(id), defn_id_(defn_id), name_(name) {}

hero::~hero() {}

hero_id_ret_t hero::id() const { return this->id_; }

hero_defn_id_ret_t hero::defn_id() const { return this->defn_id_; }

const std::string& hero::name() const { return this->name_; }