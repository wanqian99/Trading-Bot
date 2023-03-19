# Trading-Bot

This is a trading advisor bot created with C++. It guides users through their trading proces, providing help commands (explanation of the different commands), shows the current available products to do trading on, calculate statistics such as min, max, median, avg for the user, prediction that tells the user if they should ask, bid or do nothing for that currency. It also includes a function, time, which tells the user which timestamp they are currently at, and a step function to move on to the next timestamp.

### How to run:
1) g++ --std=c++11 MerkelMain.cpp tradingBot.cpp CSVReader.cpp main.cpp OrderBook.cpp OrderBookEntry.cpp Wallet.cpp
2) ./a.out

### Demo Video
https://user-images.githubusercontent.com/62084317/226182718-31c83a1c-9005-4ba1-ace2-afcefbe16817.mp4
