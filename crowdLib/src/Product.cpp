#include "Product.h"

void Product::setOwner(Agent* _new)
{
    m_owner = _new;
}

Agent* Product::getOwner() const
{
    return m_owner;
}
