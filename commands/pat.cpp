#include <iostream>
#include <fstream>
#include <string.h>
#include <map>
#include <bits/stdc++.h>

#include "include/command.h"
#include "include/json.hpp"
#include "sleepy_discord/sleepy_discord.h"

using json = nlohmann::json;

Pat::Pat(SleepyDiscord::DiscordClient *discordClient)
{
    client = discordClient;
    loadData();
}

SleepyDiscord::DiscordClient *Pat::client;
bool Pat::mayRefreshCache = true;

SleepyDiscord::SendMessageParams Pat::execute(std::string param, SleepyDiscord::Message *message) {
    std::vector<SleepyDiscord::User> mentionedUsers = message->mentions;
    std::string user = message->author.ID;
    SleepyDiscord::SendMessageParams params;
    if (mentionedUsers.size() > 1) {
        params.content = std::move("You may only ping one person at a time! <:podPing:739476530727747656>");
        return params;
    }
    if (stringBeginsWith(param, ("<@!" + user)) || stringBeginsWith(param, ("<@" + user))) {
        params.content = std::move("\\*headpats you\\*");
        return params;
    }

    std::string command;
    std::vector<std::pair<std::string, std::string>> users;

    // At first, remove the mention from the param string, if there is one
    if (!mentionedUsers.empty()) {
        size_t found = param.find(("<@" + mentionedUsers.at(0).ID + ">"));
        if (found != std::string::npos)
        {
            param.erase(found, found + 21);
        }
        else
        {
            found = param.find(("<@!" + mentionedUsers.at(0).ID + ">"));
            if (found != std::string::npos)
            {
                param.erase(found, found + 22);
            }
            else
            {
                std::cout << "The pinged user was not found in the params D:\n";
                params.content = std::move("There was a little error, I am very sorry! Tell <@!157491513054461953> that he should check my logs.");
                return params;
            }
        }
        // Now add the mentioned user to the usernames and userIDs vectors
        if (mentionedUsers.front().ID.string() == "702297628318236674")
        {
            users.push_back(std::make_pair("pod", "pod"));
        }
        else
        {
            users.push_back(std::make_pair(mentionedUsers.front().username, mentionedUsers.front().ID.string()));
        }
    }

    // Parameterize the input
    // That means finding possible commands and extracting all mentioned users into a map

    // Find a command, if there is one
    command = getCommand(param);
    // Remove the command from the params string
    if (!command.empty()) {
        param = param.substr(6, param.size() - 6);
    }
    // If there are any other users to pat then the ones directly mentioned, add them to the user table
    if ((param.empty() && users.empty()) || !param.empty()) {
        getUsers(param, message->serverID, &users);
    }

    if (command.empty()) {
        params.content = std::move(executePat(user, &users));
    }
    else {
        params.content = std::move(executeCommand(command, users.front()));
        if (params.content.empty()) {
            params.content = std::move("Something just went horribly wrong D:");
        }
    }

    return params;
}

json Pat::patData;
std::map<std::string, json> Pat::membersOnServer;

std::string Pat::getCommand(std::string param) {
    if (!param.empty()) {
        if (stringBeginsWith(param, "gcount")) {
            return "gcount";
        } else if (stringBeginsWith(param, "rcount")) {
            return "rcount";
        }
    }

    return "";
}

void Pat::getUsers(std::string param, std::string serverID, std::vector<std::pair<std::string, std::string>>* users) {
    if (!param.empty()) {
        std::vector<std::string> params = parameterize(param);

        for (std::string username : params) {
            if (username == "pod") {
                users->push_back(std::make_pair("pod", "pod"));
            } else {
                findUserID(username, serverID, users);
            }
        }
    } else {
        users->push_back(std::make_pair("pod", "pod"));
    }
}

