#include "../../lib/include/fp_integer.h"

#define BUFFER_SIZE 256

static void remove_last_newline(char* str)
{
    size_t length = strlen(str);
    if (length > 0 && str[length - 1] == '\n')
        str[length - 1] = '\0';
}

int main()
{
    FILE *file = fopen("../../../tests/fp_integer/input_test/test_gcd_ext.txt", "r");
    mpz_t a, b, expected_u, expected_v, expected_d, actual_u, actual_v, actual_d;
    mpz_inits(a, b, expected_u, expected_v, expected_d, NULL);
    char line[BUFFER_SIZE];
    fgets(line, sizeof(line), file);
    remove_last_newline(line);
    mpz_set_str(a, line, 10);
    
    fgets(line, sizeof(line), file);
    remove_last_newline(line);
    mpz_set_str(b, line, 10);
    
    fgets(line, sizeof(line), file);
    remove_last_newline(line);
    mpz_set_str(expected_u, line, 10);
    
    fgets(line, sizeof(line), file);
    remove_last_newline(line);
    mpz_set_str(expected_v, line, 10);
    
    fgets(line, sizeof(line), file);
    remove_last_newline(line);
    mpz_set_str(expected_d, line, 10);
    
    fprintf(stderr, "Test gcd\n");
    fprintf(stderr, "a = ");
    mpz_out_str(stderr, 10, a);
    fprintf(stderr, "\n");
    fprintf(stderr, "b = ");
    mpz_out_str(stderr, 10, b);
    fprintf(stderr, "\n");
    fprintf(stderr, "expected_u = ");
    mpz_out_str(stderr, 10, expected_u);
    fprintf(stderr, "\n");
    fprintf(stderr, "expected_v = ");
    mpz_out_str(stderr, 10, expected_v);
    fprintf(stderr, "\n");
    fprintf(stderr, "expected_d = ");
    mpz_out_str(stderr, 10, expected_d);
    fprintf(stderr, "\n");
    extended_euclide_algorithm(a, b, &actual_u, &actual_v, &actual_d);
    assert (mpz_cmp(expected_u, actual_u) == 0);
    assert (mpz_cmp(expected_v, actual_v) == 0);
    assert (mpz_cmp(expected_d, actual_d) == 0);
    mpz_clears(a, b, expected_u, expected_v, expected_d, actual_u, actual_v, actual_d, NULL);
    fclose(file);
    return 0;
}