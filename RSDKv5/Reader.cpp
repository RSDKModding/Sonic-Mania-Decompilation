#include "RetroEngine.hpp"

RSDKFileInfo dataFiles[0x1000];
RSDKContainer dataPacks[4];

byte dataPackCount = 0;
ushort dataFileCount = 0;

char gameLogicName[0x400];

bool32 useDataFile = false;

bool32 CheckDataFile(const char *filePath, size_t fileOffset, bool32 useBuffer)
{
    MEM_ZERO(dataPacks[dataPackCount]);
    useDataFile = false;
    FileInfo info;

    char pathBuffer[0x100];
    sprintf(pathBuffer, "%s%s", userFileDir, filePath);
    
    InitFileInfo(&info);
    info.externalFile = true;
    if (LoadFile(&info, pathBuffer, FMODE_RB)) {
        byte signature[6] = { 'R', 'S', 'D', 'K', 'v', '5' };
        byte buf          = 0;
        for (int i = 0; i < 6; ++i) {
            buf = ReadInt8(&info);
            if (buf != signature[i])
                return false;
        }
        useDataFile = true;

        strcpy(dataPacks[dataPackCount].name, pathBuffer);

        dataPacks[dataPackCount].fileCount = ReadInt16(&info);
        for (int f = 0; f < dataPacks[dataPackCount].fileCount; ++f) {
            byte b[4];
            for (int y = 0; y < 4; y++) {
                ReadBytes(&info, b, 4);
                dataFiles[f].hash[y] = (b[0] << 24) | (b[1] << 16) | (b[2] << 8) | (b[3] << 0);
            }

            dataFiles[f].offset = ReadInt32(&info, false);
            dataFiles[f].size   = ReadInt32(&info, false);

            dataFiles[f].encrypted = (dataFiles[f].size & 0x80000000) != 0;
            dataFiles[f].size &= 0x7FFFFFFF;
            dataFiles[f].useFileBuffer = useBuffer;
            dataFiles[f].packID        = dataPackCount;
        }
        dataFileCount += dataPacks[dataPackCount].fileCount;
        dataPackCount++;

        CloseFile(&info);

        return true;
    }
    else {
        useDataFile = false;
        return false;
    }
}

bool32 OpenDataFile(FileInfo *info, const char *filename)
{
    StringLowerCase(hashBuffer, filename);
    uint hash[0x4];
    GEN_HASH(hashBuffer, hash);

    for (int f = 0; f < dataFileCount; ++f) {
        RSDKFileInfo *file = &dataFiles[f];

        if (!HASH_MATCH(hash, file->hash))
            continue;

        if (!file->useFileBuffer) {
            info->file = fOpen(dataPacks[file->packID].name, "rb");
            if (!info->file) {
                return false;
            }
            fSeek(info->file, file->offset, SEEK_SET);
        }
        else {
            info->file = NULL;
            info->fileData = &dataPacks[file->packID].dataPtr[file->offset];
        }

        info->fileSize   = file->size;
        info->readPos    = 0;
        info->fileOffset = file->offset;
        info->encrypted  = file->encrypted;
        memset(info->encryptionKeyA, 0, 0x10 * sizeof(byte));
        memset(info->encryptionKeyB, 0, 0x10 * sizeof(byte));
        if (info->encrypted) {
            GenerateELoadKeys(info, filename, info->fileSize);
            info->eKeyNo      = (info->fileSize / 4) & 0x7F;
            info->eKeyPosA    = 0;
            info->eKeyPosB    = 8;
            info->eNybbleSwap = false;
        }
        printLog(PRINT_NORMAL, "Loaded File '%s'", filename);
        return true;
    }
    return false;
}

