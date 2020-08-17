#include <iostream>
#include <fstream>
#include <string.h>
#include <map>

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

SleepyDiscord::SendMessageParams Pat::execute(std::string param, SleepyDiscord::Message *message)
{
    std::vector<SleepyDiscord::User> mentionedUsers = message->mentions;
    std::string user = message->author.ID;
    SleepyDiscord::SendMessageParams params;
    if (mentionedUsers.size() > 1)
    {
        params.content = std::move("You may only ping one person at a time! <:podPing:739476530727747656>");
        return params;
    }
    if (stringBeginsWith(param, ("<@!" + user)) || stringBeginsWith(param, ("<@" + user)))
    {
        params.content = std::move("\\*headpats you\\*");
        return params;
    }

    std::string command;
    std::vector<std::string> usernames;
    std::vector<std::string> userIDs;

    // At first, remove the mention from the param string, if there is one
    if (!mentionedUsers.empty())
    {
        size_t found = param.find(("<@" + mentionedUsers.at(0).ID + ">"));
        if (found != std::string::npos)
        {
            //21
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
            usernames.push_back("pod");
            userIDs.push_back("pod");
        }
        else
        {
            usernames.push_back(mentionedUsers.front().username);
            userIDs.push_back(mentionedUsers.front().ID.string());
        }
    }

    // Parameterize the input
    // That means finding possible commands and extracting all mentioned users into a map
    if (!param.empty())
    {
        std::vector<std::string> parameters = parameterize(param);
        for (std::string p : parameters)
        {
            std::cout << "Param: " << p << '\n';
        }

        // Find commands
        if (parameters.at(0) == "gcount" || parameters.at(0) == "rcount")
        {
            command = parameters.at(0);
            if (parameters.size() > 1)
            {
                std::string id = findUserID(parameters.at(1), message->serverID);
                if (!id.empty() && id != "User not found")
                {
                    userIDs.push_back(id);
                    usernames.push_back(parameters.at(1));
                }
                else
                {
                    params.content = std::move("The user " + parameters.at(1) + " was not found, I am vewwy sowwy uwu");
                    return params;
                }
            }
            else
            {
                usernames.push_back("pod");
                userIDs.push_back("pod");
            }
        }
        // Try to find the usernames that should be given.
        else if (parameters.size() > 0)
        {
            for (std::string p : parameters)
            {
                if (stringBeginsWith(p, "pod"))
                {
                    userIDs.push_back("pod");
                    usernames.push_back("pod");
                }
                else
                {
                    std::string id = findUserID(p, message->serverID);
                    if (id != "User not found" && !id.empty())
                    {
                        userIDs.push_back(id);
                        usernames.push_back(p);
                    }
                }
            }
            mayRefreshCache = true;
        }
    } else {
        userIDs.push_back("pod");
        usernames.push_back("pod");
    }

    if (command.empty())
    {
        std::string pattedUsers;
        std::string selfPatProtection;
        std::string output;
        for (size_t i = 0; i < userIDs.size(); i++)
        {
            if (userIDs.at(i) == "pod")
            {
                pat(user, "pod");
                output += "Thank you <:podYay:739476531449036941>\n";
            } 
            else if (!userIDs.at(i).empty() && userIDs.at(i) != "User not found")
            {
                if (userIDs.at(i) == user)
                {
                    selfPatProtection += (", " + usernames.at(i));
                }
                else
                {
                    pat(user, userIDs.at(i));
                    pattedUsers += (i < userIDs.size() - 1) ? (usernames.at(i) + ", ") : usernames.at(i);
                }
            }
        }
        output += (!pattedUsers.empty()) ? "Patted " + pattedUsers : "";
        output += (!selfPatProtection.empty()) ? "\nShouldn't you know better than patting yourself" + selfPatProtection + "?" : "";

        params.content = std::move((!output.empty()) ? output : "No one could be patted D: Maybe check your spelling? If nothing helps, try `[help pat`");
    }
    else
    {
        if (command == "gcount")
        {
            if (userIDs.front() == "pod")
            {
                // If no argument is given, return the total count of times people have been headpatted
                int count = getPatGivenCount("pod");
                int fullCount = count + getPatReceivedCount("pod");
                params.content = std::move("Overall, I gave **" + std::to_string(count) + "** headpats to the people! <:podYay:739476531449036941>\n(Including the headpats I received, **" + std::to_string(fullCount) + "** heads have been patted!!)");
            }
            else
            {
                int count = getPatGivenCount(userIDs.front());
                params.content = std::move(usernames.front() + " has patted other people **" + std::to_string(count) + "** times!");
            }
        }
        else
        {
            if (userIDs.front() == "pod")
            {
                // If no argument is given, return the total count of times people have been headpatted
                int count = getPatReceivedCount("pod");
                params.content = std::move("I have been patted **" + std::to_string(count) + "** times already! <:podYay:739476531449036941>");
            }
            else
            {
                int count = getPatReceivedCount(userIDs.front());
                params.content = std::move(usernames.front() + " has been patted **" + std::to_string(count) + "** times!");
            }
        }
    }

    /*
    // Determine what the user wants!
    if (parameters.empty() || stringBeginsWith(param, "pod") || stringBeginsWith(param, "<@!702297628318236674>") || stringBeginsWith(param, "<@702297628318236674>"))
    {
        // Patting Pod himself
        pat(user, "pod");
        params.content = std::move("uwu");
    }
    else if (parameters.at(0) == "gcount")
    {
        // Access the number of times a user headpatted someone else
        if (mentionedUsers.empty() || mentionedUsers.front().ID.string() == "702297628318236674")
        {
            // If no argument is given, return the total count of times people have been headpatted
            int count = getPatGivenCount("pod");
            int fullCount = count + getPatReceivedCount("pod");
            params.content = std::move("Overall, I gave **" + std::to_string(count) + "** headpats to the people! <:podYay:739476531449036941>\n(Including the headpats I received, **" + std::to_string(fullCount) + "** heads have been patted!!)");
        }
        else if (!mentionedUsers.empty())
        {
            int count = getPatGivenCount(mentionedUsers.front().ID);
            params.content = std::move(("<" + mentionedUsers.front().username + "> has given **" + std::to_string(count) + "** headpats to other people!"));
        }
    }
    else if (parameters.at(0) == "rcount")
    {
        // Access the number of times a user has been headpatted
        if (mentionedUsers.empty() || mentionedUsers.front().ID.string() == "702297628318236674")
        {
            int count = getPatReceivedCount("pod");
            params.content = std::move("I have been patted **" + std::to_string(count) + "** times already! <:podYay:739476531449036941>");
        }
        else if (!mentionedUsers.empty())
        {
            int count = getPatReceivedCount(mentionedUsers.front().ID);
            params.content = std::move("<" + mentionedUsers.front().username + "> has been patted **" + std::to_string(count) + "** times!");
        }
    }
    else if (!mentionedUsers.empty())
    {
        // Headpat one or more users
        std::string answer = (mentionedUsers.size() > 1) ? "Multi-Headpat!\nPatted " : "Patted ";
        for (size_t i = 0; i < mentionedUsers.size(); i++)
        {
            answer += mentionedUsers.at(i).username;
            answer += (i == mentionedUsers.size() - 1) ? "" : ", ";
            pat(user, mentionedUsers.at(i).ID);
        }
        answer += "!";
        params.content = std::move(answer);
    }
    else
    {
        // If you are here, nothing of meaning has been written
        params.content = std::move("I don't really know what " + parameters.at(0) + " means, maybe it was a typo? Try one of these instead:\n" + longDescription());
    }*/

    return params;
}

