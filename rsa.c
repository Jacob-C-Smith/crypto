/** !
 * Implements abstractions for the RSA cryptosystem
 * 
 * @file rsa.c
 * 
 * @author Jacob Smith
 */

// Header file
#include <crypto/rsa.h>

// Function definitions
size_t req_bits ( _BitInt(RSA_KEY_SIZE) a );
int                   print_n_bit_int  ( _BitInt(RSA_KEY_SIZE) a );
_BitInt(RSA_KEY_SIZE) random_n_bit_int ( size_t bits );

// Function definitions
size_t req_bits ( _BitInt(RSA_KEY_SIZE) a )
{

    // Special case
    if ( a == 0 ) return 1;

    // Initialized data
    size_t ret = 0;

    // Right shift until a == 0 
    while(a > 0)
    {
        a >>= 1;
        ret++;
    }

    // Success
    return ret;
}

int print_n_bit_int ( _BitInt(RSA_KEY_SIZE) a )
{ 

    // Initialized data
    size_t bits = 0;
    void *p_int = &a;

    // Compute how many bits are required to represent a
    bits = req_bits(a);

    // Formatting
    putchar('0');
    putchar('x');

    // Print the value
    for (signed i = (signed) (bits >> 5); i >= 0; i--)
        printf("%08x", ((unsigned int *) p_int)[i]);
    
    // Formatting
    putchar('\n');

    // Done
    return 1;
}

_BitInt(RSA_KEY_SIZE) random_n_bit_int ( size_t bits )
{

    // Initialized data
    _BitInt(RSA_KEY_SIZE) r = 0;

    // Generate random integer
    for ( size_t i = 0; i < bits; i++ )
        r = ( r << 1 ) | ( rand() % 2 );
    
    // Done
    return r;
}

_BitInt(RSA_KEY_SIZE) egcd ( _BitInt(RSA_KEY_SIZE) a, _BitInt(RSA_KEY_SIZE) b, _BitInt(RSA_KEY_SIZE) *x, _BitInt(RSA_KEY_SIZE) *y )
{

    // Base case
    if ( b == 0 )
    {

        // Store x
        *x = 1;

        // Store y
        *y = 0;

        // Done
        return a;
    }

    // Initialized data
    _BitInt(RSA_KEY_SIZE) x1, y1;
    _BitInt(RSA_KEY_SIZE) gcd = egcd(b, a % b, &x1, &y1);

    // Store x
    *x = y1;
    
    // Store y
    *y = x1 - ( a / b ) * y1;

    // Done
    return gcd;
}

_BitInt(RSA_KEY_SIZE) mod_exp(_BitInt(RSA_KEY_SIZE) x, _BitInt(RSA_KEY_SIZE) a, _BitInt(RSA_KEY_SIZE) n )
{

    // Base case
    if ( a == 0 )
        
        // Done
        return 1;
    else if ( a % 2 == 1 )
        
        // Done
        return x * mod_exp(x, a-1, n) % n;
    else
    {

        // Initialized data
        _BitInt(RSA_KEY_SIZE) h = mod_exp(x, a >> 1, n);

        // Done
        return h * h % n;
    }
}

