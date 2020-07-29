#include <iostream>
#include "sleepy_discord/websocketpp_websocket.h"
#include "podSources.cpp"
#include <cstdlib>
#include "sleepy_discord/sleepy_discord.h"

int main(int, char**) {
    std::cout << "Hello, world!\n";
    std::string apiKey = std::getenv("DISCORD_KEY");
    if (apiKey.empty()) {
        std::cerr << "ERROR: DISCORD_KEY not found.\n";
        return -1;
    }
    Pod pod(apiKey, 2);
    pod.run();
}
