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
int radiom_engine_start(radiom_engine_t *eng);
int radiom_engine_stop(radiom_engine_t *eng); 
int radiom_engine_getbytes(radiom_engine_t *eng, void *dest, size_t count);
int radiom_engine_entropy_bytes(radiom_engine_t *eng, void *dest, size_t count);
int radiom_engine_refresh_buffer(radiom_engine_t *eng);
void radiom_engine_set_freq(radiom_engine_t *eng, int frequency); // Hz

#endif // __RADIOM_ENGINE_H__