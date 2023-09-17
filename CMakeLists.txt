cmake_minimum_required(VERSION 3.5)
project(myproject)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic --coverage")
include(cmake/FindGMP.cmake)

set(BUILD_TESTING ON)
enable_testing()

add_subdirectory(lib)
add_subdirectory(tests)

add_library(client client.c client.h)
target_link_libraries(client PRIVATE util fp_integer fp_poly list_lib)

add_executable(main main.c)
target_link_libraries(main PRIVATE client)
add_test(NAME main COMMAND main)
set_tests_properties(main PROPERTIES LABELS "main")
add_test(NAME main_memory COMMAND valgrind ./main)
set_tests_properties(main_memory PROPERTIES PASS_REGULAR_EXPRESSION "ERROR SUMMARY: 0 errors.*")
set_tests_properties(main_memory PROPERTIES LABELS "main;memory")