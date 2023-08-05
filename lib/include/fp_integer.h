//
// Created by guillaume on 27/07/2023.
//

#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <inttypes.h>
#include <assert.h>
#include <string.h>

#define MIN(x,y) (mpz_cmp(x, y) < 0 ? (x) : (y))

void random_prime(mpz_t, size_t);
int is_prime(mpz_t, int);
void extended_euclide_algorithm(mpz_t, mpz_t, mpz_t *, mpz_t *, mpz_t *);
void encrypt_integer(mpz_t, mpz_t, mpz_t, mpz_t);
void decrypt_integer(mpz_t, mpz_t, mpz_t, mpz_t);
void to_decimal(mpz_t, char *);
void to_string(mpz_t, char *);