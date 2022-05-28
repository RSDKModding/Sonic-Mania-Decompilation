#ifndef EOS_STATS_H
#define EOS_STATS_H

namespace RSDK
{

namespace SKU
{

#if RETRO_REV02

struct EOSStats : UserStats {
    inline void TryTrackStat(StatInfo *stat)
    { 
        // EGS has no stats
    }
};

#endif

} // namespace SKU

} // namespace RSDK

#endif // EOS_STATS_H
