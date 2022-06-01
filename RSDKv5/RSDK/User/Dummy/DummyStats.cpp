#if RETRO_REV02
#define voidToInt(x)   (int)(size_t)(x)
#define voidToFloat(x) *(float *)&(x)

void DummyStats::TryTrackStat(StatInfo *stat)
{
    if (enabled) {
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
                // int32 unused     = voidToInt(stat->data[3]);
                int32 time  = voidToInt(stat->data[4]);
                int32 rings = voidToInt(stat->data[5]);
                int32 score = voidToInt(stat->data[6]);
                PrintLog(PRINT_NORMAL, "DUMMY TrackActClear(%s, %s, %s, %d, %d, %d)", zoneName, actName, playerName, score, rings, time);
                break;
            }

            case 1: {
                char *zoneName   = (char *)stat->data[0];
                char *actName    = (char *)stat->data[1];
                char *playerName = (char *)stat->data[2];
                char *mode       = (char *)stat->data[3];
                int32 time       = voidToInt(stat->data[4]);
                PrintLog(PRINT_NORMAL, "DUMMY TrackTAClear(%s, %s, %s, %s, %d)", zoneName, actName, playerName, mode, time);
                break;
            }

            case 2: {
                char *zoneName   = (char *)stat->data[0];
                char *actName    = (char *)stat->data[1];
                char *playerName = (char *)stat->data[2];
                bool32 encore    = voidToInt(stat->data[3]);
                int32 enemyX     = voidToInt(stat->data[4]);
                int32 enemyY     = voidToInt(stat->data[5]);
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
