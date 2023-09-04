#include "../../lib/include/fp_poly.h"

int main()
{
    // fp_poly_assert_mpz
    assert (fp_poly_assert_mpz(NULL, NULL, NULL) == FP_POLY_E_ASSERT_MPZ); // error: the polynom is NULL
    fp_poly_t *p = fp_poly_init_array((uint8_t[]) {1, 1}, 2);
    list_t *mem = p->coeff;
    p->coeff = NULL;
    assert (fp_poly_assert_mpz(p, NULL, NULL) == FP_POLY_E_ASSERT_MPZ); // error: the list of the values of the coefficients of the polynom is NULL
    p->coeff = mem;
    assert (fp_poly_assert_mpz(p, NULL, NULL) == FP_POLY_E_ASSERT_MPZ); // error: the expected index of the coefficient is NULL
    mpz_t m;
    assert (fp_poly_assert_mpz(p, m, NULL) == FP_POLY_E_ASSERT_MPZ); // error: the list of the expected values of the coefficients is NULL
    mpz_init_set_ui(m, 0x5);
    list_t *lst = list_init();
    assert (fp_poly_assert_mpz(p, m, lst) == FP_POLY_E_ASSERT_MPZ); // error: the actual index of the coefficient is different from the expected index of the coefficient
    mpz_set_ui(m, 0x3);
    list_add_beginning(lst, 1);
    assert (fp_poly_assert_mpz(p, m, lst) == FP_POLY_E_ASSERT_MPZ); // error: the actual list of the values of the coefficients is shorter than the expected list of the values of the coefficients
    list_add_beginning(lst, 2);
    assert (fp_poly_assert_mpz(p, m, lst) == FP_POLY_E_ASSERT_MPZ); // error: both list of the values of the coefficient has equal size but a value doesn't match
    list_get_at_pos(lst, 0)->coeff = 1;
    assert (fp_poly_assert_mpz(p, m, lst) == FP_POLY_E_SUCCESS);
    list_add_beginning(lst, 1);
    assert (fp_poly_assert_mpz(p, m, lst) == FP_POLY_E_ASSERT_MPZ); // error: the actual list of the values of the coefficients is longer than the expected list of the values of the coefficients
    mpz_clear(m);

    // fp_poly_assert_sizet
    assert (fp_poly_assert_sizet(NULL, 0x0, NULL) == FP_POLY_E_ASSERT_SIZET); // error: the polynom is NULL
    p = fp_poly_init_array((uint8_t[]) {1, 1}, 2);
    mem = p->coeff;
    p->coeff = NULL;
    assert (fp_poly_assert_sizet(p, 0x0, NULL) == FP_POLY_E_ASSERT_SIZET); // error: the list of the values of the coefficients of the polynom is NULL
    p->coeff = mem;
    assert (fp_poly_assert_sizet(p, 0x0, NULL) == FP_POLY_E_ASSERT_SIZET); // error: the list of the expected values of the coefficients is NULL
    lst = list_init();
    assert (fp_poly_assert_sizet(p, 0x5, lst) == FP_POLY_E_ASSERT_SIZET); // error: the actual index of the coefficient is different from the expected index of the coefficient
    list_add_beginning(lst, 1);
    assert (fp_poly_assert_sizet(p, 0x3, lst) == FP_POLY_E_ASSERT_SIZET); // error: the actual list of the values of the coefficients is shorter than the expected list of the values of the coefficients
    list_add_beginning(lst, 2);
    assert (fp_poly_assert_sizet(p, 0x3, lst) == FP_POLY_E_ASSERT_SIZET); // error: both list of the values of the coefficient has equal size but a value doesn't match
    list_get_at_pos(lst, 0)->coeff = 1;
    assert (fp_poly_assert_sizet(p, 0x3, lst) == FP_POLY_E_SUCCESS);
    list_add_beginning(lst, 1);
    assert (fp_poly_assert_sizet(p, 0x3, lst) == FP_POLY_E_ASSERT_SIZET); // error: the actual list of the values of the coefficients is longer than the expected list of the values of the coefficients
    assert (fp_poly_free(p) == FP_POLY_E_SUCCESS);

    // fp_poly_assert_equality
    assert (fp_poly_assert_equality(NULL, NULL) == FP_POLY_E_ASSERT_EQUALITY); // error: both polynoms are NULL
    p = fp_poly_init_array((uint8_t[]) {1, 1}, 2);
    mem = p->coeff;
    p->coeff = NULL;
    assert (fp_poly_assert_equality(p, NULL) == FP_POLY_E_ASSERT_EQUALITY); // error: the first polynom is NULL
    p->coeff = mem;
    assert (fp_poly_assert_equality(p, NULL) == FP_POLY_E_ASSERT_EQUALITY); // error: the second polynom is NULL
    fp_poly_t *q = fp_poly_init_array((uint8_t[]) {1, 1}, 2);
    mem = q->coeff;
    q->coeff = NULL;
    assert (fp_poly_assert_equality(p, q) == FP_POLY_E_ASSERT_EQUALITY); // error: the second polynom is NULL
    q->coeff = mem;
    assert (fp_poly_assert_equality(p, q) == FP_POLY_E_SUCCESS);
    return 0;
}