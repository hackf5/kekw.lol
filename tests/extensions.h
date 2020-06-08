#pragma once

#include <extern/googletest/googletest/include/gtest/gtest.h>
#include <extern/googletest/googlemock/include/gmock/gmock.h>

#define EXPECT_THROW_MSG(stmt, etype, message)                                         \
    try {                                                                              \
        stmt;                                                                          \
        ADD_FAILURE() << "No throw detected";                                          \
    } catch (const etype& ex) {                                                        \
        auto msg = std::string(ex.what());                                             \
        EXPECT_THAT(msg, testing::HasSubstr(message))                                  \
            << "The correct exception was thrown, but with an incorrect message";      \
    } catch (const std::exception& ex) {                                               \
        ADD_FAILURE()                                                                  \
            << "Throw detected, but the exception type did not match the expectation " \
            << ex.what();                                                              \
    }
