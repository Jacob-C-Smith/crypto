/** !
 * Cross platform cryptography and hashing functions
 * 
 * @file crypto.c 
 * 
 * @author Jacob Smith
 */

// Include 
#include <crypto/crypto.h>

unsigned long long crypto_mmh64 ( const void* const k, size_t l )
{

    // Argument check
    if ( k == (void *) 0 ) goto no_k;
    
    // Constant data
    const unsigned long long m = 0xc6a4a7935bd1e995;
    const int                r = 47;

    // Initialized data
    unsigned long long   h    = 0x41C64E6D ^ (l * m);
    unsigned long long  *data = (unsigned long long*)k;
    unsigned long long  *end  = (l >> 3) + data;
    unsigned char       *d2   = 0;

    // Just go look it up on Wikipedia. I'm not going to pretend I know how this works.
    // https://en.wikipedia.org/wiki/MurmurHash
    while ( data != end )
    {
        unsigned long long k = *data++;

        k *= m;
        k ^= k >> r;
        k *= m;

        h ^= k;
        h *= m;
    }

    d2 = (unsigned char *)data;

    switch ( l & 7 )
    {
        case 7: h ^= (unsigned long long)(d2[6]) << 48;
        case 6: h ^= (unsigned long long)(d2[5]) << 40;
        case 5: h ^= (unsigned long long)(d2[4]) << 32;
        case 4: h ^= (unsigned long long)(d2[3]) << 24;
        case 3: h ^= (unsigned long long)(d2[2]) << 16;
        case 2: h ^= (unsigned long long)(d2[1]) << 8;
        case 1: h ^= (unsigned long long)(d2[0]);
            h *= m;
    }

    h ^= h >> r;
    h *= m;
    h ^= h >> r;

    // Success
    return h;

    // Error handling
    {

        // Argument error
        {
            no_k:
                #ifndef NDEBUG
                    printf("[crypto] Null pointer provided for parameter \"k\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}