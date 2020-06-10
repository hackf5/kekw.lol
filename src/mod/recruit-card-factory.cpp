#include "recruit-card-factory.h"

using namespace kekw::mod;

std::unique_ptr<available_card> recruit_card_factory::create_available() {
    static card_id_t id = 0;
    return std::unique_ptr<available_card>(new available_card(++id, "dummy", "tbd"));
}

std::unique_ptr<owned_card> recruit_card_factory::create_owned(
    std::unique_ptr<available_card> const& card) {
    return std::unique_ptr<owned_card>(
        new owned_card(card->id(), card->defn_id(), card->name()));
}

std::unique_ptr<recruit_card> recruit_card_factory::create_recruit(
    std::unique_ptr<owned_card> const& card) {
    return std::unique_ptr<recruit_card>(
        new recruit_card(card->id(), card->defn_id(), card->name()));
}