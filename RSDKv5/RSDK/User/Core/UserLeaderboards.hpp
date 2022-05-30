#ifndef USER_LEADERBOARDS_H
#define USER_LEADERBOARDS_H

namespace RSDK
{

namespace SKU
{

struct LeaderboardID {
    int32 unknownID1;     // leaderboard id (unknown platform)
    int32 unknownID2;     // leaderboard id (unknown platform)
    int32 switchID;       // leaderboard id (switch)
    const char *xboxName; // XBox One Leaderboard name (making an assumption based on the MS docs)
    const char *pcName;   // Plus Leaderboard name
};

struct LeaderboardAvail {
    int32 start;
    int32 length;
};

struct LeaderboardEntry {
    String username;
#if RETRO_REV02
    String userID;
#endif
    int32 globalRank;
    int32 score;
    bool32 isUser;
    int32 status;
};

#if RETRO_REV02
struct LeaderboardsUnknown2 {
    int32 loadType;
    int32 loadStartIndex;
    int32 loadEndIndex;
    int32 field_C;
    int32 field_10;
    int32 field_14;
};

struct LeaderboardsUnknown {
    void *parent;
    int32 loadStatus;
    int32 globalRankOffset;
    LeaderboardAvail entryCount;
    LeaderboardAvail entryStart;
    LeaderboardEntry entries[25];
};

// This is the base struct, it serves as the base for any API-specific stats
// This struct should never be removed
struct UserLeaderboards {
    virtual void StageLoad() {}
    virtual void FrameInit() {}
    virtual void OnUnknownEvent() {}
    virtual int32 unknown4() { return 0; }
#if RETRO_VER_EGS
    virtual int32 unknown6() { return 0; }
#endif
    virtual void FetchLeaderboard(LeaderboardID *leaderboard, bool32 isUser) {}
    virtual void unknown5() {}
    virtual void TrackScore(LeaderboardID *leaderboard, int32 score, void (*callback)(bool32 success, int32 rank)) {}
    virtual int32 GetStatus() { return 0; }

    LeaderboardID *currentLeaderboard;
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
    int32 score;
};

extern std::vector<LeaderboardInfo> leaderboardList;

// End custom leaderboard code

#if RETRO_REV02
extern UserLeaderboards *leaderboards;

// ====================
// API Cores
// ====================

// Dummy API
#if RETRO_USERCORE_DUMMY
#include "RSDK/User/Dummy/DummyLeaderboards.hpp"
#endif

// Steam API
#if RETRO_USERCORE_STEAM
#include "RSDK/User/Steam/SteamLeaderboards.hpp"
#endif

// Epic Games API
#if RETRO_USERCORE_EOS
#include "RSDK/User/EOS/EOSLeaderboards.hpp"
#endif

// Switch API
#if RETRO_USERCORE_NX
#include "RSDK/User/NX/NXLeaderboards.hpp"
#endif

inline int32 leaderboardsUnknown4() { return leaderboards->unknown4(); }
#if RETRO_VER_EGS
inline int32 leaderboardsUnknown6() { return leaderboards->unknown6(); }
#endif
inline void FetchLeaderboard(LeaderboardID *leaderboard, bool32 isUser) { leaderboards->FetchLeaderboard(leaderboard, isUser); }
inline void TrackScore(LeaderboardID *leaderboard, int32 score, void (*callback)(bool32 success, int32 rank))
{
    leaderboards->TrackScore(leaderboard, score, callback);
}
inline int32 GetLeaderboardsStatus() { return leaderboards->GetStatus(); }

LeaderboardAvail LeaderboardEntryLength();
LeaderboardAvail LeaderboardEntryCount();
void LoadNewLeaderboardEntries(int32 start, uint32 end, int32 type);
void ClearLeaderboardInfo();
LeaderboardEntry *ReadLeaderboardEntry(int32 entryID);
#endif

} // namespace SKU

} // namespace RSDK

#endif // USER_LEADERBOARDS_H
