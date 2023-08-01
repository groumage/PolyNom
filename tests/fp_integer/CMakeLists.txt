add_test(NAME generate_input_integer COMMAND sh -c "gp generate_input_integer.gp;" WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set_tests_properties(generate_input_integer PROPERTIES LABELS "integer")

add_executable(random_prime_generate test_random_prime.c)
target_include_directories(random_prime_generate PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/fp_integer/include)
target_link_libraries(random_prime_generate PRIVATE fp_integer)
add_test(NAME random_prime_generate COMMAND random_prime_generate)
set_tests_properties(random_prime_generate PROPERTIES LABELS "integer")

add_test(NAME random_prime_verify COMMAND sh -c "gp -q verify_input.gp;" WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set_tests_properties(random_prime_verify PROPERTIES LABELS "integer")

add_executable(gcd_ext_integer test_gcd_ext.c)
target_include_directories(gcd_ext_integer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/fp_integer/include)
target_link_libraries(gcd_ext_integer PRIVATE fp_integer)
add_test(NAME gcd_ext_integer COMMAND gcd_ext_integer)
set_tests_properties(gcd_ext_integer PROPERTIES LABELS "integer")