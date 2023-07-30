#include "../../lib/include/fp_poly.h"
#include <string.h>

static void parse_test(char *poly, uint8_t *array_coeff, size_t array_coeff_size, size_t coefficients)
{
    fp_poly_t *p = fp_poly_parse(poly);
    list_t *l = list_create_from_array(array_coeff, array_coeff_size);
    assert (fp_poly_assert_sizet(p, coefficients, l) == FP_POLY_E_SUCCESS);
    assert (list_destroy(l) == LIST_E_SUCCESS);
    assert (fp_poly_free(p) == FP_POLY_E_SUCCESS);
}

int main()
{
    parse_test("2*x^5 + 5*x^2", (uint8_t[]) {5, 2}, 2, 36);
    parse_test("x^5 + x^2", (uint8_t[]) {1, 1}, 2, 36);
    parse_test("10", (uint8_t[]) {10}, 1, 1);
    parse_test("2*x^5 + x^2", (uint8_t[]) {1, 2}, 2, 36);
    parse_test("x^5 + 2*x^2 + 10", (uint8_t[]) {10, 2, 1}, 3, 37);
    parse_test("x^13", (uint8_t[]) {1}, 1, 8192);
    parse_test("2*x^13 + x^11 + 1", (uint8_t[]) {1, 1, 2}, 3, 10241);
    parse_test("x^2 + x + 2", (uint8_t[]) {2, 1, 1}, 3, 7);
    parse_test("2*x^3 + x^2 + 1", (uint8_t[]) {1, 1, 2}, 3, 13);
    return 0;
}