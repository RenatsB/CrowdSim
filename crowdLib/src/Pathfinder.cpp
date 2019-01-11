#include "Pathfinder.h"
#include "WorldGrid.h"
#include <algorithm>

Pathfinder::Pathfinder(Vec2 _gridDim, WorldGrid* _g)
{
    sizeX=_gridDim.x;
    sizeY=_gridDim.y;
    sourceMap.reserve(sizeY);
    sourceMap.resize(sizeY);
    for(auto &o : sourceMap)
    {
        o.reserve(sizeX);
        o.resize(sizeX);
    }
    map.reserve(sizeY);
    map.resize(sizeY);
    for(auto &o : map)
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
    closed_nodes_map.reserve(sizeY);
    closed_nodes_map.resize(sizeY);
    for(auto &o : closed_nodes_map)
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
            sourceMap.at(y).at(x) = _g->getAt(x,y);
            map.at(y).at(x) = _g->getAt(x,y);
        }
    }
}

std::vector<Vec2> Pathfinder::getPath(Vec2 _start, Vec2 _finish, bool _isRoom, Vec2 _rLim)
{
    for(uint y=0; y<sizeY; ++y)
    {
        for(uint x=0; x<sizeX; ++x)
        {
            //reset map to source
            map.at(y).at(x) = sourceMap.at(y).at(x);
        }
    }
    std::vector<Vec2> retRoute;
    //retRoute.push_back(_start);

    if(_isRoom)
    {
        for(uint y = 0; y<sizeY; ++y)
        {
            for(uint x = 0; x<sizeX; ++x)
            {
                if(x < _rLim.x || y < _rLim.y || x>sizeX-_rLim.x || y>sizeY-_rLim.y)
                {
                    //set values outside the room to 1 (wall)
                    map.at(y).at(x) = 1;
                }
            }
        }
    }
    else
    {
        for(uint y = _rLim.y; y<sizeY-_rLim.y; ++y)
        {
            for(uint x = _rLim.x; x<sizeX-_rLim.x; ++x)
            {
                //set values inside the room to 1 (wall)
                map.at(y).at(x) = 1;
            }
        }
    }
    std::string route=PathFind(_start, _finish);

    // follow the route on the map and display it
    if(route.length()>0)
    {
        int j; char c;
        int x=_start.x;
        int y=_start.y;
        for(uint i=0;i<route.length();i++)
        {
            c=route.at(i);
            j=atoi(&c);
            x=x+dx[j];
            y=y+dy[j];
            retRoute.push_back(Vec2(x,y));
            if(retRoute.back().x >= sizeX-1 && retRoute.back().y >= sizeY-1)
                retRoute.pop_back();
        }

    }
    retRoute.push_back(_finish);
    return retRoute;
}
/*
std::vector<Vec2> Pathfinder::getPath(Vec2 _start, Vec2 _finish, bool _isRoom, Vec2 _rLim)
{
    for(uint y=0; y<sizeY; ++y)
    {
        for(uint x=0; x<sizeX; ++x)
        {
            //reset map to source
            map.at(y).at(x) = sourceMap.at(y).at(x);
        }
    }
    std::vector<Vec2> retRoute;


    if(_isRoom)
    {
        for(uint y = 0; y<sizeY; ++y)
        {
            for(uint x = 0; x<sizeX; ++x)
            {
                if(x < _rLim.x || y < _rLim.y || x>sizeX-_rLim.x || y>sizeY-_rLim.y)
                {
                    //set values outside the room to 1 (wall)
                    map.at(y).at(x) = 1;
                }
            }
        }
    }
    else
    {
        for(uint y = _rLim.y; y<sizeY-_rLim.y; ++y)
        {
            for(uint x = _rLim.x; x<sizeX-_rLim.x; ++x)
            {
                //set values inside the room to 1 (wall)
                map.at(y).at(x) = 1;
            }
        }
    }
    retRoute = PathFind(_start, _finish);

    retRoute.push_back(yF*sizeX+xF);
    return retRoute;
}
*/

