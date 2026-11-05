// Tests for task 5. DO NOT MODIFY this file.
// The "must not compile" test is in bledy/zadanie5_swap_values.cpp (ctest runs it).
#include <gtest/gtest.h>

#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "type_eq.hpp"
#include "zadanie5.hpp"

// Detection (C++17): does the call compile? The primary template says "no" (false_type).
// The specialization says "yes" (true_type), but it exists only when the expression
// in decltype is valid. An invalid expression removes the specialization (SFINAE, section 4.4).
// An ambiguous call is also invalid, so it also gives false.
template <typename T, typename = void>
struct can_call_average : std::false_type {};
template <typename T>
struct can_call_average<T, std::void_t<decltype(average(std::declval<const std::vector<T>&>()))>>
    : std::true_type {};

template <typename T, typename = void>
struct can_call_to_text : std::false_type {};
template <typename T>
struct can_call_to_text<T, std::void_t<decltype(to_text(std::declval<const T&>()))>> : std::true_type {};

// Calls to_text only when it is possible. Without this, one ambiguous call
// would stop the compilation of the whole test file.
template <typename T>
::testing::AssertionResult to_text_returns(const T& x, const std::string& expected) {
    if constexpr (can_call_to_text<T>::value) {
        std::string actual = to_text(x);
        if (actual == expected)
            return ::testing::AssertionSuccess();
        return ::testing::AssertionFailure() << "\n  actual:   " << ::testing::PrintToString(actual)
                                             << "\n  expected: " << ::testing::PrintToString(expected);
    } else {
        return ::testing::AssertionFailure()
               << "to_text(" << test_helpers::type_name<T>() << ") does not exist or is ambiguous";
    }
}
#define EXPECT_TO_TEXT_EQ(x, expected) EXPECT_TRUE(to_text_returns((x), (expected)))

TEST(Task5_Average, Result) {
    EXPECT_DOUBLE_EQ(average(std::vector<int>{1, 2, 3, 4}), 2.5);
    EXPECT_DOUBLE_EQ(average(std::vector<double>{1.5, 2.5}), 2.0);
    EXPECT_DOUBLE_EQ(average(std::vector<int>{-3, -3}), -3.0);
}

TEST(Task5_Average, EmptyVector) {
    EXPECT_DOUBLE_EQ(average(std::vector<int>{}), 0.0);
}

TEST(Task5_Average, OnlyForNumbers) {
    EXPECT_TRUE(can_call_average<int>::value);
    EXPECT_TRUE(can_call_average<double>::value);
    EXPECT_TRUE(can_call_average<char>::value);  // char is also an arithmetic type
    EXPECT_FALSE(can_call_average<std::string>::value);
    EXPECT_FALSE(can_call_average<int*>::value);
}

TEST(Task5_ToText, Bool) {
    EXPECT_TO_TEXT_EQ(true, "true");
    EXPECT_TO_TEXT_EQ(false, "false");
}

TEST(Task5_ToText, Integers) {
    EXPECT_TO_TEXT_EQ(42, "42");
    EXPECT_TO_TEXT_EQ(-7L, "-7");
    EXPECT_TO_TEXT_EQ(10u, "10");
}

TEST(Task5_ToText, FloatingPoint) {
    EXPECT_TO_TEXT_EQ(1.5, "1.5");
    EXPECT_TO_TEXT_EQ(0.25f, "0.25");
}

TEST(Task5_ToText, Text) {
    EXPECT_TO_TEXT_EQ(std::string("abc"), "\"abc\"");
    EXPECT_TO_TEXT_EQ("abc", "\"abc\"");  // a literal is an array const char[4], also convertible to std::string
    const char* pointer = "xyz";
    EXPECT_TO_TEXT_EQ(pointer, "\"xyz\"");
}

TEST(Task5_ToText, NotForOtherTypes) {
    EXPECT_TRUE(can_call_to_text<int>::value);
    EXPECT_FALSE(can_call_to_text<std::vector<int>>::value);
    EXPECT_FALSE(can_call_to_text<int*>::value);  // a pointer is not text
    EXPECT_FALSE((can_call_to_text<std::pair<int, int>>::value));
}

TEST(Task5_SwapValues, Works) {
    int a = 1, b = 2;
    swap_values(a, b);
    EXPECT_EQ(a, 2);
    EXPECT_EQ(b, 1);

    std::string x = "left", y = "right";
    swap_values(x, y);
    EXPECT_EQ(x, "right");
    EXPECT_EQ(y, "left");
}
