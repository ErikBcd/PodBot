#include <iostream>
#include <string.h>

#include "include/command.h"
#include "sleepy_discord/sleepy_discord.h"

// A whole class just for a single fun command. Because that's definetely... needed.

Kohaku::Kohaku() {
}

SleepyDiscord::SendMessageParams Kohaku::execute(std::string param, SleepyDiscord::Message* message) {
    SleepyDiscord::SendMessageParams params;
    params.content = std::move("Heya! Do you know Kohaku? If not, you should!\nOh and btw, I get angry at everyone who has a problem with Kohaku. Like, really angry.\n<:podAngry:739476530727616552>");
    return params;
}

bool Kohaku::isSecret() {
    return true;
}

std::string Kohaku::longDescription() {
    return "Psst! I am a very secret function! Tell Kohaku I said hi!";
}

