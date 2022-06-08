
#if RETRO_REV02
EOSCore *InitEOSCore()
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

    return core;
}
#endif
