#ifndef CROWDLIB_RANDF_H_
#define CROWDLIB_RANDF_H_
#include <cstdlib>
#include <time.h>
class RandF
{
public:
    RandF()
    {
        srand(time(NULL));
    }
    int randi(int r_low, int r_high, uint _t=0);
    float randf(float r_low, float r_high, uint _t=0);
private:
    int SimpleRandI(int r_low, int r_high);
    float SimpleRandF(float r_low, float r_high);
    uint maxType = 1;
};
#endif //CROWDLIB_RANDF_H_
