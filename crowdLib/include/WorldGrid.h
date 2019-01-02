#ifndef WORLDGRID_H_
#define WORLDGRID_H_
#include <vector>
#include <memory>
#include "Agent.h"
/// @file WorldGrid.h
/// @brief Modified from AbstractOctree.h from https://github.com/NCCA/OctreeAbstract
/// @author Renats Bikmajevs, modified from: Xiaosong Yang modifed by Jon Macey


///  _______
/// | 0 | 1 |
/// |-------|
/// | 2 | 3 |
///  -------

class BoundingBox
{
public :
    //all dimensions as integers to simplify calculations later
    int m_minx;
    int m_maxx;
    int m_miny;
    int m_maxy;
};

class GridCell
{
public:
    BoundingBox m_limit;
    std::vector<Agent *> m_agentList;
};

class WorldGrid
{
public:
    WorldGrid(uint _gDimX, uint _gDimY)
    {
      if(_gDimX>0 && _gDimY>0)
      {
        m_gridDimensionX = _gDimX;
        m_gridDimensionY = _gDimY;
      }
      checkBB();
      m_cells.clear();
      m_cells.reserve(m_cellDimX*m_cellDimY);
      m_cells.resize(m_cellDimX*m_cellDimY);
      initGrid();
    }

    /// @brief destructor
    ~WorldGrid()
    {
        for(auto c : m_cells)
        {
          free(c);
        }
        m_cells.clear();
    }

    void initGrid()
    {
        //m_cellDimX = (m_limit.m_maxx - m_limit.m_minx)/m_cellNum;
        //m_cellDimY = (m_limit.m_maxy - m_limit.m_miny)/m_cellNum;
        for(int c=0; c<m_cells.size(); ++c)
        {
          m_cells[c] = new GridCell*;
          m_cells[c]->m_limit.m_minx = m_limit.m_minx + m_cellDimX*(c%m_gridDimensionX);
          m_cells[c]->m_limit.m_maxx = m_limit.m_maxx + m_cellDimX*(c%m_gridDimensionX +1);
          m_cells[c]->m_limit.m_miny = m_limit.m_miny + m_cellDimY*(c/m_gridDimensionX);
          m_cells[c]->m_limit.m_maxy = m_limit.m_maxy + m_cellDimY*(c/m_gridDimensionX +1);
        }
    }

    void update()
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

    void addObject(Agent *_a)
    {
        int g = checkGrid(_a->getPosition());
        if(g != -1)
          m_cells[g]->m_agentList.push_back(_a);
    }

    bool checkBounds (const Vec2 &_pos, float _r, const BoundingBox &_limit)
    {
      return (_pos.x-_r > (float)_limit.m_maxx ||
              _pos.x+_r < (float)_limit.m_minx ||
              _pos.y-_r > (float)_limit.m_maxy ||
              _pos.y+_r < (float)_limit.m_miny
              );
    }

    int checkGrid(Vec2 _pos)
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
    void clearGrid()
    {
        for(auto c : m_cells)
            c->m_agentList.clear();
    }


private:
    void checkBB()
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

        m_cellDimX = (m_limit.m_maxx - m_limit.m_minx)/m_gridDimensionX;
        m_cellDimY = (m_limit.m_maxy - m_limit.m_miny)/m_gridDimensionY;

        if(m_limit.m_maxx==m_limit.m_minx)
        {
            m_limit.m_maxx = 1;
            m_limit.m_minx = -1;
            m_gridDimensionX = 1;
            m_cellDimX = 1;
        }
        if(m_limit.m_maxy==m_limit.m_miny)
        {
            m_limit.m_maxy = 1;
            m_limit.m_miny = -1;
            m_gridDimensionY = 1;
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

    void checkCollisionOnNode(GridCell* _cell)
    {

    }
private:
    uint m_cellDimX = 1;
    uint m_cellDimY = 1;
    uint m_gridDimensionX = 1;
    uint m_gridDimensionY = 1;
    BoundingBox m_limit;
    std::vector<GridCell*> m_cells;
};

#endif //WORLDGRID_H_

