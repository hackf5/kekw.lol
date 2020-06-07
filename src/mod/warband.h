#pragma once

#include <memory>
#include <vector>

namespace kekw {
namespace mod {

class card;

class warband {
   public:
    warband();
    virtual ~warband() = 0;
   private:
    std::vector<std::unique_ptr<card>> cards_;
};

}  // namespace mod
}  // namespace kekw