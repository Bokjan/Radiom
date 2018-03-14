#ifndef __RADIOM_ENGINE_H__
#define __RADIOM_ENGINE_H__

#include <stdbool.h>
#include "radiom/fifobuf.h"

void radiom_init(void); // Run this first!
void radiom_exit(void);

struct rtlsdr_dev_t;

struct radiom_engine
{
    bool is_started;
    bool is_initialized;

    rtlsdr_dev_t *dev;
    radiom_fifobuf_t fb;
};

typedef struct radiom_engine radiom_engine_t;

radiom_engine_t* radiom_get_engine(void);
int radiom_start(radiom_engine_t *eng);
int radiom_stop(radiom_engine_t *eng);


#endif // __RADIOM_ENGINE_H__