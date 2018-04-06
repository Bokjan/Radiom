#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <rtl-sdr.h>
#include "radiom/engine.h"
#include "radiom/defines.h"
#include "radiom/fifobuf.h"
#include "radiom/whitener.h"

pthread_spinlock_t radiom_engine_lock;
radiom_engine_t radiom_engine_instance;

static void radiom_initialize_device(void);

void radiom_init(void)
{
    pthread_spin_init(&radiom_engine_lock, PTHREAD_PROCESS_SHARED);
    radiom_initialize_device();
}

void radiom_exit(void)
{
    if(e->is_started)
        radiom_engine_stop();
    pthread_spin_destroy(&radiom_engine_lock);
}

radiom_engine_t* radiom_get_engine(void)
{
    static radiom_engine_t singleton;
    return &singleton;
}

static void radiom_initialize_device(void)
{
    if(radiom_engine_instance.is_initialized)
        return;
    radiom_engine_instance.dev = NULL;
    radiom_engine_instance.fb = (radiom_fifobuf_t*)malloc(sizeof(radiom_fifobuf_t));
    radiom_engine_instance.wtn = (radiom_whitener_t*)malloc(sizeof(radiom_whitener_t));
    radiom_fifobuf_init(radiom_engine_instance.fb);
    radiom_whitener_init(radiom_engine_instance.wtn);
    radiom_engine_instance.frequency = RADIOM_RADIO_FREQUENCY;
    radiom_engine_instance.sample_rate = RADIOM_RADIO_SAMPLE_RATE;
    radiom_engine_instance.is_initialized = true;
}

static int radiom_engine_start_rtlsdr(void)
{
    int r;
    r = rtlsdr_open(&radiom_engine_instance.dev, 0); // 0 is `device index`
    if(r < 0)
        return RADIOM_ERROR;
    r = rtlsdr_set_sample_rate(radiom_engine_instance.dev, radiom_engine_instance.sample_rate);
    if(r < 0)
        return RADIOM_ERROR;
    r = rtlsdr_set_tuner_gain_mode(radiom_engine_instance.dev, 0); // Auto gain
    if(r < 0)
        return RADIOM_ERROR;
    r = rtlsdr_reset_buffer(radiom_engine_instance.dev);
    if(r < 0)
        return RADIOM_ERROR;
    return RADIOM_SUCCESS;
}

int radiom_engine_start(void)
{
    int r = RADIOM_SUCCESS;
    if(radiom_engine_instance.is_started)
        return RADIOM_SUCCESS;
    pthread_spin_lock(&radiom_engine_lock);
    r = radiom_engine_start_rtlsdr();
    if(r >= 0)
        radiom_engine_instance.is_started = true;
    pthread_spin_unlock(&radiom_engine_lock);
    return r;
}

int radiom_engine_stop(void)
{
    if(eng == NULL)
        return RADIOM_SUCCESS;
    if(!radiom_engine_instance.is_started)
        rtlsdr_close(radiom_engine_instance.dev);
    if(!radiom_engine_instance.is_initialized)
    {
        radiom_fifobuf_free(radiom_engine_instance.fb);
        free(radiom_engine_instance.fb);
        free(radiom_engine_instance.wtn);
    }
    radiom_engine_instance.is_started = false;
    return RADIOM_SUCCESS;
}

int radiom_engine_getbytes(void *dest, size_t count)
{
    int r;
    size_t n, fbsize, copied = 0;
    while(count > 0)
    {
        fbsize = radiom_fifobuf_size(radiom_engine_instance.fb);
        if(count <= fbsize)
        {
            radiom_fifobuf_dequeue(radiom_engine_instance.fb, dest + copied, count);
            return RADIOM_SUCCESS;
        }
        if(fbsize == 0)
        {
            r = radiom_engine_refresh_buffer();
            if(r == RADIOM_ERROR)
            {
                fputs("Radiom: RTLSDR device read failed!", stderr);
                exit(-1);
            }
            fbsize = radiom_fifobuf_size(radiom_engine_instance.fb);
        }
        n = (count <= fbsize ? count : fbsize);
        radiom_fifobuf_dequeue(radiom_engine_instance.fb, dest + copied, n);
        count -= n;
        copied += n;
    }
    return RADIOM_SUCCESS;
}

int radiom_engine_entropy_bytes(void *dest, size_t count)
{
    int r;
    size_t n, fbsize, copied = 0;
    while(count > 0)
    {
        fbsize = radiom_fifobuf_size(radiom_engine_instance.fb);
        if(count <= fbsize)
        {
            radiom_fifobuf_dequeue2(radiom_engine_instance.fb, dest + copied, count);
            return RADIOM_SUCCESS;
        }
        if(fbsize == 0)
        {
            r = radiom_engine_refresh_buffer();
            if(r == RADIOM_ERROR)
            {
                fputs("Radiom: RTLSDR device read failed!\n", stderr);
                exit(-1);
            }
            fbsize = radiom_fifobuf_size(radiom_engine_instance.fb);
        }
        n = (count <= fbsize ? count : fbsize);
        radiom_fifobuf_dequeue2(radiom_engine_instance.fb, dest + copied, n);
        count -= n;
        copied += n;
    }
    return RADIOM_SUCCESS;
}

int radiom_engine_refresh_buffer(void)
{
    int r, c;
    r = rtlsdr_read_sync(radiom_engine_instance.dev, radiom_engine_instance.fb->raw, RADIOM_FIFOBUF_CAPACITY, &c);
    if(r < 0)
        return RADIOM_ERROR;
    radiom_whitener_whiten(radiom_engine_instance.wtn, radiom_engine_instance.fb->raw, 
                           radiom_engine_instance.fb->ptr, RADIOM_FIFOBUF_CAPACITY);
    radiom_fifobuf_setfull(radiom_engine_instance.fb);
    return RADIOM_SUCCESS;
}

void radiom_engine_set_freq(int frequency)
{
    radiom_engine_instance.frequency = frequency;
}