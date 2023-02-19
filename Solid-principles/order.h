#ifndef SOLID_PRINCIPLES__ORDER_H_
#define SOLID_PRINCIPLES__ORDER_H_
#include "item.h"
#include <vector>
#include <string>
#include <iostream>

class Order
{
public:
  void AddItem(const Item &new_item);
  float TotalPrice() const;
  void Pay(std::string payment_type, std::string security_code);
  void PrintOrder() const;

private:
  std::vector<Item> items_;
  std::string status_{"open"};
};

#endif // SOLID_PRINCIPLES__ORDER_H_