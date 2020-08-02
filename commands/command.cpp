#include <iostream>
#include "sleepy_discord/sleepy_discord.h"

#include "include/command.h"
#include "include/exceptions.h"



Command::Command() {}
SleepyDiscord::SendMessageParams Command::execute(std::string param) 
{
    SleepyDiscord::SendMessageParams params;
    params.content = "The specified command was not found!";
    return params;
}

std::string Command::description() 
{
    return "This is a generic command description. Not that useful, eh?\nTell Estugon he missed a command description if you see him, okay?";
}

std::string Command::longDescription() {
    return "This is a generic command description, but loooooong. If you see this message, Estugon forgot to implement the long description for one (or more..) commands.\nHow rude of him, isn't it? uwu";
}

bool Command::isSecret() {
    return false;
}

std::vector<std::string> Command::parameterize(std::string rawParams) {
    if (rawParams.empty()) {
        throw IllegalArgumentException("No arguments given when they were needed!");
    }
    std::vector<std::string> params;
    std::string temp = "";
    bool inString = false;

    for (char c : rawParams) {
        if (inString) {
            if (c == '>') {
                inString = false;
                if (!temp.empty()) {
                    params.push_back(temp);
                    temp.clear();
                }
            } else {
                temp += c;
            }
        } else {
            if (c == '<') {
                inString = true;
                temp.clear();
            } else {
                if (c == ' ') {
                    if (!temp.empty()) {
                        params.push_back(temp);
                        temp.clear();
                    }
                } else {
                    temp += c;
                }
            }
        }
    }
    if(!temp.empty()) {
        params.push_back(temp);
    }

    return params;
}