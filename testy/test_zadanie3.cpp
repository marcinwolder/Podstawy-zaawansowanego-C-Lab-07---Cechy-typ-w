// Tests for task 3. DO NOT MODIFY this file.
// The "must not compile" test is in bledy/zadanie3_unsupported.cpp (ctest runs it).
#include <gtest/gtest.h>

#include <string>
#include <type_traits>

#include "zadanie3.hpp"

TEST(Task3_DescribeType, BasicTypes) {
    EXPECT_EQ(describe_type<void>(), "void");
    EXPECT_EQ(describe_type<int>(), "integer");
    EXPECT_EQ(describe_type<unsigned long>(), "integer");
    EXPECT_EQ(describe_type<double>(), "floating point");
    EXPECT_EQ(describe_type<float>(), "floating point");
    EXPECT_EQ(describe_type<std::string>(), "string");
}

TEST(Task3_DescribeType, BoolAndCharBeforeIntegers) {
    // std::is_integral_v<bool> and std::is_integral_v<char> are true - the order of branches matters
    EXPECT_EQ(describe_type<bool>(), "bool");
    EXPECT_EQ(describe_type<char>(), "char");
    EXPECT_EQ(describe_type<signed char>(), "integer");  // signed char is a different type than char
}

TEST(Task3_DescribeType, Const) {
    EXPECT_EQ(describe_type<const int>(), "const integer");
    EXPECT_EQ(describe_type<const std::string>(), "const string");
}

TEST(Task3_DescribeType, Pointers) {
    EXPECT_EQ(describe_type<int*>(), "pointer to integer");
    EXPECT_EQ(describe_type<void*>(), "pointer to void");
    EXPECT_EQ(describe_type<int**>(), "pointer to pointer to integer");
}

TEST(Task3_DescribeType, ConstWithPointer) {
    EXPECT_EQ(describe_type<const char*>(), "pointer to const char");       // you cannot change the char
    EXPECT_EQ(describe_type<int* const>(), "const pointer to integer");  // you cannot move the pointer
}

TEST(Task3_DescribeType, References) {
    EXPECT_EQ(describe_type<int&>(), "reference to integer");
    EXPECT_EQ(describe_type<double&&>(), "reference to floating point");
    EXPECT_EQ(describe_type<const std::string&>(), "reference to const string");  // pitfall 6.3
}

TEST(Task3_DescribeType, Arrays) {
    EXPECT_EQ(describe_type<int[3]>(), "array 3 x integer");
    EXPECT_EQ(describe_type<char[2][5]>(), "array 2 x array 5 x char");
}

TEST(Task3_DescribeType, Decay) {
    // std::decay_t is the type "as after pass by value" (section 3.3)
    EXPECT_EQ(describe_type<std::decay_t<const int&>>(), "integer");
    EXPECT_EQ(describe_type<std::decay_t<int[3]>>(), "pointer to integer");
}

TEST(Task3_Passing, ByValueAndByReference) {
    const int constant = 1;
    int array[3] = {};
    std::string text = "abc";

    EXPECT_EQ(describe_by_value(constant), "integer");  // T = int: const is gone
    EXPECT_EQ(describe_by_reference(constant), "const integer");

    EXPECT_EQ(describe_by_value(array), "pointer to integer");  // T = int*: the array "decayed"
    EXPECT_EQ(describe_by_reference(array), "array 3 x integer");

    EXPECT_EQ(describe_by_value(text), "string");
    EXPECT_EQ(describe_by_reference(text), "string");
}
