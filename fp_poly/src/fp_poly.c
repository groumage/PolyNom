//
// Created by guillaume on 02/10/2021.
//

#include "../include/fp_poly.h"

static void fp_poly_error(fp_poly_error_t e, const char *file, const char *fct, const int line, const char *error) {
    switch (e)
    {
        case FP_POLY_E_MALLOC_ERROR:
            fprintf(stderr, "Error in [%s, %s] line %d: malloc error.\n", file, fct, line);
            break;
        case FP_POLY_E_POLY_IS_NULL:
            fprintf(stderr, "Error in [%s, %s] line %d: poly is NULL.\n", file, fct, line);
            break;
        case FP_POLY_E_LIST_COEFF_IS_NULL:
            fprintf(stderr, "Error in [%s, %s] line %d: list of coefficient is NULL.\n", file, fct, line);
            break;
        case FP_POLY_E_REQUESTED_DEGREE_IS_TOO_HIGH:
            fprintf(stderr, "Error in [%s, %s] line %d: requested degree is too high.\n", file, fct, line);
            break;
        case FP_POLY_E_COEFF_OVERFLOW:
            fprintf(stderr, "Error in [%s, %s] line %d: coefficient overflow.\n", file, fct, line);
            break;
        case FP_POLY_E_ASSERT_MPZ_FAILED:
            fprintf(stderr, "Error in [%s, %s] line %d: assert mpz function failed.\n", file, fct, line);
            break;
        case FP_POLY_E_ASSERT_SIZET_FAILED:
            fprintf(stderr, "Error in [%s, %s] line %d: assert size_t function failed : %s.\n", file, fct, line, error);
            break;
        case FP_POLY_E_ASSERT_EQUALITY_FAILED:
            fprintf(stderr, "Error in [%s, %s] line %d: assert equality function failed : %s.\n", file, fct, line, error);
            break;
        default:
            fprintf(stderr, "Unhandled error in [%s, %s] line %d.\n", file, fct, line);
            break;
    }
}

size_t fp_poly_degree(fp_poly_t *p)
{
    if (!p)
    {
        fp_poly_error(FP_POLY_E_POLY_IS_NULL, __FILE__, __func__, __LINE__, "");
        return 0;
    }
    return mpz_sizeinbase(p->index_coeff, 2);
}

static size_t index_of_n_th_set_bit(mpz_t number, size_t n)
{
    size_t setBitCount = 0;
    size_t bitPosition = 0;

    do {
        bitPosition = mpz_scan1(number, bitPosition);
        if (setBitCount == n)
            return bitPosition;
        setBitCount++;
        bitPosition++;
    } while (1);
}

// return the degree of the coefficient stored at the position pos in the list of coefficient of the polynom p
size_t fp_poly_coeff_list_to_degree(fp_poly_t *p, size_t pos)
{
    if (!p)
    {
        fp_poly_error(FP_POLY_E_POLY_IS_NULL, __FILE__, __func__, __LINE__, "");
        return 0;
    }
    return index_of_n_th_set_bit(p->index_coeff, pos);
}

static size_t count_bit_set_to_index(const mpz_t number, size_t degree) {
    size_t count;

    count = 0;
    // iterate through each bit position up to the specified index
    for (size_t i = 0; i < degree; i++)
        if (mpz_tstbit(number, i))
            count++;
    return count;
}


list_node_t *fp_poly_degree_to_node_list(fp_poly_t *p, size_t degree)
{
    if (!p)
    {
        fp_poly_error(FP_POLY_E_POLY_IS_NULL, __FILE__, __func__, __LINE__, "");
        return NULL;
    }
    if (degree >= fp_poly_degree(p))
    {
        fp_poly_error(FP_POLY_E_REQUESTED_DEGREE_IS_TOO_HIGH, __FILE__, __func__, __LINE__, "");
        return NULL;
    }
    return list_get_at_pos(p->coeff, count_bit_set_to_index(p->index_coeff, degree));
}

