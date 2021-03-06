#include "../extensions.h"

#include <array>
#include <src/mod/card.h>
#include <src/mod/card-collection.h>

using namespace kekw::mod;

class mock_card : public card {
   public:
    mock_card(card_id_param_t id = 1) : card(id, "dummy_id", "dummy") {}
    mock_card(card_id_param_t id, card_defn_id_param_t defn_id, std::string const& name)
        : card(id, defn_id, name) {}
    virtual ~mock_card(){};
    std::exception ex();
};

class mock_card_collection : public card_collection<mock_card> {
   public:
    mock_card_collection() = delete;
    mock_card_collection(index_t max_size) : card_collection(max_size) {}
    mock_card_collection(card_ptr_t* begin, card_ptr_t* end, index_t max_size)
        : card_collection(begin, end, max_size) {}
    virtual ~mock_card_collection() {}

    void add_card(card_ptr_t card) { card_collection::add_card(std::move(card)); }

    void insert_card(card_ptr_t card, index_t index) {
        card_collection::insert_card(std::move(card), index);
    }

    card_ptr_t remove_card(card_id_t id) { return card_collection::remove_card(id); }

    void move_card(card_id_t id, index_t index) { card_collection::move_card(id, index); }

    card_ptr_t replace_card(card_id_param_t id, card_ptr_t new_card) {
        return card_collection::replace_card(id, std::move(new_card));
    }

    void clear() { card_collection::clear(); }
};

typedef mock_card_collection::card_ptr_t card_ptr_t;

std::unique_ptr<mock_card_collection> create_full_target(index_t size) {
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

    auto const& ctarget = target;
    EXPECT_EQ((*ctarget.begin())->id(), 1);
    EXPECT_EQ((*(ctarget.begin() + 1))->id(), 2);
    EXPECT_EQ((*(ctarget.begin() + 2))->id(), 3);
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

    auto const& ctarget = target;
    EXPECT_EQ((*ctarget.begin())->id(), 3);
    EXPECT_EQ((*(ctarget.begin() + 1))->id(), 4);
    EXPECT_EQ((*(ctarget.begin() + 2))->id(), 2);
    EXPECT_EQ((*(ctarget.begin() + 3))->id(), 1);
}

TEST(card_collection, remove_card_reduces_size) {
    mock_card_collection target(5);

    target.add_card(card_ptr_t(new mock_card(1)));

    EXPECT_TRUE(target.contains_card(1));
    auto removed = target.remove_card(1);

    EXPECT_EQ(0, target.size());
    EXPECT_FALSE(target.contains_card(1));
    EXPECT_EQ(1, removed->id());

    auto const& ctarget = target;
    EXPECT_EQ(ctarget.begin(), ctarget.end());
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
    auto target = create_full_target(5);

    target->move_card(4, 1);
    EXPECT_THAT(target->get_card_ids(), testing::ElementsAre(1, 4, 2, 3, 5));
}

TEST(card_collection, move_card_move_backwards_moves_and_preserves_order_2) {
    auto target = create_full_target(5);

    target->move_card(5, 1);
    EXPECT_THAT(target->get_card_ids(), testing::ElementsAre(1, 5, 2, 3, 4));
}

TEST(card_collection, move_card_move_backwards_moves_and_preserves_order_3) {
    auto target = create_full_target(5);

    target->move_card(5, 0);
    EXPECT_THAT(target->get_card_ids(), testing::ElementsAre(5, 1, 2, 3, 4));
}

TEST(card_collection, move_card_move_forwards_moves_and_preserves_order_1) {
    auto target = create_full_target(5);

    target->move_card(1, 3);
    EXPECT_THAT(target->get_card_ids(), testing::ElementsAre(2, 3, 4, 1, 5));
}

TEST(card_collection, move_card_move_forwards_moves_and_preserves_order_2) {
    auto target = create_full_target(5);

    target->move_card(1, 4);
    EXPECT_THAT(target->get_card_ids(), testing::ElementsAre(2, 3, 4, 5, 1));
}

TEST(card_collection, move_card_move_forwards_moves_and_preserves_order_3) {
    auto target = create_full_target(5);

    target->move_card(2, 3);
    EXPECT_THAT(target->get_card_ids(), testing::ElementsAre(1, 3, 4, 2, 5));
}

TEST(card_collection, cannot_replace_nonexistent_card) {
    auto target = create_full_target(5);
    auto new_card = card_ptr_t(new mock_card(7));

    EXPECT_THROW_MSG(
        target->replace_card(10, std::move(new_card)),
        std::invalid_argument,
        "collection does not contain card (10)");
}

TEST(card_collection, cannot_replace_card_with_duplicate_card) {
    auto target = create_full_target(5);
    auto new_card = card_ptr_t(new mock_card(4));

    EXPECT_THROW_MSG(
        target->replace_card(3, std::move(new_card)),
        std::invalid_argument,
        "collection already contains card (4)");
}

