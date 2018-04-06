#ifndef __RADIOM_ENGINE_H__
#define __RADIOM_ENGINE_H__

#include <stddef.h>
#include <stdbool.h>

void radiom_init(void); // Run this first!
void radiom_exit(void);

struct rtlsdr_dev;
struct radiom_fifobuf;
struct radiom_whitener;

struct radiom_engine
{
    bool is_started;
    bool is_initialized;

    int frequency;
    int sample_rate;

    struct rtlsdr_dev *dev;
    struct radiom_fifobuf *fb;
    struct radiom_whitener *wtn;
};

typedef struct radiom_engine radiom_engine_t;

radiom_engine_t* radiom_get_engine(void);
int radiom_engine_start(void);
int radiom_engine_stop(void); 
int radiom_engine_getbytes(void *dest, size_t count);
int radiom_engine_entropy_bytes(void *dest, size_t count);
int radiom_engine_refresh_buffer(void);
void radiom_engine_set_freq(int frequency); // Hz

#endif // __RADIOM_ENGINE_H__