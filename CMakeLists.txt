cmake_minimum_required(VERSION 3.20)
project(type_traits_lab LANGUAGES CXX)

# C++20 by default. Older compiler: cmake -S . -B build -DCMAKE_CXX_STANDARD=17
if(NOT DEFINED CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 20)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release)
endif()

# GoogleTest: the system one, or downloaded (git) when it is missing.
find_package(GTest QUIET)
if(NOT GTest_FOUND)
  message(STATUS "GoogleTest not found on the system - downloading it (FetchContent)")
  include(FetchContent)
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.15.2
    GIT_SHALLOW    TRUE
  )
  set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)
endif()

include(GoogleTest)
enable_testing()

function(add_lab_flags target)
  target_include_directories(${target} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/zadania
    ${CMAKE_CURRENT_SOURCE_DIR}/testy)
  if(MSVC)
    target_compile_options(${target} PRIVATE /W4 /utf-8 /permissive-)
  else()
    target_compile_options(${target} PRIVATE -Wall -Wextra)
  endif()
endfunction()

# For task N this creates:
#   - the executable taskN = testy/test_zadanieN.cpp + zadania/zadanieN.hpp,
#   - "must not compile" tests from bledy/zadanieN_*.cpp (targets taskN_error_*,
#     excluded from "all": ctest builds them and checks the error message).
# All tests of task N have the label taskN, so: ctest -L task3
foreach(n RANGE 1 5)
  set(target task${n})
  add_executable(${target} testy/test_zadanie${n}.cpp)
  target_link_libraries(${target} PRIVATE GTest::gtest_main)
  add_lab_flags(${target})
  gtest_discover_tests(${target}
    TEST_PREFIX "${target}."
    DISCOVERY_MODE PRE_TEST
    PROPERTIES LABELS ${target})

  file(GLOB error_files CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/bledy/zadanie${n}_*.cpp)
  foreach(file IN LISTS error_files)
    get_filename_component(file_name ${file} NAME_WE)            # zadanie4_mutex
    string(REPLACE "zadanie${n}_" "" error_name ${file_name})    # mutex
    set(error_target ${target}_error_${error_name})
    set(test_name ${target}.MustNotCompile.${error_name})

    file(STRINGS ${file} line REGEX "^// EXPECTED_ERROR: " LIMIT_COUNT 1)
    string(REPLACE "// EXPECTED_ERROR: " "" expected "${line}")
    if(expected STREQUAL "")
      message(FATAL_ERROR "${file}: missing line // EXPECTED_ERROR: ...")
    endif()

    add_executable(${error_target} EXCLUDE_FROM_ALL ${file})
    set_target_properties(${error_target} PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD TRUE)
    add_lab_flags(${error_target})

    add_test(NAME ${test_name}
      COMMAND ${CMAKE_COMMAND}
        -DBUILD_DIR=${CMAKE_BINARY_DIR}
        -DTARGET=${error_target}
        -DCONFIG=$<CONFIG>
        -DFILE=bledy/${file_name}.cpp
        -DEXPECTED=${expected}
        -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/must_not_compile.cmake)
    # Two parallel "cmake --build" runs in one build directory can interfere.
    set_tests_properties(${test_name} PROPERTIES
      LABELS ${target}
      RESOURCE_LOCK build_dir)
  endforeach()
endforeach()
