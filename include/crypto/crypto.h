/** !
 * Include header for crypto library
 * 
 * @file crypto/crypto.h 
 * 
 * @author Jacob Smith 
 */

// Include guard
#pragma once

// Includes
#include <stdio.h>
#include <stddef.h>

// Platform dependent macros
#ifdef _WIN64
#define DLLEXPORT extern __declspec(dllexport)
#else
#define DLLEXPORT
#endif

// Type definitions
typedef unsigned long long(*crypto_hash_function_64_t)( const void *const k, size_t l );

// Function declarations 
/** !
 * Hash data using the MurMur64 hash function
 * 
 * @param k pointer to data to be hashed
 * @param l number of bytes to hash
 * 
 * @return the 64-bit hash on success, 0 on error
*/
DLLEXPORT unsigned long long crypto_mmh64 ( const void* const k, size_t l );

/** !
 * TODO: Hash data using the xxHash64 hash function
 * 
 * @param k pointer to data to be hashed
 * @param l number of bytes to hash
 * 
 * @return the 64-bit hash on success, 0 on error
*/
DLLEXPORT unsigned long long crypto_xxh64 ( const void *const k, size_t l ); 