int rsa_key_pair_construct ( rsa_key_pair **pp_rsa_key_pair )
{
    
    // Argument check
    if ( pp_rsa_key_pair == (void *) 0 ) goto no_rsa_key_pair;

    // Initialized data
    rsa_key_pair   _rsa_key_pair = { 0 },
                 *p_rsa_key_pair = (void *) 0;

    // Populate the key pair struct
    _rsa_key_pair = (rsa_key_pair) 
    {
        ._private_key = (rsa_private_key)
        {
            .p = 110048843095422197087071757611329503833WB,
            .q = 113176829162524124401163314225963653241WB,
            .b = 0
        },
        ._public_key = (rsa_public_key)
        {
            .n = 0,
            .a = 0
        }
    };

    // Store the product of the prime factors in the public key
    _rsa_key_pair._public_key.n = _rsa_key_pair._private_key.p * _rsa_key_pair._private_key.q;

    // Generate the public exponent A
    {

        // Initialized data
        size_t bits = 0;
        _BitInt(RSA_KEY_SIZE) euler_totient = 0,
                              a             = 0,
                              x             = 0,
                              y             = 0,
                              gcd           = 0;
        
        // Compute Eulers totient function on p and q
        euler_totient = (_rsa_key_pair._private_key.p - 1) * (_rsa_key_pair._private_key.q - 1),

        // Compute how many bits the result of the totient function is
        bits = req_bits(euler_totient);

        // Try 1000 times to generate public exponent A
        for (size_t i = 0; i < 1000; i++)
        {

            // Generate a random number for public exponent A
            a = random_n_bit_int(bits);

            // Fast exit
            if ( a <= 1 || a >= euler_totient ) continue;

            // Compute the GCD of A and the result of the totient function
            gcd = egcd(a, euler_totient, &x, &y);

            // Found a suitable public exponent
            if ( gcd == 1 ) break;
        }
        
        // Store the public exponent
        _rsa_key_pair._public_key.a = a;
    }

    // Compute the private exponent B
    {
        
        // Initialized data
        _BitInt(RSA_KEY_SIZE) phi = 0,
                              gcd = 0,
                              x   = 0,
                              y   = 0;
        
        // Compute Eulers totient function on p and q
        phi = (_rsa_key_pair._private_key.p - 1) * (_rsa_key_pair._private_key.q - 1),

        // Compute the GCD, storing a factor as B
        gcd = egcd(_rsa_key_pair._public_key.a, phi, &_rsa_key_pair._private_key.b, &y);

        // Error check
        if ( ( gcd > 0 ) ? (gcd != 1) : (-gcd != 1) ) goto failed_to_calculate_b;

        // Positive mod
        if ( _rsa_key_pair._private_key.b < 0 ) _rsa_key_pair._private_key.b = _rsa_key_pair._private_key.b + phi;
    }

    // Allocate memory for the key pair
    p_rsa_key_pair = CRYPTO_REALLOC(0, sizeof(rsa_key_pair));

    // TODO: Error check
    if ( p_rsa_key_pair == (void *) 0 ) goto no_mem;

    // Copy the key pair from the stack to the heap
    memcpy(p_rsa_key_pair, &_rsa_key_pair, sizeof(rsa_key_pair));

    // Return a pointer to the caller
    *pp_rsa_key_pair = p_rsa_key_pair;

    // Success
    return 1;

    // Error handling
    {
        
        // Argument errors
        {
            no_rsa_key_pair:
                #ifndef NDEBUG
                    log_error("[crypto] [rsa] Null pointer provided for parameter \"pp_rsa_key_pair\" in call to function \"%s\"\n");
                #endif

                // Error
                return 0;
        }

        // Crypto errors
        {
            failed_to_calculate_b:
                #ifndef NDEBUG
					log_error("[crypto] [rsa] Failed to compute private exponent in call to function \"%s\"\n",__FUNCTION__);
                #endif

                // Error
                return 0;
        }

		// Standard library errors
		{
			no_mem:
				#ifndef NDEBUG
					log_error("[Standard Library] Failed to allocate memory in call to function \"%s\"\n",__FUNCTION__);
				#endif

				// Error
				return 0;
		}
    }
}

int rsa_key_pair_encrypt ( rsa_key_pair *p_rsa_key_pair, void *p_plaintext, void *p_ciphertext, size_t length )
{

    // Argument check
    if ( p_rsa_key_pair == (void *) 0 ) goto no_rsa_key_pair;

    // Initialized data
    _BitInt(RSA_KEY_SIZE) x = 0;
    memcpy(&x, p_plaintext, (RSA_KEY_SIZE/8));
    _BitInt(RSA_KEY_SIZE) e = mod_exp( x, p_rsa_key_pair->_public_key.a, p_rsa_key_pair->_public_key.n);

    // Store 
    *(_BitInt(RSA_KEY_SIZE) *) p_ciphertext = e;

    // Success
    return 1;

    // Error handling
    {
        
        // Argument errors
        {
            no_rsa_key_pair:
                #ifndef NDEBUG
                    log_error("[crypto] [rsa] Null pointer provided for parameter \"p_rsa_key_pair\" in call to function \"%s\"\n");
                #endif

                // Error
                return 0;
        }
    }
}

