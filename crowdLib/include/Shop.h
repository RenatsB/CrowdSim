#ifndef CROWDLIB_SHOP_H_
#define CROWDLIB_SHOP_H_
#include <vector>
#include <memory>
#include "Product.h"
#include "Params.h"

class Shop: private Object
{
public:
    Shop(std::shared_ptr<Params> _prms):
        m_params(_prms)
    {}
    ~Shop()=default;
    std::vector<std::shared_ptr<Product>> getRemainingProducts() const;
    std::vector<std::shared_ptr<Product>> getfreeProducts() const;
    void setExits(std::vector<Vec2> _ex);
    std::vector<Vec2> getExits() const;
    uint getNumRemainingProducts() const;
    float getRemainingProductStress() const;
    float getDistanceStress() const;
    void update();
private:
    std::vector<std::shared_ptr<Product>> m_products;
    std::vector<std::shared_ptr<Product>> m_remp;
    std::vector<std::shared_ptr<Product>> m_freep;
    std::vector<Vec2> m_exits;
    std::shared_ptr<Params> m_params;
};
#endif //CROWDLIB_SHOP_H_
