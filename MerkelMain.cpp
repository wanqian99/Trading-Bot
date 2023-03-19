#include "MerkelMain.h"
#include <iostream>
#include <vector>
#include "OrderBookEntry.h"
#include "CSVReader.h"
#include <numeric>


//constructor
MerkelMain::MerkelMain()
{

}



//starts the simulator
void MerkelMain::init()
{
    //initialise an integer variable named input
    std::string input;

    //call the getEarliestTime function as 
    currentTime = orderBook.getEarliestTime();

    //insert the currencies into the wallet
    wallet.insertCurrency("BTC", 10);
    wallet.insertCurrency("DOGE", 0);
    wallet.insertCurrency("ETH", 0);
    wallet.insertCurrency("USDT", 10000);

    std::cout << "" << std::endl << std::endl;
    std::cout << "**********************************" << std::endl;
    std::cout << "  Merkelrex Trading Mode: Manual  " << std::endl;
    std::cout << "**********************************" << std::endl << std::endl;
    std::cout << "Your aim is to make money. Analyse the market and make bids and offers. " << std::endl;
    
    //while true, call getUserOption, processUserOption repeatedly
    while(true)
    {
        input = getUserOption();
        processUserOption(input);
    }
}



//prints help statement
void MerkelMain::printHelp()
{
    std::cout << "The available commands are: help, help <cmd>, allstats, prod, min, max, median, avg, avgts, predict, ask, bid, wallet, time, step" << std::endl;
}



//prints all current market stats
void MerkelMain::printMarketStats()
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

        //calls getPrediction, parse in askEntries and bidEntries to print the prediction of whether the user should ask, bid, or do nothing(null) for this currency
        std::cout << " PREDICTION: " << OrderBook::getPrediction(askEntries, bidEntries) << std::endl;
    }
}



//prints all available products
void MerkelMain::printProducts() {
    //for each product in the order book, print them out
    for (std::string const& p : orderBook.getKnownProducts())
    {
        //print each product out in terminal
        std::cout << p << std::endl;
    }
}



//prints min of product
void MerkelMain::printMin() {
    //prompts user to input
    std::cout << "" << std::endl;
    std::cout << "Enter min price to print out: product,type  eg ETH/BTC,ask" << std::endl;

    //initialise a string named input
    //get the cin with getline function, and save it as input
    std::string input;
    std::getline(std::cin, input);

    //tokenise the input with comma as the seperator
    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');

    //if the number of tokens is not equal to 2, there is an error
    //print out error message
    if (tokens.size() != 2) {
        std::cout << "MerkelMain::printMin Bad input! " << input << std::endl;
    }
    else {
        //if tokens[0], which is the product, is equal to any of the available product, continue
        if(tokens[0] == "BTC/USDT" || tokens[0] == "DOGE/BTC" || tokens[0] == "DOGE/USDT" || tokens[0] == "ETH/BTC" || tokens[0] == "ETH/USDT") {
            if(tokens[1] == "ask") {
                //initialise askEntries as a vector of OrderBookEntry,
                //calls getOrders and parse in OrderBookType::ask, product and current timestamp
                std::vector<OrderBookEntry> askEntries = orderBook.getOrders(OrderBookType::ask, tokens[0], currentTime);
                //calls getLowPrice, parse in askEntries to print the min price of ask made
                std::cout << "Min ask: " << OrderBook::getLowPrice(askEntries) << std::endl;
            }
            else if(tokens[1] == "bid") {
                //initialise bidEntries as a vector of OrderBookEntry,
                //calls getOrders and parse in OrderBookType::ask, product and current timestamp
                std::vector<OrderBookEntry> bidEntries = orderBook.getOrders(OrderBookType::bid, tokens[0], currentTime);
                //calls getLowPrice, parse in bidEntries to print the min price of bid made
                std::cout << "Min bid: " << OrderBook::getLowPrice(bidEntries) << std::endl;
            }
            //if tokens[1], which is the order type, is not ask or bid, return error message
            else {
                std::cout << "MerkelMain::printMin Bad input! " << input << std::endl;
            }
        }
        //if tokens[0], which is the product, doesnt equal to any of the available product, return error message
        else {
            std::cout << "MerkelMain::printMin Bad input! " << input << std::endl;
        }
    }
}



