#include "WorldGrid.h"
#include <iostream>
#include "Pathfinder.h"

WorldGrid::WorldGrid(std::shared_ptr<Params> _prms, uint _sX, uint _sY,
                     BoundingBox _lim, std::shared_ptr<Time> _tm)
{
  if(_sX>0 && _sY>0)
  {
    m_gridSizeX = _sX;
    m_gridSizeY = _sY;
  }
  m_limit = _lim;
  m_params = _prms;
  m_time = _tm;
  m_params = std::make_shared<Params> ();
  m_randF = std::make_shared<RandF> ();
  m_nmaker = std::make_shared<NameMaker> (m_randF);
  checkBB();
  m_cells.clear();
  m_cells.reserve(m_gridSizeY);
  m_cells.resize(m_gridSizeY);
  for(auto &o : m_cells)
  {
      o.clear();
      o.reserve(m_gridSizeX);
      o.resize(m_gridSizeX);
  }
  initGrid();
  initMap();
  m_time.get()->LaunchTimer();
  m_shop = std::make_shared<Shop> (m_params, this);
  m_shop.get()->setExits(m_exits);
  m_shop.get()->spawnProducts(m_roomLimit);
  m_shop.get()->update();
  m_pfinder = std::make_shared<Pathfinder> (Vec2(m_gridSizeX, m_gridSizeY), this);
  calcPaths();
  m_time.get()->tick();
  spawnAgents();
}
/// @brief destructor
WorldGrid::~WorldGrid()
{
    clearGrid();
    for(auto &c : m_cells)
    {
      c.clear();
    }
    m_cells.clear();
}

float WorldGrid::getCellDim() const
{
    return m_cellDimX;
}

GridCell* WorldGrid::cellAt(Vec2 _cell)
{
    return m_cells.at(_cell.y).at(_cell.x);
}

GridCell* WorldGrid::cellAt(uint _x, uint _y)
{
    return m_cells.at(_y).at(_x);
}

uint WorldGrid::getAt(Vec2 _cell)
{
    return m_cells.at(_cell.y).at(_cell.x)->m_value;
}

uint WorldGrid::getAt(uint _x, uint _y)
{
    return m_cells.at(_y).at(_x)->m_value;
}

void WorldGrid::update()
{
    m_time.get()->tick();
    for(auto &y : m_cells)
    {
        for(auto &x : y)
        {
            for(Agent* a : x->m_agentList)
            {
                a->setCell(x->m_id);
                a->update(x->m_agentList);
                m_shop.get()->update();
            }
            checkCollisionOnNode(x);
        }
    }
    m_shop.get()->update();
}

void WorldGrid::addAgent(Agent *_a)
{
    Vec2 g = checkGrid(_a->getPosition());
    if(g.x != -1 && g.y != -1)
    {
      m_cells.at(g.y).at(g.x)->m_agentList.push_back(_a);
      _a->setPosition(m_cells.at(g.y).at(g.x)->m_position);
      _a->setCell(m_cells.at(g.y).at(g.x)->m_id);
    }
}

void WorldGrid::addAgent(Agent *_a, GridCell* _cell)
{
    _cell->m_agentList.push_back(_a);
    _a->setPosition(_cell->m_position);
    _a->setCell(_cell->m_id);
}

void WorldGrid::initGrid()
{
    for(uint y=0; y<m_cells.size(); ++y)
    {
        for(uint x=0; x<m_cells.at(y).size(); ++x)
        {
            m_cells.at(y).at(x) = new GridCell;
            m_cells.at(y).at(x)->m_limit.m_minx = m_limit.m_minx + x*m_cellDimX;
            m_cells.at(y).at(x)->m_limit.m_maxx = m_limit.m_minx + (x+1)*m_cellDimX;
            m_cells.at(y).at(x)->m_limit.m_miny = m_limit.m_miny + y*m_cellDimY;
            m_cells.at(y).at(x)->m_limit.m_maxy = m_limit.m_miny + (y+1)*m_cellDimY;
            m_cells.at(y).at(x)->m_position = Vec2((m_cells.at(y).at(x)->m_limit.m_minx+m_cells.at(y).at(x)->m_limit.m_maxx)/2.f,
                                          (m_cells.at(y).at(x)->m_limit.m_miny+m_cells.at(y).at(x)->m_limit.m_maxy)/2.f);
            m_cells.at(y).at(x)->m_id = Vec2(x,y);
        }
    }
    m_killRadius = sqrt((m_limit.m_minx-m_limit.m_maxx)*(m_limit.m_minx-m_limit.m_maxx)+
                        (m_limit.m_miny-m_limit.m_maxy)*(m_limit.m_miny-m_limit.m_maxy));
}

