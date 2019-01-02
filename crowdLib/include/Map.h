#ifndef MAP_H_
#define MAP_H_
#include <vector>
#include "Vec2.h"

class Params;
class RandF;
using uint = unsigned int;

class Map
{
public:
    Map(Params* _prms, RandF* _rand, uint _sX, uint _sY):
    {
        m_params = _prms;
        m_randf = _rand;
        sizeX = _sX;
        sizeY = _sY;
        initMap();
        calcPaths();
    }
    ~Map()=default;
    std::vector<uint> getMap() {return m_values;}
    void setAt(Vec2 _cell, uint _v);
    void setAt(uint _x, uint _y, uint _v);
    uint getAt(Vec2 _cell) {return m_values.at(_cell.x+_cell.y*sizeX);}
    std::vector<Vec2> getExits;
    std::vector<Vec2> getEntrances;
    std::vector<Vec2> getInouts;
    std::vector<uint> getWalls;
    std::vector<std::vector<uint>> getExitPaths;
    std::vector<std::vector<uint>> getEnterPaths;
    uint randomToExit;
    std::vector<uint> randPathToExit;
    uint randomToEntrance;
    std::vector<uint> randPathToEntrance;
private:
    void initMap();
    void calcPaths();
    std::vector<std::vector<uint>> DijkstraComputePaths(bool _type);


    uint sizeX = 20;
    uint sizeY = 20;
    std::vector<uint> m_values;
    RandF* m_randf;
    Params* m_params;
    //these 2 paths will take most of the space, so store and calculate them here
    std::vector<std::vector<uint>> m_exitPaths;
    std::vector<std::vector<uint>> m_enterPaths;
};

#endif //MAP_H_
