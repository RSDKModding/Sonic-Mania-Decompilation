#if RETRO_REV02
SKU::NXCore *InitNXCore()
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

    // Setup default values

    memset(core->values, 0, sizeof(core->values));
    core->values[0]  = false;
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
