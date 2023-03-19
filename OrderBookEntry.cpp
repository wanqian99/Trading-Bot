#include "OrderBookEntry.h"


//constructor function with 6 variables parsed in
OrderBookEntry::OrderBookEntry( double _price, 
                                double _amount, 
                                std::string _timestamp, 
                                std::string _product, 
                                OrderBookType _orderType, 
                                std::string _username)
: price(_price), 
  amount(_amount), 
  timestamp(_timestamp),
  product(_product), 
  orderType(_orderType), 
  username(_username)
{
  
    
}



//static function that converts the string to the OrderBookType
OrderBookType OrderBookEntry::stringToOrderBookType(std::string s)
{
  if (s == "ask")
  {
    return OrderBookType::ask;
  }
  if (s == "bid")
  {
    return OrderBookType::bid;
  }
  return OrderBookType::unknown;
}
