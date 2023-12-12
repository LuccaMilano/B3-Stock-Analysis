#ifndef MAIL_H
#define MAIL_H

#include <curl/curl.h>

class EmailSender {
public:
    EmailSender();
    ~EmailSender();

    void sendEmail(const char* from, const char* to, const char* subject, const char* body);

private:
    CURL* curl;
    struct curl_slist* recipients;

    void initialize();
    void cleanup();
};

#endif // MAIL_H