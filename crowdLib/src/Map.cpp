#include "Map.h"
#include "RandF.h"
#include "Params.h"

void Map::setAt(Vec2 _cell, uint _v)
{
    m_values.at(_cell.x+_cell.y*sizeX) = _v;
}

void Map::setAt(uint _x, uint _y, uint _v)
{
    m_values.at(_x+_y*sizeX) = _v;
}

void Map::initMap()
{
///The map is defined as follows:
/// 0 - empty
/// 1 - wall
/// 2 - entrance
/// 3 - exit
/// 4 - inout
/// 5 - shelf
///

    m_values.reserve(sizeX*sizeY);
    m_values.resize(sizeX*sizeY);

    //init all to 0
    for(uint u=0; u<m_values.size(); ++u)
        m_values.at(u) = 0;

    uint roomSize = m_randf->randi(16, m_params->nav_maxRoomDim,0);

    //make walls
    for(uint y=sizeY/2-roomSize/2; y<sizeY/2+roomSize/2; ++y)
    {
        setAt(sizeX/2-roomSize/2,y,1); //  set  |    |
        setAt(sizeX/2+roomSize/2,y,1); //       |    |
    }
    for(uint x=sizeX/2-roomSize/2; x<sizeX/2-roomSize/2; ++x)
    {
        setAt(x,sizeX/2+roomSize/2,1); //  set  ------
        setAt(x,sizeX/2+roomSize/2,1); //       ------
    }

    //make holes
    uint numExits = m_randf->randi(0, m_params->nav_maxNumExits,0);
    uint numEntrances = m_randf->randi(0, m_params->nav_maxNumEntrances,0);
    uint numInouts = m_randf->randi(1, m_params->nav_maxNumInouts,0);
    std::vector<Vec2> takenXY;
    for(uint i=0; i<(numExits+numEntrances+numInouts); ++i)
    {
        Vec2 p;
        int k = m_randf->randi(0, 4, 0);
        if(k==0){
            p.y = 0;
            bool chk = false;
            do
            {
                p.x=m_randf->randi(0,roomSize);
                for(auto g : takenXY)
                {
                    if(g == p)
                    {
                        chk = true;
                    }
                }
            }
            while(chk);
        }
        if(k==1){
            p.x = 1;
            bool chk = false;
            do
            {
                p.y=m_randf->randi(0,roomSize);
                for(auto g : takenXY)
                {
                    if(g == p)
                    {
                        chk = true;
                    }
                }
            }
            while(chk);
        }
        if(k==2){
            p.y = 1;
            bool chk = false;
            do
            {
                p.x=m_randf->randi(0,roomSize);
                for(auto g : takenXY)
                {
                    if(g == p)
                    {
                        chk = true;
                    }
                }
            }
            while(chk);
            p.x=m_randf->randi(0,roomSize);
        }
        if(k==3){
            p.x = 0;
            bool chk = false;
            do
            {
                p.y=m_randf->randi(0,roomSize);
                for(auto g : takenXY)
                {
                    if(g == p)
                    {
                        chk = true;
                    }
                }
            }
            while(chk);
        }
        takenXY.push_back(p);
        if(i<numExits)
        {
            setAt(p, 3);
        }
        else
        {
            if(i<(numExits+numEntrances))
            {
                setAt(p, 2);
            }
            else
            {
                setAt(p, 4);
            }
        }
    }

    //make shelves
    //TODO
}
