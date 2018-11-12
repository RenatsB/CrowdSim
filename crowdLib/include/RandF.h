#ifndef RANDF_H_
#define RANDF_H_
#include <cstdlib>
class RandF
{
public:
    int randi(int r_low, int r_high, uint _t=0);
    float randf(float r_low, float r_high, uint _t=0);
private:
    int SimpleRandI(int r_low, int r_high);
    float SimpleRandF(float r_low, float r_high);
    uint maxType = 1;
};
#endif //RANDF_H_
