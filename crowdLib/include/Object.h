#ifndef OBJECT_H_
#define OBJECT_H_
#include "Vec2.h"
#include <string>
enum class Tag {AGENT, PRODUCT, WALL, ENTRANCE, EXIT};
class Object
{
public:
Tag m_tag;
std::string m_name;
Vec2 m_pos;
}

#endif //OBJECT_H_
