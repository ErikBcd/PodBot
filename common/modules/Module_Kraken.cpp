#include "Module.hh"
#include <iomanip>
#include "../Exceptions.cpp"

KrakenModule::KrakenModule(SleepyDiscord::DiscordClient *c) {
    client = c;
    KrakenAPIListener::startRunner();
}

SleepyDiscord::DiscordClient *KrakenModule::client;
std::thread KrakenModule::tickThread;
bool KrakenModule::runTicker;
std::vector<std::pair<const std::string, const std::string>> KrakenModule::activeTickers;

SleepyDiscord::SendMessageParams KrakenModule::execute(std::string args, SleepyDiscord::Message* m) {
    SleepyDiscord::SendMessageParams params;
    std::string out = "";
    if (args.length() > 1) {
        //switchTicker();
        addTickerMsg(m);
        if (!runTicker) {
            runTicker = true;
            tickThread = std::thread(messageTicker);
        }
        out = "Created ticker!";
        std::this_thread::sleep_for(std::chrono::seconds(1));
    } else {
        out = "Currently in testing mode. \n 1 DOGE is " + std::to_string(KrakenAPIListener::getData().ask) + " EUR.";
        if (KrakenAPIListener::getData().error) {
            out += "\n Kraken API ran into an issue:\n" + KrakenAPIListener::getData().errmsg[0];
        }
    }
    params.content = out;
    return params;
}

std::string KrakenModule::currentPrice() {
    std::string out = KrakenAPIListener::getData().currencyPair;
    std::string sep = " ∓  ";
    if (KrakenAPIListener::getFallbackData().ask > KrakenAPIListener::getData().ask) {
        sep = " ▼ ";
    } else if (KrakenAPIListener::getFallbackData().ask < KrakenAPIListener::getData().ask) {
        sep = " ▲ ";
    }
    std::stringstream str;
    str << std::fixed << std::setprecision(4) << KrakenAPIListener::getData().ask;
    out = out + sep + str.str();

    return out;
}

SleepyDiscord::Embed KrakenModule::tickerMessage() {
    SleepyDiscord::Embed embed;
    embed.title = std::move("DOGE Info");
    embed.footer.text = std::move("Data taken from Kraken");

    //std::stringstream str;
    //str << std::fixed << std::setprecision(6) << KrakenAPIListener::getData().ask;
    std::string sep = "∓  ";
    if (KrakenAPIListener::getFallbackData().ask > KrakenAPIListener::getData().ask) {
        sep = "▼ ";
    } else if (KrakenAPIListener::getFallbackData().ask < KrakenAPIListener::getData().ask) {
        sep = "▲ ";
    }
    embed.fields.push_back(
        SleepyDiscord::EmbedField(
            "Current rate: ", 
            sep + std::to_string(KrakenAPIListener::getData().ask)
        )
    );

    embed.fields.push_back(
        SleepyDiscord::EmbedField(
            "24h high: ", 
            std::to_string(KrakenAPIListener::getData().high24h)
        )
    );

    embed.fields.push_back(
        SleepyDiscord::EmbedField(
            "24h low: ", 
            std::to_string(KrakenAPIListener::getData().low24h)
        )
    );

    return embed;
}

void KrakenModule::startTicker() {
    runTicker = true;
    tickThread = std::thread(ticker);
}

void KrakenModule::stopTicker() {
    runTicker = false;
    tickThread.join();
}

void KrakenModule::switchTicker() {
    if (runTicker) {
        stopTicker();
        client->updateStatus("uwu");
    } else {
        startTicker();
    }
}

void KrakenModule::addTickerMsg(SleepyDiscord::Message* msg) {
    SleepyDiscord::Message m = client->sendMessage(msg->channelID, "", tickerMessage());
    activeTickers.push_back(std::make_pair(m.channelID.string(), m.ID.string()));
}

void KrakenModule::messageTicker() {
    while (runTicker) {
        for (std::pair<const std::string, const std::string> msg : activeTickers) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            client->editMessage(msg.first, msg.second, "", tickerMessage());
        }
    }
}

void KrakenModule::ticker() {
    while (runTicker) {
        std::this_thread::sleep_for(std::chrono::seconds(3));
        client->updateStatus(currentPrice());
    }
}