#include "WorldGrid.h"
#include <iostream>

/// @brief destructor
WorldGrid::~WorldGrid()
{
    clearGrid();
    for(auto c : m_cells)
    {
      free(c);
    }
    m_cells.clear();
}

GridCell* WorldGrid::cellAt(Vec2 _cell)
{
    return m_cells.at(_cell.y*m_gridSizeX+_cell.y);
}

uint WorldGrid::getAt(Vec2 _cell)
{
    return m_cells.at(_cell.x+_cell.y*m_gridSizeX)->m_value;
}

uint WorldGrid::getAt(uint _cell)
{
    return m_cells.at(_cell)->m_value;
}

void WorldGrid::update()
{
    for(GridCell* c : m_cells)
    {
        std::cout<<"name boop 1"<<std::endl;
        for(Agent* a : c->m_agentList)
        {
            std::cout<<"name boop 2"<<std::endl;
            a->setCell(c->m_id);
            std::cout<<"name boop 3"<<std::endl;
            a->update(c->m_agentList);
        }
        std::cout<<"name boop 4"<<std::endl;
        checkCollisionOnNode(c);
    }
}

void WorldGrid::addAgent(Agent *_a)
{
    int g = checkGrid(_a->getPosition());
    if(g != -1)
      m_cells[g]->m_agentList.push_back(_a);
}

void WorldGrid::addAgent(Agent *_a, GridCell* _cell)
{
    _cell->m_agentList.push_back(_a);
    _a->setPosition(_cell->m_position);
    _a->setCell(_cell->m_id);
}

void WorldGrid::initGrid()
{
    //m_cellDimX = (m_limit.m_maxx - m_limit.m_minx)/m_cellNum;
    //m_cellDimY = (m_limit.m_maxy - m_limit.m_miny)/m_cellNum;
    for(uint c=0; c<m_cells.size(); ++c)
    {
      m_cells[c] = new GridCell;
      m_cells[c]->m_limit.m_minx = m_limit.m_minx + m_cellDimX*(c%m_gridSizeX);
      m_cells[c]->m_limit.m_maxx = m_limit.m_maxx + m_cellDimX*(c%m_gridSizeX +1);
      m_cells[c]->m_limit.m_miny = m_limit.m_miny + m_cellDimY*(c/m_gridSizeX);
      m_cells[c]->m_limit.m_maxy = m_limit.m_maxy + m_cellDimY*(c/m_gridSizeX +1);
      m_cells[c]->m_position = Vec2((m_cells[c]->m_limit.m_minx+m_cells[c]->m_limit.m_maxx)/2.f,
                                    (m_cells[c]->m_limit.m_miny+m_cells[c]->m_limit.m_maxy)/2.f);
      m_cells[c]->m_id = c;
    }
    m_killRadius = ((m_limit.m_minx+m_limit.m_maxx)/2.f+(m_limit.m_miny+m_limit.m_maxy)/2.f)/2.f;
}

