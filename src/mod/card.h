#pragma once

#include <string>

namespace kekw {
namespace mod {

class card {
   public:
    card(std::string const& id, std::string const& name);
    virtual ~card() = 0;

   private:
    const std::string id_;
    const std::string name_;
};

}  // namespace mod
}  // namespace kekw