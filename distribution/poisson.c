#include <math.h>
#include "radiom/engine.h"
#include "radiom/defines.h"
#include "radiom/distribution/uniform.h"

int32_t radiom_poisson(radiom_engine_t *e, double mean)
{
    int32_t r;
    double limit = exp(-mean);
    double p = radiom_uniform_double(e);
    for(r = 0; p >= limit; ++r)
        p *= radiom_uniform_double(e);
    return r;
}

double radiom_exponential(radiom_engine_t *e, double lambda)
{
    double r = radiom_uniform_double(e);
    return log(1 - r) / -lambda;
}

double radiom_weibull(radiom_engine_t *e, double shape, double scale)
{
    double r = -log(radiom_uniform_double(e));
    r = pow(r, 1.0 / scale);
    return shape * r;
}