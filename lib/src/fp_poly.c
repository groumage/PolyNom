#include "../include/util.h"
#include "../include/fp_integer.h"
#include "../include/fp_poly.h"

/**
 * @brief Print a basic error message with addtionnal information to stderr.
 * 
 * @details The list of errors is:
 * - FP_POLY_E_MEMORY: a memory error, e.g., a pointer return by malloc() is NULL.
 * - FP_POLY_E_POLYNOM_IS_NULL: a polynom given as parameter to a function is NULL.
 * - FP_POLY_E_LIST_COEFFICIENT_IS_NULL: the list of coefficients of a polynom is NULL.
 * - FP_POLY_E_POLYNOM_MANIPULATION: an error occured during the manipulation of a polynom (usually, an additional message to explain this error is provided).
 * - FP_POLY_E_COEFFICIENT_ARITHMETIC: an error occured during the manipulation of coefficients (usually, a additional message to explain this error is provided).
 * 
 * @param e The error.
 * @param file The file where the error occured.
 * @param fct The function where the error occured.
 * @param line The line where the error occured.
 * @param error The additional information occured.
 */
static void fp_poly_error(fp_poly_error_t e, const char *file, const char *fct, const int line, const char *error) {
    switch (e)
    {
        case FP_POLY_E_MEMORY:
            fprintf(stderr, "Error in [%s, %s] line %d: memory error.\n", file, fct, line);
            break;
        case FP_POLY_E_POLYNOM_IS_NULL:
            fprintf(stderr, "Error in [%s, %s] line %d: the polynom is NULL.\n", file, fct, line);
            break;
        case FP_POLY_E_LIST_COEFFICIENT_IS_NULL:
            fprintf(stderr, "Error in [%s, %s] line %d: the list of coefficient is NULL.\n", file, fct, line);
            break;
        case FP_POLY_E_LIST_COEFFICIENT:
            fprintf(stderr, "Error in [%s, %s] line %d: coefficient list: %s.\n", file, fct, line, error);
            break;
        case FP_POLY_E_POLYNOM_MANIPULATION:
            fprintf(stderr, "Error in [%s, %s] line %d: polynom manipulation: %s.\n", file, fct, line, error);
            break;
        case FP_POLY_E_COEFFICIENT_ARITHMETIC:
            fprintf(stderr, "Error in [%s, %s] line %d: coefficients manipulation: %s.\n", file, fct, line, error);
            break;
        default:
            fprintf(stderr, "Unhandled error in [%s, %s] line %d.\n", file, fct, line);
            break;
    }
}

/**
 * @brief Print a basic error message to stderr
 * 
 * @details The list of errors is the same as in fp_poly_error().
 * 
 * @param e The error.
 * @param file The file where the error occured.
 * @param fct The function where the error occured.
 * @param line The line where the error occured.
*/
static void fp_poly_error_no_custom_msg(fp_poly_error_t e, const char *file, const char *fct, const int line)
{
    fp_poly_error(e, file, fct, line, "");
}

/**
 * @brief Check if a polynom is equal to zero.
 * 
 * @param p The polynom.
 * 
 * @return A boolean value: 1 if the polynom is zero, 0 otherwise.
 */
