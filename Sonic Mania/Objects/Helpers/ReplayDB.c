// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: ReplayDB Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectReplayDB *ReplayDB;

void ReplayDB_Update(void) {}

void ReplayDB_LateUpdate(void) {}

void ReplayDB_StaticUpdate(void) {}

void ReplayDB_Draw(void) {}

void ReplayDB_Create(void *data) {}

void ReplayDB_StageLoad(void) {}

int32 ReplayDB_Buffer_PackEntry(uint8 *compressed, void *uncompressed)
{
    ReplayFrame *framePtr = (ReplayFrame *)uncompressed;

    compressed[0]    = framePtr->info;
    compressed[1]    = framePtr->flags;
    bool32 forcePack = framePtr->info == REPLAY_INFO_STATECHANGE || framePtr->info == REPLAY_INFO_PASSEDGATE;
    uint8 changes    = framePtr->flags;

    int32 size    = 2 * sizeof(uint8);
    uint8 *outPtr = &compressed[2];

    // input
    if (forcePack || (changes & REPLAY_FLAG_INPUT)) {
        *outPtr = framePtr->inputs;
        ++size;
        ++outPtr;
    }

    // position
    if (forcePack || (changes & REPLAY_FLAG_POS)) {
        *((int32 *)outPtr) = framePtr->position.x;
        size += sizeof(int32);
        outPtr += sizeof(int32);

        *((int32 *)outPtr) = framePtr->position.y;
        size += sizeof(int32);
        outPtr += sizeof(int32);
    }

    // velocity
    if (forcePack || (changes & REPLAY_FLAG_VEL)) {
        *((int32 *)outPtr) = framePtr->velocity.x;
        size += sizeof(int32);
        outPtr += sizeof(int32);

        *((int32 *)outPtr) = framePtr->velocity.y;
        size += sizeof(int32);
        outPtr += sizeof(int32);
    }

    // rotation
    if (forcePack || (changes & REPLAY_FLAG_ROT)) {
        *outPtr = framePtr->rotation >> 1;
        size += sizeof(uint8);
        outPtr += sizeof(uint8);
    }

    // direction
    if (forcePack || (changes & REPLAY_FLAG_DIR)) {
        *outPtr = framePtr->dir;
        ++size;
        ++outPtr;
    }

    // anim
    if (forcePack || (changes & REPLAY_FLAG_ANIM)) {
        *outPtr = framePtr->anim;
        ++size;
        ++outPtr;
    }

    // frame
    if (forcePack || (changes & REPLAY_FLAG_FRAME)) {
        *outPtr = framePtr->frame;
        ++size;
        ++outPtr;
    }

    return outPtr - compressed;
}

int32 ReplayDB_Buffer_UnpackEntry(void *uncompressed, uint8 *compressed)
{
    ReplayFrame *framePtr = (ReplayFrame *)uncompressed;

    // compress state
    framePtr->info = compressed[0];

    bool32 forceUnpack = *compressed == REPLAY_INFO_STATECHANGE || *compressed == REPLAY_INFO_PASSEDGATE;
    uint8 changes      = compressed[1];
    framePtr->flags    = changes;

    int32 size = 2 * sizeof(uint8);
    uint8 *buf = &compressed[2];

    // input
    if (forceUnpack || (changes & REPLAY_FLAG_INPUT)) {
        framePtr->inputs = *buf++;
        size += sizeof(uint8);
    }

    // position
    if (forceUnpack || (changes & REPLAY_FLAG_POS)) {
        int32 x = *(int32 *)buf;
        buf += sizeof(int32);
        size += sizeof(int32);

        int32 y = *(int32 *)buf;
        buf += sizeof(int32);
        size += sizeof(int32);

        framePtr->position.x = x;
        framePtr->position.y = y;
    }

    // velocity
    if (forceUnpack || (changes & REPLAY_FLAG_VEL)) {
        int32 x = *(int32 *)buf;
        buf += sizeof(int32);
        size += sizeof(int32);

        int32 y = *(int32 *)buf;
        buf += sizeof(int32);
        size += sizeof(int32);

        framePtr->velocity.x = x;
        framePtr->velocity.y = y;
    }

    // rotation
    if (forceUnpack || (changes & REPLAY_FLAG_ROT)) {
        int32 rotation     = *buf++;
        framePtr->rotation = rotation << 1;
        size += sizeof(uint8);
    }

    // direction
    if (forceUnpack || (changes & REPLAY_FLAG_DIR)) {
        framePtr->dir = *buf++;
        size += sizeof(uint8);
    }

    // anim
    if (forceUnpack || (changes & REPLAY_FLAG_ANIM)) {
        framePtr->anim = *buf++;
        size += sizeof(uint8);
    }

    // frame
    if (forceUnpack || (changes & REPLAY_FLAG_FRAME)) {
        framePtr->frame = *buf++;
        size += sizeof(uint8);
    }

    return buf - compressed;
}

#if RETRO_INCLUDE_EDITOR
void ReplayDB_EditorDraw(void) {}

void ReplayDB_EditorLoad(void) {}
#endif

void ReplayDB_Serialize(void) {}
#endif
