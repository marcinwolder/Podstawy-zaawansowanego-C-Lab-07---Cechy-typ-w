// Tests for task 1. DO NOT MODIFY this file.
#include <gtest/gtest.h>

#include <type_traits>

#include "zadanie1.hpp"

struct Cat {};

TEST(Task1_IsPointer, Pointers) {
    EXPECT_TRUE(IsPointer<int*>::value);
    EXPECT_TRUE(IsPointer<const char*>::value);
    EXPECT_TRUE(IsPointer<Cat*>::value);
    EXPECT_TRUE(IsPointer<int**>::value);
}

TEST(Task1_IsPointer, NonPointers) {
    EXPECT_FALSE(IsPointer<int>::value);
    EXPECT_FALSE(IsPointer<int&>::value);
    EXPECT_FALSE(IsPointer<int[3]>::value);  // an array is NOT a pointer
    EXPECT_FALSE(IsPointer<Cat>::value);
}

TEST(Task1_IsSame, SameTypes) {
    EXPECT_TRUE((IsSame<int, int>::value));
    EXPECT_TRUE((IsSame<unsigned, unsigned int>::value));  // two names of one type
    EXPECT_TRUE((IsSame<Cat*, Cat*>::value));
}

TEST(Task1_IsSame, DifferentTypes) {
    EXPECT_FALSE((IsSame<int, long>::value));
    EXPECT_FALSE((IsSame<int, const int>::value));  // const changes the type
    EXPECT_FALSE((IsSame<int, int&>::value));
    EXPECT_FALSE((IsSame<char, signed char>::value));  // char, signed char, unsigned char - three different types
}

TEST(Task1_IsVoid, VoidAndOthers) {
    EXPECT_TRUE(IsVoid<void>::value);
    EXPECT_FALSE(IsVoid<int>::value);
    EXPECT_FALSE(IsVoid<void*>::value);
}

TEST(Task1_IsArray, Arrays) {
    EXPECT_TRUE(IsArray<int[5]>::value);
    EXPECT_TRUE(IsArray<double[1]>::value);
    EXPECT_TRUE(IsArray<int[]>::value);  // unknown size
    EXPECT_TRUE(IsArray<int[2][3]>::value);
}

TEST(Task1_IsArray, NonArrays) {
    EXPECT_FALSE(IsArray<int>::value);
    EXPECT_FALSE(IsArray<int*>::value);
}

TEST(Task1_Common, InheritsFromIntegralConstant) {
    // a "true" trait inherits from std::true_type, a "false" one from std::false_type (section 2.2)
    EXPECT_TRUE((std::is_base_of_v<std::true_type, IsPointer<int*>>));
    EXPECT_TRUE((std::is_base_of_v<std::false_type, IsPointer<int>>));
    EXPECT_TRUE((std::is_base_of_v<std::true_type, IsSame<int, int>>));
    EXPECT_TRUE((std::is_base_of_v<std::true_type, IsArray<int[2]>>));
}

TEST(Task1_Common, VShortcuts) {
    EXPECT_TRUE(IsPointer_v<int*>);
    EXPECT_FALSE(IsPointer_v<int>);
    EXPECT_TRUE((IsSame_v<int, int>));
    EXPECT_FALSE((IsSame_v<int, char>));
    EXPECT_TRUE(IsVoid_v<void>);
    EXPECT_TRUE(IsArray_v<int[4]>);
    EXPECT_TRUE(IsArray_v<int[]>);
}

TEST(Task1_Common, MatchesStandardLibrary) {
    // Your traits give the same result as std:: for plain (non-const) types.
    EXPECT_EQ(IsPointer_v<int*>, std::is_pointer_v<int*>);
    EXPECT_EQ(IsPointer_v<int (*)(int)>, std::is_pointer_v<int (*)(int)>);  // pointer to function
    EXPECT_EQ(IsArray_v<char[7]>, std::is_array_v<char[7]>);
    EXPECT_EQ((IsSame_v<long long, long long int>), (std::is_same_v<long long, long long int>));
}
