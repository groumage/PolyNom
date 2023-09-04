#include "../../lib/include/fp_poly.h"

int main()
{
    assert (fp_poly_print(NULL, NULL) == FP_POLY_E_PRINT);
    assert (fp_poly_print(stdout, NULL) == FP_POLY_E_PRINT);
    fp_poly_t *p = fp_poly_init();
    list_t *mem = p->coeff;
    p->coeff = NULL;
    assert (fp_poly_print(stdout, p) == FP_POLY_E_PRINT);
    p->coeff = mem;
    mpz_set_ui(p->index_coeff, 0x1);
    list_add_beginning(p->coeff, 0x0);
    assert (fp_poly_print(stdout, p) == FP_POLY_E_SUCCESS);
    list_get_at_pos(p->coeff, 0)->coeff = 0x1;
    assert (fp_poly_print(stdout, p) == FP_POLY_E_SUCCESS);
    mpz_set_ui(p->index_coeff, 0x3);
    list_add_beginning(p->coeff, 0x1);
    assert (fp_poly_print(stdout, p) == FP_POLY_E_SUCCESS);
    mpz_set_ui(p->index_coeff, 0x7);
    list_add_beginning(p->coeff, 0x1);
    assert (fp_poly_print(stdout, p) == FP_POLY_E_SUCCESS);
    list_get_at_pos(p->coeff, 1)->coeff = 0x2;
    list_get_at_pos(p->coeff, 2)->coeff = 0x2;
    assert (fp_poly_print(stdout, p) == FP_POLY_E_SUCCESS);
    assert (fp_poly_free(p) == FP_POLY_E_SUCCESS);
    return 0;
}