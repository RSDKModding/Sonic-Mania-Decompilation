#include "RSDK/Core/RetroEngine.hpp"

#if RETRO_REV02
// ====================
// API Cores
// ====================

namespace RSDK::SKU
{
// Dummy API
#if RETRO_USERCORE_DUMMY
#include "RSDK/User/Dummy/DummyStats.cpp"
#endif

// Steam API
#if RETRO_USERCORE_STEAM
#include "RSDK/User/Steam/SteamStats.cpp"
#endif

// Epic Games API
#if RETRO_USERCORE_EOS
#include "RSDK/User/EOS/EOSStats.cpp"
#endif

// Switch API
#if RETRO_USERCORE_NX
#include "RSDK/User/NX/NXStats.cpp"
#endif

} // namespace RSDK::SKU
#endif

using namespace RSDK;

#if RETRO_REV02
SKU::UserStats *RSDK::SKU::stats = NULL;
#endif

#if !RETRO_REV02
void GetLeaderboardName(char *buffer, int32 zoneID, int32 actID, int32 characterID)
{
    const char *characterIDs[] = { "S", "T", "K" };
    const char *zoneIDs[]      = { "GHZ", "CPZ", "SPZ", "FBZ", "PGZ", "SSZ", "HCZ", "MSZ", "OOZ", "LRZ", "MMZ", "TMZ" };

    sprintf(buffer, "%s%d_%s", zoneIDs[zoneID], actID + 1, characterIDs[characterID]);
}

void RSDK::SKU::TrackActClear(uint8 zoneID, uint8 actID, uint8 characterID, int32 time, int32 rings, int32 score)
{
    PrintLog(PRINT_NORMAL, "DUMMY TrackActClear(%d, %d, %d, %d, %d, %d)", zoneID, actID, characterID, time, rings, score);
}
void RSDK::SKU::TrackTAClear(uint8 zoneID, uint8 actID, uint8 characterID, int32 score)
{
    char leaderboardName[0x404];
    memset(leaderboardName, 0, 0x400);

    GetLeaderboardName(leaderboardName, zoneID, actID, characterID);

    PrintLog(PRINT_NORMAL, "DUMMY TrackTAClear(%d, %d, %d, %d) -> %s", zoneID, actID, characterID, score, leaderboardName);
}
void RSDK::SKU::TrackEnemyDefeat(uint8 zoneID, uint8 actID, uint8 characterID, int32 entityX, int32 entityY)
{
    PrintLog(PRINT_NORMAL, "DUMMY TrackEnemyDefeat(%d, %d, %d, %d, %d)", zoneID, actID, characterID, entityX, entityY);
}
void RSDK::SKU::TrackGameProgress(float percent) { PrintLog(PRINT_NORMAL, "DUMMY TrackGameProgress() -> %f percent complete", percent * 100); }
#endif