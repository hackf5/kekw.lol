#include "recruit-env.h"

using namespace kekw::mod;

recruit_env::recruit_env(
    std::unique_ptr<recruit_hero> hero,
    std::unique_ptr<available_card_collection> available_cards,
    std::unique_ptr<owned_card_collection> owned_cards,
    std::unique_ptr<recruit_card_collection> recruit_cards,
    std::unique_ptr<recruit_card_factory> card_factory)
    : available_cards_(std::move(available_cards)),
      owned_cards_(std::move(owned_cards)),
      recruit_cards_(std::move(recruit_cards)),
      card_factory_(std::move(card_factory)) {}

void recruit_env::refresh() {
    this->available_cards_->clear();
    for (int i = 0; i != 5; ++i) {
        this->available_cards_->add_card(this->card_factory_->create_available());
    }
}

void recruit_env::own(card_id_param_t card_id) {
    auto card = this->available_cards_->remove_card(card_id);
    this->owned_cards_->add_card(this->card_factory_->create_owned(card));
}

void recruit_env::recruit(card_id_param_t card_id, index_t index) {
    auto card = this->owned_cards_->remove_card(card_id);
    this->recruit_cards_->insert_card(this->card_factory_->create_recruit(card), index);
}

void recruit_env::dismiss(card_id_param_t card_id) {
    auto card = this->recruit_cards_->remove_card(card_id);
}