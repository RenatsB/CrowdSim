#ifndef GRID_H_
#define GRID_H_
#include "GridCell.h"
class Grid
{
public:
std::shared_ptr<GridCell> cellAt(const uint _id);
private:
std::array<std::unique_ptr<GridCell>,4> m_rootLevel;
}
#endif //GRID_H_
