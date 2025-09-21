#include <string>
#include <string_view>
#include <utility>

#include <gtest/gtest.h>

#include "cool/twine.hpp"

using namespace std::string_view_literals;

TEST(TwineConstructors, DefaultConstructedObjectIsEmpty) {
    // Assign & Act
    const cool::Twine twine;

    // Assert
    EXPECT_TRUE(twine.empty());
}

TEST(TwineConstructors, DefaultConstructedObjectConvertsToAnEmptyString) {
    // Assign
    const cool::Twine twine;

    // Act
    const auto concatenation = twine.str();

    // Assert
    EXPECT_TRUE(concatenation.empty());
}

TEST(TwineConstructors, ObjectConstructedFromEmptyStringViewIsEmpty) {
    // Assign
    constexpr std::string_view sv;

    // Act
    const cool::Twine twine{sv};

    // Assert
    EXPECT_TRUE(twine.empty());
}

TEST(TwineConstructors, ObjectConstructedFromNonEmptyStringViewIsNotEmpty) {
    // Assign
    constexpr std::string_view sv{"lorem ipsum"};

    // Act
    const cool::Twine twine{sv};

    // Assert
    EXPECT_FALSE(twine.empty());
}

TEST(TwineConstructors, ObjectConstructedFromEmptyStringViewConvertsToAnEmptyString) {
    // Assign
    const cool::Twine twine{""sv};

    // Act
    const auto concatenation = twine.str();

    // Assert
    EXPECT_TRUE(concatenation.empty());
}

TEST(TwineConstructors,
     ObjectConstructedFromNonEmptyStringViewConvertsToAStringWithTheSameContent) {
    // Assign
    constexpr std::string_view sv{"lorem ipsum"};
    const cool::Twine twine{sv};

    // Act
    const auto concatenation = twine.str();

    // Assert
    EXPECT_EQ(concatenation, sv);
}

TEST(TwineAddition, EmptyTwinePlusEmptyStringViewResultsInEmptyTwine) {
    // Assign
    cool::Twine lhs;
    constexpr std::string_view rhs;

    // Act
    const auto result = std::move(lhs) + rhs;

    // Assert
    EXPECT_TRUE(result.empty());
}

TEST(TwineAddition, EmptyTwinePlusEmptyStringViewConvertsToEmptyString) {
    // Assign & Act
    const auto concatenation = (cool::Twine{} + ""sv).str();

    // Assert
    EXPECT_TRUE(concatenation.empty());
}

TEST(TwineAddition, EmptyStringViewPlusEmptyTwineResultsInEmptyTwine) {
    // Assign
    constexpr std::string_view lhs;
    cool::Twine rhs;

    // Act
    const auto result = lhs + std::move(rhs);

    // Assert
    EXPECT_TRUE(result.empty());
}

TEST(TwineAddition, EmptyStringViewPlusEmptyTwineConvertsToEmptyString) {
    // Assign & Act
    const auto concatenation = (""sv + cool::Twine{}).str();

    // Assert
    EXPECT_TRUE(concatenation.empty());
}

TEST(TwineAddition, EmptyTwinePlusStringViewResultsInNonEmptyTwine) {
    // Assign
    cool::Twine lhs;
    constexpr std::string_view rhs{"lorem ipsum"};

    // Act
    const auto result = std::move(lhs) + rhs;

    // Assert
    EXPECT_FALSE(result.empty());
}

TEST(TwineAddition, EmptyTwinePlusStringViewConvertsToStringWithTheSameContentAsTheView) {
    // Assign
    constexpr std::string_view rhs{"lorem ipsum"};

    // Act
    const auto concatenation = (cool::Twine{} + rhs).str();

    // Assert
    EXPECT_EQ(concatenation, rhs);
}

TEST(TwineAddition, StringViewPlusEmptyTwineResultsInNonEmptyTwine) {
    // Assign
    constexpr std::string_view lhs{"lorem ipsum"};
    cool::Twine rhs;

    // Act
    const auto result = lhs + std::move(rhs);

    // Assert
    EXPECT_FALSE(result.empty());
}

TEST(TwineAddition, PointerToCharPlusEmptyTwineConvertsToStringWithTheSameContentAsTheView) {
    // Assign
    constexpr std::string_view lhs{"lorem ipsum"};

    // Act
    const auto concatenation = (lhs + cool::Twine{}).str();

    // Assert
    EXPECT_EQ(concatenation, lhs);
}

TEST(TwineAddition, TwinePlusEmptyStringViewResultsInNonEmptyTwine) {
    // Assign
    cool::Twine lhs{"lorem ipsum"};

    // Act
    const auto result = std::move(lhs) + ""sv;

    // Assert
    EXPECT_FALSE(result.empty());
}

