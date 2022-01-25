#include "RSDK/Core/RetroEngine.hpp"

RSDKFileInfo dataFiles[FILE_COUNT];
RSDKContainer dataPacks[PACK_COUNT];

uint8 dataPackCount  = 0;
uint16 dataFileCount = 0;

char gameLogicName[0x200];

bool32 useDataFile = false;

#if RETRO_PLATFORM == RETRO_ANDROID
SDL_RWops *fOpen(const char *path, const char *mode)
{
    char buffer[0x200];
    int a = 0;
    if (!strncmp(path, RSDK::SKU::userFileDir, strlen(RSDK::SKU::userFileDir)))
        a = strlen(RSDK::SKU::userFileDir);
    sprintf(buffer, "%s%s", RSDK::SKU::userFileDir, path + a);

    return SDL_RWFromFile(buffer, mode);
}
#endif

bool32 CheckDataFile(const char *filePath, size_t fileOffset, bool32 useBuffer)
{
    MEM_ZERO(dataPacks[dataPackCount]);
    useDataFile = false;
    FileInfo info;

    char pathBuffer[0x100];
    sprintf(pathBuffer, "%s%s", RSDK::SKU::userFileDir, filePath);

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

        dataPacks[dataPackCount].dataPtr = NULL;
        if (useBuffer) {
            dataPacks[dataPackCount].dataPtr = (byte *)malloc(info.fileSize);
            Seek_Set(&info, 0);
            ReadBytes(&info, dataPacks[dataPackCount].dataPtr, info.fileSize);
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
    StringLowerCase(textBuffer, filename);
    uint hash[0x4];
    GEN_HASH(textBuffer, hash);

    for (int f = 0; f < dataFileCount; ++f) {
        RSDKFileInfo *file = &dataFiles[f];

        if (!HASH_MATCH(hash, file->hash))
            continue;

        info->usingFileBuffer = file->useFileBuffer;
        if (!file->useFileBuffer) {
            info->file = fOpen(dataPacks[file->packID].name, "rb");
            if (!info->file) {
                return false;
            }
            fSeek(info->file, file->offset, SEEK_SET);
        }
        else {
            info->file     = NULL;
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
        PrintLog(PRINT_NORMAL, "Loaded File '%s'", filename);
        return true;
    }
    return false;
}

bool32 LoadFile(FileInfo *info, const char *filename, uint8 fileMode)
{
    if (info->file)
        return false;

    char filePathBuf[0x100];
    strcpy(filePathBuf, filename);

#if RETRO_USE_MOD_LOADER
    char pathLower[0x100];
    memset(pathLower, 0, sizeof(char) * 0x100);
    for (int c = 0; c < strlen(filename); ++c) {
        pathLower[c] = tolower(filename[c]);
    }

    bool addPath = false;
    if (RSDK::activeMod != -1) {
        char buf[0x100];
        sprintf(buf, "%s", filePathBuf);
        sprintf(filePathBuf, "%smods/%s/%s", RSDK::SKU::userFileDir, RSDK::modList[RSDK::activeMod].id.c_str(), buf);
        info->externalFile = true;
        addPath            = false;
    }
    else {
        for (int m = 0; m < RSDK::modList.size(); ++m) {
            if (RSDK::modList[m].active) {
                std::map<std::string, std::string>::const_iterator iter = RSDK::modList[m].fileMap.find(pathLower);
                if (iter != RSDK::modList[m].fileMap.cend()) {
                    strcpy(filePathBuf, iter->second.c_str());
                    info->externalFile = true;
                    break;
                }
            }
        }
    }
#endif

#if RETRO_PLATFORM == RETRO_OSX || RETRO_PLATFORM == RETRO_ANDROID
    if (addPath) {
        char pathBuf[0x100];
        sprintf(pathBuf, "%s%s", userFileDir, filePathBuf);
        sprintf(filePathBuf, "%s", pathBuf);
    }
#endif

    if (!info->externalFile && fileMode == FMODE_RB && useDataFile) {
        return OpenDataFile(info, filename);
    }

    if (fileMode == FMODE_RB || fileMode == FMODE_WB || fileMode == FMODE_RB_PLUS) {
        info->file = fOpen(filePathBuf, openModes[fileMode - 1]);
    }
    if (!info->file) {
        PrintLog(PRINT_NORMAL, "Couldn't load file '%s'", filePathBuf);
        return false;
    }
    info->readPos  = 0;
    info->fileSize = 0;

    if (fileMode != FMODE_WB) {
        fSeek(info->file, 0, SEEK_END);
        info->fileSize = (int)fTell(info->file);
        fSeek(info->file, 0, SEEK_SET);
    }
    PrintLog(PRINT_NORMAL, "Loaded file '%s'", filePathBuf);
    return true;
}

void GenerateELoadKeys(FileInfo *info, const char *key1, int32 key2)
{
    byte hash[0x10];

    // StringA
    StringUpperCase(textBuffer, key1);
    GEN_HASH(textBuffer, (uint *)hash);

    for (int y = 0; y < 0x10; y += 4) {
        info->encryptionKeyA[y + 3] = hash[y + 0];
        info->encryptionKeyA[y + 2] = hash[y + 1];
        info->encryptionKeyA[y + 1] = hash[y + 2];
        info->encryptionKeyA[y + 0] = hash[y + 3];
    }

    // StringB
    sprintf(textBuffer, "%d", key2); // Vary lazy ik
    GEN_HASH(textBuffer, (uint *)hash);

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
void SkipBytes(FileInfo *info, int32 size)
{
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
