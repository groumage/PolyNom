cmake_minimum_required(VERSION 3.5)
project(myproject)

include(cmake/FindGMP.cmake)
set(BUILD_TESTING ON)

add_library(list_lib STATIC list/src/list.c)
target_include_directories(list_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/list/include)
add_library(list_malloc STATIC list/src/list.c)
target_include_directories(list_malloc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/list/include)
target_compile_definitions(list_malloc PUBLIC MOCK_MALLOC)

add_library(fp_poly STATIC fp_poly/src/fp_poly.c)
target_include_directories(fp_poly PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/fp_poly/include)
target_link_libraries(fp_poly PRIVATE ${GMP_LIBRARIES})
target_link_libraries(fp_poly PRIVATE list_lib)

enable_testing()

add_executable(list list/tests/test_list.c)
target_include_directories(list PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/list/include)
target_link_libraries(list PRIVATE list_lib)
add_test(NAME list COMMAND list)
add_test(NAME list_memory COMMAND valgrind --leak-check=full ./list)
set_tests_properties(list_memory PROPERTIES TIMEOUT 10 PASS_REGULAR_EXPRESSION "ERROR SUMMARY: 0 errors.*")

add_executable(list_mock_malloc list/tests/test_list_malloc.c)
target_include_directories(list_mock_malloc PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/list/include)
target_link_libraries(list_mock_malloc PRIVATE list_malloc)
add_test(NAME list_mock_malloc COMMAND list_mock_malloc)
#add_test(NAME list_mock_malloc_memory COMMAND valgrind --leak-check=full ./list_mock_malloc)
#set_tests_properties(list_mock_malloc_memory PROPERTIES TIMEOUT 10 PASS_REGULAR_EXPRESSION "ERROR SUMMARY: 0 errors.*")

add_executable(parse fp_poly/tests/test_parse.c)
target_include_directories(parse PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/fp_poly/include)
target_link_libraries(parse PRIVATE fp_poly)
add_test(NAME parse COMMAND parse)
add_test(NAME parse_memory COMMAND valgrind --leak-check=full ./parse)
set_tests_properties(parse_memory PROPERTIES TIMEOUT 10 PASS_REGULAR_EXPRESSION "ERROR SUMMARY: 0 errors.*")

add_executable(util fp_poly/tests/test_util.c)
target_include_directories(util PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/fp_poly/include)
target_link_libraries(util PRIVATE fp_poly)
add_test(NAME util COMMAND util)
add_test(NAME util_memory COMMAND valgrind --leak-check=full ./util)
set_tests_properties(util_memory PROPERTIES TIMEOUT 10 PASS_REGULAR_EXPRESSION "ERROR SUMMARY: 0 errors.*")

add_executable(addition fp_poly/tests/test_add.c)
target_include_directories(addition PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/fp_poly/include)
target_link_libraries(addition PRIVATE fp_poly)
add_test(NAME addition COMMAND sh -c "gp -q ../fp_poly/tests/generate_input.gp; ./addition")
add_test(NAME addition_memory COMMAND valgrind --leak-check=full ./addition)
set_tests_properties(addition_memory PROPERTIES TIMEOUT 10 PASS_REGULAR_EXPRESSION "ERROR SUMMARY: 0 errors.*")

# Set the C standard
set(CMAKE_C_STANDARD 11)

# Set the C flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic --coverage")
list(APPEND CMAKE_EXE_LINKER_FLAGS "--coverage")

# Add the fp_poly library and list library to the main executable
add_executable(main main.c)
target_link_libraries(main PRIVATE fp_poly list_lib)

#add_custom_target(coverage
#    COMMAND gcovr --root=../ --html --html-details -o ../coverage/coverage.html
#    COMMENT "Generating coverage report"
#)

add_compile_options(--coverage)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")

add_custom_target(coverage
    COMMAND lcov --rc lcov_branch_coverage=1 --capture --directory . --output-file coverage.info
    COMMAND genhtml coverage.info --output-directory coverage_report
    COMMAND firefox coverage_report/index.html
    COMMENT "Generating coverage report"
)