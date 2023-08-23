add_executable(random_prime test_random_prime.c)
target_include_directories(random_prime PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/fp_integer/include)
target_link_libraries(random_prime PRIVATE fp_integer)

add_test(NAME random_prime_short COMMAND random_prime 1)
set_tests_properties(random_prime_short PROPERTIES LABELS "nominal;short-integer")
add_test(NAME random_prime_long COMMAND random_prime 0)
set_tests_properties(random_prime_long PROPERTIES LABELS "nominal;long-integer")

add_test(NAME random_prime_short_memory COMMAND valgrind ./random_prime 1)
set_tests_properties(random_prime_short_memory PROPERTIES PASS_REGULAR_EXPRESSION "ERROR SUMMARY: 0 errors.*")
set_tests_properties(random_prime_short_memory PROPERTIES LABELS "memory;short-integer")

add_test(NAME random_prime_long_memory COMMAND valgrind ./random_prime 0)
set_tests_properties(random_prime_long_memory PROPERTIES PASS_REGULAR_EXPRESSION "ERROR SUMMARY: 0 errors.*")
set_tests_properties(random_prime_long_memory PROPERTIES LABELS "memory;long-integer")

add_test(NAME random_prime_verify COMMAND sh -c "gp -q verify_input_prime.gp;" WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set_tests_properties(random_prime_verify PROPERTIES LABELS "nominal")

add_executable(gcd_ext_integer test_gcd_ext.c)
target_include_directories(gcd_ext_integer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/fp_integer/include)
target_link_libraries(gcd_ext_integer PRIVATE fp_integer)
add_test(NAME gcd_ext_integer COMMAND gcd_ext_integer)
set_tests_properties(gcd_ext_integer PROPERTIES LABELS "nominal")
add_test(NAME gcd_ext_integer_memory COMMAND valgrind ./gcd_ext_integer)
set_tests_properties(gcd_ext_integer_memory PROPERTIES PASS_REGULAR_EXPRESSION "ERROR SUMMARY: 0 errors.*")
set_tests_properties(gcd_ext_integer_memory PROPERTIES LABELS "memory")