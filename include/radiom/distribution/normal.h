#ifndef __RADIOM_NORMAL_DISTRIBUTION_H__
#define __RADIOM_NORMAL_DISTRIBUTION_H__

struct radiom_engine;
typedef struct radiom_engine radiom_engine_t;

double radiom_normal(radiom_engine_t *e, double mu, double sigma);
double radiom_lognormal(radiom_engine_t *e, double mu, double sigma);

#endif // __RADIOM_NORMAL_DISTRIBUTION_H__