fp_poly_error_t fp_poly_add_single_term(fp_poly_t *p, uint8_t coeff, size_t degree, fp_field_t *field)
{
    list_node_t *node;

    if (!p)
    {
        fp_poly_error(FP_POLY_E_POLY_IS_NULL, __FILE__, __func__, __LINE__, "");
        return FP_POLY_E_POLY_IS_NULL;
    }
    if (!(p->coeff))
    {
        fp_poly_error(FP_POLY_E_LIST_COEFF_IS_NULL, __FILE__, __func__, __LINE__, "");
        return FP_POLY_E_LIST_COEFF_IS_NULL;
    }
    if (coeff == 0)
        return FP_POLY_E_SUCCESS;
    if (field != NULL && coeff % field->order == 0)
        return FP_POLY_E_SUCCESS;
    if (mpz_tstbit(p->index_coeff, degree))
    {
        node = fp_poly_degree_to_node_list(p, degree);
        if (field != NULL)
        {
            node->coeff = (node->coeff + coeff) % field->order;
            if (node->coeff == 0)
            {
                mpz_clrbit(p->index_coeff, degree);
                list_remove_node(p->coeff, node);
            }
        }
        else if ((uint8_t) UINT8_MAX - node->coeff >= coeff)
        {
            node->coeff += coeff;
        }
        else
        {
            fp_poly_error(FP_POLY_E_COEFF_OVERFLOW, __FILE__, __func__, __LINE__, "");
            return FP_POLY_E_COEFF_OVERFLOW;
        }
    }
    else
    {
        if (field != NULL)
            coeff = coeff % field->order;
        mpz_setbit(p->index_coeff, degree);
        list_add_at(p->coeff, coeff, count_bit_set_to_index(p->index_coeff, degree));
    }
    return FP_POLY_E_SUCCESS;
}

fp_poly_error_t fp_poly_add(fp_poly_t **res, fp_poly_t *p, fp_poly_t *q, fp_field_t * f)
{
    size_t pos;
    list_node_t *node;
    fp_poly_error_t err;

    *res = fp_poly_init();
    if (!*res)
    {
        fp_poly_error(FP_POLY_E_MALLOC_ERROR, __FILE__, __func__, __LINE__, "");
        return FP_POLY_E_MALLOC_ERROR;
    }
    pos = 0;
    node = p->coeff->head;
    while (node != NULL)
    {
        err = fp_poly_add_single_term(*res, node->coeff, fp_poly_coeff_list_to_degree(p, pos), f);
        if (err)
        {
            fp_poly_error(FP_POLY_E_MALLOC_ERROR, __FILE__, __func__, __LINE__, "");
            return err;
        }
        node = node->next;
        pos += 1;
    }
    pos = 0;
    node = q->coeff->head;
    while (node != NULL)
    {
        err = fp_poly_add_single_term(*res, node->coeff, fp_poly_coeff_list_to_degree(q, pos), f);
        if (err)
        {
            fp_poly_error(err, __FILE__, __func__, __LINE__, "");
            return err;
        }
        node = node->next;
        pos += 1;
    }
    return FP_POLY_E_SUCCESS;
}

fp_poly_t *fp_poly_parse(const char* polynomial)
{
    fp_poly_t *res;
    fp_poly_error_t err;
    const char *ptr;
    uint8_t coefficient;
    size_t degree;

    res = fp_poly_init();
    ptr = polynomial;
    if (!res)
    {
        fp_poly_error(FP_POLY_E_MALLOC_ERROR, __FILE__, __func__, __LINE__, "");
        return NULL;
    }
    if (strlen(polynomial) == 0)
        return res;
    while (*ptr != '\0')
    {
        coefficient = 0;
        degree = 0;
        while (isdigit(*ptr))
        {
            coefficient = coefficient * 10 + (*ptr - '0');
            ptr++;
        }
        if (*ptr == '*')
        {
            if (*(ptr + 2) == '^')
            {
                ptr += 3;
                while (isdigit(*ptr))
                {
                    degree = degree * 10 + (*ptr - '0');
                    ptr++;
                }
            }
            else
            {
                ptr += 2;
                degree = 1;
            }
        }
        else if (*ptr == 'x')
        {
            if (*(ptr + 1) == '^')
            {
                ptr += 2;
                while (isdigit(*ptr))
                {
                    degree = degree * 10 + (*ptr - '0');
                    ptr++;
                }
            }
            else
            {
                ptr += 1;
                degree = 1;
            }
        }
        if (coefficient == 0)
            coefficient = 1;
        err = fp_poly_add_single_term(res, coefficient, degree, NULL);
        if (err)
        {
            fp_poly_error(err, __FILE__, __func__, __LINE__, "");
            return NULL;
        }
        while (*ptr == ' ' || *ptr == '+')
            ptr++;
    }
    return res;
}

