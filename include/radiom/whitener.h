#ifndef __RADIOM_WHITENER_H__
#define __RADIOM_WHITENER_H__

#include <stdint.h>
#include "../../whitener/ecrypt-sync.h"

struct radiom_whitener
{
    ECRYPT_ctx x;
    uint8_t key[32], iv[8];
};

typedef struct radiom_whitener radiom_whitener_t;

void radiom_whitener_init(radiom_whitener_t *w);

void radiom_whitener_whiten(radiom_whitener_t *w, uint8_t *in, uint8_t *out, size_t size);

#endif // __RADIOM_WHITENER_H__