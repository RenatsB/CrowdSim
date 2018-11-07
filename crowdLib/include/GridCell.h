#ifndef GRIDCELL_H_
#define GRIDCELL_H_
#include <array>
#include <memory>
class GridCell
{
public:
std::shared_ptr<GridCell> cellAt(const uint _id);
bool hasSubGrid() const;
uint getCellID() const;
private:
uint m_cellID;
std::array<std::unique_ptr<GridCell>,4> m_subGrid;
}
#endif //GRIDCELL_H_
