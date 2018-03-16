#ifndef __RADIOM_BERNOULLI_DISTRIBUTION_H__
#define __RADIOM_BERNOULLI_DISTRIBUTION_H__

#include <stdint.h>

struct radiom_engine;
typedef struct radiom_engine radiom_engine_t;

int radiom_bernoulli(radiom_engine_t *e, double probability);
int32_t radiom_binomial(radiom_engine_t *e, int32_t n, double probability);
int32_t radiom_negative_binomial(radiom_engine_t *e, int32_t n, double probability);
int32_t radiom_geometric(radiom_engine_t *e, double probability);

#endif // __RADIOM_BERNOULLI_DISTRIBUTION_H__