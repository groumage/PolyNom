#include "../../fp_poly/include/fp_poly.h"

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

    // p1 = 2 + 2*x + 2*x^3
    p1 = fp_poly_init_array((uint8_t[]) {3, 3, 0, 3}, 4);
    assert (p1 != NULL);
    coeff_p1 = list_create_from_array((uint8_t[]) {3, 3, 3}, 3);
    assert (coeff_p1 != NULL);
    assert (fp_poly_assert_sizet(p1, 11, coeff_p1) == FP_POLY_E_SUCCESS);

    // p2 = 1 + x + x^3
    p2 = fp_poly_init_array((uint8_t[]) {1, 1, 0, 1}, 4);
    assert (p2 != NULL);
    coeff_p2 = list_create_from_array((uint8_t[]) {1, 1, 1}, 3);
    assert (coeff_p2 != NULL);
    assert (fp_poly_assert_sizet(p2, 11, coeff_p2) == FP_POLY_E_SUCCESS);

    // substraction of p1 = 2 + 2*x + 2*x^3 and p2 = 1 + x + x^3 => res = 2 + 2*x + 2*x^3
    coeff_res = list_create_from_array((uint8_t[]) {2, 2, 2}, 3);
    assert (fp_poly_sub(&res, p1, p2, NULL) == FP_POLY_E_SUCCESS);
    assert (fp_poly_assert_sizet(res, 11, coeff_res) == FP_POLY_E_SUCCESS);
    assert (list_destroy(coeff_res) == LIST_E_SUCCESS);
    assert (fp_poly_free(res) == FP_POLY_E_SUCCESS);
    assert (list_destroy(coeff_p2) == LIST_E_SUCCESS);
    assert (fp_poly_free(p2) == FP_POLY_E_SUCCESS);
    assert (list_destroy(coeff_p1) == LIST_E_SUCCESS);
    assert (fp_poly_free(p1) == FP_POLY_E_SUCCESS);
}

static void pari_gp_tests(char *filename)
{
    fp_poly_t *res;
    char line[BUFFER_SIZE];
    
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
    assert (fp_poly_sub(&res, p1, p2, field) == FP_POLY_E_SUCCESS);
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
    assert (fp_poly_sub(&res, p1, p2, field) == FP_POLY_E_SUCCESS);
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
    pari_gp_tests("../../../tests/fp_poly/input_test/test_sub.txt");
    return 0;
}