void WorldGrid::initMap()
{

///The map is defined as follows:
/// 0 - empty
/// 1 - wall
/// 2 - entrance
/// 3 - exit
/// 4 - inout

    //init all to 0
    for(uint u=0; u<m_cells.size(); ++u)
        m_cells.at(u)->m_value = 0;

    if(m_params->nav_maxRoomDim>0)
    {
        uint roomSize = m_randF->randi(16, m_params->nav_maxRoomDim,0);

        //make walls
        for(uint y=m_gridSizeY/2-roomSize/2; y<m_gridSizeY/2+roomSize/2; ++y)
        {
            setAt(m_gridSizeX/2-roomSize/2,y,1); //  set  |    |
            setAt(m_gridSizeX/2+roomSize/2,y,1); //       |    |
        }
        for(uint x=m_gridSizeX/2-roomSize/2; x<m_gridSizeX/2-roomSize/2; ++x)
        {
            setAt(x,m_gridSizeY/2-roomSize/2,1); //  set  ------
            setAt(x,m_gridSizeY/2+roomSize/2,1); //       ------
        }
        m_roomLimit.m_minx = cellAt(Vec2(m_gridSizeX/2-roomSize/2,m_gridSizeY/2))->m_limit.m_minx;
        m_roomLimit.m_maxx = cellAt(Vec2(m_gridSizeX/2+roomSize/2,m_gridSizeY/2))->m_limit.m_maxx;
        m_roomLimit.m_miny = cellAt(Vec2(m_gridSizeX/2,m_gridSizeY/2-roomSize/2))->m_limit.m_miny;
        m_roomLimit.m_maxy = cellAt(Vec2(m_gridSizeX/2,m_gridSizeY/2+roomSize/2))->m_limit.m_maxy;

        //make holes
        uint numExits = m_randF->randi(0, m_params->nav_maxNumExits,0);
        uint numEntrances = m_randF->randi(0, m_params->nav_maxNumEntrances,0);
        uint numInouts = m_randF->randi(1, m_params->nav_maxNumInouts,0);
        uint exitSize = (uint)(sqrt(roomSize)/2);
        std::vector<Vec2> takenXY;
        for(uint i=0; i<(numExits+numEntrances+numInouts); ++i)
        {
            Vec2 p;
            std::vector<Vec2> p2;
            int k = m_randF->randi(0, 4, 0);


            bool chk = false;
            do
            {
                chk = false;
                //this creates XY pairs:
                /// rand - 0
                /// rand - 1
                /// o - rand
                /// 1 - rand
                /// in other words, picks horizontal walls or vertical
                /// and attempts to make a hole
                p.x = k%2;
                p.y = k%2;
                if(k>1)
                {
                    p.y=m_randF->randi(1,roomSize-1);
                    //if exit size > 1, need to check if current point is at corners,
                    //if so, instead of setting points at both its sides, just
                    //set points at one of the sides
                    if(p.y >= roomSize-1-exitSize/2 || p.y <= 1+exitSize/2)
                    {
                        for(uint e=1; e<exitSize-1; ++e)
                        {
                            Vec2 a;
                            if(p.y <= 1+exitSize/2)
                            {
                                a=Vec2(p.x, p.y+e);
                            }
                            else
                            {
                                a=Vec2(p.x, p.y-e);
                            }
                            p2.push_back(a);
                        }
                    }
                    else
                    {
                        //if not close to a corner, then just fill expand the hole
                        //in both directions
                        Vec2 b;
                        b.x=p.x;
                        for(uint e1=1; e1<(exitSize-1)/2; ++e1)
                        {
                            b.y=p.y+e1;
                            p2.push_back(b);
                            b.y=p.y-e1;
                            p2.push_back(b);
                        }

                    }
                }
                else
                {
                    p.x=m_randF->randi(1,roomSize-1);
                    //if exit size > 1, need to check if current point is at corners,
                    //if so, instead of setting points at both its sides, just
                    //set points at one of the sides
                    if(p.x >= roomSize-1-exitSize/2 || p.x <= 1+exitSize/2)
                    {
                        for(uint e=1; e<exitSize-1; ++e)
                        {
                            Vec2 a;
                            if(p.x <= 1+exitSize/2)
                            {
                                a=Vec2(p.x+e, p.y);
                            }
                            else
                            {
                                a=Vec2(p.x-e, p.y);
                            }
                            p2.push_back(a);
                        }
                    }
                    else
                    {
                        //if not close to a corner, then just fill expand the hole
                        //in both directions
                        Vec2 b;
                        b.y=p.y;
                        for(uint e1=1; e1<(exitSize-1)/2; ++e1)
                        {
                            b.x=p.x+e1;
                            p2.push_back(b);
                            b.x=p.x-e1;
                            p2.push_back(b);
                        }

                    }
                }

                for(auto g : takenXY)
                {
                    if(g == p)
                    {
                        chk = true;
                    }
                    for(auto pos : p2)
                    {
                        if(g == pos)
                        {
                            chk = true;
                        }
                    }
                }
            }
            while(chk);

            takenXY.push_back(p);
            takenXY.reserve(takenXY.size()+p2.size());
            takenXY.insert(takenXY.end(), p2.begin(), p2.end());

            //takenXY.emplace_back(p2);
            if(i<numExits)
            {
                setAt(p, 3);
                for(auto _p : p2)
                {
                    setAt(_p, 3);
                }
            }
            else
            {
                if(i<(numExits+numEntrances))
                {
                    setAt(p, 2);
                    for(auto _p : p2)
                    {
                        setAt(_p, 2);
                    }
                }
                else
                {
                    setAt(p, 4);
                    for(auto _p : p2)
                    {
                        setAt(_p, 4);
                    }
                }
            }
            m_exits.push_back(p);
            m_exits.reserve(m_exits.size()+p2.size());
            m_exits.insert(m_exits.end(), p2.begin(), p2.end());
        }
        for(auto c : m_cells)
        {
            if(c->m_value==1)
                m_walls.push_back(c->m_limit);
        }
    }
}

