#ifndef WALL_H_
#define WALL_H_
#include "Object.h"
#include <array>
class Wall: public Object
{
public:

    /*
     * CORNERS:
     * ,----------,
     * |0        1|
     * |          |
     * |          |
     * |2        3|
     * '----------'
     *
     */

    Wall(float _x, float _y, float _sizex, float _sizey):
    {
        m_corners.at(0).x = _x;
        m_corners.at(0).y = _y;
        m_corners.at(1).x = _x+_sizex;
        m_corners.at(1).y = _y;
        m_corners.at(2).x = _x;
        m_corners.at(2).y = _y+_sizey;
        m_corners.at(3).x = _x+_sizex;
        m_corners.at(3).y = _y+_sizey;
    }
    ~Wall()=default;
    std::array<Vec2, 4> getCorners() const
    {
        return m_corners;
    }
private:
    std::array<Vec2, 4> m_corners;
};

#endif //WALL_H_