static uint8_t fp_poly_is_zero(fp_poly_t *p)
{
    if (!p)
    {
        fp_poly_error_no_custom_msg(FP_POLY_E_POLYNOM_IS_NULL, __FILE__, __func__, __LINE__);
        return 0;
    }
    if (!p->coeff)
    {
        fp_poly_error_no_custom_msg(FP_POLY_E_LIST_COEFFICIENT_IS_NULL, __FILE__, __func__, __LINE__);
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

/**
 * @brief Normalize the representation of a zero polynom.
 * 
 * @details A zero polynom can be represented in various ways, e.g., it can have one or many coefficients that are zero. <br>
 * This function standardizes the representation of a zero polynom by ensuring that the polynom has only one coefficient equal to zero. <br>
 * The old coefficient list is destroyed and a new one is created.
 *
 * @param p The polynom.
 * 
 * @return FP_POLY_E_SUCCESS if the operation was successful or an error code otherwise (see @ref fp_poly_error_t for the list of error codes).
 */
static fp_poly_error_t fp_poly_normalise_zero_polynom(fp_poly_t *p)
{
    if (!p)
    {
        fp_poly_error_no_custom_msg(FP_POLY_E_POLYNOM_IS_NULL, __FILE__, __func__, __LINE__);
        return FP_POLY_E_POLYNOM_IS_NULL;
    }
    if (fp_poly_is_zero(p))
    {
        mpz_set_ui(p->index_coeff, 1);
        if (list_destroy(p->coeff) != LIST_E_SUCCESS)
        {
            fp_poly_error(FP_POLY_E_LIST_COEFFICIENT, __FILE__, __func__, __LINE__, "list_destroy() failed");
            return FP_POLY_E_LIST_COEFFICIENT;
        }
        if ((p->coeff = list_init()) == NULL)
        {
            fp_poly_error_no_custom_msg(FP_POLY_E_MEMORY, __FILE__, __func__, __LINE__);
            return FP_POLY_E_MEMORY;
        }
        if (list_add_beginning(p->coeff, 0) != LIST_E_SUCCESS)
        {
            fp_poly_error(FP_POLY_E_LIST_COEFFICIENT, __FILE__, __func__, __LINE__, "list_add_beginning() failed");
            return FP_POLY_E_LIST_COEFFICIENT;
        }
    }
    return FP_POLY_E_SUCCESS;
}

/**
 * @brief Check if a polynom is a equal to one.
 * 
 * @param p The polynom.
 * 
 * @return A boolean value: 1 if the polynom is one, 0 otherwise.
 */
static uint8_t fp_poly_is_unit(fp_poly_t *p)
{
    if (!p)
    {
        fp_poly_error_no_custom_msg(FP_POLY_E_POLYNOM_IS_NULL, __FILE__, __func__, __LINE__);
        return 0;
    }
    if (!p->coeff)
    {
        fp_poly_error_no_custom_msg(FP_POLY_E_LIST_COEFFICIENT_IS_NULL, __FILE__, __func__, __LINE__);
        return 0;
    }
    if (p->coeff->size == 1 && mpz_cmp_ui(p->index_coeff, 1) == 0 && p->coeff->head->coeff == 1)
        return 1;
    return 0;
}

/**
 * @brief Compute the degree of a polynom.
 *
 * @param p The polynom.
 *
 * @note If 0 is returned, it means either that the polynom is NULL or the polynom is a constant polynom, e.g., P(X) = 5.
 *
 * @returns The degree of the polynom or 0 if the polynom is NULL or constant.
 */
size_t fp_poly_degree(fp_poly_t *p)
{
    if (!p)
    {
        fp_poly_error_no_custom_msg(FP_POLY_E_POLYNOM_IS_NULL, __FILE__, __func__, __LINE__);
        return 0;
    }
    return mpz_sizeinbase(p->index_coeff, 2) - 1;
}

/**
 * @brief Return the position of the n-th set bit in the binary representation of a number.
 *
 * @param number The number.
 * @param n The n-th set bit to find.
 *
 * @return The position of the n-th set bit in the binary representation of the number.
 */
static size_t index_of_n_th_set_bit(mpz_t number, size_t n)
{
    size_t set_bit_count = 0, bit_position = 0;
    do {
        bit_position = mpz_scan1(number, bit_position);
        if (set_bit_count == n)
            return bit_position;
        set_bit_count++;
        bit_position++;
    } while (1);
}

/**
 * @brief Return the degree of a coefficient located at a specified position in the coefficient list of a polynom.
 *
 * @param p The polynom.
 * @param pos The position.
 *
 * @note This is the invert operation of fp_poly_degree_to_node_list().
 *
 * @return The degree of the coefficient at the position \p pos in the coefficient list of the polynom. 
 */
size_t fp_poly_coeff_list_to_degree(fp_poly_t *p, size_t pos)
{
    if (!p)
    {
        fp_poly_error_no_custom_msg(FP_POLY_E_POLYNOM_IS_NULL, __FILE__, __func__, __LINE__);
        return 0;
    }
    return index_of_n_th_set_bit(p->index_coeff, pos);
}

/**
 * @brief Count the number of set bits of a number up to a specific bit number.
 *
 * @param number The number.
 * @param index The bit number.
 *
 * @return The number of set bits up to the specified index.
 */
static size_t count_bit_set_to_index(const mpz_t number, size_t index)
{
    size_t count = 0;
    for (size_t i = 0; i < index; i++)
        if (mpz_tstbit(number, i))
            count++;
    return count;
}

/**
 * @brief Retrieves the node containing the coefficient of the specified degree.
 *
 * @param p The polynom.
 * @param degree The degree.
 *
 * @note This is the invert operation of fp_poly_coeff_list_to_degree().
 *
 * @return The node containing the coefficient at the specified degree or NULL if an error occurred.
 */
list_node_t *fp_poly_degree_to_node_list(fp_poly_t *p, size_t degree)
{
    if (!p)
    {
        fp_poly_error_no_custom_msg(FP_POLY_E_POLYNOM_IS_NULL, __FILE__, __func__, __LINE__);
        return NULL;
    }
    if (degree > fp_poly_degree(p))
    {
        fp_poly_error(FP_POLY_E_POLYNOM_MANIPULATION, __FILE__, __func__, __LINE__, "the requested degree is too high");
        return NULL;
    }
    return list_get_at_pos(p->coeff, count_bit_set_to_index(p->index_coeff, degree));
}

/**
 * @brief An auxiliary function to add or substract a single term at a specified degree.
 * 
 * @param p The polynom.
 * @param coeff The value of the term to add or substract.
 * @param degree The degree of the term to add or substract.
 * @param field The field in which the operation is performed (may be NULL).
 * @param is_addition A boolean value: 1 if the operation is an addition or 0 if it is a subtraction.
 * 
 * @note The coefficient are assumed to be stored on an uint8_t type.
 * 
 * @return FP_POLY_E_SUCCESS if the operation was successful or an error code otherwise (see @ref fp_poly_error_t for the list of error codes).
 */
static fp_poly_error_t fp_poly_add_single_term_aux(fp_poly_t *p, uint8_t coeff, size_t degree, fp_field_t *field, uint8_t is_addition)
{
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
                if (node->coeff > 0 && coeff > UINT8_MAX - node->coeff)
                {
                    fp_poly_error(FP_POLY_E_COEFFICIENT_ARITHMETIC, __FILE__, __func__, __LINE__, "coefficient overflow");
                    return FP_POLY_E_COEFFICIENT_ARITHMETIC;
                }
                node->coeff += coeff;
            }
            else
            {
                if (node->coeff < coeff)
                {
                    fp_poly_error(FP_POLY_E_COEFFICIENT_ARITHMETIC, __FILE__, __func__, __LINE__, "coefficient underflow");
                    return FP_POLY_E_COEFFICIENT_ARITHMETIC;
                }
                node->coeff -= coeff;
                if (node->coeff == 0)
                {
                    // remove the a coefficient from the coefficient list if it is equal to zero
                    mpz_clrbit(p->index_coeff, degree);
                    if (list_remove_node(p->coeff, node) != LIST_E_SUCCESS)
                    {
                        fp_poly_error(FP_POLY_E_LIST_COEFFICIENT, __FILE__, __func__, __LINE__, "list_remove_node() failed");
                        return FP_POLY_E_LIST_COEFFICIENT;
                    }
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
                // remove the a coefficient from the coefficient list if it is equal to zero
                mpz_clrbit(p->index_coeff, degree);
                if (list_remove_node(p->coeff, node) != LIST_E_SUCCESS)
                {
                    fp_poly_error(FP_POLY_E_LIST_COEFFICIENT, __FILE__, __func__, __LINE__, "list_remove_node() failed");
                    return FP_POLY_E_LIST_COEFFICIENT;
                }
            }
        }
    }
    else
    {
        // there is no term at the specified degree, so a new element is the coefficient list must be created
        if (field != NULL)
        {
            if (is_addition)
                coeff = coeff % field->order;
            else
                coeff = (field->order - coeff) % field->order;
        }
        mpz_setbit(p->index_coeff, degree);
        if (list_add_at(p->coeff, coeff, count_bit_set_to_index(p->index_coeff, degree)) != LIST_E_SUCCESS)
        {
            fp_poly_error(FP_POLY_E_LIST_COEFFICIENT, __FILE__, __func__, __LINE__, "list_add_at() failed");
            return FP_POLY_E_LIST_COEFFICIENT;
        }
    }
    if (fp_poly_normalise_zero_polynom(p) != FP_POLY_E_SUCCESS)
    {
        fp_poly_error(FP_POLY_E_POLYNOM_MANIPULATION, __FILE__, __func__, __LINE__, "fp_poly_normalise_zero_polynom() failed");
        return FP_POLY_E_POLYNOM_MANIPULATION;
    }
    return FP_POLY_E_SUCCESS;
}

/**
 * @brief A wrapper of fp_poly_add_single_term_aux() to perform unit test on polynom addition.
 * 
 * @param p The polynom.
 * @param coeff The value of the term to add.
 * @param degree The degree of the term to add.
 * @param field The field in which the operation is performed (may be NULL).
 * 
 * @return See fp_poly_add_single_term_aux().
 */
fp_poly_error_t fp_poly_add_single_term(fp_poly_t *p, uint8_t coeff, size_t degree, fp_field_t *field)
{
    return fp_poly_add_single_term_aux(p, coeff, degree, field, 1);
}

/**
 * @brief Auxiliary function to add or substract two polynoms.
 * 
 * @details This function adds or substracts polynoms \p p and \p q together. <br>
 * The result is stored in the polynom \p res.
 * 
 * @param res The parameter which will store the result of the operation.
 * @param p The first polynom.
 * @param q The second polynom.
 * @param field The field in which the operation is performed (may be NULL).
 * @param is_addition A boolean value: 1 if the operation is an addition or 0 if it is a subtraction.
 * 
 * @note This function is used by fp_poly_add() and fp_poly_sub(). It aims at factorizing the code of these two functions.
 * 
 * @return FP_POLY_E_SUCCESS if the operation was successful or an error code otherwise (see @ref fp_poly_error_t for the list of error codes).
 */
static fp_poly_error_t fp_poly_add_aux(fp_poly_t *res, fp_poly_t *p, fp_poly_t *q, fp_field_t * field, uint8_t is_addition)
{
    size_t pos = 0;
    list_node_t *node = p->coeff->head;
    while (node != NULL)
    {
        if (fp_poly_add_single_term_aux(res, node->coeff, fp_poly_coeff_list_to_degree(p, pos), field, 1) != FP_POLY_E_SUCCESS)
        {
            fp_poly_error(FP_POLY_E_POLYNOM_MANIPULATION, __FILE__, __func__, __LINE__, "fp_poly_add_single_term_aux() failed");
            return FP_POLY_E_POLYNOM_MANIPULATION;
        }
        node = node->next;
        pos += 1;
    }
    pos = 0;
    node = q->coeff->head;
    while (node != NULL)
    {
        if (fp_poly_add_single_term_aux(res, node->coeff, fp_poly_coeff_list_to_degree(q, pos), field, is_addition))
        {
            fp_poly_error(FP_POLY_E_POLYNOM_MANIPULATION, __FILE__, __func__, __LINE__, "fp_poly_add_single_term_aux() failed");
            return FP_POLY_E_POLYNOM_MANIPULATION;
        }
        node = node->next;
        pos += 1;
    }
    return FP_POLY_E_SUCCESS;
}

