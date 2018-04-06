#ifndef __RADIOM_UNIFORM_DISTRIBUTION_H__
#define __RADIOM_UNIFORM_DISTRIBUTION_H__

#include <stdint.h>

int32_t radiom_uniform_int32(void);
int32_t radiom_uniform_int32_in(int32_t min, int32_t max);

int64_t radiom_uniform_int64(void);
int64_t radiom_uniform_int64_in(int64_t min, int64_t max);

double radiom_uniform_double(void);

#endif // __RADIOM_UNIFORM_DISTRIBUTION_H__