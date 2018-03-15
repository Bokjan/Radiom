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
    eng->is_initialized = true;
}

int radiom_engine_start(radiom_engine_t *eng)
{
    if(eng->is_started)
        return RADIOM_SUCCESS;
    pthread_spin_lock(&radiom_engine_lock);
    rtlsdr_open(&eng->dev, 0/*dev_index*/);
    eng->is_started = true;
    // todo!!!!!!!!!!!!!!!!!
    pthread_spin_unlock(&radiom_engine_lock);
    return RADIOM_SUCCESS;
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
    // todo: finish
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