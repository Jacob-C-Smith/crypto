struct big_int_s;
struct rsa_public_key_s;
struct rsa_private_key_s;
struct rsa_key_pair_s;

typedef struct big_int_s         big_int;
typedef struct rsa_public_key_s  rsa_public_key;
typedef struct rsa_private_key_s rsa_private_key;
typedef struct rsa_key_pair_s    rsa_key_pair;

struct big_int_s
{
    int i;
};

struct rsa_public_key_s
{
    big_int *p_n;
    big_int *p_a;
};

struct rsa_private_key_s
{
    big_int *p_p;
    big_int *p_q;
    big_int *p_b;
};

struct rsa_key_pair_s
{
    rsa_public_key  _public_key;
    rsa_private_key _private_key;
};

int rsa_bigint_construct      ( big_int *p_result, int bits );
int rsa_prime_factor_generate ( big_int *p_result, int bits );

int rsa_public_exponent_generator  ( big_int *p_p, big_int *p_q );
int rsa_private_exponent_generator ( big_int *p_a, big_int *p_p, big_int *p_q );

int rsa_encrypt ( int i );
int rsa_decrypt ( int i );

