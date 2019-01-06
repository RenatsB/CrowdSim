#include "WorldGrid.h"

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

uint WorldGrid::getAt(Vec2 _cell)
{
    return m_cells.at(_cell.x+_cell.y*sizeX)->m_value;
}

void WorldGrid::update()
{
    for(GridCell* c : m_cells)
    {
        for(Agent* a : c->m_agentList)
        {
            a->update(c->m_agentList);
        }
        checkCollisionOnNode(c);
    }
}

void WorldGrid::addObject(Agent *_a)
{
    int g = checkGrid(_a->getPosition());
    if(g != -1)
      m_cells[g]->m_agentList.push_back(_a);
}

void WorldGrid::initGrid()
{
    //m_cellDimX = (m_limit.m_maxx - m_limit.m_minx)/m_cellNum;
    //m_cellDimY = (m_limit.m_maxy - m_limit.m_miny)/m_cellNum;
    for(int c=0; c<m_cells.size(); ++c)
    {
      m_cells[c] = new GridCell*;
      m_cells[c]->m_limit.m_minx = m_limit.m_minx + m_cellDimX*(c%m_gridSizeX);
      m_cells[c]->m_limit.m_maxx = m_limit.m_maxx + m_cellDimX*(c%m_gridSizeX +1);
      m_cells[c]->m_limit.m_miny = m_limit.m_miny + m_cellDimY*(c/m_gridSizeX);
      m_cells[c]->m_limit.m_maxy = m_limit.m_maxy + m_cellDimY*(c/m_gridSizeX +1);
    }
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
    for(uint u=0; u<m_values.size(); ++u)
        m_values.at(u) = 0;

    if(m_params->nav_maxRoomDim>0)
    {
        uint roomSize = m_randf->randi(16, m_params->nav_maxRoomDim,0);

        //make walls
        for(uint y=m_gridSizeY/2-roomSize/2; y<m_gridSizeY/2+roomSize/2; ++y)
        {
            setAt(m_gridSizeX/2-roomSize/2,y,1); //  set  |    |
            setAt(m_gridSizeX/2+roomSize/2,y,1); //       |    |
        }
        for(uint x=m_gridSizeX/2-roomSize/2; x<m_gridSizeX/2-roomSize/2; ++x)
        {
            setAt(x,m_gridSizeY/2+roomSize/2,1); //  set  ------
            setAt(x,m_gridSizeY/2+roomSize/2,1); //       ------
        }

        //make holes
        uint numExits = m_randf->randi(0, m_params->nav_maxNumExits,0);
        uint numEntrances = m_randf->randi(0, m_params->nav_maxNumEntrances,0);
        uint numInouts = m_randf->randi(1, m_params->nav_maxNumInouts,0);
        uint exitSize = (uint)(sqrt(roomSize)/2);
        std::vector<Vec2> takenXY;
        for(uint i=0; i<(numExits+numEntrances+numInouts); ++i)
        {
            Vec2 p;
            std::vector<Vec2> p2;
            int k = m_randf->randi(0, 4, 0);


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
                    p.y=m_randf->randi(1,roomSize-1);
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
                    p.x=m_randf->randi(1,roomSize-1);
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
            takenXY.emplace_back(p2);
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
            m_exits.emplace_back(p2);
        }
    }

}

void WorldGrid::calcPaths()
{
    m_exitPaths = DijkstraComputePaths(true);
    m_enterPaths = DijkstraComputePaths(false);
}

std::vector<std::vector<uint>> WorldGrid::DijkstraComputePaths(bool _type)
{
    std::vector<std::vector<uint>> ret;
    if(_type) //this is exits
    {
        //for exits we only search inside the room
        for(uint y=m_gridSizeY/2-roomSize/2+1; y<m_gridSizeY/2-roomSize/2-1; ++y)
        {
            for(uint x=m_gridSizeX/2-roomSize/2+1; x<m_gridSizeX/2-roomSize/2-1; ++x)
            {
                for(auto e : m_exits)
                {

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

                }
            }
            else
            {
                for(uint xl=0; xl<m_gridSizeX/2-roomSize/2; ++xl)
                {

                }
                for(uint xr=m_gridSizeX/2+roomSize/2; xr<m_gridSizeX; ++xr)
                {

                }
            }
        }
    }
    return ret;
}

void WorldGrid::setAt(Vec2 _cell, uint _v)
{
    m_cells.at(_cell.x+_cell.y*sizeX)->m_value = _v;
}

void WorldGrid::setAt(uint _x, uint _y, uint _v)
{
    m_cells.at(_x+_y*sizeX)->m_value = _v;
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
    float A = (float)(m_limit.m_maxx - m_limit.m_minx)%(float)m_cellDimX;
    //get 1 or -1 depending on the max bounds and subtract to reduce the limit
    m_limit.m_maxx-=A*((m_limit.m_maxx > 0) ? 0.5 : ((m_limit.m_maxx < 0) ? -0.5 : 0));
    m_limit.m_minx+=A*((m_limit.m_minx > 0) ? 0.5 : ((m_limit.m_minx < 0) ? -0.5 : 0));
    //A = (float)(m_limit.m_maxx - m_limit.m_minx)%(float)m_cellDimX;
    float B = (float)(m_limit.m_maxy - m_limit.m_miny)%(float)m_cellDimY;
    m_limit.m_maxy-=A*((m_limit.m_maxy > 0) ? 0.5 : ((m_limit.m_maxy < 0) ? -0.5 : 0));
    m_limit.m_miny+=A*((m_limit.m_miny > 0) ? 0.5 : ((m_limit.m_miny < 0) ? -0.5 : 0));
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
           int a = _pos.x % m_dimStepX;
           int b = _pos.y % m_dimStepY;
           if(abs(a)>=m_dimStepX/2)
           {
               if(a>0)
               {
                   a=m_dimStepX;
               }
               else
               {
                   a=-m_dimStepX;
               }
           }
           if(abs(b)>=m_dimStepY/2)
           {
               if(a>0)
               {
                   a=m_dimStepY;
               }
               else
               {
                   a=-m_dimStepY;
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
    uint a = m_randF->randi(0,m_exitPaths.at(_current).size(),0);
    uint b = m_randF->randi(0,m_exitPaths.at(_current).at(a).size(),0);
    return m_exitPaths.at(a).at(b);
}

std::vector<uint> WorldGrid::randPathToExit(uint _current) const
{
    uint a = m_randF->randi(0,m_exitPaths.at(_current).size(),0);
    return m_exitPaths.at(_current).at(a);
}

uint WorldGrid::randomToEntrance(uint _current) const
{
    uint a = m_randF->randi(0,m_enterPaths.at(_current).size(),0);
    uint b = m_randF->randi(0,m_enterPaths.at(_current).at(a).size(),0);
    return m_enterPaths.at(a).at(b);
}

std::vector<uint> WorldGrid::randPathToEntrance(uint _current) const
{
    uint a = m_randF->randi(0,m_enterPaths.at(_current).size(),0);
    return m_enterPaths.at(_current).at(a);
}

void WorldGrid::checkCollisionOnNode(GridCell* _cell)
{

}