//prints max of product
void MerkelMain::printMax() {
    //prompts user to input
    std::cout << "" << std::endl;
    std::cout << "Enter max price to print out: product,type  eg ETH/BTC,ask" << std::endl;

    //initialise a string named input
    //get the cin with getline function, and save it as input
    std::string input;
    std::getline(std::cin, input);

    //tokenise the input with comma as the seperator
    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');

    //if the number of tokens is not equal to 2, there is an error
    //print out error message
    if (tokens.size() != 2) {
        std::cout << "MerkelMain::printMin Bad input! " << input << std::endl;
    }
    else {
        //if tokens[0], which is the product, is equal to any of the available product, continue
        if(tokens[0] == "BTC/USDT" || tokens[0] == "DOGE/BTC" || tokens[0] == "DOGE/USDT" || tokens[0] == "ETH/BTC" || tokens[0] == "ETH/USDT") {
            if(tokens[1] == "ask") {
                //initialise askEntries as a vector of OrderBookEntry,
                //calls getOrders and parse in OrderBookType::ask, product and current timestamp
                std::vector<OrderBookEntry> askEntries = orderBook.getOrders(OrderBookType::ask, tokens[0], currentTime);
                //calls getHighPrice, parse in askEntries to print the max price of ask made
                std::cout << "Max ask: " << OrderBook::getHighPrice(askEntries) << std::endl;
            }
            else if(tokens[1] == "bid") {
                //initialise bidEntries as a vector of OrderBookEntry,
                //calls getOrders and parse in OrderBookType::ask, product and current timestamp
                std::vector<OrderBookEntry> bidEntries = orderBook.getOrders(OrderBookType::bid, tokens[0], currentTime);
                //calls getHighPrice, parse in bidEntries to print the max price of bid made
                std::cout << "Max bid: " << OrderBook::getHighPrice(bidEntries) << std::endl;
            }
            //if tokens[1], which is the order type, is not ask or bid, return error message
            else {
                std::cout << "MerkelMain::printMax Bad input! " << input << std::endl;
            }
        }
        //if tokens[0], which is the product, doesnt equal to any of the available product, return error message
        else {
            std::cout << "MerkelMain::printMax Bad input! " << input << std::endl;
        }
    }
}



//prints median of product
void MerkelMain::printMedian() {
    //prompts user to input
    std::cout << "" << std::endl;
    std::cout << "Enter median price to print out: product,type  eg ETH/BTC,ask" << std::endl;

    //initialise a string named input
    //get the cin with getline function, and save it as input
    std::string input;
    std::getline(std::cin, input);

    //tokenise the input with comma as the seperator
    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');

    //if the number of tokens is not equal to 2, there is an error
    //print out error message
    if (tokens.size() != 2) {
        std::cout << "MerkelMain::printMedian Bad input! " << input << std::endl;
    }
    else {
        //if tokens[0], which is the product, is equal to any of the available product, continue
        if(tokens[0] == "BTC/USDT" || tokens[0] == "DOGE/BTC" || tokens[0] == "DOGE/USDT" || tokens[0] == "ETH/BTC" || tokens[0] == "ETH/USDT") {
            if(tokens[1] == "ask") {
                //initialise askEntries as a vector of OrderBookEntry,
                //calls getOrders and parse in OrderBookType::ask, product and current timestamp
                std::vector<OrderBookEntry> askEntries = orderBook.getOrders(OrderBookType::ask, tokens[0], currentTime);
                //calls getMedianPrice, parse in askEntries to print the median price of ask made
                std::cout << "Median ask: " << OrderBook::getMedianPrice(askEntries) << std::endl;
            }
            else if(tokens[1] == "bid") {
                //initialise bidEntries as a vector of OrderBookEntry,
                //calls getOrders and parse in OrderBookType::ask, product and current timestamp
                std::vector<OrderBookEntry> bidEntries = orderBook.getOrders(OrderBookType::bid, tokens[0], currentTime);
                //calls getMedianPrice, parse in bidEntries to print the median price of bid made
                std::cout << "Median bid: " << OrderBook::getMedianPrice(bidEntries) << std::endl;
            }
            //if tokens[1], which is the order type, is not ask or bid, return error message
            else {
                std::cout << "MerkelMain::printMedian Bad input! " << input << std::endl;
            }
        }
        //if tokens[0], which is the product, doesnt equal to any of the available product, return error message
        else {
            std::cout << "MerkelMain::printMedian Bad input! " << input << std::endl;
        }
    }
}



