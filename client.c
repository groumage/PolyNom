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
    key_pair->public_key = malloc(sizeof(public_key_pair_t));
    key_pair->public_key->integer_or_polynomial_public_key.public_key_integer = malloc(sizeof(public_key_integer_t));
    key_pair->private_key = malloc(sizeof(private_key_pair_t));
    key_pair->private_key->integer_or_polynomial_private_key.private_key_integer = malloc(sizeof(private_key_integer_t));
    mpz_init_set(key_pair->public_key->integer_or_polynomial_public_key.public_key_integer->n, n);
    mpz_init_set(key_pair->public_key->integer_or_polynomial_public_key.public_key_integer->e, e);
    mpz_init_set(key_pair->private_key->integer_or_polynomial_private_key.private_key_integer->n, n);
    mpz_init_set(key_pair->private_key->integer_or_polynomial_private_key.private_key_integer->d, d);
    key_pair->public_key->is_polynomial = 0;
    key_pair->private_key->is_polynomial = 0;
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

void encrypt(mpz_t cipher, char *plain, public_key_pair_t *public_key)
{
    if (public_key->is_polynomial)
        //encrypt_polynom();
        return;
    else
    {
        mpz_t plain_decimal;
        mpz_init(plain_decimal);
        to_decimal(plain_decimal, plain);
        encrypt_integer(cipher, plain_decimal, public_key->integer_or_polynomial_public_key.public_key_integer->e, public_key->integer_or_polynomial_public_key.public_key_integer->n);
        mpz_clear(plain_decimal);
    }
}

void decrypt(char *plain, mpz_t cipher, private_key_pair_t *private_key)
{
    if (private_key->is_polynomial)
        //decrypt_polynom();
        return;
    else
    {
        mpz_t plain_decimal;
        mpz_init(plain_decimal);
        decrypt_integer(plain_decimal, cipher, private_key->integer_or_polynomial_private_key.private_key_integer->d, private_key->integer_or_polynomial_private_key.private_key_integer->n);
        to_string(plain_decimal, plain);
        mpz_clear(plain_decimal);
    }
}

static void print_public_key_integer(public_key_integer_t *public_key)
{
    printf("Public key is (e, n) = (");
    mpz_out_str(stdout, 10, public_key->e);
    printf(", ");
    mpz_out_str(stdout, 10, public_key->n);
    printf(")\n");
}

static void print_private_key_integer(private_key_integer_t *private_key)
{
    printf("Private key is (d, n) = (");
    mpz_out_str(stdout, 10, private_key->d);
    printf(", ");
    mpz_out_str(stdout, 10, private_key->n);
    printf(")\n");
}

static void print_public_key_polynom(public_key_polynomial_t *public_key)
{
    printf("Public key is (e, n) = (");
    fp_poly_print(public_key->e);
    printf(", ");
    fp_poly_print(public_key->n);
    printf(")\n");
}

static void print_private_key_polynom(private_key_polynomial_t *private_key)
{
    printf("Public key is (e, n) = (");
    fp_poly_print(private_key->d);
    printf(", ");
    fp_poly_print(private_key->n);
    printf(")\n");
}

void print_key_pair(key_pair_t *key_pair)
{
    if (key_pair->private_key->is_polynomial)
        print_private_key_integer(key_pair->private_key->integer_or_polynomial_private_key.private_key_integer);
    else
        print_private_key_polynom(key_pair->private_key->integer_or_polynomial_private_key.private_key_polynom);
    if (key_pair->public_key->is_polynomial)
        print_public_key_polynom(key_pair->public_key->integer_or_polynomial_public_key.public_key_polynom);
    else
        print_public_key_integer(key_pair->public_key->integer_or_polynomial_public_key.public_key_integer);
}

void free_key_pair_integer(key_pair_t *key_pair)
{
    if (key_pair->private_key->is_polynomial)
        return;
    else
    {
        mpz_clears(key_pair->private_key->integer_or_polynomial_private_key.private_key_integer->d, key_pair->private_key->integer_or_polynomial_private_key.private_key_integer->n, NULL);
        free(key_pair->private_key->integer_or_polynomial_private_key.private_key_integer);
        free(key_pair->private_key);
    }
    if (key_pair->public_key->is_polynomial)
        return;
    else
    {
        mpz_clears(key_pair->public_key->integer_or_polynomial_public_key.public_key_integer->e, key_pair->public_key->integer_or_polynomial_public_key.public_key_integer->n, NULL);
        free(key_pair->public_key->integer_or_polynomial_public_key.public_key_integer);
        free(key_pair->public_key);
    }
}