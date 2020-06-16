#pragma once

#include "transform.h"

#include <memory>

namespace kekw {
namespace world {

class spatial : public transform {
   public:
    spatial();
    spatial(spatial* parent);

    spatial* root();
    spatial* parent();

   private:
    spatial* parent_;
};

}  // namespace world
}  // namespace kekw