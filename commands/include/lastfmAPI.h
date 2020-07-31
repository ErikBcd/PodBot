#pragma once

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <vector>
#include <cstdlib>

#include <curl/curl.h>
#include "json.hpp"

/**
 * A class for retrieving and processing data
 * from lastfm.
 * 
 * Needs an API Key in the enviroment variables! Has to be named `LASTFM_KEY`!
 */
class lastfm {
    using json = nlohmann::json;
    
    public:
        /**
         * Various information about an artist.
         */
        struct Artist {
            public:
                /**
                 * Takes a string containing the artist as 
                 * argument and retrieves all information it gets about that artist.
                 * @param artist: The name of the artist.
                 */
                Artist(std::string);

                std::string title;
                std::string lastFM_url;
                std::string playcount;
                std::string listeners;
                std::vector<std::string> topTags;
                std::string image;
            private:
                /**
                 * Collects all wanted infos about the artist from lastfm.
                 */
                void getArtistInfo(std::string);
                void getArtistInfo(json);
        };

        /**
         * Various information about a song.
         */
        struct Song {
            public:
                /**
                 * Takes a string containing the song as 
                 * argument and retrieves all information it gets about that song.
                 * The second string carries the artist name, but it's optional.
                 * 
                 * @param songname: The name of the song.
                 * @param artistname: The name of the artist.
                 */
                Song(std::string, std::string);

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
                /**
                 * Collects all wanted infos about the artist from lastfm.
                 */
                void getSongInfo(std::string, std::string);
                void getSongInfo(json);
        };

        struct Album {
            public:
                Album(std::string, std::string);
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
                void getAlbumInfo(std::string, std::string);
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