void WorldGrid::initMap()
{

///The map is defined as follows:
/// 0 - empty
/// 1 - wall
/// 2 - entrance
/// 3 - exit
/// 4 - inout

    //init all cells to 0
    for(auto &y : m_cells)
    {
        for(auto &x : y)
        {
            x->m_value = 0;
        }
    }

    if(m_params->nav_maxRoomDim>0)
    {
        uint roomSize = m_randF->randi(16, m_params->nav_maxRoomDim,0);
        m_roomDim = roomSize;
        std::cout<<roomSize<<std::endl;

        //make walls
        for(uint y=m_gridSizeY/2-roomSize/2; y<m_gridSizeY/2+roomSize/2; ++y)
        {
            setAt(m_gridSizeX/2-roomSize/2,y,1); //  set  |    |
            setAt(m_gridSizeX/2+roomSize/2,y,1); //       |    |
        }
        for(uint x=m_gridSizeX/2-roomSize/2; x<m_gridSizeX/2+roomSize/2; ++x)
        {
            setAt(x,m_gridSizeY/2-roomSize/2,1); //  set  ------
            setAt(x,m_gridSizeY/2+roomSize/2,1); //       ------
        }
        m_roomLimit.m_minx = cellAt(Vec2(m_gridSizeX/2-roomSize/2,m_gridSizeY/2))->m_limit.m_minx;
        m_roomLimit.m_maxx = cellAt(Vec2(m_gridSizeX/2+roomSize/2,m_gridSizeY/2))->m_limit.m_maxx;
        m_roomLimit.m_miny = cellAt(Vec2(m_gridSizeX/2,m_gridSizeY/2-roomSize/2))->m_limit.m_miny;
        m_roomLimit.m_maxy = cellAt(Vec2(m_gridSizeX/2,m_gridSizeY/2+roomSize/2))->m_limit.m_maxy;

        m_exits.clear();
        m_entrances.clear();

        setAt(m_gridSizeX/2-roomSize/2,m_gridSizeY/2,4);
        setAt(m_gridSizeX/2-roomSize/2,m_gridSizeY/2+1,4);
        setAt(m_gridSizeX/2-roomSize/2,m_gridSizeY/2-1,4);

        /*m_exits.push_back(m_gridSizeY/2*m_gridSizeX+m_gridSizeX/2-roomSize/2);
        m_entrances.push_back(Vec2(m_gridSizeX/2-roomSize/2,m_gridSizeY/2));
        m_exits.push_back(Vec2(m_gridSizeX/2-roomSize/2,m_gridSizeY/2+1));
        m_entrances.push_back(Vec2(m_gridSizeX/2-roomSize/2,m_gridSizeY/2+1));
        m_exits.push_back(Vec2(m_gridSizeX/2-roomSize/2,m_gridSizeY/2-1));
        m_entrances.push_back(Vec2(m_gridSizeX/2-roomSize/2,m_gridSizeY/2-1));*/

        //make holes
        /*
        uint numExits = m_randF->randi(0, m_params->nav_maxNumExits,0);
        uint numEntrances = m_randF->randi(0, m_params->nav_maxNumEntrances,0);
        uint numInouts = m_randF->randi(1, m_params->nav_maxNumInouts,0);
        uint exitSize = (uint)(sqrt(roomSize)/2);
        std::vector<Vec2> takenXY;

        uint diffx = m_gridSizeX/2 - roomSize/2;
        uint diffy = m_gridSizeY/2 - roomSize/2;

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
                    p.x=m_gridSizeX/2-(((k%2)*-1)+1)*roomSize/2+(k%2)*roomSize/2;
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
                                a=Vec2(p.x, p.y+e+diffy);
                            }
                            else
                            {
                                a=Vec2(p.x, p.y-e+diffy);
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
                        for(uint e1=1; e1<exitSize/2; ++e1)
                        {
                            b.y=p.y+e1+diffy;
                            p2.push_back(b);
                            b.y=p.y-e1+diffy;
                            p2.push_back(b);
                        }

                    }
                }
                else
                {
                    p.x=m_randF->randi(1,roomSize-1);
                    p.y=m_gridSizeX/2-(((k%2)*-1)+1)*roomSize/2+(k%2)*roomSize/2;
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
                                a=Vec2(p.x+e+diffx, p.y);
                            }
                            else
                            {
                                a=Vec2(p.x-e+diffx, p.y);
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
                        for(uint e1=1; e1<exitSize/2; ++e1)
                        {
                            b.x=p.x+e1+diffx;
                            p2.push_back(b);
                            b.x=p.x-e1+diffx;
                            p2.push_back(b);
                        }

                    }
                }

                for(auto &g : takenXY)
                {
                    if(g == p)
                    {
                        chk = true;
                    }
                    for(auto &pos : p2)
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
                setAt(p.x, p.y, 3);
                for(auto &_p : p2)
                {
                    setAt(_p.x,_p.y, 3);
                    m_exits.push_back(Vec2(p.x, p.y));
                }
            }
            else
            {
                if(i<(numExits+numEntrances))
                {
                    setAt(p.x, p.y, 2);
                    for(auto &_p : p2)
                    {
                        setAt(_p.x,_p.y, 2);
                        m_entrances.push_back(Vec2(p.x, p.y));
                    }
                }
                else
                {
                    setAt(p.x, p.y, 4);
                    for(auto &_p : p2)
                    {
                        setAt(_p.x,_p.y, 4);
                        m_exits.push_back(Vec2(p.x, p.y));
                        m_entrances.push_back(Vec2(p.x, p.y));
                    }
                }
            }

        }*/
        for(auto &m : m_cells)
        {
            for(auto &n : m)
            {
                if(n->m_value==1)
                    m_walls.push_back(n);
                if(n->m_value==4)
                {
                    m_exits.push_back(n->m_id);
                    m_entrances.push_back(n->m_id);
                }
            }
        }
    }
}

