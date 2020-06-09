#pragma once

#include "available-card-collection.h"
#include "owned-card-collection.h"
#include "recruit-card-collection.h"

#include <memory>

namespace kekw {
namespace mod {

class recruit_env {
   public:
    recruit_env();
    virtual ~recruit_env() {}

   
   private:
    std::unique_ptr<available_card_collection> available_cards_;
    std::unique_ptr<owned_card_collection> owned_cards_;
    std::unique_ptr<recruit_card_collection> recruit_cards_;
};

}  // namespace mod
}  // namespace kekw
