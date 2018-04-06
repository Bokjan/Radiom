#include "radiom/engine.h"
#include "radiom/defines.h"
#include "radiom/distribution/uniform.h"

int32_t radiom_uniform_int32(void)
{
    int32_t r;
    radiom_engine_getbytes(&r, sizeof(int32_t));
    return r & RADIOM_MAX_INT32;
}

int32_t radiom_uniform_int32_in(int32_t min, int32_t max)
{
    int32_t r;
    radiom_engine_getbytes(&r, sizeof(int32_t));
    r &= RADIOM_MAX_INT32;
    double ratio = (double)r / RADIOM_MAX_INT32;
    return min + (int32_t)(ratio * (max - min + 1));
}


int64_t radiom_uniform_int64(void)
{
    int64_t r;
    radiom_engine_getbytes(&r, sizeof(int64_t));
    return r & RADIOM_MAX_INT64;
}

int64_t radiom_uniform_int64_in(int64_t min, int64_t max)
{
    int64_t r;
    radiom_engine_getbytes(&r, sizeof(int64_t));
    r &= RADIOM_MAX_INT64;
    double ratio = (long double)r / RADIOM_MAX_INT64;
    return min + (int64_t)(ratio * (max - min + 1));
}

double radiom_uniform_double(void)
{
    int32_t r;
    radiom_engine_getbytes(&r, sizeof(int32_t));
    r &= RADIOM_MAX_INT32;
    return (double)r / RADIOM_MAX_INT32;
}