#include <stdio.h>
#include "lib/include/fp_integer.h"

int main()
{
    //mpz_t p, q;
    mpz_t p;
    mpz_init(p);
    //mpz_init(q);
    random_prime(p, 249);
    mpz_out_str(stdout, 10, p);
    //printf("\n");
    //random_number(q, 500);
    //mpz_out_str(stdout, 10, q);
    mpz_clear(p);
    //mpz_clear(q);
    
    //mpz_t n;
    //mpz_init_set_str(n, "7828066842200037202752552295384488451591151725510233824351302369422008040573905046003673929589742143", 10);
    //printf("%d\n", is_prime(n, 15));
    //printf("%d\n", mpz_probab_prime_p(n, 15));
    return 0;
}