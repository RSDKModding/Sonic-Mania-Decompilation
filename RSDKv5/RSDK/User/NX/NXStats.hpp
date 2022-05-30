#if RETRO_REV02
struct NXStats : UserStats {
    inline void TryTrackStat(StatInfo *stat)
    { 
        // switch has no stats
    }
};
#endif
