// EXPECTED_ERROR: swap_values: type must be copyable
//
// This file must NOT compile. ctest checks that the compiler reports an error
// and that the error output contains the text from the EXPECTED_ERROR line.
// Task 5, TODO 3: std::unique_ptr cannot be copied.
#include <memory>

#include "zadanie5.hpp"

int main() {
    std::unique_ptr<int> a, b;
    swap_values(a, b);
}