//prints average of product
void MerkelMain::printAvg() {
    //prompts user to input
    std::cout << "" << std::endl;
    std::cout << "Enter average price to print out: product,type  eg ETH/BTC,ask" << std::endl;

    //initialise a string named input
    //get the cin with getline function, and save it as input
    std::string input;
    std::getline(std::cin, input);

    //tokenise the input with comma as the seperator
    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');

    //if the number of tokens is not equal to 2, there is an error
    //print out error message
    if (tokens.size() != 2) {
        std::cout << "MerkelMain::printAvg Bad input! " << input << std::endl;
    }
    else {
        //if tokens[0], which is the product, is equal to any of the available product, continue
        if(tokens[0] == "BTC/USDT" || tokens[0] == "DOGE/BTC" || tokens[0] == "DOGE/USDT" || tokens[0] == "ETH/BTC" || tokens[0] == "ETH/USDT") {
            if(tokens[1] == "ask") {
                //initialise askEntries as a vector of OrderBookEntry,
                //calls getOrders and parse in OrderBookType::ask, product and current timestamp
                std::vector<OrderBookEntry> askEntries = orderBook.getOrders(OrderBookType::ask, tokens[0], currentTime);
                //calls getAvgPrice, parse in askEntries to print the average price of ask made
                std::cout << "Median ask: " << OrderBook::getAvgPrice(askEntries) << std::endl;
            }
            else if(tokens[1] == "bid") {
                //initialise bidEntries as a vector of OrderBookEntry,
                //calls getOrders and parse in OrderBookType::ask, product and current timestamp
                std::vector<OrderBookEntry> bidEntries = orderBook.getOrders(OrderBookType::bid, tokens[0], currentTime);
                //calls getAvgPrice, parse in bidEntries to print the average price of bid made
                std::cout << "Median bid: " << OrderBook::getAvgPrice(bidEntries) << std::endl;
            }
            //if tokens[1], which is the order type, is not ask or bid, return error message
            else {
                std::cout << "MerkelMain::printAvg Bad input! " << input << std::endl;
            }
        }
        //if tokens[0], which is the product, doesnt equal to any of the available product, return error message
        else {
            std::cout << "MerkelMain::printAvg Bad input! " << input << std::endl;
        }
    }
}



