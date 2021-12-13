#ifndef USER_LEADERBOARDS_H
#define USER_LEADERBOARDS_H

struct LeaderboardEntry {
    TextInfo username;
#if RETRO_REV02
    TextInfo userID;
#endif
    int globalRank;
    int score;
    bool32 isUser;
    int status;
};

#if RETRO_REV02
struct DummyLeaderboardsUnknown2 {
    int loadType;
    int loadStartIndex;
    int loadEndIndex;
    int field_C;
    int field_10;
    int field_14;
};

struct DummyLeaderboardsUnknown {
    void *parent;
    int loadStatus;
    int globalRankOffset;
    int entryCount;
    int field_10;
    int entryStart;
    int entryLength;
    LeaderboardEntry entries[25];
};

// This is the base struct, it serves as the base for any API-specific stats
// This struct should never be removed
struct UserLeaderboards {
    virtual void StageLoad(void) {}
    virtual void FrameInit(void) {}
    virtual void OnUnknownEvent(void) {}
    virtual int32 unknown4(void) { return 0; }
#if RETRO_VER_EGS
    virtual int32 unknown6(void) { return 0; }
#endif
    virtual void FetchLeaderboard(const char *name, bool32 isUser) {}
    virtual void unknown5(void) {}
    virtual void TrackScore(const char *name, int32 score, void (*callback)(bool32 success, int32 rank)) {}
    virtual int32 GetStatus(void) { return 0; }

    const char *currentLeaderboard;
    DummyLeaderboardsUnknown2 unknown2;
    DummyLeaderboardsUnknown entryInfo;
    int32 status   = 0;
    int32 userRank = 0;
    bool32 isUser  = false;
    int32 list     = 0;
    int32 listSize = 0;
};

// This is the "dummy" struct, it serves as the base in the event a suitable API isn't loaded (such as in this decomp)
// This struct should never be removed, other structs such as "SteamLeaderboards" would be added and "leaderboards" would be set to that instead
struct DummyLeaderboards : UserLeaderboards {
    int32 unknown4(void) { return 0; }
#if RETRO_VER_EGS
    int32 unknown6(void) { return 0; }
#endif
    void FetchLeaderboard(const char *name, bool32 isUser);
    void unknown5(void) {}
    void TrackScore(const char *name, int32 score, void (*callback)(bool32 success, int32 rank));
    inline int32 GetStatus(void) { return status; }

    int32 loadTime;
    int32 trackTime;
    int32 trackRank;
    void (*trackCB)(bool32 success, int rank);
};
#endif

// Start custom leaderboard code
// this is added because we don't have access to any store APIs that would otherwise use this feature
struct LeaderboardInfo {
    char name[0x40];
    int score;
};

extern std::vector<LeaderboardInfo> leaderboardList;

// End custom leaderboard code

#if RETRO_REV02
extern UserLeaderboards *leaderboards;

inline int32 leaderboardsUnknown4(void) { return leaderboards->unknown4(); }
#if RETRO_VER_EGS
inline int32 leaderboardsUnknown6(void) { return leaderboards->unknown6(); }
#endif
inline void FetchLeaderboard(const char *name, bool32 isUser) { leaderboards->FetchLeaderboard(name, isUser); }
inline void TrackScore(const char *name, int32 score, void (*callback)(bool32 success, int32 rank)) { leaderboards->TrackScore(name, score, callback); }
inline int32 GetLeaderboardsStatus(void) { return leaderboards->GetStatus(); }

void FillDummyLeaderboardEntries();
Vector2 LeaderboardEntryLength();
Vector2 LeaderboardEntryCount();
void LoadNewLeaderboardEntries(int32 start, uint32 end, int32 type);
void ClearLeaderboardInfo();
LeaderboardEntry *ReadLeaderboardEntry(int entryID);
#endif

#endif // USER_LEADERBOARDS_H
