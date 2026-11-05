// EXPECTED_ERROR: reallocate: type must be movable or copyable
//
// This file must NOT compile. ctest checks that the compiler reports an error
// and that the error output contains the text from the EXPECTED_ERROR line.
// Task 4, TODO 3: std::mutex can be neither copied nor moved.
#include <mutex>
#include <vector>

#include "zadanie4.hpp"

int main() {
    std::vector<std::mutex> old_vec;
    reallocate(old_vec);
}
