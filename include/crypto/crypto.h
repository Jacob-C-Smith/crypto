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

// Platform dependent includes
#ifdef _WIN64
    #include <windows.h>
    #include <process.h>
#else
    #include <pthread.h>
    #include <semaphore.h>
    #include <time.h>
#endif

// Platform dependent macros
#ifdef _WIN64
#define DLLEXPORT extern __declspec(dllexport)
#else
#define DLLEXPORT
#endif

// Platform dependent typedefs
#ifdef _WIN64
//WIN
#else
//POSIX
#endif

// Typedefs

// Function declarations
