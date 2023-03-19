#pragma once
#include "OrderBookEntry.h"
#include "CSVReader.h"
#include <string>
#include <vector>

class OrderBook
{
    public:
        /** construct, reading a csv data file */
        OrderBook(std::string filename);
        
        /** return vector of all know products in the dataset*/
        std::vector<std::string> getKnownProducts();

        /** return vector of Orders according to the sent filters*/
        std::vector<OrderBookEntry> getOrders(OrderBookType type, 
                                              std::string product, 
                                              std::string timestamp);

        /** returns the earliest time in the orderbook*/
        std::string getEarliestTime();

        /** returns the next time after the 
         * sent time in the orderbook  
         * If there is no next timestamp, wraps around to the start
         * */
        std::string getNextTime(std::string timestamp);

        /** returns the prev time after the 
         * sent time in the orderbook  
         * If there is no prev timestamp, use the first timestamp
         * */
        std::string getPrevTime(std::string timestamp);

        //returns the timestamp vector of all orders in the csv file
        std::vector<std::string> getAllTime();

        //insert the order into orders when an ask or bid is made
        void insertOrder(OrderBookEntry& order);

        //match the ask and bids to make transactions
        std::vector<OrderBookEntry> matchAsksToBids(std::string product, std::string timestamp);

        //calculate stats of asks and bids made
        static double getHighPrice(std::vector<OrderBookEntry>& orders);
        static double getLowPrice(std::vector<OrderBookEntry>& orders);
        static double getAvgPrice(std::vector<OrderBookEntry>& orders);
        static double getMedianPrice(std::vector<OrderBookEntry>& orders);
        static std::string getPrediction(std::vector<OrderBookEntry>& orders1, std::vector<OrderBookEntry>& orders2);

    private:
        std::vector<OrderBookEntry> orders;
};
