#include "MerkelMain.h"
#include <iostream>
#include <vector>
#include "OrderBookEntry.h"
#include "CSVReader.h"


//constructor
MerkelBot::MerkelBot()
{

}


//starts the simulator
void MerkelBot::init()
{
    //initialise an integer variable named input
    int input;

    //call the getEarliestTime function as 
    currentTime = orderBook.getEarliestTime();

    //insert the currencies into the wallet
    wallet.insertCurrency("BTC", 10);
    wallet.insertCurrency("DOGE", 0);
    wallet.insertCurrency("ETH", 0);
    wallet.insertCurrency("USDT", 10000);

    std::cout << "" << std::endl << std::endl;
    std::cout << "*************************************" << std::endl;
    std::cout << "  Merkelrex Trading Mode: Automated  " << std::endl;
    std::cout << "*************************************" << std::endl << std::endl;

    //set automated to false
    automated = false;

    //while automated is true, call printMenu, getUserOption, processUserOption repeatedly
    while(automated == false)
    {
        printMenu();
        input = getUserOption();
        processUserOption(input);
    }
}



//prints the 4 menus to the terminal
void MerkelBot::printMenu()
{
    //menu options
    std::cout << "" << std::endl;
    std::cout << "===================================================" << std::endl << std::endl;
    std::cout << "|   " << "1) Print Help" << "   |   " << "2) Print Exchange Stats" << "   |" << std::endl << std::endl;
    std::cout << "|   " << "3) Print Wallet" << " |   " << "4) Start the trading bot" << "  |" << std::endl << std::endl;
    std::cout << "===================================================" << std::endl << std::endl << std::endl;

    //gets the current timestamp and prints it out onto terminal
    std::cout << "Current time is: " << currentTime << std::endl;
    std::cout << "" << std::endl;
}



////prints help statement
void MerkelBot::printHelp()
{
    std::cout << "Help - your aim is to make money. Analyse the market and make bids and offers. " << std::endl;
}



//prints all current market stats
void MerkelBot::printMarketStats()
{
    //calls getKnownProducts and loops through the vector of products returned from it
    for (std::string const& p : orderBook.getKnownProducts())
    {
        //print each product out in terminal
        std::cout << "" << std::endl;
        std::cout << "------------------" << std::endl;
        std::cout << "Product: " << p << std::endl;
        std::cout << "------------------" << std::endl;

        //initialise askEntries as a vector of OrderBookEntry,
        //calls getOrders and parse in OrderBookType::ask, product and current timestamp
        //do the same for bidEntries
        std::vector<OrderBookEntry> askEntries = orderBook.getOrders(OrderBookType::ask, 
                                                                p, currentTime);
        std::vector<OrderBookEntry> bidEntries = orderBook.getOrders(OrderBookType::bid, 
                                                                p, currentTime);

        //print out the number of asks and bids seen in the orderBook
        //calls getHighPrice, parse in askEntries and bidEntries to print the max price of ask and bids made
        //calls getAvgPrice, parse in askEntries and bidEntries to print the avg price of ask and bids made
        //calls getLowPrice, parse in askEntries and bidEntries to print the min price of ask and bids made
        //calls getMedianPrice, parse in askEntries and bidEntries to print the median price of ask and bids made

        //print out asks stats
        std::cout << "*** Asks seen: " << askEntries.size() << " ***" << std::endl;
        std::cout << "   Max ask: " << OrderBook::getHighPrice(askEntries) << std::endl;
        std::cout << "   Avg ask: " << OrderBook::getAvgPrice(askEntries) << std::endl;
        std::cout << "   Min ask: " << OrderBook::getLowPrice(askEntries) << std::endl;
        std::cout << " Median ask: " << OrderBook::getMedianPrice(askEntries) << std::endl;

        //print out bids stats
        std::cout << "*** Bids seen: " << bidEntries.size() << " ***" << std::endl;
        std::cout << "   Max bid: " << OrderBook::getHighPrice(bidEntries) << std::endl;
        std::cout << "   Avg bid: " << OrderBook::getAvgPrice(bidEntries) << std::endl;
        std::cout << "   Min bid: " << OrderBook::getLowPrice(bidEntries) << std::endl;
        std::cout << " Median bid: " << OrderBook::getMedianPrice(bidEntries) << std::endl;

        //calls getPrediction, parse in askEntries and bidEntries to print the predicted price of ask and bids made
        std::cout << " PREDICTION: " << OrderBook::getPrediction(askEntries, bidEntries) << std::endl;
    }
}



