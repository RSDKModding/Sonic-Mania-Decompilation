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
    int field_8;
    int entryCount;
    int field_10;
    int entryStart;
    int entryLength;
    LeaderboardEntry entries[25];
};

struct DummyLeaderboards {
    void (*SetDebugValues)(void);
    void (*InitUnknown1)(void);
    void (*InitUnknown2)(void);
    int (*unknown4)(void);
#if RETRO_VER_EGS
    int (*unknown6)(void);
#endif
    void (*FetchLeaderboard)(const char *name, bool32 isUser);
    void (*unknown5)(void);
    void (*TrackScore)(const char *name, int score, void (*callback)(int status, int rank));
    int (*GetStatus)(void);

    const char *currentLeaderboard;
    DummyLeaderboardsUnknown2 unknown2;
    DummyLeaderboardsUnknown unknown;
    int status;
    int userRank;
    int isUser;
    int list;
    int listSize;
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
extern DummyLeaderboards *leaderboards;

void FillDummyLeaderboardEntries();
void FetchLeaderboard(const char *name, bool32 isUser);
void TrackScore(const char *name, int score, void (*callback)(int status, int rank));
Vector2 LeaderboardEntryLength();
Vector2 LeaderboardEntryCount();
void LoadNewLeaderboardEntries(int32 start, uint32 end, int32 type);
void ClearLeaderboardInfo();
LeaderboardEntry *ReadLeaderboardEntry(int entryID);
inline int GetLeaderboardStatus() { return leaderboards->status; }
#endif

#endif // USER_LEADERBOARDS_H
