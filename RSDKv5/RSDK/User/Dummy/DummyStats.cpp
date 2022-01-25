#include "RSDK/Core/RetroEngine.hpp"

// Start Dummy Stats
std::vector<RSDK::SKU::StatInfo> RSDK::SKU::statList;
//End Dummy Stats

#if !RETRO_REV02
    // TODO: maybe?
#else
#define voidToInt(x)   (int)(size_t)(x)
#define voidToFloat(x) *(float *)&(x)

void RSDK::SKU::DummyStats::TryTrackStat(StatInfo *stat)
{
    if (stats->enabled) {
        std::string str = __FILE__;
        str += ": TrackStat() # TrackStat ";
        str += stat->name;
        str += " \r\n";
        PrintLog(PRINT_NORMAL, str.c_str());

        switch (stat->statID) {
            case 0: {
                char *zoneName   = (char *)stat->data[0];
                char *actName    = (char *)stat->data[1];
                char *playerName = (char *)stat->data[2];
                int val          = voidToInt(stat->data[3]);
                int time         = voidToInt(stat->data[4]);
                int rings        = voidToInt(stat->data[5]);
                int score        = voidToInt(stat->data[6]);
                PrintLog(PRINT_NORMAL, "DUMMY TrackActClear(%s, %s, %s, %d, %d, %d, %d)", zoneName, actName, playerName, val, score, rings, time);
                break;
            }
            case 1: {
                char *zoneName   = (char *)stat->data[0];
                char *actName    = (char *)stat->data[1];
                char *playerName = (char *)stat->data[2];
                char *mode       = (char *)stat->data[3];
                int time         = voidToInt(stat->data[4]);
                PrintLog(PRINT_NORMAL, "DUMMY TrackTAClear(%s, %s, %s, %s, %d)", zoneName, actName, playerName, mode, time);
                break;
            }
            case 2: {
                char *zoneName   = (char *)stat->data[0];
                char *actName    = (char *)stat->data[1];
                char *playerName = (char *)stat->data[2];
                bool32 encore    = voidToInt(stat->data[3]);
                int enemyX       = voidToInt(stat->data[4]);
                int enemyY       = voidToInt(stat->data[5]);
                PrintLog(PRINT_NORMAL, "DUMMY TrackEnemyDefeat(%s, %s, %s, %s, %d, %d)", zoneName, actName, playerName, encore ? "true" : "false",
                         enemyX, enemyY);
                break;
            }
            case 3: PrintLog(PRINT_NORMAL, "DUMMY TrackGameProgress() -> %f percent complete", voidToFloat(stat->data[0]) * 100); break;
        }
    }
    else {
        std::string str = __FILE__;
        str += ": TryTrackStat() # Track stat SKIPPED. Stats are disabled. \r\n";
        PrintLog(PRINT_NORMAL, str.c_str());
    }
}
#endif