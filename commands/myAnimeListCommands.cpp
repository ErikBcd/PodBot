#include <iostream>
#include <curl/curl.h>

#include "include/json.hpp"
#include "include/command.h"
#include "include/myAnimeList.h"

#include "sleepy_discord/sleepy_discord.h"

/*
 * Commands for getting information out of the MyAnimeList Database.
 * Planned Features:
 *  - ´mal search (anime | manga | character | VA | ...) "Violet Evergarden"´ -> Returns the first result in a fancy message
 *  - ´mal getByID (anime | manga | character | VA | ...)  MAL_ID´ -> Returns information about the resource the ID points to
 * 
 */

MyAnimeListCommands::MyAnimeListCommands() {}

SleepyDiscord::Embed MyAnimeListCommands::createEmbed(Anime input) {
    SleepyDiscord::Embed embed;

    embed.title = std::move(input.title);
    embed.description = std::move(input.synopsis);

    SleepyDiscord::EmbedField type;
    type.name = std::move("Type: ");
    type.value = std::move(input.type);

    SleepyDiscord::EmbedField status;
    status.name = std::move("Status: ");
    status.value = std::move(input.status);

    SleepyDiscord::EmbedField rated;
    rated.name = std::move("Rated: ");
    rated.value = std::move(input.rated);

    SleepyDiscord::EmbedField start_date;
    start_date.name = std::move("Started: ");
    start_date.value = std::move(input.start_date);

    SleepyDiscord::EmbedField end_date;
    end_date.name = std::move("Ended: ");
    end_date.value = std::move(input.end_date);

    SleepyDiscord::EmbedField mal_id;
    mal_id.name = std::move("MyAnimeList ID: ");
    mal_id.value = std::move(input.mal_id);

    SleepyDiscord::EmbedField episodes;
    episodes.name = std::move("Episodes: ");
    episodes.value = std::move(input.episodes);

    SleepyDiscord::EmbedField score;
    score.name = std::move("Weeb Score: ");
    score.value = std::move(input.score);
    
    embed.fields.push_back(start_date);
    embed.fields.push_back(end_date);
    embed.fields.push_back(status);
    embed.fields.push_back(score);
    embed.fields.push_back(rated);
    embed.fields.push_back(type);
    embed.fields.push_back(episodes);
    embed.fields.push_back(mal_id);

    return embed;
}

SleepyDiscord::SendMessageParams MyAnimeListCommands::execute(std::string param) {
    SleepyDiscord::SendMessageParams params;
    params.embed = createEmbed(MyAnimeListAPI::getAnime(param));
    return params;
}



std::string MyAnimeListCommands::description()
{
    return "Put a description here, yo.";
}
