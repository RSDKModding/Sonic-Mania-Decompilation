#ifndef USER_LEADERBOARDS_H
#define USER_LEADERBOARDS_H

namespace RSDK
{

namespace SKU
{

struct LeaderboardID {
    int id1;
    int id2;
    int id_switch;
    const char *name_legacy;
    const char *name;
};

struct LeaderboardEntry {
    String username;
#if RETRO_REV02
    String userID;
#endif
    int globalRank;
    int score;
    bool32 isUser;
    int status;
};

#if RETRO_REV02
struct LeaderboardsUnknown2 {
    int loadType;
    int loadStartIndex;
    int loadEndIndex;
    int field_C;
    int field_10;
    int field_14;
};

struct LeaderboardsUnknown {
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
    virtual void FetchLeaderboard(LeaderboardID *leaderboard, bool32 isUser) {}
    virtual void unknown5(void) {}
    virtual void TrackScore(LeaderboardID *leaderboard, int32 score, void (*callback)(bool32 success, int32 rank)) {}
    virtual int32 GetStatus(void) { return 0; }

    const char *currentLeaderboard;
    LeaderboardsUnknown2 unknown2;
    LeaderboardsUnknown entryInfo;
    int32 status   = 0;
    int32 userRank = 0;
    bool32 isUser  = false;
    int32 list     = 0;
    int32 listSize = 0;
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
inline void FetchLeaderboard(LeaderboardID *leaderboard, bool32 isUser) { leaderboards->FetchLeaderboard(leaderboard, isUser); }
inline void TrackScore(LeaderboardID *leaderboard, int32 score, void (*callback)(bool32 success, int32 rank))
{
    leaderboards->TrackScore(leaderboard, score, callback);
}
inline int32 GetLeaderboardsStatus(void) { return leaderboards->GetStatus(); }

Vector2 LeaderboardEntryLength();
Vector2 LeaderboardEntryCount();
void LoadNewLeaderboardEntries(int32 start, uint32 end, int32 type);
void ClearLeaderboardInfo();
LeaderboardEntry *ReadLeaderboardEntry(int entryID);
#endif

} // namespace SKU

} // namespace RSDK

#endif // USER_LEADERBOARDS_H
