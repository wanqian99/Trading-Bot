#include "OrderBook.h"
#include "CSVReader.h"
#include <map>
#include <algorithm>
#include <iostream>
#include <iterator>



/** construct, reading a csv data file */
OrderBook::OrderBook(std::string filename)
{
    //read the csv file by calling the function and parsing in the file
    //save it as orders
    orders = CSVReader::readCSV(filename);
}



/** return vector of all know products in the dataset*/
std::vector<std::string> OrderBook::getKnownProducts()
{
    //initialise products as a vector of strings
    std::vector<std::string> products;

    //initialise prodMap as a map with string and booleans
    std::map<std::string,bool> prodMap;

    //loop through each orders and map the product to boolean true
    for (OrderBookEntry& e : orders)
    {
        prodMap[e.product] = true;
    }
    
    // now flatten the map to a vector of strings
    for (auto const& e : prodMap)
    {
        products.push_back(e.first);
    }

    return products;
}



/** return vector of Orders according to the sent filters*/
std::vector<OrderBookEntry> OrderBook::getOrders(OrderBookType type, 
                                        std::string product, 
                                        std::string timestamp)
{
    //initialise orders_sub as a vector of OrderBookEntry
    //to be used to save sub orders containing type, product and timestamp
    std::vector<OrderBookEntry> orders_sub;

    //for each e in orders
    for (OrderBookEntry& e : orders)
    {
        //if e's orderType, product, timestamp matches those that are parsed in,
        //push it into orders_sub vector
        if (e.orderType == type && 
            e.product == product && 
            e.timestamp == timestamp )
            {
                orders_sub.push_back(e);
            }
    }

    //return the vector
    return orders_sub;
}



//Returns the max price seen in the orders
double OrderBook::getHighPrice(std::vector<OrderBookEntry>& orders)
{
    double max = orders[0].price;
    for (OrderBookEntry& e : orders)
    {
        if (e.price > max) max = e.price;
    }
    return max;
}


////Returns the min price seen in the orders
double OrderBook::getLowPrice(std::vector<OrderBookEntry>& orders)
{
    double min = orders[0].price;
    for (OrderBookEntry& e : orders)
    {
        if (e.price < min) min = e.price;
    }
    return min;
}



////Returns the average price seen in the orders
double OrderBook::getAvgPrice(std::vector<OrderBookEntry>& orders)
{
    double totalPrice = 0;
    int numOrders = 0;

    //for each e in orders, 
    //add the price to the totalPrice double and increment the numOrders integer by 1
    //return the average by dividing totalPrice with numOrders
    for (OrderBookEntry& e : orders)
    {
        totalPrice += e.price;
        numOrders++;
    }
    return totalPrice/numOrders;
}



//Returns the median price seen in the orders
double OrderBook::getMedianPrice(std::vector<OrderBookEntry>& orders)
{
    //sorts the orders by asccending order
    std::sort(orders.begin(), orders.end(), OrderBookEntry::compareByPriceAsc);

    //checks if orders size is odd or even 
    //used to calculate median price in the later part
    std::string ordersState;
    if(orders.size() % 2 == 0) ordersState = "even";
    else ordersState = "odd";

    //initialise median as the first order's price first
    double median = orders[0].price;

    //do the median calculation differently when the ordersState is even or odd
    if(ordersState == "even")
    {
        median = (orders[(orders.size()-2)/2].price + orders[orders.size()/2].price)/2;
    }
    else
    {
        median = orders[(orders.size()-1)/2].price;
    }

    //return median which will be used to print out as market stats and prediction
    return median;
}



//predict the changes in the next timestamp, and returns if it is better to bid or ask
std::string OrderBook::getPrediction(std::vector<OrderBookEntry>& orders1, std::vector<OrderBookEntry>& orders2)
{
    //initialise predict as a string
    std::string predict;

    //if the avgPrice < medianPrice, this means that the prices is negatively skewed,
    //and there would most probably be frequent small gains and few extreme loses
    //returns predict as bid
    if((getAvgPrice(orders1) < getMedianPrice(orders1)) &&
        (getAvgPrice(orders2) < getMedianPrice(orders2)))
    {
        predict = "bid";
    }

    //if the avgPrice > medianPrice, this means that the prices is positively skewed,
    //and there would most probably be frequent small loses and few extreme gains
    //returns predict as ask
    else if((getAvgPrice(orders1) > getMedianPrice(orders1)) &&
            (getAvgPrice(orders2) > getMedianPrice(orders2)))
    {
        predict = "ask";
    }

    //else if only either condition is true, make predict null and do nothing
    else
    {
        predict = "null";
    }

    //return predict
    return predict;
}



//returns the earliest time in the orderbook
std::string OrderBook::getEarliestTime()
{
    return orders[0].timestamp;
}



//returns the next time after the sent time in the orderbook  
//If there is no next timestamp, wraps around to the start
std::string OrderBook::getNextTime(std::string timestamp)
{
    std::string next_timestamp = "";
    for (OrderBookEntry& e : orders)
    {
        if (e.timestamp > timestamp) 
        {
            next_timestamp = e.timestamp;
            break;
        }
    }
    if (next_timestamp == "")
    {
        next_timestamp = orders[0].timestamp;
    }
    return next_timestamp;
}



//returns the previous time after the sent time in the orderbook  
//If there is no prev timestamp, use the first timestamp
std::string OrderBook::getPrevTime(std::string timestamp)
{
    //calls getAllTime that returns the timestamps of the order
    //save its as times
    std::vector<std::string> times = getAllTime();

    for(int i = 0; i < times.size(); ++i) {
        //if timestamp match found and if the timestamp is not the first timestamp, return the previous timestamp
        if(timestamp == times[i] && timestamp != times[0]) {
            return times[i-1];
        }
    }

    //return first timestamp if there is no previous timestamp
    return times[0];
}



