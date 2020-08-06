#include <stdio.h>
#include <string.h>
#include <regex>
#include "sleepy_discord/sleepy_discord.h"
#include "sleepy_discord/websocketpp_websocket.h"

#include "commands/include/command.h"
#include "commands/include/exceptions.h"


using namespace std;

class Pod : public SleepyDiscord::DiscordClient {
public:
	using SleepyDiscord::DiscordClient::DiscordClient;

	void onReady(SleepyDiscord::Ready message) override {
		lastfm::setAPIkey();
		commands.insert(std::make_pair("owo", new OwOfier()));
    	commands.insert(std::make_pair("mal", new MyAnimeListCommands()));
    	commands.insert(std::make_pair("generic", new Command()));
		commands.insert(std::make_pair("lastfm", new LastFMCommand()));
		commands.insert(std::make_pair("kohaku", new Kohaku()));
		commands.insert(std::make_pair("help", new Help(commands)));
		//updateStatus(indicator + " Help");
		updateStatus("Pod? help");
		cout << "RUNNING " << endl;
	}

	void onMessage(SleepyDiscord::Message message) {
		if (message.author.bot) {
			return;
		}
		try {
			if (stringBeginsWith(message.content, indicator)) {
				string msg = message.content;
				msg = msg.substr(indicator.size(), msg.size() - indicator.size());
				// Some preprocessing
				int i = 0;
				for (char c : msg) {
					c = std::tolower(c);
					msg.at(i) = c;
					i++;
				}

				SleepyDiscord::SendMessageParams params = executeCommand(msg);
				params.channelID = message.channelID;
				
				sendMessage(params);
			
			
			} else if (message.startsWith("Pod!") || message.startsWith("pod!")) {
				if (message.author.username != "Falcon") {
					sendMessage(message.channelID, message.author.username +"!");
				}
			} else if (message.isMentioned("702297628318236674")) {
				sendMessage(message.channelID, "<:podPing:739476530727747656>");
			} else if (message.startsWith("PodUpdate") && message.author.ID == "157491513054461953") {
				string msg = message.content;
				msg = msg.substr(10, msg.size() - 10);
				updateStatus(msg);
				addReaction(message.channelID, message.ID, ":podYay:739476531449036941");
			}
		} catch(SleepyDiscord::ErrorCode e) {
			std::cerr << "ALARM! ALAAHAARM!\n" << std::endl;
		}
	}
private:
	const string indicator = "Pod? ";
	map<string, Command*> commands;

	bool stringBeginsWith(std::string source, std::string key) {
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

	SleepyDiscord::SendMessageParams executeCommand(string commandRaw) {
		string command;
		string param;

		size_t space_pos = commandRaw.find(" ");
		if (space_pos != string::npos) {
			command = commandRaw.substr(0, space_pos);
			param = commandRaw.substr(command.size() + 1, 
					commandRaw.size() - command.size());
		} else {
			command = commandRaw;
		}

		if (commands.find(command) != commands.end()) {
			return commands[command]->execute(param);
		} else {
			return commands["help"]->execute(command);
		}
	} 
};