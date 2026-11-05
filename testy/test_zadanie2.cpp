// Tests for task 2. DO NOT MODIFY this file.
#include <gtest/gtest.h>

#include <string>
#include <type_traits>

#include "type_eq.hpp"
#include "zadanie2.hpp"

TEST(Task2_RemoveConst, Basics) {
    EXPECT_TYPE_EQ(RemoveConst<const int>::type, int);
    EXPECT_TYPE_EQ(RemoveConst<int>::type, int);  // no const - no change
    EXPECT_TYPE_EQ(RemoveConst<const std::string>::type, std::string);
}

TEST(Task2_RemoveConst, TopLevelOnly) {
    // const int* is a "pointer to const int" - the pointer itself is NOT const
    EXPECT_TYPE_EQ(RemoveConst<const int*>::type, const int*);
    // int* const is a "const pointer to int" - here const is at the top level
    EXPECT_TYPE_EQ(RemoveConst<int* const>::type, int*);
}

TEST(Task2_RemoveReference, Basics) {
    EXPECT_TYPE_EQ(RemoveReference<int&>::type, int);
    EXPECT_TYPE_EQ(RemoveReference<int&&>::type, int);
    EXPECT_TYPE_EQ(RemoveReference<int>::type, int);
    EXPECT_TYPE_EQ(RemoveReference<const int&>::type, const int);  // const stays
}

TEST(Task2_Common, TShortcuts) {
    EXPECT_TYPE_EQ(RemoveConst_t<const double>, double);
    EXPECT_TYPE_EQ(RemoveReference_t<char&>, char);
    EXPECT_TYPE_EQ(RemoveReference_t<std::string&&>, std::string);
}

TEST(Task2_AddPointer, Basics) {
    EXPECT_TYPE_EQ(AddPointer_t<int>, int*);
    EXPECT_TYPE_EQ(AddPointer_t<const char>, const char*);
    EXPECT_TYPE_EQ(AddPointer_t<int*>, int**);
}

TEST(Task2_AddPointer, ForReferences) {
    EXPECT_TYPE_EQ(AddPointer_t<int&>, int*);
    EXPECT_TYPE_EQ(AddPointer_t<int&&>, int*);
    EXPECT_TYPE_EQ(AddPointer_t<const int&>, const int*);
}

TEST(Task2_Pitfall63, RemoveConstDoesNotWorkThroughReference) {
    // This test passes from the start. It shows WHY RemoveConstRef is necessary.
    EXPECT_TYPE_EQ(RemoveConst_t<const int&>, const int&);  // no change!
    EXPECT_TYPE_EQ(std::remove_const_t<const int&>, const int&);  // std behaves the same
}

TEST(Task2_RemoveConstRef, Basics) {
    EXPECT_TYPE_EQ(RemoveConstRef_t<const int&>, int);
    EXPECT_TYPE_EQ(RemoveConstRef_t<const int&&>, int);
    EXPECT_TYPE_EQ(RemoveConstRef_t<int&>, int);
    EXPECT_TYPE_EQ(RemoveConstRef_t<const int>, int);
    EXPECT_TYPE_EQ(RemoveConstRef_t<const std::string&>, std::string);
    EXPECT_TYPE_EQ(RemoveConstRef_t<const char*&>, const char*);  // const "inside" the pointer stays
}

TEST(Task2_Common, MatchesStandardLibrary) {
    EXPECT_TYPE_EQ(RemoveConst_t<const long>, std::remove_const_t<const long>);
    EXPECT_TYPE_EQ(RemoveReference_t<float&>, std::remove_reference_t<float&>);
    EXPECT_TYPE_EQ(AddPointer_t<double&>, std::add_pointer_t<double&>);
#if defined(__cpp_lib_remove_cvref)
    // std::remove_cvref_t exists since C++20. An older compiler skips this part.
    EXPECT_TYPE_EQ(RemoveConstRef_t<const int&>, std::remove_cvref_t<const int&>);
#endif
}
