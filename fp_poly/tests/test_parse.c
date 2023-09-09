#include "../include/fp_poly.h"
#include <string.h>

int main()
{
    char *str_p1 = "2*x^5 + 5*x^2";
    fp_poly_t *p1;
    list_t *l1;
    char *str_p2 = "x^5 + x^2";
    fp_poly_t *p2;
    list_t *l2;
    char *str_p3 = "10";
    fp_poly_t *p3;
    list_t *l3;
    char *str_p4 = "2*x^5 + x^2";
    fp_poly_t *p4;
    list_t *l4;
    char *str_p5 = "x^5 + 2*x^2 + 10";
    fp_poly_t *p5;
    list_t *l5;
    char *str_p6 = "x^13";
    fp_poly_t *p6;
    list_t *l6;
    char *str_p7 = "2*x^13 + x^11 + 1";
    fp_poly_t *p7;
    list_t *l7;
    char *str_p8 = "x^2 + x + 2";
    fp_poly_t *p8;
    list_t *l8;
    char *str_p9 = "2*x^3 + x^2 + 1";
    fp_poly_t *p9;
    list_t *l9;

    p1 = fp_poly_parse(str_p1);
    l1 = list_create_from_array((uint8_t[]) {5, 2}, 2);
    assert (fp_poly_assert_sizet(p1, 36, l1) == FP_POLY_E_SUCCESS);
    assert (list_destroy(l1) == LIST_E_SUCCESS);
    assert (fp_poly_free(p1) == FP_POLY_E_SUCCESS);
    
    p2 = fp_poly_parse(str_p2);
    l2 = list_create_from_array((uint8_t[]) {1, 1}, 2);
    assert (fp_poly_assert_sizet(p2, 36, l2) == FP_POLY_E_SUCCESS);
    assert (list_destroy(l2) == LIST_E_SUCCESS);
    assert (fp_poly_free(p2) == FP_POLY_E_SUCCESS);
    
    p3 = fp_poly_parse(str_p3);
    l3 = list_create_from_array((uint8_t[]) {10}, 1);
    assert (fp_poly_assert_sizet(p3, 1, l3) == FP_POLY_E_SUCCESS);
    assert (list_destroy(l3) == LIST_E_SUCCESS);
    assert (fp_poly_free(p3) == FP_POLY_E_SUCCESS);

    p4 = fp_poly_parse(str_p4);
    l4 = list_create_from_array((uint8_t[]) {1, 2}, 2);
    assert (fp_poly_assert_sizet(p4, 36, l4) == FP_POLY_E_SUCCESS);
    assert (list_destroy(l4) == LIST_E_SUCCESS);
    assert (fp_poly_free(p4) == FP_POLY_E_SUCCESS);

    p5 = fp_poly_parse(str_p5);
    l5 = list_create_from_array((uint8_t[]) {10, 2, 1}, 3);
    assert (fp_poly_assert_sizet(p5, 37, l5) == FP_POLY_E_SUCCESS);
    assert (list_destroy(l5) == LIST_E_SUCCESS);
    assert (fp_poly_free(p5) == FP_POLY_E_SUCCESS);

    p6 = fp_poly_parse(str_p6);
    l6 = list_create_from_array((uint8_t[]) {1}, 1);
    assert (fp_poly_assert_sizet(p6, 8192, l6) == FP_POLY_E_SUCCESS);
    assert (list_destroy(l6) == LIST_E_SUCCESS);
    assert (fp_poly_free(p6) == FP_POLY_E_SUCCESS);

    p7 = fp_poly_parse(str_p7);
    l7 = list_create_from_array((uint8_t[]) {1, 1, 2}, 3);
    assert (fp_poly_assert_sizet(p7, 10241, l7) == FP_POLY_E_SUCCESS);
    assert (list_destroy(l7) == LIST_E_SUCCESS);
    assert (fp_poly_free(p7) == FP_POLY_E_SUCCESS);

    p8 = fp_poly_parse(str_p8);
    l8 = list_create_from_array((uint8_t[]) {2, 1, 1}, 3);
    assert (fp_poly_assert_sizet(p8, 7, l8) == FP_POLY_E_SUCCESS);
    assert (list_destroy(l8) == LIST_E_SUCCESS);
    assert (fp_poly_free(p8) == FP_POLY_E_SUCCESS);

    p9 = fp_poly_parse(str_p9);
    l9 = list_create_from_array((uint8_t[]) {1, 1, 2}, 3);
    assert (fp_poly_assert_sizet(p9, 13, l9) == FP_POLY_E_SUCCESS);
    assert (list_destroy(l9) == LIST_E_SUCCESS);
    assert (fp_poly_free(p9) == FP_POLY_E_SUCCESS);
    return 0;
}