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
struct DummyLeaderboards {
    void (*SetDebugValues)(void);
    void (*InitUnknown1)(void);
    void (*InitUnknown2)(void);
    int (*unknown4)(void);
#if RETRO_VER_EGS
    int (*unknown6)(void);
#endif
    void (*FetchLeaderboard)(const char *name, int id);
    void (*unknown5)(void);
    void (*TrackScore)(const char *name, int score, void (*callback)(int status, int rank));
    int (*GetStatus)(void);

    int field_4;
    int field_8;
    int field_C;
    int field_10;
    int field_14;
    int field_18;
    int field_1C;
    int field_20;
    int field_24;
    int prevIsUser;
    int prevRank;
    int rankScore;
    int entryStart;
    int entryLength;
    LeaderboardEntry *entryPtrs[200];
    int status;
    int downloadStatus;
    int field_364;
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
void FetchLeaderboard(const char *name, int id);
void TrackScore(const char *name, int score, void (*callback)(int status, int rank));
Vector2 LeaderboardEntryCount();
LeaderboardEntry *ReadLeaderboardEntry(int entryID);
inline int GetLeaderboardStatus() { return leaderboards->status; }
#endif

#endif // USER_LEADERBOARDS_H
