#include "../../fp_poly/include/fp_poly.h"
#include <errno.h>
#include <unistd.h>

#define BUFFER_SIZE 32768

static void remove_last_newline(char* str)
{
    size_t length;
    
    length = strlen(str);
    if (length > 0 && str[length - 1] == '\n')
        str[length - 1] = '\0';
}

static void hello_world_tests()
{
    fp_poly_t *p1, *p2, *res;
    list_t *coeff_p1, *coeff_p2, *coeff_res;

    // p1 = 1 + x + x^3
    p1 = fp_poly_init_array((uint8_t[]) {1, 1, 0, 1}, 4);
    assert (p1 != NULL);
    coeff_p1 = list_create_from_array((uint8_t[]) {1, 1, 1}, 3);
    assert (coeff_p1 != NULL);
    assert (fp_poly_assert_sizet(p1, 11, coeff_p1) == FP_POLY_E_SUCCESS);

    // p2 = 1 + x^2 + x^3
    p2 = fp_poly_init_array((uint8_t[]) {1, 0, 1, 1}, 4);
    assert (p2 != NULL);
    coeff_p2 = list_create_from_array((uint8_t[]) {1, 1, 1}, 3);
    assert (coeff_p2 != NULL);
    assert (fp_poly_assert_sizet(p2, 13, coeff_p2) == FP_POLY_E_SUCCESS);

    // add "1" to p1: p1 = 2 + x + x^3
    assert (list_destroy(coeff_p1) == LIST_E_SUCCESS);
    assert (fp_poly_add_single_term(p1, 1, 0, NULL) == FP_POLY_E_SUCCESS);
    coeff_p1 = list_create_from_array((uint8_t[]) {2, 1, 1}, 3);
    assert (fp_poly_assert_sizet(p1, 11, coeff_p1) == FP_POLY_E_SUCCESS);
    // add "x" to p1: p1 = 2 + 2*x + x^3
    assert (list_destroy(coeff_p1) == LIST_E_SUCCESS);
    assert (fp_poly_add_single_term(p1, 1, 1, NULL) == FP_POLY_E_SUCCESS);
    coeff_p1 = list_create_from_array((uint8_t[]) {2, 2, 1}, 3);
    assert (fp_poly_assert_sizet(p1, 11, coeff_p1) == FP_POLY_E_SUCCESS);
    // add "x^2" to p1: p1 = 2 + 2*x + x^2 + x^3
    assert (list_destroy(coeff_p1) == LIST_E_SUCCESS);
    assert (fp_poly_add_single_term(p1, 1, 2, NULL) == FP_POLY_E_SUCCESS);
    coeff_p1 = list_create_from_array((uint8_t[]) {2, 2, 1, 1}, 4);
    assert (fp_poly_assert_sizet(p1, 15, coeff_p1) == FP_POLY_E_SUCCESS);
    // add "5*x^3" to p1: p1 = 2 + 2*x + x^2 + 6*x^3
    assert (list_destroy(coeff_p1) == LIST_E_SUCCESS);
    assert (fp_poly_add_single_term(p1, 5, 3, NULL) == FP_POLY_E_SUCCESS);
    coeff_p1 = list_create_from_array((uint8_t[]) {2, 2, 1, 6}, 4);
    assert (fp_poly_assert_sizet(p1, 15, coeff_p1) == FP_POLY_E_SUCCESS);

    // addition of p1 = 2 + 2*x + x^2 + 6*x^3 and p2 = 1 + x^2 + x^3 => res = 3 + 3*x + 2*x^2 + 7*x^3
    coeff_res = list_create_from_array((uint8_t[]) {3, 2, 2, 7}, 4);
    assert (fp_poly_add(&res, p1, p2, NULL) == FP_POLY_E_SUCCESS);
    assert (fp_poly_assert_sizet(res, 15, coeff_res) == FP_POLY_E_SUCCESS);
    assert (list_destroy(coeff_res) == LIST_E_SUCCESS);
    assert (fp_poly_free(res) == FP_POLY_E_SUCCESS);
    assert (list_destroy(coeff_p2) == LIST_E_SUCCESS);
    assert (fp_poly_free(p2) == FP_POLY_E_SUCCESS);
    assert (list_destroy(coeff_p1) == LIST_E_SUCCESS);
    assert (fp_poly_free(p1) == FP_POLY_E_SUCCESS);

    // p1 = 1 + x + x^2 + x^3 + x^7 + x^8
    p1 = fp_poly_init_array((uint8_t[]) {1, 1, 1, 1, 0, 0, 0, 1, 1}, 9);
    assert (p1 != NULL);
    coeff_p1 = list_create_from_array((uint8_t[]) {1, 1, 1, 1, 1, 1}, 6);
    assert (coeff_p1 != NULL);
    assert (fp_poly_assert_sizet(p1, 399, coeff_p1) == FP_POLY_E_SUCCESS);

    // p2 = 1 + x + x^4 + x^6 + x^8
    p2 = fp_poly_init_array((uint8_t[]) {1, 1, 0, 0, 1, 0, 1, 0, 1}, 9);
    assert (p2 != NULL);
    coeff_p2 = list_create_from_array((uint8_t[]) {1, 1, 1, 1, 1}, 5);
    assert (coeff_p2 != NULL);
    assert (fp_poly_assert_sizet(p2, 339, coeff_p2) == FP_POLY_E_SUCCESS);

    // addition of p1 = 1 + x + x^2 + x^3 + x^7 + x^8 and p2 = 1 + x + x^4 + x^6 + x^8 => res = 2 + 2*x + x^2 + x^3 + x^4 + x^6 + x^7 + 2*x^8
    coeff_res = list_create_from_array((uint8_t[]) {2, 2, 1, 1, 1, 1, 1, 2}, 8);
    assert (fp_poly_add(&res, p1, p2, NULL) == FP_POLY_E_SUCCESS);
    assert (fp_poly_assert_sizet(res, 479, coeff_res) == FP_POLY_E_SUCCESS);

    assert (list_destroy(coeff_p2) == LIST_E_SUCCESS);
    assert (fp_poly_free(p2) == FP_POLY_E_SUCCESS);
    assert (list_destroy(coeff_p1) == LIST_E_SUCCESS);
    assert (fp_poly_free(p1) == FP_POLY_E_SUCCESS);
    assert (list_destroy(coeff_res) == LIST_E_SUCCESS);
    assert (fp_poly_free(res) == FP_POLY_E_SUCCESS);
}

