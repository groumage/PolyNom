#include "../../lib/include/fp_poly.h"

int main(int argc, char const *argv[])
{
    FILE *file = fopen("../../../tests/fp_poly/input_test/random_irred_poly.txt", "w");
    for (uint8_t i = 0; i < 10; i++)
    {
        fp_poly_t *p;
        fp_field_t *f;
        f = fp_poly_init_prime_field(2);
        // atoi(argv[1]) == 1 if it is short test
        if (atoi(argv[1]) == 1)
            p = fp_poly_init_random_irreducible(5, f);
        else
            p = fp_poly_init_random_irreducible(10, f);
        fp_poly_print(file, p);
        fwrite("\n", sizeof(char), 1, file);
    }
    fclose(file);
    return 0;
}