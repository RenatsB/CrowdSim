#ifndef ABSTRACTQUADTREE__H_
#define ABSTRACTQUADTREE__H_
#include <vector>
#include <iostream>
#include <array>
#include "Vec2.h"
#include <memory>

/// @file AbstractQuadTree.h
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
    float m_minx;
    float m_maxx;
    float m_miny;
    float m_maxy;
};


template <class T, class VEC> class AbstractQuadtree
{
protected :

  template <class V>class TreeNode
  {
  public:
      BoundingBox m_limit;
      int m_height;
      std::vector<V *> m_objectList;
      std::array<TreeNode *,4> m_child;
  };

public:
    /// @brief constructor
    /// @param[in] _height the height of the octree, should be 3 in this example
    /// @param[in] _limit the Bounding box of the tree
    AbstractQuadtree(int _height, BoundingBox _limit)
    {
      if(_height<=0)
      {
        std::cerr<<"The height of the tree is not valid\n";
        exit(EXIT_FAILURE);
      }
      m_root = new TreeNode <T>;
      createTree(m_root, _height, _limit);
    }

    /// @brief destructor
    virtual ~AbstractQuadtree()
    {
        deleteTreeNode(m_root);
        delete m_root;
    }

    void deleteTreeNode(TreeNode <T> *_node)
    {
      for(int i=0;i<4;++i)
      {
        if(_node->m_child[i]!=0)
        {
          deleteTreeNode(_node->m_child[i]);
          delete _node->m_child[i];
        }
      }
    }
    /// @brief create perfect tree, it is leaves level when _height==1
    void createTree(TreeNode <T> *_parent,  int _height, BoundingBox _limit)
    {
      _parent->m_limit = _limit;
      _parent->m_height = _height;
      _parent->m_objectList.clear();
      _height--;
      if(_height == 0) // current level is leaves, no children
      {
        for(int i=0;i<4;++i)
        {
         _parent->m_child[i]= 0;
        }
      }
      else
      {
        BoundingBox childLimit;
        for(int i=0;i<4;++i)
        {
          _parent->m_child[i] = new TreeNode <T>;
          if(i%2==0) // i = 0, 2
          {
            childLimit.m_minx = _limit.m_minx;
            childLimit.m_maxx = (_limit.m_maxx+_limit.m_minx)/2.0;
          }
          else // i = 1, 3
          {
            childLimit.m_minx = (_limit.m_maxx+_limit.m_minx)/2.0;
            childLimit.m_maxx = _limit.m_maxx;
          }
          if(i==0 || i==1)
          {
            childLimit.m_miny = _limit.m_miny;
            childLimit.m_maxy = (_limit.m_maxy + _limit.m_miny)/2.0;
          }
          else
          {
            childLimit.m_miny = (_limit.m_maxy + _limit.m_miny)/2.0;
            childLimit.m_maxy = _limit.m_maxy;
          }
         createTree(_parent->m_child[i], _height, childLimit);
        } // end for
      }// end else
    }

    /// @brief add a T into the tree, add the partile to all the leaves collide with
    void addObject(T *_a)
    {
      addObjectToNode(m_root, _a);
    }
    /// @brief some template specialization to hand different vector types
    bool checkBounds (const Vec2 &_pos, float _r, const BoundingBox &_limit)
    {
      return (_pos.x-_r > _limit.m_maxx ||
              _pos.x+_r < _limit.m_minx ||
              _pos.y-_r > _limit.m_maxy ||
              _pos.y+_r < _limit.m_miny
              );
    }
    void addObjectToNode(TreeNode <T>  *_node, T *_a)
    {
      if(_node->m_height == 1) // this is the leaves level
      {
        _node->m_objectList.push_back(_a);
      }
      else
      {
        VEC pos = _a->getPosition();
        float r = _a->getRadius();
        BoundingBox limit;
        for(int i=0;i<4;++i)
        {
          limit = _node->m_child[i]->m_limit;
          if(checkBounds(pos,r,limit))
          {
              continue;
          }
          addObjectToNode(_node->m_child[i], _a);
        }
      }
    }

    /// @brief clear off all the Ts from the tree
    void clearTree()
    {
        clearObjectFromNode(m_root);
    }

    void clearObjectFromNode(TreeNode <T> *_node)
    {
      if(_node->m_height == 1)
      {
        _node->m_objectList.clear();
      }
      else
      {
        for(int i=0;i<4;++i)
        {
            clearObjectFromNode(_node->m_child[i]);
        }
      }
    }

    /// @brief check the collision for the Ts in each leaves
    void checkCollision()
    {
        checkCollisionOnNode(m_root);
    }

    virtual void checkCollisionOnNode(TreeNode <T> *_node)=0;

  protected :
    TreeNode <T> *m_root;
};

#endif //ABSTRACTQUADTREE_H_

