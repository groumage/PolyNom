#include "../../lib/include/fp_poly.h"

#define BUFFER_SIZE 32768

static void remove_last_newline(char* str)
{
    size_t length = strlen(str);
    if (length > 0 && str[length - 1] == '\n')
        str[length - 1] = '\0';
}

static void pari_gp_test(char *filename)
{
    fp_poly_t *poly;
    char line[BUFFER_SIZE];
    FILE *file = fopen(filename, "r");
    for (size_t i = 0; i < 5; i++)
    {
        fgets(line, sizeof(line), file);
        fp_field_t *field = fp_poly_init_prime_field(atoi(line));
        fgets(line, sizeof(line), file);
        remove_last_newline(line);
        poly = fp_poly_parse(line);
        assert (fp_poly_is_irreducible(poly, field) == 1);
        assert (fp_poly_free(poly) == FP_POLY_E_SUCCESS);
        assert (fp_poly_free_field(field) == FP_POLY_E_SUCCESS);
    }
    fclose(file);
}

int main()
{
    pari_gp_test("../../../tests/fp_poly/input_test/test_check_poly_irred.txt");
    return 0;
}