/**
 * @brief Add two polynoms.
 * 
 * A new polynom is created to store the result of the addition.
 * 
 * @param res The parameter which will store the result of the addition.
 * @param p The first polynom.
 * @param q The second polynom.
 * @param field The field in which the addition is performed (may be NULL).
 * 
 * @return FP_POLY_E_SUCCESS if the operation was successful or an error code otherwise (see @ref fp_poly_error_t for the list of error codes).
 */
fp_poly_error_t fp_poly_add(fp_poly_t **res, fp_poly_t *p, fp_poly_t *q, fp_field_t *f)
{
    *res = fp_poly_init();
    if (!*res)
    {
        fp_poly_error_no_custom_msg(FP_POLY_E_MEMORY, __FILE__, __func__, __LINE__);
        return FP_POLY_E_MEMORY;
    }
    if (!p)
    {
        fp_poly_error_no_custom_msg(FP_POLY_E_POLYNOM_IS_NULL, __FILE__, __func__, __LINE__);
        return FP_POLY_E_POLYNOM_IS_NULL;
    }
    if (!q)
    {
        fp_poly_error_no_custom_msg(FP_POLY_E_POLYNOM_IS_NULL, __FILE__, __func__, __LINE__);
        return FP_POLY_E_POLYNOM_IS_NULL;
    }
    if (!(p->coeff))
    {
        fp_poly_error_no_custom_msg(FP_POLY_E_LIST_COEFFICIENT_IS_NULL, __FILE__, __func__, __LINE__);
        return FP_POLY_E_LIST_COEFFICIENT_IS_NULL;
    }
    if (!(p->coeff->head))
    {
        fp_poly_error(FP_POLY_E_POLYNOM_MANIPULATION, __FILE__, __func__, __LINE__, "head of list of coefficient is NULL");
        return FP_POLY_E_POLYNOM_MANIPULATION;
    }
    if (!(q->coeff))
    {
        fp_poly_error_no_custom_msg(FP_POLY_E_LIST_COEFFICIENT_IS_NULL, __FILE__, __func__, __LINE__);
        return FP_POLY_E_LIST_COEFFICIENT_IS_NULL;
    }
    if (!(q->coeff->head))
    {
        fp_poly_error(FP_POLY_E_POLYNOM_MANIPULATION, __FILE__, __func__, __LINE__, "head of list of coefficient is NULL");
        return FP_POLY_E_POLYNOM_MANIPULATION;
    }
    if (!fp_poly_is_zero(p) && fp_poly_is_zero(q))
    {
        mpz_set((*res)->index_coeff, p->index_coeff);
        (*res)->coeff = list_copy(p->coeff);
        return FP_POLY_E_SUCCESS;
    }
    if (fp_poly_is_zero(p) && !fp_poly_is_zero(q))
    {
        mpz_set((*res)->index_coeff, q->index_coeff);
        (*res)->coeff = list_copy(q->coeff);
        return FP_POLY_E_SUCCESS;
    }
    return fp_poly_add_aux(*res, p, q, f, 1);
}

/**
 * @brief Substract two polynoms.
 * 
 * A new polynom is created to store the result of the substraction.
 * 
 * @param res The parameter which will store the result of the substraction.
 * @param p The first polynom.
 * @param q The second polynom.
 * @param field The field in which the substraction is performed (may be NULL).
 * 
 * @return FP_POLY_E_SUCCESS if the operation was successful or an error code otherwise (see @ref fp_poly_error_t for the list of error codes).
 */
fp_poly_error_t fp_poly_sub(fp_poly_t **res, fp_poly_t *p, fp_poly_t *q, fp_field_t *f)
{
    *res = fp_poly_init();
    if (!*res)
    {
        fp_poly_error(FP_POLY_E_MEMORY, __FILE__, __func__, __LINE__, "");
        return FP_POLY_E_MEMORY;
    }
    if (!p)
    {
        fp_poly_error(FP_POLY_E_POLYNOM_IS_NULL, __FILE__, __func__, __LINE__, "");
        return FP_POLY_E_POLYNOM_IS_NULL;
    }
    if (!q)
    {
        fp_poly_error(FP_POLY_E_POLYNOM_IS_NULL, __FILE__, __func__, __LINE__, "");
        return FP_POLY_E_POLYNOM_IS_NULL;
    }
    if (!(p->coeff))
    {
        fp_poly_error(FP_POLY_E_LIST_COEFFICIENT_IS_NULL, __FILE__, __func__, __LINE__, "");
        return FP_POLY_E_LIST_COEFFICIENT_IS_NULL;
    }
    if (!(p->coeff->head))
    {
        fp_poly_error(FP_POLY_E_POLYNOM_MANIPULATION, __FILE__, __func__, __LINE__, "head of list of coefficient is NULL");
        return FP_POLY_E_POLYNOM_MANIPULATION;
    }
    if (!(q->coeff))
    {
        fp_poly_error(FP_POLY_E_LIST_COEFFICIENT_IS_NULL, __FILE__, __func__, __LINE__, "");
        return FP_POLY_E_LIST_COEFFICIENT_IS_NULL;
    }
    if (!(q->coeff->head))
    {
        fp_poly_error(FP_POLY_E_POLYNOM_MANIPULATION, __FILE__, __func__, __LINE__, "head of list of coefficient is NULL");
        return FP_POLY_E_POLYNOM_MANIPULATION;
    }
    if (!fp_poly_is_zero(p) && fp_poly_is_zero(q))
    {
        mpz_set((*res)->index_coeff, p->index_coeff);
        (*res)->coeff = list_copy(p->coeff);
        return FP_POLY_E_SUCCESS;
    }
    /*
    if (fp_poly_is_zero(p) && !fp_poly_is_zero(q))
    {
        // TODO: handle the other case
        return FP_POLY_E_SUCCESS;
    }
    */
    return fp_poly_add_aux(*res, p, q, f, 0);
}

/**
 * @brief Multiply two polynoms.
 * 
 * A new polynom is created to store the result of the substraction.
 * 
 * @param res The parameter which will store the result of the multiplication.
 * @param p The first polynom.
 * @param q The second polynom.
 * @param field The field in which the substraction is performed (may be NULL).
 * 
 * @return FP_POLY_E_SUCCESS if the operation was successful or an error code otherwise (see @ref fp_poly_error_t for the list of error codes).
 */
