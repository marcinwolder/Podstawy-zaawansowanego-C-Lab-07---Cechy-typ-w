// EXPECTED_ERROR: describe_type: unsupported type
//
// This file must NOT compile. ctest checks that the compiler reports an error
// and that the error output contains the text from the EXPECTED_ERROR line.
// Task 3, TODO 1: a type outside the table ends in your static_assert, not in "?".
#include <vector>

#include "zadanie3.hpp"

int main() {
    describe_type<std::vector<int>>();
}
