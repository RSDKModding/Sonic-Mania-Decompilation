#if RETRO_REV02

struct EOSStats : UserStats {
    inline void TryTrackStat(StatInfo *stat)
    { 
        // EGS has no stats
    }
};

#endif
