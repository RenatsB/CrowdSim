#include "Agent.h"
#include <cstdlib>
#include "WorldGrid.h"
#include <iostream>

void Agent::initAgent(std::string _name, WorldGrid *_w)
{
    m_tag = Tag::AGENT;
    m_name = _name;
    m_grid = _w;
    m_aggressiveness = m_rand.get()->randf(m_Params->aggression_minLvl, m_Params->aggression_maxLvl);
    m_desire = m_rand.get()->randf(m_Params->desire_minLvl, m_Params->desire_maxLvl);
    m_power = m_rand.get()->randf(m_Params->power_minLvl, m_Params->power_maxLvl);
    m_resolve = m_rand.get()->randf(m_Params->resolve_minLvl, m_Params->resolve_maxLvl);
    if(dodont(m_Params->stability_chance)) {m_mentalStability = 1.f;} else
    {m_mentalStability = m_rand.get()->randf(m_Params->stability_minLvl, m_Params->stability_maxLvl);}
    m_desperation = m_rand.get()->randf(m_Params->desire_minLvl, m_Params->desperation_maxLvl);
    m_health = m_rand.get()->randf(m_Params->health_minLvl, m_Params->health_maxLvl);
    m_speed = m_rand.get()->randf(m_Params->speed_minLvl, m_Params->speed_maxLvl);
    m_initialValues = std::array<float,8>{m_aggressiveness,m_desire,m_power,m_resolve,
    m_mentalStability,m_desperation,m_health,m_speed};
    m_weight = m_rand.get()->randf(m_Params.get()->weight_minLvl, m_Params.get()->weight_maxLvl)/m_Params.get()->weight_maxLvl;
}

void Agent::update(std::vector<Agent*> _neighbours)
{
    //first of all, update neighbours
    m_neighbours = _neighbours;
    //then update all the dynamic params
    updateInfluences();
    //then run the logics
    if(m_Time != nullptr)
    {
        if(m_Time.get()->GetTime() >= m_timer)
        {
            switch(m_state)
            {
            case AgentState::FOLLOW : {follow();break;}
            case AgentState::FLEE : {flee();break;}
            case AgentState::CHARGE : {charge();break;}
            case AgentState::ATTACKFRENZY : {frenzy();break;}
            case AgentState::GIVEUP : {flee();break;}
            case AgentState::BUY : {buy();break;}
            case AgentState::FAILED : {fail();break;}
            case AgentState::SUCCESS : {succeed();break;}
            default : {makeDecision();break;}
            }
        }
    }
}

void Agent::takeProduct(std::shared_ptr<Product> _tgt)
{
    _tgt.get()->setOwner(this);
    m_grabOffset = Vec2(_tgt.get()->getPosition()-m_pos);
}

void Agent::makeDecision()
{
    if(m_desire <= m_Params.get()->giveup_desireLvl)
    {
        if(dodont(m_Params.get()->giveup_chance))
        {
            changeState(AgentState::GIVEUP);
        }
        else
        {
            changeState(AgentState::FOLLOW);
        }
    }
    else
    {
        if(m_aggressiveness >= m_Params->frenzy_minAggressionLvl
                || m_mentalStability <= m_Params->frenzy_maxStabilityLvl)
        {
            if(dodont(m_Params->frenzy_chance))
            {
                changeState(AgentState::ATTACKFRENZY);
            }
            else
            {
                changeState(AgentState::FOLLOW);
            }
        }
        else
        {
            if(m_shop.get()->getNumRemainingProducts() == 0)
            {
                changeState(AgentState::FAILED);
            }
            else
            {
                changeState(AgentState::FOLLOW);
            }
        }
    }
}

bool Agent::dodont(const float _chanceZeroToOne)
{
    if(_chanceZeroToOne>=1.f)
        return true;
    if(_chanceZeroToOne<=0.f)
        return false;
    //otherwise do this:
    float value = m_rand.get()->randf(0.f, 1.f);
    if(value<=_chanceZeroToOne){return true;}
    else{return false;}
}

