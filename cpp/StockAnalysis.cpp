#include "../header/StockAnalysis.h"

StockAnalysis::StockAnalysis(const string& symbol, double sellPrice, double buyPrice)
    : symbol_(symbol), sellPrice_(sellPrice), buyPrice_(buyPrice), curl_(nullptr) {
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl_ = curl_easy_init();

    chunk_.memory = (char*)malloc(1);
    chunk_.size = 0;
}

StockAnalysis::~StockAnalysis() {
    curl_easy_cleanup(curl_);
    curl_global_cleanup();
    free(chunk_.memory);
}

// Handle received data (response body) from the server.
size_t StockAnalysis::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct* mem = (struct MemoryStruct*)userp;

    mem->memory = (char*)realloc(mem->memory, mem->size + realsize + 1);
    if (mem->memory == NULL) {
        cerr << "Memory allocation error" << endl;
        return 0;
    }

    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

int StockAnalysis::checkStockPrice() {
    if (curl_) {
        string url = "https://www.alphavantage.co/query?function=GLOBAL_QUOTE&symbol=" + symbol_ + "&apikey=W69EFB1QD6AL04I2";

        curl_easy_setopt(curl_, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl_, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl_, CURLOPT_WRITEDATA, (void*)&chunk_);

        CURLcode res = curl_easy_perform(curl_);

        if (res != CURLE_OK) {
            cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
        } 
        else {
            string json_data = chunk_.memory;
            curl_easy_setopt(curl_, CURLOPT_URL, url.c_str());

            curl_easy_setopt(curl_, CURLOPT_SSL_VERIFYPEER, 0L);

            curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl_, CURLOPT_WRITEDATA, (void*)&chunk_);

            // Parse JSON response to extract stock price
            string price_field = "\"05. price\": \"";
            size_t start_pos = json_data.find(price_field);

            // Extract the price value from string
            start_pos += price_field.length();
            size_t end_pos = json_data.find("\"", start_pos);
            string price_str = json_data.substr(start_pos, end_pos - start_pos);       
        
            // Convert the price to a double
            cout << "json value: " << json_data << endl;
            double current_price = stod(price_str);

            // Check if the current price is above or below the thresholds
            if (current_price > sellPrice_) {
                cout << "Stock " << symbol_ << " is above the threshold, it's advised to sell. Current price: " << current_price << endl;
                return 1;
            } 
            else if (current_price < buyPrice_) {
                cout << "Stock " << symbol_ << " is below the threshold, it's advised to buy. Current price: " << current_price << endl;
                return 2;
            }
            else {
                cout << "Stock " << symbol_ << " is not above or below the threshold. Current price: " << current_price << endl;
                return 0;
            }
            
        }
    }
    return -1;   
}