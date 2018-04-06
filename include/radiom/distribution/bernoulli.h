#ifndef __RADIOM_BERNOULLI_DISTRIBUTION_H__
#define __RADIOM_BERNOULLI_DISTRIBUTION_H__

#include <stdint.h>

int radiom_bernoulli(double probability);
int32_t radiom_binomial(int32_t n, double probability);
int32_t radiom_negative_binomial(int32_t n, double probability);
int32_t radiom_geometric(double probability);

#endif // __RADIOM_BERNOULLI_DISTRIBUTION_H__