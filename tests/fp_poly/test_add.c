#include "../../lib/include/fp_poly.h"
#include <unistd.h>

#define BUFFER_SIZE 32768

static void remove_last_newline(char* str)
{
    size_t length = strlen(str);
    if (length > 0 && str[length - 1] == '\n')
        str[length - 1] = '\0';
}

static void add_single_term_test(fp_poly_t *p, uint8_t coeff, size_t degree, fp_field_t *f, size_t expected_index_coeff, uint8_t *expected_coeff, uint8_t len_expected_coeff)
{
    assert (fp_poly_add_single_term(p, coeff, degree, f) == FP_POLY_E_SUCCESS);
    list_t *coeff_into_list = list_create_from_array(expected_coeff, len_expected_coeff);
    assert (fp_poly_assert_sizet(p, expected_index_coeff, coeff_into_list) == FP_POLY_E_SUCCESS);
    assert (list_destroy(coeff_into_list) == LIST_E_SUCCESS);
}

static void add_polynom_wrapper(fp_poly_t *p, fp_poly_t *q, fp_field_t *f, size_t expected_index_coeff, uint8_t *expected_coeff, uint8_t len_expected_coeff)
{
    fp_poly_t *res;
    assert (fp_poly_add(&res, p, q, f) == FP_POLY_E_SUCCESS);
    list_t *coeff_into_list = list_create_from_array(expected_coeff, len_expected_coeff);
    assert (fp_poly_assert_sizet(res, expected_index_coeff, coeff_into_list) == FP_POLY_E_SUCCESS);
    assert (list_destroy(coeff_into_list) == LIST_E_SUCCESS);
    fp_poly_free(res);
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

static void hello_world_tests()
{
    fp_poly_t *p1 = init_and_assert((uint8_t[]) {1, 1, 0, 1}, 4, (uint8_t[]) {1, 1, 1}, 3, 11); // p1 = 1 + x + x^3
    fp_poly_t *p2 = init_and_assert((uint8_t[]) {1, 0, 1, 1}, 4, (uint8_t[]) {1, 1, 1}, 3, 13); // p2 = 1 + x^2 + x^3
    add_single_term_test(p1, 1, 0, NULL, 11, (uint8_t[]) {2, 1, 1}, 3); // add "1" to p1: p1 = 2 + x + x^3
    add_single_term_test(p1, 1, 1, NULL, 11, (uint8_t[]) {2, 2, 1}, 3); // add "x" to p1: p1 = 2 + 2*x + x^3
    add_single_term_test(p1, 1, 2, NULL, 15, (uint8_t[]) {2, 2, 1, 1}, 4); // add "x^2" to p1: p1 = 2 + 2*x + x^2 + x^3
    add_single_term_test(p1, 5, 3, NULL, 15, (uint8_t[]) {2, 2, 1, 6}, 4); // add "5*x^3" to p1: p1 = 2 + 2*x + x^2 + 6*x^3
    add_polynom_wrapper(p1, p2, NULL, 15, (uint8_t[]) {3, 2, 2, 7}, 4); // addition of p1 = 2 + 2*x + x^2 + 6*x^3 and p2 = 1 + x^2 + x^3 => res = 3 + 3*x + 2*x^2 + 7*x^3
    assert (fp_poly_free(p2) == FP_POLY_E_SUCCESS);
    assert (fp_poly_free(p1) == FP_POLY_E_SUCCESS);
    p1 = init_and_assert((uint8_t[]) {1, 1, 1, 1, 0, 0, 0, 1, 1}, 9, (uint8_t[]) {1, 1, 1, 1, 1, 1}, 6, 399); // p1 = 1 + x + x^2 + x^3 + x^7 + x^8
    p2 = init_and_assert((uint8_t[]) {1, 1, 0, 0, 1, 0, 1, 0, 1}, 9, (uint8_t[]) {1, 1, 1, 1, 1}, 5, 339); // p2 = 1 + x + x^4 + x^6 + x^8
    add_polynom_wrapper(p1, p2, NULL, 479, (uint8_t[]) {2, 2, 1, 1, 1, 1, 1, 2}, 8); // addition of p1 = 1 + x + x^2 + x^3 + x^7 + x^8 and p2 = 1 + x + x^4 + x^6 + x^8 => res = 2 + 2*x + x^2 + x^3 + x^4 + x^6 + x^7 + 2*x^8
    assert (fp_poly_free(p2) == FP_POLY_E_SUCCESS);
    assert (fp_poly_free(p1) == FP_POLY_E_SUCCESS);
}

static void pari_gp_tests(char *filename)
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
    assert (fp_poly_add(&res, poly[0], poly[1], field) == FP_POLY_E_SUCCESS);
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
    assert (fp_poly_add(&res, poly[0], poly[1], field) == FP_POLY_E_SUCCESS);
    assert (fp_poly_assert_equality(poly[2], res) == FP_POLY_E_SUCCESS);
    assert (fp_poly_free(res) == FP_POLY_E_SUCCESS);
    for (size_t i = 0; i < 3; i++)
        assert (fp_poly_free(poly[i]) == FP_POLY_E_SUCCESS);
    assert (fp_poly_free_field(field) == FP_POLY_E_SUCCESS);
    fclose(file);
}

void test_error()
{
    fp_poly_t *p = fp_poly_init_array((uint8_t[]) {1, 1, 0, 1}, 4);
    fp_poly_t *q = fp_poly_init_array((uint8_t[]) {1, 0, 1, 1}, 4);
    fp_poly_t *res;
    list_t *mem;
    list_node_t *mem_node;
    assert (fp_poly_add(&res, NULL, q, NULL) == FP_POLY_E_POLY_IS_NULL);
    assert (fp_poly_add(&res, p, NULL, NULL) == FP_POLY_E_POLY_IS_NULL);
    mem = p->coeff;
    p->coeff = NULL;
    assert (fp_poly_add(&res, p, q, NULL) == FP_POLY_E_LIST_COEFF_IS_NULL);
    p->coeff = mem;
    mem_node = p->coeff->head;
    p->coeff->head = NULL;
    assert (fp_poly_add(&res, p, q, NULL) == FP_POLY_E_LIST_COEFF_HEAD_IS_NULL);
    p->coeff->head = mem_node;
    mem = q->coeff;
    q->coeff = NULL;
    assert (fp_poly_add(&res, p, q, NULL) == FP_POLY_E_LIST_COEFF_IS_NULL);
    q->coeff = mem;
    mem_node = q->coeff->head;
    q->coeff->head = NULL;
    assert (fp_poly_add(&res, p, q, NULL) == FP_POLY_E_LIST_COEFF_HEAD_IS_NULL);
    q->coeff->head = mem_node;
    p->coeff->head->coeff = 200;
    q->coeff->head->coeff = 200;
    assert (fp_poly_add(&res, p, q, NULL) == FP_POLY_E_COEFF_OVERFLOW);
    assert (fp_poly_free(p) == FP_POLY_E_SUCCESS);
    assert (fp_poly_free(q) == FP_POLY_E_SUCCESS);
}

int main()
{
    hello_world_tests();
    test_error();
    pari_gp_tests("../../../tests/fp_poly/input_test/test_add.txt");
    return 0;
}