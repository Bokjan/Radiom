#ifndef __RADIOM_FIFOBUF_H__
#define __RADIOM_FIFOBUF_H__

#include <stdint.h>
#include "defines.h"

struct radiom_fifobuf
{
    uint8_t *ptr, *raw;
    size_t head, tail;
};

typedef struct radiom_fifobuf radiom_fifobuf_t;

void radiom_fifobuf_init(radiom_fifobuf_t *fb);
void radiom_fifobuf_free(radiom_fifobuf_t *fb);
size_t radiom_fifobuf_size(radiom_fifobuf_t *fb);
uint8_t* radim_fifobuf_getptr(radiom_fifobuf_t *fb);
int radiom_fifobuf_dequeue(radiom_fifobuf_t *fb, void *dest, size_t size);
int radiom_fifobuf_dequeue2(radiom_fifobuf_t *fb, void *dest, size_t size);
void radiom_fifobuf_setfull(radiom_fifobuf_t *fb);

#endif // __RADIOM_FIFOBUF_H__