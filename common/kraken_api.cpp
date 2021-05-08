#include "kraken_api.hh"

using json = nlohmann::json;

bool KrakenAPIListener::running = false;
KrakenData KrakenAPIListener::current;
KrakenData KrakenAPIListener::former;
std::thread KrakenAPIListener::runThread;
std::string KrakenAPIListener::currPair = "XDGEUR";

void KrakenAPIListener::changeCurrency(std::string curr) {
    currPair = curr;
}

void KrakenAPIListener::startRunner() {
    running = true;
    runThread = std::thread(runner);
    std::cout << "Started thread\n";
}

void KrakenAPIListener::stopRunner() {
    running = false;
    runThread.join();
}

KrakenData KrakenAPIListener::getFallbackData() {
    return former;
}

KrakenData KrakenAPIListener::getData() {
    return current;
}

void KrakenAPIListener::writeData(KrakenData data) {
    if (!data.error) {
        former = current;
    }
    current = data;
}

void KrakenAPIListener::runner() {
    while (running) {
        std::this_thread::sleep_for(std::chrono::seconds(3));
        if (!running) { break; } 
        std::string request = "https://api.kraken.com/0/public/Ticker?pair=" + currPair;
        try
        {
            std::string answer = getRequest(request);
            extract_data(answer);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
        
    }
    std::cout << "[Kraken API] Listener exiting...";
}

void KrakenAPIListener::extract_data(std::string data) {
    auto jData = json::parse(data);
    std::vector<std::pair<double, double>> prizes;
    std::vector<long> timestamps;

    auto error = *jData.find("error");
    if (error.size() != 0) {
        std::vector<std::string> errorMsgs;
        for (auto& err : error) {
            errorMsgs.push_back(err);
        }
        KrakenData kData = current;
        kData.currencyPair = currPair;
        kData.error = true;
        kData.errmsg = errorMsgs;

        writeData(kData);
        return;
    }

    auto results = *jData.find("result");
    results = *results.find(currPair);

    KrakenData kData;

    kData.currencyPair = currPair;

    auto res = *results.find("a");
    kData.ask                 = atof(((std::string) res[0]).c_str());

    res = *results.find("b");
    kData.bid                 = atof(((std::string) res[0]).c_str());

    res = *results.find("c");
    kData.last_trade          = atof(((std::string) res[0]).c_str());

    res = *results.find("v");
    kData.volume              = atof(((std::string) res[0]).c_str());
    kData.volume24h           = atof(((std::string) res[1]).c_str());

    res = *results.find("p");
    kData.volAvgPrice         = atof(((std::string) res[0]).c_str());
    kData.volAvgPrice24h      = atof(((std::string) res[1]).c_str());

    res = *results.find("t");
    kData.numberOfTradesToday = res[0];
    kData.numberOfTrades24h   = res[1];
    res = *results.find("l");
    kData.low                 = atof(((std::string) res[0]).c_str());
    kData.low24h              = atof(((std::string) res[1]).c_str());

    res = *results.find("h");
    kData.high                = atof(((std::string) res[0]).c_str());
    kData.high24h             = atof(((std::string) res[1]).c_str());

    res = *results.find("o");
    kData.opening             = atof(((std::string) res).c_str());

    writeData(kData);
}

std::string KrakenAPIListener::getRequest(std::string request) {
    CURL *eInit = curl_easy_init();
    std::string readBuffer;

    if (eInit) {
        curl_easy_setopt(eInit, CURLOPT_URL, request.c_str());
        curl_easy_setopt(eInit, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(eInit, CURLOPT_WRITEDATA, &readBuffer);
    }

    CURLcode cCode = curl_easy_perform(eInit);
    curl_easy_cleanup(eInit);
    if (cCode != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(cCode));
        readBuffer = "Fuck this shit I'm out";
    }

    return readBuffer;
}

size_t KrakenAPIListener::WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string*)userp)->append((char*) contents, size * nmemb);
    return size * nmemb;
}