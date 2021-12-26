#ifndef USER_STATS_H
#define USER_STATS_H

#include <vector>

struct StatInfo {
    byte statID;
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

// This is the "dummy" struct, it serves as the base in the event a suitable API isn't loaded (such as in this decomp)
// This struct should never be removed, other structs such as "SteamStats" would be added and "stats" would be set to that instead
struct DummyStats : UserStats {
    void TryTrackStat(StatInfo *stat);
};

//Start Dummy Stats code

extern std::vector<StatInfo> statList;

//End Dummy Stats code

extern UserStats *stats;

// Rev01 ver of TrackStat basically
#if !RETRO_REV02
void TrackActClear(byte zoneID, byte actID, byte characterID, int time, int rings, int score);
void TrackTAClear(byte zoneID, byte actID, byte characterID, int time);
void TrackEnemyDefeat(byte zoneID, byte actID, byte characterID, int entityX, int entityY);
void TrackGameProgress(float percent);
#else
inline void TryTrackStat(StatInfo *stat) { stats->TryTrackStat(stat); }
inline bool32 GetStatsEnabled() { return stats->enabled; }
inline void SetStatsEnabled(bool32 enabled) { stats->enabled = enabled; }
#endif

#endif // USER_STATS_H
