#ifndef CROWDLIB_PRODUCT_H_
#define CROWDLIB_PRODUCT_H_
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
#endif //CROWDLIB_PRODUCT_H_
