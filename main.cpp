#include <iostream>
#include <sstream>
#include <fstream>
#include <unordered_map>

#include "header/StockAnalysis.h"
#include "header/Mail.h"

using namespace std;

// Parse the config file to a map
unordered_map<string, string> parseConfigFile(const string &filename) {
    unordered_map<string, string> config;
    ifstream file(filename);

    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            // Skip lines whit "[]"
            if (line[0] == '[') {
                continue;
            }

            istringstream iss(line);
            string key, value;
            if (getline(iss, key, '=') && getline(iss, value)) {
                config[key] = value;
            } else {
                cerr << "Invalid line format: " << line << endl;
            }
        }
        file.close();
    } else {
        cerr << "Unable to open config file: " << filename << endl;
    }

    return config;
}

int main(int argc, char *argv[]){

    if(argc != 4){
        cout << "Usage: " << argv[0] << " <stock> <Price to Sell> <Price to buy>" << endl;
        return 1;
    }

    string stock = argv[1];
    float sellPrice = stof(argv[2]);
    float buyPrice = stof(argv[3]);
    int mailMessage = -1;

    unordered_map<string, string> config = parseConfigFile("config/config.ini");

    StockAnalysis StockAnalysis(stock, sellPrice, buyPrice);

    // Send the mail
    EmailSender emailSender(config["SMTP_URL"]);
    const char* senderEmail = "mockup430@gmail.com";
    const char* recipientEmail = config["Recipient_Email"].c_str();
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

        // Sleep for some time before checking again (15 min - delay from the Alpha Vantage API)
        this_thread::sleep_for(chrono::minutes(15));
    }
    return 0;
}