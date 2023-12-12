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

    cout << stock << " " << sellPrice << " "<< buyPrice;

    // Send the mail
    EmailSender emailSender;
    const char* senderEmail = "mockup430@gmail.com";
    const char* recipientEmail = "luccsamilano@gmail.com";
    const char* emailSubject = "Hey there";
    const char* emailBody = "Hello, this is a test email from C++ using libcurl.";

    emailSender.sendEmail(senderEmail, recipientEmail, emailSubject, emailBody);

    return 0;
}