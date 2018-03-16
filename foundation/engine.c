#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <rtl-sdr.h>
#include "radiom/engine.h"
#include "radiom/defines.h"
#include "radiom/fifobuf.h"
#include "radiom/whitener.h"

pthread_spinlock_t radiom_engine_lock;

static void radiom_initialize_device(radiom_engine_t *eng);

void radiom_init(void)
{
    pthread_spin_init(&radiom_engine_lock, PTHREAD_PROCESS_SHARED);
    radiom_initialize_device(radiom_get_engine());
}

void radiom_exit(void)
{
    radiom_engine_t *e = radiom_get_engine();
    if(e->is_started)
        radiom_engine_stop(e);
    pthread_spin_destroy(&radiom_engine_lock);
}

radiom_engine_t* radiom_get_engine(void)
{
    static radiom_engine_t singleton;
    return &singleton;
}

static void radiom_initialize_device(radiom_engine_t *eng)
{
    if(eng->is_initialized)
        return;
    eng->dev = NULL;
    eng->fb = (radiom_fifobuf_t*)malloc(sizeof(radiom_fifobuf_t));
    eng->wtn = (radiom_whitener_t*)malloc(sizeof(radiom_whitener_t));
    radiom_fifobuf_init(eng->fb);
    radiom_whitener_init(eng->wtn);
    eng->frequency = RADIOM_RADIO_FREQUENCY;
    eng->sample_rate = RADIOM_RADIO_SAMPLE_RATE;
    eng->is_initialized = true;
}

static int radiom_engine_start_rtlsdr(radiom_engine_t *eng)
{
    int r;
    r = rtlsdr_open(&eng->dev, 0); // 0 is `device index`
    if(r < 0)
        return RADIOM_ERROR;
    r = rtlsdr_set_sample_rate(eng->dev, eng->sample_rate);
    if(r < 0)
        return RADIOM_ERROR;
    r = rtlsdr_set_tuner_gain_mode(eng->dev, 0); // Auto gain
    if(r < 0)
        return RADIOM_ERROR;
    r = rtlsdr_reset_buffer(eng->dev);
    if(r < 0)
        return RADIOM_ERROR;
    return RADIOM_SUCCESS;
}

int radiom_engine_start(radiom_engine_t *eng)
{
    int r = RADIOM_SUCCESS;
    if(eng->is_started)
        return RADIOM_SUCCESS;
    pthread_spin_lock(&radiom_engine_lock);
    r = radiom_engine_start_rtlsdr(eng);
    if(r >= 0)
        eng->is_started = true;
    pthread_spin_unlock(&radiom_engine_lock);
    return r;
}

int radiom_engine_stop(radiom_engine_t *eng)
{
    if(eng == NULL)
        return RADIOM_SUCCESS;
    if(!eng->is_started)
        rtlsdr_close(eng->dev);
    if(!eng->is_initialized)
    {
        radiom_fifobuf_free(eng->fb);
        free(eng->fb);
        free(eng->wtn);
    }
    eng->is_started = false;
    return RADIOM_SUCCESS;
}

int radiom_engine_getbytes(radiom_engine_t *eng, void *dest, size_t count)
{
    int r;
    size_t n, fbsize, copied = 0;
    while(count > 0)
    {
        fbsize = radiom_fifobuf_size(eng->fb);
        if(count <= fbsize)
        {
            radiom_fifobuf_dequeue(eng->fb, dest + copied, count);
            return RADIOM_SUCCESS;
        }
        if(fbsize == 0)
        {
            r = radiom_engine_refresh_buffer(eng);
            if(r == RADIOM_ERROR)
            {
                fputs("Radiom: RTLSDR device read failed!", stderr);
                exit(-1);
            }
            fbsize = radiom_fifobuf_size(eng->fb);
        }
        n = (count <= fbsize ? count : fbsize);
        radiom_fifobuf_dequeue(eng->fb, dest + copied, n);
        count -= n;
        copied += n;
    }
    return RADIOM_SUCCESS;
}

int radiom_engine_entropy_bytes(radiom_engine_t *eng, void *dest, size_t count)
{
    int r;
    size_t n, fbsize, copied = 0;
    while(count > 0)
    {
        fbsize = radiom_fifobuf_size(eng->fb);
        if(count <= fbsize)
        {
            radiom_fifobuf_dequeue2(eng->fb, dest + copied, count);
            return RADIOM_SUCCESS;
        }
        if(fbsize == 0)
        {
            r = radiom_engine_refresh_buffer(eng);
            if(r == RADIOM_ERROR)
            {
                fputs("Radiom: RTLSDR device read failed!", stderr);
                exit(-1);
            }
            fbsize = radiom_fifobuf_size(eng->fb);
        }
        n = (count <= fbsize ? count : fbsize);
        radiom_fifobuf_dequeue2(eng->fb, dest + copied, n);
        count -= n;
        copied += n;
    }
    return RADIOM_SUCCESS;
}

int radiom_engine_refresh_buffer(radiom_engine_t *eng)
{
    int r, c;
    r = rtlsdr_read_sync(eng->dev, eng->fb->raw, RADIOM_FIFOBUF_CAPACITY, &c);
    if(r < 0)
        return RADIOM_ERROR;
    radiom_whitener_whiten(eng->wtn, eng->fb->raw, 
                           eng->fb->ptr, RADIOM_FIFOBUF_CAPACITY);
    radiom_fifobuf_setfull(eng->fb);
    return RADIOM_SUCCESS;
}

void radiom_engine_set_freq(radiom_engine_t *eng, int frequency)
{
    eng->frequency = frequency;
}