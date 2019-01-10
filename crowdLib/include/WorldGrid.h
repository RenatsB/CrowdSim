#ifndef CROWDLIB_WORLDGRID_H_
#define CROWDLIB_WORLDGRID_H_
#include <vector>
#include <memory>
#include "Agent.h"
#include "Shop.h"
#include "NameMaker.h"
/// @file WorldGrid.h
/// @brief Space separation and object management module
/// @author Renats Bikmajevs

class Pathfinder;
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
    WorldGrid(std::shared_ptr<Params> _prms, uint _sX, uint _sY,
              BoundingBox _lim, std::shared_ptr<Time> _tm);
    /// @brief destructor
    ~WorldGrid();

    uint getAt(Vec2 _cell);
    uint getAt(uint _cell);
    GridCell* cellAt(Vec2 _cell);
    GridCell* cellAt(uint _x, uint _y);
    GridCell* cellAt(uint _i);
    float getCellDim() const;
    void update();
    std::vector<Agent*> getAgents();
    std::vector<GridCell*> getWalls();
    std::vector<Vec2> getProducts();
    void addAgent(Agent *_a);
    void addAgent(Agent *_a, GridCell* _cell);
    uint randomToExit(uint _current) const;
    std::vector<uint> randPathToExit(uint _current) const;
    uint randomToEntrance(uint _current) const;
    std::vector<uint> randPathToEntrance(uint _current) const;
    bool insideRoom(Vec2 _pos);
    Vec2 getGridMidpoint();
    float getKillRadius();
private:
    void initGrid();
    void initMap();
    void spawnAgents();
    void calcPaths();
    void AStarPathfind();
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
    std::shared_ptr<RandF> m_randF;
    std::shared_ptr<Params> m_params;
    std::shared_ptr<Shop> m_shop;
    std::shared_ptr<Time> m_time;
    uint m_cellDimX = 1;
    uint m_cellDimY = 1;
    uint m_gridSizeX = 1;
    uint m_gridSizeY = 1;
    float m_navPTDistance = 0.25f;
    float m_killRadius;
    Vec2 m_midPoint;
    BoundingBox m_limit;
    BoundingBox m_roomLimit;
    std::vector<GridCell*> m_cells;
    std::vector<Vec2> m_exits;
    std::vector<Vec2> m_entrances;
    std::vector<GridCell*> m_walls;
    //these 2 paths will take most of the space, so store and calculate them here
    //path are stored for each cell
    std::vector<std::vector<std::vector<uint>>> m_exitPaths;
    std::vector<std::vector<std::vector<uint>>> m_enterPaths;
    std::shared_ptr<NameMaker> m_nmaker;
    std::shared_ptr<Pathfinder> m_pfinder;
};

#endif //CROWDLIB_WORLDGRID_H_

