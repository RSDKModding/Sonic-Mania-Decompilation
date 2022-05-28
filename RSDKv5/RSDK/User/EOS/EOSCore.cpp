#include "RSDK/Core/RetroEngine.hpp"

using namespace RSDK;

#if RETRO_REV02
SKU::EOSCore *RSDK::SKU::InitEOSCore()
{
    // Initalize API subsystems
    EOSCore *core = new EOSCore;

    if (achievements)
        delete achievements;
    achievements = new EOSAchievements;

    if (leaderboards)
        delete leaderboards;
    leaderboards = new EOSLeaderboards;

    if (richPresence)
        delete richPresence;
    richPresence = new EOSRichPresence;

    if (stats)
        delete stats;
    stats = new EOSStats;

    if (userStorage)
        delete userStorage;
    userStorage = new EOSUserStorage;

    // Setup default values

    engine.hasPlus   = false; // TODO: DLC check
    core->values[0]  = (bool32 *)&engine.hasPlus;
    core->valueCount = 1;

    // TODO: remove
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
