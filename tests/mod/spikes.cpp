#include <src/mod/recruit-env.h>
#include <tests/extensions.h>

extern "C" {
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>
}

using namespace kekw::mod;

template <class T>
using u_ptr = std::unique_ptr<T>;

std::shared_ptr<recruit_env> create_target(index_t max_size) {
    return std::shared_ptr<recruit_env>(new recruit_env(
        u_ptr<recruit_hero>(new recruit_hero(1, "dummy-hero", "cleefster")),
        u_ptr<available_card_collection>(new available_card_collection(max_size)),
        u_ptr<owned_card_collection>(new owned_card_collection(max_size)),
        u_ptr<recruit_card_collection>(new recruit_card_collection(max_size)),
        u_ptr<recruit_card_factory>(new recruit_card_factory())));
}

TEST(spikes, spike1) {
    index_t max_size = 5;
    auto target = create_target(max_size);

    EXPECT_EQ(target->available_view()->size(), 0);
    EXPECT_EQ(target->available_view()->max_size(), max_size);

    target->refresh();
    EXPECT_EQ(target->available_view()->size(), max_size);

    auto id = target->available_view()->at(2)->id();

    auto owned = target->own(id);
    EXPECT_TRUE(target->owned_view()->contains_card(id));
    EXPECT_FALSE(target->available_view()->contains_card(id));
    EXPECT_EQ(owned->id(), id);

    auto recruited = target->recruit(id, 0);
    EXPECT_TRUE(target->recruit_view()->contains_card(id));
    EXPECT_FALSE(target->owned_view()->contains_card(id));
    EXPECT_EQ(recruited->id(), id);

    auto dismissed = target->dismiss(id);
    EXPECT_FALSE(target->recruit_view()->contains_card(id));
    EXPECT_EQ(dismissed->id(), id);

    for (int i = 1; i <= 10; ++i) {
        create_target(1);
    }
}

TEST(spikes, spike2) {
    auto *L = luaL_newstate();
    luaL_openlibs(L);

    // Here we load the string and use lua_pcall for run the code
    if (luaL_loadfile(L, "lua/core/test.lua") == LUA_OK) {
        if (lua_pcall(L, 0, 1, 0) == LUA_OK) {
            // If it was executed successfuly we
            // remove the code from the stack
            lua_pop(L, lua_gettop(L));
        }
    }

    lua_close(L);
}