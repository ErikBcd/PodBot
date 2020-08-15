#include <iostream>
#include <string.h>

#include "include/command.h"
#include "sleepy_discord/sleepy_discord.h"

Help::Help(std::map<std::string, Command*> commands) {
    Help::availableCommands = commands;
}

bool Help::isSecret() {
    return true;
}

SleepyDiscord::SendMessageParams Help::execute(std::string command, SleepyDiscord::Message* message) {
    SleepyDiscord::Embed embed;
    if (availableCommands.find(command) != availableCommands.end()) {
        Command* c = availableCommands[command];
        embed.title = std::move(command);
        std::string desc = c->longDescription();
        embed.description = std::move(desc);
        embed.author.iconUrl = std::move("https://cdn.discordapp.com/avatars/702297628318236674/19fac1c410c614d0e0745a44c3321d46.png?size=256");
        embed.author.name = std::move("Estugon");
    } else if (command.empty()) {
        embed.title = std::move("Pod Info");
        embed.description = std::move("This is a little side project I am working on, so this bot is by no means perfect.\nHowever, do you know what is perfect? That profile picture, made by Niatsuna! <:podYay:739476531449036941>\nTip: Instead of \"Pod?\", you can also use \"[\"!");
        embed.author.iconUrl = std::move("https://cdn.discordapp.com/avatars/702297628318236674/19fac1c410c614d0e0745a44c3321d46.png?size=256");
        embed.author.name = std::move("Estugon");
        for (std::map<std::string, Command*>::iterator it = availableCommands.begin(); it != availableCommands.end(); ++it) {
            if (it->first != "generic" && !it->second->isSecret()) {
                SleepyDiscord::EmbedField field;
                field.name = std::move(it->first);
                field.value = std::move(it->second->description());
                embed.fields.push_back(field);
            }
        }
    } else {
        embed.title = std::move("Pod Info");
        embed.description = std::move("Your specified command was not found uwu These are the commands I know! \nTip: Instead of \"Pod?\", you can also use \"[\"!");
        embed.author.iconUrl = std::move("https://cdn.discordapp.com/avatars/702297628318236674/19fac1c410c614d0e0745a44c3321d46.png?size=256");
        embed.author.name = std::move("Estugon");
        for (std::map<std::string, Command*>::iterator it = availableCommands.begin(); it != availableCommands.end(); ++it) {
            if (it->first != "generic" && !it->second->isSecret()) {
                SleepyDiscord::EmbedField field;
                field.name = std::move(it->first);
                field.value = std::move(it->second->description());
                embed.fields.push_back(field);
            }
        }
    }

    SleepyDiscord::SendMessageParams params;
    params.embed = embed;
    return params;
}

std::map<std::string, Command*> Help::availableCommands;