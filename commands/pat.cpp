#include <iostream>
#include <fstream>
#include <string.h>

#include "include/command.h"
#include "include/json.hpp"
#include "sleepy_discord/sleepy_discord.h"

using json = nlohmann::json;

Pat::Pat() {
    loadData();
}

SleepyDiscord::SendMessageParams Pat::execute(std::string param, SleepyDiscord::Message* message) {
    std::vector<SleepyDiscord::User> mentionedUsers = message->mentions;
    std::string user = message->author.ID;
    SleepyDiscord::SendMessageParams params;
    if (stringBeginsWith(param, ("<@!"+user)) || stringBeginsWith(param, ("<@"+user))) {
        params.content = std::move("\\*headpats you\\*");
        return params;
    }

    // Parameterize the input
    std::vector<std::string> parameters;
    if (!param.empty()) {
        parameters = parameterize(param);
        for (std::string p : parameters) {
            std::cout << "Param: " << p << '\n';
        }
    }

    // Determine what the user wants!
    if (parameters.empty() || stringBeginsWith(param, "pod") || stringBeginsWith(param, "<@!702297628318236674>") || stringBeginsWith(param, "<@702297628318236674>")) {
        // Patting Pod himself
        pat(user, "pod");
        params.content = std::move("uwu");
    } else if (parameters.at(0) == "gcount") {
        // Access the number of times a user headpatted someone else
        if (mentionedUsers.empty() || mentionedUsers.front().ID.string() == "702297628318236674") {
            // If no argument is given, return the total count of times people have been headpatted
            int count = getPatGivenCount("pod");
            int fullCount = count + getPatReceivedCount("pod");
            params.content = std::move("Overall, I gave **"+std::to_string(count)+"** headpats to the people! <:podYay:739476531449036941>\n(Including the headpats I received, **"+std::to_string(fullCount)+"** heads have been patted!!)");
        } else if (!mentionedUsers.empty()) {
            int count = getPatGivenCount(mentionedUsers.front().ID);
            params.content = std::move(("<"+mentionedUsers.front().username+"> has given **"+std::to_string(count)+"** headpats to other people!"));
        }
    } else if (parameters.at(0) == "rcount") {
        // Access the number of times a user has been headpatted
        if (mentionedUsers.empty() || mentionedUsers.front().ID.string() == "702297628318236674") {
            int count = getPatReceivedCount("pod");
            params.content = std::move("I have been patted **"+std::to_string(count)+"** times already! <:podYay:739476531449036941>");
        } else if (!mentionedUsers.empty()) {
            int count = getPatReceivedCount(mentionedUsers.front().ID);
            params.content = std::move("<"+mentionedUsers.front().username+"> has been patted **"+std::to_string(count)+"** times!");
        }
    } else if (!mentionedUsers.empty()) {
        // Headpat one or more users
        std::string answer = (mentionedUsers.size() > 1) ? "Multi-Headpat!\nPatted " : "Patted ";
        for (size_t i = 0; i < mentionedUsers.size(); i++) {
            answer += mentionedUsers.at(i).username;
            answer += (i == mentionedUsers.size() - 1) ? "" : ", ";
            pat(user, mentionedUsers.at(i).ID);
        }
        answer += "!";
        params.content = std::move(answer);
    } else {
        // If you are here, nothing of meaning has been written
        params.content = std::move("I don't really know what "+parameters.at(0)+" means, maybe it was a typo? Try one of these instead:\n"+longDescription());
    }

    return params;
}

json Pat::patData;

void Pat::pat(std::string source, std::string target) {

    // Look if the user exist in the json, and then either create the user or update the users values
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

    // For some overall stats: The accumulated given pats are saved in pod's "given" entry.
    if (patData["headpat_count"]["user"].contains("pod") && target != "pod") {
        int givenCount = patData["headpat_count"]["user"]["pod"].value<int>("given", -1);
        if (givenCount == -1) {
            std::cout << "PAT ERROR: GIVEN COUNT == -1\n";
            return;
        }
        givenCount++;
        patData["headpat_count"]["user"]["pod"]["given"] = givenCount;
    } else if (target != "pod") {
        patData["headpat_count"]["user"]["pod"] = {{"received", 0}, {"given", 1}};
    }

    saveData();
}


void Pat::saveData() {
    std::string dataString = patData.dump();
    struct stat buffer;
    // At first. delete the old backup...
    if (stat("PAT_DATA_BACKUP.json", &buffer) == 0) {
        if (remove("PAT_DATA_BACKUP.json") != 0) {
            std::cout << "Could not delete Backup File!\n";
        }
    }

    // ..then rename the old database (so it is the new backup)...
    if (stat("PAT_DATA.json", &buffer) == 0) {
        if (rename("PAT_DATA.json", "PAT_DATA_BACKUP.json") != 0) {
            std::cout << "Error renaming old PAT_DATA!\n";
        }
    }

    // ...and after that, write the new json file to disk.
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