bool32 LoadFile(FileInfo *info, const char *filename, byte fileMode)
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
        if (!info->file) {
            printLog(PRINT_NORMAL, "Couldn't load file '%s'", filename);
            return false;
        }
        info->readPos  = 0;
        info->fileSize = 0;

        if (fileMode != FMODE_WB) {
            fSeek(info->file, 0, SEEK_END);
            info->fileSize = (int)fTell(info->file);
            fSeek(info->file, 0, SEEK_SET);
        }
        printLog(PRINT_NORMAL, "Loaded file '%s'", filename);
        return true;
    }
    
    return false;
}


void GenerateELoadKeys(FileInfo *info, const char *key1, int key2)
{
    byte hash[0x10];

    // StringA
    StringUpperCase(hashBuffer, key1);
    GEN_HASH(hashBuffer, (uint *)hash);

    for (int y = 0; y < 0x10; y += 4) {
        info->encryptionKeyA[y + 3] = hash[y + 0];
        info->encryptionKeyA[y + 2] = hash[y + 1];
        info->encryptionKeyA[y + 1] = hash[y + 2];
        info->encryptionKeyA[y + 0] = hash[y + 3];
    }

    // StringB
    sprintf(hashBuffer, "%d", key2); // Vary lazy ik
    GEN_HASH(hashBuffer, (uint*)hash);

    for (int y = 0; y < 0x10; y += 4) {
        info->encryptionKeyB[y + 3] = hash[y + 0];
        info->encryptionKeyB[y + 2] = hash[y + 1];
        info->encryptionKeyB[y + 1] = hash[y + 2];
        info->encryptionKeyB[y + 0] = hash[y + 3];
    }
}

void DecryptBytes(FileInfo *info, void *buffer, size_t size)
{
    if (size) {
        byte *data = (byte *)buffer;
        while (size > 0) {
            *data ^= info->eKeyNo ^ info->encryptionKeyB[info->eKeyPosB];
            if (info->eNybbleSwap)
                *data = ((*data << 4) + (*data >> 4)) & 0xFF;
            *data ^= info->encryptionKeyA[info->eKeyPosA];

            info->eKeyPosA++;
            info->eKeyPosB++;

            if (info->eKeyPosA <= 0x0F) {
                if (info->eKeyPosB > 0x0C) {
                    info->eKeyPosB = 0;
                    info->eNybbleSwap ^= 1;
                }
            }
            else if (info->eKeyPosB <= 0x08) {
                info->eKeyPosA = 0;
                info->eNybbleSwap ^= 1;
            }
            else {
                info->eKeyNo += 2;
                info->eKeyNo &= 0x7F;

                if (info->eNybbleSwap) {
                    info->eNybbleSwap = false;

                    info->eKeyPosA = info->eKeyNo % 7;
                    info->eKeyPosB = (info->eKeyNo % 0xC) + 2;
                }
                else {
                    info->eNybbleSwap = true;

                    info->eKeyPosA = (info->eKeyNo % 0xC) + 3;
                    info->eKeyPosB = info->eKeyNo % 7;
                }
            }

            ++data;
            --size;
        }
    }
}
void SkipBytes(FileInfo* info, int size) {
    if (size) {
        while (size > 0) {
            info->eKeyPosA++;
            info->eKeyPosB++;

            if (info->eKeyPosA <= 0x0F) {
                if (info->eKeyPosB > 0x0C) {
                    info->eKeyPosB = 0;
                    info->eNybbleSwap ^= 1;
                }
            }
            else if (info->eKeyPosB <= 0x08) {
                info->eKeyPosA = 0;
                info->eNybbleSwap ^= 1;
            }
            else {
                info->eKeyNo += 2;
                info->eKeyNo &= 0x7F;

                if (info->eNybbleSwap) {
                    info->eNybbleSwap = false;

                    info->eKeyPosA = info->eKeyNo % 7;
                    info->eKeyPosB = (info->eKeyNo % 0xC) + 2;
                }
                else {
                    info->eNybbleSwap = true;

                    info->eKeyPosA = (info->eKeyNo % 0xC) + 3;
                    info->eKeyPosB = info->eKeyNo % 7;
                }
            }
            --size;
        }
    }
}
