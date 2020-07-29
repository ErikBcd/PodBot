#pragma once
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <vector>

struct Anime {
    std::string title;
    std::string synopsis;
    std::string type;
    
    std::string status;
    std::string rated;
    std::string genre;
    std::string start_date;
    std::string end_date;
    
    std::string imageURL;
    std::string malURL;

    std::string mal_id;
    std::string episodes;
    std::string score;
};

struct Manga {
    std::string title;
    std::string synopsis;
    std::string type;
    std::string imageURL;
    std::string malURL;
    std::string status;
    std::string score;
    std::string chapters;
    std::string volumes;
    std::string start_date;
    std::string end_date;
};

class MyAnimeListAPI {
    public:
        static std::vector<Anime> searchAnime(std::string, int = 1);
    private:
        static std::string getResults(std::string);
        static std::string getField(std::string);
        static std::string getRequest(std::string);
        static size_t WriteCallback(void*, size_t, size_t, void*);
};

