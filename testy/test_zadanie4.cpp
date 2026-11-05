// Tests for task 4. DO NOT MODIFY this file.
// The "must not compile" test is in bledy/zadanie4_mutex.cpp (ctest runs it).
#include <gtest/gtest.h>

#include <string>
#include <type_traits>
#include <vector>

#include "type_eq.hpp"
#include "zadanie4.hpp"

// The test types count how many times they were copied and moved.
struct Counters {
    inline static int copies = 0;
    inline static int moves = 0;
    static void reset() { copies = moves = 0; }
};

struct Safe {  // noexcept move -> we move
    int value;
    Safe(int v) : value(v) {}
    Safe(const Safe& o) : value(o.value) { ++Counters::copies; }
    Safe(Safe&& o) noexcept : value(o.value) { ++Counters::moves; }
};

struct Risky {  // move without noexcept -> we copy
    int value;
    Risky(int v) : value(v) {}
    Risky(const Risky& o) : value(o.value) { ++Counters::copies; }
    Risky(Risky&& o) : value(o.value) { ++Counters::moves; }
};

struct MoveOnly {  // no copy -> we move, even without noexcept
    int value;
    MoveOnly(int v) : value(v) {}
    MoveOnly(const MoveOnly&) = delete;
    MoveOnly(MoveOnly&& o) : value(o.value) { ++Counters::moves; }
};

template <typename T>
std::vector<T> three_elements() {
    std::vector<T> v;
    v.reserve(3);
    v.emplace_back(1);
    v.emplace_back(2);
    v.emplace_back(3);
    Counters::reset();
    return v;
}

TEST(Task4_StdVector, DoesTheSame) {
    // This test passes from the start. It shows how std::vector behaves on reallocation.
    std::vector<Safe> s;
    s.reserve(1);
    s.emplace_back(1);
    Counters::reset();
    s.emplace_back(2);  // no space -> new buffer -> the old element is MOVED
    EXPECT_EQ(Counters::moves, 1);
    EXPECT_EQ(Counters::copies, 0);

    std::vector<Risky> r;
    r.reserve(1);
    r.emplace_back(1);
    Counters::reset();
    r.emplace_back(2);  // the move can throw -> the old element is COPIED
    EXPECT_EQ(Counters::moves, 0);
    EXPECT_EQ(Counters::copies, 1);
}

TEST(Task4_MoveResult, TestTypes) {
    EXPECT_TYPE_EQ(MoveResult<Safe>, Safe&&);
    EXPECT_TYPE_EQ(MoveResult<Risky>, const Risky&);
    EXPECT_TYPE_EQ(MoveResult<MoveOnly>, MoveOnly&&);
}

TEST(Task4_MoveResult, LibraryTypes) {
    EXPECT_TYPE_EQ(MoveResult<int>, int&&);
    EXPECT_TYPE_EQ(MoveResult<std::string>, std::string&&);  // the move of std::string is noexcept
    EXPECT_TYPE_EQ(MoveResult<std::vector<int>>, std::vector<int>&&);
}

TEST(Task4_MoveOrCopy, SafeAndRisky) {
    Safe s(7);
    Risky r(8);
    Counters::reset();

    Safe new_s(move_or_copy(s));
    EXPECT_EQ(Counters::moves, 1);
    EXPECT_EQ(Counters::copies, 0);

    Counters::reset();
    Risky new_r(move_or_copy(r));
    EXPECT_EQ(Counters::moves, 0);
    EXPECT_EQ(Counters::copies, 1);

    EXPECT_EQ(new_s.value, 7);
    EXPECT_EQ(new_r.value, 8);
}

TEST(Task4_Reallocate, SafeIsMoved) {
    auto old_vec = three_elements<Safe>();
    auto new_vec = reallocate(old_vec);
    EXPECT_GE(new_vec.capacity(), 6u);
    EXPECT_EQ(Counters::moves, 3);
    EXPECT_EQ(Counters::copies, 0);
    ASSERT_EQ(new_vec.size(), 3u);  // ASSERT: without 3 elements the next checks make no sense
    EXPECT_EQ(new_vec[0].value, 1);
    EXPECT_EQ(new_vec[2].value, 3);
}

TEST(Task4_Reallocate, RiskyIsCopied) {
    auto old_vec = three_elements<Risky>();
    auto new_vec = reallocate(old_vec);
    EXPECT_EQ(new_vec.size(), 3u);
    EXPECT_EQ(Counters::moves, 0);
    EXPECT_EQ(Counters::copies, 3);
    EXPECT_EQ(old_vec.size(), 3u);  // the old data is untouched - the copy was safe
}

TEST(Task4_Reallocate, MoveOnlyIsMoved) {
    auto old_vec = three_elements<MoveOnly>();
    auto new_vec = reallocate(old_vec);
    EXPECT_EQ(new_vec.size(), 3u);
    EXPECT_EQ(Counters::moves, 3);
}

TEST(Task4_Reallocate, EmptyVector) {
    std::vector<Safe> old_vec;
    auto new_vec = reallocate(old_vec);
    EXPECT_TRUE(new_vec.empty());
    EXPECT_GE(new_vec.capacity(), 1u);
}

TEST(Task4_Reallocate, StdString) {
    std::vector<std::string> old_vec = {"the", "black", "cat"};
    auto new_vec = reallocate(old_vec);
    ASSERT_EQ(new_vec.size(), 3u);
    EXPECT_EQ(new_vec[2], "cat");
}
