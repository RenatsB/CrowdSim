#ifndef PARAMS_H_
#define PARAMS_H_
class Params
{
public:

private:
void clampParams();
public:
float power_minLvl = 0.3f; //makes sense to have same as punch power draw,
//but can be smaller in case you want some agents to never punch anyone
float power_maxLvl = 12.5f;
float power_recharge = 0.56f;
float health_minLvl = 20.0f;
float health_maxLvl = 100.f;
float frenzy_stopHPtriggerLvl = 25.f;
float speed_minLvl = 0.1f;
float speed_maxLvl = 2.5f;
float charge_brakeInflRadiusCoefficient = 3.f;
float charge_powerDraw = 0.68f;
float punch_powerDraw = 0.3f;

//NOTE: all following values should be 0-1, or else they will be clamped
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
#endif //PARAMS_H_
