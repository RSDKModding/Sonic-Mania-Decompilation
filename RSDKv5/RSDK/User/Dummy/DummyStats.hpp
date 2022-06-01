#include <vector>

// This is the "dummy" struct, it serves as the base in the event a suitable API isn't loaded (such as in this decomp)
// This struct should never be removed, other structs such as "SteamStats" would be added and "stats" would be set to that instead
struct DummyStats : UserStats {
    void TryTrackStat(StatInfo *stat);
};
