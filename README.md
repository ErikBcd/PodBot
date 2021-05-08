## PodBot ##

New version of my smol bot.

Build:
1. Create build directory
    `mkdir build`
2. Configure cmake (I use Ninja for compiling)
    `cmake -G "Ninja" ..`
3. Build
    `ninja`

To launch, set a enviroment variable called `DISCORD_KEY` which contains your Discord API key, and then launch `Pod` in the build directory.
