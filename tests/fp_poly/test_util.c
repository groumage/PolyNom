#include "../../lib/include/fp_poly.h"

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

int main()
{
    fp_poly_t *p = init_and_assert((uint8_t[]) {1, 2, 0, 0, 5, 0, 0, 8, 0, 7}, 10, (uint8_t[]) {1, 2, 5, 8, 7}, 5, 659); // p = 1 + 2*x + 5*x^4 + 8*x^7 + 7*x^9
    assert (fp_poly_coeff_list_to_degree(p, 0) == 0);
    assert (fp_poly_coeff_list_to_degree(p, 1) == 1);
    assert (fp_poly_coeff_list_to_degree(p, 2) == 4);
    assert (fp_poly_coeff_list_to_degree(p, 3) == 7);
    assert (fp_poly_coeff_list_to_degree(p, 4) == 9);
    assert (fp_poly_degree_to_node_list(p, 0) == p->coeff->head);
    assert (fp_poly_degree_to_node_list(p, 0)->coeff == 1);
    assert (fp_poly_degree_to_node_list(p, 1) == p->coeff->head->next);
    assert (fp_poly_degree_to_node_list(p, 1)->coeff == 2);
    assert (fp_poly_degree_to_node_list(p, 4) == p->coeff->head->next->next);
    assert (fp_poly_degree_to_node_list(p, 4)->coeff == 5);
    assert (fp_poly_degree_to_node_list(p, 7) == p->coeff->head->next->next->next);
    assert (fp_poly_degree_to_node_list(p, 7)->coeff == 8);
    assert (fp_poly_degree_to_node_list(p, 9) == p->coeff->head->next->next->next->next);
    assert (fp_poly_degree_to_node_list(p, 9)->coeff == 7);
    assert (fp_poly_free(p) == FP_POLY_E_SUCCESS);
    
    // fp_poly_degree
    assert (fp_poly_degree(NULL) == 0);

    // fp_poly_is_unit
    assert (fp_poly_is_unit(NULL) == 0);
    return 0;
}