/**
 * @file fp_poly.h
 * @brief Header of polynomial manipulation in finite field.
 * @author Guillaume Roumage
 * @date 04/2024
*/

/*! @mainpage PolyNom
 * 
 * @section sec_one Introduction
 * 
 * PolyNom is a polynom manipulation library written in C with [GMP library](https://gmplib.org/).
 * 
 * Many operations are handle, such as, addition, multiplication over Galois Field, and PolyNom can handle arbitrary long polynoms.
 * 
 * A batch of unit tests assert the accuracy of the library with the use of the language [PariGP](https://pari.math.u-bordeaux.fr/).
 * 
 * @section sec_two Why this project?
 * 
 * - Get my hands dirty my delving into the implementation of theoritical concepts I learn at university. 
 * - Experiment the test-driven development and code coverage.
 * - Become familiar with the GMP library.
 * - Challenge myself with a (relatively) large project.
 */

#ifndef FP_POLY_H
#define FP_POLY_H

#include <inttypes.h>
#include <gmp.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../../lib/include/list.h"

#define FP_VARN 'x'

/**
 * @brief Structure representing a polynom.
 * 
 * The degree of the polynom is stored in an mpz_t number and the coefficient of the polynom are stored in a linked list. <br>
 * <br>
 * The index of the most/least significant bit of the mpz_t number is the degree of coefficient of the highest/lowest degree. <br>
 * The first/last element of the linked list is the coefficient of highest/lowest degree. <br>
 * 
 * For instance, the polynom 2 + x + x^3 is stored as:
 * - index_coeff = 11_{10} = 1011_{2}
 * - coeff = [2, 1, 1]
 * 
 * @param index_coeff The mpz_t number representing the degree of the coefficients of the polynom.
 * @param coeff The linked list that stores the coefficients of the polynom.
*/
typedef struct fp_poly_t
{
    mpz_t index_coeff;
    list_t *coeff;
} fp_poly_t;

/**
 * @brief Structure representing a finite field.
 * 
 * The field is defined by its order and an optionnal irreducible polynom.
 * 
 * @param order The order of the field.
 * @param irreducible_polynom The irreducible polynom that defines the field.
*/
typedef struct fp_field_t
{
    uint8_t order;
    fp_poly_t *irreducible_polynom;
} fp_field_t;

/**
 * @brief Enumeration of the errors code.
 * 
 * The errors code are the following:
 * - FP_POLY_E_SUCCESS indicates the success of the operation. <br>
 * - FP_POLY_E_POLYNOM_IS_NULL indicates that the polynom on which the operation is performed is NULL. <br>
 * - FP_POLY_E_FIELD_IS_NULL indicates that the field within which the operation is performed is NULL. <br>
 * - FP_POLY_E_LIST_COEFFICIENT_IS_NULL indicates that the list of coefficients is NULL. <br>
 * - FP_POLY_E_LIST_COEFFICIENT indicates that the list of coefficients is not valid (usually, a custom error message is printed). <br>
 * - FP_POLY_E_MEMORY indicates that a memory allocation failed. <br>
 * - FP_POLY_E_POLYNOM_MANIPULATION indicates that a polynom manipulation failed (usually, a custom error message is printed). <br>
 * - FP_POLY_E_FIELD_MANIPULATION indicates that a field manipulation failed (usually, a custom error message is printed). <br>
 * - FP_POLY_E_COEFFICIENT_ARITHMETIC indicates that a coefficient arithmetic operation failed (usually, a custom error message is printed). <br>
 * - FP_POLY_E_FILE_DESCRIPTOR_IS_NULL indicates that the file descriptor is NULL. <br>
*/
typedef enum fp_poly_error_e
{
    FP_POLY_E_SUCCESS,
    FP_POLY_E_POLYNOM_IS_NULL,
    FP_POLY_E_FIELD_IS_NULL,
    FP_POLY_E_LIST_COEFFICIENT_IS_NULL,
    FP_POLY_E_LIST_COEFFICIENT,
    FP_POLY_E_MEMORY,
    FP_POLY_E_POLYNOM_MANIPULATION,
    FP_POLY_E_FIELD_MANIPULATION,
    FP_POLY_E_COEFFICIENT_ARITHMETIC,
    FP_POLY_E_FILE_DESCRIPTOR_IS_NULL,
} fp_poly_error_t;

fp_poly_t *fp_poly_init(void);
fp_poly_t *fp_poly_init_sizet(size_t, list_t *);
fp_poly_t *fp_poly_init_mpz(mpz_t, list_t *);
fp_poly_t *fp_poly_init_array(uint8_t *, size_t);
fp_poly_t *fp_poly_init_random(size_t degree, fp_field_t *field);
fp_poly_t *fp_poly_init_random_irreducible(size_t, fp_field_t *);
fp_poly_t *fp_poly_parse(const char *);
fp_poly_error_t fp_poly_free (fp_poly_t *);
fp_poly_error_t fp_poly_assert_mpz(fp_poly_t*, mpz_t, list_t *);
fp_poly_error_t fp_poly_assert_sizet(fp_poly_t*, size_t, list_t *);
fp_poly_error_t fp_poly_assert_equality(fp_poly_t *, fp_poly_t *);
fp_poly_error_t fp_poly_print(FILE*, fp_poly_t *);
uint8_t fp_poly_is_irreducible(fp_poly_t *p, fp_field_t *f);

size_t fp_poly_degree(fp_poly_t *);
size_t fp_poly_coeff_list_to_degree(fp_poly_t *, size_t);
list_node_t *fp_poly_degree_to_node_list(fp_poly_t *, size_t);

//fp_poly_error_t fp_poly_add_single_term(fp_poly_t *, uint8_t, size_t, fp_field_t *);
fp_poly_error_t fp_poly_add(fp_poly_t **, fp_poly_t *, fp_poly_t *, fp_field_t *);
//fp_poly_error_t fp_poly_sub_single_term(fp_poly_t *, uint8_t, size_t, fp_field_t *);
fp_poly_error_t fp_poly_sub(fp_poly_t **, fp_poly_t *, fp_poly_t *, fp_field_t *);
//fp_poly_error_t fp_poly_mul_single_term(fp_poly_t *, uint8_t, size_t, fp_field_t *);
fp_poly_error_t fp_poly_mul(fp_poly_t **, fp_poly_t *, fp_poly_t *, fp_field_t *);
fp_poly_error_t fp_poly_mul_fq(fp_poly_t **, fp_poly_t *, fp_poly_t *, fp_field_t *);
fp_poly_error_t fp_poly_div(fp_poly_t **, fp_poly_t **, fp_poly_t *, fp_poly_t *, fp_field_t *);
fp_poly_error_t fp_poly_gcd(fp_poly_t **, fp_poly_t *, fp_poly_t *, fp_field_t *);
fp_poly_error_t fp_poly_gcd_extended(fp_poly_t **, fp_poly_t **, fp_poly_t **, fp_poly_t *, fp_poly_t *, fp_field_t *);

fp_field_t *fp_poly_init_prime_field(uint8_t);
fp_field_t *fp_poly_init_galois_field(uint8_t, fp_poly_t *);
fp_poly_error_t fp_poly_free_field (fp_field_t *);

#endif //FP_POLY_H