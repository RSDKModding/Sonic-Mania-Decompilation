#if RETRO_REV02

// This is the "dummy" struct, it serves as the base in the event a suitable API isn't loaded (such as in this decomp)
// This struct should never be removed, other structs such as "SteamLeaderboards" would be added and "leaderboards" would be set to that instead
struct DummyLeaderboards : UserLeaderboards {
    int32 unknown4() { return 0; }
#if RETRO_VER_EGS
    int32 unknown6() { return 0; }
#endif
    void FetchLeaderboard(LeaderboardID *leaderboard, bool32 isUser);
    void unknown5() {}
    void TrackScore(LeaderboardID *leaderboard, int32 score, void (*callback)(bool32 success, int32 rank));
    inline int32 GetStatus() { return status; }

    int32 loadTime;
    int32 trackTime;
    int32 trackRank;
    void (*trackCB)(bool32 success, int32 rank);
};
#endif

// Start custom leaderboard code

extern std::vector<LeaderboardInfo> leaderboardList;

// End custom leaderboard code

void FillDummyLeaderboardEntries();
