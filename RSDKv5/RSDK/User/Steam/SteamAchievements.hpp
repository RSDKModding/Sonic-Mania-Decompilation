#if RETRO_REV02

struct SteamAchievements : UserAchievements {
    void TryUnlockAchievement(const char *name)
    {
        if (name) {
            // try unlock a steam achievement
        }
    }
};

#endif
