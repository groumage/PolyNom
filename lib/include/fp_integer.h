/**
 * @file fp_integer.h
 * @brief Header of integer manipulation in finite field.
 * @author Guillaume Roumage
 * @date 04/2024
*/

#ifndef INTEGER_H
#define INTEGER_H

#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <assert.h>
#include <string.h>
#include <time.h>

#define MIN(x,y) (mpz_cmp(x, y) < 0 ? (x) : (y))

void random_prime_mpz(mpz_t, size_t);
uint8_t is_prime_mpz(mpz_t, uint8_t);
uint8_t is_prime(uint32_t, uint8_t);
void extended_euclide_algorithm(mpz_t, mpz_t, mpz_t *, mpz_t *, mpz_t *);
void encrypt_integer(mpz_t, mpz_t, mpz_t, mpz_t);
void decrypt_integer(mpz_t, mpz_t, mpz_t, mpz_t);
void to_decimal(mpz_t, char *);
void to_string(mpz_t, char *);
uint32_t my_pow(uint32_t, uint8_t);

#endif //INTEGER_H