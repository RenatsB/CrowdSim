#include "Shop.h"
#include "WorldGrid.h"
#include "RandF.h"

void Shop::spawnProducts(BoundingBox _bb)
{
    m_products.clear();
    m_products.reserve(m_params.get()->entity_numProducts);
    m_products.resize(m_params.get()->entity_numProducts);
    for(auto &p : m_products)
    {
        p = std::make_shared<Product>();
        p.get()->setTag(Tag::PRODUCT);
        //p.get()->setPosition(Vec2(0.f,0.f));
        p.get()->setPosition(Vec2(m_randF.get()->randf(_bb.m_minx, _bb.m_maxx),m_randF.get()->randf(_bb.m_miny, _bb.m_maxy)));
    }
}

void Shop::setExits(std::vector<Vec2> _ex)
{
    m_exits.clear();
    m_exits = _ex;
}

std::vector<Vec2> Shop::getExits() const
{
    return m_exits;
}

std::vector<std::shared_ptr<Product>> Shop::getRemainingProducts() const
{
    return m_remp;
}

std::vector<std::shared_ptr<Product>> Shop::getfreeProducts() const
{
    return m_freep;
}

std::vector<Vec2> Shop::getProductPositions()
{
    std::vector<Vec2> ret;
    for(auto &p : m_products)
    {
        ret.push_back(p.get()->getPosition());
    }
    return ret;
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
    for(auto &ob : m_remp)
    {
        float minDist = 1000000.f;
        Vec2 ret;
        for(auto &ex : m_exits)
        {
            float d = ob.get()->getPosition().distance(ex);
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
    for(auto &i : m_products)
    {
        if(i.get()->getTag() == Tag::PRODUCT)
            rem.push_back(i);
    }
    m_remp = rem;

    std::vector<std::shared_ptr<Product>> free;
    for(auto &u : rem)
    {
        if(u.get()->getOwner() == nullptr)
            free.push_back(u);
    }
    m_freep = free;
}
