# B3-Stock-Analysis
Assignment for a B3 Stock Analysis, in C++

The application sends an email, based on a config file, with the destination + SMTP config. 
The email will contain the instructions to either buy or sell a certain active of the stock market, based on prices informed to the program.

Usage: ./main <stock-name> <Price to Sell> <Price to Buy>

Example: ./main PETR4 22.67 22.59

Compilation: g++ -I<path-to-curl-include> -o main cpp/Mail.cpp main.cpp -L<path-to-curl-lib> -lcurl
*The application needs the library curl, from c++ to compile. Can be found in: https://curl.se/
