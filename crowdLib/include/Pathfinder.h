#ifndef CROWDLIB_PATHFINDER_H_
#define CROWDLIB_PATHFINDER_H_
#include <iostream>
#include <iomanip>
#include <queue>
#include <string>
#include <math.h>
#include <ctime>
#include <vector>
#include <memory>
//below code was adapted from : http://code.activestate.com/recipes/577457-a-star-shortest-path-algorithm/
class RandF;

class node
{
// current position
int xPos;
int yPos;
// total distance already travelled to reach the node
int level;
// priority=level+remaining distance estimate
int priority;  // smaller: higher priority

public:
    node(int xp, int yp, int d, int p)
        {xPos=xp; yPos=yp; level=d; priority=p;}

    int getxPos() const {return xPos;}
    int getyPos() const {return yPos;}
    int getLevel() const {return level;}
    int getPriority() const {return priority;}

    void updatePriority(const int & xDest, const int & yDest)
    {
         priority=level+estimate(xDest, yDest)*10; //A*
    }

    // give better priority to going strait instead of diagonally
    void nextLevel(const int & i) // i: direction
    {
         level+=(i%2==0?10:14);
    }

    // Estimation function for the remaining distance to the goal.
    const int & estimate(const int & xDest, const int & yDest) const
    {
        static int xd, yd, d;
        xd=xDest-xPos;
        yd=yDest-yPos;

        // Euclidian Distance
        d=static_cast<int>(sqrt(xd*xd+yd*yd));

        // Manhattan distance
        //d=abs(xd)+abs(yd);

        // Chebyshev distance
        //d=max(abs(xd), abs(yd));

        return(d);
    }
};

// Determine priority (in the priority queue)
inline bool operator<(const node & a, const node & b)
{
  return a.getPriority() > b.getPriority();
}
class WorldGrid;

class Pathfinder
{
public:
Pathfinder(uint _sX, uint _sY, WorldGrid* _g);
~Pathfinder()=default;

//adaptation of the output of original function
std::vector<uint> getPath(const uint &xS, const uint &yS,
                          const uint &xF, const uint &yF);
private:
// A-star algorithm.
// The route returned is a string of direction digits.
std::string PathFind(const uint & xStart, const uint & yStart,
                     const uint & xFinish, const uint & yFinish);
private:
std::shared_ptr<RandF> m_randF;
int dx[8]={1, 1, 0, -1, -1, -1, 0, 1};
int dy[8]={0, 1, 1, 1, 0, -1, -1, -1};
//std::array<std::pair<uint,uint>> m_dir={{1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1}};
uint sizeX=60; // horizontal size of the map
uint sizeY=60; // vertical size size of the map
std::vector<std::vector<int>> map;
std::vector<std::vector<int>> closed_nodes_map; // map of closed (tried-out) nodes
std::vector<std::vector<int>> open_nodes_map; // map of open (not-yet-tried) nodes
std::vector<std::vector<int>> dir_map; // map of directions
};

#endif //CROWDLIB_PATHFINDER_H_
