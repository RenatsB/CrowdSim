#ifndef CROWDLIB_AGENT_H_
#define CROWDLIB_AGENT_H_
#include "EnumClasses.h"
#include <memory>
#include "Time.h"
#include "Params.h"
#include "Shop.h"
#include "RandF.h"
class WorldGrid;

class Agent: public Object
{
public:
Agent(std::string _name, WorldGrid* _g, std::shared_ptr<Shop> _s, std::shared_ptr<Time> _t, std::shared_ptr<Params> _p, std::shared_ptr<RandF> _r)
{
    m_grid = _g;
    m_shop = _s;
    m_Time = _t;
    m_Params = _p;
    m_rand = _r;
    initAgent(_name, _g);
}
void initAgent(std::string _name, WorldGrid* _w);
void setCell(Vec2 _id);
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
void takeProduct(std::shared_ptr<Product> _tgt);
void fail();
void giveUp();
void succeed();
void punch();
void charge();
void follow();
void flee();
void navigate(MoveType _m = MoveType::PATH);
void buy();
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
Vec2 m_grabOffset;
Vec2 m_navPoint;
std::vector<Vec2> m_navPath;
Vec2 m_cellID=Vec2(0.f,0.f);
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