fp_poly_t *fp_poly_init(void)
{
    fp_poly_t *res;
    
    res = (fp_poly_t *) malloc(sizeof(fp_poly_t));
    if (!res)
    {
        fp_poly_error(FP_POLY_E_MALLOC_ERROR, __FILE__, __func__, __LINE__, "");
        return NULL;
    }
    mpz_init_set_ui(res->index_coeff, 0x0);
    res->coeff = list_init();
    return res;
}

fp_poly_t *fp_poly_init_sizet(size_t pos_coeff, list_t *coeff)
{
    fp_poly_t *res;
    
    res = (fp_poly_t *) malloc(sizeof(fp_poly_t));
    if (!res)
    {
        fp_poly_error(FP_POLY_E_MALLOC_ERROR, __FILE__, __func__, __LINE__, "");
        return NULL;
    }
    mpz_init_set_ui(res->index_coeff, pos_coeff);
    res->coeff = coeff;
    return res;
}

fp_poly_t *fp_poly_init_mpz(mpz_t pos_coeff, list_t *coeff)
{
    fp_poly_t *res;
    
    res = (fp_poly_t *) malloc(sizeof(fp_poly_t));
    if (!res)
    {
        fp_poly_error(FP_POLY_E_MALLOC_ERROR, __FILE__, __func__, __LINE__, "");
        return NULL;
    }
    mpz_init_set(res->index_coeff, pos_coeff);
    res->coeff = coeff;
    return res;
}

fp_poly_t *fp_poly_init_array(uint8_t *coeff, size_t len)
{
    fp_poly_t *res;
    
    res = (fp_poly_t *) malloc(sizeof(fp_poly_t));
    if (!res)
    {
        fp_poly_error(FP_POLY_E_MALLOC_ERROR, __FILE__, __func__, __LINE__, "");
        return NULL;
    }
    mpz_init_set_ui(res->index_coeff, 0x0);
    res->coeff = list_init();
    for (size_t i = 0; i < len; i++)
    {
        if (coeff[i] != 0)
        {
            mpz_setbit(res->index_coeff, i);
            list_add_end(res->coeff, coeff[i]);
            //list_add_beginning(res->coeff, coeff[i]);
        }
    }
    return res;
}

fp_poly_error_t fp_poly_free(fp_poly_t *p)
{
    if (!p)
    {
        fp_poly_error(FP_POLY_E_POLY_IS_NULL, __FILE__, __func__, __LINE__, "");
        return FP_POLY_E_POLY_IS_NULL;
    }
    if (!p->coeff)
    {
        fp_poly_error(FP_POLY_E_LIST_COEFF_IS_NULL, __FILE__, __func__, __LINE__, "");
        return FP_POLY_E_LIST_COEFF_IS_NULL;
    }
    list_destroy(p->coeff);
    mpz_clear(p->index_coeff);
    free(p);
    return FP_POLY_E_SUCCESS;
}

