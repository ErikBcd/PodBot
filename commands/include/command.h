#ifndef COMMANDS_H
#define COMMANDS_H


#include <iostream>
#include "sleepy_discord/sleepy_discord.h"
#include "myAnimeList.h"
#include "lastfmAPI.h"

class Command {
    public:
        Command();
        virtual SleepyDiscord::SendMessageParams execute(std::string);
        virtual std::string description();
        virtual std::string longDescription();
};

class Help : public Command {
    public:
        Help(std::map<std::string, Command*>);
        SleepyDiscord::SendMessageParams execute(std::string);
    private:
        static std::map<std::string, Command*> availableCommands;
};

class OwOfier : public Command {
public:
    OwOfier();
    SleepyDiscord::SendMessageParams execute(std::string);
    std::string description();
    std::string longDescription();
private:
    std::string owofy(std::string);
};

class MyAnimeListCommands : public Command {
    public:
        MyAnimeListCommands();
        SleepyDiscord::SendMessageParams execute(std::string);
        SleepyDiscord::Embed createEmbed(Anime);
        std::string description();
        std::string longDescription();
};

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
        std::vector<std::string> parseParams(std::string);
};

#endif // !COMMANDS_H