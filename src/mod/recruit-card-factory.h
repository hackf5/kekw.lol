#pragma once

#include "available-card.h"
#include "owned-card.h"
#include "recruit-card.h"

#include <memory>

namespace kekw {
namespace mod {

class recruit_card_factory {
   public:
    virtual ~recruit_card_factory(){};

    std::unique_ptr<available_card> create_available();
    std::unique_ptr<owned_card> create_owned(std::unique_ptr<available_card> const& card);
    std::unique_ptr<recruit_card> create_recruit(std::unique_ptr<owned_card> const& card);
};

}  // namespace mod
}  // namespace kekw