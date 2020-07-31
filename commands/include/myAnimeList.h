#pragma once
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <vector>

/**
 * Structure for saving some information
 * that MyAnimeList provides for an anime.
 */
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

/**
 * Structure for saving some information
 * that MyAnimeList provides for a manga.
 */
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

/**
 * A class for interacting with myanimelist.com
 * 
 * TODO: This class will probably be rewritten. 
 * It's currently not very functional. :')
 */
class MyAnimeListAPI {
    public:
        /**
         * Searches the MyAnimeList database for a given anime.
         * Will return the results as a vector of anime structs.
         * 
         * Returns the first n results, default is 1, maximum is 20.
         */
        static std::vector<Anime> searchAnime(std::string, int = 1);
    private:
        static std::string getResults(std::string);
        static std::string getField(std::string, std::string);
        static std::string getRequest(std::string);
        static size_t WriteCallback(void*, size_t, size_t, void*);
};