void Agent::getHit(float _power)
{
    //the more instable the Agent is, the less sensitive it will be
    m_health-=_power*m_mentalStability; //take damage
    if(m_health<=0)
    {
        changeState(AgentState::FLEE);
    }
    else
    {m_speed = m_initialValues.at(7)*m_health;}
}

void Agent::receiveFail()
{
    changeState(AgentState::FAILED);
}

void Agent::fail()
{
    if(m_grid->insideRoom(m_pos))
    {
        if(m_navPath.empty())
        {
            pickRandomPtoExit();
        }
        navigate(MoveType::PATH);
    }
    else
    {
        navigate(MoveType::OUT);
    }
}

void Agent::succeed()
{
    navigate(MoveType::OUT);
    m_tgt.get()->setPosition(m_pos+m_grabOffset);
}

void Agent::buy()
{
    if(m_grid->insideRoom(m_pos))
    {
        if(m_navPath.empty())
        {
            pickRandomPtoExit();
        }
        navigate(MoveType::PATH);
        if(m_tgt != nullptr && m_tgt.get()->getOwner() == this)
        {
            m_tgt.get()->setPosition(m_pos+m_grabOffset);
        }
        else
        {
            changeState(AgentState::FOLLOW);
        }
    }
    else
    {
        changeState(AgentState::SUCCESS);
        navigate(MoveType::OUT);
        m_tgt.get()->setPosition(m_pos+m_grabOffset);
    }

}

void Agent::wait()
{
    return;
}

void Agent::wait(float _seconds)
{
    m_timer = m_Time.get()->GetTime()+_seconds;
}

void Agent::punch()
{
    if(m_attackTgt != nullptr)
    {
        if(m_pos.distance(m_attackTgt->getPosition()) <= m_influenceRadius*3.f)
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
                    changeState(AgentState::IDLE);
                    wait(2.f);
                }
            }
        }
    }
    else
    {
        pickClosestAgent();
        punch();
    }
}

void Agent::frenzy()
{
    if(m_tgt == nullptr || m_tgt.get()->getTag()!=Tag::AGENT)
    {
        m_tgt = nullptr;
        pickClosestAgent();
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
                changeState(AgentState::IDLE);
                wait(3.f);
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
            changeState(AgentState::FLEE);
            pickRandomPtoExit();
            wait(4.f);
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
            changeState(AgentState::FOLLOW);
            wait(1.5f);
        }
        else
        {
            m_lookVector = m_tgt.get()->getPosition()-m_pos;
            float tgtDist = m_lookVector.magnitude();
            float coefficient = 1.f;
            float minDist = m_influenceRadius*m_Params->charge_brakeInflRadiusCoefficient;
            if(tgtDist <= minDist)
            {
                if(tgtDist<=m_influenceRadius*1.1f)
                {
                    changeState(AgentState::BUY);
                    takeProduct(m_tgt);
                }
                coefficient = tgtDist/minDist;
            }
            m_speed = (m_initialValues.at(7)*(m_health/m_initialValues.at(6))
                     +m_initialValues.at(7)*(m_health/m_initialValues.at(6))
                     *0.5f*(m_aggressiveness+m_desire+m_desperation))*coefficient;
            m_power -= m_Params->charge_powerDraw;
            navigate(MoveType::TARGET);
        }
    }
}

void Agent::flee()
{
    m_speed = m_initialValues.at(7)*(m_health/m_initialValues.at(6));

    if(m_grid->insideRoom(m_pos))
    {
        if(m_navPath.empty())
        {
            pickRandomPtoExit();
        }
        navigate(MoveType::PATH);
    }
    else
    {
        navigate(MoveType::OUT);
    }
}

