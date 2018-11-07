#ifndef AGENT_H_
#define AGENT_H_
#include "Object.h"
#include "EnumClasses.h"
#include <memory>
#include "Time.h"
#include "Params.h"
#include "Product.h"

class Agent: private Object
{
public:
void initAgent(std::string _name);
void setPosition(const Vec2 _pos);
void makeDecision();
void getHit(float _power);
void receiveFail();
float getHealth() const;
float getInfluenceRadius() const;
float getWeight() const;
void update();
private:
void frenzy();
void pickRandomProduct();
void pickRandomAgent();
void pickRandomExit();
void pickRandomEntrance();
void takeProduct(std::shared_ptr<Product> _tgt);
void punch();
void charge();
void follow();
void flee();
void navigate(bool _avoidCollision = false);
void exit();
void finish();
void wait();
void wait(float _seconds);
bool dodont(const float _chancePercent);
void updateInfluences();
int randi(int r_low, int r_high);
float randf(float r_low, float r_high);
//-----------------------------------------------------------------------------------------------------
/// @brief The state of the Agent, as defined in AgentState class (see EnumClasses.h)
//-----------------------------------------------------------------------------------------------------
AgentState m_state = AgentState::IDLE;
//-----------------------------------------------------------------------------------------------------
/// @brief A set of parameters that describe the personality of the Agent
//-----------------------------------------------------------------------------------------------------
float m_aggressiveness;
float m_desire;
float m_power;
float m_resolve;
float m_mentalStability;
float m_desperation;
float m_health;
float m_speed;
std::array<float,8> m_initialValues;
//weight affects the drag when Agents collide
float m_weight;
//-----------------------------------------------------------------------------------------------------
/// @brief Since Agents will be just circles, this variable stores the radius for the circle
//-----------------------------------------------------------------------------------------------------
float m_influenceRadius;
float m_timer=0.f; //used for some timed decisions
Vec2 m_lookVector;
Vec2 m_fleeOrigin;
Vec2 m_navPoint;
std::shared_ptr<Product> m_tgt;
std::shared_ptr<Agent> m_attackTgt;
std::shared_ptr<Time> m_Time;
std::shared_ptr<Params> m_Params;
};
#endif //AGENT_H_
