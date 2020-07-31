#include "include/myAnimeList.h"

#include "include/json.hpp"
#include <curl/curl.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <regex>
#include <vector>

struct Anime;

struct Manga;


using json = nlohmann::json;

std::string MyAnimeListAPI::getResults(std::string in) {
    std::string out;
    std::string regex = "\\\"results\\\":\\\[(.*?)]";
    std::regex rgx(regex.c_str());
    std::smatch match;
    if (std::regex_search(in.cbegin(), in.cend(), match, rgx)) {
        out = match[1];
    }

    return out;
}

std::vector<Anime> MyAnimeListAPI::searchAnime(std::string name, int limit) {
    std::vector<Anime> output;

    size_t pos;
    while ((pos = name.find(' ')) != std::string::npos) {
        name.replace(pos, 1, "%20");
    }
    std::string request = "https://api.jikan.moe/v3/search/anime?q="+name+"&limit=1";
    std::string res = getRequest(request);
    std::cout << "Answer: \n" << res << '\n';
    

    //res = getResults(res);
    auto jOut = json::parse(res);
    int results = 0;
    if (jOut.find("status") != jOut.end()) {
        auto status = jOut.find("status");
        std::cout << "MyAnimeList returned " << *status << " (" << *jOut.find("type") << ") for this.\n"; 
        int x = *status;
        throw (x);
    }
    std::cout << "b1\n";
    for (const auto& item : jOut) {
        std::cout << "b2\n";
        std::cout << item.dump() << '\n';
        /*Anime next;
        next.title = *item.find("title");
        next.synopsis = *item.find("synopsis");
        next.type = *item.find("type");
        next.start_date = *item.find("start_date");    
        next.end_date = *item.find("end_date");       
        next.rated = *item.find("rated");
        next.score = std::to_string((long long) *item.find("score"));
        next.episodes = std::to_string((long long) *item.find("episodes"));
        */
        results++;

        if (limit == results) {
            break;
        }
    }

    
    
    return output;
}
        


std::string MyAnimeListAPI::getField(std::string fieldName, std::string source) {
    std::string out = "Field \""+fieldName+"\" was not found!";
    std::string regex = "\\\""+fieldName+"\\\":\\\"(.*?)\\\"";

    std::regex rgx(regex.c_str());
    std::smatch match;
    if (std::regex_search(source.cbegin(), source.cend(), match, rgx)) {
        out = match[1];
    }
    return out;
}

std::string MyAnimeListAPI::getRequest(std::string request) {
    CURL *eInit = curl_easy_init();
    std::string readBuffer;

    if (eInit) {
        curl_easy_setopt(eInit, CURLOPT_URL, request.c_str());
        curl_easy_setopt(eInit, CURLOPT_WRITEFUNCTION, MyAnimeListAPI::WriteCallback);
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

size_t MyAnimeListAPI::WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string*)userp)->append((char*) contents, size * nmemb);
    return size * nmemb;
}
