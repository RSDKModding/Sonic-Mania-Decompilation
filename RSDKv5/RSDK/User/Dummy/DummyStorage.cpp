#if RETRO_REV02
int32 DummyUserStorage::TryAuth()
{
    if (authStatus == STATUS_CONTINUE) {
        std::string str = __FILE__;
        str += ": TryAuth() # WARNING TryAuth() when auth currently in progress. \r\n";
        PrintLog(PRINT_NORMAL, str.c_str());
    }

    if (!authStatus) {
        authStatus = STATUS_CONTINUE;
        authTime   = GetAPIValue(GetAPIValueID("SYSTEM_USERSTORAGE_AUTH_TIME", 0));
    }
    return authStatus;
}
int32 DummyUserStorage::TryInitStorage()
{
    if (storageStatus == STATUS_CONTINUE) {
        std::string str = __FILE__;
        str += ": TryInitStorage() # WARNING TryInitStorage() when auth currently in progress. \r\n";
        PrintLog(PRINT_NORMAL, str.c_str());
    }
    else {
        storageStatus   = STATUS_CONTINUE;
        storageInitTime = GetAPIValue(GetAPIValueID("SYSTEM_USERSTORAGE_STORAGE_INIT_TIME", 0));
    }
    return storageStatus;
}
bool32 DummyUserStorage::GetUsername(String *name)
{
#if !RETRO_USE_ORIGINAL_CODE
    if (strlen(customSettings.username) > 0)
        InitString(name, (char *)customSettings.username, false);
    else
        InitString(name, (char *)"IntegerGeorge802", false);
#else
    InitString(name, (char *)"IntegerGeorge802", false);
#endif
    return true;
}
void DummyUserStorage::ClearPrerollErrors()
{
    if (authStatus != STATUS_OK)
        authStatus = STATUS_NONE;

    authTime = 0;
    if (storageStatus != STATUS_OK)
        storageStatus = STATUS_NONE;
}

void DummyUserStorage::ProcessFileLoadTime()
{
    for (int32 f = fileList.Count() - 1; f >= 0; --f) {
        DummyFileInfo *file = fileList.At(f);
        if (!file)
            continue;

        if (!file->storageTime) {
            int32 status   = 0;
            bool32 success = false;
            switch (file->type) {
                case 1:
                    success = LoadUserFile(file->path, file->fileBuffer, file->fileSize);
                    status  = STATUS_NOTFOUND;

                    if (file->fileSize >= 4) {
                        uint8 *bufTest = (uint8 *)file->fileBuffer;
                        // quick and dirty zlib check
                        if (bufTest[0] == 0x78 && (bufTest[1] == 0x01 || bufTest[1] == 0x9C)) {
                            uLongf destLen = file->fileSize;

                            uint8 *compData = NULL;
                            AllocateStorage(file->fileSize, (void **)&compData, DATASET_TMP, false);
                            memcpy(compData, file->fileBuffer, file->fileSize);

                            uncompress((Bytef *)file->fileBuffer, &destLen, compData, file->fileSize);

                            RemoveStorageEntry((void **)&compData);
                        }
                    }
                    break;

                case 2:
                    success = SaveUserFile(file->path, file->fileBuffer, file->fileSize);
                    status  = STATUS_ERROR;

                    if (file->compressed)
                        RemoveStorageEntry((void **)&file->fileBuffer);
                    break;

                case 3:
                    success = DeleteUserFile(file->path);
                    status  = STATUS_ERROR;
                    break;
            }

            if (success)
                status = STATUS_OK;

            if (file->callback)
                file->callback(status);

            fileList.Remove(f);
        }
        else {
            --file->storageTime;
        }
    }
}

bool32 DummyUserStorage::TryLoadUserFile(const char *filename, void *buffer, uint32 size, void (*callback)(int32 status))
{
    bool32 success = false;
    memset(buffer, 0, size);

    if (!noSaveActive) {
        DummyFileInfo *file = fileList.Append();
        file->callback      = callback;
        memset(file->path, 0, sizeof(file->path));
        file->fileBuffer = buffer;
        file->fileSize   = size;
        file->type       = 1;
        strncpy(file->path, filename, sizeof(file->path));
        file->storageTime = GetAPIValue(GetAPIValueID("SYSTEM_USERSTORAGE_STORAGE_LOAD_TIME", 0));

        success = true;
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
bool32 DummyUserStorage::TrySaveUserFile(const char *filename, void *buffer, uint32 size, void (*callback)(int32 status), bool32 compressed)
{
    bool32 success = false;
    if (!noSaveActive) {
        DummyFileInfo *file = fileList.Append();
        file->callback      = callback;
        memset(file->path, 0, sizeof(file->path));
        file->type = 2;
        strncpy(file->path, filename, sizeof(file->path));
        file->storageTime = GetAPIValue(GetAPIValueID("SYSTEM_USERSTORAGE_STORAGE_SAVE_TIME", 0));

        if (compressed) {
            AllocateStorage(size, (void **)&file->fileBuffer, DATASET_TMP, false);

            uLongf clen = size;
            compress((Bytef *)file->fileBuffer, &clen, (Bytef *)buffer, (uLong)size);
            file->fileSize   = (int32)clen;
            file->compressed = true;
        }
        else {
            file->fileBuffer = buffer;
            file->fileSize   = size;
        }

        success = true;
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
bool32 DummyUserStorage::TryDeleteUserFile(const char *filename, void (*callback)(int32 status))
{
    if (!noSaveActive) {
        DummyFileInfo *file = fileList.Append();
        file->callback      = callback;
        memset(file->path, 0, sizeof(file->path));
        file->fileBuffer = NULL;
        file->fileSize   = 0;
        file->type       = 3;
        strncpy(file->path, filename, sizeof(file->path));
        file->storageTime = GetAPIValue(GetAPIValueID("SYSTEM_USERSTORAGE_STORAGE_DELETE_TIME", 0));
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

    return true;
}
#endif
