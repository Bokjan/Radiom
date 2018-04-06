#include <math.h>
#include <float.h>
#include <stdbool.h>
#include "radiom/engine.h"
#include "radiom/defines.h"
#include "radiom/distribution/normal.h"
#include "radiom/distribution/uniform.h"

// Box-Muller
double radiom_normal(double mean, double stddev)
{
    static const double two_pi = 2.0 * 3.14159265358979323846;

    double u1, u2;
    static double z0, z1;
    static bool generate = false;

    generate = !generate;
    if(!generate)
        return z1 * stddev + mean;
    
    do
    {
        u1 = radiom_uniform_double();
        u2 = radiom_uniform_double();
    }
    while(u1 <= DBL_MIN);

    z0 = sqrt(-2.0 * log(u1)) * cos(two_pi * u2);
    z1 = sqrt(-2.0 * log(u1)) * sin(two_pi * u2);

    return z0 * stddev + mean;
}

double radiom_lognormal(double mean, double stddev)
{
    double r;
    do
    {
        r = radiom_normal(0.0, 1.0);
    }
    while(r > 1.0 || r < -1.0);
    r = r * log(stddev) + log(mean);
    return exp(r);
}