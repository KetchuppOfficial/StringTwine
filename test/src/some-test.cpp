/*
 * This is a file with the single purpose of testing build system. It will be removed in the future
 * or replaces by something actually meaningful.
 */

#include <gtest/gtest.h>

#include "cool/string_twine.hpp"

TEST(Twine, Something) {
    cool::Twine t{"hello"};
    EXPECT_TRUE(t.isUnary());
}
