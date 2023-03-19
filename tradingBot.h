#pragma once

#include <vector>
#include "OrderBookEntry.h"
#include "OrderBook.h"
#include "Wallet.h"


class MerkelBot
{
    public:
    //constructor
        MerkelBot();
        /** Call this to start the sim */
        void init();
    private: 
        //prints the 4 menus to the terminal
        void printMenu();

        //prints help statement
        void printHelp();

        //prints all current market stats
        void printMarketStats();

        //function that allows user to enter ask
        void enterAsk(std::string currs, std::string price, std::string amt);

        //function that allows user to enter bid
        void enterBid(std::string currs, std::string price, std::string amt);

        //prints current assets
        void printWallet();

        //goes to the next timestamp
        void gotoNextTimeframe();

        //gets and returns the user option
        int getUserOption();

        //process and calls functions based on the user option parsed in
        bool processUserOption(int userOption);

        //start the trading bot
        void startBot();

        //predict the future currency changes
        void getPredictionCurrency();

        //current timestamp string
        std::string currentTime;
        bool automated;

        //OrderBook orderBook{"20200317.csv"};
        OrderBook orderBook{"20200601.csv"};

        Wallet wallet;

};