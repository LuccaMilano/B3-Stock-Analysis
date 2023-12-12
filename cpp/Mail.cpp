#include "../header/Mail.h"
#include <iostream>

EmailSender::EmailSender() {
    initialize();
}

EmailSender::~EmailSender() {
    cleanup();
}

// Set config for sending the mail
void EmailSender::initialize() {
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        //curl_easy_setopt(curl, CURLOPT_URL, "smtps://smtp-mail.outlook.com:587");
        curl_easy_setopt(curl, CURLOPT_URL, "smtps://smtp.gmail.com:465");
        curl_easy_setopt(curl, CURLOPT_USERNAME, "mockup430@gmail.com");
        curl_easy_setopt(curl, CURLOPT_PASSWORD, "hbma qwxz bzxy ccwe");

        // curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1_2);
        // curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);

        // Enable SSL/TLS
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);

        // Set the version of TLS to use (TLSv1.2)
        curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1_2);

        // Disable SSL certificate verification (for testing purposes)
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        recipients = nullptr; // Initialize recipients list
    }
}

// Destructor to clean the lists
void EmailSender::cleanup() {
    if (curl) {
        curl_easy_cleanup(curl);
    }

    if (recipients) {
        curl_slist_free_all(recipients);
    }

    curl_global_cleanup();
}

// Assemble the mail template and send it 
void EmailSender::sendEmail(const char* from, const char* to, const char* subject, const char* body) {
    if (curl) {
        // Set sender and recipient email addresses
        curl_easy_setopt(curl, CURLOPT_MAIL_FROM, from);
        recipients = curl_slist_append(recipients, to);
        curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

        // Set the email subject and body
        const char* emailData[] = {
            R"(Subject: )", subject,
            "\r\n",
            body
        };

        curl_easy_setopt(curl, CURLOPT_READDATA, emailData);
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        // Perform the email send
        CURLcode res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }
    }
}