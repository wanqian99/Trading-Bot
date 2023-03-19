#pragma once

#include "OrderBookEntry.h"
#include <vector>
#include <string>


//this CSVReader class allows the csv file to be read, allowing each line to be processed as when necessary.
class CSVReader
{
    public:
        //constructor
        CSVReader();

        //reads the csv file
        static std::vector<OrderBookEntry> readCSV(std::string csvFile);

        //tokenise the line in the csv file that is parsed in, by the seperator
        static std::vector<std::string> tokenise(std::string csvLine, char separator);

        //converts each line in the csv file from strings to an OrderBookEntry, with price and amount as double
        static OrderBookEntry stringsToOBE(std::string price, 
                                           std::string amount, 
                                           std::string timestamp, 
                                           std::string product, 
                                           OrderBookType OrderBookType);

    private:
        //converts each line in the csv file from strings to an OrderBookEntry, with the tokens that is parsed in
        static OrderBookEntry stringsToOBE(std::vector<std::string> strings);
     
};