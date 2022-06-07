#include "RSDK/Core/RetroEngine.hpp"
#include <string>

// ====================
// API Cores
// ====================

namespace RSDK::SKU
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

} // namespace RSDK::SKU

using namespace RSDK;

#if RETRO_REV02
SKU::UserLeaderboards *RSDK::SKU::leaderboards = NULL;

RSDK::SKU::LeaderboardLoadInfo *RSDK::SKU::LeaderboardLoadList::InitLoadInfo()
{
    LeaderboardLoadInfo *info = NULL;
    AllocateStorage(sizeof(LeaderboardLoadInfo), (void **)&info, DATASET_TMP, true);
    memset(info, 0, sizeof(LeaderboardLoadInfo));
    info->parent = this;
    return info;
}

void RSDK::SKU::LeaderboardLoadList::Clear()
{
    while (true) {
        LeaderboardLoadInfo *current = this->next;
        if (!current)
            break;

        LeaderboardLoadInfo *next = current->next;
        if (next)
            next->prev = NULL;
        this->next = next;

        if (current == this->prev)
            this->prev = NULL;

        --this->count;

        RemoveStorageEntry((void **)&current);
    }

    this->prev   = NULL;
    this->next   = NULL;
    this->last   = NULL;
    this->count  = 0;
    this->unused = 0;
}

void RSDK::SKU::LeaderboardLoadList::AddLoadInfoPrev(LeaderboardLoadInfo *info)
{
    LeaderboardLoadInfo *next = this->next;
    if (next) {
        if (this->count != 10) {
            next->prev = info;
            info->next = next;
            ++this->count;
            this->next = info;
        }
    }
    else {
        Clear();

        this->count = 1;
        this->prev  = info;
        this->next  = info;
        this->last  = info;
        if (info->avail.start == 1)
            info->disableLoadPrev = true;
    }
}

void RSDK::SKU::LeaderboardLoadList::AddLoadInfoNext(LeaderboardLoadInfo *info)
{
    LeaderboardLoadInfo *last = this->last;
    if (last) {
        if (this->count != 10) {
            last->next = info;
            info->prev = last;
            ++this->count;
            this->last = info;
        }
    }
    else {
        Clear();

        this->count = 1;
        this->prev  = info;
        this->next  = info;
        this->last  = info;

        if (info->avail.start == 1)
            info->disableLoadPrev = true;
    }
}

void RSDK::SKU::LeaderboardLoadList::RemoveLoadInfoPrev()
{
    LeaderboardLoadInfo *last = this->last;

    if (this->last) {
        LeaderboardLoadInfo *current = last->prev;
        if (current)
            current->next = NULL;

        this->last = current;

        if (last == this->prev)
            this->prev = NULL;

        --this->count;
    }
}
void RSDK::SKU::LeaderboardLoadList::RemoveLoadInfoNext()
{
    LeaderboardLoadInfo *next = this->next;

    if (next) {
        LeaderboardLoadInfo *current = next->next;
        if (current)
            current->prev = NULL;
        this->next = current;

        if (next == this->prev)
            this->prev = NULL;

        --this->count;
    }
}

void RSDK::SKU::LeaderboardEntryInfo::Setup()
{
    this->viewSize.start  = 0;
    this->viewSize.length = 0;
    memset(this->entries, 0, sizeof(this->entries));

    LeaderboardLoadInfo *info = this->loadList->next;
    int32 count               = 0;
    if (info) {
        this->viewSize.start = info->avail.start;

        while (info) {
            LeaderboardEntry **entryList = &entries[count];
            LeaderboardEntry *entry      = info->entries;

            for (int32 e = 0; e < info->avail.length; ++e) {
                *entryList++ = &entry[e];
                ++count;
            }

            info = info->next;
        }
    }

    this->viewSize.length = count;
}

void RSDK::SKU::LeaderboardEntryInfo::HandleTimers()
{
    UserLeaderboards *leaderboards = this->parent;
    if (leaderboards->currentLeaderboard && this->loadType) {
        LeaderboardLoadInfo *next = this->loadList->next;
        LeaderboardLoadInfo *last = this->loadList->last;

        if (next && last) {
            switch (this->loadType) {
                default:
                case LEADERBOARD_LOAD_INIT:
                case LEADERBOARD_LOAD_NEXT:
                    if ((this->loadSize.start + this->loadSize.length) != (last->avail.start + last->avail.length) && !next->disableLoadNext
                        && last->status == STATUS_OK) {
                        if (this->loadList->count == 10) {
                            this->loadList->RemoveLoadInfoNext();
                            RemoveStorageEntry((void **)&next);
                        }
                        LeaderboardLoadInfo *info = this->loadList->InitLoadInfo();
                        info->avail.start         = last->avail.length + last->avail.start;
                        info->avail.length        = 20;
                        this->loadList->AddLoadInfoNext(info);
                        leaderboards->LoadLeaderboards(info);
                        Setup();
                    }
                    break;

                case LEADERBOARD_LOAD_PREV:
                    if (next->avail.start != this->loadSize.start && !next->disableLoadPrev && next->status == STATUS_OK) {
                        if (this->loadList->count == 10) {
                            this->loadList->RemoveLoadInfoPrev();
                            RemoveStorageEntry((void **)&last);
                        }
                        LeaderboardLoadInfo *info = this->loadList->InitLoadInfo();

                        info->avail.start = next->avail.start;
                        if (info->avail.start > 20)
                            info->avail.start -= 20;
                        else
                            info->avail.start = 1;

                        info->avail.length = next->avail.start - info->avail.start;

                        if (info->avail.start == 1)
                            info->disableLoadPrev = true;

                        this->loadList->AddLoadInfoPrev(info);
                        leaderboards->LoadLeaderboards(info);
                        Setup();
                    }
                    break;
            }
        }
    }
}

void RSDK::SKU::LeaderboardEntryInfo::LoadLeaderboardEntries(int32 start, uint32 length, int32 type)
{
    switch (type) {
        default:
        case LEADERBOARD_LOAD_INIT:
            loadType = (LeaderboardLoadTypes)type;

            loadSize.start  = start;
            loadSize.length = length;
            break;

        case LEADERBOARD_LOAD_PREV:
            loadType = (LeaderboardLoadTypes)type;

            loadSize.start  = maxVal(start, 1);
            loadSize.length = minVal(length, 200);
            break;

        case LEADERBOARD_LOAD_NEXT: {
            loadType = (LeaderboardLoadTypes)type;

            int32 startPos = start;
            int32 endPos   = start + length;
            if (length - start < 200)
                startPos = endPos - 200;

            loadSize.start  = startPos;
            loadSize.length = endPos - startPos;
            break;
        }
    }
}

void RSDK::SKU::ResetLeaderboardInfo()
{
    leaderboards->status             = STATUS_NONE;
    leaderboards->currentLeaderboard = NULL;

    leaderboards->loadList.Clear();
    leaderboards->entryInfo.Setup();
}

SKU::LeaderboardEntry *RSDK::SKU::ReadLeaderboardEntry(int32 entryID)
{
    if (entryID >= leaderboards->entryInfo.viewSize.start
        && entryID < leaderboards->entryInfo.viewSize.start + leaderboards->entryInfo.viewSize.length)
        return leaderboards->entryInfo.entries[entryID - leaderboards->entryInfo.viewSize.start];

    return NULL;
}
#endif
