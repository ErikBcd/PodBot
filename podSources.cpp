#include <stdio.h>
#include <string.h>
#include <regex>
#include "sleepy_discord/sleepy_discord.h"
#include "sleepy_discord/websocketpp_websocket.h"
using namespace std;

class Pod : public SleepyDiscord::DiscordClient {
	string indicator = "Pod?";
public:
	using SleepyDiscord::DiscordClient::DiscordClient;

	void onReady(SleepyDiscord::Ready message) override {
		updateStatus(indicator + " Help");
		cout << "RUNNING " << endl;
	}

	void onMessage(SleepyDiscord::Message message) {
		//cout << "Got Message" << endl;
		if (message.startsWith(indicator)) {
			string msg = message.content.substr( indicator.size(), message.content.size() - indicator.size());
			cout << "MSG = " +msg << endl;
			if (message.startsWith(indicator + " OwOfy!")) {
				sendMessage(message.channelID, owofyRegEx(msg));
			}
			else if (message.startsWith(indicator + " Help") || message.startsWith(indicator + " help")) {
				sendMessage(message.channelID, helpCommands());
			}
			else if (message.startsWith(indicator + " Ping!")) {
				sendMessage(message.channelID, "Pong!");
			}
			else if (message.startsWith(indicator + " Good Bot") || message.startsWith(indicator + "Good bot")) {
				sendMessage(message.channelID, "Thank you kindly, " + message.author.username);
			}
			else if (message.startsWith(indicator + " Update Status") && message.author.username == "Estugon") {
				updateStatus(msg.substr(string(" Update Status ").size(), msg.size() - string(" Update Status ").size()));
			}
			else if (message.startsWith(indicator + " Goodbye") && message.author.username == "Estugon") {
				sendMessage(message.channelID, "Okay, bye! o/");
				this->quit();
			}
		}
	}

	string helpCommands() {
		return	("I know these commands: \n`Pod? OwOfy! *Super cool text here*` OwOfies the given text\n`Pod? Ping!` Pong!\n`Pod? Help` Prints this text \nGot any ideas for new commands? Message Estugon! Baii");
	}

	string owofy(string msg) {
		msg = msg.substr(string(" OwOfy!").size(), msg.size() - string(" OwOfy!").size());
		string owoMsg = "";
		bool isEmpty = true;
		for (char c : msg) {
			if (c == 'r' || c == 'l') {
				owoMsg += 'w';
			}
			else if (c == 'R' || c == 'L') {
				owoMsg += 'W';
			}
			else {
				owoMsg += c;
			}
			if (c != ' ') {
				isEmpty = false;
			}
		}

		if (!isEmpty) {
			if ((&owoMsg.at(owoMsg.length() - 1))[0] == '?') {
				owoMsg += " UwU";
			}
			else {
				owoMsg += " OwO";
			}
			return owoMsg;
		}
		else {
			return "No usable Text found UwU \\nUsage: " + indicator + " OwOfy! *Very cool Text*";
		}
	}

	string owofyRegEx(string input) {
		
		input = input.substr(string(" OwOfy!").size(), input.size() - string(" OwOfy!").size());

		regex w("(?:r|l)");
		string output = input;
		
		output = regex_replace (input, w, "w");


		regex upperCaseW("(?:R|L)");
		output = regex_replace (output, upperCaseW, "W");


		regex ny("n([aeiou])");
		regex upperCaseNy("N([aeiou])");
		regex upperCaseNY("N([AEIOU])");
		regex ove("(ove)");
		regex exclamation("\!");
		regex question("\\?");
		regex point("\\.");

		output = regex_replace (output, ny, "ny$1");
		output = regex_replace (output, upperCaseNy, "Ny$1");
		output = regex_replace (output, upperCaseNY, "NY$1");
		output = regex_replace(output, ove, "uv");
		output = regex_replace(output, exclamation, "$& >w<");
		output = regex_replace(output, question, "$& UwU");
		output = regex_replace(output, point, "$& OwO");

		return output;
	}
};