std::string Pat::executePat(std::string user, std::vector<std::pair<std::string, std::string>>* users) {
    std::string pattedUsers;
    std::string selfPatProtection;
    std::string output;
    std::unordered_set<std::string> pattedUserSet;
    for (size_t i = 0; i < users->size(); i++) {
        std::pair<std::string, std::string> u = users->at(i);
        if (pattedUserSet.find(u.second) == pattedUserSet.end()) {
            if (u.first == "pod" || u.second == "702297628318236674") {
                pat(user, "pod");
                output += "Thank you <a:podPat:762750253086015579>\n";
                pattedUserSet.insert("702297628318236674");
                pattedUserSet.insert("pod");
                pattedUserSet.insert("Pod");
            } else {
                if (u.second == user) {
                    selfPatProtection += (", " + u.first);
                }
                else {
                    pat(user, u.second);
                    pattedUsers += (i == users->size()-1) ? u.first : u.first + ", ";
                    pattedUserSet.insert(u.second);
                }
            }
        }
    }

    if (pattedUsers[pattedUsers.size() - 2] == ',' && pattedUsers[pattedUsers.size() - 1] == ' ') {
        pattedUsers = pattedUsers.substr(0, pattedUsers.size() - 2);
    }

    output += (!pattedUsers.empty()) ? "Patted " + pattedUsers + '!' : "";
    output += (!selfPatProtection.empty()) ? "\nShouldn't you know better than patting yourself" + selfPatProtection + "?" : "";

    return (!output.empty()) ? output : "No one could be patted D: Maybe check your spelling? If nothing helps, try `[help pat`";
}

std::string Pat::executeCommand(std::string command, std::pair<std::string, std::string> user) {
    std::string output;
    if (command == "gcount") {
        if (user.second == "pod" || user.second == "702297628318236674") {
            // If no argument is given, return the total count of times people have been headpatted
            int count = getPatGivenCount("pod");
            int fullCount = count + getPatReceivedCount("pod");
            output = "Overall, I gave **" + std::to_string(count) + "** headpats to the people! <:podYay:739476531449036941>\n(Including the headpats I received, **" + std::to_string(fullCount) + "** heads have been patted!)";
        }
        else {
            int count = getPatGivenCount(user.second);
            output = user.first + " has patted other people **" + std::to_string(count) + "** times!";
        }
    } else {
        if (user.second == "pod" || user.second == "702297628318236674") {
            // If no argument is given, return the total count of times people have been headpatted
            int count = getPatReceivedCount("pod");
            output = "I have been patted **" + std::to_string(count) + "** times already! <:podYay:739476531449036941>";
        }
        else {
            int count = getPatReceivedCount(user.second);
            output = user.first + " has been patted **" + std::to_string(count) + "** times!";
        }
    }

    return output;
}

void Pat::pat(std::string source, std::string target)
{

    // Look if the user exist in the json, and then either create the user or update the users values
    if (patData["headpat_count"]["user"].contains(target))
    {
        int receivedCount = patData["headpat_count"]["user"][target].value<int>("received", -1);
        if (receivedCount == -1)
        {
            std::cout << "PAT ERROR: RECEIVED COUNT == -1\n";
            return;
        }
        receivedCount++;
        patData["headpat_count"]["user"][target]["received"] = receivedCount;
    }
    else
    {
        patData["headpat_count"]["user"][target] = {{"received", 1}, {"given", 0}};
    }

    if (patData["headpat_count"]["user"].contains(source))
    {
        int givenCount = patData["headpat_count"]["user"][source].value<int>("given", -1);
        if (givenCount == -1)
        {
            std::cout << "PAT ERROR: GIVEN COUNT == -1\n";
            return;
        }
        givenCount++;
        patData["headpat_count"]["user"][source]["given"] = givenCount;
    }
    else
    {
        patData["headpat_count"]["user"][source] = {{"received", 0}, {"given", 1}};
    }

    // For some overall stats: The accumulated given pats are saved in pod's "given" entry.
    if (patData["headpat_count"]["user"].contains("pod") && target != "pod")
    {
        int givenCount = patData["headpat_count"]["user"]["pod"].value<int>("given", -1);
        if (givenCount == -1)
        {
            std::cout << "PAT ERROR: GIVEN COUNT == -1\n";
            return;
        }
        givenCount++;
        patData["headpat_count"]["user"]["pod"]["given"] = givenCount;
    }
    else if (target != "pod")
    {
        patData["headpat_count"]["user"]["pod"] = {{"received", 0}, {"given", 1}};
    }

    saveData();
}

