#include <gmp.h>
#include <assert.h>
#include "lib/include/fp_integer.h"
#include "lib/include/fp_poly.h"

typedef struct private_key_integer_t
{
    mpz_t d;
    mpz_t n;
} private_key_integer_t;

typedef struct public_key_integer_t
{
    mpz_t e;
    mpz_t n;
} public_key_integer_t;

typedef struct private_key_polynomial_t
{
    fp_poly_t *d;
    fp_poly_t *n;
} private_key_polynomial_t;

typedef struct public_key_polynomial_t
{
    fp_poly_t *e;
    fp_poly_t *n;
} public_key_polynomial_t;

typedef struct private_key_pair_t
{
    union {
        private_key_polynomial_t *private_key_polynom;
        private_key_integer_t *private_key_integer;
    } integer_or_polynomial_private_key;
    unsigned int is_polynomial:1;
} private_key_pair_t;

typedef struct public_key_pair_t
{
    union {
        public_key_polynomial_t *public_key_polynom;
        public_key_integer_t *public_key_integer;
    } integer_or_polynomial_public_key;
    unsigned int is_polynomial:1;
} public_key_pair_t;

typedef struct key_pair_t
{
    private_key_pair_t *private_key;
    public_key_pair_t *public_key;
} key_pair_t;

void generate_key_pair(key_pair_t *, size_t, uint8_t);
void encrypt(mpz_t, char *, public_key_pair_t *);
void decrypt(char *, mpz_t, private_key_pair_t *);
void print_key_pair(key_pair_t *key_pair);
void free_key_pair_integer(key_pair_t *);