#include <iostream>
#include <stdio.h>
#include <string.h>

#include <curl/curl.h>
#include "include/json.hpp"
#include "include/lastfmAPI.h"
#include "include/exceptions.h"



lastfm::Artist::Artist(std::string name) {
    getArtistInfo(name);
}


void lastfm::Artist::getArtistInfo(std::string name) {
    if (name.empty()) {
        throw new IllegalArgumentException("Artist name must not be empty!");
    }
    std::vector<std::string> arguments;
    arguments.push_back("artist="+name);
    arguments.push_back("autocorrect=1");
    auto lastFMdata = json::parse(get("artist.getInfo", arguments));

    if (lastFMdata.find("error") != lastFMdata.end()) {
        std::string error = *lastFMdata.find("message");
        throw new lastFMError(error);
    }

    getArtistInfo(lastFMdata);
}

void lastfm::Artist::getArtistInfo(json lastFMdata) {

    lastFMdata = *lastFMdata.find("artist");

    title = getField(lastFMdata, "name");
    lastFM_url = getField(lastFMdata, "url");
    image = getImage(lastFMdata, "medium");

    auto stats = lastFMdata.find("stats");
    if (stats != lastFMdata.end()) {
        playcount = getField(*stats, "playcount");
        listeners = getField(*stats, "listeners");
    }

    auto tags = *lastFMdata.find("tags");
    tags = *tags.find("tag");
    for (auto& tag : tags) {
        topTags.push_back(getField(tag, "name"));
    }
}


lastfm::Song::Song(std::string name, std::string artist) {
    getSongInfo(name, artist); 
}
void lastfm::Song::getSongInfo(std::string name, std::string artist) {
    if (name.empty() || artist.empty()) {
        throw IllegalArgumentException("Song and artist names must not be empty!");
    }
    std::vector<std::string> arguments;
    arguments.push_back("track="+name);
    arguments.push_back("artist="+artist);
    arguments.push_back("autocorrect=1");

    auto lastFMdata = json::parse(get("track.getInfo", arguments));
    if (lastFMdata.find("error") != lastFMdata.end()) {
        std::string error = *lastFMdata.find("message");
        throw lastFMError(error);
    }
    getSongInfo(lastFMdata);
}

void lastfm::Song::getSongInfo(json lastFMdata) {
    lastFMdata = *lastFMdata.find("track");

    title = getField(lastFMdata, "name");
    lastFM_url = getField(lastFMdata, "url");
    duration = getField(lastFMdata, "duration");
    duration = duration.substr(0, duration.size() -3);
    listeners = getField(lastFMdata, "listeners");
    playcount = getField(lastFMdata, "playcount");

    auto jAlbum = lastFMdata.find("album");
    if (jAlbum != lastFMdata.end()) {
        artist = getField(*jAlbum, "artist");
        album = getField(*jAlbum, "title");
        image = getImage(*jAlbum, "medium");
        auto attr = (*jAlbum).find("@attr");
        if (attr != (*jAlbum).end()) {
            trackNumber = getField(*attr, "position");
        }
    }

    auto tags = *lastFMdata.find("toptags");
    tags = *tags.find("tag");
    for (auto& tag : tags) {
        topTags.push_back(getField(tag, "name"));
    }
}

lastfm::Album::Album(std::string name, std::string artist) {
    getAlbumInfo(name, artist);
}

void lastfm::Album::getAlbumInfo(std::string name, std::string artist) {
    if (name.empty() || artist.empty()) {
        throw IllegalArgumentException("Album and artist names must not be empty!");
    }

    std::vector<std::string> arguments;
    arguments.push_back("album="+name);
    arguments.push_back("autocorrect=1");
    arguments.push_back("artist="+artist);
    
    auto lastFMdata = json::parse(get("album.getInfo", arguments));
    if (lastFMdata.find("error") != lastFMdata.end()) {
        std::string error = *lastFMdata.find("message");
        throw lastFMError(error);
    }
    Album::getAlbumInfo(lastFMdata);
}

