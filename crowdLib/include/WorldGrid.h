#ifndef WORLDGRID_H_
#define WORLDGRID_H_
#include <vector>
#include <memory>
#include "Agent.h"
#include "Shop.h"
/// @file WorldGrid.h
/// @brief Space separation and object management module
/// @author Renats Bikmajevs


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
    Vec2 m_position = Vec2(0.f,0.f);
    std::vector<Agent *> m_agentList;
    uint m_value = 0;
    uint m_id =0;
};

class WorldGrid
{
public:
    WorldGrid(Params* _prms, RandF* _rand, uint _sX, uint _sY)
    {
      if(_sX>0 && _sY>0)
      {
        m_gridSizeX = _sX;
        m_gridSizeY = _sY;
      }
      m_params = _prms;
      m_randF = _rand;
      checkBB();
      m_cells.clear();
      m_cells.reserve(m_cellDimX*m_cellDimY);
      m_cells.resize(m_cellDimX*m_cellDimY);
      initGrid();
    }
    /// @brief destructor
    ~WorldGrid();

    uint getAt(Vec2 _cell);
    GridCell* cellAt(Vec2 _cell);
    void update();
    void addObject(Agent *_a);
    uint randomToExit(uint _current) const;
    std::vector<uint> randPathToExit(uint _current) const;
    uint randomToEntrance(uint _current) const;
    std::vector<uint> randPathToEntrance(uint _current) const;
private:
    void initGrid();
    void initMap();
    void calcPaths();
    std::vector<std::vector<std::vector<uint>>> StarComputePaths(bool _type);
    std::vector<std::vector<uint>> PathTrace(uint _x, uint _y, Vec2 _e);
    void setAt(Vec2 _cell, uint _v);
    void setAt(uint _x, uint _y, uint _v);
    void checkBB();
    bool checkBounds (const Vec2 &_pos, float _r, const BoundingBox &_limit);
    int checkGrid(Vec2 _pos);
    /// @brief clear off all the agents from the grid
    void clearGrid();
    void checkCollisionOnNode(GridCell* _cell);
private:
    RandF* m_randF;
    Params* m_params;
    Shop* m_shop;
    uint m_cellDimX = 1;
    uint m_cellDimY = 1;
    uint m_gridSizeX = 1;
    uint m_gridSizeY = 1;
    BoundingBox m_limit;
    std::vector<GridCell*> m_cells;
    std::vector<Vec2> m_exits;
    std::vector<Vec2> m_entrances;
    //these 2 paths will take most of the space, so store and calculate them here
    //path are stored for each cell
    std::vector<std::vector<std::vector<uint>>> m_exitPaths;
    std::vector<std::vector<std::vector<uint>>> m_enterPaths;
};

#endif //WORLDGRID_H_

