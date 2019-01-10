#include "Object.h"

void Object::setPosition(Vec2 _p)
{
    m_pos = _p;
}

Vec2 Object::getPosition()
{
    return m_pos;
}

void Object::addPosition(Vec2 _d)
{
    m_pos+=_d;
}

void Object::setTag(Tag _t)
{
    m_tag = _t;
}

Tag Object::getTag()
{
    return m_tag;
}