//function that allows user to enter ask
void MerkelBot::enterAsk(std::string currs, std::string price, std::string amt)
{
    //prompts user to input an ask
    std::cout << "" << std::endl;

    //initialise a string named input
    //get the currs, price and amount and save as input
    //prints it out on terminal
    std::string input;
    input = currs + "," + price + "," + amt;
    std::cout << "Bot entered ask: " << input << std::endl;

    //tokenise the input with comma as the seperator
    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');

    //if the number of tokens is not equal to 3, there is an error
    //print out error message
    if (tokens.size() != 3)
    {
        std::cout << "MerkelMain::enterAsk Bad input! " << input << std::endl;
    }
    else {
        try {
            //calls CSVReader function stringsToOBE and parse in the 3 tokens as parameters,
            //along with the currentTime and OrderBookType
            OrderBookEntry obe = CSVReader::stringsToOBE(
                tokens[1],
                tokens[2], 
                currentTime, 
                tokens[0], 
                OrderBookType::ask 
            );

            //set the obe's username to simuser
            obe.username = "simuser";

            //if wallet.canFulfilOrder returns true and when the obe amount is more than 0,
            //calls insertOrder and parse in the obe, to insert the order
            //record the transaction by calling allSuccessfulTransactions
            //else print out error message
            if (wallet.canFulfillOrder(obe)  && obe.amount != 0)
            {
                std::cout << "Wallet looks good. " << std::endl;
                orderBook.insertOrder(obe);
                wallet.allSuccessfulTransactions("Successful Ask", input, currentTime);
            }
            else {
                std::cout << "Wallet has insufficient funds . " << std::endl;
            }
        //print error message if try block has an error
        } catch (const std::exception& e)
        {
            std::cout << " MerkelMain::enterAsk Bad input " << std::endl;
        }   
    }
    //calls allTransactions to record all the ask that have occured in a text file
    wallet.allTransactions("Asks", input, currentTime);
}



//function that allows user to enter bid
void MerkelBot::enterBid(std::string currs, std::string price, std::string amt)
{
    //prompts user to input a bid
    std::cout << "" << std::endl;

    //initialise a string named input
    //get the currs, price and amount and save as input
    //prints it out on terminal
    std::string input;
    input = currs + "," + price + "," + amt;
    std::cout << "Bot entered bid: " << input << std::endl;

    //tokenise the input with comma as the seperator
    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');

    //if the number of tokens is not equal to 3, there is an error
    //print out error message
    if (tokens.size() != 3)
    {
        std::cout << "MerkelMain::enterBid Bad input! " << input << std::endl;
    }
    else {
        try {
            //call stringsToOBE and parse in the tokens as parameters
            OrderBookEntry obe = CSVReader::stringsToOBE(
                tokens[1],
                tokens[2], 
                currentTime, 
                tokens[0], 
                OrderBookType::bid 
            );

            //set the obe's username to simuser
            obe.username = "simuser";

            //if wallet.canFulfilOrder returns true and when the obe amount is more than 0,
            //calls insertOrder and parse in the obe, to insert the order
            //record the transaction by calling allSuccessfulTransactions
            //else print out error message
            if (wallet.canFulfillOrder(obe) && obe.amount > 0)
            {
                std::cout << "Wallet looks good. " << std::endl;
                orderBook.insertOrder(obe);
                wallet.allSuccessfulTransactions("Successful Bid", input, currentTime);
            }
            else {
                std::cout << "Wallet has insufficient funds . " << std::endl;
            }
        //print error message if try block has an error
        } catch (const std::exception& e)
        {
            std::cout << " MerkelMain::enterBid Bad input " << std::endl;
        }   
    }
    //calls allTransactions to record all the bids that have occured in a text file
    wallet.allTransactions("Bids", input, currentTime);
}