fp_poly_error_t fp_poly_assert_mpz(fp_poly_t *p, mpz_t expected_pos_coeff, list_t *expected_coeff)
{
    list_node_t *node_p;
    list_node_t *node_expected;
    size_t pos_p;
    size_t pos_expected;

    if (!p)
    {
        fp_poly_error(FP_POLY_E_POLY_IS_NULL, __FILE__, __func__, __LINE__, "");
        return FP_POLY_E_POLY_IS_NULL;
    }
    if (!p->coeff)
    {
        fp_poly_error(FP_POLY_E_LIST_COEFF_IS_NULL, __FILE__, __func__, __LINE__, "");
        return FP_POLY_E_LIST_COEFF_IS_NULL;
    }
    if (!expected_coeff)
    {
        fp_poly_error(FP_POLY_E_LIST_COEFF_IS_NULL, __FILE__, __func__, __LINE__, "");
        return FP_POLY_E_LIST_COEFF_IS_NULL;
    }
    if (mpz_cmp(p->index_coeff, expected_pos_coeff) != 0)
    {
        fp_poly_error(FP_POLY_E_ASSERT_MPZ_FAILED, __FILE__, __func__, __LINE__, "");
        return FP_POLY_E_ASSERT_MPZ_FAILED;
    }
    pos_p = 0;
    pos_expected = 0;
    node_p = p->coeff->head;
    node_expected = expected_coeff->head;
    while (node_p != NULL && node_expected != NULL)
    {
        if (node_p->coeff != node_expected->coeff)
        {
            fp_poly_error(FP_POLY_E_ASSERT_MPZ_FAILED, __FILE__, __func__, __LINE__, "");
            return FP_POLY_E_ASSERT_MPZ_FAILED;
        }
        pos_p += 1;
        pos_expected += 1;
        node_p = node_p->next;
        node_expected = node_expected->next;
    }
    if (node_p != NULL || node_expected != NULL)
    {
        fp_poly_error(FP_POLY_E_ASSERT_MPZ_FAILED, __FILE__, __func__, __LINE__, "");
        return FP_POLY_E_ASSERT_MPZ_FAILED;
    }
    return FP_POLY_E_SUCCESS;
}

fp_poly_error_t fp_poly_assert_sizet(fp_poly_t * p, size_t expected_pos_coeff, list_t *expected_coeff)
{
    list_node_t *node_p;
    list_node_t *node_expected;
    size_t pos_p;
    size_t pos_expected;

    if (!p)
    {
        fp_poly_error(FP_POLY_E_POLY_IS_NULL, __FILE__, __func__, __LINE__, "");
        return FP_POLY_E_POLY_IS_NULL;
    }
    if (!p->coeff)
    {
        fp_poly_error(FP_POLY_E_LIST_COEFF_IS_NULL, __FILE__, __func__, __LINE__, "");
        return FP_POLY_E_LIST_COEFF_IS_NULL;
    }
    if (!expected_coeff)
    {
        fp_poly_error(FP_POLY_E_LIST_COEFF_IS_NULL, __FILE__, __func__, __LINE__, "");
        return FP_POLY_E_LIST_COEFF_IS_NULL;
    }
    if (mpz_cmp_ui(p->index_coeff, expected_pos_coeff) != 0)
    {
        char buffer[100];
        snprintf(buffer, 100, "expected: %lu , got: %s\n", expected_pos_coeff, mpz_get_str(NULL, 10, p->index_coeff));
        fp_poly_error(FP_POLY_E_ASSERT_SIZET_FAILED, __FILE__, __func__, __LINE__, buffer);
        return FP_POLY_E_ASSERT_SIZET_FAILED;
    }
    pos_p = 0;
    pos_expected = 0;
    node_p = p->coeff->head;
    node_expected = expected_coeff->head;
    while (node_p != NULL && node_expected != NULL)
    {
        if (node_p->coeff != node_expected->coeff)
        {
            char buffer[100];
            snprintf(buffer, 100, "expected coeff : %u , got coeff: %u\n", node_expected->coeff, node_p->coeff);
            fp_poly_error(FP_POLY_E_ASSERT_SIZET_FAILED, __FILE__, __func__, __LINE__, buffer);
            return FP_POLY_E_ASSERT_SIZET_FAILED;
        }
        pos_p += 1;
        pos_expected += 1;
        node_p = node_p->next;
        node_expected = node_expected->next;
    }
    if (node_p != NULL || node_expected != NULL)
    {
        fp_poly_error(FP_POLY_E_ASSERT_SIZET_FAILED, __FILE__, __func__, __LINE__, "");
        return FP_POLY_E_ASSERT_SIZET_FAILED;
    }
    return FP_POLY_E_SUCCESS;
}

