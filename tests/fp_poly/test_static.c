#include "../../lib/include/fp_poly.h"
#include "../../lib/src/fp_poly.c"

int main()
{
    // fp_poly_is_unit
    assert (fp_poly_is_unit(NULL) == 0);
    fp_poly_t *p = fp_poly_init_array((uint8_t[]) {1}, 1);
    assert (fp_poly_is_unit(p) == 1);
    list_add_beginning(p->coeff, 1);
    assert (fp_poly_is_unit(p) == 0);
    assert (fp_poly_free(p) == FP_POLY_E_SUCCESS);

    // fp_poly_is_zero
    p = fp_poly_init_array((uint8_t[]) {0}, 1);
    assert (fp_poly_is_zero(NULL) == 0);
    assert (fp_poly_is_zero(p) == 1);
    list_add_beginning(p->coeff, 1);
    assert (fp_poly_is_zero(p) == 0);
    assert (fp_poly_free(p) == FP_POLY_E_SUCCESS);

    // fp_poly_normalise_zero_polynom
    assert (fp_poly_normalise_zero_polynom(NULL) == FP_POLY_E_POLY_IS_NULL);
    p = fp_poly_init_array((uint8_t[]) {0}, 1);
    assert (fp_poly_normalise_zero_polynom(p) == FP_POLY_E_SUCCESS);
    list_t *lst = list_init();
    list_add_beginning(lst, 0);
    assert (fp_poly_assert_sizet(p, 0x1, lst) == FP_POLY_E_SUCCESS);
    list_add_beginning(p->coeff, 0);
    assert (fp_poly_normalise_zero_polynom(p) == FP_POLY_E_SUCCESS);
    assert (fp_poly_assert_sizet(p, 0x1, lst) == FP_POLY_E_SUCCESS);
    mpz_set_ui(p->index_coeff, 0x2);
    assert (fp_poly_normalise_zero_polynom(p) == FP_POLY_E_SUCCESS);
    assert (fp_poly_assert_sizet(p, 0x1, lst) == FP_POLY_E_SUCCESS);
    assert (fp_poly_free(p) == FP_POLY_E_SUCCESS);

    // fp_poly_degree
    assert (fp_poly_degree(NULL) == 0);
    p = fp_poly_init_array((uint8_t[]) {0}, 1);
    assert (fp_poly_degree(p) == 0);
    assert (fp_poly_free(p) == FP_POLY_E_SUCCESS);
    p = fp_poly_init_array((uint8_t[]) {1, 0, 0}, 3);
    assert (fp_poly_degree(p) == 0);
    assert (fp_poly_free(p) == FP_POLY_E_SUCCESS);
    p = fp_poly_init_array((uint8_t[]) {1, 0, 1}, 3);
    assert (fp_poly_degree(p) == 2);
    assert (fp_poly_free(p) == FP_POLY_E_SUCCESS);
    p = fp_poly_init_array((uint8_t[]) {1, 1, 1}, 3);
    assert (fp_poly_degree(p) == 2);
    assert (fp_poly_free(p) == FP_POLY_E_SUCCESS);

    //fp_poly_coeff_list_to_degree
    assert (fp_poly_coeff_list_to_degree(NULL, 0) == 0);

    // fp_poly_degree_to_node
    assert (fp_poly_degree_to_node_list(NULL, 0) == NULL);
    p = fp_poly_init_array((uint8_t[]) {1, 0, 1}, 3);
    assert (fp_poly_degree_to_node_list(p, 3) == NULL);
    assert (fp_poly_free(p) == FP_POLY_E_SUCCESS);

    //fp_poly_inv
    fp_field_t *field = fp_poly_init_prime_field(4);
    assert (fp_poly_inv(2, field) == 0);
    assert (fp_poly_free_field(field) == FP_POLY_E_SUCCESS);

    // fp_poly_parse
    assert (fp_poly_parse(NULL) == NULL);
    assert (fp_poly_parse("") == NULL);

    // fp_poly_count_set_bits
    assert (fp_poly_count_set_bits(0) == 0);
    assert (fp_poly_count_set_bits(1) == 1);
    assert (fp_poly_count_set_bits(2) == 1);
    assert (fp_poly_count_set_bits(3) == 2);
    return 0;
}