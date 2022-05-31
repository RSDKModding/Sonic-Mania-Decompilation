#if RETRO_REV02
struct SteamUserStorage : UserStorage {
    int32 TryAuth()
    {
        authStatus = STATUS_OK;
        return authStatus;
    }
    int32 TryInitStorage()
    {
        storageStatus = STATUS_OK;
        return storageStatus;
    }
    bool32 GetUsername(String *userName) { return false; }
    bool32 TryLoadUserFile(const char *filename, void *buffer, uint32 size, void (*callback)(int32 status))
    {
        // load file from steam cloud
        return false;
    }
    bool32 TrySaveUserFile(const char *filename, void *buffer, uint32 size, void (*callback)(int32 status), bool32 compressed)
    {
        // save file to steam cloud
        return false;
    }
    bool32 TryDeleteUserFile(const char *filename, void (*callback)(int32 status))
    {
        // delete file from steam cloud
        return false;
    }
    void ClearPrerollErrors() {}
};

#endif