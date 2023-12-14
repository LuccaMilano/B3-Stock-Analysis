#ifndef MAIL_H
#define MAIL_H

#include <curl/curl.h>

class EmailSender {
public:
    EmailSender();
    ~EmailSender();

    void sendEmail(const char* from, const char* to, const char* subject, const char* body);
    static size_t payloadSource(char* ptr, size_t size, size_t nmemb, void* userp);

private:
    CURL* curl;
    struct curl_slist* recipients;

    void initialize();
    void cleanup();
};

#endif // MAIL_H