cmake_minimum_required(VERSION 3.15)

project(custom-stl
    VERSION 0.1.0
    DESCRIPTION "Custom STL C++"
    LANGUAGES CXX
)

# Use C++20 (Fixed your comment which said C99)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Add include directory
# It's better to use the absolute path variable CMAKE_CURRENT_SOURCE_DIR
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

FetchContent_MakeAvailable(googletest)
# ===============================
# Core Library Sources
# ===============================

# NOTE: CMake does not support wildcards like src/* directly in 'set' 
# for compilation. You should list files explicitly or use file(GLOB ...).
# But for now, we just focus on main.cpp.

enable_testing()

add_executable(test_sort tests/test_sort.cpp)
target_link_libraries(test_sort gtest_main)
add_test(NAME SortTests COMMAND test_sort)

add_executable(test_find tests/test_find.cpp)
target_link_libraries(test_find gtest_main)
add_test(NAME FindTests COMMAND test_find)

add_executable(test_reverse tests/test_reverse.cpp)
target_link_libraries(test_reverse gtest_main)
add_test(NAME ReverseTests COMMAND test_reverse)

add_executable(test_rotate tests/test_rotate.cpp)
target_link_libraries(test_rotate gtest_main)
add_test(NAME RotateTests COMMAND test_rotate)

add_executable(test_vector tests/test_vector.cpp)
target_link_libraries(test_vector gtest_main)
add_test(NAME VectorTests COMMAND test_vector)

add_executable(main src/main.cpp)
