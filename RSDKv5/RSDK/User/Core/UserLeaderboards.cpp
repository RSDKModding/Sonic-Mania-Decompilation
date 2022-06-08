#include "RSDK/Core/RetroEngine.hpp"
#include <string>

// ====================
// API Cores
// ====================

namespace RSDK
{
namespace SKU
{
// Dummy API
#if RETRO_USERCORE_DUMMY
#include "RSDK/User/Dummy/DummyLeaderboards.cpp"
#endif

// Steam API
#if RETRO_USERCORE_STEAM
#include "RSDK/User/Steam/SteamLeaderboards.cpp"
#endif

// Epic Games API
#if RETRO_USERCORE_EOS
#include "RSDK/User/EOS/EOSLeaderboards.cpp"
#endif

// Switch API
#if RETRO_USERCORE_NX
#include "RSDK/User/NX/NXLeaderboards.cpp"
#endif
} // namespace SKU
} // namespace RSDK

using namespace RSDK;

#if RETRO_REV02
SKU::UserLeaderboards *RSDK::SKU::leaderboards = NULL;

SKU::LeaderboardAvail RSDK::SKU::LeaderboardEntryLength() { return leaderboards->entryInfo.entryCount; }

SKU::LeaderboardAvail RSDK::SKU::LeaderboardEntryCount() { return leaderboards->entryInfo.entryStart; }

void RSDK::SKU::LoadNewLeaderboardEntries(int32 start, uint32 end, int32 type)
{
    switch (type) {
        default:
            leaderboards->unknown2.loadType       = type;
            leaderboards->unknown2.loadStartIndex = start;
            leaderboards->unknown2.loadEndIndex   = end;
            break;
        case 1:
            leaderboards->unknown2.loadType = type;
            if (!start)
                start = 1;
            leaderboards->unknown2.loadStartIndex = start;
            if (end > 200)
                end = 200;
            leaderboards->unknown2.loadEndIndex = end;
            break;
        case 2: {
            int32 startPos                  = start;
            int32 endPos                    = start + end - 1;
            leaderboards->unknown2.loadType = 2;
            if (start < (endPos - 199))
                startPos = endPos - 199;
            leaderboards->unknown2.loadStartIndex = startPos;
            leaderboards->unknown2.loadEndIndex   = start + end - startPos;
            break;
        }
    }
}

void RSDK::SKU::ClearLeaderboardInfo()
{
    leaderboards->status             = STATUS_NONE;
    leaderboards->currentLeaderboard = NULL;
    // ClearLeaderboardsUnknown2(&leaderboards->unknown2);
    // SetupLeaderboardEntries(&leaderboards->unknown);
}

SKU::LeaderboardEntry *RSDK::SKU::ReadLeaderboardEntry(int32 entryID)
{
    if (entryID < leaderboards->entryInfo.entryStart.start
        || entryID >= leaderboards->entryInfo.entryStart.start + leaderboards->entryInfo.entryStart.length)
        return NULL;
    else
        return &leaderboards->entryInfo.entries[entryID - leaderboards->entryInfo.entryStart.start];
}
#endif
