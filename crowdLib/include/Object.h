#ifndef CROWDLIB_OBJECT_H_
#define CROWDLIB_OBJECT_H_
#include "Vec2.h"
#include "Colour.h"
#include <string>
#include "EnumClasses.h"
class Object
{
public:
Object()=default;
~Object()=default;
void setPosition(Vec2 _p);
Vec2 getPosition();
void addPosition(Vec2 _d);
void setTag(Tag _t);
Tag getTag();
protected:
Tag m_tag;
std::string m_name;
Colour m_colour;
Vec2 m_pos = Vec2(0.f,0.f);
};

#endif //CROWDLIB_OBJECT_H_