//returns the timestamp vector of all orders in the csv file
std::vector<std::string> OrderBook::getAllTime()
{
    //times vector of strings
    std::vector<std::string> times;

    //for each order in orders
    for(OrderBookEntry& order : orders)
    {
        //push each timestamp to times vector
        times.push_back(order.timestamp);
    }
    return times;
}



//insert the order into orders when an ask or bid is made
//attempted to optimize the code as std::sort takes a lot of time
void OrderBook::insertOrder(OrderBookEntry& order)
{
    //calls getAllTime that returns the timestamps of the order
    //save its as times
    std::vector<std::string> times = getAllTime();

    //use lower_bound for efficiency
    //to find the position that matched the order.timestamp parsed in
    auto pos = std::lower_bound(times.begin(), times.end(), order.timestamp);

    //get the index by using distance
    std::size_t index = std::distance(times.begin(), pos);

    //user the iterator and save iter as a vector of OrderBookEntry
    //add the beginning of orders (index 0) and pos
    std::vector<OrderBookEntry>::iterator iter = orders.begin() + index;

    //insert the order at position iter
    orders.insert(iter, order);    
}



//match the ask and bids to make transactions
std::vector<OrderBookEntry> OrderBook::matchAsksToBids(std::string product, std::string timestamp)
{
    // asks = orderbook.asks
    std::vector<OrderBookEntry> asks = getOrders(OrderBookType::ask, 
                                                 product, 
                                                 timestamp);
    // bids = orderbook.bids
    std::vector<OrderBookEntry> bids = getOrders(OrderBookType::bid, 
                                                 product, 
                                                 timestamp);

    // sales = []
    std::vector<OrderBookEntry> sales; 

    //check to ensure we have bids and asks to process
    if (asks.size() == 0 || bids.size() == 0)
    {
        std::cout << " OrderBook::matchAsksToBids no bids or asks" << std::endl;
        return sales;
    }


    // sort asks lowest first
    std::sort(asks.begin(), asks.end(), OrderBookEntry::compareByPriceAsc);
    // sort bids highest first
    std::sort(bids.begin(), bids.end(), OrderBookEntry::compareByPriceDesc);


    /********** PRINTS ASKS STATS **********/
    std::cout << "------------------------" << std::endl;
    std::cout << "   Max ask: " << getHighPrice(asks) << std::endl;
    std::cout << "   Avg ask: " << getAvgPrice(asks) << std::endl;
    std::cout << "   Min ask: " << getLowPrice(asks) << std::endl;
    std::cout << "  Median ask: " << getMedianPrice(asks) << std::endl;

    /********** PRINTS BIDS STATS **********/
    std::cout << "   Max bid: " << getHighPrice(bids) << std::endl;
    std::cout << "   Avg bid: " << getAvgPrice(bids) << std::endl;
    std::cout << "   Min bid: " << getLowPrice(bids) << std::endl;
    std::cout << "  Median bid: " << getMedianPrice(bids) << std::endl;
    std::cout << "------------------------" << std::endl;
    

    // for ask in asks
    for (OrderBookEntry& ask : asks)
    {
    //     for bid in bids:
        for (OrderBookEntry& bid : bids)
        {
    //         if bid.price >= ask.price # we have a match
            if (bid.price >= ask.price)
            {
    //             sale = new order()
    //             sale.price = ask.price
                OrderBookEntry sale{ask.price, 0, timestamp, product, OrderBookType::asksale};

                if (bid.username == "simuser")
                {
                    sale.username = "simuser";
                    sale.orderType = OrderBookType::bidsale;
                }
                if (ask.username == "simuser")
                {
                    sale.username = "simuser";
                    sale.orderType =  OrderBookType::asksale;
                }
            
    //             # now work out how much was sold and 
    //             # create new bids and asks covering 
    //             # anything that was not sold
    //             if bid.amount == ask.amount: # bid completely clears ask
                if (bid.amount == ask.amount)
                {
    //                 sale.amount = ask.amount
                    sale.amount = ask.amount;
    //                 sales.append(sale)
                    sales.push_back(sale);
    //                 bid.amount = 0 # make sure the bid is not processed again
                    bid.amount = 0;
    //                 # can do no more with this ask
    //                 # go onto the next ask
    //                 break
                    break;
                }
    //           if bid.amount > ask.amount:  # ask is completely gone slice the bid
                if (bid.amount > ask.amount)
                {
    //                 sale.amount = ask.amount
                    sale.amount = ask.amount;
    //                 sales.append(sale)
                    sales.push_back(sale);
    //                 # we adjust the bid in place
    //                 # so it can be used to process the next ask
    //                 bid.amount = bid.amount - ask.amount
                    bid.amount =  bid.amount - ask.amount;
    //                 # ask is completely gone, so go to next ask                
    //                 break
                    break;
                }


    //             if bid.amount < ask.amount # bid is completely gone, slice the ask
                if (bid.amount < ask.amount && 
                   bid.amount > 0)
                {
    //                 sale.amount = bid.amount
                    sale.amount = bid.amount;
    //                 sales.append(sale)
                    sales.push_back(sale);
    //                 # update the ask
    //                 # and allow further bids to process the remaining amount
    //                 ask.amount = ask.amount - bid.amount
                    ask.amount = ask.amount - bid.amount;
    //                 bid.amount = 0 # make sure the bid is not processed again
                    bid.amount = 0;
    //                 # some ask remains so go to the next bid
    //                 continue
                    continue;
                }
            }
        }
    }
    return sales;             
}