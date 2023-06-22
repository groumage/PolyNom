#include "../../fp_poly/include/fp_poly.h"
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
    fp_poly_t *q, *r;
    fp_poly_t *poly[4];
    char line[BUFFER_SIZE];
    FILE *file = fopen(filename, "r");

    fgets(line, sizeof(line), file);
    fp_field_t *field = fp_poly_init_prime_field(atoi(line));
    for (size_t i = 0; i < 4; i++)
    {
        fgets(line, sizeof(line), file);
        remove_last_newline(line);
        poly[i] = fp_poly_parse(line);
    }
    assert (fp_poly_div(&q, &r, poly[0], poly[1], field) == FP_POLY_E_SUCCESS);
    assert (fp_poly_assert_equality(poly[2], q) == FP_POLY_E_SUCCESS);
    assert (fp_poly_assert_equality(poly[3], r) == FP_POLY_E_SUCCESS);
    assert (fp_poly_free(q) == FP_POLY_E_SUCCESS);
    assert (fp_poly_free(r) == FP_POLY_E_SUCCESS);
    for (size_t i = 0; i < 4; i++)
        assert (fp_poly_free(poly[i]) == FP_POLY_E_SUCCESS);
    assert (fp_poly_free_field(field) == FP_POLY_E_SUCCESS);
    fclose(file);
}

int main()
{
    //hello_world_tests();
    pari_gp_test("../../../tests/fp_poly/input_test/test_div.txt");
    return 0;
}