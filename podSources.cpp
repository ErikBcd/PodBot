#include <stdio.h>
#include <string.h>
#include <regex>
#include "sleepy_discord/sleepy_discord.h"
#include "sleepy_discord/websocketpp_websocket.h"

#include "commands/include/command.h"


using namespace std;

class Pod : public SleepyDiscord::DiscordClient {
	string indicator = "Pod? ";
public:
	using SleepyDiscord::DiscordClient::DiscordClient;

	void onReady(SleepyDiscord::Ready message) override {
		commands.insert(std::make_pair("owo", new OwOfier()));
    	commands.insert(std::make_pair("mal", new MyAnimeListCommands()));
    	commands.insert(std::make_pair("generic", new Command()));
		//updateStatus(indicator + " Help");
		updateStatus("Under Construction!");
		cout << "RUNNING " << endl;
	}

	void onMessage(SleepyDiscord::Message message) {
		
		if (message.startsWith(indicator)) {
			string msg = message.content;
			msg = msg.substr(indicator.size(), msg.size() - indicator.size());
			SleepyDiscord::SendMessageParams params = executeCommand(msg);
			params.channelID = message.channelID;

			cout << "Sending Message" << sendMessage(params).text;
		}
	}
private:
	map<string, Command*> commands;
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
			return commands["generic"]->execute(param);
		}
	} 
};