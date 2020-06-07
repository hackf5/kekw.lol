#include <extern/googletest/googletest/include/gtest/gtest.h>
#include <src/mod/card.h>
#include <src/mod/card-collection.h>

using namespace kekw::mod;
typedef card_collection::card_ptr_t card_ptr_t;

class mock_card : public card {
   public:
    mock_card(card_id_param_t id = 1) : card(id, "dummy_id", "dummy") {}
    mock_card(card_id_param_t id, card_type_id_param_t type_id, std::string const& name) : card(id, type_id, name) {}
    virtual ~mock_card(){};
};

class mock_card_collection : public card_collection {
   public:
    mock_card_collection() = delete;
    mock_card_collection(size_t max_size) : card_collection(max_size) {}
    virtual ~mock_card_collection() {}

    void add_card(card_ptr_t card) { card_collection::add_card(std::move(card)); }
};

TEST(card, id_returns_id) {
    mock_card target(3, "floop", "barry");
    EXPECT_EQ(3, target.id());
}

TEST(card, type_id_returns_type_id) {
    mock_card target(3, "floop", "barry");
    EXPECT_EQ("floop", target.type_id());
}

TEST(card, name_returns_name) {
    mock_card target(3, "floop", "barry");
    EXPECT_EQ("barry", target.name());
}

TEST(card_collection, initial_size_is_0) {
    mock_card_collection target(5);
    EXPECT_EQ(0, target.size());
}

TEST(card_collection, max_size_return_max_size) {
    mock_card_collection target(5);
    EXPECT_EQ(5, target.max_size());
}

TEST(card_collection, add_card_increases_size) {
    mock_card_collection target(5);
    target.add_card(card_ptr_t(new mock_card()));
    EXPECT_EQ(1, target.size());
}

TEST(card_collection_death, cannot_add_more_than_max_size) {
    mock_card_collection target(2);

    target.add_card(card_ptr_t(new mock_card(1)));
    EXPECT_EQ(1, target.size());

    target.add_card(card_ptr_t(new mock_card(2)));
    EXPECT_EQ(2, target.size());

    EXPECT_THROW(target.add_card(card_ptr_t(new mock_card(3))), std::out_of_range);
}

TEST(card_collection_death, cannot_add_card_with_same_id) {
    mock_card_collection target(5);

    target.add_card(card_ptr_t(new mock_card(1)));
    EXPECT_THROW(target.add_card(card_ptr_t(new mock_card(1))), std::runtime_error);
}