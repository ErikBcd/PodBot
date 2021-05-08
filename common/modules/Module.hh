#ifndef MODULE_HH
#define MODULE_HH

#include <iostream>
#include "sleepy_discord/sleepy_discord.h"
#include "../json.hpp"
#include "../kraken_api.hh"
#include "../Exceptions.cpp"

class Module {
    public:
        Module();
        virtual SleepyDiscord::SendMessageParams execute(std::string, SleepyDiscord::Message*);
        virtual std::string description();
        virtual std::string longDescription();
        virtual std::vector<std::string> parameterize(std::string);
        virtual bool isSecret();
};

class KrakenModule : public Module {
    public:
        KrakenModule(SleepyDiscord::DiscordClient*);
        SleepyDiscord::SendMessageParams execute(std::string, SleepyDiscord::Message*);
        /* std::string description();
        std::string longDescription();
        std::vector<std::string> parameterize(std::string);
        bool isSecret(); */

        void switchTicker();
        void startTicker();
        void stopTicker();
        static std::string currentPrice();
        static SleepyDiscord::Embed tickerMessage();
    private:
        static std::vector<std::pair<const std::string, const std::string>> activeTickers;
        static std::thread tickThread;
        static bool runTicker;
        static void ticker();
        static void addTickerMsg(SleepyDiscord::Message*);
        static void messageTicker();
        static SleepyDiscord::DiscordClient *client;
};

#endif 