#ifndef READER_H
#define READER_H

#if RETRO_USING_SDL1 || RETRO_USING_SDL2
#define FileIO                                          SDL_RWops
#define fOpen(path, mode)                               SDL_RWFromFile(path, mode)
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

#include <zlib/zlib.h>

enum Scopes {
    SCOPE_NONE,
    SCOPE_GLOBAL,
    SCOPE_STAGE,
};

struct FileInfo {
    int fileSize;
    int externalFile;
    FileIO *file;
    byte* fileData;
    int readPos;
    int fileOffset;
    byte usingFileBuffer;
    byte encrypted;
    byte eNybbleSwap;
    byte encryptionKeyA[0x10];
    byte encryptionKeyB[0x10];
    byte eKeyPosA;
    byte eKeyPosB;
    byte eKeyNo;
};

struct RSDKFileInfo {
    uint hash[0x4];
    int size;
    int offset;
    byte encrypted;
    byte useFileBuffer;
    int packID;
};

struct RSDKContainer {
    char name[0x100];
    byte *dataPtr;
    int fileCount;
};

extern RSDKFileInfo dataFiles[0x1000];
extern RSDKContainer dataPacks[4];

extern byte dataPackCount;
extern ushort dataFileCount;

extern char gameLogicName[0x400];

extern bool32 useDataFile;

bool32 CheckDataFile(const char *filename, size_t fileOffset, bool32 useBuffer);
bool32 OpenDataFile(FileInfo *info, const char *filename);

enum FileModes {
    FMODE_NONE,
    FMODE_RB,
    FMODE_WB,
    FMODE_RB_PLUS,
    FMODE_4
};

static const char *openModes[3] = { "rb", "wb", "rb+" };

inline bool32 LoadFile(FileInfo *info, const char *filename, byte fileMode)
{
    if (info->file)
        return false;

    if (!info->externalFile && fileMode == FMODE_RB && useDataFile) {
        return OpenDataFile(info, filename);
    }
    else {
        if (fileMode == FMODE_RB || fileMode == FMODE_WB || fileMode == FMODE_RB_PLUS) {
            info->file = fOpen(filename, openModes[fileMode - 1]);
        }
        if (!info->file)
            return false;
        info->readPos  = 0;
        info->fileSize = 0;

        if (fileMode != FMODE_WB) {
            fSeek(info->file, 0, SEEK_END);
            info->fileSize = (int)fTell(info->file);
            fSeek(info->file, 0, SEEK_SET);
        }
        return info->file != NULL;
    }
    return false;
}

inline void CloseFile(FileInfo *info)
{
    if (!info->usingFileBuffer) {
        if (info->file)
            fClose(info->file);
    }
    info->file = NULL;
}

void GenerateELoadKeys(FileInfo *info, const char *key1, int key2);
void DecryptBytes(FileInfo *info, void *buffer, size_t size);
void SkipBytes(FileInfo *info, int size);

inline void Seek_Set(FileInfo* info, int count) {
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

inline void Seek_Cur(FileInfo *info, int count)
{
    info->readPos += count;
    if (info->encrypted) {
        SkipBytes(info, count);
    }

    if (info->usingFileBuffer) {
        info->fileData += count;
    }
    else {
        fSeek(info->file, count, SEEK_CUR);
    }
}

inline size_t ReadBytes(FileInfo *info, void* data, int count)
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

inline byte ReadInt8(FileInfo *info)
{
    byte result = 0;
    size_t bytesRead = 0;
    if (info->usingFileBuffer) {
        bytesRead = sizeof(byte);
        result             = *info->fileData;
        info->fileData++;
    }
    else {
        bytesRead = fRead(&result, 1, sizeof(byte), info->file);
    }
    if (info->encrypted)
        DecryptBytes(info, &result, bytesRead);
    info->readPos += bytesRead;
    return result;
}

inline short ReadInt16(FileInfo *info)
{
    short result  = 0;
    size_t bytesRead = 0;
    if (info->usingFileBuffer) {
        bytesRead = sizeof(short);
        result    = *(short *)info->fileData;
        info->fileData += sizeof(short);
    }
    else {
        bytesRead = fRead(&result, 1, sizeof(short), info->file);
    }
    if (info->encrypted)
        DecryptBytes(info, &result, bytesRead);
    info->readPos += bytesRead;
    return result;
}

inline int ReadInt32(FileInfo *info, bool32 swapEndian)
{
    int result   = 0;
    size_t bytesRead = 0;
    if (info->usingFileBuffer) {
        bytesRead = sizeof(int);
        result    = *(int *)info->fileData;
        info->fileData += sizeof(int);
    }
    else {
        bytesRead = fRead(&result, 1, sizeof(int), info->file);
    }
    if (info->encrypted)
        DecryptBytes(info, &result, bytesRead);

    if (swapEndian) {
        byte bytes[sizeof(int)];
        memcpy(bytes, &result, sizeof(int));

        int max = sizeof(int) - 1;
        for (int i = 0; i < sizeof(int) / 2; ++i) {
            byte store     = bytes[i];
            bytes[i]       = bytes[max - i];
            bytes[max - i] = store;
        }
        memcpy(&result, bytes, sizeof(int));
    }

    info->readPos += bytesRead;
    return result;
}
inline int64 ReadInt64(FileInfo *info)
{
    int64 result  = 0;
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
    float result    = 0;
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

inline void ReadString(FileInfo *info, char* buffer) { 
    byte size = ReadInt8(info);
    ReadBytes(info, buffer, size);
    buffer[size] = 0;
}

inline int ReadZLibRSDK(FileInfo *info, byte **buffer)
{
    if (!buffer)
        return 0;
    uint complen  = ReadInt32(info, false) - 4;
    uint decompLE = ReadInt32(info, false);
    uint destLen      = (uint)((decompLE << 24) | ((decompLE << 8) & 0x00FF0000) | ((decompLE >> 8) & 0x0000FF00) | (decompLE >> 24));

    byte *compData = NULL;
    AllocateStorage(complen, (void**)&compData, DATASET_TMP, false);
    AllocateStorage(destLen, (void **)buffer, DATASET_TMP, false);
    ReadBytes(info, compData, complen);

    uncompress(*buffer, (uLongf *)&destLen, compData, complen);
    compData = NULL;
    return destLen;
}

inline int ReadZLib(FileInfo *info, byte **buffer, int cSize, int size)
{
    if (!buffer)
        return 0;
    uint complen  = cSize;
    uint decompLE = size;
    uint destLen  = (uint)((decompLE << 24) | ((decompLE << 8) & 0x00FF0000) | ((decompLE >> 8) & 0x0000FF00) | (decompLE >> 24));

    byte *compData = NULL;
    AllocateStorage(complen, (void **)&compData, DATASET_TMP, false);
    ReadBytes(info, compData, complen);

    uncompress(*buffer, (uLongf *)&destLen, compData, complen);
    compData = NULL;
    return destLen;
}

inline void ReadHash(FileInfo *info, uint *buffer)
{
    buffer[0] = ReadInt32(info, false);
    buffer[1] = ReadInt32(info, false);
    buffer[2] = ReadInt32(info, false);
    buffer[3] = ReadInt32(info, false);
}

#endif
