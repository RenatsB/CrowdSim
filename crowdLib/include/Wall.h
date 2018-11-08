#ifndef WALL_H_
#define WALL_H_
#include "Object.h"
#include <array>
class Wall: public Object
{
public:
    std::array<Vec2, 4> getCorners() const;
private:
    std::array<Vec2, 4> m_corners;
};

#endif //WALL_H_
