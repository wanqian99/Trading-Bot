#pragma once

#include <string>
#include <map>
#include "OrderBookEntry.h"
#include <iostream>

class Wallet 
{
    public:
        //constructor
        Wallet();
        /** insert currency to the wallet */
        void insertCurrency(std::string type, double amount);

        /** remove currency from the wallet */
        bool removeCurrency(std::string type, double amount);
        
        /** check if the wallet contains this much currency or more */
        bool containsCurrency(std::string type, double amount);

        /** checks if the wallet can cope with this ask or bid.*/
        bool canFulfillOrder(OrderBookEntry order);

        /** update the contents of the wallet
        assumes the order was made by the owner of the wallet */
        void processSale(OrderBookEntry& sale);

        /** generate a string representation of the wallet */
        std::string toString();

        /**this function calls toString that returns the currency and amount the wallet has */
        friend std::ostream& operator<<(std::ostream& os, Wallet& wallet);

        //returns vector of currency
        std::vector<std::string> walletCurr();

        //returns vector of the amount the currency has
        std::vector<std::string> walletAmt();

        //saves the timestamp and total wallet assets to walletAssets.txt
        void walletAssets(std::string timestamp);

        //saves all bids and asks made to allTransactions.txt
        void allTransactions(std::string type, std::string input, std::string timestamp);

        //saves all successful bids and asks to allSucessfulTransactions.txt
        void allSuccessfulTransactions(std::string type, std::string input, std::string timestamp);
    private:
        //let currencies be a map of string to double (eg BTC : 10.000)
        std::map<std::string,double> currencies;

        //set up the text files that will record the wallet changes and transactions made
        std::string assets = "walletAssets.txt";
        std::string allBidsAsks = "allTransactions.txt";
        std::string successfulBidsAsks = "allSuccessfulTransactions.txt";

        //saves the wallet in and out history to walletAssets.txt
        void walletChange(std::string incomingCurrency, double incomingAmount, std::string outgoingCurrency, double outgoingAmount);        
};



	

