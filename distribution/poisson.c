#include <math.h>
#include "radiom/engine.h"
#include "radiom/defines.h"
#include "radiom/distribution/uniform.h"

int32_t radiom_poisson(double mean)
{
    int32_t r;
    double limit = exp(-mean);
    double p = radiom_uniform_double();
    for(r = 0; p >= limit; ++r)
        p *= radiom_uniform_double();
    return r;
}

double radiom_exponential(double lambda)
{
    double r = radiom_uniform_double();
    return log(1 - r) / -lambda;
}

double radiom_weibull(double shape, double scale)
{
    double r = -log(radiom_uniform_double());
    r = pow(r, 1.0 / scale);
    return shape * r;
}