void WorldGrid::spawnAgents()
{
    std::vector<GridCell*> spawnZone;
    for(auto c : m_cells)
    {
        if(c->m_id<m_gridSizeX || c->m_id>m_cells.size()-m_gridSizeX ||
           c->m_id%m_gridSizeX == 0 || c->m_id%m_gridSizeX == m_gridSizeX-1)
        {
            spawnZone.push_back(c);
        }
    }
    for(uint i=0; i<m_params.get()->entity_numAgents; ++i)
    {
        std::cout<<"boop 1"<<std::endl;
        Agent* nAgent = new Agent(m_nmaker.get()->makeName(),
                                  this, m_shop, m_time,
                                  m_params,m_randF);
        std::cout<<"boop 2"<<std::endl;
        addAgent(nAgent, spawnZone.at(m_randF.get()->randi(0,spawnZone.size()-1)));
        std::cout<<"boop 3"<<std::endl;
    }
}

void WorldGrid::calcPaths()
{
    m_exitPaths = StarComputePaths(true);
    m_enterPaths = StarComputePaths(false);
}

std::vector<std::vector<std::vector<uint>>> WorldGrid::StarComputePaths(bool _type)
{
    std::vector<std::vector<std::vector<uint>>> ret;
    ret.resize(m_cells.size());
    if(m_params->nav_maxRoomDim>0)
    {
        uint roomSize = m_randF->randi(16, m_params->nav_maxRoomDim,0);
        if(_type) //this is exits
        {
            //for exits we only search inside the room
            for(uint y=m_gridSizeY/2-roomSize/2+1; y<m_gridSizeY/2-roomSize/2-1; ++y)
            {
                for(uint x=m_gridSizeX/2-roomSize/2+1; x<m_gridSizeX/2-roomSize/2-1; ++x)
                {
                    //for all exits
                    for(auto e : m_exits)
                    {
                        ret.at(y*m_gridSizeX+x) = PathTrace(x,y,e);
                    }
                }
            }
        }
        else //this is entrances
        {
            //for entrances we search whole grid
            //layout with room should be:
            /// ************
            /// ***      ***
            /// ***      ***
            /// ***      ***
            /// ************
            for(uint y=0; y<m_gridSizeY; ++y)
            {
                if(y < m_gridSizeY/2-roomSize/2 || y > m_gridSizeY/2+roomSize/2)
                {
                    for(uint x=0; x<m_gridSizeX; ++x)
                    {
                        for(auto e : m_entrances)
                        {
                            std::vector<std::vector<uint>> temp = PathTrace(x,y,e);
                            ret.at(y*m_gridSizeX+x).reserve(ret.at(y*m_gridSizeX+x).size()+temp.size());
                            ret.at(y*m_gridSizeX+x).insert(ret.at(y*m_gridSizeX+x).end(), temp.begin(), temp.end());
                        }
                    }
                }
                else
                {
                    for(uint xl=0; xl<m_gridSizeX/2-roomSize/2; ++xl)
                    {
                        for(auto e : m_entrances)
                        {
                            std::vector<std::vector<uint>> temp = PathTrace(xl,y,e);
                            ret.at(y*m_gridSizeX+xl).reserve(ret.at(y*m_gridSizeX+xl).size()+temp.size());
                            ret.at(y*m_gridSizeX+xl).insert(ret.at(y*m_gridSizeX+xl).end(), temp.begin(), temp.end());
                        }
                    }
                    for(uint xr=m_gridSizeX/2+roomSize/2; xr<m_gridSizeX; ++xr)
                    {
                        for(auto e : m_entrances)
                        {
                            std::vector<std::vector<uint>> temp = PathTrace(xr,y,e);
                            ret.at(y*m_gridSizeX+xr).reserve(ret.at(y*m_gridSizeX+xr).size()+temp.size());
                            ret.at(y*m_gridSizeX+xr).insert(ret.at(y*m_gridSizeX+xr).end(), temp.begin(), temp.end());
                        }
                    }
                }
            }
        }
    }
    return ret;
}

