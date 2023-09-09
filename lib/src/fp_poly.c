//
// Created by guillaume on 02/10/2021.
//

#include "../include/util.h"
#include "../include/fp_integer.h"
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
        case FP_POLY_E_LIST_COEFF_HEAD_IS_NULL:
            fprintf(stderr, "Error in [%s, %s] line %d: list of coefficient head is NULL.\n", file, fct, line);
            break;
        case FP_POLY_E_REQUESTED_DEGREE_IS_TOO_HIGH:
            fprintf(stderr, "Error in [%s, %s] line %d: requested degree is too high.\n", file, fct, line);
            break;
        case FP_POLY_E_COEFF_OVERFLOW:
            fprintf(stderr, "Error in [%s, %s] line %d: coefficient overflow.\n", file, fct, line);
            break;
        case FP_POLY_E_ASSERT_MPZ:
            fprintf(stderr, "Error in [%s, %s] line %d: function fp_poly_assert_mpz failed : %s.\n", file, fct, line, error);
            break;
        case FP_POLY_E_ASSERT_SIZET:
            fprintf(stderr, "Error in [%s, %s] line %d: assert size_t function failed : %s.\n", file, fct, line, error);
            break;
        case FP_POLY_E_ASSERT_EQUALITY:
            fprintf(stderr, "Error in [%s, %s] line %d: assert equality function failed : %s.\n", file, fct, line, error);
            break;
        case FP_POLY_E_INIT_SIZET:
            fprintf(stderr, "Error in [%s, %s] line %d: fp_poly_init size_t function failed: %s\n", file, fct, line, error);
            break;
        case FP_POLY_INIT_ARRAY:
            fprintf(stderr, "Error in [%s, %s] line %d: fp_poly_init_array function failed: %s\n", file, fct, line, error);
            break;
        case FP_POLY_E_INIT_MPZ:
            fprintf(stderr, "Error in [%s, %s] line %d: fp_poly_init_mpz function failed: %s\n", file, fct, line, error);
            break;
        case FP_POLY_E_FREE:
            fprintf(stderr, "Error in [%s, %s] line %d: fp_poly_free function failed: %s\n", file, fct, line, error);
            break;
        case FP_POLY_E_FREE_FIELD:
            fprintf(stderr, "Error in [%s, %s] line %d: fp_poly_free_field function failed: %s\n", file, fct, line, error);
            break;
        default:
            fprintf(stderr, "Unhandled error in [%s, %s] line %d.\n", file, fct, line);
            break;
    }
}

static void fp_poly_error_no_custom_msg(fp_poly_error_t e, const char *file, const char *fct, const int line)
{
    fp_poly_error(e, file, fct, line, "");
}

static uint8_t fp_poly_is_zero(fp_poly_t *p)
{
    if (!p)
    {
        fp_poly_error_no_custom_msg(FP_POLY_E_POLY_IS_NULL, __FILE__, __func__, __LINE__);
        return 0;
    }
    list_node_t *node = p->coeff->head;
    while (node != NULL)
    {
        if (node->coeff != 0)
            return 0;
        node = node->next;
    }
    return 1;
}

static fp_poly_error_t fp_poly_normalise_zero_polynom(fp_poly_t *p)
{
    if (!p)
    {
        fp_poly_error_no_custom_msg(FP_POLY_E_POLY_IS_NULL, __FILE__, __func__, __LINE__);
        return FP_POLY_E_POLY_IS_NULL;
    }
    if (fp_poly_is_zero(p))
    {
        mpz_set_ui(p->index_coeff, 1);
        list_destroy(p->coeff);
        p->coeff = list_init();
        if (!p->coeff)
        {
            fp_poly_error(FP_POLY_E_MALLOC_ERROR, __FILE__, __func__, __LINE__, "");
            return FP_POLY_E_MALLOC_ERROR;
        }
        list_add_beginning(p->coeff, 0);
        /*
        fp_poly_free(p);
        p = fp_poly_init_array((uint8_t[]) {0}, 1);
        */
    }
    return FP_POLY_E_SUCCESS;
}

static uint8_t fp_poly_is_unit(fp_poly_t *p)
{
    if (!p)
    {
        fp_poly_error_no_custom_msg(FP_POLY_E_POLY_IS_NULL, __FILE__, __func__, __LINE__);
        return 0;
    }
    if (p->coeff->size == 1 && mpz_cmp_ui(p->index_coeff, 1) == 0 && p->coeff->head->coeff == 1)
        return 1;
    return 0;
}

/*
* Return the degree of the polynom p.
*
* Parameters:
* - p: the polynom.
*
* Returns:
* - the degree of the polynom p.
*/
size_t fp_poly_degree(fp_poly_t *p)
{
    if (!p)
    {
        fp_poly_error(FP_POLY_E_POLY_IS_NULL, __FILE__, __func__, __LINE__, "");
        return 0;
    }
    /*
    if (mpz_cmp_ui(p->index_coeff, 0) == 0 || mpz_cmp_ui(p->index_coeff, 1) == 0)
        return 0;
    else
    */
    return mpz_sizeinbase(p->index_coeff, 2) - 1;
}

/*
* Return the index of the n-th set bit in the number.
*
* Parameters:
* - number: the number.
* - n: the index of the set bit.
*
* Returns:
* - the index of the n-th set bit in the number.
*/
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

/*
* Return the degree of the coefficient stored at the position pos in the list of coefficient of the polynom p.
*
* Parameters:
* - p: the polynom.
* - pos: the position of the coefficient in the list of coefficient.
*
* Returns:
* - the degree of the coefficient stored at the position pos in the list of coefficient of the polynom p.
*/
size_t fp_poly_coeff_list_to_degree(fp_poly_t *p, size_t pos)
{
    if (!p)
    {
        fp_poly_error(FP_POLY_E_POLY_IS_NULL, __FILE__, __func__, __LINE__, "");
        return 0;
    }
    return index_of_n_th_set_bit(p->index_coeff, pos);
}

