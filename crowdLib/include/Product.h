#ifndef PRODUCT_H_
#define PRODUCT_H_
#include "Object.h"
#include <memory>
class Agent;
class Product: public Object
{
public:
void setOwner(std::shared_ptr<Agent> _new);
std::shared_ptr<Agent> getOwner() const;
private:
std::shared_ptr<Agent> m_owner;
};
#endif //PRODUCT_H_
