#ifndef CROWDLIB_AGENT_H_
#define CROWDLIB_AGENT_H_
#include "EnumClasses.h"
#include <memory>
#include "Time.h"
#include "Params.h"
#include "Shop.h"
#include "RandF.h"
class WorldGrid;

class Agent: private Object
{
public:
void initAgent(std::string _name, WorldGrid* _w);
void setPosition(const Vec2 _pos);
void setCell(uint _id);
void setDrag(float _x, float _y);
void makeDecision();
void getHit(float _power);
void receiveFail();
float getHealth() const;
float getWeight() const;
float getRadius() const;
Vec2 getPosition() const;
void update(std::vector<Agent*> _neighbours);
private:
void frenzy();
void pickRandomProduct();
void pickClosestAgent();
void pickRandomPtoExit();
void pickRandomPtoEntrance();
void pickRandomCellToExit();
void pickRandomCellToEntrance();
void takeProduct(std::shared_ptr<Product> _tgt);
void punch();
void charge();
void follow();
void flee();
void navigate(bool _customDir = false);
void enter();
void exit(bool _carryProduct);
void walkaway();
void finish();
void wait();
void wait(float _seconds);
bool dodont(const float _chanceZeroToOne);
void updateInfluences();
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
std::vector<uint> m_navPath;
uint m_cellID =0;
Vec2 m_drag;
WorldGrid* m_grid;
std::shared_ptr<Shop> m_shop;
std::shared_ptr<Product> m_tgt;
Agent* m_attackTgt;
std::shared_ptr<Time> m_Time;
std::shared_ptr<Params> m_Params;
std::shared_ptr<RandF> m_rand;
std::vector<Agent*> m_neighbours;
};
#endif //CROWDLIB_AGENT_H_
