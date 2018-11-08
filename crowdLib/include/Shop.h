#ifndef SHOP_H_
#define SHOP_H_
#include <vector>
#include <memory>
#include "Product.h"
#include "Wall.h"
class Shop: private Object
{
public:
    Shop()=default;
    ~Shop()=default;
    uint getRemainingProducts() const;
    std::vector<std::shared_ptr<Wall>> getWalls() const;
    std::vector<std::shared_ptr<Object>> getExits() const;
private:
    std::vector<std::shared_ptr<Product>> m_products;
    std::vector<std::shared_ptr<Wall>> m_walls;
    std::vector<std::shared_ptr<Object>> m_exits;
};
#endif //SHOP_H_
