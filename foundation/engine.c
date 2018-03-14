#include <stdlib.h>
#include <pthread.h>
#include <rtl-sdr.h>
#include "radiom/engine.h"

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
        radiom_stop(e);
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
    radiom_fifobuf_alloc(&eng->fb);
    eng->is_initialized = true;
}

int radiom_start(radiom_engine_t *eng)
{
    if(eng->is_started)
        return 0;
    pthread_spin_lock(&radiom_engine_lock);
    rtlsdr_open(&eng->dev, 0/*dev_index*/);
    eng->is_started = true;
    // todo!!!!!!!!!!!!!!!!!
    pthread_spin_unlock(&radiom_engine_lock);
    return 0;
}

int radiom_stop(radiom_engine_t *eng)
{
    if(eng == NULL)
        return 0;
    if(!eng->is_started)
        rtlsdr_close(eng->dev);
    if(!eng->is_initialized)
        radiom_fifobuf_free(&eng->fb);
    eng->is_started = false;
    return 0;
}