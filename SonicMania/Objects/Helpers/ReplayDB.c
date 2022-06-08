// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: ReplayDB Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

#if MANIA_USE_PLUS
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
    compressed[1]    = framePtr->changedValues;
    bool32 forcePack = framePtr->info == REPLAY_INFO_STATECHANGE || framePtr->info == REPLAY_INFO_PASSEDGATE;
    uint8 changes    = framePtr->changedValues;

    uint8 *compressedBuffer = &compressed[2];

    // input
    if (forcePack || (changes & REPLAY_CHANGED_INPUT)) {
        *compressedBuffer = framePtr->inputs;
        ++compressedBuffer;
    }

    // position
    if (forcePack || (changes & REPLAY_CHANGED_POS)) {
        *((int32 *)compressedBuffer) = framePtr->position.x;
        compressedBuffer += sizeof(int32);

        *((int32 *)compressedBuffer) = framePtr->position.y;
        compressedBuffer += sizeof(int32);
    }

    // velocity
    if (forcePack || (changes & REPLAY_CHANGED_VEL)) {
        *((int32 *)compressedBuffer) = framePtr->velocity.x;
        compressedBuffer += sizeof(int32);

        *((int32 *)compressedBuffer) = framePtr->velocity.y;
        compressedBuffer += sizeof(int32);
    }

    // rotation
    if (forcePack || (changes & REPLAY_CHANGED_ROT)) {
        *compressedBuffer = framePtr->rotation >> 1;
        compressedBuffer += sizeof(uint8);
    }

    // direction
    if (forcePack || (changes & REPLAY_CHANGED_DIR)) {
        *compressedBuffer = framePtr->direction;
        ++compressedBuffer;
    }

    // anim
    if (forcePack || (changes & REPLAY_CHANGED_ANIM)) {
        *compressedBuffer = framePtr->anim;
        ++compressedBuffer;
    }

    // frame
    if (forcePack || (changes & REPLAY_CHANGED_FRAME)) {
        *compressedBuffer = framePtr->frame;
        ++compressedBuffer;
    }

    return (int32)(compressedBuffer - compressed);
}

int32 ReplayDB_Buffer_UnpackEntry(void *uncompressed, uint8 *compressed)
{
    ReplayFrame *framePtr = (ReplayFrame *)uncompressed;

    // compress state
    framePtr->info = compressed[0];

    bool32 forceUnpack      = *compressed == REPLAY_INFO_STATECHANGE || *compressed == REPLAY_INFO_PASSEDGATE;
    uint8 changes           = compressed[1];
    framePtr->changedValues = changes;

    uint8 *compressedBuffer = &compressed[2];

    // input
    if (forceUnpack || (changes & REPLAY_CHANGED_INPUT)) {
        framePtr->inputs = *compressedBuffer++;
    }

    // position
    if (forceUnpack || (changes & REPLAY_CHANGED_POS)) {
        int32 x = *(int32 *)compressedBuffer;
        compressedBuffer += sizeof(int32);

        int32 y = *(int32 *)compressedBuffer;
        compressedBuffer += sizeof(int32);

        framePtr->position.x = x;
        framePtr->position.y = y;
    }

    // velocity
    if (forceUnpack || (changes & REPLAY_CHANGED_VEL)) {
        int32 x = *(int32 *)compressedBuffer;
        compressedBuffer += sizeof(int32);

        int32 y = *(int32 *)compressedBuffer;
        compressedBuffer += sizeof(int32);

        framePtr->velocity.x = x;
        framePtr->velocity.y = y;
    }

    // rotation
    if (forceUnpack || (changes & REPLAY_CHANGED_ROT)) {
        int32 rotation     = *compressedBuffer++;
        framePtr->rotation = rotation << 1;
    }

    // direction
    if (forceUnpack || (changes & REPLAY_CHANGED_DIR)) {
        framePtr->direction = *compressedBuffer++;
    }

    // anim
    if (forceUnpack || (changes & REPLAY_CHANGED_ANIM)) {
        framePtr->anim = *compressedBuffer++;
    }

    // frame
    if (forceUnpack || (changes & REPLAY_CHANGED_FRAME)) {
        framePtr->frame = *compressedBuffer++;
    }

    return (int32)(compressedBuffer - compressed);
}

#if RETRO_INCLUDE_EDITOR
void ReplayDB_EditorDraw(void) {}

void ReplayDB_EditorLoad(void) {}
#endif

void ReplayDB_Serialize(void) {}
#endif
