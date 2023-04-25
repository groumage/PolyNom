cmake_minimum_required(VERSION 3.5)

project(myproject)

include(cmake/FindGMP.cmake)

set (BUILD_TESTING ON)


# Add list library
add_library(list STATIC list/src/list.c)
target_include_directories(list PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/list/include)

# Add fp_poly library
add_library(fp_poly STATIC fp_poly/src/fp_poly.c)
target_include_directories(fp_poly PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/fp_poly/include)
target_link_libraries(fp_poly PRIVATE ${GMP_LIBRARIES})
target_link_libraries(fp_poly PRIVATE list)

enable_testing()

# Add list test executable
add_executable(list_test list/tests/test_list.c)
target_include_directories(list_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/list/include)
target_link_libraries(list_test PRIVATE list)
add_test(NAME list_test COMMAND list_test)
add_test(NAME list_test_mem_check COMMAND valgrind --leak-check=full ./list_test)
set_tests_properties(list_test_mem_check PROPERTIES TIMEOUT 10 PASS_REGULAR_EXPRESSION "ERROR SUMMARY: 0 errors.*")

# Add fp_poly_add_test executable
add_executable(add_test fp_poly/tests/test_add.c)
target_include_directories(add_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/fp_poly/include)
target_link_libraries(add_test PRIVATE fp_poly)
add_test(NAME add_test COMMAND add_test)
add_test(NAME add_test_mem_check COMMAND valgrind --leak-check=full ./add_test)
set_tests_properties(add_test_mem_check PROPERTIES TIMEOUT 10 PASS_REGULAR_EXPRESSION "ERROR SUMMARY: 0 errors.*")

# Set the C standard
set(CMAKE_C_STANDARD 11)

# Set the C flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic --coverage")
set(CMAKE_EXE_LINKER_FLAGS "--coverage")


# Add the fp_poly library and list library to the main executable
add_executable(main main.c)
target_link_libraries(main PRIVATE fp_poly list)

add_custom_target(coverage
    COMMAND gcovr --root=../ --html --html-details -o ../coverage/coverage.html
    COMMENT "Generating coverage report"
)