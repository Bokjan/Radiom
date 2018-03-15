#include <malloc.h>
#include <string.h>
#include "radiom/fifobuf.h"

void radiom_fifobuf_init(radiom_fifobuf_t *fb)
{
    fb->head = 0;
    fb->tail = 0;
    fb->ptr = (uint8_t*)malloc(sizeof(uint8_t) * RADIOM_FIFOBUF_CAPACITY * 2);
    fb->raw = fb->ptr + RADIOM_FIFOBUF_CAPACITY;
}

void radiom_fifobuf_free(radiom_fifobuf_t *fb)
{
    fb->head = 0;
    fb->tail = 0;
    free(fb->ptr);
}

size_t radiom_fifobuf_size(radiom_fifobuf_t *fb)
{
    return fb->tail - fb->head;
}

int radiom_fifobuf_deque(radiom_fifobuf_t *fb, void *dest, size_t size)
{
    if(fb->tail - fb->head < size)
        return -1;
    memcpy(dest, fb->ptr + fb->head, size);
    fb->head += size;
    return 0;
}

uint8_t* radim_fifobuf_getptr(radiom_fifobuf_t *fb)
{
    return fb->ptr;
}

void radiom_fifobuf_setfull(radiom_fifobuf_t *fb)
{
    fb->head = 0;
    fb->tail = RADIOM_FIFOBUF_CAPACITY - 1;
}