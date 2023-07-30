#include "../../lib/include/fp_poly.h"
#include <unistd.h>

#define BUFFER_SIZE 32768

static void remove_last_newline(char* str)
{
    size_t length = strlen(str);
    if (length > 0 && str[length - 1] == '\n')
        str[length - 1] = '\0';
}

static fp_poly_t *init_and_assert(uint8_t* coeff, size_t len_coeff, uint8_t* list_coeff, size_t len_list_coeff, size_t expected_index_coeff)
{
    fp_poly_t *p = fp_poly_init_array(coeff, len_coeff);
    assert (p != NULL);
    list_t *coeff_p = list_create_from_array(list_coeff, len_list_coeff);
    assert (coeff_p != NULL);
    assert (fp_poly_assert_sizet(p, expected_index_coeff, coeff_p) == FP_POLY_E_SUCCESS);
    assert (list_destroy(coeff_p) == LIST_E_SUCCESS);
    return p;
}

/*
static void mul_single_term_test(fp_poly_t *p, uint8_t coeff, size_t degree, fp_field_t *f, size_t expected_index_coeff, uint8_t *expected_coeff, uint8_t len_expected_coeff)
{
    assert (fp_poly_mul_single_term(p, coeff, degree, f) == FP_POLY_E_SUCCESS);
    list_t *coeff_into_list = list_create_from_array(expected_coeff, len_expected_coeff);
    assert (fp_poly_assert_sizet(p, expected_index_coeff, coeff_into_list) == FP_POLY_E_SUCCESS);
    assert (list_destroy(coeff_into_list) == LIST_E_SUCCESS);
}
*/

static void hello_world_tests()
{
    fp_poly_t *p1 = init_and_assert((uint8_t[]) {0, 0, 0, 0, 0, 0, 0, 12}, 8, (uint8_t[]) {12}, 1, 128);
    fp_poly_t *p2 = init_and_assert((uint8_t[]) {11, 9, 12}, 3, (uint8_t[]) {11, 9, 12}, 3, 7);
    fp_field_t *f = fp_poly_init_prime_field(13);
    fp_poly_t *res;
    fp_poly_mul(&res, p1, p2, f);
    list_t *coeff_res = list_create_from_array((uint8_t[]) {2, 4, 1}, 3);
    assert (coeff_res != NULL);
    assert (fp_poly_assert_sizet(res, 896, coeff_res) == FP_POLY_E_SUCCESS);
    assert (list_destroy(coeff_res) == LIST_E_SUCCESS);
    assert (fp_poly_free(p2) == FP_POLY_E_SUCCESS);
    assert (fp_poly_free(p1) == FP_POLY_E_SUCCESS);
    assert (fp_poly_free(res) == FP_POLY_E_SUCCESS);
    assert (fp_poly_free_field(f) == FP_POLY_E_SUCCESS);
}

static void pari_gp_test(char *filename)
{
    fp_poly_t *res;
    fp_poly_t *poly[3];
    char line[BUFFER_SIZE];
    FILE *file = fopen(filename, "r");

    fgets(line, sizeof(line), file);
    fp_field_t *field = fp_poly_init_prime_field(atoi(line));
    for (size_t i = 0; i < 3; i++)
    {
        fgets(line, sizeof(line), file);
        remove_last_newline(line);
        poly[i] = fp_poly_parse(line);
    }
    assert (fp_poly_mul(&res, poly[0], poly[1], field) == FP_POLY_E_SUCCESS);
    assert (fp_poly_assert_equality(poly[2], res) == FP_POLY_E_SUCCESS);
    assert (fp_poly_free(res) == FP_POLY_E_SUCCESS);
    
    for (size_t i = 0; i < 3; i++)
        assert (fp_poly_free(poly[i]) == FP_POLY_E_SUCCESS);
    assert (fp_poly_free_field(field) == FP_POLY_E_SUCCESS);
    fgets(line, sizeof(line), file);
    field = fp_poly_init_prime_field(atoi(line));
    for (size_t i = 0; i < 3; i++)
    {
        fgets(line, sizeof(line), file);
        remove_last_newline(line);
        poly[i] = fp_poly_parse(line);
    }
    assert (fp_poly_mul(&res, poly[0], poly[1], field) == FP_POLY_E_SUCCESS);
    assert (fp_poly_assert_equality(poly[2], res) == FP_POLY_E_SUCCESS);
    assert (fp_poly_free(res) == FP_POLY_E_SUCCESS);
    for (size_t i = 0; i < 3; i++)
        assert (fp_poly_free(poly[i]) == FP_POLY_E_SUCCESS);
    assert (fp_poly_free_field(field) == FP_POLY_E_SUCCESS);
    
    fclose(file);
}

int main()
{
    hello_world_tests();
    pari_gp_test("../../../tests/fp_poly/input_test/test_mul.txt");
    return 0;
}