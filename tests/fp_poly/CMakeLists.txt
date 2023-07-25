# suite 1: short without valgrind
# ctest -L suite1
# suite 2: short with valgrind
# ctest -L suite2
# suite 3: long without valgrind
# ctest -L suite3
# suite 4: long with valgrind
# ctest -L suite4

add_test(NAME generate_input_test_short COMMAND sh -c "echo -n short_test=1 > arg_pari_gp.gp; gp -q arg_pari_gp.gp generate_input.gp;" WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set_tests_properties(generate_input_test_short PROPERTIES LABELS "suite1;suite2")

add_test(NAME generate_input_test COMMAND sh -c "echo -n short_test=0 > arg_pari_gp.gp; gp -q arg_pari_gp.gp generate_input.gp;" WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set_tests_properties(generate_input_test PROPERTIES LABELS "suite3;suite4")

add_executable(parse test_parse.c)
target_include_directories(parse PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/fp_poly/include)
target_link_libraries(parse PRIVATE fp_poly)
add_test(NAME parse COMMAND parse)
add_test(NAME parse_memory COMMAND valgrind --leak-check=full ./parse)
set_tests_properties(parse_memory PROPERTIES TIMEOUT 10 PASS_REGULAR_EXPRESSION "ERROR SUMMARY: 0 errors.*")

add_executable(util test_util.c)
target_include_directories(util PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/fp_poly/include)
target_link_libraries(util PRIVATE fp_poly)
add_test(NAME util COMMAND util)
add_test(NAME util_memory COMMAND valgrind --leak-check=full ./util)
set_tests_properties(util_memory PROPERTIES TIMEOUT 10 PASS_REGULAR_EXPRESSION "ERROR SUMMARY: 0 errors.*")

add_executable(addition test_add.c)
target_include_directories(addition PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/fp_poly/include)
target_link_libraries(addition PRIVATE fp_poly)
add_test(NAME addition COMMAND ./addition)
set_tests_properties(addition PROPERTIES LABELS "suite1;suite3")
add_test(NAME addition_memory COMMAND valgrind --leak-check=full ./addition)
set_tests_properties(addition_memory PROPERTIES TIMEOUT 10 PASS_REGULAR_EXPRESSION "ERROR SUMMARY: 0 errors.*")
set_tests_properties(addition_memory PROPERTIES LABELS "suite2;suite4")

add_executable(substraction test_sub.c)
target_include_directories(substraction PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/fp_poly/include)
target_link_libraries(substraction PRIVATE fp_poly)
add_test(NAME substraction COMMAND substraction)
set_tests_properties(substraction PROPERTIES LABELS "suite1;suite3")
add_test(NAME substraction_memory COMMAND valgrind --leak-check=full ./substraction)
set_tests_properties(substraction_memory PROPERTIES TIMEOUT 10 PASS_REGULAR_EXPRESSION "ERROR SUMMARY: 0 errors.*")
set_tests_properties(substraction_memory PROPERTIES LABELS "suite2;suite4")

add_executable(multiplication test_mul.c)
target_include_directories(multiplication PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/fp_poly/include)
target_link_libraries(multiplication PRIVATE fp_poly)
add_test(NAME multiplication COMMAND multiplication)
set_tests_properties(multiplication PROPERTIES LABELS "suite1;suite3")
add_test(NAME multiplication_memory COMMAND valgrind --leak-check=full ./multiplication)
set_tests_properties(multiplication_memory PROPERTIES PASS_REGULAR_EXPRESSION "ERROR SUMMARY: 0 errors.*")
set_tests_properties(multiplication_memory PROPERTIES LABELS "suite2;suite4")

add_executable(division test_div.c)
target_include_directories(division PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/fp_poly/include)
target_link_libraries(division PRIVATE fp_poly)
add_test(NAME division COMMAND division)
set_tests_properties(division PROPERTIES LABELS "suite1;suite3")
add_test(NAME division_memory COMMAND valgrind --leak-check=full ./division)
set_tests_properties(division_memory PROPERTIES PASS_REGULAR_EXPRESSION "ERROR SUMMARY: 0 errors.*")
set_tests_properties(division_memory PROPERTIES LABELS "suite2;suite4")

add_executable(gcd test_gcd.c)
target_include_directories(gcd PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/fp_poly/include)
target_link_libraries(gcd PRIVATE fp_poly)
add_test(NAME gcd COMMAND gcd)
set_tests_properties(gcd PROPERTIES LABELS "suite1;suite3")
add_test(NAME gcd_memory COMMAND valgrind --leak-check=full ./gcd)
set_tests_properties(gcd_memory PROPERTIES PASS_REGULAR_EXPRESSION "ERROR SUMMARY: 0 errors.*")
set_tests_properties(gcd_memory PROPERTIES LABELS "suite2;suite4")

add_executable(multiplication_fq test_mul_fq.c)
target_include_directories(multiplication_fq PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/fp_poly/include)
target_link_libraries(multiplication_fq PRIVATE fp_poly)
add_test(NAME multiplication_fq COMMAND multiplication_fq)
set_tests_properties(multiplication_fq PROPERTIES LABELS "suite1;suite3")
add_test(NAME multiplication_fq_memory COMMAND valgrind --leak-check=full ./multiplication_fq)
set_tests_properties(multiplication_fq_memory PROPERTIES PASS_REGULAR_EXPRESSION "ERROR SUMMARY: 0 errors.*")
set_tests_properties(multiplication_fq_memory PROPERTIES LABELS "suite2;suite4")

add_executable(gcd_ext test_gcd_ext.c)
target_include_directories(gcd_ext PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/fp_poly/include)
target_link_libraries(gcd_ext PRIVATE fp_poly)
add_test(NAME gcd_ext COMMAND gcd_ext)
set_tests_properties(multiplication_fq PROPERTIES LABELS "suite1;suite3")
add_test(NAME gcd_ext_memory COMMAND valgrind --leak-check=full ./gcd_ext)
set_tests_properties(gcd_ext_memory PROPERTIES PASS_REGULAR_EXPRESSION "ERROR SUMMARY: 0 errors.*")
set_tests_properties(gcd_ext_memory PROPERTIES LABELS "suite2;suite4")