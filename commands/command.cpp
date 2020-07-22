#include <iostream>
#include "sleepy_discord/sleepy_discord.h"

#include "include/command.h"



Command::Command() {}
SleepyDiscord::SendMessageParams execute(std::string param) 
{
    SleepyDiscord::SendMessageParams params;
    params.content = "The specified command was not found!";
    return params;
}

std::string description() 
{
    return "This is a generic command description. Not that useful, eh?\nTell Estugon he missed a command description if you see him, okay?";
}