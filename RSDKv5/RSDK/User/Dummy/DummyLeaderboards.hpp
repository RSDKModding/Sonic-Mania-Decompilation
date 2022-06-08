#if RETRO_REV02

struct DummyLeaderboardCallback {
    uint8 type;
    int32 loadTime;
    bool32 isUser;
    LeaderboardLoadInfo *info;
    int32 trackScore;
    int32 trackRank;
    void (*trackCB)(bool32 success, int32 rank);
};

// This is the "dummy" struct, it serves as the base in the event a suitable API isn't loaded (such as in this decomp)
// This struct should never be removed, other structs such as "SteamLeaderboards" would be added and "leaderboards" would be set to that instead
struct DummyLeaderboards : UserLeaderboards {
    void FrameInit() { 
        UserLeaderboards::FrameInit();

        for (int32 i = callbackList.Count() - 1; i >= 0; --i) {
            DummyLeaderboardCallback *item = callbackList.At(i);
        
            if (item) {
                if (item->loadTime) {
                    item->loadTime--;
                }
                else {
                    switch (item->type) {
                        case 0: FinishLeaderboardFetch(item); break;
                        case 1: FinishLeaderboardLoad(item); break;
                        case 2:
                            if (item->trackCB)
                                item->trackCB(true, item->trackRank);
                            break;
                    }

                    callbackList.Remove(i);
                }
            }
        }

    }

    void FetchLeaderboard(LeaderboardID *leaderboard, bool32 isUser);
    void LoadLeaderboards(LeaderboardLoadInfo *info);
    void TrackScore(LeaderboardID *leaderboard, int32 score, void (*callback)(bool32 success, int32 rank));

    void FillLeaderboardEntries(LeaderboardLoadInfo *info);
    void FinishLeaderboardFetch(DummyLeaderboardCallback *callback);
    void FinishLeaderboardLoad(DummyLeaderboardCallback *callback);

    List<DummyLeaderboardCallback> callbackList;
};
#endif

