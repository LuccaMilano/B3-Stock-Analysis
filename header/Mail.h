#ifndef MAIL_H
#define MAIL_H

#include <iostream>
#include <curl/curl.h>

using namespace std;

class EmailSender {
public:
    EmailSender(string smtpURL);
    ~EmailSender();

    void sendEmail(const char* from, const char* to, const char* subject, const char* body);
    static size_t payloadSource(char* ptr, size_t size, size_t nmemb, void* userp);

private:
    CURL* curl;
    struct curl_slist* recipients;
    const char* smtpURLptr;

    void cleanup();
};

#endif // MAIL_H