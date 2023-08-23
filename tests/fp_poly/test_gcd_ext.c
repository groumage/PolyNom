#include "../../lib/include/fp_poly.h"
#include <unistd.h>

#define BUFFER_SIZE 32768

static void remove_last_newline(char* str)
{
    size_t length = strlen(str);
    if (length > 0 && str[length - 1] == '\n')
        str[length - 1] = '\0';
}

static void pari_gp_test(char *filename)
{
    fp_poly_t *res, *u, *v;
    fp_poly_t *poly[5];
    char line[BUFFER_SIZE];
    FILE *file = fopen(filename, "r");

    fgets(line, sizeof(line), file);
    fp_field_t *field = fp_poly_init_prime_field(atoi(line));
    for (size_t i = 0; i < 5; i++)
    {
        fgets(line, sizeof(line), file);
        remove_last_newline(line);
        poly[i] = fp_poly_parse(line);
    }
    assert (fp_poly_gcd_extended(&res, &u, &v, poly[0], poly[1], field) == FP_POLY_E_SUCCESS);
    assert (fp_poly_assert_equality(poly[2], u) == FP_POLY_E_SUCCESS);
    assert (fp_poly_assert_equality(poly[3], v) == FP_POLY_E_SUCCESS);
    assert (fp_poly_assert_equality(poly[4],res) == FP_POLY_E_SUCCESS);
    assert (fp_poly_free(res) == FP_POLY_E_SUCCESS);
    assert (fp_poly_free(u) == FP_POLY_E_SUCCESS);
    assert (fp_poly_free(v) == FP_POLY_E_SUCCESS);
    for (size_t i = 0; i < 5; i++)
        assert (fp_poly_free(poly[i]) == FP_POLY_E_SUCCESS);
    assert (fp_poly_free_field(field) == FP_POLY_E_SUCCESS);
    fclose(file);
}

int main()
{
    //hello_world_tests();
    pari_gp_test("../../../tests/fp_poly/input_test/test_gcd_ext.txt");
    return 0;
}