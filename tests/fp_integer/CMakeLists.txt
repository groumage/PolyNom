add_executable(random_prime_generate test_random_prime.c)
target_include_directories(random_prime_generate PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/fp_integer/include)
target_link_libraries(random_prime_generate PRIVATE fp_integer)
add_test(NAME random_prime_generate COMMAND random_prime_generate)
set_tests_properties(random_prime_generate PROPERTIES LABELS "suite1")

add_test(NAME random_prime_verify COMMAND sh -c "gp -q verify_input.gp;" WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set_tests_properties(random_prime_verify PROPERTIES LABELS "suite1")