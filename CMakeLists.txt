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

# Add the fp_poly library and list library to the main executable
add_executable(main main.c)
target_link_libraries(main PRIVATE fp_integer fp_poly list_lib)

add_compile_options(--coverage)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")

add_custom_target(coverage
    COMMAND lcov --rc lcov_branch_coverage=1 --capture --directory . --output-file coverage.info
    COMMAND genhtml coverage.info --output-directory coverage_report
    COMMAND firefox coverage_report/index.html
    COMMENT "Generating coverage report"
)