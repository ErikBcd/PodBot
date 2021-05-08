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
                if (message.author.ID.string() != "157491513054461953") {
                    sendMessage(message.channelID, "You are not allowed to do that, wtf");
                    return;
                }
                std::cout << "dogeticker from " << message.author.username << std::endl;
                SleepyDiscord::SendMessageParams p = modules["doge"]->execute("switch", &message);
                p.channelID = message.channelID;
                
                sendMessage(p);
            } else if (message.startsWith("pod doge")) {
                std::cout << "doge from " << message.author.username << std::endl;
                SleepyDiscord::SendMessageParams p = modules["doge"]->execute("", &message);
                p.channelID = message.channelID;
                sendMessage(p);

            } else if (message.startsWith("pod stop") && message.author.ID.string() == "157491513054461953") {
                sendMessage(message.channelID, "Shutting down!");
                this->quit();
            } else if (message.startsWith("Pod!") || message.startsWith("pod!")) {
				sendMessage(message.channelID, message.author.username +"!");
			} else if (message.isMentioned("702297628318236674")) {
				sendMessage(message.channelID, "<:podPing:739476530727747656>");
			} else if (message.startsWith("PodUpdate") && message.author.ID == "157491513054461953") {
				std::string msg = message.content;
				msg = msg.substr(10, msg.size() - 10);
				updateStatus(msg);
				addReaction(message.channelID, message.ID, ":podYay:739476531449036941");
			}

        }

    private:
        std::map<std::string, Module*> modules;
};