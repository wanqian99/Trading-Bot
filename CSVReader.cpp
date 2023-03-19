#include "CSVReader.h"
#include <iostream>
#include <fstream>



//CSVReader constructor
CSVReader::CSVReader()
{

}



//reads the csv file that is parsed in as a string
std::vector<OrderBookEntry> CSVReader::readCSV(std::string csvFilename)
{
    //initialise a vector of OrderBookEntry variable
    std::vector<OrderBookEntry> entries;

    //input the csv file
    std::ifstream csvFile(csvFilename);

    //initialise a string variable
    std::string line;

    //checks if the file is open
    if (csvFile.is_open())
    {
        //get the line from the csv file, save it as string named line
        while(std::getline(csvFile, line))
        {
            try {
                //call the tokenise function, parse in the line to tokenise and the comma as seperator
                //save it as an OrderBookEntry named obe
                //push it into the entries vector
                OrderBookEntry obe = stringsToOBE(tokenise(line, ','));
                entries.push_back(obe);
            }catch(const std::exception& e)
            {
                //prints out error message
                std::cout << "CSVReader::readCSV bad data"  << std::endl;
            }
        }// end of while
    }    

    //prints out number of entries read from the csv file
    std::cout << "CSVReader::readCSV read " << entries.size() << " entries"  << std::endl;
    return entries; 
}



//tokenise function that seperates the line parsed in, by the seperator
std::vector<std::string> CSVReader::tokenise(std::string csvLine, char separator)
{
    //vector of strings called tokens
    std::vector<std::string> tokens;

    //signed integer start and end, where they can be either positive or negative integer
    signed int start, end;

    //initialise token as string
    std::string token;

    //search from the beginning (position 0) of the line
    //make the start be the first element that doesnt match the seperator
    start = csvLine.find_first_not_of(separator, 0);

    //while end > 0
    do{
        //search from the start position
        //make the end be first element that matches the seperator
        end = csvLine.find_first_of(separator, start);

        //if the start = length of the line or start = end,
        //means there is only one token, break
        if (start == csvLine.length() || start == end) break;

        //if end > 0
        //make token be a substring of the line, from start to end-start
        //else make token be a substring of the line, from start to the length of the line - start
        if (end >= 0) token = csvLine.substr(start, end - start);
        else token = csvLine.substr(start, csvLine.length() - start);

        //push the token into the vector called tokens
        tokens.push_back(token);

        //set start to be end + 1
        start = end + 1;
    } while(end > 0);

    return tokens; 
}



//converts each line in the csv file from strings to an OrderBookEntry, with price and amount as double
OrderBookEntry CSVReader::stringsToOBE(std::string priceString, 
                                       std::string amountString, 
                                       std::string timestamp, 
                                       std::string product, 
                                       OrderBookType orderType)
{
    //initialise price and amount as double
    double price, amount;

    //try convert price and amount from the input string type to double type
    //else throw an exception
    try {
         price = std::stod(priceString);
         amount = std::stod(amountString);
    }catch(const std::exception& e){
        std::cout << "CSVReader::stringsToOBE Bad float! " << priceString<< std::endl;
        std::cout << "CSVReader::stringsToOBE Bad float! " << amountString<< std::endl; 
        throw;        
    }

    //save them as an OrderBookEntry obe with the 5 arguments
    //return obe
    OrderBookEntry obe{price, 
                    amount, 
                    timestamp,
                    product, 
                    orderType};
                
    return obe;
}



//converts each line in the csv file from strings to an OrderBookEntry, with the tokens that is parsed in
OrderBookEntry CSVReader::stringsToOBE(std::vector<std::string> tokens)
{
    //initialise price and amount as double
    double price, amount;

    //there isnt 5 tokens, means something is wrong with that line, throw an exception
    if (tokens.size() != 5) // bad
    {
        std::cout << "Bad line " << std::endl;
        throw std::exception();
    }

    // we have 5 tokens
    // try: make price and amount be token[3] and token[4], and convert them to double
    //catch: throw an error message if there is an error
    try {
         price = std::stod(tokens[3]);
         amount = std::stod(tokens[4]);
    }catch(const std::exception& e){
        std::cout << "" << std::endl;
        std::cout << "CSVReader::stringsToOBE Bad float! " << tokens[3]<< std::endl;
        std::cout << "CSVReader::stringsToOBE Bad float! " << tokens[4]<< std::endl; 
        throw;        
    }

    //save them as an OrderBookEntry obe with the 5 arguments
    //return obe
    OrderBookEntry obe{price, 
                        amount, 
                        tokens[0],
                        tokens[1], 
                        OrderBookEntry::stringToOrderBookType(tokens[2])};

    return obe; 
}