fp_poly_error_t fp_poly_mul(fp_poly_t **res, fp_poly_t *p, fp_poly_t *q, fp_field_t *f)
{
    *res = fp_poly_init();
    if (!*res)
    {
        fp_poly_error(FP_POLY_E_MEMORY, __FILE__, __func__, __LINE__, "");
        return FP_POLY_E_MEMORY;
    }
    if (!p)
    {
        fp_poly_error(FP_POLY_E_POLYNOM_IS_NULL, __FILE__, __func__, __LINE__, "");
        return FP_POLY_E_POLYNOM_IS_NULL;
    }
    if (!q)
    {
        fp_poly_error(FP_POLY_E_POLYNOM_IS_NULL, __FILE__, __func__, __LINE__, "");
        return FP_POLY_E_POLYNOM_IS_NULL;
    }
    if (!(p->coeff))
    {
        fp_poly_error(FP_POLY_E_LIST_COEFFICIENT_IS_NULL, __FILE__, __func__, __LINE__, "");
        return FP_POLY_E_LIST_COEFFICIENT_IS_NULL;
    }
    if (!(p->coeff->head))
    {
        fp_poly_error(FP_POLY_E_POLYNOM_MANIPULATION, __FILE__, __func__, __LINE__, "head of list of coefficient is NULL");
        return FP_POLY_E_POLYNOM_MANIPULATION;
    }
    if (!(q->coeff))
    {
        fp_poly_error(FP_POLY_E_LIST_COEFFICIENT_IS_NULL, __FILE__, __func__, __LINE__, "");
        return FP_POLY_E_LIST_COEFFICIENT_IS_NULL;
    }
    if (!(q->coeff->head))
    {
        fp_poly_error(FP_POLY_E_POLYNOM_MANIPULATION, __FILE__, __func__, __LINE__, "head of list of coefficient is NULL");
        return FP_POLY_E_POLYNOM_MANIPULATION;
    }
    if (fp_poly_is_zero(p) || fp_poly_is_zero(q))
    {
        mpz_set_ui((*res)->index_coeff, 1);
        if (list_add_beginning((*res)->coeff, 0) != LIST_E_SUCCESS)
        {
            fp_poly_error(FP_POLY_E_LIST_COEFFICIENT, __FILE__, __func__, __LINE__, "list_add_beginning() failed");
            return FP_POLY_E_LIST_COEFFICIENT;
        }
        return FP_POLY_E_SUCCESS;
    }
    size_t pos_p = 0;
    list_node_t *node_p = p->coeff->head;
    while (node_p != NULL)
    {
        size_t pos_q = 0;
        list_node_t *node_q = q->coeff->head;
        while (node_q != NULL)
        {
            fp_poly_error_t err = fp_poly_add_single_term_aux(*res, node_p->coeff * node_q->coeff, fp_poly_coeff_list_to_degree(p, pos_p) + fp_poly_coeff_list_to_degree(q, pos_q), f, 1);
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

/**
 * @brief Retrieve the remainder of the division of the multiplication of two polynoms by an irreducible polynom.
 * 
 * @param res The parameter which will store the remainder of the division.
 * @param p The first polynom.
 * @param q The second polynom.
 * @param f The field in which the operation is performed.
 * 
 * @return FP_POLY_E_SUCCESS if the operation was successful or an error code otherwise (see @ref fp_poly_error_t for the list of error codes).
 */
fp_poly_error_t fp_poly_mul_fq(fp_poly_t **res, fp_poly_t *p, fp_poly_t *q, fp_field_t *f)
{
    fp_poly_t *tmp_res, *tmp_q, *tmp_r;
    if (fp_poly_mul(&tmp_res, p, q, f) != FP_POLY_E_SUCCESS)
    {
        fp_poly_error(FP_POLY_E_POLYNOM_MANIPULATION, __FILE__, __func__, __LINE__, "");
        return FP_POLY_E_POLYNOM_MANIPULATION;
    }
    if (fp_poly_div(&tmp_q, &tmp_r, tmp_res, f->irreducible_polynom, f) != FP_POLY_E_SUCCESS)
    {
        fp_poly_error(FP_POLY_E_POLYNOM_MANIPULATION, __FILE__, __func__, __LINE__, "");
        return FP_POLY_E_POLYNOM_MANIPULATION;
    }
    if (fp_poly_free(tmp_res) != FP_POLY_E_SUCCESS)
    {
        fp_poly_error(FP_POLY_E_POLYNOM_MANIPULATION, __FILE__, __func__, __LINE__, "");
        return FP_POLY_E_POLYNOM_MANIPULATION;
    }
    if (fp_poly_free(tmp_q) != FP_POLY_E_SUCCESS)
    {
        fp_poly_error(FP_POLY_E_POLYNOM_MANIPULATION, __FILE__, __func__, __LINE__, "");
        return FP_POLY_E_POLYNOM_MANIPULATION;
    }
    *res = tmp_r;
    return FP_POLY_E_SUCCESS;
}

/**
 * @brief Compute the inverse of an integer within a field.
 * 
 * @param element The integer.
 * @param field The field.
 * 
 * @return The inverse of the integer within the field or 0 if no inverse is found.
 */
uint8_t fp_poly_inv(uint8_t element, fp_field_t *field)
{
    for (uint8_t i = 1; i < field->order; i++)
        if ((element * i) % field->order == 1)
            return i;
    return 0;
}

/**
 * @brief Divide two polynoms.
 * 
 * @param q The parameter which will store the quotient.
 * @param r The parameter which will store the remainder.
 * @param n The dividend.
 * @param d The divisor.
 * @param f The field in which the division is performed.
 * 
 * @note A field is required since only the multiplication of integer is supported (impossible to multiply an integer by a fraction).
 * 
 * @return FP_POLY_E_SUCCESS if the operation was successful or an error code otherwise (see @ref fp_poly_error_t for the list of error codes).
 */
fp_poly_error_t fp_poly_div(fp_poly_t **q, fp_poly_t **r, fp_poly_t *n, fp_poly_t *d, fp_field_t *f)
{
    if ((*r = fp_poly_init_mpz(n->index_coeff, list_copy(n->coeff))) == NULL)
    {
        fp_poly_error_no_custom_msg(FP_POLY_E_MEMORY, __FILE__, __func__, __LINE__);
        return FP_POLY_E_MEMORY;
    }
    if ((*q = fp_poly_init()) == NULL)
    {
        fp_poly_error_no_custom_msg(FP_POLY_E_MEMORY, __FILE__, __func__, __LINE__);
        return FP_POLY_E_MEMORY;
    }
    mpz_set_ui((*q)->index_coeff, 0x1);
    if (list_add_beginning((*q)->coeff, 0) != LIST_E_SUCCESS)
    {
        fp_poly_error(FP_POLY_E_LIST_COEFFICIENT, __FILE__, __func__, __LINE__, "list_add_beginning() failed");
        return FP_POLY_E_LIST_COEFFICIENT;
    }
    fp_poly_t *t;
    if ((t = fp_poly_init()) == NULL)
    {
        fp_poly_error_no_custom_msg(FP_POLY_E_MEMORY, __FILE__, __func__, __LINE__);
        return FP_POLY_E_MEMORY;
    }
    if (list_add_beginning(t->coeff, 0) != LIST_E_SUCCESS)
    {
        fp_poly_error(FP_POLY_E_LIST_COEFFICIENT, __FILE__, __func__, __LINE__, "list_add_beginning() failed");
        return FP_POLY_E_LIST_COEFFICIENT;
    }
    mpz_t bitwise;
    mpz_init(bitwise);
    while (fp_poly_is_zero(*r) == 0 && fp_poly_degree(*r) >= fp_poly_degree(d))
    {
        mpz_set_ui(bitwise, 0);
        mpz_setbit(bitwise, fp_poly_degree(*r) - fp_poly_degree(d));
        mpz_set(t->index_coeff, bitwise);
        uint16_t tmp = (uint16_t) fp_poly_inv(d->coeff->tail->coeff, f) * (uint16_t) (*r)->coeff->tail->coeff;
        tmp = tmp % f->order;
        t->coeff->tail->coeff = (uint8_t) tmp;
        fp_poly_t *mem = *q;
        fp_poly_error_t err;
        if ((err = fp_poly_add(q, *q, t, f) != FP_POLY_E_SUCCESS))
        {
            fp_poly_error(err, __FILE__, __func__, __LINE__, "");
            return err;
        }
        if (fp_poly_free(mem) != FP_POLY_E_SUCCESS)
        {
            fp_poly_error(FP_POLY_E_POLYNOM_MANIPULATION, __FILE__, __func__, __LINE__, "");
            return FP_POLY_E_POLYNOM_MANIPULATION;
        }
        fp_poly_t *intermediate;
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
    }
    fp_poly_free(t);
    mpz_clear(bitwise);
    return FP_POLY_E_SUCCESS;
}

/**
 * @brief Compute the greatest common divisor of two polynoms.
 * 
 * @param res The polynom which will store the greatest common divisor.
 * @param p The first polynom.
 * @param q The second polynom.
 * @param f The field in which the operation is performed.
 * 
 * @return FP_POLY_E_SUCCESS if the operation was successful or an error code otherwise (see @ref fp_poly_error_t for the list of error codes).
 */
fp_poly_error_t fp_poly_gcd(fp_poly_t **res, fp_poly_t *p, fp_poly_t *q, fp_field_t *f)
{
    fp_poly_t *r1 = fp_poly_init_mpz(p->index_coeff, list_copy(p->coeff));
    if (!r1)
    {
        fp_poly_error(FP_POLY_E_POLYNOM_MANIPULATION, __FILE__, __func__, __LINE__, "initialisation of polynom failed");
        return FP_POLY_E_POLYNOM_MANIPULATION;
    }
    fp_poly_t *r2 = fp_poly_init_mpz(q->index_coeff, list_copy(q->coeff));
    if (!r2)
    {
        fp_poly_error(FP_POLY_E_POLYNOM_MANIPULATION, __FILE__, __func__, __LINE__, "initialisation of polynom failed");
        return FP_POLY_E_POLYNOM_MANIPULATION;
    }
    while (fp_poly_is_zero(r2) == 0)
    {
        fp_poly_t *q_tmp, *r_tmp;
        if (fp_poly_div(&q_tmp, &r_tmp, r1, r2, f)!= FP_POLY_E_SUCCESS)
        {
            fp_poly_error(FP_POLY_E_POLYNOM_MANIPULATION, __FILE__, __func__, __LINE__, "division of polynoms failed");
            return FP_POLY_E_POLYNOM_MANIPULATION;
        }
        if (fp_poly_free(q_tmp) != FP_POLY_E_SUCCESS)
        {
            fp_poly_error(FP_POLY_E_POLYNOM_MANIPULATION, __FILE__, __func__, __LINE__, "free of polynom failed");
            return FP_POLY_E_POLYNOM_MANIPULATION;
        }
        fp_poly_t *mem = r1;
        r1 = r2;
        r2 = r_tmp;
        if (fp_poly_free(mem) != FP_POLY_E_SUCCESS)
        {
            fp_poly_error(FP_POLY_E_POLYNOM_MANIPULATION, __FILE__, __func__, __LINE__, "free of polynom failed");
            return FP_POLY_E_POLYNOM_MANIPULATION;
        }
    }
    *res = r1;
    if (fp_poly_free(r2) != FP_POLY_E_SUCCESS)
    {
        fp_poly_error(FP_POLY_E_POLYNOM_MANIPULATION, __FILE__, __func__, __LINE__, "free of polynom failed");
        return FP_POLY_E_POLYNOM_MANIPULATION;
    }
    return FP_POLY_E_SUCCESS;
}

/**
 * @brief Compute the extended greatest common divisor of two polynoms.
 * 
 * @param res The polynom which will store the greatest common divisor.
 * @param u The first coefficient of the extended greatest common divisor.
 * @param v The second coefficient of the extended greatest common divisor.
 * @param p The first polynom.
 * @param q The second polynom.
 * @param f The field in which the operation is performed.
 * 
 * @return FP_POLY_E_SUCCESS if the operation was successful or an error code otherwise (see @ref fp_poly_error_t for the list of error codes).
 */
fp_poly_error_t fp_poly_gcd_extended(fp_poly_t **res, fp_poly_t **u, fp_poly_t **v, fp_poly_t *p, fp_poly_t *q, fp_field_t *f)
{
    fp_poly_t *old_r = fp_poly_init_mpz(p->index_coeff, list_copy(p->coeff));
    fp_poly_t *r = fp_poly_init_mpz(q->index_coeff, list_copy(q->coeff));
    fp_poly_t *old_s = fp_poly_init_array((uint8_t[]) {1}, 0x1);
    fp_poly_t *s = fp_poly_init_array((uint8_t[]) {0}, 0x1);
    fp_poly_t *old_t = fp_poly_init_array((uint8_t[]) {0}, 0x1);
    fp_poly_t *t = fp_poly_init_array((uint8_t[]) {1}, 0x1);
    fp_poly_t *quot, *rem, *prov, *tmp;
    while (!fp_poly_is_zero(r))
    {
        fp_poly_div(&quot, &rem, old_r, r, f);

        prov = fp_poly_init_mpz(r->index_coeff, list_copy(r->coeff));
        fp_poly_mul(&tmp, quot, prov, f);
        fp_poly_sub(&r, old_r, tmp, f);
        old_r = prov;

        prov = fp_poly_init_mpz(s->index_coeff, list_copy(s->coeff));
        fp_poly_mul(&tmp, quot, prov, f);
        fp_poly_sub(&s, old_s, tmp, f);
        old_s = prov;

        prov = fp_poly_init_mpz(t->index_coeff, list_copy(t->coeff));
        fp_poly_mul(&tmp, quot, prov, f);
        fp_poly_sub(&t, old_t, tmp, f);
        old_t = prov;
    }
    // custom error handling: sometimes, a "+0" is kept at the end of the polynom
    // thus raising an error concerning the expected index of coeff during the unit test
    if (old_s->coeff->head->coeff == 0)
    {
        list_remove_head(old_s->coeff);
        mpz_clrbit(old_s->index_coeff, 0);
    }
    *res = old_r;
    *u = old_s;
    *v = old_t;
    fp_poly_free(r);
    fp_poly_free(s);
    fp_poly_free(t);
    return FP_POLY_E_SUCCESS;
}

/**
 * @brief Parse a string to create a polynom.
 *
 * @param polynomial The string.
 *
 * @return A pointer to the polynom if the operation was successful or NULL otherwise.
 */
fp_poly_t *fp_poly_parse(const char* polynomial)
{
    if (!polynomial)
    {
        fp_poly_error(FP_POLY_E_POLYNOM_IS_NULL, __FILE__, __func__, __LINE__, "");
        return NULL;
    }
    if (strlen(polynomial) == 0)
    {
        fp_poly_error(FP_POLY_E_POLYNOM_MANIPULATION, __FILE__, __func__, __LINE__, "the polynom string is empty");
        return NULL;
    }
    fp_poly_t *res = fp_poly_init();
    const char *ptr = polynomial;
    if (!res)
    {
        fp_poly_error(FP_POLY_E_MEMORY, __FILE__, __func__, __LINE__, "");
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
        uint8_t coefficient = 0;
        size_t degree = 0;
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
        if (fp_poly_add_single_term_aux(res, coefficient, degree, NULL, 1) != FP_POLY_E_SUCCESS)
        {
            fp_poly_error(FP_POLY_E_POLYNOM_MANIPULATION, __FILE__, __func__, __LINE__, "fp_poly_add_single_term_aux() failed");
            return NULL;
        }
        while (*ptr == ' ' || *ptr == '+')
            ptr++;
    }
    return res;
}

/**
 * @brief Initialize an empty polynom (without any coefficients).
 *
 * @return A pointer to the polynom if the operation was successful or NULL otherwise.
 */
fp_poly_t *fp_poly_init(void)
{
    fp_poly_t *res = (fp_poly_t *) malloc(sizeof(fp_poly_t));
    if (!res)
    {
        fp_poly_error_no_custom_msg(FP_POLY_E_MEMORY, __FILE__, __func__, __LINE__);
        return NULL;
    }
    mpz_init_set_ui(res->index_coeff, 0x0);
    res->coeff = list_init();
    return res;
}

/**
 * @brief Count the number of set bi of a number.
 * 
 * @param n The number.
 * 
 * @return The number of set bit.
 */
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

/**
 * @brief Initialize a polynom when the degree of the coefficient are given as a size_t number.
 * 
 * @param pos_coeff The degree of the coefficient (as a size_t number).
 * @param coeff The coefficient list.
 * 
 * @return A pointer to the polynom if the operation was successful or NULL otherwise.
 */
fp_poly_t *fp_poly_init_sizet(size_t pos_coeff, list_t *coeff)
{
    if (!coeff)
    {
        fp_poly_error_no_custom_msg(FP_POLY_E_LIST_COEFFICIENT_IS_NULL, __FILE__, __func__, __LINE__);
        return NULL;
    }
    if (pos_coeff == 0)
    {
        fp_poly_error(FP_POLY_E_POLYNOM_MANIPULATION, __FILE__, __func__, __LINE__, "the position of the coefficient is missing");
        return NULL;
    }
    fp_poly_t *res = (fp_poly_t *) malloc(sizeof(fp_poly_t));
    if (!res)
    {
        fp_poly_error(FP_POLY_E_MEMORY, __FILE__, __func__, __LINE__, "");
        return NULL;
    }
    if (fp_poly_count_set_bits(pos_coeff) != coeff->size)
    {
        char buffer[150];
        snprintf(buffer, 150, "the number of coefficients in the list and the index of the coefficients are not consistent: number of coefficients is %ld but index of coeff is %ld", coeff->size, fp_poly_count_set_bits(pos_coeff));
        fp_poly_error(FP_POLY_E_POLYNOM_MANIPULATION, __FILE__, __func__, __LINE__, buffer);
        return NULL;
    }
    mpz_init_set_ui(res->index_coeff, pos_coeff);
    res->coeff = coeff;
    return res;
}

/**
 * @brief Initialize a polynom when the degree of the coefficient are given as an mpz_t number.
 * 
 * @param pos_coeff The degree of the coefficient (as a mpz_t number).
 * @param coeff The coefficient list.
 * 
 * @return A pointer to the polynom if the operation was successful or NULL otherwise.
 */
fp_poly_t *fp_poly_init_mpz(mpz_t pos_coeff, list_t *coeff)
{
    if (!coeff)
    {
        fp_poly_error_no_custom_msg(FP_POLY_E_LIST_COEFFICIENT_IS_NULL, __FILE__, __func__, __LINE__);
        return NULL;
    }
    if (mpz_cmp_ui(pos_coeff, 0) == 0)
    {
        fp_poly_error(FP_POLY_E_POLYNOM_MANIPULATION, __FILE__, __func__, __LINE__, "the position of the coefficient is missing");
        return NULL;
    }
    fp_poly_t *res = (fp_poly_t *) malloc(sizeof(fp_poly_t));
    if (!res)
    {
        fp_poly_error(FP_POLY_E_MEMORY, __FILE__, __func__, __LINE__, "");
        return NULL;
    }
    if (fp_poly_count_set_bits(mpz_get_ui(pos_coeff)) != coeff->size)
    {
        char buffer[150];
        snprintf(buffer, 150, "the number of coefficients in the list and the index of the coefficients are not consistent: number of coefficients is %ld but index of coeff is %ld", coeff->size, fp_poly_count_set_bits(mpz_get_ui(pos_coeff)));
        fp_poly_error(FP_POLY_E_POLYNOM_MANIPULATION, __FILE__, __func__, __LINE__, buffer);
        return NULL;
    }
    mpz_init_set(res->index_coeff, pos_coeff);
    res->coeff = coeff;
    return res;
}


/**
 * @brief Initialize a polynom when the coefficient are given as an array.
 * 
 * @param coeff The array.
 * @param len The length of the array.
 * 
 * @return A pointer to the polynom if the operation was successful or NULL otherwise.
 */
fp_poly_t *fp_poly_init_array(uint8_t *coeff, size_t len)
{
    if (!coeff)
    {
        fp_poly_error_no_custom_msg(FP_POLY_E_LIST_COEFFICIENT_IS_NULL, __FILE__, __func__, __LINE__);
        return NULL;
    }
    if (len == 0)
    {
        fp_poly_error(FP_POLY_E_POLYNOM_MANIPULATION, __FILE__, __func__, __LINE__, "the length of the array is zero");
        return NULL;
    }
    fp_poly_t *res = (fp_poly_t *) malloc(sizeof(fp_poly_t));
    if (!res)
    {
        fp_poly_error(FP_POLY_E_MEMORY, __FILE__, __func__, __LINE__, "");
        return NULL;
    }
    res->coeff = list_init();
    mpz_init_set_ui(res->index_coeff, 0x0);
    uint8_t is_all_coeff_zero = 1;
    for (size_t i = 0; i < len; i++)
        if (coeff[i] != 0)
            is_all_coeff_zero = 0;
    if (is_all_coeff_zero)
    {
        mpz_set_ui(res->index_coeff, 0x1);
        list_add_beginning(res->coeff, 0);
        return res;
    }
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

/**
 * @brief Auxiliary function to check the irreducibility of a polynom within a field.
 * 
 * @details This function returns the polynom x^n - x.
 * 
 * @param n The degree.
 * @param f The field.
 * 
 * @return The polynom x^n - x.
 */
static fp_poly_t *fp_poly_is_irreducible_aux(uint64_t n, fp_field_t *f)
{
    mpz_t deg_x_n_minux_x;
    mpz_init_set_ui(deg_x_n_minux_x, 0x0);
    mpz_setbit(deg_x_n_minux_x, n-1);
    mpz_setbit(deg_x_n_minux_x, 0);
    list_t *list = list_init();
    list_add_beginning(list, 1);
    list_add_beginning(list, f->order - 1);
    fp_poly_t *x_n_minux_x = fp_poly_init_mpz(deg_x_n_minux_x, list);
    return x_n_minux_x;
}

/**
 * @brief Check if a polynom is irreducible within a field.
 * 
 * @param p The polynom.
 * @param f The field.
 * 
 * @return A boolean value: 1 if the polynom is irreducible, 0 otherwise.
 */
uint8_t fp_poly_is_irreducible(fp_poly_t *p, fp_field_t *f)
{
    fp_poly_t *x_n_minus_x, *q, *r, *res_gcd;
    x_n_minus_x = fp_poly_is_irreducible_aux(my_pow(f->order, fp_poly_degree(p)), f);
    if (fp_poly_div(&q, &r, x_n_minus_x, p, f) != FP_POLY_E_SUCCESS)
    {
        fp_poly_error(FP_POLY_E_POLYNOM_MANIPULATION, __FILE__, __func__, __LINE__, "fp_poly_div() failed");
        return 0;
    }
    if (!fp_poly_is_zero(r))
        return 0;
    for (size_t i = 0; i < fp_poly_degree(p); i++)
    {
        if (is_prime(i, 15))
        {
            if (fp_poly_degree(p) % i == 0)
            {
                x_n_minus_x = fp_poly_is_irreducible_aux(i, f);
                if (fp_poly_gcd(&res_gcd, x_n_minus_x, p, f) != FP_POLY_E_SUCCESS)
                {
                    fp_poly_error(FP_POLY_E_POLYNOM_MANIPULATION, __FILE__, __func__, __LINE__, "fp_poly_gcd() failed");
                    return 0;
                }
                if (!fp_poly_is_unit(res_gcd))
                    return 0;
            }
        }
    }
    return 1;
}

/**
 * @brief Initialize a random polynom of a given degree within a field.
 * 
 * @param degree The degree.
 * @param f The field.
 * 
 * @return A pointer to the polynom if the operation was successful or NULL otherwise.
 */
fp_poly_t *fp_poly_init_random(size_t degree, fp_field_t *f)
{
    if (!f)
    {
        fp_poly_error_no_custom_msg(FP_POLY_E_FIELD_IS_NULL, __FILE__, __func__, __LINE__);
        return NULL;
    }
    fp_poly_t *res = (fp_poly_t *) malloc(sizeof(fp_poly_t));
    if (!res)
    {
        fp_poly_error(FP_POLY_E_MEMORY, __FILE__, __func__, __LINE__, "");
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

/**
 * @brief Initialize a random irreducible polynom of a given degree within a field.
 *
 * @details The Cohen's irreducibility criterion (https://en.wikipedia.org/wiki/Cohn%27s_irreducibility_criterion) is implemented.
 *
 * @param digits The number of digits of the polynom.
 * @param field The field of the polynom.
 *
 * @return A pointer to the polynom if the operation was successful or NULL otherwise.
 */
fp_poly_t *fp_poly_init_random_irreducible(size_t digits, fp_field_t *field)
{
    mpz_t rand;
    list_t *list = list_init();
    mpz_init(rand);
    random_prime_mpz(rand, digits);
    /*
    for (size_t i = 0; i < mpz_sizeinbase(rand, 2); i++)
        if (mpz_tstbit(rand, i))
            list_add_beginning(list, 1);
    */
    char *rand_change_base = mpz_get_str(NULL, field->order, rand);
    mpz_clear(rand);
    mpz_t rand_poly;
    mpz_init(rand_poly);
    for (size_t i = 0; i < strlen(rand_change_base); i++)
    {
        if (rand_change_base[i] - '0' != 0)
        {
            list_add_end(list, rand_change_base[i] - '0');
            mpz_setbit(rand_poly, i);
        }
    }
    fp_poly_t *res = (fp_poly_t*) malloc(sizeof(fp_poly_t));
    mpz_init_set(res->index_coeff, rand_poly);
    res->coeff = list;
    return res;
}

/**
 * @brief Free a polynom.
 * 
 * @param p The polynom.
 * 
 * @return FP_POLY_E_SUCCESS if the operation was successful or an error code otherwise (see @ref fp_poly_error_t for the list of error codes).
 */
fp_poly_error_t fp_poly_free(fp_poly_t *p)
{
    if (!p)
    {
        fp_poly_error_no_custom_msg(FP_POLY_E_POLYNOM_IS_NULL, __FILE__, __func__, __LINE__);
        return FP_POLY_E_POLYNOM_IS_NULL;
    }
    if (!p->coeff)
    {
        fp_poly_error_no_custom_msg(FP_POLY_E_LIST_COEFFICIENT_IS_NULL, __FILE__, __func__, __LINE__);
        return FP_POLY_E_LIST_COEFFICIENT_IS_NULL;
    }
    list_destroy(p->coeff);
    mpz_clear(p->index_coeff);
    free(p);
    return FP_POLY_E_SUCCESS;
}

/**
 * @brief Check that a polynom is equal to an expected one when the expected degree of the coefficient is given as an mpz_t type.
 *
 * @param p The polynom.
 * @param expected_pos_coeff The expected degree of the coefficient (as an mpz_t type).
 * @param expected_coeff The expected coefficient list.
 *
 * @return FP_POLY_E_SUCCESS if the equality is true or an error code otherwise (see @ref fp_poly_error_t for the list of error codes).
 */
fp_poly_error_t fp_poly_assert_mpz(fp_poly_t *p, mpz_t expected_pos_coeff, list_t *expected_coeff)
{
    if (!p)
    {
        fp_poly_error_no_custom_msg(FP_POLY_E_POLYNOM_IS_NULL, __FILE__, __func__, __LINE__);
        return FP_POLY_E_POLYNOM_IS_NULL;
    }
    if (!p->coeff)
    {
        fp_poly_error_no_custom_msg(FP_POLY_E_LIST_COEFFICIENT_IS_NULL, __FILE__, __func__, __LINE__);
        return FP_POLY_E_LIST_COEFFICIENT_IS_NULL;
    }
    if (!expected_pos_coeff)
    {
        fp_poly_error(FP_POLY_E_POLYNOM_MANIPULATION, __FILE__, __func__, __LINE__, "expected index of coefficients is missing");
        return FP_POLY_E_POLYNOM_MANIPULATION;
    }
    if (!expected_coeff)
    {
        fp_poly_error(FP_POLY_E_POLYNOM_MANIPULATION, __FILE__, __func__, __LINE__, "expected list of coefficients is missing");
        return FP_POLY_E_POLYNOM_MANIPULATION;
    }
    if (mpz_cmp(p->index_coeff, expected_pos_coeff) != 0)
    {
        char buffer[150];
        snprintf(buffer, 150, "expected index of coefficients: %s but got index of coefficients: %s", mpz_get_str(NULL, 10, p->index_coeff), mpz_get_str(NULL, 10, expected_pos_coeff));
        fp_poly_error(FP_POLY_E_POLYNOM_MANIPULATION, __FILE__, __func__, __LINE__, buffer);
        return FP_POLY_E_POLYNOM_MANIPULATION;
    }
    size_t pos_p = 0;
    size_t pos_expected = 0;
    list_node_t *node_p = p->coeff->head;
    list_node_t *node_expected = expected_coeff->head;
    while (node_p != NULL && node_expected != NULL)
    {
        if (node_p->coeff != node_expected->coeff)
        {
            char buffer[100];
            snprintf(buffer, 100, "expected coeff: %u but got coeff: %u at pos = %ld\n", node_expected->coeff, node_p->coeff, pos_p);
            fp_poly_error(FP_POLY_E_COEFFICIENT_ARITHMETIC, __FILE__, __func__, __LINE__, buffer);
            return FP_POLY_E_COEFFICIENT_ARITHMETIC;
        }
        pos_p += 1;
        pos_expected += 1;
        node_p = node_p->next;
        node_expected = node_expected->next;
    }
    if (node_p != NULL)
    {
        fp_poly_error(FP_POLY_E_POLYNOM_MANIPULATION, __FILE__, __func__, __LINE__, "there is more coefficients in the polynom than expected");
        return FP_POLY_E_POLYNOM_MANIPULATION;
    }
    if (node_expected != NULL)
    {
        fp_poly_error(FP_POLY_E_POLYNOM_MANIPULATION, __FILE__, __func__, __LINE__, "there is less coefficients in the polynom than expected");
        return FP_POLY_E_POLYNOM_MANIPULATION;
    }
    return FP_POLY_E_SUCCESS;
}

/**
 * @brief Check that a polynom is equal to an expected one when the expected degree of the coefficient is given as a size_t type.
 *
 * @param p The polynom.
 * @param expected_pos_coeff The expected degree of the coefficient (as an size_t type).
 * @param expected_coeff The expected coefficient list.
 *
 * @return FP_POLY_E_SUCCESS if the equality is true or an error code otherwise (see @ref fp_poly_error_t for the list of error codes).
 */
fp_poly_error_t fp_poly_assert_sizet(fp_poly_t *p, size_t expected_pos_coeff, list_t *expected_coeff)
{
    if (!p)
    {
        fp_poly_error_no_custom_msg(FP_POLY_E_POLYNOM_IS_NULL, __FILE__, __func__, __LINE__);
        return FP_POLY_E_POLYNOM_IS_NULL;
    }
    if (!p->coeff)
    {
        fp_poly_error_no_custom_msg(FP_POLY_E_LIST_COEFFICIENT_IS_NULL, __FILE__, __func__, __LINE__);
        return FP_POLY_E_LIST_COEFFICIENT_IS_NULL;
    }
    if (!expected_coeff)
    {
        fp_poly_error(FP_POLY_E_POLYNOM_MANIPULATION, __FILE__, __func__, __LINE__, "the expected list of coefficients is missing");
        return FP_POLY_E_POLYNOM_MANIPULATION;
    }
    if (mpz_cmp_ui(p->index_coeff, expected_pos_coeff) != 0)
    {
        char buffer[150];
        snprintf(buffer, 150, "expected index of coefficients: %s but got index of coefficients: %ld\n", mpz_get_str(NULL, 10, p->index_coeff), expected_pos_coeff);
        fp_poly_error(FP_POLY_E_POLYNOM_MANIPULATION, __FILE__, __func__, __LINE__, buffer);
        return FP_POLY_E_POLYNOM_MANIPULATION;
    }
    size_t pos_p = 0;
    size_t pos_expected = 0;
    list_node_t *node_p = p->coeff->head;
    list_node_t *node_expected = expected_coeff->head;
    while (node_p != NULL && node_expected != NULL)
    {
        if (node_p->coeff != node_expected->coeff)
        {
            char buffer[100];
            snprintf(buffer, 100, "expected coeff: %u but got coeff: %u  at pos = %ld\n", node_expected->coeff, node_p->coeff, pos_p);
            fp_poly_error(FP_POLY_E_COEFFICIENT_ARITHMETIC, __FILE__, __func__, __LINE__, buffer);
            return FP_POLY_E_COEFFICIENT_ARITHMETIC;
        }
        pos_p += 1;
        pos_expected += 1;
        node_p = node_p->next;
        node_expected = node_expected->next;
    }
    if (node_p != NULL)
    {
        fp_poly_error(FP_POLY_E_POLYNOM_MANIPULATION, __FILE__, __func__, __LINE__, "there is more coefficients in the polynom than expected");
        return FP_POLY_E_POLYNOM_MANIPULATION;
    }
    if (node_expected != NULL)
    {
        fp_poly_error(FP_POLY_E_POLYNOM_MANIPULATION, __FILE__, __func__, __LINE__, "there is less coefficients in the polynom than expected");
        return FP_POLY_E_POLYNOM_MANIPULATION;
    }
    return FP_POLY_E_SUCCESS;
}

/**
 * @brief Check that a polynom is equal to expected one.
 *
 * @param expected_p The expected polynom.
 * @param actual The polynom.
 *
 * @return FP_POLY_E_SUCCESS if the equality is true or an error code otherwise (see @ref fp_poly_error_t for the list of error codes).
 */
fp_poly_error_t fp_poly_assert_equality(fp_poly_t *expected_p, fp_poly_t *actual)
{
    if (!expected_p)
    {
        fp_poly_error(FP_POLY_E_POLYNOM_IS_NULL, __FILE__, __func__, __LINE__, "");
        return FP_POLY_E_POLYNOM_IS_NULL;
    }
    if (!expected_p->coeff)
    {
        fp_poly_error(FP_POLY_E_LIST_COEFFICIENT_IS_NULL, __FILE__, __func__, __LINE__, "");
        return FP_POLY_E_LIST_COEFFICIENT_IS_NULL;
    }
    if (!actual)
    {
        fp_poly_error(FP_POLY_E_POLYNOM_IS_NULL, __FILE__, __func__, __LINE__, "");
        return FP_POLY_E_POLYNOM_IS_NULL;
    }
    if (!actual->coeff)
    {
        fp_poly_error(FP_POLY_E_LIST_COEFFICIENT_IS_NULL, __FILE__, __func__, __LINE__, "");
        return FP_POLY_E_LIST_COEFFICIENT_IS_NULL;
    }
    return fp_poly_assert_mpz(expected_p, actual->index_coeff, actual->coeff);
}

/**
 * @brief Print a polynom.
 *
 * @param p The polynom.
 *
 * @returns FP_POLY_E_SUCCESS if the operation was successful or an error code otherwise (see @ref fp_poly_error_t for the list of error codes).
 */
fp_poly_error_t fp_poly_print(FILE *fd, fp_poly_t *p)
{
    if (!fd)
    {
        fp_poly_error_no_custom_msg(FP_POLY_E_FILE_DESCRIPTOR_IS_NULL, __FILE__, __func__, __LINE__);
        return FP_POLY_E_FILE_DESCRIPTOR_IS_NULL;
    }
    if (!p)
    {
        fp_poly_error_no_custom_msg(FP_POLY_E_POLYNOM_IS_NULL, __FILE__, __func__, __LINE__);
        return FP_POLY_E_POLYNOM_IS_NULL;
    }
    if (!p->coeff)
    {
        fp_poly_error_no_custom_msg(FP_POLY_E_LIST_COEFFICIENT_IS_NULL, __FILE__, __func__, __LINE__);
        return FP_POLY_E_LIST_COEFFICIENT_IS_NULL;
    }
    list_node_t *node = p->coeff->head;
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

/**
 * @brief Initialize the field of integer modulo.
 * 
 * @param order The order.
 * 
 * @return A pointer to the field if the operation was successful or NULL otherwise.
*/
fp_field_t *fp_poly_init_prime_field(uint8_t order)
{
    return fp_poly_init_galois_field(order, NULL);
}

/**
 * @brief Initialize a Galois Field.
 * 
 * @param order The order.
 * 
 * @return A pointer to the field if the operation was successful or NULL otherwise.
*/
fp_field_t *fp_poly_init_galois_field(uint8_t order, fp_poly_t *irreducible_polynom)
{
    if (order == 0)
    {
        fp_poly_error(FP_POLY_E_FIELD_MANIPULATION, __FILE__, __func__, __LINE__, "order is zero");
        return NULL;
    }
    fp_field_t *field = (fp_field_t *)malloc(sizeof(fp_field_t));
    if (!field)
    {
        fp_poly_error(FP_POLY_E_MEMORY, __FILE__, __func__, __LINE__, "");
        return NULL;
    }
    // TODO: check that the order must be the degree of the irreducible polynom ? and check that the order is a prime nu;ber or a power of a prime number
    field->order = order;
    field->irreducible_polynom = irreducible_polynom;
    return field;
}

/**
 * @brief Free a field.
 * 
 * @param field The field.
 * 
 * @return FP_POLY_E_SUCCESS if the operation was successful or an error code otherwise (see @ref fp_poly_error_t for the list of error codes).
*/
fp_poly_error_t fp_poly_free_field(fp_field_t *field)
{
    if (!field)
    {
        fp_poly_error(FP_POLY_E_FIELD_IS_NULL, __FILE__, __func__, __LINE__, "");
        return FP_POLY_E_FIELD_IS_NULL;
    }
    if (field->irreducible_polynom)
    {
        if (fp_poly_free(field->irreducible_polynom) != FP_POLY_E_SUCCESS)
        {
            fp_poly_error(FP_POLY_E_POLYNOM_MANIPULATION, __FILE__, __func__, __LINE__, "fp_poly_free() failed");
            return FP_POLY_E_POLYNOM_MANIPULATION;
        }
    }
    free(field);
    return FP_POLY_E_SUCCESS;
}