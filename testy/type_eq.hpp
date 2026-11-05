// Test helpers for Lab 07 (GoogleTest). DO NOT MODIFY this file.
//
// The compiler computes type traits, but the tests check the result only at
// run time (EXPECT_TRUE, EXPECT_TYPE_EQ). Because of this, the program compiles
// from the start and shows ALL tests, not only the first error, as static_assert
// or ::testing::StaticAssertTypeEq would.
//
//   EXPECT_TYPE_EQ(TypeA, TypeB)   - the same types; on failure prints both types
//                                    with const and & (typeid would drop them)
#pragma once

#include <gtest/gtest.h>

#include <string>
#include <string_view>
#include <type_traits>

namespace test_helpers {

// Type name as text, e.g. "const int &". The compiler puts T into the function name.
template <typename T>
std::string_view type_name() {
#if defined(__clang__)
    std::string_view p = __PRETTY_FUNCTION__;  // "... type_name() [T = const int &]"
    auto start = p.find("T = ") + 4;
    return p.substr(start, p.rfind(']') - start);
#elif defined(__GNUC__)
    std::string_view p = __PRETTY_FUNCTION__;  // "... [with T = const int&; std::string_view = ...]"
    auto start = p.find("T = ") + 4;
    auto end = p.find("; ", start);
    if (end == std::string_view::npos)
        end = p.rfind(']');
    return p.substr(start, end - start);
#elif defined(_MSC_VER)
    std::string_view p = __FUNCSIG__;  // "... type_name<const int&>(void)"
    auto start = p.find("type_name<") + 10;
    return p.substr(start, p.rfind(">(void)") - start);
#else
    return "?";
#endif
}

template <typename A, typename B>
::testing::AssertionResult same_types() {
    if constexpr (std::is_same_v<A, B>)
        return ::testing::AssertionSuccess();
    else
        return ::testing::AssertionFailure()
               << "\n  actual:   " << type_name<A>() << "\n  expected: " << type_name<B>();
}

}  // namespace test_helpers

// The macro is variadic, because the comma in <int, int> would split a macro argument.
#define EXPECT_TYPE_EQ(...) EXPECT_TRUE((test_helpers::same_types<__VA_ARGS__>()))