void WorldGrid::spawnAgents()
{
    std::vector<GridCell*> spawnZone;
    for(auto &y : m_cells)
    {
        for(auto &x : y)
        {
            if(x->m_id.x==0 || x->m_id.x==m_gridSizeX-1 ||
               x->m_id.y==0 || x->m_id.y==m_gridSizeX-1)
            {
                spawnZone.push_back(x);
            }
        }
    }
    for(uint i=0; i<m_params.get()->entity_numAgents; ++i)
    {
        Agent* nAgent = new Agent(m_nmaker.get()->makeName(),
                                  this, m_shop, m_time,
                                  m_params,m_randF);
        addAgent(nAgent, spawnZone.at(m_randF.get()->randi(0,spawnZone.size()-1)));
    }
}

void WorldGrid::calcPaths()
{
    //m_exitPaths = StarComputePaths(true);
    //m_enterPaths = StarComputePaths(false);
    //m_exitPaths = AStarPathfind(true);
    //m_enterPaths = AStarPathfind(false);
    AStarPathfind();
}



void WorldGrid::setAt(Vec2 _cell, uint _v)
{
    m_cells.at(_cell.y).at(_cell.x)->m_value = _v;
}

void WorldGrid::setAt(uint _x, uint _y, uint _v)
{
    m_cells.at(_y).at(_x)->m_value = _v;
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
    float hlfx = m_cellDimX*m_gridSizeX;
    float hlfy = m_cellDimY*m_gridSizeY;

    if(hlfx != m_limit.m_maxx - m_limit.m_minx)
    {
        float A = hlfx - (m_limit.m_maxx - m_limit.m_minx);
        m_limit.m_maxx += A;
    }
    if(hlfy != m_limit.m_maxy - m_limit.m_miny)
    {
        float A = hlfy - (m_limit.m_maxy - m_limit.m_miny);
        m_limit.m_maxy += A;
    }

    /*
    //get 1 or -1 depending on the max bounds and subtract to reduce the limit
    m_limit.m_maxx-=A*((m_limit.m_maxx > 0) ? 0.5 : ((m_limit.m_maxx < 0) ? -0.5 : 0));
    m_limit.m_minx+=A*((m_limit.m_minx > 0) ? 0.5 : ((m_limit.m_minx < 0) ? -0.5 : 0));
    //A = (float)(m_limit.m_maxx - m_limit.m_minx)%(float)m_cellDimX;
    float B = (m_limit.m_maxy - m_limit.m_miny)%m_cellDimY;
    m_limit.m_maxy-=B*((m_limit.m_maxy > 0) ? 0.5 : ((m_limit.m_maxy < 0) ? -0.5 : 0));
    m_limit.m_miny+=B*((m_limit.m_miny > 0) ? 0.5 : ((m_limit.m_miny < 0) ? -0.5 : 0));*/

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
        if(!checkBounds(_pos, 0.f, m_limit))
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
    for(auto &c : m_cells)
    {
        for(auto &c1 : c)
        {
            c1->m_agentList.clear();
        }
    }
}

