#include "sleepy_discord/sleepy_discord.h"
#include "common/modules/Module.hh"
#include "common/Exceptions.cpp"

class Pod : public SleepyDiscord::DiscordClient {
    public:
        using SleepyDiscord::DiscordClient::DiscordClient;
        void onReady(SleepyDiscord::Ready message) override {
            modules.insert(std::make_pair("generic", new Module()));
            modules.insert(std::make_pair("doge", new KrakenModule(this)));

            updateStatus("hello.");
            std::cout << "Running..\n";
        }

        void onMessage(SleepyDiscord::Message message) override {
            if (message.author.bot) {return;}

            if (message.startsWith("pod hello")) {
                std::cout << "hello from " << message.author.username << std::endl;
                sendMessage(message.channelID, "Hello " + message.author.username);
            } else if (message.startsWith("pod dogeticker")) {
                std::cout << "dogeticker from " << message.author.username << std::endl;
                SleepyDiscord::SendMessageParams p = modules["doge"]->execute("switch", &message);
                p.channelID = message.channelID;
                
                sendMessage(p);
            } else if (message.startsWith("pod doge")) {
                std::cout << "doge from " << message.author.username << std::endl;
                SleepyDiscord::SendMessageParams p = modules["doge"]->execute("", &message);
                p.channelID = message.channelID;
                sendMessage(p);

            } else if (message.startsWith("pod ")) {
                std::cout << "pod from " << message.author.username << std::endl;
                sendMessage(message.channelID, "Hey " + message.author.username + ", I am quite busy right now, so I can't follow your order, sorry.");
            }

        }

    private:
        std::map<std::string, Module*> modules;
};