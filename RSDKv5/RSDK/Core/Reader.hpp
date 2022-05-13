#ifndef READER_H
#define READER_H

#if RETRO_RENDERDEVICE_SDL2 || RETRO_AUDIODEVICE_SDL2 || RETRO_INPUTDEVICE_SDL2
#define FileIO SDL_RWops
#if RETRO_PLATFORM != RETRO_ANDROID
#define fOpen(path, mode) SDL_RWFromFile(path, mode)
#else
SDL_RWops *fOpen(const char *path, const char *mode);
#endif
#define fRead(buffer, elementSize, elementCount, file)  SDL_RWread(file, buffer, elementSize, elementCount)
#define fSeek(file, offset, whence)                     SDL_RWseek(file, offset, whence)
#define fTell(file)                                     SDL_RWtell(file)
#define fClose(file)                                    SDL_RWclose(file)
#define fWrite(buffer, elementSize, elementCount, file) SDL_RWwrite(file, buffer, elementSize, elementCount)
#else
#define FileIO                                          FILE
#define fOpen(path, mode)                               fopen(path, mode)
#define fRead(buffer, elementSize, elementCount, file)  fread(buffer, elementSize, elementCount, file)
#define fSeek(file, offset, whence)                     fseek(file, offset, whence)
#define fTell(file)                                     ftell(file)
#define fClose(file)                                    fclose(file)
#define fWrite(buffer, elementSize, elementCount, file) fwrite(buffer, elementSize, elementCount, file)
#endif

#if RETRO_PLATFORM == RETRO_ANDROID
#include <zlib.h>
#else
#include <zlib/zlib.h>
#endif

#define FILE_COUNT (0x1000)
#define PACK_COUNT (4)

enum Scopes {
    SCOPE_NONE,
    SCOPE_GLOBAL,
    SCOPE_STAGE,
};

struct FileInfo {
    int32 fileSize;
    int32 externalFile;
    FileIO *file;
    uint8 *fileData;
    int32 readPos;
    int32 fileOffset;
    uint8 usingFileBuffer;
    uint8 encrypted;
    uint8 eNybbleSwap;
    uint8 encryptionKeyA[0x10];
    uint8 encryptionKeyB[0x10];
    uint8 eKeyPosA;
    uint8 eKeyPosB;
    uint8 eKeyNo;
};

struct RSDKFileInfo {
    RETRO_HASH_MD5(hash);
    int32 size;
    int32 offset;
    uint8 encrypted;
    uint8 useFileBuffer;
    int32 packID;
};

struct RSDKContainer {
    char name[0x100];
    uint8 *dataPtr;
    int32 fileCount;
};

extern RSDKFileInfo dataFiles[FILE_COUNT];
extern RSDKContainer dataPacks[PACK_COUNT];

extern uint8 dataPackCount;
extern uint16 dataFileCount;

extern char gameLogicName[0x200];

extern bool32 useDataFile;

bool32 CheckDataFile(const char *filename, size_t fileOffset, bool32 useBuffer);
bool32 OpenDataFile(FileInfo *info, const char *filename);

enum FileModes { FMODE_NONE, FMODE_RB, FMODE_WB, FMODE_RB_PLUS, FMODE_4 };

static const char *openModes[3] = { "rb", "wb", "rb+" };

inline void InitFileInfo(FileInfo *info)
{
    info->file            = NULL;
    info->fileSize        = 0;
    info->externalFile    = false;
    info->usingFileBuffer = false;
    info->encrypted       = false;
    info->readPos         = 0;
    info->fileOffset      = 0;
}

bool32 LoadFile(FileInfo *info, const char *filename, uint8 fileMode);

inline void CloseFile(FileInfo *info)
{
    if (!info->usingFileBuffer && info->file)
        fClose(info->file);

    info->file = NULL;
}

void GenerateELoadKeys(FileInfo *info, const char *key1, int32 key2);
void DecryptBytes(FileInfo *info, void *buffer, size_t size);
void SkipBytes(FileInfo *info, int32 size);

inline void Seek_Set(FileInfo *info, int32 count)
{
    if (info->readPos != count) {
        info->readPos = count;

        if (info->encrypted) {
            info->eKeyNo      = (info->fileSize / 4) & 0x7F;
            info->eKeyPosA    = 0;
            info->eKeyPosB    = 8;
            info->eNybbleSwap = false;
            SkipBytes(info, count);
        }

        if (info->usingFileBuffer) {
            info->fileData = &info->fileData[info->readPos];
        }
        else {
            fSeek(info->file, info->readPos + info->fileOffset, SEEK_SET);
        }
    }
}

inline void Seek_Cur(FileInfo *info, int32 count)
{
    info->readPos += count;

    if (info->encrypted)
        SkipBytes(info, count);

    if (info->usingFileBuffer) {
        info->fileData += count;
    }
    else {
        fSeek(info->file, count, SEEK_CUR);
    }
}

inline size_t ReadBytes(FileInfo *info, void *data, int32 count)
{
    size_t bytesRead = 0;

    if (info->usingFileBuffer) {
        bytesRead = count;
        memcpy(data, info->fileData, count);
        info->fileData += count;
    }
    else {
        bytesRead = fRead(data, 1, count, info->file);
    }

    if (info->encrypted)
        DecryptBytes(info, data, bytesRead);

    info->readPos += bytesRead;
    return bytesRead;
}

