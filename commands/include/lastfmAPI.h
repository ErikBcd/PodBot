#pragma once

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <vector>
#include <cstdlib>

#include <curl/curl.h>
#include "json.hpp"



class IllegalArgumentException : public std::exception {
    public:
        explicit IllegalArgumentException(const std::string& message);
        virtual const char* what() const throw();
    private:
        std::string message_;
};

class lastFMError : public std::exception {
    public:
        explicit lastFMError(const std::string& message);
        virtual const char* what() const throw();
    private:
        std::string message_;
};

class lastfm {
    using json = nlohmann::json;
    
    public:
        struct Artist {
            public:
                Artist(std::string);

                std::string title;
                std::string lastFM_url;
                std::string playcount;
                std::string listeners;
                std::vector<std::string> topTags;
                std::string image;
            private:
                void getArtistInfo(std::string);
                void getArtistInfo(json);
        };

        struct Song {
            public:
                Song(std::string, std::string = "");

                std::string title;
                std::string artist;
                std::string album;
                std::string trackNumber;
                std::string releaseDate;
                std::string lastFM_url;
                std::string playcount;
                std::string listeners;
                std::vector<std::string> topTags;
                std::string duration;
                std::string image;
            private:
                void getSongInfo(std::string, std::string = "");
                void getSongInfo(json);
        };

        struct Album {
            public:
                Album(std::string, std::string = "");
                std::string title;
                std::string artist;

                std::string releaseDate;
                std::string lastFM_url;
                std::string playcount;
                std::string listeners;
                std::vector<std::string> topTags;
                struct track {
                    std::string title;
                    std::string duration;
                    std::string author;
                    std::string trackNumber;
                };
                std::vector<track> tracks;
                std::string image;
            private:
                void getAlbumInfo(std::string, std::string = "");
                void getAlbumInfo(json);
        };

        struct User {
            public:
                User(std::string);
                std::string name;
                std::string realname;
                std::string id;
                std::string playcount;
                std::string url;
                std::string playlists;
                std::string age;
                std::string country;
                std::string image;
            private:
                void getUserInfo(std::string);
                void getUserInfo(json);
        };
        static std::string get(std::string, std::vector<std::string>);

        static void setAPIkey();

    private:
        static std::string API_KEY;

        static std::string getRequest(std::string request);

        static size_t WriteCallback(void*, size_t, size_t, void*);

        /*
         * Returns the value of a given field, or, if the field is not found, an empty string.
         */
        static std::string getField(const json, const std::string);

        /*
         * Returns the url of a image. If the given size (i.e. "medium", "small", "large", ...) 
         * is not found, it returns the last size it finds.
         */
        static std::string getImage(const json, const std::string);
};