std::vector<Vec2> WorldGrid::randPathToExit(Vec2 _current) const
{
    std::vector<Vec2> ret;
    if(!m_exitPaths.empty())
    {
        if(!m_exitPaths.at(_current.y).empty())
        {
            if(!m_exitPaths.at(_current.y).at(_current.x).empty())
            {
                uint a = m_randF->randi(0,m_exitPaths.at(_current.y).at(_current.x).size()-1,0);
                ret = m_exitPaths.at(_current.y).at(_current.x).at(a);
            }
        }
    }
    return ret;
}


std::vector<Vec2> WorldGrid::randPathToEntrance(Vec2 _current) const
{
    std::vector<Vec2> ret;
    if(!m_enterPaths.empty())
    {
        if(!m_enterPaths.at(_current.y).empty())
        {
            if(!m_enterPaths.at(_current.y).at(_current.x).empty())
            {
                uint a = m_randF->randi(0,m_enterPaths.at(_current.y).at(_current.x).size()-1,0);
                ret = m_enterPaths.at(_current.y).at(_current.x).at(a);
            }

        }
    }
    return ret;
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
    return (_pos.x < (float)m_roomLimit.m_maxx &&
            _pos.x > (float)m_roomLimit.m_minx &&
            _pos.y < (float)m_roomLimit.m_maxy &&
            _pos.y > (float)m_roomLimit.m_miny
            );
}

void WorldGrid::AStarPathfind()
{
    //super cheat since others do not work
    m_exitPaths.clear();
    m_exitPaths.reserve(m_gridSizeY);
    m_exitPaths.resize(m_gridSizeY);
    for(auto &o : m_exitPaths)
    {
        o.reserve(m_gridSizeX);
        o.resize(m_gridSizeX);
    }
    m_enterPaths.clear();
    m_enterPaths.reserve(m_gridSizeY);
    m_enterPaths.resize(m_gridSizeY);
    for(auto &o : m_enterPaths)
    {
        o.reserve(m_gridSizeX);
        o.resize(m_gridSizeX);
    }

    if(m_params->nav_maxRoomDim>0)
    {

        for(uint y=0; y<m_gridSizeY; ++y)
        {
            for(uint x=0; x<m_gridSizeX; ++x)
            {
                Vec2 checkPos = Vec2(x*m_cellDimX,y*m_cellDimY);
                m_exitPaths.at(y).at(x).reserve(m_exits.size());
                m_exitPaths.at(y).at(x).resize(m_exits.size());
                m_enterPaths.at(y).at(x).reserve(m_entrances.size());
                m_enterPaths.at(y).at(x).resize(m_entrances.size());
                for(uint e=0; e<m_exits.size(); ++e)
                {
                    if(insideRoom(checkPos))
                    {
                        std::vector<Vec2> easyV;
                        easyV.push_back(m_exits.at(e));
                        m_exitPaths.at(y).at(x).at(e) = easyV;
                    }
                    else
                    {
                        std::vector<Vec2> easyV;
                        easyV.push_back(m_entrances.at(e));
                        m_enterPaths.at(y).at(x).at(e) = easyV;
                    }
                }
            }
        }
    }
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
    for(auto &c : m_cells)
    {
        for(auto &c1 : c)
        {
            if(c1->m_agentList.size()>0)
            {
                ret.reserve(ret.size()+c1->m_agentList.size());
                ret.insert(ret.end(), c1->m_agentList.begin(), c1->m_agentList.end());
            }
        }
    }
    return ret;
}

std::vector<GridCell*> WorldGrid::getWalls()
{
    return m_walls;
}

std::vector<Vec2> WorldGrid::getProducts()
{
    return m_shop.get()->getProductPositions();
}

