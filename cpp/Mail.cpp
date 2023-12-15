#include "../header/Mail.h"

// Set SMTP config for sending the mail
EmailSender::EmailSender(string smtpURL) : smtpURLptr(smtpURL.c_str()){
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        // Set the mail SMTP and user
        curl_easy_setopt(curl, CURLOPT_URL, smtpURLptr);
        curl_easy_setopt(curl, CURLOPT_USERNAME, "mockup430@gmail.com");
        curl_easy_setopt(curl, CURLOPT_PASSWORD, "hbma qwxz bzxy ccwe");

        // Disable SSL certificate verification (shouldn't go to production, testing only)
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        recipients = nullptr; 
    }
}

EmailSender::~EmailSender() {
    cleanup();
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

// Reads the email data passed through curl
size_t EmailSender::payloadSource(char* ptr, size_t size, size_t nmemb, void* userp) {
    const char** payload_text = reinterpret_cast<const char**>(userp);

    if (!payload_text || !(*payload_text)) {
        return 0;
    }

    const char* data = *payload_text;
    size_t len = strlen(data);


    if (len > 0) {
        if (len > size * nmemb) {
            len = size * nmemb;
        }

        memcpy(ptr, data, len);
        *payload_text += len;

        return len;
    }

    return 0;
}

// Assemble the mail template and send it 
void EmailSender::sendEmail(const char* from, const char* to, const char* subject, const char* body) {
    if (curl) {
        // Set sender and recipient email addresses
        curl_easy_setopt(curl, CURLOPT_MAIL_FROM, from);
        recipients = curl_slist_append(recipients, to);
        curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
        
        // Format the mail data for the payloadSource
        std::string emailData = std::string("Subject: ") + subject + "\r\n" +
                        "From: " + from + "\r\n" +
                        "To: " + to + "\r\n" +
                        "\r\n" + body + "\r\n";

        const char* emailDataPtr = emailData.c_str();

        // Set the payload function to read the email data
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, payloadSource);
        curl_easy_setopt(curl, CURLOPT_READDATA, &emailDataPtr);
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        // Send the mail
        CURLcode res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }
    }
}