std::vector<std::vector<uint>> WorldGrid::PathTrace(uint _x, uint _y, Vec2 _e)
{
    std::vector<std::vector<uint>> workVec;
    std::vector<std::vector<uint>> retVec;
    float distance = INFINITY;
    using dr=Direction;

    //find paths
    dr dir = dr::N;
    Vec2 opVec = m_cells.at(_y*m_gridSizeX+_x)->m_position;
    distance = opVec.distance(_e);
    Vec2 newPos;

    //NORTH
    if(_y!=0)
    {
        newPos = Vec2(m_cells.at((_y-1)*m_gridSizeX+_x)->m_position);
    }

    if(_y!=0 && newPos.distance(_e) <= distance && newPos.distance(_e) >= m_navPTDistance)
    {
        if(m_cells.at((_y-1)*m_gridSizeX+_x)->m_value != 1)
        {
            std::vector<std::vector<uint>> temp = PathTrace(_x, _y-1, _e);
            workVec.reserve(workVec.size()+temp.size());
            workVec.insert(workVec.end(), temp.begin(), temp.end());
        }
    }
    //END NORTH

    for(uint i=1; i<8; ++i)
    {
        auto newDir = static_cast<dr>(static_cast<int>(dir) + i);
        switch(newDir)
        {
        case dr::E :
        {
            newPos = Vec2(m_cells.at(_y*m_gridSizeX+_x+1)->m_position);
            if(_x!=m_gridSizeX-1 && newPos.distance(_e) <= distance && newPos.distance(_e) >= m_navPTDistance)
            {
                if(m_cells.at(_y*m_gridSizeX+_x+1)->m_value != 1)
                {
                    std::vector<std::vector<uint>> temp = PathTrace(_x+1, _y, _e);
                    workVec.reserve(workVec.size()+temp.size());
                    workVec.insert(workVec.end(), temp.begin(), temp.end());
                }
            }
            break;
        }
        case dr::S :
        {
            newPos = Vec2(m_cells.at((_y+1)*m_gridSizeX+_x)->m_position);
            if(_y!=m_gridSizeY-1 && newPos.distance(_e) <= distance && newPos.distance(_e) >= m_navPTDistance)
            {
                if(m_cells.at((_y+1)*m_gridSizeX+_x)->m_value != 1)
                {
                    std::vector<std::vector<uint>> temp = PathTrace(_x, _y+1, _e);
                    workVec.reserve(workVec.size()+temp.size());
                    workVec.insert(workVec.end(), temp.begin(), temp.end());
                }
            }
            break;
        }
        case dr::W :
        {
            newPos = Vec2(m_cells.at(_y*m_gridSizeX+_x-1)->m_position);
            if(_x!=0 && newPos.distance(_e) <= distance && newPos.distance(_e) >= m_navPTDistance)
            {
                if(m_cells.at(_y*m_gridSizeX+_x-1)->m_value != 1)
                {
                    std::vector<std::vector<uint>> temp = PathTrace(_x-1, _y, _e);
                    workVec.reserve(workVec.size()+temp.size());
                    workVec.insert(workVec.end(), temp.begin(), temp.end());
                }
            }
            break;
        }
        default : {break;}
        }
    }
    std::vector<uint> pt;

    if(workVec.size()!=0) // is this
    {
        for(auto p : workVec)
        {
            for(auto t : p)
            {
                pt.clear();
                pt.push_back(_y*m_gridSizeX+_x);
                pt.push_back(t);
                retVec.push_back(pt);
            }
        }
    }
    else
    {
        pt.clear();
        pt.push_back(_y*m_gridSizeX+_x);
        retVec.push_back(pt);
    }

    return  retVec;
}