int rsa_key_pair_decrypt ( rsa_key_pair *p_rsa_key_pair, void *p_ciphertext, void *p_plaintext, size_t length )
{

    // Argument check
    if ( p_rsa_key_pair == (void *) 0 ) goto no_rsa_key_pair;

    // Initialized data
    _BitInt(RSA_KEY_SIZE) e = mod_exp( *(_BitInt(RSA_KEY_SIZE) *) p_ciphertext, p_rsa_key_pair->_private_key.b, p_rsa_key_pair->_public_key.n);

    // Store 
    memcpy(p_plaintext, &e, (RSA_KEY_SIZE/8));

    // Success
    return 1;

    // Error handling
    {
        
        // Argument errors
        {
            no_rsa_key_pair:
                #ifndef NDEBUG
                    log_error("[crypto] [rsa] Null pointer provided for parameter \"p_rsa_key_pair\" in call to function \"%s\"\n");
                #endif

                // Error
                return 0;
        }
    }
}


int rsa_key_pair_info ( rsa_key_pair *p_rsa_key_pair )
{

    // Argument check
    if ( p_rsa_key_pair == (void *) 0 ) goto no_rsa_key_pair;

    // Formatting
    log_info("=== RSA key pair @ %p ===\n", p_rsa_key_pair);

    // Print the private key
    log_info("  Private key:\n");
    printf("    p : "); print_n_bit_int(p_rsa_key_pair->_private_key.p);
    printf("    q : "); print_n_bit_int(p_rsa_key_pair->_private_key.q);
    printf("    b : "); print_n_bit_int(p_rsa_key_pair->_private_key.b);

    // Print the public key
    log_info("  Public key:\n");
    printf("    n : "); print_n_bit_int(p_rsa_key_pair->_public_key.n);
    printf("    a : "); print_n_bit_int(p_rsa_key_pair->_public_key.a);

    // Success
    return 1;

    // Error handling
    {
        
        // Argument errors
        {
            no_rsa_key_pair:
                #ifndef NDEBUG
                    log_error("[crypto] [rsa] Null pointer provided for parameter \"p_rsa_key_pair\" in call to function \"%s\"\n");
                #endif

                // Error
                return 0;
        }
    }
}

int rsa_key_pair_destroy ( rsa_key_pair **pp_rsa_key_pair )
{
    
    // Argument check
    if ( pp_rsa_key_pair == (void *) 0 ) goto no_rsa_key_pair;

    // Initialized data
    rsa_key_pair *p_rsa_key_pair = *pp_rsa_key_pair;

    // No more pointer for caller
    *pp_rsa_key_pair = (void *) 0;

    // NOTE: According to the Department of Defense's standards, this is
    //       how you should wipe data. 

    // !!! WIPE THAT FUCKER CLEAN, JUST LIKE THE DoD TAUGHT ME !!!
    
    // Zero pass
    memset(p_rsa_key_pair, 0x00, sizeof(rsa_key_pair));

    // One pass
    memset(p_rsa_key_pair, 0xFF, sizeof(rsa_key_pair));

    // Random pass
    for (size_t i = 0; i < sizeof(rsa_key_pair); i++)
        ((unsigned char *)p_rsa_key_pair)[i] = rand() % 256;

    // And a little extra couldn't hurt
    memset(p_rsa_key_pair, 0x00, sizeof(rsa_key_pair));

    // Release the memory
    p_rsa_key_pair = CRYPTO_REALLOC(p_rsa_key_pair, 0);

    // Success
    return 1;

    // Error handling
    {
        
        // Argument errors
        {
            no_rsa_key_pair:
                #ifndef NDEBUG
                    log_error("[crypto] [rsa] Null pointer provided for parameter \"pp_rsa_key_pair\" in call to function \"%s\"\n");
                #endif

                // Error
                return 0;
        }
    }
}