void Agent::follow()
{
    if(m_tgt == nullptr)
    {
        pickRandomProduct();
        if(m_tgt == nullptr)
        {
            changeState(AgentState::FAILED);
            wait(1.5f);
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
        if(m_grid->insideRoom(m_pos))
        {
            navigate(MoveType::TARGET);
            if(m_pos.distance(m_tgt.get()->getPosition()) <= m_influenceRadius*1.5f)
            {
                changeState(AgentState::BUY);
                takeProduct(m_tgt);
                pickRandomPtoExit();
            }
        }
        else
        {
            if(m_navPath.empty())
            {
                pickRandomPtoEntrance();
                if(m_navPath.empty() || m_navPath.size() == 1)
                {
                    navigate(MoveType::TARGET);
                }
            }
            navigate(MoveType::PATH);
        }

    }
}

void Agent::navigate(MoveType _m)
{
    switch(_m)
    {
    case MoveType::PATH :
    {
        if(!m_navPath.empty())
        {
            if(m_navPoint==m_navPath.at(0))
            {
                if(m_pos.distance(m_navPoint)<=25.f*m_influenceRadius)
                {
                    if(m_navPath.size()<2)
                    {
                        m_navPath.clear();
                    }
                    else
                    {
                        m_navPoint = m_grid->cellAt(m_navPath.front())->m_position;
                        m_navPath.erase(m_navPath.begin());
                    }
                }
            }
            else
            {
                m_navPoint = m_grid->cellAt(m_navPath.at(0))->m_position;
            }
        }
        else
        {
            if(m_pos.distance(m_navPoint)<=5.f*m_influenceRadius)
            {
                m_state = AgentState::IDLE;
            }
        }
        m_lookVector = m_navPoint - m_pos;
        break;
    }
    case MoveType::TARGET :
    {
        m_lookVector = m_tgt.get()->getPosition() - m_pos;
        break;
    }
    case MoveType::OUT :
    {
        m_lookVector = m_pos - m_grid->getGridMidpoint();
        break;
    }
    case MoveType::CUSTOM :
    {
        m_lookVector = m_navPoint - m_pos;
        break;
    }
    }

    /*switch(m_state)
    {
    case AgentState::CHARGE :
    {
        std::cout<<m_name<<" is Charging"<<std::endl;
        break;
    }
    case AgentState::FAILED :
    {
        std::cout<<m_name<<" has Failed"<<std::endl;
        break;
    }
    case AgentState::FLEE :
    {
        std::cout<<m_name<<" is Fleeing"<<std::endl;
        break;
    }
    case AgentState::FOLLOW :
    {
        std::cout<<m_name<<" is Following in ";
        if(_m == MoveType::PATH)
            std::cout<<"PATH";
        if(_m == MoveType::TARGET)
            std::cout<<"TARGET";
        if(_m == MoveType::OUT)
            std::cout<<"OUT";
        if(_m == MoveType::CUSTOM)
            std::cout<<"CUSTOM";
        std::cout<<" mode"<<std::endl;
        break;
    }
    case AgentState::GIVEUP :
    {
        //std::cout<<m_name<<" has given up"<<std::endl;
        break;
    }
    case AgentState::BUY :
    {
        //std::cout<<m_name<<" is buying"<<std::endl;
        break;
    }
    case AgentState::SUCCESS :
    {
        //std::cout<<m_name<<" is succeeding"<<std::endl;
        break;
    }
    default : {break;}
    }*/
    m_pos += m_drag;
    m_drag = Vec2(0.f,0.f);
    m_lookVector.normalize();
    m_pos += m_lookVector*m_speed*m_Time.get()->DeltaTime();
}

void Agent::updateInfluences()
{
    float maxTime = m_Params.get()->time_saleStart+m_Params.get()->time_saleDuration;
    float timeleft = maxTime-m_Time.get()->GetTime();

    if(timeleft <= 0.f)
    {
        receiveFail();
        return;
    }
    //GET REMAINING TIME STRESS
    float timeStress = 1.f-timeleft/maxTime;
    //GET REMAINING PRODUCTS STRESS
    float remainingProductsStress = m_shop.get()->getRemainingProductStress();
    //GET PRODUCT DISTANCE STRESS IF TOO LITTLE LEFT
    float distanceStress = m_shop.get()->getDistanceStress();
    float stress = std::max(timeStress,remainingProductsStress*distanceStress);

    m_desperation = m_initialValues.at(5) + (m_Params.get()->desperation_maxLvl-m_initialValues.at(5))*stress;
    m_resolve = m_initialValues.at(3) + (m_Params.get()->resolve_maxLvl-m_initialValues.at(3))*stress;
    m_influenceRadius = (m_weight/(m_grid->getKillRadius()))*(((m_health/m_Params.get()->health_maxLvl)
                                      +(m_power/m_Params.get()->power_maxLvl))/2.f);
    //This probably needs explanation:
    //Red channel describes how angry the Agent is. Rule: without energy anger is useless
    //Green channel describes how mentally stable the Agent is. Rule: more desperation->less control
    //Blue channel describes the power state, affected by power of will. Rule: no will->power irrelevant
    //Alpha channel describes how much health the Agent has compared to global Max
    //All 4 channels can be used in combination, but are intended for use as separate colours,
    //each representing its chanel respectively, and other values set to 0
    m_colour = Colour(
                        m_aggressiveness*(m_power/m_initialValues.at(2)),
                        m_mentalStability-m_desperation,
                        (m_power/m_Params.get()->power_maxLvl)*m_desire*m_resolve,
                        m_health/m_Params.get()->health_maxLvl
                      );
}

float Agent::getRadius() const
{
    return m_influenceRadius;
}

float Agent::getHealth() const
{
    return m_health;
}

float Agent::getWeight() const
{
    return m_weight;
}

Vec2 Agent::getPosition() const
{
    return m_pos;
}

void Agent::pickRandomProduct()
{
    std::vector<std::shared_ptr<Product>> products;
    std::vector<std::shared_ptr<Product>> freeProducts;
    products = m_shop.get()->getRemainingProducts();
    freeProducts = m_shop.get()->getfreeProducts();

    if(!freeProducts.empty())
    {
        int randNum = m_rand.get()->randi(0,freeProducts.size()-1,0);
        m_tgt = freeProducts.at(randNum);
    }
    else
    {
        int randNum = m_rand.get()->randi(0,products.size()-1,0);
        m_tgt = products.at(randNum);
    }
}

void Agent::pickClosestAgent()
{
    float minDist = m_Params.get()->nav_maxX+m_Params.get()->nav_maxY;
    //reason for above assignment is that nothing goes beyond the space square,
    //which means maximum possible distance is the length of its diagonal.
    //for simplicity we just add X and Y values here, which summ is > than diag.
    Agent* newTgt = nullptr;
    for(auto &a : m_neighbours)
    {
        float dist = getPosition().distance(a->getPosition());
        if(dist < minDist)
        {
            newTgt = a;
            minDist = dist;
        }
    }
    m_attackTgt = newTgt;
}

void Agent::pickRandomPtoExit()
{
    m_navPath.clear();
    std::cout<<m_name<<" picked path to Exit"<<std::endl;
    m_navPath = m_grid->randPathToExit(m_cellID);
}

void Agent::pickRandomPtoEntrance()
{
    m_navPath.clear();
    std::cout<<m_name<<" picked path to Entrance"<<std::endl;
    m_navPath = m_grid->randPathToEntrance(m_cellID);
}

void Agent::setDrag(float _x, float _y)
{
    m_drag.x = _x;
    m_drag.y = _y;
}

void Agent::setCell(Vec2 _id)
{
    m_cellID = _id;
}

void Agent::changeState(AgentState _s)
{
    m_state = _s;
    m_navPath.clear();
}



























