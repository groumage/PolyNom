#include <assert.h>
#include <stdio.h>
#include "lib/include/fp_integer.h"
#include "client.h"

#define VERBOSE 0

int main()
{
    char *plain_from_alice = "Hi Bob, want to play with me ?\0";
    char decipher_plain_from_alice[strlen(plain_from_alice) + 1];
    char *plain_from_bob = "Hi Alice, sure !\0";
    char decipher_plain_from_bob[strlen(plain_from_bob) + 1];

    key_pair_t key_pair_alice, key_pair_bob;
    uint8_t alice_use_polynom = 0;
    uint8_t bob_use_polynom = 0;
    mpz_t cipher;

    printf("Alice generates its public and private key...\n");
    generate_key_pair(&key_pair_alice, 100, alice_use_polynom);
    if (VERBOSE)
        print_key_pair(&key_pair_alice);
    
    printf("Bob generates its public and private key...\n");
    generate_key_pair(&key_pair_bob, 100, bob_use_polynom);
    if (VERBOSE)
        print_key_pair(&key_pair_bob);
    
    mpz_init(cipher);
    printf("Alice encrypts a message...\n");
    printf("\tM = %s\n", plain_from_alice);
    encrypt(cipher, plain_from_alice, key_pair_bob.public_key);
    printf("Alice send the encrypted message to Bob...\n");
    printf("Bob decrypts the message...\n");
    decrypt(decipher_plain_from_alice, cipher, key_pair_bob.private_key);
    printf("\tM = %s\n", decipher_plain_from_alice);
    mpz_clear(cipher);

    mpz_init(cipher);
    printf("Bob encrypts a message...\n");
    printf("\tM = %s\n", plain_from_bob);
    encrypt(cipher, plain_from_bob, key_pair_bob.public_key);
    printf("Bob send the encrypted message to Bob...\n");
    printf("Alice decrypts the message...\n");
    decrypt(decipher_plain_from_bob, cipher, key_pair_bob.private_key);
    printf("\tM = %s\n", decipher_plain_from_bob);
    mpz_clear(cipher);

    free_key_pair_integer(&key_pair_alice);
    free_key_pair_integer(&key_pair_bob);
    /*
    alice_use_polynom = 1;
    bob_use_polynom = 1;
    printf("Alice generates its public and private key...\n");
    generate_key_pair(&key_pair_alice, 100, alice_use_polynom);
    if (VERBOSE)
        print_key_pair(&key_pair_alice);
    
    free_key_pair_integer(&key_pair_alice);
    */
    return 0;
}