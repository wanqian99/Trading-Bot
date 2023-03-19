#pragma once

#include <string>


//make an enum class called OrderBookType that has bid, ask, unknown, asksale, bidsale as the types
enum class OrderBookType{bid, ask, unknown, asksale, bidsale};

class OrderBookEntry
{
    public:
        //constructor with 6 variables parsed in
        OrderBookEntry( double _price, 
                        double _amount, 
                        std::string _timestamp, 
                        std::string _product, 
                        OrderBookType _orderType, 
                        std::string username = "dataset");

        //static function that converts the string to OrderBookType
        static OrderBookType stringToOrderBookType(std::string s);

        //compare the timestamps of the parameters parsed in
        static bool compareByTimestamp(OrderBookEntry& e1, OrderBookEntry& e2)
        {
            return e1.timestamp < e2.timestamp;
        }  

        //sort the price by ascending order
        static bool compareByPriceAsc(OrderBookEntry& e1, OrderBookEntry& e2)
        {
            return e1.price < e2.price;
        }

        //sort the price by descending order
         static bool compareByPriceDesc(OrderBookEntry& e1, OrderBookEntry& e2)
        {
            return e1.price > e2.price;
        }

        //initialise variables and types
        double price;
        double amount;
        std::string timestamp;
        std::string product;
        OrderBookType orderType;
        std::string username;
};
