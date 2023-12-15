# B3-Stock-Analysis
Assignment for a B3 Stock Analysis, in C++


The application sends an email, based on a config file, with the destination + SMTP config. 
The email will contain the instructions to either buy or sell a certain active of the stock market, based on prices informed to the program.


Usage: ./main \<stock-name\> \<price-to-sell\> \<price-to-buy\>

Example: ./main PETR4.SA 22.67 22.59

Compilation: g++ -I\<path-to-curl-include\> -o main cpp/Mail.cpp main.cpp -L\<path-to-curl-lib\> -lcurl

*This application needs the library curl, from c++ to compile. Can be found in: https://curl.se/
*This application uses Alpha Vantage API (there's a 15 minutes delay between updates in the stock market). Can be found in: https://www.alphavantage.co/

The config file should have the SMTP url of the chosen mail system (gmail in the example) and the destinatary (in the format written in the example).