void WorldGrid::setAt(Vec2 _cell, uint _v)
{
    m_cells.at(_cell.x+_cell.y*m_gridSizeX)->m_value = _v;
}

void WorldGrid::setAt(uint _x, uint _y, uint _v)
{
    m_cells.at(_x+_y*m_gridSizeX)->m_value = _v;
}

void WorldGrid::checkBB()
{
    if(m_limit.m_maxx<m_limit.m_minx)
    {
        int temp = m_limit.m_maxx;
        m_limit.m_maxx = m_limit.m_minx;
        m_limit.m_minx = temp;
    }
    if(m_limit.m_maxy<m_limit.m_miny)
    {
        int temp = m_limit.m_maxy;
        m_limit.m_maxy = m_limit.m_miny;
        m_limit.m_miny = temp;
    }

    m_cellDimX = (m_limit.m_maxx - m_limit.m_minx)/m_gridSizeX;
    m_cellDimY = (m_limit.m_maxy - m_limit.m_miny)/m_gridSizeY;

    if(m_limit.m_maxx==m_limit.m_minx)
    {
        m_limit.m_maxx = 1;
        m_limit.m_minx = -1;
        m_gridSizeX = 1;
        m_cellDimX = 1;
    }
    if(m_limit.m_maxy==m_limit.m_miny)
    {
        m_limit.m_maxy = 1;
        m_limit.m_miny = -1;
        m_gridSizeY = 1;
        m_cellDimY = 1;
    }
    float A = (m_limit.m_maxx - m_limit.m_minx)%m_cellDimX;
    //get 1 or -1 depending on the max bounds and subtract to reduce the limit
    m_limit.m_maxx-=A*((m_limit.m_maxx > 0) ? 0.5 : ((m_limit.m_maxx < 0) ? -0.5 : 0));
    m_limit.m_minx+=A*((m_limit.m_minx > 0) ? 0.5 : ((m_limit.m_minx < 0) ? -0.5 : 0));
    //A = (float)(m_limit.m_maxx - m_limit.m_minx)%(float)m_cellDimX;
    float B = (m_limit.m_maxy - m_limit.m_miny)%m_cellDimY;
    m_limit.m_maxy-=B*((m_limit.m_maxy > 0) ? 0.5 : ((m_limit.m_maxy < 0) ? -0.5 : 0));
    m_limit.m_miny+=B*((m_limit.m_miny > 0) ? 0.5 : ((m_limit.m_miny < 0) ? -0.5 : 0));
    m_midPoint = Vec2((m_limit.m_minx+m_limit.m_maxx)/2.f,(m_limit.m_miny+m_limit.m_maxy)/2.f);
}
bool WorldGrid::checkBounds (const Vec2 &_pos, float _r, const BoundingBox &_limit)
{
  return (_pos.x-_r > (float)_limit.m_maxx ||
          _pos.x+_r < (float)_limit.m_minx ||
          _pos.y-_r > (float)_limit.m_maxy ||
          _pos.y+_r < (float)_limit.m_miny
          );
}

