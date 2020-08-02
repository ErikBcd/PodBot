#ifndef COMMANDS_H
#define COMMANDS_H


#include <iostream>
#include "sleepy_discord/sleepy_discord.h"
#include "myAnimeList.h"
#include "lastfmAPI.h"

/**
 * A generic command.
 */
class Command {
    public:
        Command();
        virtual SleepyDiscord::SendMessageParams execute(std::string);
        virtual std::string description();
        virtual std::string longDescription();
        virtual std::vector<std::string> parameterize(std::string);
        virtual bool isSecret();
};

/**
 * Helpflul help command. 
 * Will return a message with either a general overview over every possible command,
 * or a help message for a specified command.
 */
class Help : public Command {
    public:
        Help(std::map<std::string, Command*>);
        SleepyDiscord::SendMessageParams execute(std::string);
        bool isSecret();
    private:
        static std::map<std::string, Command*> availableCommands;
        
};

/**
 * A command for owofieing strings.
 */
class OwOfier : public Command {
public:
    OwOfier();
    SleepyDiscord::SendMessageParams execute(std::string);
    std::string description();
    std::string longDescription();
private:
    std::string owofy(std::string);
};

/**
 * Commands for interacting with the MyAnimeList Database.
 * If successfull, returns a message with information about 
 * an anime/a mange/whatever the database has to offer.
 */
class MyAnimeListCommands : public Command {
    public:
        MyAnimeListCommands();
        SleepyDiscord::SendMessageParams execute(std::string);
        SleepyDiscord::Embed createEmbed(Anime);
        std::string description();
        std::string longDescription();
};

/**
 * Commands for interacting with the LastFM Database.
 * If successfull, returns a message with information about 
 * a song/an album/whatever the database has to offer.
 */
class LastFMCommand : public Command {
    public:
        LastFMCommand();
        SleepyDiscord::SendMessageParams execute(std::string);
        std::string description();
        std::string longDescription();
    private:
        SleepyDiscord::Embed createEmbed(lastfm::Song);
        SleepyDiscord::Embed createEmbed(lastfm::Artist);
        SleepyDiscord::Embed createEmbed(lastfm::Album);
        SleepyDiscord::Embed createEmbed(lastfm::User);
};

class Kohaku : public Command {
    public:
        Kohaku();
        SleepyDiscord::SendMessageParams execute(std::string);
        bool isSecret();
        virtual std::string longDescription();
};

#endif // !COMMANDS_H