cmake_minimum_required(VERSION 3.5)
project(myproject)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic --coverage")
list(APPEND CMAKE_EXE_LINKER_FLAGS "--coverage")
include(cmake/FindGMP.cmake)

set(BUILD_TESTING ON)
enable_testing()

add_subdirectory(lib)
add_subdirectory(list)
add_subdirectory(tests)

add_library(client client.c client.h)
target_link_libraries(client PRIVATE fp_integer fp_poly list_lib)
add_executable(main main.c)
target_link_libraries(main PRIVATE client fp_integer fp_poly list_lib)
add_test(NAME main COMMAND main)
set_tests_properties(main PROPERTIES LABELS "suite1;suite3")
add_test(NAME main_memory COMMAND valgrind ./main)
set_tests_properties(main_memory PROPERTIES TIMEOUT 10 PASS_REGULAR_EXPRESSION "ERROR SUMMARY: 0 errors.*")
set_tests_properties(main PROPERTIES LABELS "suite2;suite4")

add_compile_options(--coverage)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")

add_custom_target(coverage
    COMMAND lcov --rc lcov_branch_coverage=1 --capture --directory . --output-file coverage.info
    COMMAND genhtml coverage.info --output-directory coverage_report
    COMMAND firefox coverage_report/index.html
    COMMENT "Generating coverage report"
)