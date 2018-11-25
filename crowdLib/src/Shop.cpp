#include "Shop.h"

std::vector<std::shared_ptr<Product>> Shop::getRemainingProducts() const
{
    return m_remp;
}

std::vector<std::shared_ptr<Wall>> Shop::getWalls() const
{
    return m_walls;
}

std::vector<std::shared_ptr<Object> > Shop::getExits() const
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
    //since the number of exits is expected to be small, we can get away with
    //checking against all exits. If necessary, this can be modified
    //to use space hashing if user decides to have insanely large amounts of exits
    float stress = 0.f;
    for(auto ob : m_remp)
    {
        float minDist = 1000000.f;
        std::shared_ptr<Object> ret;
        for(auto ex : m_exits)
        {
            float d = ob.get()->m_pos.distance(ex.get()->m_pos);
            if(d < minDist)
            {
                ret = ex;
                minDist = d;
            }
        }
        if(minDist <= m_params.get()->stress_distanceToExits)
        {
            stress+=((minDist/m_params.get()->stress_distanceToExits)/m_remp.size());
        }
    }
    return stress;
}

void Shop::update()
{
    std::vector<std::shared_ptr<Product>> rem;
    for(auto i : m_products)
    {
        if(i.get()->m_tag == Tag::PRODUCT)
            rem.push_back(i);
    }
    m_remp = rem;

    std::vector<std::shared_ptr<Product>> free;
    for(auto u : rem)
    {
        if(u.get()->getOwner() == nullptr)
            free.push_back(u);
    }
    m_freep = free;
}
