#if RETRO_REV02
SKU::SteamCore *InitSteamCore()
{
    // Initalize API subsystems
    SteamCore *core = new SteamCore;

    if (achievements)
        delete achievements;
    achievements = new SteamAchievements;

    if (leaderboards)
        delete leaderboards;
    leaderboards = new SteamLeaderboards;

    if (richPresence)
        delete richPresence;
    richPresence = new SteamRichPresence;

    if (stats)
        delete stats;
    stats = new SteamStats;

    if (userStorage)
        delete userStorage;
    userStorage = new SteamUserStorage;

    core->initialized = true;

    return core;
}
#endif
