#include "../../lib/include/fp_poly.h"

int main()
{
    // fp_poly_init_array
    assert (fp_poly_init_array(NULL, 0) == NULL);
    assert (fp_poly_init_array((uint8_t[]) {1, 1}, 0) == NULL);
    fp_poly_t *p = fp_poly_init_array((uint8_t[]) {0}, 1);
    assert (mpz_cmp_ui(p->index_coeff, 0x1) == 0);
    assert (p->coeff->size == 1);
    assert (p->coeff->head->coeff == 0);
    assert (fp_poly_free(p) == FP_POLY_E_SUCCESS);
    p = fp_poly_init_array((uint8_t[]) {0, 1}, 2);
    assert (mpz_cmp_ui(p->index_coeff, 0x2) == 0);
    assert (p->coeff->size == 1);
    assert (p->coeff->head->coeff == 1);
    assert (fp_poly_free(p) == FP_POLY_E_SUCCESS);

    // fp_poly_init_sizet
    assert (fp_poly_init_sizet(0, NULL) == NULL);
    list_t *lst = list_init();
    list_add_beginning(lst, 1);
    list_add_beginning(lst, 1);
    assert (fp_poly_init_sizet(0, lst) == NULL);
    assert (fp_poly_init_sizet(1, lst) == NULL);
    p = fp_poly_init_sizet(3, lst);
    assert (p->coeff->size == 2);
    assert (p->coeff->head->coeff == 1);
    assert (p->coeff->head->next->coeff == 1);
    assert (fp_poly_free(p) == FP_POLY_E_SUCCESS);

    // fp_poly_init_mpz
    mpz_t m;
    mpz_init_set_ui(m, 0x0);
    assert (fp_poly_init_mpz(m, NULL) == NULL);
    lst = list_init();
    list_add_beginning(lst, 1);
    list_add_beginning(lst, 1);
    assert (fp_poly_init_mpz(m, lst) == NULL);
    mpz_set_ui(m, 0x1);
    assert (fp_poly_init_mpz(m, lst) == NULL);
    mpz_set_ui(m, 0x3);
    p = fp_poly_init_mpz(m, lst);
    assert (p->coeff->size == 2);
    assert (p->coeff->head->coeff == 1);
    assert (p->coeff->head->next->coeff == 1);
    assert (fp_poly_free(p) == FP_POLY_E_SUCCESS);
    mpz_clear(m);

    //fp_poly_init_random
    assert (fp_poly_init_random(0, NULL) == NULL);
    fp_field_t *f = fp_poly_init_prime_field(2);
    for (size_t i = 0; i < 10; i++)
    {
        p = fp_poly_init_random(i, f);
        assert (fp_poly_degree(p) == i);
        fp_poly_free(p);
    }

    //fp_poly_free
    assert (fp_poly_free(NULL) == FP_POLY_E_FREE);
    p = fp_poly_init_array((uint8_t[]) {1, 1}, 2);
    list_t *mem = p->coeff;
    p->coeff = NULL;
    assert (fp_poly_free(p) == FP_POLY_E_FREE);
    p->coeff = mem;
    assert (fp_poly_free(p) == FP_POLY_E_SUCCESS);

    //fp_poly_free_field
    assert (fp_poly_free_field(NULL) == FP_POLY_E_FREE_FIELD);

    //fp_poly_init_galois_field
    assert (fp_poly_init_galois_field(0, NULL) == NULL);
    return 0;
}