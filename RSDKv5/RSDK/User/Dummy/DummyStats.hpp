#ifndef DUMMY_STATS_H
#define DUMMY_STATS_H

#include <vector>

namespace RSDK
{

namespace SKU
{

// This is the "dummy" struct, it serves as the base in the event a suitable API isn't loaded (such as in this decomp)
// This struct should never be removed, other structs such as "SteamStats" would be added and "stats" would be set to that instead
struct DummyStats : UserStats {
    void TryTrackStat(StatInfo *stat);
};

// Start Dummy Stats code

extern std::vector<StatInfo> statList;

// End Dummy Stats code

} // namespace SKU

} // namespace RSDK

#endif // DUMMY_STATS_H
