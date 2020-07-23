#include <iostream>
#include "sleepy_discord/websocketpp_websocket.h"
#include "podSources.cpp"
#include "sleepy_discord/sleepy_discord.h"

int main(int, char**) {
    std::cout << "Hello, world!\n";
    Pod pod("NzAyMjk3NjI4MzE4MjM2Njc0.XxjVcQ.Cjqtfjbr1z30tUHhqCXXWpoUYTk", 2);
    pod.run();
}
