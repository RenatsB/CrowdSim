#include "RandF.h"

int RandF::randi(int r_low, int r_high, uint _t)
{
    switch(_t)
    {
        case 0:
        {
            return randi(r_low, r_high);
        }
        default:
        {
            return 0.f;
        }
    }
}

float RandF::randf(float r_low, float r_high, uint _t)
{
    switch(_t)
    {
        case 0:
        {
            return randf(r_low, r_high);
        }
        default:
        {
            return 0.f;
        }
    }
}


int RandF::SimpleRandI(int r_low, int r_high)
{
    double r = rand()/(1.0 + RAND_MAX);
    int range = r_high - r_low +1;
    int r_scaled = (r * range) + r_low;
    return r_scaled;
}

float RandF::SimpleRandF(float r_low, float r_high)
{
    float r = r_low + (rand()/(RAND_MAX / (r_high-r_low)));
    return r;
}
