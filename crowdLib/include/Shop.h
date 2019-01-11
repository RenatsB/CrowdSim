#ifndef CROWDLIB_SHOP_H_
#define CROWDLIB_SHOP_H_
#include <vector>
#include <memory>
#include "Product.h"
#include "Params.h"

class RandF;
class WorldGrid;

class BoundingBox;

class Shop: private Object
{
public:
    Shop(std::shared_ptr<Params> _prms, WorldGrid* _g):
        m_params(_prms)
    {
        m_grid = _g;
    }
    ~Shop()=default;
    std::vector<std::shared_ptr<Product>> getRemainingProducts() const;
    std::vector<std::shared_ptr<Product>> getfreeProducts() const;
    void setExits(std::vector<Vec2> _ex);
    std::vector<Vec2> getExits() const;
    uint getNumRemainingProducts() const;
    float getRemainingProductStress() const;
    float getDistanceStress() const;
    void update();
    void spawnProducts(BoundingBox _bb);
    std::vector<Vec2> getProductPositions();
private:
    std::vector<std::shared_ptr<Product>> m_products;
    std::vector<std::shared_ptr<Product>> m_remp;
    std::vector<std::shared_ptr<Product>> m_freep;
    std::vector<Vec2> m_exits;
    std::shared_ptr<Params> m_params;
    std::shared_ptr<RandF> m_randF;
    WorldGrid* m_grid;
};
#endif //CROWDLIB_SHOP_H_
