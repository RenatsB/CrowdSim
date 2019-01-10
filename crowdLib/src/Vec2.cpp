#include "Vec2.h"
#include <cmath>
//-----------------------------------------------------------------------------------------------------
float Vec2::distance(const Vec2 *_tgt) const
{
    return sqrt((x-_tgt->x)*(x-_tgt->x) + (y-_tgt->y)*(y-_tgt->y));
}
float Vec2::distance(const Vec2 &_tgt) const
{
    return sqrt((x-_tgt.x)*(x-_tgt.x) + (y-_tgt.y)*(y-_tgt.y));
}
//-----------------------------------------------------------------------------------------------------
float Vec2::distance(Vec2 *_a, Vec2 *_b) const
{
    return sqrt((_a->x-_b->x)*(_a->x-_b->x) + (_a->y-_b->y)*(_a->y-_b->y));
}
float Vec2::distance(Vec2 &_a, Vec2 &_b) const
{
    return sqrt((_a.x-_b.x)*(_a.x-_b.x) + (_a.y-_b.y)*(_a.y-_b.y));
}
//-----------------------------------------------------------------------------------------------------
float Vec2::magnitude(Vec2 &_v) const
{
    return sqrt(_v.x*_v.x + _v.y*_v.y);
}
//-----------------------------------------------------------------------------------------------------
float Vec2::magnitude() const
{
    return sqrt(x*x + y*y);
}
//-----------------------------------------------------------------------------------------------------
void Vec2::normalize(Vec2 *_v)
{
    float len = _v->magnitude();
    if(len!=0.f)
    {
        _v->x/=len;
        _v->y/=len;
    }
}
//-----------------------------------------------------------------------------------------------------
void Vec2::normalize()
{
    float len = this->magnitude();
    if(len!=0.f)
    {
        this->x/=len;
        this->y/=len;
    }
}
//-----------------------------------------------------------------------------------------------------