inline uint8 ReadInt8(FileInfo *info)
{
    int8 result      = 0;
    size_t bytesRead = 0;

    if (info->usingFileBuffer) {
        bytesRead = sizeof(int8);
        result    = *info->fileData;
        info->fileData++;
    }
    else {
        bytesRead = fRead(&result, 1, sizeof(int8), info->file);
    }

    if (info->encrypted)
        DecryptBytes(info, &result, bytesRead);

    info->readPos += bytesRead;
    return result;
}

inline int16 ReadInt16(FileInfo *info)
{
    int16 result     = 0;
    size_t bytesRead = 0;

    if (info->usingFileBuffer) {
        bytesRead = sizeof(int16);
        result    = *(int16 *)info->fileData;
        info->fileData += sizeof(int16);
    }
    else {
        bytesRead = fRead(&result, 1, sizeof(int16), info->file);
    }

    if (info->encrypted)
        DecryptBytes(info, &result, bytesRead);

    info->readPos += bytesRead;
    return result;
}

inline int32 ReadInt32(FileInfo *info, bool32 swapEndian)
{
    int32 result     = 0;
    size_t bytesRead = 0;

    if (info->usingFileBuffer) {
        bytesRead = sizeof(int32);
        result    = *(int32 *)info->fileData;
        info->fileData += sizeof(int32);
    }
    else {
        bytesRead = fRead(&result, 1, sizeof(int32), info->file);
    }

    if (info->encrypted)
        DecryptBytes(info, &result, bytesRead);

    if (swapEndian) {
        byte bytes[sizeof(int32)];
        memcpy(bytes, &result, sizeof(int32));

        int max = sizeof(int32) - 1;
        for (int i = 0; i < sizeof(int32) / 2; ++i) {
            byte store     = bytes[i];
            bytes[i]       = bytes[max - i];
            bytes[max - i] = store;
        }
        memcpy(&result, bytes, sizeof(int32));
    }

    info->readPos += bytesRead;
    return result;
}
inline int64 ReadInt64(FileInfo *info)
{
    int64 result     = 0;
    size_t bytesRead = 0;

    if (info->usingFileBuffer) {
        bytesRead = sizeof(int64);
        result    = *(int64 *)info->fileData;
        info->fileData += sizeof(int64);
    }
    else {
        bytesRead = fRead(&result, 1, sizeof(int64), info->file);
    }

    if (info->encrypted)
        DecryptBytes(info, &result, bytesRead);

    info->readPos += bytesRead;
    return result;
}

inline float ReadSingle(FileInfo *info)
{
    float result     = 0;
    size_t bytesRead = 0;

    if (info->usingFileBuffer) {
        bytesRead = sizeof(float);
        result    = *(float *)info->fileData;
        info->fileData += sizeof(float);
    }
    else {
        bytesRead = fRead(&result, 1, sizeof(float), info->file);
    }

    if (info->encrypted)
        DecryptBytes(info, &result, bytesRead);

    info->readPos += bytesRead;
    return result;
}

inline void ReadString(FileInfo *info, char *buffer)
{
    byte size = ReadInt8(info);
    ReadBytes(info, buffer, size);
    buffer[size] = 0;
}

inline int32 ReadZLibRSDK(FileInfo *info, uint8 **buffer)
{
    if (!buffer)
        return 0;

    uLongf complen = ReadInt32(info, false) - 4;
    uint decompLE  = ReadInt32(info, false);
    uLongf destLen = (uint)((decompLE << 24) | ((decompLE << 8) & 0x00FF0000) | ((decompLE >> 8) & 0x0000FF00) | (decompLE >> 24));

    byte *compData = NULL;
    RSDK::AllocateStorage((int32)complen, (void **)&compData, RSDK::DATASET_TMP, false);
    RSDK::AllocateStorage((int32)destLen, (void **)buffer, RSDK::DATASET_TMP, false);
    ReadBytes(info, compData, (int32)complen);

    uncompress(*buffer, &destLen, compData, complen);

    RSDK::RemoveStorageEntry((void **)&compData);

    return (int32)destLen;
}

inline int32 ReadZLib(FileInfo *info, uint8 **buffer, int32 cSize, int32 size)
{
    if (!buffer)
        return 0;

    uLongf complen = cSize;
    uint decompLE  = size;
    uLongf destLen = (uint)((decompLE << 24) | ((decompLE << 8) & 0x00FF0000) | ((decompLE >> 8) & 0x0000FF00) | (decompLE >> 24));

    byte *compData = NULL;
    RSDK::AllocateStorage((int32)complen, (void **)&compData, RSDK::DATASET_TMP, false);
    ReadBytes(info, compData, (int32)complen);

    uncompress(*buffer, &destLen, compData, complen);
    compData = NULL;
    return (int32)destLen;
}

inline int32 ReadZLib(FileInfo *info, uint8 **cBuffer, int32 cSize, uint8 **buffer, int32 size)
{
    if (!buffer || !cBuffer)
        return 0;

    uLongf complen = cSize;
    uLongf destLen = size;

    uncompress(*buffer, &destLen, *cBuffer, complen);
    *cBuffer = NULL;
    return (int32)destLen;
}

#endif
