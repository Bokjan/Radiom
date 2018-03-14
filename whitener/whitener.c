#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "radiom/whitener.h"

#define RADIOM_KEY_ROUNDS 5

static uint8_t key[32] = 
{
    31, 41, 59, 26, 53, 58, 97, 93, 23, 84, 62, 64, 33, 83, 27, 95,
    2, 88, 41, 97, 16, 93, 99, 37, 51, 5, 82, 9, 74, 94, 45, 92
}; // pi
static uint8_t nonce[8] = 
{
    27, 18, 28, 18, 
    28, 45, 90, 45
}; // e

void radiom_whitener_init(radiom_whitener_t *w)
{
    srand(time(NULL));
    memcpy(w->key, key, sizeof(key));
    memcpy(w->nonce, nonce, sizeof(nonce));
    for(int i = 0; i < RADIOM_KEY_ROUNDS; ++i)
    {
        for(int j = 0; j < 32; ++j)
            w->key[j] ^= rand() & 0xFF;
        for(int j = 0; j < 8; ++j)
            w->nonce[j] ^= rand() & 0xFF;
    }
    ECRYPT_keysetup(&w->x, w->key, 256, 64);
    ECRYPT_ivsetup(&w->x, w->nonce);
}

void radiom_whitener_whiten(radiom_whitener_t *w, uint8_t *in, uint8_t *out, size_t size)
{
    ECRYPT_encrypt_bytes(&w->x, in, out, size);
}