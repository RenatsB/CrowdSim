#ifndef GRID_H_
#define GRID_H_
#include "GridCell.h"
class Grid
{
public:
    Grid(const uint _depth)
    {
        uint d=1;
        std::shared_ptr<GridCell> ref = nullptr;
        for(uint y=0; y<m_rootLevel.size(); ++y)
        {
            GridCell newCell;
            newCell.setCellID(y);
            m_rootLevel.at(y) = *newCell;
        }
        while(d!=_depth)
        {

            //init all children
            //for(uint i =0; i<m_subGrid)
        }
    }
    std::shared_ptr<GridCell> cellAt(const uint _id);
    std::shared_ptr<GridCell> cellsAtLevel(const uint _depth);
private:
    std::array<std::unique_ptr<GridCell>,4> m_rootLevel;
};
#endif //GRID_H_
