#include <iostream>
#include <stdio.h>
#include <regex>

#include "include/exceptions.h"
#include "include/command.h"
#include "include/lastfmAPI.h"
#include "sleepy_discord/sleepy_discord.h"

LastFMCommand::LastFMCommand() {}

/*
 * LastFM Command Pattern: 
 * Pod? lastfm artist "name" -> Artist
 * Pod? lastfm album "name" (-a "artist name") -> Album
 * Pod? lastfm song "name" (-a "artist name") -> Song
 * Pod? lastfm user "name" -> User
 */
SleepyDiscord::SendMessageParams LastFMCommand::execute(std::string args, SleepyDiscord::Message*) {
    std::string command;
    std::string param;
    std::map<std::string, int> commands;

    commands["user"] = 0;
    commands["artist"] = 1;
    commands["album"] = 2;
    commands["song"] = 3;

    size_t space_pos = args.find(" ");
    if (space_pos != std::string::npos) {
        command = args.substr(0, space_pos);
        param = args.substr(command.size() + 1, 
                args.size() - command.size());
    } else {
        command = args;
    }

    SleepyDiscord::Embed embed;
    std::string search;
    std::vector<std::string> lastFMparams;
    try
    {
        lastFMparams = parameterize(param);
    }
    catch(const std::exception& e)
    {
        SleepyDiscord::SendMessageParams params;
        params.content = std::move("Error: "+((std::string) e.what()));
        return params;
    }
    
    if (!lastFMparams.empty()) {
        search = lastFMparams.front();
        lastFMparams.erase(lastFMparams.begin());
    }

    if (commands.find(command) != commands.end()) {
        try
        {
            switch (commands[command]) {
                case 0:
                    embed = createEmbed(lastfm::User(search));
                    break;
                case 1:
                    embed = createEmbed(lastfm::Artist(search));          
                    break;
                case 2:
                    if (lastFMparams.empty()) {
                        throw IllegalArgumentException("Usage: Pod? lastfm album <album> <artist>");
                    }
                    embed = createEmbed(lastfm::Album(search, lastFMparams.front()));
                    break;
                case 3:
                    if (lastFMparams.empty()) {
                        throw IllegalArgumentException("Usage: Pod? lastfm song <song> <artist>");
                    }
                    embed = createEmbed(lastfm::Song(search, lastFMparams.front()));
                    break;
        }
        }
        catch(const std::exception& e)
        {
            SleepyDiscord::SendMessageParams params;
            params.content = std::move("Error <:podAngry:739476530727616552> "+((std::string) e.what()));
            return params;
        }
        
        
    } else {
	    SleepyDiscord::SendMessageParams params;
	    params.content = std::move("Error <:podAngry:739476530727616552> The command was not found!");
	    return params;
    }

    SleepyDiscord::SendMessageParams params;
    params.embed = embed;
    return params;

}

std::string LastFMCommand::description() {
    return "Get some info from the last.fm database!";
};

SleepyDiscord::Embed LastFMCommand::createEmbed(lastfm::Song song) {
    SleepyDiscord::Embed output;

    if (song.title.empty()) {
        output.title = std::move("Song was not found!");
        return output;
    }
    output.title = std::move(song.title);
    output.author.name = (!song.artist.empty()) ? std::move(song.artist) : std::move("Artist not found. UwU");
    if (!song.lastFM_url.empty()) {
        output.url = std::move(song.lastFM_url);
    }
    
    if (!song.album.empty()) {
        SleepyDiscord::EmbedField album;
        album.name = std::move(song.album);
        int duration = std::stoi(song.duration);
        int minutes = duration / 60;
        int seconds = duration % 60;
        album.value = std::move("Track Number: "+song.trackNumber
                                +"\nDuration: "+std::to_string(minutes)+":"+std::to_string(seconds));
        output.fields.push_back(album);
    }

    SleepyDiscord::EmbedField stats;
    stats.name = std::move("lastFM Stats");
    stats.value = std::move("Playcount: "+song.playcount+
                            "\nListeners: "+song.listeners);
    output.fields.push_back(stats);

    SleepyDiscord::EmbedField tags;
    tags.name = std::move("Top Tags: ");
    std::string tagVal = "";
    for (std::string tag : song.topTags) {
        tagVal += tag+"\n";
    }
    if (!tagVal.empty()) {
        tags.value = std::move(tagVal);
        output.fields.push_back(tags);
    }
    
    if (!song.image.empty()) {
        output.image.url = std::move(song.image);
    }
    return output;
}

