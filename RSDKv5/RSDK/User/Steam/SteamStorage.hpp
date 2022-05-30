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
    bool32 TryLoadUserFile(const char *filename, void *buffer, uint32 size, int32 (*callback)(int32))
    {
        // load file from steam cloud
        return false;
    }
    bool32 TrySaveUserFile(const char *filename, void *buffer, uint32 size, int32 (*callback)(int32), bool32 compress)
    {
        // save file to steam cloud
        return false;
    }
    bool32 TryDeleteUserFile(const char *filename, int32 (*callback)(int32))
    {
        // delete file from steam cloud
        return false;
    }
    void ClearPrerollErrors() {}
};

#endif