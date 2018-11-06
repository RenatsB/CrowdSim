#ifndef OBJECT_H_
#define OBJECT_H_
#include "Vec2.h"
#include "Colour.h"
#include <string>
#include "EnumClasses.h"
class Object
{
public:
Tag m_tag;
std::string m_name;
Colour m_colour;
Vec2 m_pos;
};

#endif //OBJECT_H_
