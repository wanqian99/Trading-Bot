#pragma once

#include <vector>
#include "OrderBookEntry.h"
#include "OrderBook.h"
#include "Wallet.h"
#include "tradingBot.h"


class MerkelMain
{
    public:
        //constructor
        MerkelMain();
        /** Call this to start the sim */
        void init();
    private:
        //prints help statement
        void printHelp();

        //prints all current market stats
        void printMarketStats();

        //prints all available products
        void printProducts();

        //prints min of product
        void printMin();

        //prints median of product
        void printMax();

        //prints median of product
        void printMedian();

        //prints average of product
        void printAvg();

        //prints average of product over timestamps
        void printAvgTS();

        //prints prediction
        void printPredict();

        //function that allows user to enter ask
        void enterAsk();

        //function that allows user to enter bid
        void enterBid();

        //prints current assets
        void printWallet();

        //goes to the next timestamp
        void gotoNextTimeframe();

        //gets and returns the user option
        std::string getUserOption();

        //process and calls functions based on the user option parsed in
        void processUserOption(std::string userOption);

        //current timestamp string
        std::string currentTime;

        //OrderBook orderBook{"20200317.csv"};
        OrderBook orderBook{"20200601.csv"};

        Wallet wallet;
};