void lastfm::Album::getAlbumInfo(json lastFMdata) {
    lastFMdata = *lastFMdata.find("album");

    title = getField(lastFMdata, "name");
    artist = getField(lastFMdata, "artist");
    releaseDate = getField(lastFMdata, "releasedate");
    lastFM_url = getField(lastFMdata, "url");

    auto jTracks = *lastFMdata.find("tracks");
    jTracks = *jTracks.find("track");

    for (auto& track : jTracks) {
        track.flatten();
        struct Album::track t;
        t.title = getField(track, "name");
        auto attr = track.find("@attr");
        if (attr != track.end()) {
            t.trackNumber = getField(*attr, "rank");
        }
        //t.trackNumber = (attr != track.end()) ? *(*attr).rbegin() : "";

        t.duration = getField(track, "duration");

        tracks.push_back(t);
    }

    auto tags = *lastFMdata.find("tags");
    tags = *tags.find("tag");
    for (auto& tag : tags) {
        topTags.push_back(getField(tag, "name"));
    }

    image = getImage(lastFMdata, "medium");

    playcount = getField(lastFMdata, "playcount");
    listeners = getField(lastFMdata, "listeners");
}


lastfm::User::User(std::string name) {
    getUserInfo(name);
}

void lastfm::User::getUserInfo(std::string name) {
    if (name.empty()) {
        throw new IllegalArgumentException("User name must not be empty!");
    }

    std::vector<std::string> arguments;
    arguments.push_back("user="+name);
    auto lastFMdata = json::parse(get("user.getInfo", arguments));

    if (lastFMdata.find("error") != lastFMdata.end()) {
        std::string error = *lastFMdata.find("message");
        throw lastFMError(error);
    }

    getUserInfo(lastFMdata);
}

void lastfm::User::getUserInfo(json lastFMdata) {
    lastFMdata = *lastFMdata.find("user");

    name         = getField(lastFMdata, "name");
    realname     = getField(lastFMdata, "realname");
    age          = getField(lastFMdata, "age");
    country      = getField(lastFMdata, "country");
    playcount    = getField(lastFMdata, "playcount");
    id           = getField(lastFMdata, "id");
    url          = getField(lastFMdata, "url");
    image        = getImage(lastFMdata, "medium");
    playlists    = getField(lastFMdata, "playlists");
}

using json = nlohmann::json;
    
std::string lastfm::get(std::string method, std::vector<std::string> arguments) {
    std::string request = "http://ws.audioscrobbler.com/2.0/?method=" + method + "&";
    for(std::string arg : arguments) {
        request += arg + '&';
    }
    request += "api_key=" + API_KEY + "&format=json";

    return getRequest(request);
}

void lastfm::setAPIkey() {
    API_KEY = std::getenv("LASTFM_KEY");
}

std::string lastfm::API_KEY;

std::string lastfm::getRequest(std::string request) {
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

size_t lastfm::WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string*)userp)->append((char*) contents, size * nmemb);
    return size * nmemb;
}

/*
    * Returns the value of a given field, or, if the field is not found, an empty string.
    */
std::string lastfm::getField(const json lastFMdata, const std::string term) {
    auto field = lastFMdata.find(term);
    return (field != lastFMdata.end()) ? *field : "";
}

/*
    * Returns the url of a image. If the given size (i.e. "medium", "small", "large", ...) 
    * is not found, it returns the last size it finds.
    */
std::string lastfm::getImage(const json lastFMdata, const std::string neededSize) {
    std::string image;
    auto images = *lastFMdata.find("image");
    for (auto& i : images) {
        auto test = i.find("size");
        if (test != i.end()) {
            std::string size = *test;
            auto url = i.find("#text");
            if (size == neededSize && url != i.end()) 
            {
                image = *url;
                break;
            } else if (url != i.end()) 
            {
                image = *url;
            }
        }
    }

    return image;
}