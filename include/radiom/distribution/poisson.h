#ifndef __RADIOM_POISSON_DISTRIBUTION_H__
#define __RADIOM_POISSON_DISTRIBUTION_H__

#include <stdint.h>

struct radiom_engine;
typedef struct radiom_engine radiom_engine_t;

int32_t radiom_poisson(radiom_engine_t *e, double mean);
double radiom_exponential(radiom_engine_t *e, double lambda);
double radiom_weibull(radiom_engine_t *e, double shape, double scale);

#endif // __RADIOM_POISSON_DISTRIBUTION_H__