/*void WorldGrid::AStarPathfind()
{
    //std::vector<std::vector<std::vector<uint>>> ret;
    //ret.resize(m_cells.size());
    m_exitPaths.clear();
    m_exitPaths.reserve(m_cells.size());
    m_exitPaths.resize(m_cells.size());
    m_enterPaths.clear();
    m_enterPaths.reserve(m_cells.size());
    m_enterPaths.resize(m_cells.size());
    if(m_params->nav_maxRoomDim>0)
    {

        for(uint y=0; y<m_gridSizeY; ++y)
        {
            for(uint x=0; x<m_gridSizeY; ++x)
            {
                Vec2 checkPos = Vec2(x*m_cellDimX,y*m_cellDimY);
                for(uint e=0; e<m_exits.size(); ++e)
                {
                    if(insideRoom(checkPos))
                    {
                        m_exitPaths.at(y*m_gridSizeX+x).reserve(m_exits.size());
                        m_exitPaths.at(y*m_gridSizeX+x).resize(m_exits.size());
                        m_exitPaths.at(y*m_gridSizeX+x).at(e) = m_pfinder.get()->getPath(x,y,m_exits.at(e).x,m_exits.at(e).y,true,m_roomDim,m_roomDim);
                    }
                    else
                    {
                        m_enterPaths.at(y*m_gridSizeX+x).reserve(m_entrances.size());
                        m_enterPaths.at(y*m_gridSizeX+x).resize(m_entrances.size());
                        m_enterPaths.at(y*m_gridSizeX+x).at(e) = m_pfinder.get()->getPath(x,y,m_entrances.at(e).x,m_entrances.at(e).y,false,m_roomDim,m_roomDim);
                    }
                }


            }
        }
    }*/
        /*for(uint y=0; y<m_gridSizeY; ++y)
        {
            for(uint x=0; x<m_gridSizeX; ++x)
            {
                Vec2 checkPos = Vec2(x*m_cellDimX,y*m_cellDimY);
                if(insideRoom(checkPos))
                {
                    //for all exits
                    for(uint e=0; e<m_exits.size(); ++e)
                    {
                        m_exitPaths.at(y*m_gridSizeX+x).reserve(m_exits.size());
                        m_exitPaths.at(y*m_gridSizeX+x).resize(m_exits.size());
                        m_exitPaths.at(y*m_gridSizeX+x).at(e) = m_pfinder.get()->getPath(x,y,m_exits.at(e).x,m_exits.at(e).y);
                    }
                }
                else
                {
                    //for all exits
                    for(uint e=0; e<m_entrances.size(); ++e)
                    {
                        m_enterPaths.at(y*m_gridSizeX+x).reserve(m_entrances.size());
                        m_enterPaths.at(y*m_gridSizeX+x).resize(m_entrances.size());
                        m_enterPaths.at(y*m_gridSizeX+x).at(e) = m_pfinder.get()->getPath(x,y,m_entrances.at(e).x,m_entrances.at(e).y);
                    }
                }
            }

        }*/

        /*uint roomSize = m_randF->randi(16, m_params->nav_maxRoomDim,0);
        if(_type) //this is exits
        {
            //for exits we only search inside the room
            for(uint y=m_gridSizeY/2-roomSize/2+1; y<m_gridSizeY/2-roomSize/2-1; ++y)
            {
                for(uint x=m_gridSizeX/2-roomSize/2+1; x<m_gridSizeX/2-roomSize/2-1; ++x)
                {
                    //for all exits
                    for(uint e=0; e<m_exits.size(); ++e)
                    {
                        ret.at(y*m_gridSizeX+x).reserve(m_exits.size());
                        ret.at(y*m_gridSizeX+x).resize(m_exits.size());
                        ret.at(y*m_gridSizeX+x).at(e) = m_pfinder.get()->getPath(x,y,m_exits.at(e).x,m_exits.at(e).y);
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
                        for(uint e=0; e<m_entrances.size(); ++e)
                        {
                            ret.at(y*m_gridSizeX+x).reserve(m_entrances.size());
                            ret.at(y*m_gridSizeX+x).resize(m_entrances.size());
                            ret.at(y*m_gridSizeX+x).at(e) = m_pfinder.get()->getPath(x,y,m_entrances.at(e).x,m_entrances.at(e).y);
                        }
                    }
                }
                else
                {
                    for(uint xl=0; xl<m_gridSizeX/2-roomSize/2; ++xl)
                    {
                        for(uint e=0; e<m_entrances.size(); ++e)
                        {
                            ret.at(y*m_gridSizeX+xl).reserve(m_entrances.size());
                            ret.at(y*m_gridSizeX+xl).resize(m_entrances.size());
                            ret.at(y*m_gridSizeX+xl).at(e) = m_pfinder.get()->getPath(xl,y,m_entrances.at(e).x,m_entrances.at(e).y);
                        }
                    }
                    for(uint xr=m_gridSizeX/2+roomSize/2; xr<m_gridSizeX; ++xr)
                    {
                        for(uint e=0; e<m_entrances.size(); ++e)
                        {
                            ret.at(y*m_gridSizeX+xr).reserve(m_entrances.size());
                            ret.at(y*m_gridSizeX+xr).resize(m_entrances.size());
                            ret.at(y*m_gridSizeX+xr).at(e) = m_pfinder.get()->getPath(xr,y,m_entrances.at(e).x,m_entrances.at(e).y);
                        }
                    }
                }
            }
        }*/
    //}
    //return ret;
