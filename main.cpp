#include <iostream>
#include "header/StockAnalysis.h"
#include "header/Mail.h"

using namespace std;

int main(int argc, char *argv[]){

    if(argc != 4){
        cout << "Usage: " << argv[0] << " <stock> <Price to Sell> <Price to buy>" << endl;
        return 1;
    }

    string stock = argv[1];
    float sellPrice = stof(argv[2]);
    float buyPrice = stof(argv[3]);
    int mailMessage = -1;

    StockAnalysis StockAnalysis(stock, sellPrice, buyPrice);

    // Send the mail
    EmailSender emailSender;
    const char* senderEmail = "mockup430@gmail.com";
    const char* recipientEmail = "mockup430@gmail.com";
    const char* emailSubject = "Stock Market Trading";
    string emailBodySell = "Stock " + stock + " is above the price to sell: " + to_string(sellPrice) + ".";
    const char* emailBodySellPtr = emailBodySell.c_str();
    string  emailBodyBuy = "Stock " + stock +  " is below the price to buy: " + to_string(buyPrice) + ".";
    const char* emailBodyBuyPtr = emailBodyBuy.c_str();

    while (true) {
        // Check if the price of the stock is whithin range
        mailMessage = StockAnalysis.checkStockPrice();
        // Depending on the return code, send messages of buy, sell or no message
        switch (mailMessage)
        {
        case 1: //sell
            emailSender.sendEmail(senderEmail, recipientEmail, emailSubject, emailBodySellPtr);
            break;
        case 2: //buy
            emailSender.sendEmail(senderEmail, recipientEmail, emailSubject, emailBodyBuyPtr);
            break;
        default: //nothing
            break;
        }

        // Sleep for some time before checking again
        std::this_thread::sleep_for(std::chrono::minutes(1));
    }

    return 0;
}