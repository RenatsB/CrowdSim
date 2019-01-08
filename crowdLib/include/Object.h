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

Tag m_tag;
std::string m_name;
Colour m_colour;
Vec2 m_pos;
};

#endif //CROWDLIB_OBJECT_H_