static void pari_gp_tests(char *filename)
{
    fp_poly_t *res;
    char line[32786];
    FILE *file = fopen(filename, "r");
    fgets(line, sizeof(line), file);
    fp_field_t *field = fp_poly_init_prime_field(atoi(line));
    fgets(line, sizeof(line), file);
    remove_last_newline(line);
    fp_poly_t *p1 = fp_poly_parse(line);
    fgets(line, sizeof(line), file);
    remove_last_newline(line);
    fp_poly_t *p2 = fp_poly_parse(line);
    fgets(line, sizeof(line), file);
    remove_last_newline(line);
    fp_poly_t *p3 = fp_poly_parse(line);
    assert (fp_poly_add(&res, p1, p2, field) == FP_POLY_E_SUCCESS);
    assert (fp_poly_assert_equality(p3, res) == FP_POLY_E_SUCCESS);
    assert (fp_poly_free(res) == FP_POLY_E_SUCCESS);
    assert (fp_poly_free(p3) == FP_POLY_E_SUCCESS);
    assert (fp_poly_free(p2) == FP_POLY_E_SUCCESS);
    assert (fp_poly_free(p1) == FP_POLY_E_SUCCESS);
    assert (fp_poly_free_field(field) == FP_POLY_E_SUCCESS);
    fgets(line, sizeof(line), file);
    field = fp_poly_init_prime_field(atoi(line));
    fgets(line, sizeof(line), file);
    remove_last_newline(line);
    p1 = fp_poly_parse(line);
    fgets(line, sizeof(line), file);
    remove_last_newline(line);
    p2 = fp_poly_parse(line);
    fgets(line, sizeof(line), file);
    remove_last_newline(line);
    p3 = fp_poly_parse(line);
    assert (fp_poly_add(&res, p1, p2, field) == FP_POLY_E_SUCCESS);
    assert (fp_poly_assert_equality(p3, res) == FP_POLY_E_SUCCESS);
    assert (fp_poly_free(res) == FP_POLY_E_SUCCESS);
    assert (fp_poly_free(p3) == FP_POLY_E_SUCCESS);
    assert (fp_poly_free(p2) == FP_POLY_E_SUCCESS);
    assert (fp_poly_free(p1) == FP_POLY_E_SUCCESS);
    assert (fp_poly_free_field(field) == FP_POLY_E_SUCCESS);
    fclose(file);
}

int main()
{
    hello_world_tests();
    pari_gp_tests("../../../tests/fp_poly/input_test/test_add.txt");
    return 0;
}