/** !
 * Abstractions for the RSA cryptosystem
 * 
 * @file crypto/rsa.h
 * 
 * @author Jacob Smith
 */

// Standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// log module
#include <log/log.h>

// crypto
#include <crypto/crypto.h>

// Preprocessor definitions
#define RSA_KEY_SIZE 2048

struct rsa_public_key_s;
struct rsa_private_key_s;
struct rsa_key_pair_s;

// Type definitions
typedef struct rsa_public_key_s  rsa_public_key;
typedef struct rsa_private_key_s rsa_private_key;
typedef struct rsa_key_pair_s    rsa_key_pair;

// Structure definitions
struct rsa_public_key_s
{
    _BitInt(RSA_KEY_SIZE) n,
                          a;
};

struct rsa_private_key_s
{
    _BitInt(RSA_KEY_SIZE) p,
                          q,
                          b;
};

struct rsa_key_pair_s
{
    rsa_public_key  _public_key;
    rsa_private_key _private_key;
};

// Constructors
/** !
 * Construct a public private key pair
 * 
 * @param pp_rsa_key_pair result
 * 
 * @return 1 on success, 0 on error
 */
DLLEXPORT int rsa_key_pair_construct ( rsa_key_pair **pp_rsa_key_pair );

// Encrypt
DLLEXPORT int rsa_key_pair_encrypt ( rsa_key_pair *p_rsa_key_pair, void *p_plaintext, void *p_ciphertext, size_t length );

// Decrypt
DLLEXPORT int rsa_key_pair_decrypt ( rsa_key_pair *p_rsa_key_pair, void *p_plaintext, void *p_ciphertext, size_t length );

// Info
/** !
 * Print a public private key pair
 * 
 * @param p_rsa_key_pair the key pair
 * 
 * @return 1 on success, 0 on error
 */
DLLEXPORT int rsa_key_pair_info ( rsa_key_pair *p_rsa_key_pair );

// Destructors
/** !
 * (Securely) Destroy a public private key pair
 * 
 * @param pp_rsa_key_pair pointer to key pair pointer
 * 
 * @return 1 on success, 0 on error
 */
DLLEXPORT int rsa_key_pair_destroy ( rsa_key_pair **pp_rsa_key_pair );