void Pat::saveData()
{
    std::string dataString = patData.dump();
    struct stat buffer;
    // At first. delete the old backup...
    if (stat("PAT_DATA_BACKUP.json", &buffer) == 0)
    {
        if (remove("PAT_DATA_BACKUP.json") != 0)
        {
            std::cout << "Could not delete Backup File!\n";
        }
    }

    // ..then rename the old database (so it is the new backup)...
    if (stat("PAT_DATA.json", &buffer) == 0)
    {
        if (rename("PAT_DATA.json", "PAT_DATA_BACKUP.json") != 0)
        {
            std::cout << "Error renaming old PAT_DATA!\n";
        }
    }

    // ...and after that, write the new json file to disk.
    std::ofstream out("PAT_DATA.json");
    out << dataString;
    out.close();
}

bool Pat::findUserID(std::string username, std::string serverID, std::vector<std::pair<std::string, std::string>>* users) {
    // Try to find the user in the map structure first
    
    if (membersOnServer.find(serverID) != membersOnServer.end()) {
        for (json& item : membersOnServer[serverID]["members"]) {
            if (stringBeginsWith(item["nick"], username) || stringBeginsWith(item["username"], username)) {
                std::cout << "Got ID from cache!\n";
                users->push_back(std::make_pair(item["username"], item["id"]));
                return true;
            }
        }
    }

    if (mayRefreshCache) {
        std::cout << "Building new Server cache!\n";
        json newServerCache;
        std::list<SleepyDiscord::ServerMember> members;
        try
        {
            members = client->listMembers(serverID, 1000);
        }
        catch (SleepyDiscord::ErrorCode e)
        {
            std::cout << "Error in findUserID: " << e << '\n';
            return false;
        }

        std::string userID;
        int ctr = 0;

        for (SleepyDiscord::ServerMember member : members)
        {
            newServerCache["members"][("user"+ std::to_string(ctr))]["nick"] = member.nick;
            newServerCache["members"][("user"+ std::to_string(ctr))]["username"] = member.user.username;
            newServerCache["members"][("user"+ std::to_string(ctr))]["id"] = member.ID;
            if (stringBeginsWith(member.nick, username) || stringBeginsWith(member.user.username, username))
            {
                users->push_back(std::make_pair(member.user.username, member.ID));
                userID = member.ID;
            }
            ctr++;
        }

        // If the cache had to be refreshed, set the mayBeRefreshed boolean to false, 
        // so the cache is not refreshed too often.
        membersOnServer[serverID] = newServerCache;
        
        if (!userID.empty()) {
            return true;
        }
    }

    return false;
}

int Pat::getPatReceivedCount(std::string user)
{
    if (patData["headpat_count"]["user"].contains(user))
    {
        return patData["headpat_count"]["user"][user]["received"];
    }
    else
    {
        return 0;
    }
}

int Pat::getPatGivenCount(std::string user)
{
    if (patData["headpat_count"]["user"].contains(user))
    {
        return patData["headpat_count"]["user"][user]["given"];
    }
    else
    {
        return 0;
    }
}

void Pat::loadData()
{
    std::string line;
    std::string jsonData;
    std::ifstream dataFile("PAT_DATA.json");
    if (dataFile.is_open())
    {
        while (std::getline(dataFile, line))
        {
            jsonData += line;
        }
        dataFile.close();
        patData = json::parse(jsonData);
    }
    else
    {
        std::cout << "Could not open PAT_DATA File\n";
    }
}

bool Pat::stringBeginsWith(std::string source, std::string key) {
    if (source.size() < key.size()) {
        return false;
    }
    for (size_t i = 0; i < key.size(); i++) {
        if (((source[i] < 91 && source[i] > 64) || (source[i] < 123 && source[i] > 96)) &&
                ((key[i] < 91 && key[i] > 64) || (key[i] < 123 && key[i] > 96))) {
            
            if (source[i] - key[i] != 0 && abs(source[i] - key[i]) != 32) {
                return false;
            }
        } else {
            if (source[i] != key[i]) {
                return false;
            }
        }
    }
    return true;
}

std::string Pat::longDescription()
{
    std::string out = "Headpat a user or pod!\n`Pod? pat` for patting Pod (recommended usage!)\n`Pod? pat *@User*` for patting a user\n`Pod? rcount *user (or none for Pod's count)*` for getting the number of times a user was patted\n`Pod? pat gcount *user*` for getting the number of times a user has given pats to someone!";
    return out;
}

std::string Pat::description()
{
    std::string out = "Headpat a user!";
    return out;
}