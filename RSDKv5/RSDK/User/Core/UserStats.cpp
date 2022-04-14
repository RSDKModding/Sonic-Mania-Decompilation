#include "RSDK/Core/RetroEngine.hpp"

#if RETRO_REV02
RSDK::SKU::UserStats *RSDK::SKU::stats = NULL;
#endif

#if !RETRO_REV02
void GetLeaderboardName(char *buffer, int zoneID, int actID, int characterID)
{
    const char *characterIDs[] = { "S", "T", "K" };
    const char *zoneIDs[]      = { "GHZ", "CPZ", "SPZ", "FBZ", "PGZ", "SSZ", "HCZ", "MSZ", "OOZ", "LRZ", "MMZ", "TMZ" };

    sprintf(buffer, "%s%d_%s", zoneIDs[zoneID], actID + 1, characterIDs[characterID]);
}

void RSDK::SKU::TrackActClear(byte zoneID, byte actID, byte characterID, int time, int rings, int score)
{
    PrintLog(PRINT_NORMAL, "DUMMY TrackActClear(%d, %d, %d, %d, %d, %d)", zoneID, actID, characterID, time, rings, score);
}
void RSDK::SKU::TrackTAClear(byte zoneID, byte actID, byte characterID, int score)
{
    char leaderboardName[0x404];
    memset(leaderboardName, 0, 0x400);

    GetLeaderboardName(leaderboardName, zoneID, actID, characterID);

    PrintLog(PRINT_NORMAL, "DUMMY TrackTAClear(%d, %d, %d, %d) -> %s", zoneID, actID, characterID, score, leaderboardName);
}
void RSDK::SKU::TrackEnemyDefeat(byte zoneID, byte actID, byte characterID, int entityX, int entityY)
{
    PrintLog(PRINT_NORMAL, "DUMMY TrackEnemyDefeat(%d, %d, %d, %d, %d)", zoneID, actID, characterID, entityX, entityY);
}
void RSDK::SKU::TrackGameProgress(float percent) { PrintLog(PRINT_NORMAL, "DUMMY TrackGameProgress() -> %f percent complete", percent * 100); }
#endif