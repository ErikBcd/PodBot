#include <iostream>
#include <curl/curl.h>
#include <vector>
#include <thread>
#include "json.hpp"

struct KrakenData {
    std::string currencyPair;

    double ask;
    double bid;
    double last_trade;

    double volume;
    double volume24h;
    double volAvgPrice;
    double volAvgPrice24h;

    int numberOfTradesToday;
    int numberOfTrades24h;

    double low;
    double low24h;
    double high;
    double high24h;

    double opening;

    bool error = false;
    std::vector<std::string> errmsg;
};

class KrakenAPIListener {
    using json = nlohmann::json;
    public:
        /** @brief: Returns the current KrakenData */
        static KrakenData getData();
        /** @brief: Returns the last KrakenData without error flag */
        static KrakenData getFallbackData();
        /** @brief: Stops the thread */
        static void stopRunner();
        /** @brief: Starts the thread */
        static void startRunner();
        /** @brief: Changes the currency pair to the given one */
        static void changeCurrency(std::string);
    private:
        static std::thread runThread;
        static bool running;
        static std::string currPair;

        static void runner();

        static KrakenData current; 
        static KrakenData former;

        
        static void extract_data(std::string);
        static void writeData(KrakenData);

        /* Curl */
        static std::string getRequest(std::string);
        static size_t WriteCallback(void*, size_t, size_t, void*);
};