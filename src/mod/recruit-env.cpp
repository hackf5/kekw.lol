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

owned_card const* recruit_env::own(card_id_param_t card_id) {
    auto card = this->available_cards_->remove_card(card_id);
    this->owned_cards_->add_card(this->card_factory_->create_owned(card));

    return this->owned_view().at(this->owned_view().size() - 1);
}

recruit_card const* recruit_env::recruit(card_id_param_t card_id, index_t index) {
    this->recruit_cards_->insert_card(
        this->card_factory_->create_recruit(this->owned_cards_->remove_card(card_id)),
        index);

    return this->recruit_view().at(index);
}

std::unique_ptr<recruit_card> recruit_env::dismiss(card_id_param_t card_id) {
    return this->recruit_cards_->remove_card(card_id);
}

recruit_env::available_view_t recruit_env::available_view() const {
    return *this->available_cards_.get();
}

recruit_env::owned_view_t recruit_env::owned_view() const {
    return *this->owned_cards_.get();
}

recruit_env::recruit_view_t recruit_env::recruit_view() const {
    return *this->recruit_cards_.get();
}