//prints average of product over timestamps
void MerkelMain::printAvgTS() {
    //prompts user to input an ask
    std::cout << "Enter average price over timestamps to print out: product,type,timestamps  eg ETH/BTC,ask,10" << std::endl;

    //initialise a string named input
    //get the cin with getline function, and save it as input
    std::string input;
    std::getline(std::cin, input);

    //tokenise the input with comma as the seperator
    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');

    //if the number of tokens is not equal to 2, there is an error
    //print out error message
    if (tokens.size() != 3) {
        std::cout << "MerkelMain::printAvgTS Bad input! " << input << std::endl;
    }
    else {
        //if tokens[0], which is the product, is equal to any of the available product, continue
        if(tokens[0] == "BTC/USDT" || tokens[0] == "DOGE/BTC" || tokens[0] == "DOGE/USDT" || tokens[0] == "ETH/BTC" || tokens[0] == "ETH/USDT") {
            if(tokens[1] == "ask") {
                int timestamps = std::stod(tokens[2]);
                std::vector<double> askTimestamps;
                std::string time = currentTime;

                for(int i = 0; i < timestamps; ++i) {
                    //initialise askEntries as a vector of OrderBookEntry,
                    //calls getOrders and parse in OrderBookType::ask, product and current timestamp
                    std::vector<OrderBookEntry> askEntries = orderBook.getOrders(OrderBookType::ask, tokens[0], time);

                    //calls getAvgPrice, parse in askEntries and bidEntries to print the avg price of ask made
                    //push it into the askTimestamps vector
                    askTimestamps.push_back(OrderBook::getAvgPrice(askEntries));

                    //set the time to the previous time
                    time = orderBook.getPrevTime(time);
                }
                
                //calculate average of askTimestamps vector
                float average = accumulate(askTimestamps.begin(), askTimestamps.end(), 0.0f) / askTimestamps.size();

                //print average ask price for product over timestamps
                std::cout << "Average ask price for " << tokens[0] << " over " << tokens[2] << " timestamps is: " << average << std::endl;
            }

            else if(tokens[1] == "bid") {
                int timestamps = std::stod(tokens[2]);
                std::vector<double> bidTimestamps;
                std::string time = currentTime;

                for(int i = 0; i < timestamps; ++i) {
                    //initialise bidEntries as a vector of OrderBookEntry,
                    //calls getOrders and parse in OrderBookType::ask, product and current timestamp
                    std::vector<OrderBookEntry> bidEntries = orderBook.getOrders(OrderBookType::bid, tokens[0], time);

                    //calls getAvgPrice, parse in askEntries and bidEntries to print the avg price of bid made
                    //push it into the bidTimestamps vector
                    bidTimestamps.push_back(OrderBook::getAvgPrice(bidEntries));

                    //set the time to the previous time
                    time = orderBook.getPrevTime(time);
                }
                
                //calculate average of askTimestamps vector
                float average = accumulate(bidTimestamps.begin(), bidTimestamps.end(), 0.0f) / bidTimestamps.size();

                //print average ask price for product over timestamps
                std::cout << "Average bid price for " << tokens[0] << " over " << tokens[2] << " timestamps is: " << average << std::endl;
            }
            ////if tokens[1], which is the order type, is not ask or bid, return error message
            else {
                std::cout << "MerkelMain::printAvgTS Bad input! " << input << std::endl;
            }
        }
        //if tokens[0], which is the product, doesnt equal to any of the available product, return error message
        else {
            std::cout << "MerkelMain::printAvgTS Bad input! " << input << std::endl;
        }
    }
}



//prints prediction
void MerkelMain::printPredict() {
    //prompts user to input an ask
    std::cout << "" << std::endl;
    std::cout << "Enter currency to predict: product  eg ETH/BTC" << std::endl;

    //initialise a string named input
    //get the cin with getline function, and save it as input
    std::string input;
    std::getline(std::cin, input);

    //if tokens[0], which is the product, is equal to any of the available product, continue
    if(input == "BTC/USDT" || input == "DOGE/BTC" || input == "DOGE/USDT" || input == "ETH/BTC" || input == "ETH/USDT") {
        //initialise askEntries as a vector of OrderBookEntry,
        //calls getOrders and parse in OrderBookType::ask, product and current timestamp
        //do the same for bidEntries
        std::vector<OrderBookEntry> askEntries = orderBook.getOrders(OrderBookType::ask, input, currentTime);
        std::vector<OrderBookEntry> bidEntries = orderBook.getOrders(OrderBookType::bid, input, currentTime);

        //calls getPrediction, parse in askEntries and bidEntries to print the prediction of whether the user should ask, bid, or do nothing(null) for this currency
        std::cout << "PREDICTION for " << input << ": " << OrderBook::getPrediction(askEntries, bidEntries) << std::endl;
    }
    //if tokens[0], which is the product, doesnt equal to any of the available product, return error message
    else {
        std::cout << "MerkelMain::printPredict Bad input! " << input << std::endl;
    }
}



