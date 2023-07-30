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
    mpz_init(s);
    mpz_init(d);
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
                mpz_clears(s, d, x, y, n_minus_1, NULL);
                return 0;
            }
            mpz_clear(n_minus_1);
            mpz_set(x, y);
        }
        if (mpz_cmp_ui(y, 1) != 0)
        {
            mpz_clears(s, d, x, y, NULL);
            return 0;
        }
        mpz_clears(x, y, NULL);
    }
    mpz_clears(s, d, NULL);
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