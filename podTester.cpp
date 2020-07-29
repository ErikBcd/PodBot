#include <stdio.h>
#include <string.h>
#include <iostream>
#include <map>
#include <vector>

#include "commands/include/myAnimeList.h"
#include "commands/include/lastfmAPI.h"

using namespace std;

int main() {

    try
    {
        lastfm::Album album = lastfm::Album("Believe", "Cher");
        cout << "Album = " << album.title << "\nArtist = " << album.artist << "\nRelease Date = " << album.releaseDate << "\nPlaycount = " << album.playcount << '\n';
        for (lastfm::Album::track track : album.tracks) {
            cout << "-----------------------------------------------\nTrack Title: " << track.title << "\nDuration: " << track.duration << "\nTrack Number: " << track.trackNumber << endl;
        }
        cout << "-----------------------------------------------\n";
        for (string tag : album.topTags) {
            cout << "Tag: " << tag << '\n';
        }
        cout << "Image: " << album.image << '\n';
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
    }

    try
    {
        lastfm::Song s = lastfm::Song("Sky and Sand", "Paul Kalkbrenner");
        cout << "Name = " << s.title
             << "\nAlbum = " << s.album
             << "\nArtist = " << s.artist
             << "\nTrack Number = " << s.trackNumber
             << "\nDuration = " << s.duration
             << "\nURL = " << s.lastFM_url
             << "\nImage = " << s.image 
             << "\nPlaycount = " << s.playcount
             << "\nListeners = " << s.listeners << endl;
        for (string tag : s.topTags) {
            cout << "Tag: " << tag << '\n';
        }

    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    

    try
    {
        lastfm::Artist a = lastfm::Artist("Cher");
        cout << "Name = " << a.title
             << "\nURL = " << a.lastFM_url
             << "\nImage = " << a.image 
             << "\nPlaycount = " << a.playcount
             << "\nListeners = " << a.listeners << endl;
        for (string tag : a.topTags) {
            cout << "Tag: " << tag << '\n';
        }
        

    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    try
    {
        lastfm::User a = lastfm::User("Es_tu_the_gon");
        cout << "Name = " << a.name
             << "\nReal Name = " << a.realname
             << "\nURL = " << a.url
             << "\nImage = " << a.image 
             << "\nPlaycount = " << a.playcount
             << "\nAge = " << a.age 
             << "\nCountry = " << a.country
             << "\nID = " << a.id << endl;
        

    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}