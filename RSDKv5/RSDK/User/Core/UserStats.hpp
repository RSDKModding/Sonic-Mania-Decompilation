#ifndef USER_STATS_H
#define USER_STATS_H

#include <vector>

namespace RSDK
{

namespace SKU
{

struct StatInfo {
    uint8 statID;
    const char *name;
    void *data[64];
};

// This is the base struct, it serves as the base for any API-specific stats
// This struct should never be removed
struct UserStats {
    virtual void StageLoad(void) {}
    virtual void FrameInit(void) {}
    virtual void OnUnknownEvent(void) {}
    virtual void TryTrackStat(StatInfo *stat) {}

    bool32 enabled;
};

extern UserStats *stats;

// Rev01 ver of TrackStat basically
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