fp_poly_error_t fp_poly_assert_equality(fp_poly_t *expected_p, fp_poly_t *actual)
{
    list_node_t *expected_node;
    list_node_t *actual_node;

    if (!expected_p)
    {
        fp_poly_error(FP_POLY_E_POLY_IS_NULL, __FILE__, __func__, __LINE__, "");
        return FP_POLY_E_POLY_IS_NULL;
    }
    if (!expected_p->coeff)
    {
        fp_poly_error(FP_POLY_E_LIST_COEFF_IS_NULL, __FILE__, __func__, __LINE__, "");
        return FP_POLY_E_LIST_COEFF_IS_NULL;
    }
    if (!actual)
    {
        fp_poly_error(FP_POLY_E_POLY_IS_NULL, __FILE__, __func__, __LINE__, "");
        return FP_POLY_E_POLY_IS_NULL;
    }
    if (!actual->coeff)
    {
        fp_poly_error(FP_POLY_E_LIST_COEFF_IS_NULL, __FILE__, __func__, __LINE__, "");
        return FP_POLY_E_LIST_COEFF_IS_NULL;
    }
    if (mpz_cmp(expected_p->index_coeff, actual->index_coeff) != 0)
    {
        char buffer[1024];
        snprintf(buffer, 1024, "expected: %s , got: %s", mpz_get_str(NULL, 10, expected_p->index_coeff), mpz_get_str(NULL, 10, actual->index_coeff));
        fp_poly_error(FP_POLY_E_ASSERT_EQUALITY_FAILED, __FILE__, __func__, __LINE__, buffer);
        return FP_POLY_E_ASSERT_EQUALITY_FAILED;
    }
    expected_node = expected_p->coeff->head;
    actual_node = actual->coeff->head;
    while (expected_node != NULL && actual_node != NULL)
    {
        if (expected_node->coeff != actual_node->coeff)
        {
            char buffer[1024];
            snprintf(buffer, 1024, "expected: %u , got: %u", expected_node->coeff, actual_node->coeff);
            fp_poly_error(FP_POLY_E_ASSERT_EQUALITY_FAILED, __FILE__, __func__, __LINE__, buffer);
            return FP_POLY_E_ASSERT_EQUALITY_FAILED;
        }
        expected_node = expected_node->next;
        actual_node = actual_node->next;
    }
    return FP_POLY_E_SUCCESS;
}

fp_poly_error_t fp_poly_print(fp_poly_t *p)
{
    list_node_t *node;

    if (!p)
    {
        fp_poly_error(FP_POLY_E_POLY_IS_NULL, __FILE__, __func__, __LINE__, "");
        return FP_POLY_E_POLY_IS_NULL;
    }
    if (!p->coeff)
    {
        fp_poly_error(FP_POLY_E_LIST_COEFF_IS_NULL, __FILE__, __func__, __LINE__, "");
        return FP_POLY_E_LIST_COEFF_IS_NULL;
    }
    node = p->coeff->head;
    for (size_t i = 0; i < fp_poly_degree(p); i++)
    {
        if (mpz_tstbit(p->index_coeff, i))
        {
            if (i == 0)
                fprintf(stderr, "%u + ", node->coeff);
            else if (i == 1)
            {
                if (node->coeff == 1)
                    fprintf(stderr, "x + ");
                else
                    fprintf(stderr, "%u*x + ", node->coeff);
            }
            else
            {
                if (node->coeff == 1)
                    fprintf(stderr, "x^%ld + ", i);
                else
                    fprintf(stderr, "%u*x^%ld + ", node->coeff, i);
            }
            node = node->next;
        }
    }
    return FP_POLY_E_SUCCESS;
}

fp_field_t *fp_poly_init_prime_field(uint8_t order)
{
    return fp_poly_init_galois_field(order, NULL);
}

fp_field_t *fp_poly_init_galois_field(uint8_t order, fp_poly_t *irreducible_polynom)
{
    fp_field_t *field;

    field = (fp_field_t *)malloc(sizeof(fp_field_t));
    if (!field)
    {
        fp_poly_error(FP_POLY_E_MALLOC_ERROR, __FILE__, __func__, __LINE__, "");
        return NULL;
    }
    field->order = order;
    field->irreducible_polynom = irreducible_polynom;
    //field->modulus = (1 << order) - 1;
    return field;
}

fp_poly_error_t fp_poly_free_field(fp_field_t *field)
{
    if (!field)
    {
        fp_poly_error(FP_POLY_E_FIELD_IS_NULL, __FILE__, __func__, __LINE__, "");
        return FP_POLY_E_FIELD_IS_NULL;
    }
    if (field->irreducible_polynom)
        fp_poly_free(field->irreducible_polynom);
    free(field);
    return FP_POLY_E_SUCCESS;
}