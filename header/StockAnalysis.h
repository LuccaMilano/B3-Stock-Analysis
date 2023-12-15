#ifndef STOCKANALYSIS_H
#define STOCKANALYSIS_H

#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <curl/curl.h>

using namespace std;

struct MemoryStruct {
    char* memory;
    size_t size;
};

class StockAnalysis {
public:
    StockAnalysis(const string& symbol, double sellPrice, double buyPrice);
    ~StockAnalysis();

    int checkStockPrice();

private:
    string symbol_;
    double sellPrice_;
    double buyPrice_;
    CURL* curl_;
    struct MemoryStruct chunk_;

    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
};


#endif // STOCKANALYSIS_H