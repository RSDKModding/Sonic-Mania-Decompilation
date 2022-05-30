#if RETRO_REV02

// This is the "dummy" struct, it serves as the base in the event a suitable API isn't loaded (such as in this decomp)
// This struct should never be removed, other structs such as "SteamLeaderboards" would be added and "leaderboards" would be set to that instead
struct DummyLeaderboards : UserLeaderboards {
    void FrameInit()
    {
        if (status == STATUS_CONTINUE) {
            if (loadTime) {
                loadTime--;
            }
            else {
                status = STATUS_OK;

                if (!isUser) {
                    entryInfo.entryStart.start  = 1;
                    entryInfo.entryStart.length = 20;
                    entryInfo.globalRankOffset  = 1;
                    entryInfo.entryCount.start  = 20;
                }
                else {
                    entryInfo.entryStart.start  = userRank - 10;
                    entryInfo.entryStart.length = 20;
                    entryInfo.globalRankOffset  = userRank - 10;
                    entryInfo.entryCount.start  = 20;
                }

                FillLeaderboardEntries();
            }
        }

        if (trackTime != 0) {
            trackTime--;
        }
        else {
            status = STATUS_OK;

            if (trackCB) {
                trackCB(true, trackRank);
            }

            trackTime = -1;
            trackCB   = NULL;
        }
    }

    void FetchLeaderboard(LeaderboardID *leaderboard, bool32 isUser);
    void TrackScore(LeaderboardID *leaderboard, int32 score, void (*callback)(bool32 success, int32 rank));

    void FillLeaderboardEntries();

    int32 loadTime;
    int32 trackTime;
    int32 trackRank;
    void (*trackCB)(bool32 success, int32 rank);
};
#endif

// Start custom leaderboard code

extern std::vector<LeaderboardInfo> leaderboardList;

// End custom leaderboard code

