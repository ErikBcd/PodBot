#include <iostream>
#include <regex>
#include <string.h>

#include "include/command.h"
#include "sleepy_discord/sleepy_discord.h"

// Takes a text and owofies it. Example: ´Pod? owo *usermessage*´ 

OwOfier::OwOfier() {}



std::string owofy(std::string input) {
    using namespace std;

    regex w("(?:r|l)");
    string output = input;
    
    output = regex_replace (input, w, "w");


    regex upperCaseW("(?:R|L)");
    output = regex_replace (output, upperCaseW, "W");


    regex ny("n([aeiou])");
    regex upperCaseNy("N([aeiou])");
    regex upperCaseNY("N([AEIOU])");
    regex ove("(ove)");
    regex exclamation("\!");
    regex question("\\?");
    regex point("\\.");

    output = regex_replace (output, ny, "ny$1");
    output = regex_replace (output, upperCaseNy, "Ny$1");
    output = regex_replace (output, upperCaseNY, "NY$1");
    output = regex_replace(output, ove, "uv");
    output = regex_replace(output, exclamation, "$& >w<");
    output = regex_replace(output, question, "$& UwU");
    output = regex_replace(output, point, "$& OwO");

    return output;
}

SleepyDiscord::SendMessageParams execute(std::string param) 
{
    SleepyDiscord::SendMessageParams params;
    params.content = owofy(param);
    return params;
}

std::string description() {
    return "Usage: 'Pod? owo *your text here*'\nConvert your text in a more glorious form!";
}
