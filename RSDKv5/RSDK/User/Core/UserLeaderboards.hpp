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
enum LeaderboardLoadTypes {
    LEADERBOARD_LOAD_INIT,
    LEADERBOARD_LOAD_PREV,
    LEADERBOARD_LOAD_NEXT,
};

struct UserLeaderboards;
struct LeaderboardLoadList;

struct LeaderboardLoadInfo
{
    LeaderboardLoadList *parent;
    int32 status;
    LeaderboardAvail avail;
    LeaderboardEntry entries[20];
    uint8 disableLoadPrev;
    uint8 disableLoadNext;
    LeaderboardLoadInfo *prev;
    LeaderboardLoadInfo *next;
    int32 unused;
};

struct LeaderboardLoadList {
    LeaderboardLoadInfo *InitLoadInfo();
    void Clear();
    void AddLoadInfoPrev(LeaderboardLoadInfo *info);
    void AddLoadInfoNext(LeaderboardLoadInfo *info);
    void RemoveLoadInfoPrev();
    void RemoveLoadInfoNext();

    UserLeaderboards *parent;
    void *unused;
    int32 count;
    LeaderboardLoadInfo *prev;
    LeaderboardLoadInfo *next;
    LeaderboardLoadInfo *last;
};

struct LeaderboardEntryInfo {
    void Setup();
    void HandleTimers();
    void LoadLeaderboardEntries(int32 start, uint32 length, int32 type);

    UserLeaderboards *parent;
    LeaderboardLoadList *loadList;
    LeaderboardLoadTypes loadType;
    LeaderboardAvail loadSize;
    LeaderboardAvail viewSize;
    LeaderboardEntry *entries[200];
};

// This is the base struct, it serves as the base for any API-specific stats
// This struct should never be removed
struct UserLeaderboards {
    UserLeaderboards()
    {
        memset(&loadList, 0, sizeof(loadList));
        memset(&entryInfo, 0, sizeof(entryInfo));

        loadList.parent       = this;
        entryInfo.loadList = &loadList;
        entryInfo.parent      = this;
    }

    virtual void StageLoad()
    {
        this->currentLeaderboard = NULL;
        this->status             = STATUS_NONE;

        this->loadList.prev       = NULL;
        this->loadList.next       = NULL;
        this->loadList.last       = NULL;
        this->loadList.count      = 0;
        this->loadList.unused     = 0;

        this->entryInfo.Setup();
    }
    virtual void FrameInit() { entryInfo.HandleTimers(); }
    virtual void OnUnknownEvent() {}
#if RETRO_VER_EGS
    virtual bool32 CheckLeaderboardsEnabled() { return true; }
#endif
    virtual int32 InitLeaderboards() { return 0; }
    virtual void FetchLeaderboard(LeaderboardID *leaderboard, bool32 isUser) {}
    virtual void LoadLeaderboards(LeaderboardLoadInfo *info) {}
    virtual void TrackScore(LeaderboardID *leaderboard, int32 score, void (*callback)(bool32 success, int32 rank)) {}
    virtual int32 GetStatus() { return this->status; }

    LeaderboardID *currentLeaderboard;
    LeaderboardLoadList loadList;
    LeaderboardEntryInfo entryInfo;
    int32 status   = STATUS_NONE;
    int32 userRank = 0;
    bool32 isUser  = false;
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

inline LeaderboardAvail LeaderboardEntryViewSize() { return leaderboards->entryInfo.viewSize; }
inline LeaderboardAvail LeaderboardEntryLoadSize() { return leaderboards->entryInfo.loadSize; }
inline void LoadLeaderboardEntries(int32 start, uint32 length, int32 type)
{
    leaderboards->entryInfo.LoadLeaderboardEntries(start, length, type);
}
void ResetLeaderboardInfo();
LeaderboardEntry *ReadLeaderboardEntry(int32 entryID);
#endif

} // namespace SKU

} // namespace RSDK

#endif // USER_LEADERBOARDS_H
