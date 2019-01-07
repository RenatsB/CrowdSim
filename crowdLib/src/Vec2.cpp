#include "Vec2.h"
#include <cmath>
//-----------------------------------------------------------------------------------------------------
float Vec2::distance(const Vec2 *_tgt)
{
    Vec2 a = *this-*_tgt;
    return Vec2::magnitude(a);
}
float Vec2::distance(const Vec2 &_tgt)
{
    Vec2 a = *this-_tgt;
    return Vec2::magnitude(a);
}
//-----------------------------------------------------------------------------------------------------
float Vec2::distance(Vec2 *_a, Vec2 *_b)
{
    Vec2 a = (*_a)-(*_b);
    return Vec2::magnitude(a);
}
float Vec2::distance(Vec2 &_a, Vec2 &_b)
{
    Vec2 a = _a-_b;
    return Vec2::magnitude(a);
}
//-----------------------------------------------------------------------------------------------------
float Vec2::magnitude(Vec2 &_v)
{
    return sqrt(_v.x*_v.x + _v.y*_v.y);
}
//-----------------------------------------------------------------------------------------------------
float Vec2::magnitude()
{
    return sqrt(x*x + y*y);
}
//-----------------------------------------------------------------------------------------------------
void Vec2::normalize(Vec2 *_v)
{
    float len = Vec2::magnitude(*_v);
    _v->x/=len;
    _v->y/=len;
}
//-----------------------------------------------------------------------------------------------------
void Vec2::normalize()
{
    float len = Vec2::magnitude();
    x/=len;
    y/=len;
}
//-----------------------------------------------------------------------------------------------------
