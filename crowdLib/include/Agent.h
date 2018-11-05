#ifndef AGENT_H_
#define AGENT_H_
#include "Object.h"
class Agent: private Object
{
public:

private:
unsigned float m_aggressiveness;
unsigned float m_desire;
unsigned float m_power;
unsigned float m_resolve;
unsigned float m_mentalStability;
unsigned float m_desperation;
}
#endif //AGENT_H_
