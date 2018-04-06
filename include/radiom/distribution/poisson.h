#ifndef __RADIOM_POISSON_DISTRIBUTION_H__
#define __RADIOM_POISSON_DISTRIBUTION_H__

#include <stdint.h>

struct radiom_engine;
typedef struct radiom_engine radiom_engine_t;

int32_t radiom_poisson(double mean);
double radiom_exponential(double lambda);
double radiom_weibull(double shape, double scale);

#endif // __RADIOM_POISSON_DISTRIBUTION_H__