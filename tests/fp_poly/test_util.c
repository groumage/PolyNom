#include "../../fp_poly/include/fp_poly.h"

int main()
{
    fp_poly_t *p1;
    list_t *coeff_p1;

    // p1 = 1 + x + x^3
    p1 = fp_poly_init_array((uint8_t[]) {1, 1, 0, 1}, 4);
    assert (p1 != NULL);
    coeff_p1 = list_create_from_array((uint8_t[]) {1, 1, 1}, 3);
    assert (coeff_p1 != NULL);
    assert (fp_poly_assert_sizet(p1, 11, coeff_p1) == FP_POLY_E_SUCCESS);
    assert (list_destroy(coeff_p1) == LIST_E_SUCCESS);
    assert (fp_poly_free(p1) == FP_POLY_E_SUCCESS);

    // p1 = 1 + 2*x + 5*x^4 + 8x^7 + 7x^9
    p1 = fp_poly_init_array((uint8_t[]) {1, 2, 0, 0, 5, 0, 0, 8, 0, 7}, 10);
    assert (p1 != NULL);
    coeff_p1 = list_create_from_array((uint8_t[]) {1, 2, 5, 8, 7}, 5);
    assert (coeff_p1 != NULL);
    assert (fp_poly_assert_sizet(p1, 659, coeff_p1) == FP_POLY_E_SUCCESS);

    assert (fp_poly_coeff_list_to_degree(p1, 0) == 0);
    assert (fp_poly_coeff_list_to_degree(p1, 1) == 1);
    assert (fp_poly_coeff_list_to_degree(p1, 2) == 4);
    assert (fp_poly_coeff_list_to_degree(p1, 3) == 7);
    assert (fp_poly_coeff_list_to_degree(p1, 4) == 9);
    assert (fp_poly_degree_to_node_list(p1, 0) == p1->coeff->head);
    assert (fp_poly_degree_to_node_list(p1, 0)->coeff == 1);
    assert (fp_poly_degree_to_node_list(p1, 1) == p1->coeff->head->next);
    assert (fp_poly_degree_to_node_list(p1, 1)->coeff == 2);
    assert (fp_poly_degree_to_node_list(p1, 4) == p1->coeff->head->next->next);
    assert (fp_poly_degree_to_node_list(p1, 4)->coeff == 5);
    assert (fp_poly_degree_to_node_list(p1, 7) == p1->coeff->head->next->next->next);
    assert (fp_poly_degree_to_node_list(p1, 7)->coeff == 8);
    assert (fp_poly_degree_to_node_list(p1, 9) == p1->coeff->head->next->next->next->next);
    assert (fp_poly_degree_to_node_list(p1, 9)->coeff == 7);
    
    assert (list_destroy(coeff_p1) == LIST_E_SUCCESS);
    assert (fp_poly_free(p1) == FP_POLY_E_SUCCESS);
    return 0;
}