/** ! 
 * Crypto example program
 * 
 * @file main.c
 * 
 * @author Jacob Smith
 */

// Standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>

// crypto
#include <crypto/crypto.h>
#include <crypto/rsa.h>

// Entry point
int main ( int argc, const char *argv[] )
{

    // Initialized data
    rsa_key_pair *p_rsa_key_pair = (void *) 0;
    char _plain   [ (RSA_KEY_SIZE / 8) ] = "Hello Mother Father and Sisters";
    char _cipher  [ (RSA_KEY_SIZE / 8) ] = { 0 };
    char _decrypt [ (RSA_KEY_SIZE / 8) ] = { 0 };
    
    // Seed the RNG
    srand((unsigned int)time(0));

    // Construct the key pair
    rsa_key_pair_construct(&p_rsa_key_pair);

    // Print the key pair
    rsa_key_pair_info(p_rsa_key_pair);

    // Encrypt the plain text
    rsa_key_pair_encrypt(p_rsa_key_pair, &_plain, &_cipher, 0);

    // Decrypt the cipher text
    rsa_key_pair_decrypt(p_rsa_key_pair, &_cipher, &_decrypt, 0);

    // Print the plain text
    log_info("\nPlain text     : ");
    printf("%s\n",  &_plain);
    log_info("Encrypted text : ");
    
    // Cipher
    for (size_t i = 0; i < 32; i++)
        if ( _cipher[i] == 0 ) printf("00"); else printf("%hhx", _cipher[i]);
    
    log_info("\nDecrypted text : ");
    printf("%s\n", &_decrypt);

    // Success
    return EXIT_SUCCESS;
}
