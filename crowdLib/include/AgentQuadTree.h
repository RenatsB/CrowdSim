#ifndef AGENTQUADTREE_H_
#define AGENTQUADTREE_H_
#include "Vec2.h"
#include "Agent.h"
#include "AbstractQuadTree.h"

/// @file AgentQuadTree.h
/// @brief Modified from ParticleOctree.h from https://github.com/NCCA/OctreeAbstract
/// @author Renats Bikmajevs, adapted from: Xiaosong Yang modifed by Jon Macey

class AgentQuadtree: public AbstractQuadtree <Agent,Vec2>
{
public:
AgentQuadtree(int _height, BoundingBox _limit):
    AbstractQuadtree <Agent,Vec2> ( _height,  _limit)
{}
std::vector<Agent*> getNeighbours(Agent* _ref);
TreeNode<Agent> getAgentNode(Agent* _ref);

virtual void  checkCollisionOnNode(TreeNode <Agent> *node);
};

#endif //AGENTQUADTREE_H_

