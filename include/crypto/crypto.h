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

// Memory management macro
#ifndef CRYPTO_REALLOC
    #define CRYPTO_REALLOC(p, sz) realloc(p,sz)
    #define CRYPTO_FREE(p) free(p)
#endif