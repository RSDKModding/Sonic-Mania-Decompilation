#if RETRO_REV02

struct SteamStats : UserStats {
    inline void TryTrackStat(StatInfo *stat)
    { 
        // steam has no stats
    }
};

#endif
