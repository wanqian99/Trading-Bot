#include <iostream>
#include "Wallet.h"
#include "MerkelMain.h"
#include "tradingBot.h"



int getUserOption();
void processUserOption(int userOption);
void printMenu();



//main function
int main()
{   
    std::cout << "" << std::endl;
    std::cout << "**********************************************************************************************************************" << std::endl << std::endl;
    std::cout << "                                         Welcome to the Merkelrex Trading Bot                                         " << std::endl << std::endl;
    std::cout << "**********************************************************************************************************************" << std::endl << std::endl;
    std::cout << "The Merklerex exchange simulation allows trading activities such as bidding for and offering trades, market analysis." << std::endl;

    int input;

    //while true, call printMenu, getUserOption, processUserOption repeatedly
    while(true)
    {
        printMenu();
        input = getUserOption();
        processUserOption(input);
    }
}



//gets the user option
int getUserOption()
{
    //initialise userOption to 0
    int userOption = 0;
    std::string line;

    //prompts the user to input an option
    //get the cin input and save it as a string
    std::cout << "Type in option 1 or 2:" << std::endl;
    std::getline(std::cin, line);

    //convert the line from strong to integer
    //catch exception if there is an error
    try{
        userOption = std::stoi(line);
    }catch(const std::exception& e)
    {
        // 
    }

    //prints out what the user has typed
    std::cout << "You chose: " << userOption << std::endl;
    std::cout << "Processing command......" << std::endl;
    return userOption;
}



// calls either manual mode or bot mode based on user input
void processUserOption(int userOption)
{
    if (userOption == 0) // bad input
    {
        std::cout << "Invalid choice. Choose option 1 or 2" << std::endl;
    }
    if (userOption == 1)
    {
        MerkelMain app{};
        app.init();
    }
    if (userOption == 2)
    {
        MerkelBot app{};
        app.init();
    }
}



//print the menu that lets user chose manual or automated trading
void printMenu()
{
    std::cout << "" << std::endl;
    std::cout << "===============================" << std::endl;

    // 1 manual trading
    std::cout << "1: Start manual trading " << std::endl;
    // 2 automated trading
    std::cout << "2: Start automated trading bot" << std::endl;

    std::cout << "===============================" << std::endl;
    std::cout << "" << std::endl;
}