TEST(card_collection, replace_card_with_new_card) {
    auto target = create_full_target(5);
    auto new_card = card_ptr_t(new mock_card(7));

    auto old_card = target->replace_card(3, std::move(new_card));

    EXPECT_TRUE(target->contains_card(7));
    EXPECT_FALSE(target->contains_card(3));
    EXPECT_THAT(target->get_card_ids(), testing::ElementsAre(1, 2, 7, 4, 5));
    EXPECT_EQ(old_card->id(), 3);
}

TEST(card_collection, clear_removes_all_cards) {
    auto target = create_full_target(5);

    target->clear();

    auto const& ctarget = *target;
    EXPECT_EQ(target->size(), 0);
    EXPECT_EQ(ctarget.begin(), ctarget.end());
    EXPECT_FALSE(target->contains_card(1));
}

TEST(card_collection, add_from_collection_adds_and_initializes) {
    std::array<card_ptr_t, 3> cards{card_ptr_t(new mock_card(1)),
                                    card_ptr_t(new mock_card(2)),
                                    card_ptr_t(new mock_card(3))};
    auto target = mock_card_collection(cards.begin(), cards.end(), 4);

    EXPECT_EQ(target.size(), 3);
    EXPECT_EQ(target.max_size(), 4);
    EXPECT_THAT(target.get_card_ids(), testing::ElementsAre(1, 2, 3));
    EXPECT_TRUE(target.contains_card(1));
    EXPECT_TRUE(target.contains_card(2));
    EXPECT_TRUE(target.contains_card(3));

    target.add_card(card_ptr_t(new mock_card(4)));
    EXPECT_EQ(target.size(), 4);
    EXPECT_TRUE(target.contains_card(4));

    EXPECT_THROW_MSG(
        target.add_card(card_ptr_t(new mock_card(5))), std::out_of_range, "");
}

TEST(card_collection, cannot_add_more_cards_than_max_size_from_ctor) {
    std::array<card_ptr_t, 3> cards{card_ptr_t(new mock_card(1)),
                                    card_ptr_t(new mock_card(2)),
                                    card_ptr_t(new mock_card(3))};

    EXPECT_THROW_MSG(
        mock_card_collection(cards.begin(), cards.end(), 2),
        std::invalid_argument,
        "too many cards given; size=3, max_size=2");
}

TEST(card_collection, cannot_add_more_duplicate_card_from_ctor) {
    std::array<card_ptr_t, 3> cards{card_ptr_t(new mock_card(1)),
                                    card_ptr_t(new mock_card(2)),
                                    card_ptr_t(new mock_card(1))};

    EXPECT_THROW_MSG(
        mock_card_collection(cards.begin(), cards.end(), 5),
        std::invalid_argument,
        "duplicate card found (1)");
}

TEST(card_collection, is_empty_returns_true_when_contains_no_cards) {
    auto target = mock_card_collection(5);

    EXPECT_TRUE(target.is_empty());
}

TEST(card_collection, is_empty_returns_false_when_contains_a_card) {
    auto target = mock_card_collection(5);

    target.add_card(card_ptr_t(new mock_card(1)));
    EXPECT_FALSE(target.is_empty());
}

TEST(card_collection, is_full_returns_false_until_it_contains_max_cards) {
    auto target = mock_card_collection(3);

    EXPECT_FALSE(target.is_full());

    target.add_card(card_ptr_t(new mock_card(1)));
    EXPECT_FALSE(target.is_full());

    target.add_card(card_ptr_t(new mock_card(2)));
    EXPECT_FALSE(target.is_full());

    target.add_card(card_ptr_t(new mock_card(3)));
    EXPECT_TRUE(target.is_full());
}

TEST(card_collection, at_throws_if_index_eq_size) {
    auto target = mock_card_collection(3);

    target.add_card(card_ptr_t(new mock_card(1)));

    EXPECT_THROW_MSG(
        target.at(1),
        std::invalid_argument,
        "index (1) must be strictly less than size (1)");
}

TEST(card_collection, at_throws_if_index_gt_size) {
    auto target = mock_card_collection(3);

    target.add_card(card_ptr_t(new mock_card(1)));

    EXPECT_THROW_MSG(
        target.at(2),
        std::invalid_argument,
        "index (2) must be strictly less than size (1)");
}

TEST(card_collection, at_gets_element_at_index) {
    auto target = create_full_target(5);

    EXPECT_EQ(target->at(0)->id(), 1);
    EXPECT_EQ(target->at(0)->id(), 1);
    EXPECT_EQ(target->at(1)->id(), 2);
    EXPECT_EQ(target->at(1)->id(), 2);
    EXPECT_EQ(target->at(4)->id(), 5);
}