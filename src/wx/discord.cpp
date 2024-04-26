#include "discord_rpc.h"
#include <iostream>
extern "C" {
#include "lua.hpp"
}

lua_State* L;

// Load settings from Lua script
bool loadSettings(const char** clientId, const char** largeImageKey, const char** smallImageKey) {
    luaL_dofile(L, "configs/config.lua");
    lua_getglobal(L, "clientID");
    *clientId = lua_tostring(L, -1);
    lua_getglobal(L, "largeImageKey");
    *largeImageKey = lua_tostring(L, -1);
    lua_getglobal(L, "smallImageKey");
    *smallImageKey = lua_tostring(L, -1);

    return strcmp(*clientId, "your_client_id") != 0;  // Check if default
}

// Initialize
void initDiscord(const char* clientId, const char* largeImageKey, const char* smallImageKey) {
    if (!clientId || strcmp(clientId, "your_client_id") == 0) {
        std::cout << "Disabling Discord Rich Presence." << std::endl;
        return;
    }

    DiscordEventHandlers handlers{};
    memset(&handlers, 0, sizeof(handlers));
    Discord_Initialize(clientId, &handlers, 1, nullptr);

    DiscordRichPresence discordPresence{};
    memset(&discordPresence, 0, sizeof(discordPresence));
    discordPresence.state = "Playing: WIP";
    discordPresence.details = "VisualBoyAdvance-M";
    discordPresence.startTimestamp = time(nullptr);
    discordPresence.largeImageKey = largeImageKey;
    discordPresence.smallImageKey = smallImageKey;
    Discord_UpdatePresence(&discordPresence);
}

// Shutdown Discord
void shutdownDiscord() {
    Discord_Shutdown();
}

int main() {
    L = luaL_newstate();
    luaL_openlibs(L);

    const char* clientId;
    const char* largeImageKey;
    const char* smallImageKey;
    if (loadSettings(&clientId, &largeImageKey, &smallImageKey)) {
        initDiscord(clientId, largeImageKey, smallImageKey);

            while (true) {
            // WIP
        }


        shutdownDiscord();
    }

    lua_close(L);
    return 0;
}