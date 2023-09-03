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
#include <stddef.h>

// Platform dependent macros
#ifdef _WIN64
#define DLLEXPORT extern __declspec(dllexport)
#else
#define DLLEXPORT
#endif

// Function declarations
/**!
 * Hash l bytes of pointer k using the MurMur64 hash function
 * 
 * @param k pointer to data to be hashed
 * @param l number of bytes to hash
 * 
 * @return the MurMur64 hash
*/
DLLEXPORT unsigned long long crypto_mmh64 ( const void* const k, size_t l )
