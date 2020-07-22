#include "include/myAnimeList.h"

#include "include/json.hpp"
#include <curl/curl.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <regex>

struct Anime;

struct Manga;


using json = nlohmann::json;

std::string getResults(std::string in) {
    std::string out;
    std::string regex = "\\\"results\\\":\\\[(.*?)]";
    std::regex rgx(regex.c_str());
    std::smatch match;
    if (std::regex_search(in.cbegin(), in.cend(), match, rgx)) {
        out = match[1];
    }

    return out;
}

Anime getAnime(std::string name) {
    struct Anime output;

    size_t pos;
    while ((pos = name.find(' ')) != std::string::npos) {
        name.replace(pos, 1, "%20");
    }
    std::string request = "https://api.jikan.moe/v3/search/anime?q="+name+"&limit=1";
    std::string res = getResults(request);

    res = getResults(res);
    auto jResults = json::parse(res);

    output.title = *jResults.find("title");
    output.synopsis = *jResults.find("synopsis");
    output.type = *jResults.find("type");
    output.start_date = *jResults.find("start_date");    
    output.end_date = *jResults.find("end_date");       
    output.rated = *jResults.find("rated");
    output.score = *jResults.find("score");
    output.episodes = *jResults.find("episodes");
    
    return output;
}
        


std::string getField(std::string fieldName, std::string source) {
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

size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string*)userp)->append((char*) contents, size * nmemb);
    return size * nmemb;
}