//prints current assets
void MerkelBot::printWallet()
{
    std::cout << "" << std::endl;
    std::cout << wallet.toString() << std::endl;
}



//goes to the next timestamp  
void MerkelBot::gotoNextTimeframe()
{
    std::cout << "*** Going to next time frame. ***" << std::endl;

    //calls getKnownProducts and loops through the vector of products returned from it
    for (std::string p : orderBook.getKnownProducts())
    {
        //prints to terminal the product that is matching
        std::cout << "" << std::endl;
        std::cout << "*** Matching " << p << " ***" << std::endl;

        //calls matchAsksToBids, parsing in the product and currentTime,
        //and saving it as sales which is a vector of OrderBookEntry
        std::vector<OrderBookEntry> sales =  orderBook.matchAsksToBids(p, currentTime);
        
        //print the total number of sales made
        std::cout << "Sales: " << sales.size() << std::endl;

        //for each sale, print the price and amount
        //update the wallet to process the sale
        for (OrderBookEntry& sale : sales)
        {
            std::cout << "Sale price: " << sale.price << " amount " << sale.amount << std::endl; 
            if (sale.username == "simuser")
            {
                // update the wallet
                wallet.processSale(sale);
            }
        }
        
    }
    //move on to the next timeframe by calling getNextTime
    currentTime = orderBook.getNextTime(currentTime);

    //record the change in wallet assets in a text file
    wallet.walletAssets(currentTime);
}



//gets and returns the user option
int MerkelBot::getUserOption()
{
    //initialise userOption as an integer
    int userOption = 0;

    //initialise line as a string
    //prompts the user to type in an option
    //save the cin from the terminal to line
    std::string line;
    std::cout << "Type in 1-4" << std::endl;
    std::getline(std::cin, line);

    //try: convert the cin from string to integer and save it as userOption
    //else catch error
    try{
        userOption = std::stoi(line);
    }catch(const std::exception& e)
    {
        // 
    }

    //prints out what user typed and return userOption
    std::cout << "You chose: " << userOption << std::endl;
    return userOption;
}



//process and calls functions based on the user option parsed in
bool MerkelBot::processUserOption(int userOption)
{
    if (userOption == 0) // bad input
    {
        std::cout << "Invalid choice. Choose 1-3" << std::endl;
        automated = false;
    }
    if (userOption == 1) 
    {
        printHelp();
        automated = false;
    }
    if (userOption == 2) 
    {
        printMarketStats();
        automated = false;
    }
    if (userOption == 3) 
    {
        printWallet();
        automated = false;
    }
    if(userOption == 4)
    {
        startBot();
        automated = true;
    }
    //return automated
    return automated; 
}



