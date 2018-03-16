#include <math.h>
#include "radiom/engine.h"
#include "radiom/defines.h"
#include "radiom/distribution/uniform.h"

int radiom_bernoulli(radiom_engine_t *e, double probability)
{
    return radiom_uniform_double(e) < probability ? 1 : 0;
}

int32_t radiom_binomial(radiom_engine_t *e, int32_t n, double probability)
{
    int32_t r = 0;
    for(int32_t i = 0; i < n; ++i)
        if(radiom_uniform_double(e) < probability)
            ++r;
    return r;
}

int32_t radiom_negative_binomial(radiom_engine_t *e, int32_t n, double probability)
{
    int32_t t = 0, r = 0;
    for( ; ;)
    {
        if(radiom_uniform_double(e) < probability)
            ++r;
        else
            ++t;
        if(t > n)
            break;
    }
    return r;
}

int32_t radiom_geometric(radiom_engine_t *e, double probability)
{
    int32_t r = 0;
    do
        ++r;
    while(radiom_uniform_double(e) < probability);
    return r;
}