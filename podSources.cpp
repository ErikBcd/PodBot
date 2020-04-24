#include "sleepy_discord/sleepy_discord.h"
#include "sleepy_discord/websocketpp_websocket.h"
#include <stdio.h>
#include <string.h>

class Pod01 : public SleepyDiscord::DiscordClient {
public:
    using SleepyDiscord::DiscordClient::DiscordClient;

    void onReady(std::string* startMsg) override {
        updateStatus("Pod? Help");
    }

    void onMessage(SleepyDiscord::Message message) override {
        if (message.startsWith("Pod? ")) {
            //std::string msg = message.content.substr(0, 5);
            if (message.startsWith("Pod? Help")) {
                sendMessage(message.channelID, "Hi! I am currently unter maintenance. Please come back later :3");
            } 
            else if (message.startsWith("Pod? Stop")) {
                sendMessage(message.channelID, "Okay, Goodbye :3");
                exit;
            }
        }
    }
};