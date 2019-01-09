#include "Pathfinder.h"
#include "WorldGrid.h"

Pathfinder::Pathfinder(uint _sX, uint _sY, WorldGrid *_g)
{
    sizeX=_sX;
    sizeY=_sY;
    map.reserve(sizeY);
    map.resize(sizeY);
    for(auto &o : map)
    {
        o.reserve(sizeX);
        o.resize(sizeX);
    }
    closed_nodes_map.reserve(sizeY);
    closed_nodes_map.resize(sizeY);
    for(auto &o : closed_nodes_map)
    {
        o.reserve(sizeX);
        o.resize(sizeX);
    }
    open_nodes_map.reserve(sizeY);
    open_nodes_map.resize(sizeY);
    for(auto &o : open_nodes_map)
    {
        o.reserve(sizeX);
        o.resize(sizeX);
    }
    dir_map.reserve(sizeY);
    dir_map.resize(sizeY);
    for(auto &o : dir_map)
    {
        o.reserve(sizeX);
        o.resize(sizeX);
    }
    for(uint y=0; y<sizeY; ++y)
    {
        for(uint x=0; x<sizeX; ++x)
        {
            map.at(y).at(x) = _g->getAt(y*sizeX+x);
        }
    }
}
std::vector<uint> Pathfinder::getPath(const uint &xS, const uint &yS,const uint &xF, const uint &yF)
{
    std::vector<uint> retRoute;
    retRoute.push_back(yS*sizeX+xS); //push start

    std::string route=PathFind(xS, yS, xF, yF);

    // follow the route on the map and display it
    if(route.length()>0)
    {
        int j; char c;
        int x=xS;
        int y=yS;
        for(uint i=0;i<route.length();i++)
        {
            c =route.at(i);
            j=atoi(&c);
            x=x+dx[j];
            y=y+dy[j];
            retRoute.push_back(y*sizeX+x);
        }

    }
    retRoute.push_back(yF*sizeX+xF);
    return retRoute;
}

std::string Pathfinder::PathFind(const uint & xStart, const uint & yStart,
                                           const uint & xFinish, const uint & yFinish)
{
    static std::priority_queue<node> pq[2]; // list of open (not-yet-tried) nodes
    static int pqi; // pq index
    static node* n0;
    static node* m0;
    static int i, j, x, y, xdx, ydy;
    static char c;
    pqi=0;

    // reset the node maps
    for(uint y=0;y<sizeY;y++)
    {
        for(uint x=0;x<sizeX;x++)
        {
            closed_nodes_map[y][x]=0;
            open_nodes_map[y][x]=0;
        }
    }

    // create the start node and push into list of open nodes
    n0=new node(xStart, yStart, 0, 0);
    n0->updatePriority(xFinish, yFinish);
    pq[pqi].push(*n0);
    open_nodes_map[y][x]=n0->getPriority(); // mark it on the open nodes map

    // A* search
    while(!pq[pqi].empty())
    {
        // get the current node w/ the highest priority
        // from the list of open nodes
        n0=new node( pq[pqi].top().getxPos(), pq[pqi].top().getyPos(),
                     pq[pqi].top().getLevel(), pq[pqi].top().getPriority());

        x=n0->getxPos(); y=n0->getyPos();

        pq[pqi].pop(); // remove the node from the open list
        open_nodes_map[y][x]=0;
        // mark it on the closed nodes map
        closed_nodes_map[y][x]=1;

        // quit searching when the goal state is reached
        //if((*n0).estimate(xFinish, yFinish) == 0)
        if((uint)x==xFinish && (uint)y==yFinish)
        {
            // generate the path from finish to start
            // by following the directions
            std::string path="";
            while(!((uint)x==xStart && (uint)y==yStart))
            {
                j=dir_map[y][x];
                c='0'+(j+4)%8; //4 was dir/2 and 8 was dir
                path=c+path;
                x+=dx[j];
                y+=dy[j];
            }

            // garbage collection
            delete n0;
            // empty the leftover nodes
            while(!pq[pqi].empty()) pq[pqi].pop();
            return path;
        }

        // generate moves (child nodes) in all possible directions
        for(i=0;i<8;i++)
        {
            xdx=x+dx[i]; ydy=y+dy[i];

            if(!(xdx<0 || xdx>(int)sizeX-1 || ydy<0 || ydy>(int)sizeY-1 || map[ydy][xdx]==1
                || closed_nodes_map[ydy][xdx]==1))
            {
                // generate a child node
                m0=new node( xdx, ydy, n0->getLevel(),
                             n0->getPriority());
                m0->nextLevel(i);
                m0->updatePriority(xFinish, yFinish);

                // if it is not in the open list then add into that
                if(open_nodes_map[ydy][xdx]==0)
                {
                    open_nodes_map[ydy][xdx]=m0->getPriority();
                    pq[pqi].push(*m0);
                    // mark its parent node direction
                    dir_map[ydy][xdx]=(i+4)%8;
                }
                else if(open_nodes_map[ydy][xdx]>m0->getPriority())
                {
                    // update the priority info
                    open_nodes_map[ydy][xdx]=m0->getPriority();
                    // update the parent direction info
                    dir_map[ydy][xdx]=(i+4)%8;

                    // replace the node
                    // by emptying one pq to the other one
                    // except the node to be replaced will be ignored
                    // and the new node will be pushed in instead
                    while(!(pq[pqi].top().getxPos()==xdx &&
                           pq[pqi].top().getyPos()==ydy))
                    {
                        pq[1-pqi].push(pq[pqi].top());
                        pq[pqi].pop();
                    }
                    pq[pqi].pop(); // remove the wanted node

                    // empty the larger size pq to the smaller one
                    if(pq[pqi].size()>pq[1-pqi].size()) pqi=1-pqi;
                    while(!pq[pqi].empty())
                    {
                        pq[1-pqi].push(pq[pqi].top());
                        pq[pqi].pop();
                    }
                    pqi=1-pqi;
                    pq[pqi].push(*m0); // add the better node instead
                }
                else delete m0; // garbage collection
            }
        }
        delete n0; // garbage collection
    }
    return ""; // no route found
}
