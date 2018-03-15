#ifndef __RADIOM_UNIFORM_DISTRIBUTION_H__
#define __RADIOM_UNIFORM_DISTRIBUTION_H__

#include <stdint.h>

struct radiom_engine;
typedef struct radiom_engine radiom_engine_t;

int32_t radiom_uniform_int32(radiom_engine_t *e);
int32_t radiom_uniform_int32_in(radiom_engine_t *e, int32_t min, int32_t max);

int64_t radiom_uniform_int64(radiom_engine_t *e);
int64_t radiom_uniform_int64_in(radiom_engine_t *e, int64_t min, int64_t max);

double radiom_uniform_double(radiom_engine_t *e);

#endif // __RADIOM_UNIFORM_DISTRIBUTION_H__