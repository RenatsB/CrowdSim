#ifndef SHOP_H_
#define SHOP_H_
#include <vector>
#include <memory>
#include "Product.h"

class Shop: private Object
{
public:
    Shop()=default;
    ~Shop()=default;
    std::vector<std::shared_ptr<Product>> getRemainingProducts() const;
    std::vector<std::shared_ptr<Product>> getfreeProducts() const;
    void setExits(std::vector<Vec2> _ex);
    void setEntrances(std::vector<Vec2> _entr);
    std::vector<Vec2> getExits() const;
    std::vector<Vec2> getEntrances() const;
    uint getNumRemainingProducts() const;
    float getRemainingProductStress() const;
    float getDistanceStress() const;
    void update();
private:
    std::vector<std::shared_ptr<Product>> m_products;
    std::vector<std::shared_ptr<Product>> m_remp;
    std::vector<std::shared_ptr<Product>> m_freep;
    std::vector<Vec2> m_exits;
    std::vector<Vec2> m_entrs;
};
#endif //SHOP_H_