TEST(TwineAddition, TwinePlusEmptyStringViewConvertsToStringWithTheSameContentAsTheTwine) {
    // Assign
    constexpr std::string_view content = "lorem ipsum";

    // Act
    const auto concatenation = (cool::Twine{content} + ""sv).str();

    // Assert
    EXPECT_EQ(concatenation, content);
}

TEST(TwineAddition, EmptyStringViewPlusTwineResultsInNonEmptyTwine) {
    // Assign
    cool::Twine rhs{"lorem ipsum"};

    // Act
    const auto result = ""sv + std::move(rhs);

    // Assert
    EXPECT_FALSE(result.empty());
}

TEST(TwineAddition, EmptyStringViewPlusTwineConvertsToStringWithTheSameContentAsTheView) {
    // Assign
    constexpr std::string_view content = "lorem ipsum";

    // Act
    const auto concatenation = (""sv + cool::Twine{content}).str();

    // Assert
    EXPECT_EQ(concatenation, content);
}

TEST(TwineAddition, TwinePlusStringViewResultsInNonEmptyTwine) {
    // Assign
    cool::Twine lhs{"lorem "};
    constexpr std::string_view rhs{"ipsum"};

    // Act
    const auto result = std::move(lhs) + rhs;

    // Assert
    EXPECT_FALSE(result.empty());
}

TEST(TwineAddition, TwinePlusStringViewConvertsToStringWithKnownContent) {
    // Assign & Act
    const auto concatenation = (cool::Twine{"lorem "} + "ipsum"sv).str();

    // Assert
    EXPECT_EQ(concatenation, "lorem ipsum");
}

TEST(TwineAddition, StringViewPlusTwineResultsInNonEmptyTwine) {
    // Assign
    constexpr std::string_view lhs{"lorem "};
    cool::Twine rhs{"ipsum"};

    // Act
    const auto result = lhs + std::move(rhs);

    // Assert
    EXPECT_FALSE(result.empty());
}

TEST(TwineAddition, StringViewPlusTwineConvertsToStringWithKnownContent) {
    // Assign & Act
    const auto concatenation = ("lorem " + cool::Twine{"ipsum"}).str();

    // Assert
    EXPECT_EQ(concatenation, "lorem ipsum");
}

TEST(TwineAddition, EmptyTwinePlusEmptyTwineResultsInEmptyTwine) {
    // Assign
    cool::Twine lhs;
    cool::Twine rhs;

    // Act
    const auto result = std::move(lhs) + std::move(rhs);

    // Assert
    EXPECT_TRUE(result.empty());
}

TEST(TwineAddition, TwinePlusEmptyTwineResultsInNonEmptyTwine) {
    // Assign
    cool::Twine lhs{"lorem ipsum"};
    cool::Twine rhs;

    // Act
    const auto result = std::move(lhs) + std::move(rhs);

    // Assert
    EXPECT_FALSE(result.empty());
}

TEST(TwineAddition, TwinePlusEmptyTwineConvertsToStringWithTheSameContentAsTheFirstTwine) {
    // Assign
    constexpr std::string_view content{"lorem ipsum"};

    // Act
    const auto concatenation = (cool::Twine{content} + cool::Twine{}).str();

    // Assert
    EXPECT_EQ(concatenation, content);
}

TEST(TwineAddition, EmptyTwinePlusTwineResultsInNonEmptyTwine) {
    // Assign
    cool::Twine lhs;
    cool::Twine rhs{"lorem ipsum"};

    // Act
    const auto result = std::move(lhs) + std::move(rhs);

    // Assert
    EXPECT_FALSE(result.empty());
}

TEST(TwineAddition, EmptyTwinePlusTwineConvertsToStringWithTheSameContentAsTheSecondTwine) {
    // Assign
    constexpr std::string_view content{"lorem ipsum"};

    // Act
    const auto concatenation = (cool::Twine{} + cool::Twine{content}).str();

    // Assert
    EXPECT_EQ(concatenation, content);
}

TEST(TwineAddition, NonEmptyTwinePlusNonEmptyTwineResultsInNonEmptyTwine) {
    // Assign
    cool::Twine lhs{"lorem "};
    cool::Twine rhs{"ipsum"};

    // Act
    const auto result = std::move(lhs) + std::move(rhs);

    // Assert
    EXPECT_FALSE(result.empty());
}

TEST(TwineAddition, LongConcatenation) {
    // Assign
    constexpr std::string_view result =
        "lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt "
        "ut labore et dolore magna aliqua.";

    // Act
    const auto concatenation =
        (cool::Twine{"lorem ipsum "} + "dolor sit amet, " + "consectetur adipiscing " +
         "elit, sed do " + "eiusmod tempor incididunt " + "ut labore et dolore " + "magna aliqua.")
            .str();

    // Assert
    EXPECT_EQ(concatenation, result);
}