/*std::vector<Vec2> Pathfinder::PathFind(Vec2 _start, Vec2 _finish)
{
    //Returns a list of tuples as a path from the given start to the given end in the given maze

    // Create start and end node
    Node *start_node = new Node(nullptr, _start);
    //start_node.g = start_node.h = start_node.f = 0;
    Node *end_node = new Node(nullptr, _finish);
    //end_node.g = end_node.h = end_node.f = 0

    // Initialize both open and closed list
    open_nodes_map.clear();
    closed_nodes_map.clear();

    // Add the start node
    open_nodes_map.push_back(start_node);

    // Loop until you find the end
    while(open_nodes_map.size() > 0)
    {

        // Get the current node
        Node *current_node = open_nodes_map.at(0);
        uint current_index = 0;
        for(uint index=0; index<open_nodes_map.size(); ++index)
        {
            if(open_nodes_map.at(index)->f < current_node->f)
            {
                current_node = open_nodes_map.at(index);
                current_index = index;
            }
        }

        // Pop current off open list, add to closed list
        open_nodes_map.erase(open_nodes_map.begin()+current_index);
        closed_nodes_map.push_back(current_node);

        // Found the goal
        if(current_node == end_node)
        {
            std::vector<Vec2> path;
            Node* current = current_node;
            while(current != nullptr)
            {
                path.push_back(current->pos);
                current = current->parent;
            }
            std::reverse(path.begin(),path.end());
            return path; // Return reversed path
        }
        m_dir=Direction::CURRENT;
        // Generate children
        std::vector<Node*> children;
        int dirs[8][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
        for(auto new_position : dirs) // Adjacent squares
        {
            // Get node position
            Vec2 node_position = Vec2(current_node->pos.x + new_position[0],current_node->pos.y + new_position[1]);

            // Make sure within range
            if(node_position.x > (int)sizeX - 1 || node_position.x < 0 || node_position.y > (int)sizeY - 1 || node_position.y < 0)
            {
                continue;
            }

            // Make sure walkable terrain
            if(map.at(node_position.y).at(node_position.x) != 0)
            {
                continue;
            }

            // Create new node
            Node* new_node = new Node(current_node, node_position);

            // Append
            children.push_back(new_node);
        }
        // Loop through children
        for(auto child : children)
        {
            // Child is on the closed list
            for(auto closed_child : closed_nodes_map)
            {
                if(child == closed_child)
                {
                    continue;
                }
            }
            // Create the f, g, and h values
            child->g = current_node->g + 1;
            child->h = ((child->pos.x - end_node->pos.x) * (child->pos.x - end_node->pos.x))
                     + ((child->pos.y - end_node->pos.y) * (child->pos.y - end_node->pos.y));
            child->f = child->g + child->h;

            // Child is already in the open list
            for(auto open_node : open_nodes_map)
            {
                if(child == open_node && child->g > open_node->g)
                {
                    continue;
                }
            }
            // Add the child to the open list
            open_nodes_map.push_back(child);
        }
    }
    std::vector<Vec2> ret;
    return ret;
}*/


std::string Pathfinder::PathFind(Vec2 _start, Vec2 _finish)
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
            closed_nodes_map.at(y).at(x)=0;
            open_nodes_map.at(y).at(x)=0;
        }
    }

    // create the start node and push into list of open nodes
    n0=new node(_start.x, _start.y, 0, 0);
    n0->updatePriority(_finish.x, _finish.y);
    pq[pqi].push(*n0);
    open_nodes_map.at(y).at(x)=n0->getPriority(); // mark it on the open nodes map

    // A* search
    while(!pq[pqi].empty())
    {
        // get the current node w/ the highest priority
        // from the list of open nodes
        n0=new node( pq[pqi].top().getxPos(), pq[pqi].top().getyPos(),
                     pq[pqi].top().getLevel(), pq[pqi].top().getPriority());

        x=n0->getxPos(); y=n0->getyPos();

        pq[pqi].pop(); // remove the node from the open list
        open_nodes_map.at(y).at(x)=0;
        // mark it on the closed nodes map
        closed_nodes_map.at(y).at(x)=1;

        // quit searching when the goal state is reached
        //if((*n0).estimate(xFinish, yFinish) == 0)
        if((uint)x==_finish.x && (uint)y==_finish.y)
        {
            // generate the path from finish to start
            // by following the directions
            std::string path="";
            while(!((uint)x==_start.x && (uint)y==_start.y))
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
                m0->updatePriority(_finish.x, _finish.y);

                // if it is not in the open list then add into that
                if(open_nodes_map.at(ydy).at(xdx)==0)
                {
                    open_nodes_map.at(ydy).at(xdx)=m0->getPriority();
                    pq[pqi].push(*m0);
                    // mark its parent node direction
                    dir_map.at(ydy).at(xdx)=(i+4)%8;
                }
                else if((int)open_nodes_map.at(ydy).at(xdx)>m0->getPriority())
                {
                    // update the priority info
                    open_nodes_map.at(ydy).at(xdx)=m0->getPriority();
                    // update the parent direction info
                    dir_map.at(ydy).at(xdx)=(i+4)%8;

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
