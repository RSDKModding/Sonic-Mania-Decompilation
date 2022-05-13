#include "RSDK/Core/RetroEngine.hpp"
#include "zlib/zlib.h"

#if RETRO_REV02
RSDK::SKU::NXCore *RSDK::SKU::InitNXCore()
{
    // Initalize API subsystems
    NXCore *core = new NXCore;

    if (achievements)
        delete achievements;
    achievements = new NXAchievements;

    if (leaderboards)
        delete leaderboards;
    leaderboards = new NXLeaderboards;

    if (richPresence)
        delete richPresence;
    richPresence = new NXRichPresence;

    if (stats)
        delete stats;
    stats = new NXStats;

    if (userStorage)
        delete userStorage;
    userStorage = new NXUserStorage;

    //Setup default values

    engine.hasPlus   = false; // TODO: DLC check
    core->values[0]  = (bool32 *)&engine.hasPlus;
    core->valueCount = 1;

    //TODO: remove
    leaderboards->userRank = 0;
    leaderboards->isUser   = false;

    achievements->enabled      = true;
    leaderboards->status       = GetAPIValue(GetAPIValueID("SYSTEM_LEADERBOARD_STATUS", 0));
    stats->enabled             = true;
    userStorage->authStatus    = STATUS_NONE;
    userStorage->storageStatus = STATUS_NONE;
    userStorage->saveStatus    = STATUS_NONE;
    userStorage->noSaveActive  = false;

    return core;
}
#endif
