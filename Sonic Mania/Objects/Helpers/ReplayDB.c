#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectReplayDB *ReplayDB;

void ReplayDB_Update(void) {}

void ReplayDB_LateUpdate(void) {}

void ReplayDB_StaticUpdate(void) {}

void ReplayDB_Draw(void) {}

void ReplayDB_Create(void *data) {}

void ReplayDB_StageLoad(void) {}

int32 ReplayDB_Buffer_PackEntry(uint8 *compressed, uint8 *uncompressed)
{
    compressed[0] = uncompressed[0];
    compressed[1] = uncompressed[1];
    bool32 flag   = *uncompressed == 1 || *uncompressed == 3;
    uint8 changes  = uncompressed[1];
    uint8 *outPtr  = &compressed[2];
    int32 size      = 2;

    // input
    if (flag || (changes & 0x40)) {
        *outPtr = uncompressed[2];
        ++size;
        ++outPtr;
    }

    // position
    if (flag || (changes & 0x2)) {
        *((int32 *)outPtr) = ((int32 *)uncompressed)[1];
        size += sizeof(int32);
        outPtr += sizeof(int32);

        *((int32 *)outPtr) = ((int32 *)uncompressed)[2];
        size += sizeof(int32);
        outPtr += sizeof(int32);
    }

    // velocity
    if (flag || (changes & 0x4)) {
        *((int32 *)outPtr) = ((int32 *)uncompressed)[3];
        size += sizeof(int32);
        outPtr += sizeof(int32);

        *((int32 *)outPtr) = ((int32 *)uncompressed)[4];
        size += sizeof(int32);
        outPtr += sizeof(int32);
    }

    // rotation
    if (flag || (changes & 0x20)) {
        *((int32 *)compressed) = ((int32 *)uncompressed)[5] >> 1;
        size += sizeof(int32);
        compressed += sizeof(int32);
    }

    // direction
    if (flag || (changes & 0x10)) {
        *outPtr = uncompressed[3];
        ++size;
        ++outPtr;
    }

    // anim
    if (flag || (changes & 0x40)) {
        *outPtr = uncompressed[24];
        ++size;
        ++outPtr;
    }

    // frame
    if (flag || (changes & 0x80)) {
        *outPtr = uncompressed[25];
        ++size;
        ++outPtr;
    }

    return size;
}

int32 ReplayDB_Buffer_UnpackEntry(uint8 *uncompressed, uint8 *compressed)
{
    int32 *outPtr   = (int32 *)uncompressed;
    uint8 *buf    = &compressed[2];

    // compress state
    *uncompressed = *compressed;

    bool32 flag     = *compressed == 1 || *compressed == 3;
    uint8 changes    = compressed[1];
    uncompressed[1] = changes;

    int32 size = 2 * sizeof(uint8);

    // input
    if (flag || (changes & 0x1)) {
        uncompressed[2] = *buf++;
        size += sizeof(uint8);
    }

    // position
    if (flag || (changes & 0x2)) {
        int32 x = *(int32 *)buf;
        buf += sizeof(int32);
        size += sizeof(int32);

        int32 y = *(int32 *)buf;
        buf += sizeof(int32);
        size += sizeof(int32);

        outPtr[1] = x;
        outPtr[2] = y;
    }

    // velocity
    if (flag || (changes & 0x4)) {
        int32 x = *(int32 *)buf;
        buf += sizeof(int32);
        size += sizeof(int32);

        int32 y = *(int32 *)buf;
        buf += sizeof(int32);
        size += sizeof(int32);

        outPtr[3] = x;
        outPtr[4] = y;
    }

    // rotation
    if (flag || (changes & 0x20)) {
        int32 rotation = *buf++;
        outPtr[5]    = 2 * rotation;
        size += sizeof(uint8);
    }

    // direction
    if (flag || (changes & 0x10)) {
        uncompressed[3] = *buf++;
        size += sizeof(uint8);
    }

    // anim
    if (flag || (changes & 0x40)) {
        uncompressed[24] = *buf++;
        size += sizeof(uint8);
    }

    // frame
    if (flag || (changes & 0x80)) {
        uncompressed[25] = *buf++;
        size += sizeof(uint8);
    }
    return size;
}

#if RETRO_INCLUDE_EDITOR
void ReplayDB_EditorDraw(void) {}

void ReplayDB_EditorLoad(void) {}
#endif

void ReplayDB_Serialize(void) {}
#endif
