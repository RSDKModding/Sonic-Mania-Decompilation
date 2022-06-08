#ifndef USER_STATS_H
#define USER_STATS_H

#include <vector>

namespace RSDK
{

namespace SKU
{

#if RETRO_REV02
struct StatInfo {
    uint8 statID;
    const char *name;
    void *data[64];
};

// This is the base struct, it serves as the base for any API-specific stats
// This struct should never be removed
struct UserStats {
    virtual void StageLoad() { enabled = true; }
    virtual void FrameInit() {}
    virtual void OnUnknownEvent() {}
    virtual void TryTrackStat(StatInfo *stat) {}

    bool32 enabled = true;
};

extern UserStats *stats;

// ====================
// API Cores
// ====================

// Dummy API
#if RETRO_USERCORE_DUMMY
#include "RSDK/User/Dummy/DummyStats.hpp"
#endif

// Steam API
#if RETRO_USERCORE_STEAM
#include "RSDK/User/Steam/SteamStats.hpp"
#endif

// Epic Games API
#if RETRO_USERCORE_EOS
#include "RSDK/User/EOS/EOSStats.hpp"
#endif

// Switch API
#if RETRO_USERCORE_NX
#include "RSDK/User/NX/NXStats.hpp"
#endif

#endif

// Rev01 ver of TrackStat stuff basically
#if !RETRO_REV02
void TrackActClear(uint8 zoneID, uint8 actID, uint8 characterID, int32 time, int32 rings, int32 score);
void TrackTAClear(uint8 zoneID, uint8 actID, uint8 characterID, int32 time);
void TrackEnemyDefeat(uint8 zoneID, uint8 actID, uint8 characterID, int32 entityX, int32 entityY);
void TrackGameProgress(float percent);
#else
inline void TryTrackStat(StatInfo *stat) { stats->TryTrackStat(stat); }
inline bool32 GetStatsEnabled() { return stats->enabled; }
inline void SetStatsEnabled(bool32 enabled) { stats->enabled = enabled; }
#endif

} // namespace SKU

} // namespace RSDK

#endif // USER_STATS_H
