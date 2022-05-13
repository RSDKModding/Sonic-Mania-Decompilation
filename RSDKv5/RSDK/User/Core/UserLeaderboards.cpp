#include "RSDK/Core/RetroEngine.hpp"
#include <string>

#if RETRO_REV02
RSDK::SKU::UserLeaderboards *RSDK::SKU::leaderboards = NULL;

RSDK::SKU::LeaderboardAvail RSDK::SKU::LeaderboardEntryLength() { return leaderboards->entryInfo.entryCount; }

RSDK::SKU::LeaderboardAvail RSDK::SKU::LeaderboardEntryCount() { return leaderboards->entryInfo.entryStart; }

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

RSDK::SKU::LeaderboardEntry *RSDK::SKU::ReadLeaderboardEntry(int entryID)
{
    if (entryID < leaderboards->entryInfo.entryStart.start
        || entryID >= leaderboards->entryInfo.entryStart.start + leaderboards->entryInfo.entryStart.length)
        return NULL;
    else
        return &leaderboards->entryInfo.entries[entryID - leaderboards->entryInfo.entryStart.start];
}
#endif
