#ifndef SHOP_H_
#define SHOP_H_
#include <vector>
#include <memory>
#include "Product.h"
#include "Map.h"
#include "Params.h"
class RandF;

class Shop: private Object
{
public:
    Shop(std::shared_ptr<Params> _p, RandF* _rand):
    {
        m_params(_p);
        m_map = new Map(_rand, m_params.get()->nav_gridSizeX, m_params.get()->nav_gridSizeY);
    }
    ~Shop()=default;
    std::vector<std::shared_ptr<Product>> getRemainingProducts() const;
    std::vector<std::shared_ptr<Product>> getfreeProducts() const;
    std::vector<uint> getWalls() const;
    std::vector<std::shared_ptr<Object>> getExits() const;
    uint getNumRemainingProducts() const;
    float getRemainingProductStress() const;
    float getDistanceStress() const;
    void update();
private:
    std::vector<std::shared_ptr<Product>> m_products;
    std::vector<std::shared_ptr<Product>> m_remp;
    std::vector<std::shared_ptr<Product>> m_freep;
    std::vector<std::shared_ptr<Object>> m_exits;
    std::shared_ptr<Params> m_params;
    Map* m_map;
};
#endif //SHOP_H_
