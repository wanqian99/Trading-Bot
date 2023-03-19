#include "Wallet.h"
#include <iostream>
#include "CSVReader.h"
#include <fstream>

//constructor
Wallet::Wallet()
{


}



//insert currency to the walle
void Wallet::insertCurrency(std::string type, double amount)
{
    //initialise balance as double
    double balance;

    //throw an exception if amount is lesser than 0
    //if the type of currency is 0,
    //make the balance be 0 as there is currently no amount of currency of that type in the wallet
    //else make the balance be the map of currencies and the type
    if (amount < 0)
    {
        throw std::exception();
    }
    if (currencies.count(type) == 0) // not there yet
    {
        balance = 0;
    }
    else { // is there 
        balance = currencies[type];
    }

    //add amount to balance
    balance += amount; 

    //map currencies type to balance
    currencies[type] = balance; 
}



//remove currency from the wallet
bool Wallet::removeCurrency(std::string type, double amount)
{
    //return false if the amount is less than 0 and if the currency of that type is 0
    //else, calls containsCurrency, parsing in the currency type and amount
    //if it returns true, map currencies type to minus the amount
    //else returns false if there isnt enough amount of that currency in the wallet
    if (amount < 0)
    {
        return false; 
    }
    if (currencies.count(type) == 0) // not there yet
    {
        //std::cout << "No currency for " << type << std::endl;
        return false;
    }
    else { // is there - do  we have enough
        if (containsCurrency(type, amount))// we have enough
        {
            //std::cout << "Removing " << type << ": " << amount << std::endl;
            currencies[type] -= amount;
            return true;
        } 
        else // they have it but not enough.
            return false; 
    }
}



//check if the wallet contains this much currency or more
bool Wallet::containsCurrency(std::string type, double amount)
{
    //return true if there is equal or more than enough amount of that currency
    //otherwise return false
    if (currencies.count(type) == 0) // not there yet
        return false;
    else 
        return currencies[type] >= amount;
    
}



//generate a string representation of the wallet
std::string Wallet::toString()
{
    //initialise s as a string
    std::string s;

    //for each pair in currencies
    for (std::pair<std::string,double> pair : currencies)
    {
        //save first part of pair as currency string
        std::string currency = pair.first;

        //save second part of pair as amount of type double
        double amount = pair.second;

        //make s be the addition of the currency and amount the currency has
        s += currency + " : " + std::to_string(amount) + "\n";
    }
    //return s
    return s;
}



//returns vector of currency
std::vector<std::string> Wallet::walletCurr()
{
    //let totalCurr be a vector of strings that would store all the available currency types
    std::vector<std::string> totalCurr;

    //get the first part of the pair and push it into totalCurr
    //return totalCurr
    for (std::pair<std::string,double> pair : currencies)
    {
        std::string currency = pair.first;
        totalCurr.push_back(currency);
    }
    return totalCurr;
}



////returns vector of the amount the currency has
std::vector<std::string> Wallet::walletAmt()
{
    //let totalCurr be a vector of strings that would store all the available amount of the currency
    std::vector<std::string> totalAmt;

    //get the second part of the pair and push it into totalAmt
    //return totalAmt
    for (std::pair<std::string,double> pair : currencies)
    {
        double amount = pair.second;
        totalAmt.push_back(std::to_string(amount));
    }
    return totalAmt;
}



//checks if the wallet can cope with this ask or bid
bool Wallet::canFulfillOrder(OrderBookEntry order)
{
    //let currs be a vector of string that contains tokenised order product
    std::vector<std::string> currs = CSVReader::tokenise(order.product, '/');

    // if OrderBookType is ask
    if (order.orderType == OrderBookType::ask)
    {
        //let order.amount be saved as amount (type double)
        double amount = order.amount;

        //let currency be the first part of currs that is tokenised
        std::string currency = currs[0];

        //print out the currency that can be fulfilled, along with the amount
        std::cout << "Wallet::canFulfillOrder " << currency << " : " << amount << std::endl;

        //returns boolean value after calling containsCurrency
        return containsCurrency(currency, amount);
    }
    // if OrderBookType is bid
    if (order.orderType == OrderBookType::bid)
    {
        //let order.amount multiplied by the order.price be saved as amount (type double)
        double amount = order.amount * order.price;

        //let currency be the second part of currs that is tokenised
        std::string currency = currs[1];

        //print out the currency that can be fulfilled, along with the amount
        std::cout << "Wallet::canFulfillOrder " << currency << " : " << amount << std::endl;

        //returns boolean value after calling containsCurrency
        return containsCurrency(currency, amount);
    }

    return false; 
}
      