//}

/*std::vector<std::vector<std::vector<uint>>> WorldGrid::StarComputePaths(bool _type)
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
                    for(auto &e : m_exits)
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
                        for(auto &e : m_entrances)
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
                        for(auto &e : m_entrances)
                        {
                            std::vector<std::vector<uint>> temp = PathTrace(xl,y,e);
                            ret.at(y*m_gridSizeX+xl).reserve(ret.at(y*m_gridSizeX+xl).size()+temp.size());
                            ret.at(y*m_gridSizeX+xl).insert(ret.at(y*m_gridSizeX+xl).end(), temp.begin(), temp.end());
                        }
                    }
                    for(uint xr=m_gridSizeX/2+roomSize/2; xr<m_gridSizeX; ++xr)
                    {
                        for(auto &e : m_entrances)
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
    if(_y!=0) //don't check north if there are no cells above
    {
        newPos = Vec2(m_cells.at((_y-1)*m_gridSizeX+_x)->m_position);
        if(newPos.distance(_e) <= distance && distance - newPos.distance(_e) <= m_navPTDistance)
        {
            if(m_cells.at((_y-1)*m_gridSizeX+_x)->m_value != 1)
            {
                std::vector<std::vector<uint>> temp = PathTrace(_x, _y-1, _e);
                workVec.reserve(workVec.size()+temp.size());
                workVec.insert(workVec.end(), temp.begin(), temp.end());
            }
        }
    }
    //END NORTH

    for(uint i=1; i<4; ++i)
    {
        auto newDir = static_cast<dr>(static_cast<int>(dir) + i);
        switch(newDir)
        {
        case dr::E :
        {
            if(_x!=m_gridSizeX+1) //don't check east if there are no cells to the right
            {
                newPos = Vec2(m_cells.at(_y*m_gridSizeX+_x+1)->m_position);
                if(newPos.distance(_e) <= distance && distance - newPos.distance(_e) <= m_navPTDistance)
                {
                    if(m_cells.at(_y*m_gridSizeX+_x+1)->m_value != 1)
                    {
                        std::vector<std::vector<uint>> temp = PathTrace(_x+1, _y, _e);
                        workVec.reserve(workVec.size()+temp.size());
                        workVec.insert(workVec.end(), temp.begin(), temp.end());
                    }
                }
            }
            break;
        }
        case dr::S :
        {
            if(_y!=m_gridSizeY+1) //don't check south if there are no cells below
            {
                newPos = Vec2(m_cells.at((_y+1)*m_gridSizeX+_x)->m_position);
                if(newPos.distance(_e) <= distance && distance - newPos.distance(_e) <= m_navPTDistance)
                {
                    if(m_cells.at((_y+1)*m_gridSizeX+_x)->m_value != 1)
                    {
                        std::vector<std::vector<uint>> temp = PathTrace(_x, _y+1, _e);
                        workVec.reserve(workVec.size()+temp.size());
                        workVec.insert(workVec.end(), temp.begin(), temp.end());
                    }
                }
            }
            break;
        }
        case dr::W :
        {
            if(_x!=0) //don't check west if there are no to the left
            {
                newPos = Vec2(m_cells.at(_y*m_gridSizeX+_x-1)->m_position);
                if(newPos.distance(_e) <= distance && distance - newPos.distance(_e) <= m_navPTDistance)
                {
                    if(m_cells.at(_y*m_gridSizeX+_x-1)->m_value != 1)
                    {
                        std::vector<std::vector<uint>> temp = PathTrace(_x-1, _y, _e);
                        workVec.reserve(workVec.size()+temp.size());
                        workVec.insert(workVec.end(), temp.begin(), temp.end());
                    }
                }
            }
            break;
        }
        default : {break;}
        }
    }
    std::vector<uint> pt;

    if(workVec.size()!=0)
    {
        for(auto &p : workVec)
        {
            for(auto &t : p)
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
}*/
