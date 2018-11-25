#include "AgentQuadTree.h"

/*void AgentQuadtree::checkCollisionOnNode(TreeNode<Agent> *node)
{
    if(node->m_height !=1)
        {
            for(int i=0;i<8;++i)
            {
                checkCollisionOnNode(node->m_child[i]);
            }
        }
        else
        {
            if(node->m_objectList.size()<=1)
            {
                return;
            }
            Vec2 P, Q, Vp, N, force;
            float Rp, Rq, dist, speed;
            for(auto currentAgent : node->m_objectList)
            {
                P = currentAgent->getPosition();
                Vp = currentAgent->getCurrentSpeed();
                speed = Vp.length();
                Rp = currentAgent->getRadius();
                force.m_x = force.m_y = force.m_z = 0.0;

                for(auto testAgent : node->m_objectList )
                {
                    // no need to self test
                    if(testAgent==currentParticle)
                    {
                      // continue
                      continue;
                    }
                    Q = testAgent->getPosition();
                    Rq = testAgent->getRadius();
                    dist = Rq + Rq - (P-Q).length();
                    if(dist > 0) // collision between P and Q
                    {
                      N = P-Q;
                      N.normalize();
                      force += dist*N;
                    }
                }
                Vp += force;
                Vp.normalize();
                currentAgent->setSpeed(Vp*speed);
            }
        }
}*/
