#include "../../fp_poly/include/fp_poly.h"
#include <unistd.h>

#define BUFFER_SIZE 32768

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

static void mul_single_term_test(fp_poly_t *p, uint8_t coeff, size_t degree, fp_field_t *f, size_t expected_index_coeff, uint8_t *expected_coeff, uint8_t len_expected_coeff)
{
    assert (fp_poly_mul_single_term(p, coeff, degree, f) == FP_POLY_E_SUCCESS);
    list_t *coeff_into_list = list_create_from_array(expected_coeff, len_expected_coeff);
    assert (fp_poly_assert_sizet(p, expected_index_coeff, coeff_into_list) == FP_POLY_E_SUCCESS);
    assert (list_destroy(coeff_into_list) == LIST_E_SUCCESS);
}

static void hello_world_tests()
{
    fp_poly_t *p1 = init_and_assert((uint8_t[]) {1, 1, 0, 1}, 4, (uint8_t[]) {1, 1, 1}, 3, 11); // p1 = 1 + x + x^3
    //fp_poly_t *p2 = fp_poly_init_array((uint8_t[]) {1, 1, 0, 1}, 4);
    mul_single_term_test(p1, 1, 1, NULL, 22, (uint8_t[]) {1, 1, 1}, 3);
    //assert (fp_poly_free(p2) == FP_POLY_E_SUCCESS);
    assert (fp_poly_free(p1) == FP_POLY_E_SUCCESS);
}

int main()
{
    hello_world_tests();
    return 0;
}