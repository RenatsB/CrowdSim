#ifndef CROWDLIB_PARAMS_H_
#define CROWDLIB_PARAMS_H_
#include <cmath>

typedef unsigned int uint;

class Params
{
public:
//-----------------------------------------------------------------------------------------------------
/// @brief Custom constructor, that also checks all parameters
//-----------------------------------------------------------------------------------------------------
Params(){clampParams();}
//-----------------------------------------------------------------------------------------------------
/// @brief Default destructor
//-----------------------------------------------------------------------------------------------------
~Params()=default;
private:
//-----------------------------------------------------------------------------------------------------
/// @brief Checks parameters, clamps some and fixes if some others are negative
//-----------------------------------------------------------------------------------------------------
void clampParams()
{
    time_saleDuration=fabs(time_saleDuration);
    time_saleStart=fabs(time_saleStart);

    nav_queryRadius=fabs(nav_queryRadius);
    if(nav_maxNumEntrances==0 && nav_maxNumExits==0 && nav_maxNumInouts==0)
        nav_maxNumInouts=1;
    if(nav_gridSizeX<10 || nav_gridSizeY<10)
    {
        nav_gridSizeX=10;
        nav_gridSizeY=10;
        nav_maxRoomDim=0;
    }
    else
    {
        if(nav_maxRoomDim<6)
            nav_maxRoomDim=0;
    }

    if(entity_numProducts<1)
        entity_numProducts=1;
    if(entity_numAgents<1)
        entity_numAgents=1;

    if(stress_distanceToExits<0)
        stress_distanceToExits=0;

    power_minLvl=fabs(power_minLvl);
    power_maxLvl=fabs(power_maxLvl);
    power_recharge=fabs(power_recharge);
    health_minLvl=fabs(health_minLvl);
    health_maxLvl=fabs(health_maxLvl);
    frenzy_stopHPtriggerLvl=fabs(frenzy_stopHPtriggerLvl);
    speed_minLvl=fabs(speed_minLvl);
    speed_maxLvl=fabs(speed_maxLvl);
    charge_brakeInflRadiusCoefficient=fabs(charge_brakeInflRadiusCoefficient);
    charge_powerDraw=fabs(charge_powerDraw);
    punch_powerDraw=fabs(punch_powerDraw);

    clamp(giveup_chance);
    clamp(giveup_desireLvl);
    clamp(aggression_minLvl);
    clamp(aggression_maxLvl);
    clamp(desire_minLvl);
    clamp(desire_maxLvl);
    clamp(resolve_minLvl);
    clamp(resolve_maxLvl);
    clamp(stability_chance);
    clamp(stability_minLvl);
    clamp(stability_maxLvl);
    clamp(desperation_minLvl);
    clamp(desperation_maxLvl);
    clamp(frenzy_minAggressionLvl);
    clamp(frenzy_maxStabilityLvl);
    clamp(frenzy_chance);
    clamp(frenzy_stopChance);
}
//-----------------------------------------------------------------------------------------------------
/// @brief Clamp a value by reference to 0-1
//-----------------------------------------------------------------------------------------------------
void clamp(float &_p)
{
    if(_p>1.f)
        _p=1.f;
    if(_p<0.f)
        _p=0.f;
}

public:
float time_saleDuration = 180.f; //lets give it 3 minutes by default
float time_saleStart = 7.f; //give it 7 seconds to prepare

float nav_queryRadius = 12.f;
float nav_minX = -100.f;
float nav_maxX = 100.f;
float nav_minY = -100.f;
float nav_maxY = 100.f;
uint nav_gridSizeX = 32; //grid size less than 10 will be clamped
uint nav_gridSizeY = 32;
uint nav_maxRoomDim = 20; //rooms less than size 6 will be set to 0
uint nav_maxNumExits = 3;
uint nav_maxNumEntrances = 3;
uint nav_maxNumInouts = 2; //will be set to 1 if all 3 types of exits/entrances set to 0

uint entity_numProducts = 5;
uint entity_numAgents = 5;

uint stress_remainingProductsForDistanceCheck = 10;
float stress_distanceToExits = 2.f;

float power_minLvl = 0.3f; //makes sense to have same as punch power draw,
//but can be smaller in case you want some agents to never punch anyone
float power_maxLvl = 12.5f;
float power_recharge = 0.56f;
float health_minLvl = 20.0f;
float health_maxLvl = 100.f;
float frenzy_stopHPtriggerLvl = 25.f;
float speed_minLvl = 0.15f;
float speed_maxLvl = 0.45f;
float charge_brakeInflRadiusCoefficient = 3.f;
float charge_powerDraw = 0.68f;
float punch_powerDraw = 0.3f;

//NOTE: all following values should be 0-1, or else they will be clamped
float giveup_desireLvl = 0.15f;
float giveup_chance = 0.35f;
float aggression_minLvl = 0.f;
float aggression_maxLvl = 1.f;
float desire_minLvl = 0.f;
float desire_maxLvl = 1.f;
float resolve_minLvl = 0.1f;
float resolve_maxLvl = 1.f;
float stability_chance = 0.85;
float stability_minLvl = 0.1f;
float stability_maxLvl = 1.f;
float desperation_minLvl = 0.f;
float desperation_maxLvl = 1.f;
float frenzy_minAggressionLvl = 0.85f;
float frenzy_maxStabilityLvl = 0.4f;
float frenzy_chance = 0.2f;
float frenzy_stopChance = 0.35f;
};
#endif //CROWDLIB_PARAMS_H_