/*
* Count the number of bit sets to 1 that are less than the index degree.
*
* Parameters:
* - number: the number to count the bit sets to 1.
* - degree: the index until which bits are count.
*
* Returns:
* - the number of bit sets to 1 that are less than the index degree.
*/
static size_t count_bit_set_to_index(const mpz_t number, size_t degree) {
    size_t count;

    count = 0;
    
    for (size_t i = 0; i < degree; i++)
        if (mpz_tstbit(number, i))
            count++;
    return count;
}

/*
 * Return a node containing the coefficient at the specified degree.
 *
 * Parameters:
 * - p: the polynom.
 * - degree: the degree associated to the coefficient we want.
 *
 * Returns:
 * - NULL if an error occurred.
 * - a node if the operation was successful.
 * - FP_POLY_E_POLY_IS_NULL if the polynom p is NULL.
 * - FP_POLY_E_REQUESTED_DEGREE_IS_TOO_HIGH if the requested degree is too high.
 *
 */
list_node_t *fp_poly_degree_to_node_list(fp_poly_t *p, size_t degree)
{
    if (!p)
    {
        fp_poly_error(FP_POLY_E_POLY_IS_NULL, __FILE__, __func__, __LINE__, "");
        return NULL;
    }
    if (degree > fp_poly_degree(p))
    {
        fp_poly_error(FP_POLY_E_REQUESTED_DEGREE_IS_TOO_HIGH, __FILE__, __func__, __LINE__, "");
        return NULL;
    }
    return list_get_at_pos(p->coeff, count_bit_set_to_index(p->index_coeff, degree));
}

static fp_poly_error_t fp_poly_add_single_term_aux(fp_poly_t *p, uint8_t coeff, size_t degree, fp_field_t *field, uint8_t is_addition)
{
    fp_poly_error_t err;
    if (coeff == 0)
        return FP_POLY_E_SUCCESS;
    if (field != NULL && coeff % field->order == 0)
        return FP_POLY_E_SUCCESS;
    if (mpz_tstbit(p->index_coeff, degree))
    {
        list_node_t *node = fp_poly_degree_to_node_list(p, degree);
        if (field == NULL)
        {
            if (is_addition)
            {
                if (node-> coeff > 0 && coeff > UINT8_MAX - node->coeff)
                {
                    fp_poly_error(FP_POLY_E_COEFF_OVERFLOW, __FILE__, __func__, __LINE__, "");
                    return FP_POLY_E_COEFF_OVERFLOW;
                }
                node->coeff += coeff;
            }
            else
            {
                if (node->coeff < coeff)
                {
                    fp_poly_error(FP_POLY_E_COEFF_OVERFLOW, __FILE__, __func__, __LINE__, "");
                    return FP_POLY_E_COEFF_UNDERFLOW;
                }
                node->coeff -= coeff;
                if (node->coeff == 0)
                {
                    mpz_clrbit(p->index_coeff, degree);
                    list_remove_node(p->coeff, node);
                }
            }
        }
        else
        {
            if (is_addition)
                node->coeff = (node->coeff + coeff) % field->order;
            else
            {
                if (node->coeff < coeff)
                    node->coeff = (field->order + node->coeff - coeff);
                else
                    node->coeff = (node->coeff - coeff) % field->order;
            }
            if (node->coeff == 0)
            {
                mpz_clrbit(p->index_coeff, degree);
                list_remove_node(p->coeff, node);
            }
        }
    }
    else
    {
        if (field != NULL)
        {
            if (is_addition)
                coeff = coeff % field->order;
            else
                coeff = (field->order - coeff) % field->order;
        }
        mpz_setbit(p->index_coeff, degree);
        list_add_at(p->coeff, coeff, count_bit_set_to_index(p->index_coeff, degree));
    }
    err = fp_poly_normalise_zero_polynom(p);
    if (err != FP_POLY_E_SUCCESS)
    {
        fp_poly_error(err, __FILE__, __func__, __LINE__, "");
        return err;
    }
    return FP_POLY_E_SUCCESS;
}

/*
 * Add a single term at a specified degree to the polynom within a field.
 *
 * Parameters:
 * - p: the polynom.
 * - coeff: the value of the term to add.
 * - degree: the degree of the term to add.
 * - field: the field in which the operation is performed (optionnal).
 *
 * Returns:
 * - FP_POLY_E_SUCCESS if the operation was successful.
 * - FP_POLY_E_POLY_IS_NULL if the polynom p is NULL.
 * - FP_POLY_E_LIST_COEFF_IS_NULL if the list of coefficient of the polynom p is NULL.
 * - FP_POLY_E_COEFF_OVERFLOW if there is no field provided and if the term is too high to be stored within an uint8_t.
 *
 */
fp_poly_error_t fp_poly_add_single_term(fp_poly_t *p, uint8_t coeff, size_t degree, fp_field_t *field)
{
    return fp_poly_add_single_term_aux(p, coeff, degree, field, 1);
}

