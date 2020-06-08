#include "../extensions.h"

#include <src/mod/card.h>
#include <src/mod/card-collection.h>

using namespace kekw::mod;
typedef card_collection::card_ptr_t card_ptr_t;

class mock_card : public card {
   public:
    mock_card(card_id_param_t id = 1) : card(id, "dummy_id", "dummy") {}
    mock_card(card_id_param_t id, card_defn_id_param_t defn_id, std::string const& name)
        : card(id, defn_id, name) {}
    virtual ~mock_card(){};
    std::exception ex();
};

class mock_card_collection : public card_collection {
   public:
    mock_card_collection() = delete;
    mock_card_collection(size_t max_size) : card_collection(max_size) {}
    virtual ~mock_card_collection() {}

    void add_card(card_ptr_t card) { card_collection::add_card(std::move(card)); }
    void insert_card(card_ptr_t card, size_t index) {
        card_collection::insert_card(std::move(card), index);
    }
    void remove_card(card_id_t id) { card_collection::remove_card(id); }
    void move_card(card_id_t id, size_t index) { card_collection::move_card(id, index); }

    cards_iterator_t begin() { return card_collection::begin(); }
    cards_iterator_t end() { return card_collection::end(); }
};

std::unique_ptr<mock_card_collection> create_move_target(size_t size) {
    auto target = std::unique_ptr<mock_card_collection>(new mock_card_collection(size));

    for (int i = 0; i != size; ++i) {
        target->add_card(card_ptr_t(new mock_card(i + 1)));
    }

    return target;
}

TEST(card, id_returns_id) {
    mock_card target(3, "floop", "barry");
    EXPECT_EQ(3, target.id());
}

