#include "sleepy_discord/sleepy_discord.h"
#include "base.cpp"

int main() {
    std::cout << "Hello, world!\n";
    std::string apiKey = std::getenv("DISCORD_KEY");
    if (apiKey.empty()) {
        std::cerr << "ERROR: DISCORD_KEY not found.\n";
        return -1;
    }
    Pod client(apiKey, SleepyDiscord::USER_CONTROLED_THREADS);
    client.setIntents(SleepyDiscord::Intent::SERVER_MESSAGES);
    client.run();
}