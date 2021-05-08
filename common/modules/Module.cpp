#include "Module.hh"
#include "../Exceptions.cpp"

Module::Module() {}

SleepyDiscord::SendMessageParams Module::execute(std::string param, SleepyDiscord::Message*) {
    SleepyDiscord::SendMessageParams params;
    params.content = "Sorry, but this command was not found. Try `pod help` once I implemented it!\n";
    return params;
}

std::string Module::description() 
{
    return "This is a generic command description. Not that useful, eh?\nTell Estugon he missed a command description if you see him, okay?";
}

std::string Module::longDescription() {
    return "This is a generic command description, but loooooong. If you see this message, Estugon forgot to implement the long description for one (or more..) commands.\nHow rude of him, isn't it? uwu";
}

bool Module::isSecret() {
    return false;
}

std::vector<std::string> Module::parameterize(std::string rawParams) {
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

