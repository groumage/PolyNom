#include "client.h"

static void generate_key_pair_integer(key_pair_t *, size_t);
static void generate_key_pair_polynomial(key_pair_t *, size_t);

static void generate_key_pair_integer(key_pair_t *key_pair, size_t key_size)
{
    mpz_t p, q, d, e, n, phi, gcd, foo;
    mpz_inits(p, q, d, e, n, phi, NULL);
    random_prime(p, key_size/2);
    random_prime(q, key_size/2);
    mpz_mul(n, p, q);
    assert (mpz_sizeinbase(n, 2) >= key_size);
    mpz_sub_ui(p, p, 1);
    mpz_sub_ui(q, q, 1);
    mpz_mul(phi, p, q);
    mpz_set_ui(e, 65537);
    extended_euclide_algorithm(phi, e, &foo, &d, &gcd);
    key_pair->public_key = malloc(sizeof(public_key_t));
    key_pair->private_key = malloc(sizeof(private_key_t));
    mpz_init_set(key_pair->public_key->n, n);
    mpz_init_set(key_pair->public_key->e, e);
    mpz_init_set(key_pair->private_key->n, n);
    mpz_init_set(key_pair->private_key->d, d);
    key_pair->is_polynomial = 0;
    mpz_clears(p, q, d, e, n, phi, foo, gcd, NULL);
}

static void generate_key_pair_polynomial(key_pair_t *key_pair, size_t key_size)
{
    return;
}

void generate_key_pair(key_pair_t *key_pair, size_t key_size, uint8_t use_polynom)
{
    if (use_polynom)
        generate_key_pair_polynomial(key_pair, key_size);
    else
        generate_key_pair_integer(key_pair, key_size);
}

void encrypt(mpz_t cipher, char *plain, public_key_t *public_key, uint8_t use_polynom)
{
    if (use_polynom)
        //encrypt_polynom();
        return;
    else
    {
        mpz_t plain_decimal;
        mpz_init(plain_decimal);
        to_decimal(plain_decimal, plain);
        encrypt_integer(cipher, plain_decimal, public_key->e, public_key->n);
        mpz_clear(plain_decimal);
    }
}

void decrypt(char *plain, mpz_t cipher, private_key_t *private_key, uint8_t use_polynom)
{
    if (use_polynom)
        //decrypt_polynom();
        return;
    else
    {
        mpz_t plain_decimal;
        mpz_init(plain_decimal);
        decrypt_integer(plain_decimal, cipher, private_key->d, private_key->n);
        to_string(plain_decimal, plain);
        mpz_clear(plain_decimal);
    }
}

void print_public_key(public_key_t *public_key)
{
    printf("Public key is (e, n) = (");
    mpz_out_str(stdout, 10, public_key->e);
    printf(", ");
    mpz_out_str(stdout, 10, public_key->n);
    printf(")\n");
}

void print_private_key(private_key_t *private_key)
{
    printf("Private key is (d, n) = (");
    mpz_out_str(stdout, 10, private_key->d);
    printf(", ");
    mpz_out_str(stdout, 10, private_key->n);
    printf(")\n");
}

void print_key_pair(key_pair_t *key_pair)
{
    print_public_key(key_pair->public_key);
    print_private_key(key_pair->private_key);
}

void free_key_pair(key_pair_t *key_pair)
{
    mpz_clears(key_pair->public_key->n, key_pair->public_key->e, NULL);
    mpz_clears(key_pair->private_key->n, key_pair->private_key->d, NULL);
    free(key_pair->public_key);
    free(key_pair->private_key);
}