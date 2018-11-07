#include "Agent.h"
#include <cstdlib>
void Agent::initAgent(std::string _name)
{
    srand (time(NULL));
    m_tag = Tag::AGENT;
    m_name = _name;
    m_aggressiveness = randf(m_Params->aggression_minLvl, m_Params->aggression_maxLvl);
    m_desire = randf(m_Params->desire_minLvl, m_Params->desire_maxLvl);
    m_power = randf(m_Params->power_minLvl, m_Params->power_maxLvl);
    m_resolve = randf(m_Params->resolve_minLvl, m_Params->resolve_maxLvl);
    if(dodont(m_Params->stability_chance)) {m_mentalStability = 1.f;} else
    {m_mentalStability = randf(m_Params->stability_minLvl, m_Params->stability_maxLvl);}
    m_desperation = randf(m_Params->desire_minLvl, m_Params->desperation_maxLvl);
    m_health = randf(m_Params->health_minLvl, m_Params->health_maxLvl);
    m_speed = randf(m_Params->speed_minLvl, m_Params->speed_maxLvl);
    m_initialValues = std::array<float,8>{m_aggressiveness,m_desire,m_power,m_resolve,
    m_mentalStability,m_desperation,m_health,m_speed};
}
void Agent::update()
{
    switch(m_state)
    {
    case AgentState::FOLLOW : {follow();break;}
    case AgentState::FLEE : {flee();break;}
    case AgentState::CHARGE : {charge();break;}
    case AgentState::ATTACKFRENZY : {frenzy();break;}
    case AgentState::GIVEUP : {giveup();break;}
    case AgentState::EXIT : {exit();break;}
    case AgentState::DONE : {wait();break;}
    case AgentState::FAILED : {wait();break;}
    default : {makeDecision();break;}
    }
}
void Agent::setPosition(const Vec2 _pos)
{
    m_pos = _pos;
}
int Agent::randi(int r_low, int r_high)
{
    double r = rand()/(1.0 + RAND_MAX);
    int range = r_high - r_low +1;
    int r_scaled = (r * range) + r_low;
    return r_scaled;
}
float Agent::randf(float r_low, float r_high)
{
    float r = r_low + (rand()/(RAND_MAX / (r_high-r_low)));
    return r;
}
void Agent::makeDecision()
{
    if(m_aggressiveness >= m_Params->frenzy_minAggressionLvl
            || m_mentalStability <= m_Params->frenzy_maxStabilityLvl)
    {
        if(dodont(m_Params->frenzy_chance))
        {
            m_state = AgentState::ATTACKFRENZY;
            pickRandomAgent();
        }
    }

}

bool Agent::dodont(const float _chancePercent)
{
    if(_chancePercent>=1.f)
        return true;
    if(_chancePercent<=0.f)
        return false;
    //otherwise do this:
    float value = randf(0.f, 1.f);
    if(value<=_chancePercent){return true;}
    else{return false;}
}

void Agent::getHit(float _power)
{
    //the more instable the Agent is, the less sensitive it will be
    m_health-=_power*m_mentalStability; //take damage
    if(m_health<=0)
    {m_fleeOrigin = m_pos; m_state = AgentState::FLEE; flee();}
    else
    {m_speed = m_initialValues.at(7)*m_health;}
}
void Agent::receiveFail()
{
    m_fleeOrigin = m_pos;
    m_state = AgentState::FLEE;
    flee();
}

void Agent::fail()
{
    m_state = AgentState::FAILED;
}

void Agent::punch()
{
    if(m_attackTgt != nullptr)
    {
        if(m_pos.distance(m_attackTgt->m_pos) <= m_influenceRadius*2.f)
        {
            if(m_Params->punch_powerDraw <= m_power)
            {
                m_attackTgt->getHit(m_power*m_aggressiveness+m_power*m_desire*m_resolve*m_desperation);
                m_power -= m_Params->punch_powerDraw;
            }
            else
            {
                if(m_state == AgentState::ATTACKFRENZY)
                {
                    m_state = AgentState::IDLE;
                }
            }
        }
    }
    else
    {
        pickRandomAgent();
        punch();
    }
}

void Agent::frenzy()
{
    if(m_tgt == nullptr)
    {
        pickRandomAgent();
    }
    else
    {
        if(m_attackTgt->getHealth() <= m_Params->frenzy_stopHPtriggerLvl)
        {
            if(!dodont(m_Params->frenzy_stopChance))
            {
                punch();
            }
            else
            {

            }
        }
    }
}

void Agent::charge()
{
    if(m_tgt == nullptr)
    {
        pickRandomProduct();
        if(m_tgt == nullptr)
        {
            m_state = AgentState::FLEE;
            flee();
        }
        else
        {
            charge();
        }
    }
    else
    {
        if(m_power < m_Params->charge_powerDraw)
        {
            m_state = AgentState::FOLLOW;
            follow();
        }
        else
        {
            m_speed = m_initialValues.at(7)*m_health + m_initialValues.at(7)*m_health*0.5f*(m_aggressiveness+m_desire+m_desperation);
            m_lookVector = m_tgt.get()->m_pos-m_pos;
            float tgtDist = m_lookVector.magnitude();
            m_lookVector.normalize();
            float coefficient = 1.f;
            float minDist = m_influenceRadius*m_Params->charge_brakeInflRadiusCoefficient;
            if(tgtDist <= minDist)
            {
                if(tgtDist<=m_influenceRadius*1.1f)
                {
                    m_state = AgentState::EXIT;
                    takeProduct(m_tgt);
                    exit();
                }
                coefficient = tgtDist/minDist;
            }
            navigate();
            m_pos += m_lookVector*coefficient*m_speed*m_power*m_aggressiveness;
            m_power -= m_Params->charge_powerDraw;
        }
    }
}

void Agent::flee()
{
    m_speed = m_initialValues.at(7)*m_health + m_initialValues.at(7)*(m_health/m_initialValues.at(6));
    pickRandomExit();
    //m_lookVector =
    navigate();
    m_pos += m_lookVector*m_speed*m_power*m_aggressiveness;
}

void Agent::follow()
{
    if(m_tgt == nullptr)
    {
        pickRandomProduct();
        if(m_tgt == nullptr)
        {
            m_state = AgentState::FLEE;
            flee();
        }
        else
        {
            follow();
        }
    }
    else
    {
        m_speed = m_initialValues.at(7)*(m_health/m_initialValues.at(6))
                 +m_initialValues.at(7)*(m_desire+m_desperation
                 +m_resolve*m_aggressiveness)*(m_health/m_initialValues.at(6));


    }
}