/*
* Documentation todo.
*
* Parameters:
*
* Pre-condition:
* - The following variable are not NULL: *res, (2) p, (3) q, (4) list of coeff of p, (5) the head of list of coeff of p, (6) list of coeff of q and (7) the head of list of coeff of q.
*
* Returns:
*/
static fp_poly_error_t fp_poly_add_aux(fp_poly_t *res, fp_poly_t *p, fp_poly_t *q, fp_field_t * field, uint8_t is_addition)
{
    size_t pos;
    list_node_t *node;
    fp_poly_error_t err;

    pos = 0;
    node = p->coeff->head;
    while (node != NULL)
    {
        err = fp_poly_add_single_term_aux(res, node->coeff, fp_poly_coeff_list_to_degree(p, pos), field, 1);
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
        err = fp_poly_add_single_term_aux(res, node->coeff, fp_poly_coeff_list_to_degree(q, pos), field, is_addition);
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

/*
 * Add two polynom together within a field.
 *
 * Parameters:
 * - res: the polynom which store the result of the addition.
 * - p: the first polynom.
 * - q: the second polynom.
 * - field: the field in which the operation is performed (optionnal).
 *
 * Returns:
 * - FP_POLY_E_SUCCESS if the operation was successful.
 * - FP_POLY_E_MALLOC_ERROR if there was an error during the memory allocation.
 * - any error return by fp_poly_add_single_term.
 *
 */
fp_poly_error_t fp_poly_add(fp_poly_t **res, fp_poly_t *p, fp_poly_t *q, fp_field_t *f)
{
    *res = fp_poly_init();
    if (!*res)
    {
        fp_poly_error(FP_POLY_E_MALLOC_ERROR, __FILE__, __func__, __LINE__, "");
        return FP_POLY_E_MALLOC_ERROR;
    }
    if (!p)
    {
        fp_poly_error(FP_POLY_E_POLY_IS_NULL, __FILE__, __func__, __LINE__, "");
        return FP_POLY_E_POLY_IS_NULL;
    }
    if (!q)
    {
        fp_poly_error(FP_POLY_E_POLY_IS_NULL, __FILE__, __func__, __LINE__, "");
        return FP_POLY_E_POLY_IS_NULL;
    }
    if (!(p->coeff))
    {
        fp_poly_error(FP_POLY_E_LIST_COEFF_IS_NULL, __FILE__, __func__, __LINE__, "");
        return FP_POLY_E_LIST_COEFF_IS_NULL;
    }
    if (!(p->coeff->head))
    {
        fp_poly_error(FP_POLY_E_LIST_COEFF_HEAD_IS_NULL, __FILE__, __func__, __LINE__, "");
        return FP_POLY_E_LIST_COEFF_HEAD_IS_NULL;
    }
    if (!(q->coeff))
    {
        fp_poly_error(FP_POLY_E_LIST_COEFF_IS_NULL, __FILE__, __func__, __LINE__, "");
        return FP_POLY_E_LIST_COEFF_IS_NULL;
    }
    if (!(q->coeff->head))
    {
        fp_poly_error(FP_POLY_E_LIST_COEFF_HEAD_IS_NULL, __FILE__, __func__, __LINE__, "");
        return FP_POLY_E_LIST_COEFF_HEAD_IS_NULL;
    }
    return fp_poly_add_aux(*res, p, q, f, 1);
}

/*
fp_poly_error_t fp_poly_sub_single_term(fp_poly_t *p, uint8_t coeff, size_t degree, fp_field_t *field)
{
    return fp_poly_add_single_term_aux(p, coeff, degree, field, 0);
}
*/

fp_poly_error_t fp_poly_sub(fp_poly_t **res, fp_poly_t *p, fp_poly_t *q, fp_field_t *f)
{
    *res = fp_poly_init();
    if (!*res)
    {
        fp_poly_error(FP_POLY_E_MALLOC_ERROR, __FILE__, __func__, __LINE__, "");
        return FP_POLY_E_MALLOC_ERROR;
    }
    if (!p)
    {
        fp_poly_error(FP_POLY_E_POLY_IS_NULL, __FILE__, __func__, __LINE__, "");
        return FP_POLY_E_POLY_IS_NULL;
    }
    if (!q)
    {
        fp_poly_error(FP_POLY_E_POLY_IS_NULL, __FILE__, __func__, __LINE__, "");
        return FP_POLY_E_POLY_IS_NULL;
    }
    if (!(p->coeff))
    {
        fp_poly_error(FP_POLY_E_LIST_COEFF_IS_NULL, __FILE__, __func__, __LINE__, "");
        return FP_POLY_E_LIST_COEFF_IS_NULL;
    }
    if (!(p->coeff->head))
    {
        fp_poly_error(FP_POLY_E_LIST_COEFF_HEAD_IS_NULL, __FILE__, __func__, __LINE__, "");
        return FP_POLY_E_LIST_COEFF_HEAD_IS_NULL;
    }
    if (!(q->coeff))
    {
        fp_poly_error(FP_POLY_E_LIST_COEFF_IS_NULL, __FILE__, __func__, __LINE__, "");
        return FP_POLY_E_LIST_COEFF_IS_NULL;
    }
    if (!(q->coeff->head))
    {
        fp_poly_error(FP_POLY_E_LIST_COEFF_HEAD_IS_NULL, __FILE__, __func__, __LINE__, "");
        return FP_POLY_E_LIST_COEFF_HEAD_IS_NULL;
    }
    return fp_poly_add_aux(*res, p, q, f, 0);
}

/*
fp_poly_error_t fp_poly_mul_single_term(fp_poly_t *p, uint8_t coeff, size_t degree, fp_field_t *field)
{
    size_t pos;
    list_node_t *node;
    fp_poly_error_t err = 0;

    pos = 0;
    node = p->coeff->head;
    mpz_mul_2exp(p->index_coeff, p->index_coeff, degree);
    while (node != NULL)
    {
        node->coeff = node->coeff * coeff;
        if (err)
        {
            fp_poly_error(err, __FILE__, __func__, __LINE__, "");
            return err;
        }
        if (node->coeff == 0)
        {
            mpz_clrbit(p->index_coeff, fp_poly_coeff_list_to_degree(p, pos));
            list_node_t *tmp = node;
            list_remove_node(p->coeff, tmp);
        }
        node = node->next;
        pos += 1;
    }
    return FP_POLY_E_SUCCESS;
}
*/

fp_poly_error_t fp_poly_mul(fp_poly_t **res, fp_poly_t *p, fp_poly_t *q, fp_field_t *f)
{
    size_t pos_q, pos_p;
    list_node_t *node_q, *node_p;
    fp_poly_error_t err = 0;

    *res = fp_poly_init();
    if (!*res)
    {
        fp_poly_error(FP_POLY_E_MALLOC_ERROR, __FILE__, __func__, __LINE__, "");
        return FP_POLY_E_MALLOC_ERROR;
    }
    pos_p = 0;
    node_p = p->coeff->head;
    while (node_p != NULL)
    {
        pos_q = 0;
        node_q = q->coeff->head;
        while (node_q != NULL)
        {
            err = fp_poly_add_single_term_aux(*res, node_p->coeff * node_q->coeff, fp_poly_coeff_list_to_degree(p, pos_p) + fp_poly_coeff_list_to_degree(q, pos_q), f, 1);
            if (err)
            {
                fp_poly_error(err, __FILE__, __func__, __LINE__, "");
                return err;
            }
            node_q = node_q->next;
            pos_q += 1;
        }
        node_p = node_p->next;
        pos_p += 1;
    }
    return FP_POLY_E_SUCCESS;
}

fp_poly_error_t fp_poly_mul_fq(fp_poly_t **res, fp_poly_t *p, fp_poly_t *q, fp_field_t *f)
{
    fp_poly_t *tmp_res, *tmp_q, *tmp_r;
    fp_poly_error_t err;
    if ((err = fp_poly_mul(&tmp_res, p, q, f)) != FP_POLY_E_SUCCESS)
    {
        fp_poly_error(err, __FILE__, __func__, __LINE__, "");
        return err;
    }
    if ((err = fp_poly_div(&tmp_q, &tmp_r, tmp_res, f->irreducible_polynom, f)) != FP_POLY_E_SUCCESS)
    {
        fp_poly_error(err, __FILE__, __func__, __LINE__, "");
        return FP_POLY_E_SUCCESS;
    }
    fp_poly_free(tmp_res);
    fp_poly_free(tmp_q);
    *res = tmp_r;
    return FP_POLY_E_SUCCESS;
}

uint8_t fp_poly_inv(uint8_t element, fp_field_t *field) {
    for (uint8_t i = 1; i < field->order; i++) {
        if ((element * i) % field->order == 1) {
            return i;
        }
    }
    return 0; // If no inverse is found
}

fp_poly_error_t fp_poly_div(fp_poly_t **q, fp_poly_t **r, fp_poly_t *n, fp_poly_t *d, fp_field_t *f)
{
    fp_poly_t *t, *intermediate, *mem;
    fp_poly_error_t err;
    mpz_t bitwise;
    *r = fp_poly_init_mpz(n->index_coeff, list_copy(n->coeff));
    *q = fp_poly_init();
    mpz_set_ui((*q)->index_coeff, 0x1);
    list_add_beginning((*q)->coeff, 0);
    t = fp_poly_init();
    list_add_beginning(t->coeff, 0);
    mpz_init(bitwise);
    while (fp_poly_is_zero(*r) == 0 && fp_poly_degree(*r) >= fp_poly_degree(d))
    {
        /*
        if (fp_poly_is_zero(*r))
        {
            fp_poly_free(*r);
            break;
        }
        */
        mpz_set_ui(bitwise, 0);
        mpz_setbit(bitwise, fp_poly_degree(*r) - fp_poly_degree(d));
        mpz_set(t->index_coeff, bitwise);
        uint16_t tmp = (uint16_t) fp_poly_inv(d->coeff->tail->coeff, f) * (uint16_t) (*r)->coeff->tail->coeff;
        tmp = tmp % f->order;
        t->coeff->tail->coeff = (uint8_t) tmp;
        mem = *q;
        if ((err = fp_poly_add(q, *q, t, f) != FP_POLY_E_SUCCESS))
        {
            fp_poly_error(err, __FILE__, __func__, __LINE__, "");
            return err;
        }
        fp_poly_free(mem);
        if ((err = fp_poly_mul(&intermediate, t, d, f)) != FP_POLY_E_SUCCESS)
        {
            fp_poly_error(err, __FILE__, __func__, __LINE__, "");
            return err;
        }
        mem = *r;
        if ((err = fp_poly_sub(r, *r, intermediate, f)) != FP_POLY_E_SUCCESS)
        {
            fp_poly_error(err, __FILE__, __func__, __LINE__, "");
            return err;
        }
        fp_poly_free(mem);
        fp_poly_free(intermediate);
        //fp_poly_normalise_zero_polynom(*r);
    }
    fp_poly_free(t);
    mpz_clear(bitwise);
    return FP_POLY_E_SUCCESS;
}

fp_poly_error_t fp_poly_gcd(fp_poly_t **res, fp_poly_t *p, fp_poly_t *q, fp_field_t *f)
{
    fp_poly_t *q_tmp, *r_tmp, *r1, *r2, *mem;
    fp_poly_error_t err;
    r1 = fp_poly_init_mpz(p->index_coeff, list_copy(p->coeff));
    r2 = fp_poly_init_mpz(q->index_coeff, list_copy(q->coeff));
    while (fp_poly_is_zero(r2) == 0)
    {
        /*
        fprintf(stderr, "toto1\n");
        fprintf(stderr, "r1 = ");
        fp_poly_print(stderr, r1);
        fprintf(stderr, "\n");
        fprintf(stderr, "r2 = ");
        fp_poly_print(stderr, r2);
        fprintf(stderr, "\n");
        */
        err = fp_poly_div(&q_tmp, &r_tmp, r1, r2, f);
        //fprintf(stderr, "toto2\n\n");
        fp_poly_free(q_tmp);
        mem = r1;
        r1 = r2;
        r2 = r_tmp;
        fp_poly_free(mem);
        if (err)
        {
            fp_poly_error(err, __FILE__, __func__, __LINE__, "");
            return err;
        }
        //fp_poly_normalise_zero_polynom(r2);
    }
    *res = r1;
    fp_poly_free(r2);
    return FP_POLY_E_SUCCESS;
}

/*
fp_poly_error_t fp_poly_gcd_extended(fp_poly_t **res, fp_poly_t **u, fp_poly_t **v, fp_poly_t *p, fp_poly_t *q, fp_field_t *f)
{
    fp_poly_t *q_tmp, *r_tmp, *old_r, *r, *old_s, *s, *old_t, *t, *tmp, *tmp_mul;
    fp_poly_error_t err;
    old_r = fp_poly_init_mpz(p->index_coeff, list_copy(p->coeff));
    r = fp_poly_init_mpz(q->index_coeff, list_copy(q->coeff));
    old_s = fp_poly_init();
    s = fp_poly_init();
    old_t = fp_poly_init();
    t = fp_poly_init();
    mpz_set_ui(old_s->index_coeff, 0x1);
    list_add_beginning(old_s->coeff, 0x1);
    mpz_set_ui(s->index_coeff, 0x0);
    mpz_set_ui(old_t->index_coeff, 0x0);
    mpz_set_ui(t->index_coeff, 0x1);
    list_add_beginning(t->coeff, 0x1);
    while (fp_poly_is_zero(r) == 0)
    {
        
        fp_poly_print(stderr, r);
        fprintf(stderr, "     degre = %ld      is_zero?%u\n", fp_poly_degree(r), fp_poly_is_zero(r));
        
        fp_poly_div(&q_tmp, &r_tmp, old_r, r, f);

        tmp = r;
        fp_poly_mul(&tmp_mul, q_tmp, r, f);
        fp_poly_sub(&r, old_r, tmp_mul, f);
        //fp_poly_free(old_r);
        old_r = tmp;
        fprintf(stderr, "r = ");
        fp_poly_print(stderr, r);
        fprintf(stderr, "\n");
        fprintf(stderr, "old_r = ");
        fp_poly_print(stderr, old_r);
        fprintf(stderr, "\n");
        //fp_poly_free(tmp_mul);
        
        //aux(old_r, r, q_tmp);
        
        tmp = s;
        fp_poly_mul(&tmp_mul, q_tmp, s, f);
        fprintf(stderr, "SUB: ");
        fp_poly_print(stderr, old_s);
        fprintf(stderr, " - ");
        fp_poly_print(stderr, tmp_mul);
        fprintf(stderr, "\n");
        fp_poly_sub(&s, old_s, tmp_mul, f);
        //fp_poly_free(old_s);
        old_s = tmp;
        //fprintf(stderr, "s = ");
        //fp_poly_print(stderr, s);
        //fprintf(stderr, "\n");
        //fprintf(stderr, "old_s = ");
        //fp_poly_print(stderr, old_s);
        //fprintf(stderr, "\n");

        //fp_poly_free(tmp_mul);
        
        //aux(old_s, s, q_tmp);
        
        tmp = t;
        fp_poly_mul(&tmp_mul, q_tmp, t, f);
        fp_poly_sub(&t, old_t, tmp_mul, f);
        //fp_poly_free(old_t);
        old_t = tmp;
        fprintf(stderr, "t = ");
        fp_poly_print(stderr, t);
        fprintf(stderr, "\n");
        fprintf(stderr, "old_t = ");
        fp_poly_print(stderr, old_t);
        fprintf(stderr, "\n");
        fprintf(stderr, "\n");
        //fp_poly_free(tmp_mul);
        
        //aux(old_t, t, q_tmp);
        
        //fp_poly_free(q_tmp);
        //fp_poly_free(r_tmp);
    }
    *res = old_r;
    *u = old_s;
    *v = old_t;
    fp_poly_free(r);
    fp_poly_free(s);
    fp_poly_free(t);
    return FP_POLY_E_SUCCESS;
}
*/

/*
* Parse a string to create a polynom.
*
* Parameters:
* - polynomial: the string to parse.
*
* Returns:
* - a pointer to the polynom if the operation was successful.
* - NULL if there was an error during the memory allocation.
*/
fp_poly_t *fp_poly_parse(const char* polynomial)
{
    fp_poly_t *res;
    fp_poly_error_t err;
    const char *ptr;
    uint8_t coefficient;
    size_t degree;

    if (!polynomial)
    {
        fp_poly_error(FP_POLY_E_POLY_IS_NULL, __FILE__, __func__, __LINE__, "");
        return NULL;
    }
    if (strlen(polynomial) == 0)
    {
        fp_poly_error(FP_POLY_E_STR_OF_ZERO_LENGTH, __FILE__, __func__, __LINE__, "");
        return NULL;
    }
    res = fp_poly_init();
    ptr = polynomial;
    if (!res)
    {
        fp_poly_error(FP_POLY_E_MALLOC_ERROR, __FILE__, __func__, __LINE__, "");
        return NULL;
    }
    if (strlen(polynomial) == 1 && polynomial[0] == '0')
    {
        mpz_set_ui(res->index_coeff, 0x1);
        list_add_beginning(res->coeff, 0);
        return res;
    }
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
        err = fp_poly_add_single_term_aux(res, coefficient, degree, NULL, 1);
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

/*
* Initialize a polynom where index of the coefficient is zero.
*
* Returns:
* - a pointer to the polynom if the operation was successful.
* - NULL if there was an error during the memory allocation.
*/
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

static size_t fp_poly_count_set_bits(size_t n)
{
    size_t count = 0;
    while (n)
    {
        count += n & 1;
        n >>= 1;
    }
    return count;
}

/*
static size_t fp_poly_count_set_bits_mpz(mpz_t n)
{
    size_t count = 0;
    for (size_t i = 0; i < mpz_sizeinbase(n, 2); i++)
        count += mpz_tstbit(n, i);
    return count;
}
*/

/*
* Initialize a polynom with a specified index coefficient and list of coefficients. The index coefficient is a size_t.
*
* Parameters:
* - pos_coeff: the index of the coefficient.
* - coeff: the list of coefficients.
*
* Returns:
* - a pointer to the polynom if the operation was successful.
* - NULL if there was an error during the memory allocation.
*/
fp_poly_t *fp_poly_init_sizet(size_t pos_coeff, list_t *coeff)
{
    if (!coeff)
    {
        fp_poly_error(FP_POLY_E_INIT_SIZET, __FILE__, __func__, __LINE__, "list of coefficients is NULL");
        return NULL;
    }
    if (pos_coeff == 0)
    {
        fp_poly_error(FP_POLY_E_INIT_SIZET, __FILE__, __func__, __LINE__, "index of the coefficients is zero");
        return NULL;
    }
    fp_poly_t *res = (fp_poly_t *) malloc(sizeof(fp_poly_t));
    if (!res)
    {
        fp_poly_error(FP_POLY_E_MALLOC_ERROR, __FILE__, __func__, __LINE__, "");
        return NULL;
    }
    if (fp_poly_count_set_bits(pos_coeff) != coeff->size)
    {
        fp_poly_error(FP_POLY_E_INIT_SIZET, __FILE__, __func__, __LINE__, "the number of coefficients in the list and the index of the coefficients are not consistent");
        return NULL;
    }
    mpz_init_set_ui(res->index_coeff, pos_coeff);
    res->coeff = coeff;
    return res;
}

/*
* Initialize a polynom with a specified index coefficient and list of coefficients. The index coefficient is a mpz_t.
*
* Parameters:
* - pos_coeff: the index of the coefficient.
* - coeff: the list of coefficients.
*
* Returns:
* - a pointer to the polynom if the operation was successful.
* - NULL if there was an error during the memory allocation.
*/
fp_poly_t *fp_poly_init_mpz(mpz_t pos_coeff, list_t *coeff)
{
    if (!coeff)
    {
        fp_poly_error(FP_POLY_E_INIT_MPZ, __FILE__, __func__, __LINE__, "list of coefficients is NULL");
        return NULL;
    }
    if (mpz_cmp_ui(pos_coeff, 0) == 0)
    {
        fp_poly_error(FP_POLY_E_INIT_MPZ, __FILE__, __func__, __LINE__, "index of the coefficients is zero");
        return NULL;
    }
    fp_poly_t *res = (fp_poly_t *) malloc(sizeof(fp_poly_t));
    if (!res)
    {
        fp_poly_error(FP_POLY_E_MALLOC_ERROR, __FILE__, __func__, __LINE__, "");
        return NULL;
    }
    if (fp_poly_count_set_bits(mpz_get_ui(pos_coeff)) != coeff->size)
    {
        char buffer[150];
        snprintf(buffer, 150, "the number of coefficients in the list and the index of the coefficients are not consistent: number of coefficients is %ld but index of coeff is %ld", coeff->size, fp_poly_count_set_bits(mpz_get_ui(pos_coeff)));
        fp_poly_error(FP_POLY_E_INIT_MPZ, __FILE__, __func__, __LINE__, buffer);
        return NULL;
    }
    mpz_init_set(res->index_coeff, pos_coeff);
    res->coeff = coeff;
    return res;
}

/*
* Initialize a polynom with a n array of coefficients. The coefficient in the array are given from the LOWEST to the HIGHEST degree.
*
* Parameters:
* - coeff: the array of coefficients.
* - len: the length of the array.
*
* Returns:
* - a pointer to the polynom if the operation was successful.
* - NULL if there was an error during the memory allocation.
*/
fp_poly_t *fp_poly_init_array(uint8_t *coeff, size_t len)
{
    if (!coeff)
    {
        fp_poly_error(FP_POLY_INIT_ARRAY, __FILE__, __func__, __LINE__, "the array of coefficients is NULL");
        return NULL;
    }
    if (len == 0)
    {
        fp_poly_error(FP_POLY_INIT_ARRAY, __FILE__, __func__, __LINE__, "the len of the array is zero");
        return NULL;
    }
    fp_poly_t *res = (fp_poly_t *) malloc(sizeof(fp_poly_t));
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
        }
    }
    return res;
}

static fp_poly_t *fp_poly_is_irreducible_aux(uint64_t n, fp_field_t *f)
{
    fp_poly_t *x_n_minux_x;
    mpz_t deg_x_n_minux_x;
    list_t *list;
    mpz_init_set_ui(deg_x_n_minux_x, 0x0);
    //fprintf(stderr, "n = %ld\n", n);
    mpz_setbit(deg_x_n_minux_x, n-1);
    mpz_setbit(deg_x_n_minux_x, 0);
    list = list_init();
    list_add_beginning(list, 1);
    list_add_beginning(list, f->order - 1);
    x_n_minux_x = fp_poly_init_mpz(deg_x_n_minux_x, list);
    return x_n_minux_x;
}

uint8_t fp_poly_is_irreducible(fp_poly_t *p, fp_field_t *f)
{
    fp_poly_t *x_n_minus_x, *q, *r, *res_gcd;
    x_n_minus_x = fp_poly_is_irreducible_aux(my_pow(f->order, fp_poly_degree(p)), f);
    fp_poly_div(&q, &r, x_n_minus_x, p, f);
    if (!fp_poly_is_zero(r))
        return 0;
    for (size_t i = 0; i < fp_poly_degree(p); i++)
    {
        if (is_prime(i, 15))
        {
            if (fp_poly_degree(p) % i == 0)
            {
                x_n_minus_x = fp_poly_is_irreducible_aux(i, f);
                fp_poly_gcd(&res_gcd, x_n_minus_x, p, f);
                if (!fp_poly_is_unit(res_gcd))
                    return 0;
            }
        }
    }
    return 1;
}

fp_poly_t *fp_poly_init_random(size_t degree, fp_field_t *f)
{
    if (!f)
    {
        fp_poly_error(FP_POLY_E_INIT_RANDOM, __FILE__, __func__, __LINE__, "field is NULL");
        return NULL;
    }
    fp_poly_t *res = (fp_poly_t *) malloc(sizeof(fp_poly_t));
    if (!res)
    {
        fp_poly_error(FP_POLY_E_MALLOC_ERROR, __FILE__, __func__, __LINE__, "");
        return NULL;
    }
    mpz_init_set_ui(res->index_coeff, 0x0);
    res->coeff = list_init();
    unsigned char buffer[8];
    do
    {
        read_urandom_full(buffer, 8);
    } while (buffer_to_ulong(buffer, 8) % f->order == 0);
    mpz_setbit(res->index_coeff, degree);
    list_add_end(res->coeff, buffer_to_ulong(buffer, 8) % f->order);
    for (size_t i = 0; i < degree; i++)
    {
        unsigned char buffer[8];
        read_urandom_full(buffer, 8);
        if (buffer_to_ulong(buffer, 8) % f->order != 0)
        {
            mpz_setbit(res->index_coeff, i);
            list_add_end(res->coeff, buffer_to_ulong(buffer, 8) % f->order);
        }
    }
    return res;
}

fp_poly_t *fp_poly_init_random_irreducible(size_t digits, fp_field_t *f)
{
    /*
    fp_poly_t *res;
    res = fp_poly_init_random(degree, f);
    while (!fp_poly_is_irreducible(res, f))
    {
        fp_poly_free(res);
        res = fp_poly_init_random(degree, f);
    }
    return res;
    */
    fp_poly_t *res;
    mpz_t rand;
    list_t *list;
    list = list_init();
    mpz_init(rand);
    random_prime_mpz(rand, digits);
    for (size_t i = 0; i < mpz_sizeinbase(rand, 2); i++)
        if (mpz_tstbit(rand, i))
            list_add_beginning(list, 1);
    res = fp_poly_init_mpz(rand, list);
    return res;
}

/*
* Free a polynom.
*
* Parameters:
* - p: the polynom to free.
*
* Returns:
* - FP_POLY_E_SUCCESS if the operation was successful.
* - FP_POLY_E_POLY_IS_NULL if the polynom is NULL.
* - FP_POLY_E_LIST_COEFF_IS_NULL if the list of coefficients is NULL.
*/
fp_poly_error_t fp_poly_free(fp_poly_t *p)
{
    if (!p)
    {
        fp_poly_error(FP_POLY_E_FREE, __FILE__, __func__, __LINE__, "poly is NULL");
        return FP_POLY_E_FREE;
    }
    if (!p->coeff)
    {
        fp_poly_error(FP_POLY_E_FREE, __FILE__, __func__, __LINE__, "list of coefficients is NULL");
        return FP_POLY_E_FREE;
    }
    list_destroy(p->coeff);
    mpz_clear(p->index_coeff);
    free(p);
    return FP_POLY_E_SUCCESS;
}

/*
* Assert that a the parameters of a polynom is equal to the expected parameters. The expected index of the coefficient is a mpz_t.
*
* Parameters:
* - p: the polynom to check.
* - expected_pos_coeff: the expected index of the coefficient.
* - expected_coeff: the expected list of coefficients.
*
* Returns:
* - FP_POLY_E_SUCCESS if the operation was successful.
* - FP_POLY_E_POLY_IS_NULL if the polynom is NULL.
* - FP_POLY_E_LIST_COEFF_IS_NULL if the list of coefficients is NULL.
* - FP_POLY_E_ASSERT_MPZ_FAILED if the assertion failed.
*/
fp_poly_error_t fp_poly_assert_mpz(fp_poly_t *p, mpz_t expected_pos_coeff, list_t *expected_coeff)
{
    list_node_t *node_p;
    list_node_t *node_expected;
    size_t pos_p;
    size_t pos_expected;

    if (!p)
    {
        fp_poly_error(FP_POLY_E_ASSERT_MPZ, __FILE__, __func__, __LINE__, "poly is NULL");
        return FP_POLY_E_ASSERT_MPZ;
    }
    if (!p->coeff)
    {
        fp_poly_error(FP_POLY_E_ASSERT_MPZ, __FILE__, __func__, __LINE__, "list of coefficients is NULL");
        return FP_POLY_E_ASSERT_MPZ;
    }
    if (!expected_pos_coeff)
    {
        fp_poly_error(FP_POLY_E_ASSERT_MPZ, __FILE__, __func__, __LINE__, "expected index of coefficients is NULL");
        return FP_POLY_E_ASSERT_MPZ;
    }
    if (!expected_coeff)
    {
        fp_poly_error(FP_POLY_E_ASSERT_MPZ, __FILE__, __func__, __LINE__, "expected list of coefficients is NULL");
        return FP_POLY_E_ASSERT_MPZ;
    }
    if (mpz_cmp(p->index_coeff, expected_pos_coeff) != 0)
    {
        char buffer[150];
        snprintf(buffer, 150, "expected index of coefficients : %s , got index of coefficients: %s\n", mpz_get_str(NULL, 10, p->index_coeff), mpz_get_str(NULL, 10, expected_pos_coeff));
        fp_poly_error(FP_POLY_E_ASSERT_MPZ, __FILE__, __func__, __LINE__, buffer);
        return FP_POLY_E_ASSERT_MPZ;
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

/*
* Assert that a the parameters of a polynom is equal to the expected parameters. The expected index of the coefficient is a size_t.
*
* Parameters:
* - p: the polynom to check.
* - expected_pos_coeff: the expected index of the coefficient.
* - expected_coeff: the expected list of coefficients.
*
* Returns:
* - FP_POLY_E_SUCCESS if the operation was successful.
* - FP_POLY_E_POLY_IS_NULL if the polynom is NULL.
* - FP_POLY_E_LIST_COEFF_IS_NULL if the list of coefficients is NULL.
* - FP_POLY_E_ASSERT_SIZET_FAILED if the assertion failed.
*/
fp_poly_error_t fp_poly_assert_sizet(fp_poly_t *p, size_t expected_pos_coeff, list_t *expected_coeff)
{
    list_node_t *node_p;
    list_node_t *node_expected;
    size_t pos_p;
    size_t pos_expected;

    if (!p)
    {
        fp_poly_error(FP_POLY_E_ASSERT_SIZET, __FILE__, __func__, __LINE__, "");
        return FP_POLY_E_ASSERT_SIZET;
    }
    if (!p->coeff)
    {
        fp_poly_error(FP_POLY_E_ASSERT_SIZET, __FILE__, __func__, __LINE__, "");
        return FP_POLY_E_ASSERT_SIZET;
    }
    if (!expected_coeff)
    {
        fp_poly_error(FP_POLY_E_ASSERT_SIZET, __FILE__, __func__, __LINE__, "");
        return FP_POLY_E_ASSERT_SIZET;
    }
    if (mpz_cmp_ui(p->index_coeff, expected_pos_coeff) != 0)
    {
        char buffer[150];
        snprintf(buffer, 150, "expected index of coefficients : %s , got index of coefficients: %ld\n", mpz_get_str(NULL, 10, p->index_coeff), expected_pos_coeff);
        fp_poly_error(FP_POLY_E_ASSERT_SIZET, __FILE__, __func__, __LINE__, buffer);
        return FP_POLY_E_ASSERT_SIZET;
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
            snprintf(buffer, 100, "expected coeff : %u , got coeff: %u (pos = %ld)\n", node_expected->coeff, node_p->coeff, pos_p);
            fp_poly_error(FP_POLY_E_ASSERT_SIZET, __FILE__, __func__, __LINE__, buffer);
            return FP_POLY_E_ASSERT_SIZET;
        }
        pos_p += 1;
        pos_expected += 1;
        node_p = node_p->next;
        node_expected = node_expected->next;
    }
    if (node_p != NULL)
    {
        fp_poly_error(FP_POLY_E_ASSERT_SIZET, __FILE__, __func__, __LINE__, "There is more coefficients in the polynom than expected");
        return FP_POLY_E_ASSERT_SIZET;
    }
    if (node_expected != NULL)
    {
        fp_poly_error(FP_POLY_E_ASSERT_SIZET, __FILE__, __func__, __LINE__, "There is less coefficients in the polynom than expected");
        return FP_POLY_E_ASSERT_SIZET;
    }
    return FP_POLY_E_SUCCESS;
}

/*
* Assert that a two polynoms are equal.
*
* Parameters:
* - expected_p: the expected polynom.
* - actual: the actual polynom.
*
* Returns:
* - FP_POLY_E_SUCCESS if the operation was successful.
* - FP_POLY_E_POLY_IS_NULL if one of the polynom is NULL.
* - FP_POLY_E_LIST_COEFF_IS_NULL if one of the list of coefficients is NULL.
* - FP_POLY_E_ASSERT_EQUALITY_FAILED if the assertion failed.
*/
fp_poly_error_t fp_poly_assert_equality(fp_poly_t *expected_p, fp_poly_t *actual)
{
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
        snprintf(buffer, 1024, "wrong index_coeff: expected: %s , actual: %s", mpz_get_str(NULL, 10, expected_p->index_coeff), mpz_get_str(NULL, 10, actual->index_coeff));
        fp_poly_error(FP_POLY_E_ASSERT_EQUALITY_FAILED, __FILE__, __func__, __LINE__, buffer);
        return FP_POLY_E_ASSERT_EQUALITY_FAILED;
    }
    list_node_t *expected_node = expected_p->coeff->head;
    list_node_t *actual_node = actual->coeff->head;
    size_t pos = 0;
    while (expected_node != NULL && actual_node != NULL)
    {
        if (expected_node->coeff != actual_node->coeff)
        {
            char buffer[1024];
            snprintf(buffer, 1024, "expected: %u , got: %u (pos = %ld)", expected_node->coeff, actual_node->coeff, pos);
            fp_poly_error(FP_POLY_E_ASSERT_EQUALITY_FAILED, __FILE__, __func__, __LINE__, buffer);
            return FP_POLY_E_ASSERT_EQUALITY_FAILED;
        }
        expected_node = expected_node->next;
        actual_node = actual_node->next;
        pos += 1;
    }
    return FP_POLY_E_SUCCESS;
}

/*
* Print a polynom.
*
* Parameters:
* - p: the polynom to print.
*
* Returns:
* - FP_POLY_E_SUCCESS if the operation was successful.
* - FP_POLY_E_POLY_IS_NULL if the polynom is NULL.
* - FP_POLY_E_LIST_COEFF_IS_NULL if the list of coefficients is NULL.
*/
fp_poly_error_t fp_poly_print(FILE *fd, fp_poly_t *p)
{
    list_node_t *node;

    if (!fd)
    {
        fp_poly_error(FP_POLY_E_PRINT, __FILE__, __func__, __LINE__, "file descriptor is NULL");
        return FP_POLY_E_PRINT;
    }
    if (!p)
    {
        fp_poly_error(FP_POLY_E_PRINT, __FILE__, __func__, __LINE__, "");
        return FP_POLY_E_PRINT;
    }
    if (!p->coeff)
    {
        fp_poly_error(FP_POLY_E_PRINT, __FILE__, __func__, __LINE__, "");
        return FP_POLY_E_PRINT;
    }
    node = p->coeff->head;
    if (fp_poly_is_zero(p))
    {
        fprintf(fd, "0");
        return FP_POLY_E_SUCCESS;
    }
    for (size_t i = 0; i <= fp_poly_degree(p); i++)
    {
        if (mpz_tstbit(p->index_coeff, i))
        {
            if (i == 0)
                fprintf(fd, "%u", node->coeff);
            else if (i == 1)
            {
                if (node->coeff == 1)
                    fprintf(fd, "x");
                else
                    fprintf(fd, "%u*x", node->coeff);
            }
            else
            {
                if (node->coeff == 1)
                    fprintf(fd, "x^%ld", i);
                else
                    fprintf(fd, "%u*x^%ld", node->coeff, i);
            }
            if (node->next != NULL)
                fprintf(fd, " + ");
            node = node->next;
        }
    }
    return FP_POLY_E_SUCCESS;
}

/*
* Initialize a prime field.
*
* Parameters:
* - order: the order of the field.
*
* Returns:
* - a pointer to the field if the operation was successful.
* - NULL if there was an error during the memory allocation.
*/
fp_field_t *fp_poly_init_prime_field(uint8_t order)
{
    return fp_poly_init_galois_field(order, NULL);
}

/*
* Initialize a Galois field.
*
* Parameters:
* - order: the order of the field.
* - irreducible_polynom: the irreducible polynom of the field.
*
* Returns:
* - a pointer to the field if the operation was successful.
* - NULL if there was an error during the memory allocation.
*/
fp_field_t *fp_poly_init_galois_field(uint8_t order, fp_poly_t *irreducible_polynom)
{
    if (order == 0)
    {
        fp_poly_error(FP_POLY_E_INIT_GALOIS_FIELD, __FILE__, __func__, __LINE__, "order is zero");
        return NULL;
    }
    fp_field_t *field = (fp_field_t *)malloc(sizeof(fp_field_t));
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

/*
* Free a field.
*
* Parameters:
* - field: the field to free.
*
* Returns:
* - FP_POLY_E_SUCCESS if the operation was successful.
* - FP_POLY_E_FIELD_IS_NULL if the field is NULL.
*/
fp_poly_error_t fp_poly_free_field(fp_field_t *field)
{
    if (!field)
    {
        fp_poly_error(FP_POLY_E_FREE_FIELD, __FILE__, __func__, __LINE__, "");
        return FP_POLY_E_FREE_FIELD;
    }
    if (field->irreducible_polynom)
        fp_poly_free(field->irreducible_polynom);
    free(field);
    return FP_POLY_E_SUCCESS;
}