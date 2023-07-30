#include "../../lib/include/fp_integer.h"

int main()
{
    FILE *file = fopen("../../../tests/fp_integer/input_test/random_prime.txt", "w");
    for (uint8_t i = 0; i < 10; i++)
    {
        mpz_t p;
        mpz_init(p);
        random_prime(p, 100);
        fwrite(mpz_get_str(NULL, 10, p), sizeof(char), mpz_sizeinbase(p, 10), file);
        fwrite("\n", sizeof(char), 1, file);
    }
    fclose(file);
    return 0;
}