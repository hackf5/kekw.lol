#pragma once

#include <memory>
#include <string>
#include <vector>

namespace kekw {
namespace mod {

class card;

class card_collection {
   public:
    virtual ~card_collection() = 0;
    int size() const;
    int max_size() const;

   protected:
    card_collection(int max_size);

    void add_card(std::unique_ptr<card> card);
    void insert_card(int index, std::unique_ptr<card> card);
    void remove_card(std::string id);
    

   private:
    std::vector<std::unique_ptr<card>> cards_;
    int max_size_;
};

}  // namespace mod
}  // namespace kekw