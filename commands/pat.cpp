#include <iostream>
#include <fstream>
#include <string.h>

#include "include/command.h"
#include "include/json.hpp"
#include "sleepy_discord/sleepy_discord.h"

/*
Pat();
        SleepyDiscord::SendMessageParams execute(std::string);
        std::string description();
        std::string longDescription();
    private:
        void loadData();
        void saveData();
        */

using json = nlohmann::json;

Pat::Pat() {
    loadData();
}

SleepyDiscord::SendMessageParams Pat::execute(std::string param, SleepyDiscord::Message* message) {
    std::string user = message->author.ID;
    std::cout << "User: " << user << " Params: " << param << '\n';
    SleepyDiscord::SendMessageParams params;
    if (stringBeginsWith(param, ("<@!"+user)) || stringBeginsWith(param, ("<@"+user))) {
        params.content = std::move("\\*headpats you\\*");
        return params;
    }
    std::vector<std::string> parameters;
    if (!param.empty()) {
        parameters = parameterize(param);
        for (std::string p : parameters) {
            std::cout << "Param: " << p << '\n';
        }
    }

    // Since the second argument has to be a user, we can try to reformat the user to the correct id
    if (parameters.size() > 1) {
        std::string target = parameters.at(1);
        if (target[0] == '@') {
            if (target[1] == '!') {
                target = target.substr(2, target.size() - 2);
            } else {
                target = target.substr(1, target.size() - 1);
            }
            std::cout << "Target: " << target << '\n';
            parameters.at(1).swap(target);
        }
    }
    if (parameters.empty() || stringBeginsWith(param, "pod") || stringBeginsWith(param, "<@!702297628318236674>") || stringBeginsWith(param, "<@702297628318236674>")) {
        std::cout << "Got Patted!\n";
        pat(user, "pod");
        params.content = std::move("uwu");
    } else if (stringBeginsWith(param, "rcount")) {
        std::cout << "r!\n";
        if (parameters.size() == 1) {
            int count = getPatReceivedCount("pod");
            params.content = std::move("I have been patted "+std::to_string(count)+" times already! <:podYay:739476531449036941>");
        } else {
            int count = getPatReceivedCount(parameters.at(1));
            if (parameters.at(1)[0] != '!') {
                std::string temp = "!";
                temp += parameters.at(1);
                parameters.at(1).swap(temp);
            }
            params.content = std::move("<@"+parameters.at(1)+"> was pattet "+std::to_string(count)+ " times!");
        }
    } else if (stringBeginsWith(param, "gcount")) {
        std::cout << "g\n";
        if (parameters.size() > 1) {
            int count = getPatGivenCount(parameters.at(1));
            params.content = std::move("<@"+parameters.at(1)+"> has patted other people " + std::to_string(count)+" times!");
        } else {
            params.content = std::move("I need the of the person whose headpat count you want to know!");
        }
    } else if (parameters.at(0)[0] == '@') {
        std::string target = parameters.at(0);
        if (target.size() > 2) {
            target = target.substr(2, target.size() - 2);
            pat(user, target);
            params.content = std::move("Patted <"+parameters.at(0)+">!");
        } else {
            params.content = std::move("The user was not found :(");
        }
    } else {
        std::string error = "Sorry, I don't know what I can do for you D: \n" + longDescription();
        params.content = std::move(error);
    }

    return params;
}

json Pat::patData;

void Pat::pat(std::string source, std::string target) {
    if (patData["headpat_count"]["user"].contains(target)) {
        int receivedCount = patData["headpat_count"]["user"][target].value<int>("received", -1);
        if (receivedCount == -1) {
            std::cout << "PAT ERROR: RECEIVED COUNT == -1\n";
            return;
        }
        receivedCount++;
        patData["headpat_count"]["user"][target]["received"] = receivedCount;
    } else {
        patData["headpat_count"]["user"][target] = {{"received", 1}, {"given", 0}};
    }

    if (patData["headpat_count"]["user"].contains(source)) {
        int givenCount = patData["headpat_count"]["user"][source].value<int>("given", -1);
        if (givenCount == -1) {
            std::cout << "PAT ERROR: GIVEN COUNT == -1\n";
            return;
        }
        givenCount++;
        patData["headpat_count"]["user"][source]["given"] = givenCount;
    } else {
        patData["headpat_count"]["user"][source] = {{"received", 0}, {"given", 1}};
    }

    saveData();
}

void Pat::saveData() {
    std::string dataString = patData.dump();
    struct stat buffer;
    if (stat("PAT_DATA_BACKUP.json", &buffer) == 0) {
        if (remove("PAT_DATA_BACKUP.json") != 0) {
            std::cout << "Could not delete Backup File!\n";
        }
    }
    if (stat("PAT_DATA.json", &buffer) == 0) {
        if (rename("PAT_DATA.json", "PAT_DATA_BACKUP.json") != 0) {
            std::cout << "Error renaming old PAT_DATA!\n";
        }
    }

    std::ofstream out("PAT_DATA.json");
    out << dataString;
    out.close();
}

int Pat::getPatReceivedCount(std::string user) {
    if (patData["headpat_count"]["user"].contains(user)) {
        return patData["headpat_count"]["user"][user]["received"];
    } else {
        return 0;
    }
}

int Pat::getPatGivenCount(std::string user) {
    if (patData["headpat_count"]["user"].contains(user)) {
        return patData["headpat_count"]["user"][user]["given"];
    } else {
        return 0;
    }
}

void Pat::loadData() {
    std::string line;
    std::string jsonData;
    std::ifstream dataFile("PAT_DATA.json");
    if (dataFile.is_open()) {
        while (std::getline(dataFile, line)) {
            jsonData += line;
        }
        dataFile.close();
        patData = json::parse(jsonData);
    } else {
        std::cout << "Could not open PAT_DATA File\n";
    }
}

bool Pat::stringBeginsWith(std::string source, std::string key) {
    if (source.size() < key.size()) {
        return false;
    }
    for (size_t i = 0; i < key.size(); i++) {
        if (source[i] - key[i] != 0 && abs(source[i] - key[i]) != 32) {
            return false;
        }
    }
    return true;
}

std::string Pat::longDescription() {
    std::string out = "Headpat a user or pod!\n`Pod? pat` for patting Pod (recommended usage!)\n`Pod? pat *@User*` for patting a user\n`Pod? rcount *user (or none for Pod's count)*` for getting the number of times a user was patted\n`Pod? pat gcount *user*` for getting the number of times a user has given pats to someone!";
    return out;
}

std::string Pat::description() {
    std::string out = "Headpat a user!";
    return out;
}