#if RETRO_REV02

struct SteamLeaderboards : UserLeaderboards {
    void FetchLeaderboard(LeaderboardID *leaderboard, bool32 isUser) {
        // fetch leaderboards from steam
    }
    void LoadLeaderboards() {
        // do a thing
    }
    void TrackScore(LeaderboardID *leaderboard, int32 score, void (*callback)(bool32 success, int32 rank)) {
        // set leaderboard on steam
    }
    int32 GetStatus() { return status; }
};

#endif
