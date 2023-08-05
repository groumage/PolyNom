//
// Created by guillaume on 27/07/2023.
//

#include "../include/fp_integer.h"

void read_urandom_full(unsigned char *buffer, size_t size)
{
    int fd = open("/dev/urandom", O_RDONLY);
    size_t totalRead = 0;

    if (fd == -1)
    {
        perror("Failed to open /dev/urandom");
        exit(EXIT_FAILURE);
    }

    while (totalRead < size)
    {
        ssize_t bytesRead = read(fd, buffer + totalRead, size - totalRead);
        if (bytesRead <= 0)
        {
            perror("Failed to read from /dev/urandom");
            close(fd);
            exit(EXIT_FAILURE);
        }
        totalRead += bytesRead;
    }

    close(fd);
}

unsigned long int buffer_to_ulong(const unsigned char *buffer, size_t size) {
    if (size > sizeof(unsigned long int))
    {
        fprintf(stderr, "Buffer size exceeds the size of unsigned long int.\n");
        exit(EXIT_FAILURE);
    }

    unsigned long int result = 0;

    for (size_t i = 0; i < size; ++i)
    {
        result <<= 8; // Shift the current result left by 8 bits (1 byte)
        result |= buffer[i]; // Set the least significant byte with the buffer content
    }

    return result;
}

void random_number(mpz_t number, size_t digits)
{
    gmp_randstate_t state;
    unsigned char buffer[8];
    gmp_randinit_default(state);
    read_urandom_full(buffer, 8);
    //printf("%ld\n", sizeof(unsigned long int));
    
    gmp_randseed_ui(state, buffer_to_ulong(buffer, 8));
    //seed_gmp_random(state);
    //mpz_urandomb(number, state, size);
    //gmp_randclear(state);

    mpz_t lowerBound, upperBound;
    mpz_init(lowerBound);
    mpz_init(upperBound);
    mpz_ui_pow_ui(lowerBound, 10, digits - 1); // 10^(digits-1)
    mpz_ui_pow_ui(upperBound, 10, digits);     // 10^digits
    mpz_t range;
    mpz_init(range);
    mpz_sub(range, upperBound, lowerBound);
    mpz_urandomm(number, state, range);
    mpz_add(number, number, lowerBound);
    mpz_clear(lowerBound);
    mpz_clear(upperBound);
    mpz_clear(range);
    gmp_randclear(state);
}

//miller-rabin test for primality
int is_prime(mpz_t n, int k)
{
    if (mpz_cmp_ui(n, 2) < 0)
        return 0;
    if (mpz_cmp_ui(n, 2) == 0)
        return 1;
    if (mpz_even_p(n))
        return 0;
    mpz_t s, d;
    mpz_inits(s, d, NULL);
    mpz_sub_ui(d, n, 1); // n-1 = 2^s * d
    while (mpz_even_p(d))
    {
        mpz_add_ui(s, s, 1);
        mpz_div_ui(d, d, 2);
    }
    gmp_randstate_t state;
    unsigned char buffer[8];
    gmp_randinit_default(state);
    read_urandom_full(buffer, 8);
    gmp_randseed_ui(state, buffer_to_ulong(buffer, 8));
    for (int i = 0; i < k; i++)
    {
        mpz_t a, x, bound, y;
        mpz_inits(a, x, bound, y, NULL);
        mpz_sub_ui(bound, n, 3);
        mpz_urandomm(a, state, bound);
        mpz_add_ui(a, a, 2);
        mpz_powm(x, a, d, n);
        mpz_clears(a, bound, NULL);
        for (size_t j = 0; j < mpz_get_ui(s); j++)
        {
            mpz_t n_minus_1;
            mpz_inits(n_minus_1, NULL);
            mpz_powm_ui(y, x, 2, n);
            mpz_sub_ui(n_minus_1, n, 1);
            if (mpz_cmp_ui(y, 1) == 0 && mpz_cmp_ui(x, 1) != 0 && mpz_cmp(x, n_minus_1) != 0)
            {
                gmp_randclear(state);
                mpz_clears(s, d, x, y, n_minus_1, NULL);
                return 0;
            }
            mpz_clear(n_minus_1);
            mpz_set(x, y);
        }
        if (mpz_cmp_ui(y, 1) != 0)
        {
            mpz_clears(s, d, x, y, NULL);
            gmp_randclear(state);
            return 0;
        }
        mpz_clears(x, y, NULL);
    }
    mpz_clears(s, d, NULL);
    gmp_randclear(state);
    return 1;
}

void random_prime(mpz_t number, size_t digits)
{
    random_number(number, digits);
    while (!is_prime(number, 15))
    {
        random_number(number, digits);
    }
}

void extended_euclide_algorithm(mpz_t a, mpz_t b, mpz_t *u, mpz_t *v, mpz_t *d)
{
    mpz_t old_s, s, old_r, old_t, t, r, t1, t2, t3, q;
    mpz_inits(old_s, s, old_r, old_t, t, r, t1, t2, t3, q, NULL);
    mpz_set(old_r, a);
    mpz_set(r, b);
    mpz_set_ui(old_s, 1);
    mpz_set_ui(s, 0);
    mpz_set_ui(old_t, 0);
    mpz_set_ui(t, 1);
    while (mpz_cmp_ui(r, 0) != 0)
    {
        mpz_fdiv_q(q, old_r, r);
        mpz_set(t1, r);
        mpz_mul(r, q, t1);
        mpz_sub(r, old_r, r);
        mpz_set(old_r, t1);
        mpz_set(t2, s);
        mpz_mul(s, q, t2);
        mpz_sub(s, old_s, s);
        mpz_set(old_s, t2);
        mpz_set(t3, t);
        mpz_mul(t, q, t3);
        mpz_sub(t, old_t, t);
        mpz_set(old_t, t3);
    }
    mpz_init_set(*u, old_s);
    mpz_init_set(*v, old_t);
    mpz_init_set(*d, old_r);
    mpz_clears(old_s, s, old_r, old_t, t, r, t1, t2, t3, q, NULL);
}

void to_decimal(mpz_t str_decimal, char *str)
{
    mpz_init_set_ui(str_decimal, 0);
    for (size_t i = 0; i < strlen(str); i++)
    {
        mpz_mul_ui(str_decimal, str_decimal, 95);
        mpz_add_ui(str_decimal, str_decimal, str[i] - 32);
    }
}

void encrypt_integer(mpz_t cipher, mpz_t plain, mpz_t e, mpz_t n)
{
    mpz_init(cipher);
    mpz_powm(cipher, plain, e, n);
}

void decrypt_integer(mpz_t plain, mpz_t cipher, mpz_t d, mpz_t n)
{
    mpz_init(plain);
    mpz_powm(plain, cipher, d, n);
}

void revstr(char *str)  
{  
    int len, temp;  
    len = strlen(str);

    for (int i = 0; i < len/2; i++)  
    {
        temp = str[i];  
        str[i] = str[len - i - 1];  
        str[len - i - 1] = temp;  
    }  
}  

void to_string(mpz_t msg_decimal, char *msg)
{
    mpz_t q, r, div;
    size_t i = 0;
    mpz_inits(q, r, div, NULL);
    mpz_set_ui(div, 95);
    while (mpz_cmp_ui(msg_decimal, 0) != 0)
    {
        mpz_fdiv_qr(q, r, msg_decimal, div);
        mpz_set(msg_decimal, q);
        msg[i++] = (char) mpz_get_ui(r) + 32;
    }
    mpz_clears(q, r, div, NULL);
    msg[i] = '\0';
    revstr(msg);
}