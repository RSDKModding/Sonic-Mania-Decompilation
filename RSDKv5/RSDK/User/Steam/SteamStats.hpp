#ifndef STEAM_STATS_H
#define STEAM_STATS_H

namespace RSDK
{

namespace SKU
{

#if RETRO_REV02

struct SteamStats : UserStats {
    inline void TryTrackStat(StatInfo *stat) { /*steam has no stats*/ }
};

#endif

} // namespace SKU

} // namespace RSDK

#endif // STEAM_STATS_H
