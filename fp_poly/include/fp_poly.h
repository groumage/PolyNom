//
// Created by guillaume on 02/10/2021.
//

#ifndef FP_POLY_H
#define FP_POLY_H

#include <inttypes.h>
#include <gmp.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h> 
#include "../../list/include/list.h"

#define FP_VARN 'x'

typedef struct fp_poly_t
{
    mpz_t index_coeff;
    list_t *coeff;
} fp_poly_t;

typedef struct fp_field_t
{
    uint8_t order;
    fp_poly_t *irreducible_polynom;
} fp_field_t;

typedef enum fp_poly_error_e
{
    FP_POLY_E_SUCCESS,
    FP_POLY_E_MALLOC_ERROR,
    FP_POLY_E_POLY_IS_NULL,
    FP_POLY_E_FIELD_IS_NULL,
    FP_POLY_E_LIST_COEFF_IS_NULL,
    FP_POLY_E_REQUESTED_DEGREE_IS_TOO_HIGH,
    FP_POLY_E_COEFF_OVERFLOW,
    FP_POLY_E_COEFF_LESS_THAN_ZERO,
    FP_POLY_E_ASSERT_MPZ_FAILED,
    FP_POLY_E_ASSERT_SIZET_FAILED,
    FP_POLY_E_ASSERT_EQUALITY_FAILED,
} fp_poly_error_t;

fp_poly_t *fp_poly_init(void);
fp_poly_t *fp_poly_init_sizet(size_t, list_t *);
fp_poly_t *fp_poly_init_mpz(mpz_t, list_t *);
fp_poly_t *fp_poly_init_array(uint8_t *, size_t);
fp_poly_t *fp_poly_parse(const char *);
fp_poly_error_t fp_poly_free (fp_poly_t *);
fp_poly_error_t fp_poly_pretty_print(fp_poly_t *);
fp_poly_error_t fp_poly_assert_mpz(fp_poly_t*, mpz_t, list_t *);
fp_poly_error_t fp_poly_assert_sizet(fp_poly_t*, size_t, list_t *);
fp_poly_error_t fp_poly_assert_equality(fp_poly_t *, fp_poly_t *);
fp_poly_error_t fp_poly_print(fp_poly_t *);

size_t fp_poly_degree(fp_poly_t *);
size_t fp_poly_coeff_list_to_degree(fp_poly_t *, size_t);
list_node_t *fp_poly_degree_to_node_list(fp_poly_t *, size_t);

fp_poly_error_t fp_poly_add_single_term(fp_poly_t *, uint8_t, size_t, fp_field_t *);
fp_poly_error_t fp_poly_add(fp_poly_t **, fp_poly_t *, fp_poly_t *, fp_field_t *);
fp_poly_error_t fp_poly_sub_single_term(fp_poly_t *, uint8_t, size_t, fp_field_t *);
fp_poly_error_t fp_poly_sub(fp_poly_t **, fp_poly_t *, fp_poly_t *, fp_field_t *);
//fp_poly_error_t fp_poly_mul_single_term(fp_poly_t *, uint8_t, size_t, fp_field_t *);
fp_poly_error_t fp_poly_mul(fp_poly_t **, fp_poly_t *, fp_poly_t *, fp_field_t *);
fp_poly_error_t fp_poly_div(fp_poly_t **, fp_poly_t **, fp_poly_t *, fp_poly_t *, fp_field_t *);

fp_field_t *fp_poly_init_prime_field(uint8_t);
fp_field_t *fp_poly_init_galois_field(uint8_t, fp_poly_t *);
fp_poly_error_t fp_poly_free_field (fp_field_t *);

/******/

/*
fp_poly_error_t fp_poly_init_from_consts(fp_poly_t **, mpz_t, const uint8_t *, fp_field_t *field);

fp_poly_error_t fp_poly_init_fp (fp_field_t **, uint8_t);

fp_poly_error_t fp_poly_init_fq (fp_field_t **, uint8_t, fp_poly_t **);

fp_poly_error_t fp_poly_add(fp_poly_t **, fp_poly_t *, fp_poly_t *, fp_field_t *);

fp_poly_error_t fp_poly_sub(fp_poly_t **, fp_poly_t *, fp_poly_t *, fp_field_t *);

fp_poly_error_t fp_poly_mul(fp_poly_t **, fp_poly_t *, fp_poly_t *, fp_field_t *);

fp_poly_error_t fp_poly_div(fp_poly_t **, fp_poly_t **, fp_poly_t *, fp_poly_t *, fp_field_t *);

fp_poly_error_t fp_poly_euclide(fp_poly_t **, fp_poly_t *, fp_poly_t *, fp_field_t *);

fp_poly_error_t fp_poly_extended_euclide(fp_poly_t **, fp_poly_t *, fp_poly_t *, fp_poly_t **, fp_poly_t **, fp_field_t *);

fp_poly_error_t fp_poly_mul_fq (fp_poly_t **, fp_poly_t *, fp_poly_t *, fp_field_t *);

fp_poly_error_t fp_poly_inv_fq (fp_poly_t **, fp_poly_t *, fp_field_t *);

fp_poly_error_t fp_poly_div_fq (fp_poly_t **, fp_poly_t *, fp_poly_t *, fp_field_t *);

fp_poly_error_t fp_poly_irreducible_polynom(fp_poly_t **, uint64_t degree, uint8_t characteristic);

uint8_t fp_poly_is_generator (fp_poly_t *, fp_field_t *);

uint8_t fp_poly_is_zero (fp_poly_t *);

//void fp_poly_free (fp_poly_t **);

void fp_poly_free_field (fp_field_t *);

//fp_poly_error_t fp_poly_parse(fp_poly_t **, char *, uint8_t);
*/
#endif //FP_POLY_H
