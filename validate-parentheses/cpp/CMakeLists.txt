cmake_minimum_required(VERSION 3.16)
project(validate_parentheses CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

add_compile_options(-Wall -Wextra)

include_directories(include)

add_library(solution src/solution.cpp)

add_executable(main src/main.cpp)
target_link_libraries(main PRIVATE solution)

add_executable(tests tests/test_solution.cpp)
target_link_libraries(tests PRIVATE solution)

enable_testing()
add_test(NAME solution_tests COMMAND tests)
