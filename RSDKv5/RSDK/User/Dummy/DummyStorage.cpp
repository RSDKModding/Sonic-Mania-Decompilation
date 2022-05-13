#include "RSDK/Core/RetroEngine.hpp"

#if RETRO_REV02
int RSDK::SKU::DummyUserStorage::TryAuth()
{
    if (authStatus == STATUS_CONTINUE) {
        std::string str = __FILE__;
        str += ": TryAuth() # WARNING TryAuth() when auth currently in progress. \r\n";
        PrintLog(PRINT_NORMAL, str.c_str());
    }

    if (!authStatus) {
        authStatus                                          = STATUS_CONTINUE;
        API_TypeOf(userStorage, DummyUserStorage)->authTime = GetAPIValue(GetAPIValueID("SYSTEM_USERSTORAGE_AUTH_TIME", 0));
    }
    return authStatus;
}
int RSDK::SKU::DummyUserStorage::TryInitStorage()
{
    if (storageStatus == STATUS_CONTINUE) {
        std::string str = __FILE__;
        str += ": TryInitStorage() # WARNING TryInitStorage() when auth currently in progress. \r\n";
        PrintLog(PRINT_NORMAL, str.c_str());
    }
    else {
        storageStatus                                              = STATUS_CONTINUE;
        API_TypeOf(userStorage, DummyUserStorage)->storageInitTime = GetAPIValue(GetAPIValueID("SYSTEM_USERSTORAGE_STORAGE_INIT_TIME", 0));
    }
    return storageStatus;
}
bool32 RSDK::SKU::DummyUserStorage::GetUsername(String *name)
{
    InitString(name, (char *)"IntegerGeorge802", false);
    return true;
}
void RSDK::SKU::DummyUserStorage::ClearPrerollErrors()
{
    if (authStatus != STATUS_OK)
        authStatus = STATUS_NONE;

    authTime = 0;
    if (storageStatus != STATUS_OK)
        storageStatus = STATUS_NONE;
}

bool32 RSDK::SKU::DummyUserStorage::TryLoadUserFile(const char *filename, void *buffer, uint32 bufSize, int32 (*callback)(int32))
{
    bool32 success = false;
    memset(buffer, 0, bufSize);
    if (!noSaveActive) {
        success = LoadUserFile(filename, buffer, bufSize);

        if (bufSize >= 4) {
            byte *bufTest = (byte *)buffer;
            // quick and dirty zlib check
            if (bufTest[0] == 0x78 && (bufTest[1] == 0x01 || bufTest[1] == 0x9C)) {
                uLongf destLen = bufSize;

                byte *compData = NULL;
                RSDK::AllocateStorage(bufSize, (void **)&compData, RSDK::DATASET_TMP, false);
                memcpy(compData, buffer, bufSize);

                uncompress((Bytef *)buffer, &destLen, compData, bufSize);

                RSDK::RemoveStorageEntry((void **)&compData);
            }
        }

        if (callback)
            callback(STATUS_OK);
    }
    else {
        std::string str = __FILE__;
        str += ": TryLoadUserFile() # TryLoadUserFile(";
        str += filename;
        str += ", ...) failing due to noSave \r\n";
        PrintLog(PRINT_NORMAL, str.c_str());

        if (callback)
            callback(STATUS_ERROR);
    }

    return success;
}
bool32 RSDK::SKU::DummyUserStorage::TrySaveUserFile(const char *filename, void *buffer, uint32 size, int32 (*callback)(int32), bool32 compressData)
{
    bool32 success = false;
    if (!noSaveActive) {
        if (compressData) {
            int *cbuf = NULL;
            RSDK::AllocateStorage(size, (void **)&cbuf, RSDK::DATASET_TMP, false);

            uLongf clen = size;
            compress((Bytef *)cbuf, &clen, (Bytef *)buffer, (uLong)size);

            success = SaveUserFile(filename, cbuf, (uint32)clen);

            RSDK::RemoveStorageEntry((void **)&cbuf);
        }
        else {
            success = SaveUserFile(filename, buffer, size);
        }

        if (callback)
            callback(STATUS_OK);
    }
    else {
        std::string str = __FILE__;
        str += ": TrySaveUserFile() # TrySaveUserFile(";
        str += filename;
        str += ", ...) failing due to noSave \r\n";
        PrintLog(PRINT_NORMAL, str.c_str());

        if (callback)
            callback(STATUS_ERROR);
    }

    return success;
}
bool32 RSDK::SKU::DummyUserStorage::TryDeleteUserFile(const char *filename, int32 (*callback)(int32))
{
    if (!noSaveActive) {
        DeleteUserFile(filename);

        if (callback)
            callback(STATUS_OK);
    }
    else {
        std::string str = __FILE__;
        str += ": TryDeleteUserFile() # TryDeleteUserFile(";
        str += filename;
        str += ", ...) failing due to noSave \r\n";
        PrintLog(PRINT_NORMAL, str.c_str());

        if (callback)
            callback(STATUS_ERROR);
    }

    return false;
}
#endif