TEST(card, defn_id_returns_defn_id) {
    mock_card target(3, "floop", "barry");
    EXPECT_EQ("floop", target.defn_id());
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

TEST(card_collection, cannot_add_more_than_max_size) {
    mock_card_collection target(2);

    target.add_card(card_ptr_t(new mock_card(1)));
    EXPECT_EQ(1, target.size());

    target.add_card(card_ptr_t(new mock_card(2)));
    EXPECT_EQ(2, target.size());

    EXPECT_THROW_MSG(
        target.add_card(card_ptr_t(new mock_card(3))),
        std::out_of_range,
        "collection is at max size");
}

TEST(card_collection, cannot_add_card_with_duplicate_id) {
    mock_card_collection target(5);

    target.add_card(card_ptr_t(new mock_card(1)));
    EXPECT_THROW_MSG(
        target.add_card(card_ptr_t(new mock_card(1))),
        std::invalid_argument,
        "collection already contains card");
}

TEST(card_collection, add_card_adds_card) {
    mock_card_collection target(5);

    target.add_card(card_ptr_t(new mock_card(1)));
    target.add_card(card_ptr_t(new mock_card(2)));
    target.add_card(card_ptr_t(new mock_card(3)));

    EXPECT_EQ((*target.begin())->id(), 1);
    EXPECT_EQ((*(target.begin() + 1))->id(), 2);
    EXPECT_EQ((*(target.begin() + 2))->id(), 3);
}

TEST(card_collection, insert_card_increases_size) {
    mock_card_collection target(5);

    target.insert_card(card_ptr_t(new mock_card(1)), 0);
    EXPECT_EQ(1, target.size());

    target.insert_card(card_ptr_t(new mock_card(2)), 1);
    EXPECT_EQ(2, target.size());
}

TEST(card_collection, cannot_insert_more_than_max_size) {
    mock_card_collection target(2);

    target.insert_card(card_ptr_t(new mock_card(1)), 0);
    EXPECT_EQ(1, target.size());

    target.insert_card(card_ptr_t(new mock_card(2)), 1);
    EXPECT_EQ(2, target.size());

    EXPECT_THROW_MSG(target.insert_card(card_ptr_t(new mock_card(3)), 2);
                     , std::out_of_range, "collection is at max size");
}

TEST(card_collection, cannot_insert_card_with_duplicate_id) {
    mock_card_collection target(5);

    target.insert_card(card_ptr_t(new mock_card(1)), 0);
    EXPECT_THROW_MSG(
        target.insert_card(card_ptr_t(new mock_card(1)), 1),
        std::invalid_argument,
        "collection already contains card");
}

TEST(card_collection, cannot_insert_card_when_index_gt_size) {
    mock_card_collection target(5);

    EXPECT_THROW_MSG(
        target.insert_card(card_ptr_t(new mock_card(1)), 1),
        std::invalid_argument,
        "index (1) is larger than size (0)");
}

TEST(card_collection, insert_card_adds_card) {
    mock_card_collection target(5);

    target.insert_card(card_ptr_t(new mock_card(1)), 0);
    target.insert_card(card_ptr_t(new mock_card(2)), 0);
    target.insert_card(card_ptr_t(new mock_card(3)), 0);
    target.insert_card(card_ptr_t(new mock_card(4)), 1);

    EXPECT_EQ((*target.begin())->id(), 3);
    EXPECT_EQ((*(target.begin() + 1))->id(), 4);
    EXPECT_EQ((*(target.begin() + 2))->id(), 2);
    EXPECT_EQ((*(target.begin() + 3))->id(), 1);
}

TEST(card_collection, remove_card_reduces_size) {
    mock_card_collection target(5);

    target.add_card(card_ptr_t(new mock_card(1)));

    EXPECT_TRUE(target.contains_card(1));
    target.remove_card(1);
    EXPECT_EQ(0, target.size());
    EXPECT_FALSE(target.contains_card(1));
    EXPECT_EQ(target.begin(), target.end());
}

TEST(card_collection, cannot_remove_card_that_does_not_exist) {
    mock_card_collection target(5);
    target.add_card(card_ptr_t(new mock_card(1)));

    EXPECT_THROW_MSG(
        target.remove_card(2), std::invalid_argument, "collection does not contain card");
}

TEST(card_collection, cannot_move_card_when_index_greater_than_size) {
    mock_card_collection target(5);
    target.add_card(card_ptr_t(new mock_card(1)));
    target.add_card(card_ptr_t(new mock_card(2)));

    EXPECT_THROW_MSG(
        target.move_card(1, 2),
        std::invalid_argument,
        "index (2) must be strictly less than size (2)");
}

TEST(card_collection, cannot_move_card_that_is_not_in_collection) {
    mock_card_collection target(5);
    target.add_card(card_ptr_t(new mock_card(1)));
    target.add_card(card_ptr_t(new mock_card(2)));

    EXPECT_THROW_MSG(
        target.move_card(3, 0),
        std::invalid_argument,
        "collection does not contain card (3)");
}

TEST(card_collection, move_card_move_backwards_moves_and_preserves_order_1) {
    auto target = create_move_target(5);

    target->move_card(4, 1);
    EXPECT_THAT(target->get_card_ids(), testing::ElementsAre(1, 4, 2, 3, 5));
}

TEST(card_collection, move_card_move_backwards_moves_and_preserves_order_2) {
    auto target = create_move_target(5);

    target->move_card(5, 1);
    EXPECT_THAT(target->get_card_ids(), testing::ElementsAre(1, 5, 2, 3, 4));
}

TEST(card_collection, move_card_move_backwards_moves_and_preserves_order_3) {
    auto target = create_move_target(5);

    target->move_card(5, 0);
    EXPECT_THAT(target->get_card_ids(), testing::ElementsAre(5, 1, 2, 3, 4));
}

TEST(card_collection, move_card_move_forwards_moves_and_preserves_order_1) {
    auto target = create_move_target(5);

    target->move_card(1, 3);
    EXPECT_THAT(target->get_card_ids(), testing::ElementsAre(2, 3, 4, 1, 5));
}

TEST(card_collection, move_card_move_forwards_moves_and_preserves_order_2) {
    auto target = create_move_target(5);

    target->move_card(1, 4);
    EXPECT_THAT(target->get_card_ids(), testing::ElementsAre(2, 3, 4, 5, 1));
}

TEST(card_collection, move_card_move_forwards_moves_and_preserves_order_3) {
    auto target = create_move_target(5);

    target->move_card(2, 3);
    EXPECT_THAT(target->get_card_ids(), testing::ElementsAre(1, 3, 4, 2, 5));
}