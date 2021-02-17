#include "RetroEngine.hpp"

RSDKContainer rsdkContainer;
char rsdkName[0x400];

bool32 useDataFile = false;

bool32 CheckDataFile(const char *filePath)
{
    MEM_ZERO(rsdkContainer);
    MEM_ZERO(rsdkName);
    useDataFile = false;
    FileInfo info;

    MEM_ZERO(info);
    info.externalFile = true;
    if (LoadFile(&info, filePath)) {
        byte signature[6] = { 'R', 'S', 'D', 'K', 'v', '5' };
        byte buf          = 0;
        for (int i = 0; i < 6; ++i) {
            buf = ReadInt8(&info);
            if (buf != signature[i])
                return false;
        }
        useDataFile = true;

        StrCopy(rsdkName, filePath);

        rsdkContainer.fileCount = ReadInt16(&info);
        for (int f = 0; f < rsdkContainer.fileCount; ++f) {
            byte b[4];
            for (int y = 0; y < 4; y++) {
                ReadBytes(&info, b, 4);
                rsdkContainer.files[f].hash[y] = (b[0] << 24) | (b[1] << 16) | (b[2] << 8) | (b[3] << 0);
                //rsdkContainer.files[f].hash[y] = ReadInt32(&info);
            }

            rsdkContainer.files[f].offset  = ReadInt32(&info);
            rsdkContainer.files[f].filesize = ReadInt32(&info);

            rsdkContainer.files[f].encrypted = (rsdkContainer.files[f].filesize & 0x80000000) != 0;
            rsdkContainer.files[f].filesize &= 0x7FFFFFFF;

            rsdkContainer.files[f].fileID = f;
        }

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
    memset(hashBuffer, 0, 0x400);
    StringLowerCase(hashBuffer, filename);
    uint buffer[0x4];
    int len = StrLength(hashBuffer);
    GenerateHash(buffer, len);

    for (int f = 0; f < rsdkContainer.fileCount; ++f) {
        RSDKFileInfo *file = &rsdkContainer.files[f];

        bool32 match = true;
        for (int h = 0; h < 4; ++h) {
            if (buffer[h] != file->hash[h]) {
                match = false;
                break;
            }
        }
        if (!match)
            continue;

        
        info->file = fOpen(rsdkName, "rb");
        if (!info->file) {
            return false;
        }
        fSeek(info->file, 0, SEEK_END);
        info->fileSize         = file->filesize;
        info->readPos           = 0;
        fSeek(info->file, file->offset, SEEK_SET);

        info->fileOffset = file->offset;
        info->encrypted = file->encrypted;
        memset(info->encryptionKeyA, 0, 0x10 * sizeof(byte));
        memset(info->encryptionKeyB, 0, 0x10 * sizeof(byte));
        if (info->encrypted) {
            GenerateELoadKeys(info, filename, info->fileSize);
            info->eKeyNo      = (info->fileSize / 4) & 0x7F;
            info->eKeyPosA = 0;
            info->eKeyPosB = 8;
            info->eNybbleSwap = false;
        }
        printLog(SEVERITY_NONE, "Loaded File '%s'", filename);
        return true;
    }
    printLog(SEVERITY_NONE, "Couldn't load file '%s'", filename);
    return false;
}


void GenerateELoadKeys(FileInfo *info, const char *key1, int key2)
{
    byte hash[0x10];

    // StringA
    StringUpperCase(hashBuffer, key1);
    int len = StrLength(hashBuffer);
    GenerateHash((uint *)hash, len);

    for (int y = 0; y < 0x10; y += 4) {
        info->encryptionKeyA[y + 3] = hash[y + 0];
        info->encryptionKeyA[y + 2] = hash[y + 1];
        info->encryptionKeyA[y + 1] = hash[y + 2];
        info->encryptionKeyA[y + 0] = hash[y + 3];
    }

    // StringB
    sprintf(hashBuffer, "%d", key2); // Vary lazy ik
    len = StrLength(hashBuffer);
    GenerateHash((uint*)hash, len);

    for (int y = 0; y < 0x10; y += 4) {
        info->encryptionKeyB[y + 3] = hash[y + 0];
        info->encryptionKeyB[y + 2] = hash[y + 1];
        info->encryptionKeyB[y + 1] = hash[y + 2];
        info->encryptionKeyB[y + 0] = hash[y + 3];
    }
}

void DecryptBytes(FileInfo *info, void *buffer, int size)
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