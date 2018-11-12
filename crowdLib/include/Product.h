#ifndef PRODUCT_H_
#define PRODUCT_H_
#include "Object.h"
#include <memory>
class Agent;
class Product: public Object
{
public:
void setOwner(Agent* _new);
Agent* getOwner() const;
private:
Agent* m_owner;
};
#endif //PRODUCT_H_