int WorldGrid::checkGrid(Vec2 _pos)
{
    int c = -1;
        if(checkBounds(_pos, 0.f, m_limit))
        {
           int a = (int)_pos.x % m_cellDimX;
           int b = (int)_pos.y % m_cellDimY;
           if((uint)abs(a)>=m_cellDimX/2)
           {
               if(a>0)
               {
                   a=m_cellDimX;
               }
               else
               {
                   a=-m_cellDimX;
               }
           }
           if((uint)abs(b)>=m_cellDimY/2)
           {
               if(a>0)
               {
                   a=m_cellDimY;
               }
               else
               {
                   a=-m_cellDimY;
               }
           }
        }
    return c;
}

/// @brief clear off all the agents from the grid
void WorldGrid::clearGrid()
{
    for(auto c : m_cells)
        c->m_agentList.clear();
}

uint WorldGrid::randomToExit(uint _current) const
{
    uint a = m_randF->randi(0,m_exitPaths.at(_current).size()-1,0);
    uint b = m_randF->randi(0,m_exitPaths.at(_current).at(a).size()-1,0);
    return m_exitPaths.at(_current).at(a).at(b);
}

std::vector<uint> WorldGrid::randPathToExit(uint _current) const
{
    std::cout<<m_exitPaths.size()<<" | "<<std::endl;
    uint a = m_randF->randi(0,m_exitPaths.at(_current).size()-1,0);
    return m_exitPaths.at(_current).at(a);
}

uint WorldGrid::randomToEntrance(uint _current) const
{
    uint a = m_randF->randi(0,m_enterPaths.at(_current).size()-1,0);
    uint b = m_randF->randi(0,m_enterPaths.at(_current).at(a).size()-1,0);
    return m_enterPaths.at(_current).at(a).at(b);
}

std::vector<uint> WorldGrid::randPathToEntrance(uint _current) const
{
    uint a = m_randF->randi(0,m_enterPaths.at(_current).size()-1,0);
    return m_enterPaths.at(_current).at(a);
}

void WorldGrid::checkCollisionOnNode(GridCell* _cell)
{
    for(auto a1 : _cell->m_agentList)
    {
        for(auto a2 : _cell->m_agentList)
        {
            if(a1 != a2)
            {
                float dist = a1->getPosition().distance(a2->getPosition());
                float diff = a1->getRadius()+a2->getRadius();
                dist -= diff;
                if(dist<0)
                {
                    Vec2 d1 = a1->getPosition()-a2->getPosition();
                    Vec2 d2 = a2->getPosition()-a1->getPosition();
                    d1.normalize();
                    d2.normalize();
                    d1*=dist/a1->getWeight();
                    d2*=dist/a2->getWeight();
                    a1->setDrag(d1.x, d1.y);
                    a2->setDrag(d2.x, d2.y);
                }
            }
        }
    }
}

bool WorldGrid::insideRoom(Vec2 _pos)
{
    return checkBounds(_pos, 0.f, m_roomLimit);
}

Vec2 WorldGrid::getGridMidpoint()
{
    return m_midPoint;
}

float WorldGrid::getKillRadius()
{
    return m_killRadius;
}

std::vector<Agent*> WorldGrid::getAgents()
{
    std::vector<Agent*> ret;
    for(auto c : m_cells)
    {
        if(c->m_agentList.size()>0)
        {
            ret.reserve(ret.size()+c->m_agentList.size());
            ret.insert(ret.end(), c->m_agentList.begin(), c->m_agentList.end());
        }
    }
    return ret;
}

std::vector<BoundingBox> WorldGrid::getWalls()
{
    return m_walls;
}
