#include <gmp.h>
#include <assert.h>
#include "lib/include/fp_integer.h"

typedef struct private_key_t
{
    mpz_t d;
    mpz_t n;
} private_key_t;

typedef struct public_key_t
{
    mpz_t e;
    mpz_t n;
} public_key_t;

typedef struct key_pair_t
{
    private_key_t *private_key;
    public_key_t *public_key;
    unsigned int is_polynomial:1;
} key_pair_t;

void generate_key_pair(key_pair_t *, size_t, uint8_t);
void encrypt(mpz_t, char *, public_key_t *, uint8_t);
void decrypt(char *, mpz_t, private_key_t *, uint8_t);
void print_key_pair(key_pair_t *key_pair);
void print_public_key(public_key_t *public_key);
void print_private_key(private_key_t *private_key);
void free_key_pair(key_pair_t *);