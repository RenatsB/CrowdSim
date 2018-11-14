#include "Shop.h"

std::vector<std::shared_ptr<Product>> Shop::getRemainingProducts() const
{
    std::vector<std::shared_ptr<Product>> rem;
    for(auto i : m_products)
    {
        if(i.get()->m_tag == Tag::PRODUCT)
            rem.push_back(i);
    }
    return rem;
}

std::vector<std::shared_ptr<Wall>> Shop::getWalls() const
{
    return m_walls;
}

std::vector<std::shared_ptr<Object>> Shop::getExits() const
{
    return m_exits;
}

uint Shop::getNumRemainingProducts() const
{
    return m_remp.size();
}

float Shop::getRemainingProductStress() const
{
    return m_remp.size()/m_products.size();
}

float Shop::getDistanceStress() const
{

}

void Shop::update()
{

}