//function that allows user to enter ask
void MerkelMain::enterAsk()
{
    //prompts user to input an ask
    std::cout << "" << std::endl;
    std::cout << "Make an ask - enter the amount: product,price, amount, eg  ETH/BTC,200,0.5" << std::endl;

    //initialise a string named input
    //get the cin with getline function, and save it as input
    std::string input;
    std::getline(std::cin, input);

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
            if (wallet.canFulfillOrder(obe) && obe.amount > 0)
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
void MerkelMain::enterBid()
{
    //prompts user to input a bid
    std::cout << "" << std::endl;
    std::cout << "Make an bid - enter the amount: product,price, amount, eg  ETH/BTC,200,0.5" << std::endl;

    //initialise a string named input
    //get the cin with getline function, and save it as input
    std::string input;
    std::getline(std::cin, input);

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
void MerkelMain::printWallet()
{
    //calls toString function and print the wallet assets
    std::cout << "" << std::endl;
    std::cout << wallet.toString() << std::endl;
}



//goes to the next timestamp
void MerkelMain::gotoNextTimeframe()
{
    std::cout << "*** Going to next time frame. ***" << std::endl;

    //calls getKnownProducts and loops through the vector of products returned from it
    for (std::string& p : orderBook.getKnownProducts())
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

    //gets the current timestamp and prints it out onto terminal
    std::cout << "Current time is: " << currentTime << std::endl;
}



//gets and returns the user option
std::string MerkelMain::getUserOption()
{
    //initialise userOption as string
    std::string userOption;

    //initialise line as a string
    //prompts the user to type in an option
    //save the cin from the terminal to line
    std::string line;
    std::cout << "" << std::endl;
    std::cout << "Enter a command" << std::endl;
    std::getline(std::cin, line);

    //try: save the cin line as userOption
    //else catch error
    try{
        userOption = line;
    }catch(const std::exception& e)
    {
        // 
    }

    //prints out what user typed and return userOption
    std::cout << "Processing command......" << std::endl;
    std::cout << "" << std::endl;
    return userOption;
}



//process and calls functions based on the user option parsed in
void MerkelMain::processUserOption(std::string userOption)
{
    if (userOption == "help") 
    {
        printHelp();
    }
    else if (userOption == "help min")
    {
        std::cout << "min - ETH/BTC,ask -> minimum ETH/BTC ask is 1.0" << std::endl;
        std::cout << "min - ETH/BTC,bid -> minimum ETH/BTC bid is 1.0" << std::endl;
    }
    else if (userOption == "help max")
    {
        std::cout << "max - ETH/BTC,ask -> maximum ETH/BTC ask is 1.0" << std::endl;
        std::cout << "max - ETH/BTC,bid -> maximum ETH/BTC bid is 1.0" << std::endl;
    }
    else if (userOption == "help median")
    {
        std::cout << "median - ETH/BTC,ask -> median ETH/BTC ask is 1.0" << std::endl;
        std::cout << "median - ETH/BTC,bid -> median ETH/BTC bid is 1.0" << std::endl;
    }
    else if (userOption == "help avg")
    {
        std::cout << "average - ETH/BTC,ask -> average ETH/BTC ask is 1.0" << std::endl;
        std::cout << "average - ETH/BTC,bid -> average ETH/BTC bid is 1.0" << std::endl;
    }
    else if (userOption == "help avgts")
    {
        std::cout << "avg - ETH/BTC,ask,10 -> average ETH/BTC ask over last 10 time steps" << std::endl;
        std::cout << "avg - ETH/BTC,bid,10 -> average ETH/BTC bid over last 10 time steps" << std::endl;
    }
    else if (userOption == "help predict")
    {
        std::cout << "predict - ETH/BTC -> predict if user should ask or bid for ETH/BTC" << std::endl;
    }
    else if (userOption == "help ask")
    {
        std::cout << "ask BTC/USDT,5200,5 -> selling 5 BTC, at 5200 USDT for each BTC" << std::endl;
    }
    else if (userOption == "help bid")
    {
        std::cout << "bid ETH/BTC,10,0.5 -> buying 0.5 ETH, paying 10BTC for each ETH" << std::endl;
    }
    else if (userOption == "allstats") 
    {
        printMarketStats();
    }
    else if (userOption == "prod") 
    {
        printProducts();
    }
    else if (userOption == "min") 
    {
        printMin();
    }
    else if (userOption == "max") 
    {
        printMax();
    }
    else if (userOption == "median") 
    {
        printMedian();
    }
    else if (userOption == "avg") 
    {
        printAvg();
    }
    else if (userOption == "avgts") 
    {
        printAvgTS();
    }
    else if (userOption == "predict") 
    {
        printPredict();
    }
    else if (userOption == "ask") 
    {
        enterAsk();
    }
    else if (userOption == "bid") 
    {
        enterBid();
    }
    else if (userOption == "wallet") 
    {
        printWallet();
    }
    else if (userOption == "time") 
    {
        //gets the current timestamp and prints it out onto terminal
        std::cout << "Current time is: " << currentTime << std::endl;
    }
    else if (userOption == "step") 
    {
        gotoNextTimeframe();
    }
    else // bad input
    {
        std::cout << "Invalid command! You typed: " << userOption << std::endl;
    }
}