SleepyDiscord::Embed LastFMCommand::createEmbed(lastfm::Artist artist) {
    SleepyDiscord::Embed output;

    if (artist.title.empty()) {
        output.title = std::move("Artist not found!");
        output.description = std::move("Either the artist is not in the last.fm database you misspelled them.");
        return output;
    }

    output.title = std::move(artist.title);
    output.url = std::move(artist.lastFM_url);
    SleepyDiscord::EmbedField stats;
    stats.name = std::move("lastFM Stats");
    stats.value = std::move("Playcount: "+artist.playcount+"\nListeners: "+artist.listeners);

    output.fields.push_back(stats);
    SleepyDiscord::EmbedField topTags;
    std::string tags;
    topTags.name = std::move("Top Tags");
    for (std::string t : artist.topTags) {
        tags += t+'\n';
    }
    if (!tags.empty()) {
        topTags.value = std::move(tags);
        output.fields.push_back(topTags);
    }

    if (!artist.image.empty()) {
        output.image.url = std::move(artist.image);
        output.image.height = std::move(400);
        output.image.width = std::move(400);
    }
    return output;
}
SleepyDiscord::Embed LastFMCommand::createEmbed(lastfm::Album album){
    SleepyDiscord::Embed output;
    output.title = std::move(album.title);
    output.author.name = (!album.artist.empty()) ? std::move(album.artist) : "Artist not found!";
    output.url = std::move(album.lastFM_url);

    output.description = std::move("Released: "+album.releaseDate);
    
    SleepyDiscord::EmbedField tracks;
    tracks.name = std::move("Songs");
    std::string trackVal = "";
    for (lastfm::Album::track t : album.tracks) {
        std::string track;
        if (t.trackNumber.size() == 1) {
            t.trackNumber = '0'+t.trackNumber;
        }
        int duration = std::stoi(t.duration);
        int min = duration / 60;
        int sec = duration % 60;
        std::string dur = std::to_string(min) + ":" + std::to_string(sec);
        if (min < 10) {
            dur = '0'+dur;
        }
        if (sec < 10) {
            dur += '0';
        }
        track += t.trackNumber  + " (" + dur +")" + " | " + t.title + '\n';
        trackVal += track;
    }
    if (trackVal.size() >= 1024) {
        std::string endText = "...\nSee the other tracks on last.fm\n";
        trackVal = trackVal.substr(0, (1024 - endText.size())) + endText;
    }

    tracks.value = std::move(trackVal);
    if (!tracks.value.empty()) {
        output.fields.push_back(tracks);
    }

    SleepyDiscord::EmbedField topTags;
    
    std::string tags;
    topTags.name = std::move("Top Tags");
    for (std::string t : album.topTags) {
        tags += t+'\n';
    }
    if (!tags.empty()) {
        topTags.value = std::move(tags);
        output.fields.push_back(topTags);
    }

    SleepyDiscord::EmbedField stats;
    stats.name = std::move("lastFM Stats");
    stats.value = std::move("Playcount: "+album.playcount+"\nListeners: "+album.listeners);

    output.fields.push_back(stats);

    if (!album.image.empty()) {
        output.image.url = std::move(album.image);
    }

    return output;
}
SleepyDiscord::Embed LastFMCommand::createEmbed(lastfm::User user){
    SleepyDiscord::Embed output;
    output.title = std::move(user.name);
    output.url = std::move(user.url);
    output.description = std::move(user.realname + " " + user.id);

    output.thumbnail.url = std::move(user.image);

    SleepyDiscord::EmbedField userInfo;
    userInfo.name = std::move("User Info");
    userInfo.value = std::move("Age: " + user.age + "\nCountry: "+user.country
                                + "\nPlaycount: "+user.playcount+"\nPlaylists: "+user.playlists);

    output.fields.push_back(userInfo);

    return output;
}

std::string LastFMCommand::longDescription() {
    std::string help = "Search the last.fm database for music or users!\n";
    help            += "Usage: `Pod? lastfm ` + \n";
    help            += "`artist <artist name here>` - This will give you some information about the artist.\n";
    help            += "`album <album name here> <artist name here (optional, but recommended)>` - This gives some information about the specified album.\n";
    help            += "`song <song name here> <artist name here (optional, but recommended)>` - Some information about the song.\n";
    help            += "`user <username here>` - Some information about the user.\n";
    help            += "\nThis command is still very early in development. So the functionality is *very* basic uwu";
    return help;
}
