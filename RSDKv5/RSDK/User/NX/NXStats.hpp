#ifndef NX_STATS_H
#define NX_STATS_H

namespace RSDK
{

namespace SKU
{

#if RETRO_REV02

struct NXStats : UserStats {
    inline void TryTrackStat(StatInfo *stat)
    { 
        // switch has no stats
    }
};

#endif

} // namespace SKU

} // namespace RSDK

#endif // NX_STATS_H
