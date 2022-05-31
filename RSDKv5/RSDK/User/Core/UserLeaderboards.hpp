#ifndef USER_LEADERBOARDS_H
#define USER_LEADERBOARDS_H

namespace RSDK
{

namespace SKU
{

struct LeaderboardID {
    int32 idPS4;        // leaderboard id (PS4)
    int32 idUnknown;    // leaderboard id (unknown platform)
    int32 idSwitch;     // leaderboard id (switch)
    const char *idXbox; // Xbox One Leaderboard id (making an assumption based on the MS docs)
    const char *idPC;   // Leaderboard id (as a string, used for PC platforms)
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
    virtual void StageLoad()
    {
        currentLeaderboard = 0;
        // callback           = 0;
        // field_18           = 0;
        // downloadCallback   = 0;
        // end                = 0;
        // start              = 0;
        status             = STATUS_NONE;
        // ClearUnknown1(&unknown);
    }
    virtual void FrameInit() {}
    virtual void OnUnknownEvent() {}
#if RETRO_VER_EGS
    virtual bool32 CheckLeaderboardsEnabled() { return true; }
#endif
    virtual int32 InitLeaderboards() { return 0; }
    virtual void FetchLeaderboard(LeaderboardID *leaderboard, bool32 isUser) {}
    virtual void LoadLeaderboards(void *info) {}
    virtual void TrackScore(LeaderboardID *leaderboard, int32 score, void (*callback)(bool32 success, int32 rank)) {}
    virtual int32 GetStatus() { return status; }

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

inline void InitLeaderboards() { leaderboards->InitLeaderboards(); }
#if RETRO_VER_EGS
inline bool32 CheckLeaderboardsEnabled() { return leaderboards->CheckLeaderboardsEnabled(); }
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