// update the contents of the wallet, assumes the order was made by the owner of the wallet
//does the calculations of the sale, processes it and records the wallet changes over time
void Wallet::processSale(OrderBookEntry& sale)
{
    //let currs be a vector of string that contains tokenised sale product
    std::vector<std::string> currs = CSVReader::tokenise(sale.product, '/');

    // if OrderBookType is asksale
    if (sale.orderType == OrderBookType::asksale)
    {
        //let sale.amount be saved as outgoingAmount (type double)
        double outgoingAmount = sale.amount;

        //let outgoingCurrency be the first part of currs that is tokenised
        std::string outgoingCurrency = currs[0];

        //let sale.amount multiplied by sale.price be saved as incomingAmount (type double)
        double incomingAmount = sale.amount * sale.price;

        //let Incomingcurrency be the second part of currs that is tokenised
        std::string incomingCurrency = currs[1];

        //map incoming Currency to the addition incoming Amount
        //map outgoingCurrency to the addition of outgoingAmount
        currencies[incomingCurrency] += incomingAmount;
        currencies[outgoingCurrency] -= outgoingAmount;

        //call walletChange and parse in the incoming and outgoing currency and amount to record the changes in the wallet
        walletChange(incomingCurrency, incomingAmount, outgoingCurrency, outgoingAmount);
    }

    // if OrderBookType is bidsale
    if (sale.orderType == OrderBookType::bidsale)
    {
        //let sale.amount be saved as incomingAmount (type double)
        double incomingAmount = sale.amount;

        //let incomingcurrency be the second part of currs that is tokenised
        std::string incomingCurrency = currs[0];

        //let sale.amount multiplied by sale.price be saved as outgoingAmount (type double)
        double outgoingAmount = sale.amount * sale.price;

        //let outgoingCurrency be the first part of currs that is tokenised
        std::string outgoingCurrency = currs[1];

        //map incoming Currency to the addition incoming Amount
        //map outgoingCurrency to the addition of outgoingAmount
        currencies[incomingCurrency] += incomingAmount;
        currencies[outgoingCurrency] -= outgoingAmount;

        //call walletChange and parse in the incoming and outgoing currency and amount to record the changes in the wallet
        walletChange(incomingCurrency, incomingAmount, outgoingCurrency, outgoingAmount);
    }
}



//this function calls toString that returns the currency and amount the wallet has
std::ostream& operator<<(std::ostream& os,  Wallet& wallet)
{
    os << wallet.toString();
    return os;
}



//saves the wallet in and out history to walletAssets.txt
void Wallet::walletChange(std::string incomingCurrency, double incomingAmount, 
                          std::string outgoingCurrency, double outgoingAmount)
{
    //open the file and name it as assets, which is walletAssets.txt
    //make it such that every new line is saved at the end of the file
    std::ofstream file;
    file.open(assets, std::ios::app);

    //when the file is open, save the wallet in and out currency and amount to the text file
    //else print an error message
    if (file.is_open())
    {
        file << "Wallet in: " << incomingAmount << " " << incomingCurrency << "  |  " 
             << "Wallet out: " << outgoingAmount << " " << outgoingCurrency << std::endl;
    }
    else
    {
        std::cout << "Transaction history file not opened!" << std::endl;
    }

    //close the file after its done recording the changes
    file.close();
}



//saves the timestamp and total wallet assets to walletAssets.txt
void Wallet::walletAssets(std::string timestamp)
{
    //open the file and name it as assets, which is walletAssets.txt
    //make it such that every new line is saved at the end of the file
    std::ofstream file;
    file.open(assets, std::ios::app);

    if (file.is_open())
    {
        //when the file is open, save current timestamp and the wallet assets to the text file
        //else print an error message
        file << "Current Timestamp ------- " << timestamp << std::endl
             << "Wallet----------------------------------------------" << std::endl
             << toString() << std::endl << std::endl;
    }
    else
    {
        std::cout << "Transaction history file not opened!" << std::endl;
    }

    //close the file after its done recording the changes
    file.close();
}



//saves all bids and asks made to allTransactions.txt
void Wallet::allTransactions(std::string type, std::string input, std::string timestamp)
{
    //open the file and name it as allBidsAsks, which is allTransactions.txt
    //make it such that every new line is saved at the end of the file
    std::ofstream file;
    file.open(allBidsAsks, std::ios::app);

    //when the file is open, save current timestamp and the ask/bid made, along with the price and amount
    //else print an error message
    if(file.is_open())
    {
        file << "Current Timestamp ------- " << timestamp << std::endl
             << type << "              ------- " << input << std::endl << std::endl;
    }
    else
    {
        std::cout << "allTransactions file not opened!" << std::endl;
    }

    //close the file after its done recording the changes
    file.close();
}



//saves all successful bids and asks to allSucessfulTransactions.txt
void Wallet::allSuccessfulTransactions(std::string type, std::string input, std::string timestamp)
{
    //open the file and name it as successfulBidsAsks, which is allSuccessfulTransactions.txt
    //make it such that every new line is saved at the end of the file
    std::ofstream file;
    file.open(successfulBidsAsks, std::ios::app);

    //when the file is open, save current timestamp and the ask/bid made, along with the price and amount
    //else print an error message
    if(file.is_open())
    {
        file << "Current Timestamp ------- " << timestamp << std::endl
             << type << "    ------- " << input << std::endl << std::endl;
    }
    else
    {
        std::cout << "allSuccessfulTransactions file not opened!" << std::endl;
    }

    //close the file after its done recording the changes
    file.close();
}