json Pat::patData;
std::map<std::string, json> Pat::membersOnServer;

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

std::string Pat::findUserID(std::string username, std::string serverID)
{
    // Try to find the user in the map structure first
    
    if (membersOnServer.find(serverID) != membersOnServer.end()) {
        //std::cout << "JSON DUMP: \n" << membersOnServer[serverID].dump() << "\n\n";
        for (json& item : membersOnServer[serverID]["members"]) {
            //std::cout << "Item! ------ Name: " << item["username"] << " ------ Nick: "<< item["nick"];
            if (stringBeginsWith(item["nick"], username) || stringBeginsWith(item["username"], username)) {
                std::cout << "Got ID from cache!\n";
                return item["id"];
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
            return "";
        }

        //std::cout << "Members: " << members.size() << '\n';
        std::string userID;
        int ctr = 0;

        for (SleepyDiscord::ServerMember member : members)
        {
            //std::cout << "MemberNick: " << member.nick << " MemberUsername: " << member.user.username << '\n';
            newServerCache["members"][("user"+ std::to_string(ctr))]["nick"] = member.nick;
            newServerCache["members"][("user"+ std::to_string(ctr))]["username"] = member.user.username;
            newServerCache["members"][("user"+ std::to_string(ctr))]["id"] = member.ID;
            if (stringBeginsWith(member.nick, username) || stringBeginsWith(member.user.username, username))
            {
                userID = member.ID;
            }
            ctr++;
        }

        // If the cache had to be refreshed, set the mayBeRefreshed boolean to false, 
        // so the cache is not refreshed too often.
        membersOnServer[serverID] = newServerCache;
        //std::cout << "New cache dump: \n" << newServerCache.dump() << "\nDump in Map:\n" << membersOnServer[serverID].dump() << '\n';
        if (!userID.empty()) {
            return userID;
        }
    }

    return "User not found";
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

bool Pat::stringBeginsWith(std::string source, std::string key)
{
    if (source.size() < key.size())
    {
        return false;
    }
    for (size_t i = 0; i < key.size(); i++)
    {
        if (source[i] - key[i] != 0 && abs(source[i] - key[i]) != 32)
        {
            return false;
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