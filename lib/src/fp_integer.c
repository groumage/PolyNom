#include "../include/fp_integer.h"
#include "../include/util.h"

/**
 * @brief Generate a random number of type <a href="https://gmplib.org/manual/Nomenclature-and-Types">mpz_t</a>.
 * 
 * @param number The pointer to an mpz_t variable where the generated number will be stored.
 * @param digits The number of digits of the random number (in base 10).
 */
static void random_number_mpz(mpz_t number, size_t digits)
{
    gmp_randstate_t state;
    unsigned char buffer[8];
    gmp_randinit_default(state);
    read_urandom_full(buffer, 8);    
    gmp_randseed_ui(state, buffer_to_ulong(buffer, 8));
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

/**
 * @brief Raise a number to a specified power using the naive method.
 * 
 * @param n The number to raise.
 * @param p The power.
 */
uint32_t my_pow(uint32_t n, uint8_t p)
{
    uint32_t res = 1;
    for (size_t i = 0; i < p; i++)
        res *= n;
    return res;
}

/**
 * @brief Check if a number of type <a href="https://gmplib.org/manual/Nomenclature-and-Types">mpz_t</a> is prime using the <a href="https://en.wikipedia.org/wiki/Miller%E2%80%93Rabin_primality_test#Miller%E2%80%93Rabin_test">Miller-Rabin test</a>.
 * 
 * @param n The number to check.
 * @param k The number of iterations in the Miller-Rabin test.
 */
uint8_t is_prime_mpz(mpz_t n, uint8_t k)
{
    if (mpz_cmp_ui(n, 2) < 0)
        return 0;
    if (mpz_cmp_ui(n, 2) == 0)
        return 1;
    if (mpz_cmp_ui(n, 3) == 0)
        return 0;
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
    for (uint8_t i = 0; i < k; i++)
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

/**
 * @brief Check if a number of type uint32_t is prime using the <a href="https://en.wikipedia.org/wiki/Miller%E2%80%93Rabin_primality_test#Miller%E2%80%93Rabin_test">Miller-Rabin test</a>.
 * 
 * @param n The number to check.
 * @param k The number of iterations in the Miller-Rabin test.
 */
uint8_t is_prime(uint32_t n, uint8_t k)
{
    if (n < 2)
        return 0;
    if (n == 2)
        return 1;
    if (n == 3)
        return 0;
    if (n % 2 == 0)
        return 0;
    uint32_t s, d;
    s = 0;
    d = n - 1;
    while (d % 2 == 0)
    {
        s++;
        d /= 2;
    }
    srand(time(NULL));
    for (uint8_t i = 0; i < k; i++)
    {
        uint32_t a, x, y;
        a = rand() % (n - 3) + 2;
        x = my_pow(a, d) % n;
        for (size_t j = 0; j < s; j++)
        {
            y = my_pow(x, 2) % n;
            if (y == 1 && x != 1 && x != n - 1)
                return 0;
            x = y;
        }
        if (y != 1)
            return 0;
    }
    return 1;
}

/**
 * @brief Generate a prime random number of type <a href="https://gmplib.org/manual/Nomenclature-and-Types">mpz_t</a> using the <a href="https://en.wikipedia.org/wiki/Miller%E2%80%93Rabin_primality_test#Miller%E2%80%93Rabin_test">Miller-Rabin test</a>.
 * 
 * @param number The parameter that will store the random number.
 * @param digits The number of digits of the random number.
 * 
 * @note The number of iterations in the Miller-Rabin test is set to 15.
 */
void random_prime_mpz(mpz_t number, size_t digits)
{
    random_number_mpz(number, digits);
    while (!is_prime_mpz(number, 15))
    {
        random_number_mpz(number, digits);
    }
}

/**
 * @brief Compute the greatest common divisor of two numbers of type <a href="https://gmplib.org/manual/Nomenclature-and-Types">mpz_t</a> along with the Bézout coefficients.
 * 
 * @param a The first number.
 * @param b The second number.
 * @param u The first Bézout coefficient.
 * @param v The second Bézout coefficient.
 */
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

/**
 * @brief Convert a string to a decimal number.
 * 
 * @param str_decimal The string converted to a decimal number. 
 * @param str The initial string. 
 * 
 * @note The mpz_t number is initialized in this function. Thus, it needs to be cleared after use.
 */
void to_decimal(mpz_t decimal, char *str)
{
    mpz_init_set_ui(decimal, 0);
    for (size_t i = 0; i < strlen(str); i++)
    {
        mpz_mul_ui(decimal, decimal, 95);
        mpz_add_ui(decimal, decimal, str[i] - 32);
    }
}

/**
 * @brief Encrypt a string using the RSA algorithm.
 *  
 * @param cipher The encrypted string.
 * @param plain The initial string.
 * @param e The public key.
 * @param n The modulus.
 * 
 * @note The mpz_t cipher is initialized in this function. Thus, it needs to be cleared after use.
 * 
 * @code{.c}
int main()
{
    char *msg_sent = "Hello World!";
    char msg_received[strlen(msg_sent) + 1];
    mpz_t plain, cipher, e, n, d;

    mpz_inits(plain, cipher, e, n, d, NULL);
    to_decimal(plain, msg_sent);
    encrypt_integer(cipher, plain, e, n);
    decrypt_integer(plain, cipher, d, n);
    to_string(plain, msg_received);
    mpz_clears(plain, cipher, e, n, d, NULL);
    return 0;
}
 * @endcode
 */
void encrypt_integer(mpz_t cipher, mpz_t plain, mpz_t e, mpz_t n)
{
    mpz_init(cipher);
    mpz_powm(cipher, plain, e, n);
}

/**
 * @brief Decrypt a string using the RSA algorithm.
 * 
 * @param plain The decrypted string.
 * @param cipher The initial string.
 * @param d The private key.
 * @param n The modulus.
 * 
 * @note The mpz_t plain is initialized in this function. Thus, it needs to be cleared after use.
 * 
 * @code{.c}
int main()
{
    char *msg_sent = "Hello World!";
    char msg_received[strlen(msg_sent) + 1];
    mpz_t plain, cipher, e, n, d;

    mpz_inits(plain, cipher, e, n, d, NULL);
    to_decimal(plain, msg_sent);
    encrypt_integer(cipher, plain, e, n);
    decrypt_integer(plain, cipher, d, n);
    to_string(plain, msg_received);
    mpz_clears(plain, cipher, e, n, d, NULL);
    return 0;
}
 * @endcode
 */
void decrypt_integer(mpz_t plain, mpz_t cipher, mpz_t d, mpz_t n)
{
    mpz_init(plain);
    mpz_powm(plain, cipher, d, n);
}

/**
 * @brief Reverse a string.
 * 
 * @param str The string.
 */
void revstr(char *str)  
{
    // TODO: move this functiom to util.c  
    int len, temp;  
    len = strlen(str);

    for (int i = 0; i < len/2; i++)  
    {
        temp = str[i];  
        str[i] = str[len - i - 1];  
        str[len - i - 1] = temp;  
    }  
}  

/**
 * @brief Convert a number to a string.
 * 
 * @param decimal The initial number.
 * @param str The pointer to the string where the number will be stored.
 */
void to_string(mpz_t decimal, char *str)
{
    mpz_t q, r, div;
    size_t i = 0;
    mpz_inits(q, r, div, NULL);
    mpz_set_ui(div, 95);
    while (mpz_cmp_ui(decimal, 0) != 0)
    {
        mpz_fdiv_qr(q, r, decimal, div);
        mpz_set(decimal, q);
        str[i++] = (char) mpz_get_ui(r) + 32;
    }
    mpz_clears(q, r, div, NULL);
    str[i] = '\0';
    revstr(str);
}