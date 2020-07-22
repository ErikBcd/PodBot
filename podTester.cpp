#include <stdio.h>
#include <string.h>
#include <iostream>
#include <map>

#include "commands/command.h"
#include "commands/myAnimeList.h"
#include "commands/owofier.h"

using namespace std;

int main() {
    bool running = true;
    string indicator = "Pod? ";

    map<string, Command*> commands;

    commands.insert(std::make_pair("owo", new OwOfier()));
    commands.insert(std::make_pair("mal", new MyAnimeList()));
    commands.insert(std::make_pair("generic", new Command()));

    while (running) {
        string input;
        cout << "Please enter a command uwu\n";
        getline(cin, input);

        cout << "Command = " << input << endl;

        if (input.rfind(indicator, 0) == 0) {
            string msg = input.substr(indicator.size(), input.size() - indicator.size());
			string command;
            string param = "";
            cout << "ehhhhhhhh " << msg << endl;
            size_t space_pos = msg.find(" ");
            if (space_pos != string::npos) {
                command = msg.substr(0, space_pos);
                param = msg.substr(command.size() + 1, msg.size() - command.size());
            } else {
                command = msg;
            }
            cout << "Command: " << command << endl;

            cout << "Your command was: " << command << "\nAnd the params were: " << param << endl;

            if (commands.find(command) != commands.end()) {
                cout << "Out: " << commands[command]->execute(param) << endl;
            } else {
                cout << "Out: " << commands["generic"]->execute(param) << endl;
            }

        }
    }
}