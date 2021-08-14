#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectReplayDB *ReplayDB;

void ReplayDB_Update(void) {}

void ReplayDB_LateUpdate(void) {}

void ReplayDB_StaticUpdate(void) {}

void ReplayDB_Draw(void) {}

void ReplayDB_Create(void *data) {}

void ReplayDB_StageLoad(void) {}

int ReplayDB_Buffer_PackEntry(byte *compressed, byte *uncompressed)
{
    compressed[0] = uncompressed[0];
    compressed[1] = uncompressed[1];
    bool32 flag   = *uncompressed == 1 || *uncompressed == 3;
    byte changes  = uncompressed[1];
    byte *outPtr  = &compressed[2];
    int size      = 2;

    // input
    if (flag || (changes & 0x40)) {
        *outPtr = uncompressed[2];
        ++size;
        ++outPtr;
    }

    // position
    if (flag || (changes & 0x2)) {
        *((int *)outPtr) = ((int *)uncompressed)[1];
        size += sizeof(int);
        outPtr += sizeof(int);

        *((int *)outPtr) = ((int *)uncompressed)[2];
        size += sizeof(int);
        outPtr += sizeof(int);
    }

    // velocity
    if (flag || (changes & 0x4)) {
        *((int *)outPtr) = ((int *)uncompressed)[3];
        size += sizeof(int);
        outPtr += sizeof(int);

        *((int *)outPtr) = ((int *)uncompressed)[4];
        size += sizeof(int);
        outPtr += sizeof(int);
    }

    // rotation
    if (flag || (changes & 0x20)) {
        *((int *)compressed) = ((int *)uncompressed)[5] >> 1;
        size += sizeof(int);
        compressed += sizeof(int);
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

int ReplayDB_Buffer_UnpackEntry(byte *uncompressed, byte *compressed)
{
    int *outPtr   = (int *)uncompressed;
    byte *buf    = &compressed[2];

    // compress state
    *uncompressed = *compressed;

    bool32 flag     = *compressed == 1 || *compressed == 3;
    byte changes    = compressed[1];
    uncompressed[1] = changes;

    int size = 2 * sizeof(byte);

    // input
    if (flag || (changes & 0x1)) {
        uncompressed[2] = *buf++;
        size += sizeof(byte);
    }

    // position
    if (flag || (changes & 0x2)) {
        int x = *(int *)buf;
        buf += sizeof(int);
        size += sizeof(int);

        int y = *(int *)buf;
        buf += sizeof(int);
        size += sizeof(int);

        outPtr[1] = x;
        outPtr[2] = y;
    }

    // velocity
    if (flag || (changes & 0x4)) {
        int x = *(int *)buf;
        buf += sizeof(int);
        size += sizeof(int);

        int y = *(int *)buf;
        buf += sizeof(int);
        size += sizeof(int);

        outPtr[3] = x;
        outPtr[4] = y;
    }

    // rotation
    if (flag || (changes & 0x20)) {
        int rotation = *buf++;
        outPtr[5]    = 2 * rotation;
        size += sizeof(byte);
    }

    // direction
    if (flag || (changes & 0x10)) {
        uncompressed[3] = *buf++;
        size += sizeof(byte);
    }

    // anim
    if (flag || (changes & 0x40)) {
        uncompressed[24] = *buf++;
        size += sizeof(byte);
    }

    // frame
    if (flag || (changes & 0x80)) {
        uncompressed[25] = *buf++;
        size += sizeof(byte);
    }
    return size;
}

void ReplayDB_EditorDraw(void) {}

void ReplayDB_EditorLoad(void) {}

void ReplayDB_Serialize(void) {}
#endif
