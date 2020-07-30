#include <iostream>
#include "sleepy_discord/sleepy_discord.h"

#include "include/command.h"



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