//
// Created by bender on 10/11/2021.
//

#include "order.h"

void Order::AddItem(const Item &new_item) {
  items_.push_back(new_item);
}

void Order::PrintOrder() const {
  for (auto &item : items_) {
    std::cout << item.item_ << std::endl;
  }
}

float Order::TotalPrice() const {
  float total{0.0};
  for (auto &item : items_) {
    total += static_cast<float>(item.quantity_) * item.price_;
  }
  return total;
}

void Order::Pay(std::string payment_type, std::string security_code) {
  if (payment_type == "debit") {
    std::cout << "Processing debit payment type" << " ----" ;
    std::cout << "Verifying security code: {" << security_code << "}" << std::endl;
    status_ = "paid";
  } else if (payment_type == "credit") {
    std::cout << "Processing credit payment type" << "----";
    std::cout <<"Verifying security code: {" << security_code << "}"  << std::endl;
    status_ = "paid";
  } else {
    std::cout << "Error" << std::endl;
  }
}