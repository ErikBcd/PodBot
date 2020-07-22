#ifndef COMMANDS_H
#define COMMANDS_H


#include <iostream>
#include "sleepy_discord/sleepy_discord.h"
#include "myAnimeList.h"

class Command {
    public:
        Command();
        virtual SleepyDiscord::SendMessageParams execute(std::string);
        virtual std::string description();
};

class OwOfier : public Command {
public:
    OwOfier();
    SleepyDiscord::SendMessageParams execute(std::string);
    std::string description();
private:
    std::string owofy(std::string);
};

class MyAnimeListCommands : public Command {
    public:
        MyAnimeListCommands();
        SleepyDiscord::SendMessageParams execute(std::string);
        SleepyDiscord::Embed createEmbed(Anime);
        std::string description();
};

#endif // !COMMANDS_H