//predict the future currency changes
void MerkelBot::getPredictionCurrency()
{
    //initialise the variables as vectors
    std::vector<std::string> bidOrask;
    std::vector<double> maxAsk;
    std::vector<double> avgAsk;
    std::vector<double> minAsk;
    std::vector<double> maxBid;
    std::vector<double> avgBid;
    std::vector<double> minBid;

    //loop through getKnownProducts
    for (std::string const& p : orderBook.getKnownProducts())
    {
        //initialise askEntries as a vector of OrderBookEntry,
        //calls getOrders and parse in OrderBookType::ask, product and current timestamp
        //do the same for bidEntries
        std::vector<OrderBookEntry> askEntries = orderBook.getOrders(OrderBookType::ask, 
                                                                p, currentTime);
        std::vector<OrderBookEntry> bidEntries = orderBook.getOrders(OrderBookType::bid, 
                                                                p, currentTime);

        //push the stats of each product to their respective vector
        //this will be used for computing the price and amount to bid and ask in the latter parts
        maxAsk.push_back(OrderBook::getHighPrice(askEntries));
        avgAsk.push_back(OrderBook::getAvgPrice(askEntries));
        minAsk.push_back(OrderBook::getLowPrice(askEntries));

        maxBid.push_back(OrderBook::getHighPrice(bidEntries));
        avgBid.push_back(OrderBook::getAvgPrice(bidEntries));
        minBid.push_back(OrderBook::getLowPrice(bidEntries));

        //call getPrediction which returns whether the bot should ask, bid or do nothing for each product
        bidOrask.push_back(OrderBook::getPrediction(askEntries, bidEntries));        
    }

    //for each string in the vector
    for(int i = 0; i < bidOrask.size(); ++i)
    {
        //if the prediction is to ask
        if(bidOrask[i] == "ask")
        {
            //initialise amt, price, askAmt as string
            std::string amt;
            std::string price;
            std::string askAmt;

            //tokenise the ith order and save it as curr
            //this splits the curr into 2 parts, eg. BTC and USDT
            std::vector<std::string> curr = CSVReader::tokenise(orderBook.getKnownProducts()[i], '/');

            //calculate the price that the bot should ask
            //to gain maximum profit, make the price be random from the avgbidprice to the maxbidprice
            double randomPrice = (double)rand() / RAND_MAX;
            price = std::to_string(avgBid[i]+ randomPrice*(maxBid[i]-avgBid[i]));

            //loop through walletCurr function that returns the currency types in the wallet
            //this returns the amount to ask
            for(int j = 0; j < wallet.walletCurr().size(); ++j)
            {
                //if the currency type in the wallet is equal to the curr[0] (first part of curr)
                //make amount be the walletAmt at jth index
                //returns the remaining amount the wallet have of that currency
                if(wallet.walletCurr()[j] == curr[0])
                {
                    amt = wallet.walletAmt()[j];
                }
            }

            //calculate the amount the bot should ask
            //risks and sell only 20% of remaining amount the currency have
            askAmt = std::to_string((std::stod(amt) * 20/100)/std::stod(price));

            //calls enterAsk to ask with the curr, price and askAmt parsed in
            enterAsk(orderBook.getKnownProducts()[i], price, askAmt);
        }

        //if the prediction is to bid
        if(bidOrask[i] == "bid")
        {
            //initialise amt, price, askAmt as string
            std::string amt;
            std::string price;
            std::string bidAmt;

            //tokenise the ith order and save it as curr
            //this splits the curr into 2 parts, eg. BTC and USDT
            std::vector<std::string> curr = CSVReader::tokenise(orderBook.getKnownProducts()[i], '/');

            //calculate the price that the bot should bid
            //to gain maximum profit, make the price be random from the minaskprice to the avgaskprice
            double randomPrice = (double)rand() / RAND_MAX;
            price = std::to_string(minAsk[i]+ randomPrice*(avgAsk[i]-minAsk[i]));
            
            //loop through walletCurr function that returns the currency types in the wallet
            //this returns the amount to ask
            for(int j = 0; j < wallet.walletCurr().size(); ++j)
            {
                //if the currency type in the wallet is equal to the curr[1] (second part of curr)
                //make amount be the walletAmt at jth index
                //returns the remaining amount the wallet have of that currency
                if(wallet.walletCurr()[j] == curr[1])
                {
                    amt = wallet.walletAmt()[j];
                }
            }

            //calculate the amount the bot should bid
            //risks and buy with only 20% of remaining amount of the currency the wallet have
            bidAmt = std::to_string((std::stod(amt) * 20/100)/std::stod(price));
            
            //calls enterBid to bid with the curr, price and bidAmt parsed in
            enterBid(orderBook.getKnownProducts()[i], price, bidAmt);
        }
    }
}


//start the trading bot
void MerkelBot::startBot()
{
    std::cout << "" << std::endl;
    std::cout << "--------------------------------------" << std::endl;
    std::cout << "      Starting trading bot......      " << std::endl;
    std::cout << "--------------------------------------" << std::endl;

    //while true, call getPredictionCurrency, gotoNextTimefram, printWallet repeatedly
    while(true)
    {
        getPredictionCurrency();
        gotoNextTimeframe();
        printWallet();
    }
}