#include "../header/StockAnalysis.h"

StockAnalysis::StockAnalysis(const std::string& symbol, double sellPrice, double buyPrice)
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

size_t StockAnalysis::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct* mem = (struct MemoryStruct*)userp;

    mem->memory = (char*)realloc(mem->memory, mem->size + realsize + 1);
    if (mem->memory == NULL) {
        std::cerr << "Memory allocation error" << std::endl;
        return 0;
    }

    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

void StockAnalysis::checkStockPrice() {
    if (curl_) {
        std::string url = "https://www.alphavantage.co/query?function=GLOBAL_QUOTE&symbol=" + symbol_ + "&apikey=W69EFB1QD6AL04I2";

        curl_easy_setopt(curl_, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl_, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl_, CURLOPT_WRITEDATA, (void*)&chunk_);

        CURLcode res = curl_easy_perform(curl_);

        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        } 
        else {
            std::string json_data = chunk_.memory;
            curl_easy_setopt(curl_, CURLOPT_URL, url.c_str());

            curl_easy_setopt(curl_, CURLOPT_SSL_VERIFYPEER, 0L);

            curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl_, CURLOPT_WRITEDATA, (void*)&chunk_);

            // Parse JSON response to extract stock price
            std::string price_field = "\"05. price\": \"";
            size_t start_pos = json_data.find(price_field);

            // Extract the price value from string
            start_pos += price_field.length();
            size_t end_pos = json_data.find("\"", start_pos);
            std::string price_str = json_data.substr(start_pos, end_pos - start_pos);

            std::cout << "Price: " << price_str << std::endl;          
        
            // Convert the price to a double
            std::cout << "json value: " << json_data << std::endl;
            double current_price = std::stod(price_str);

            // Check if the current price is above the threshold
            if (current_price > sellPrice_) {
                std::cout << "Stock " << symbol_ << " has passed the threshold, it's advised to sell. Current price: " << current_price << std::endl;
            } 
            else if (current_price < buyPrice_) {
                std::cout << "Stock " << symbol_ << " is below the threshold, it's advised to buy. Current price: " << current_price << std::endl;
            }
            else {
                std::cout << "Stock " << symbol_ << " is not above or below the threshold. Current price: " << current_price << std::endl;
            }
            
        }
    }   
}

void StockAnalysis::run() {
    while (true) {
        checkStockPrice();

        // Sleep for some time before checking again
        std::this_thread::sleep_for(std::chrono::minutes(1));
    }
}