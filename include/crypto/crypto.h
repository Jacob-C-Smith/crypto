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
