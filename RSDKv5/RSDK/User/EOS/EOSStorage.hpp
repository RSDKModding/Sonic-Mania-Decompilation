#if RETRO_REV02
struct EOSUserStorage : UserStorage {
    int32 TryAuth()
    {
        // init EGS authorization
        authStatus = STATUS_OK;
        return authStatus;
    }
    int32 TryInitStorage()
    {
        // init EGS storage
        storageStatus = STATUS_OK;
        return storageStatus;
    }
    bool32 GetUsername(String *userName) 
    { 
        // get EGS username
        return false; 
    }
    bool32 TryLoadUserFile(const char *filename, void *buffer, uint32 size, int32 (*callback)(int32))
    {
        // load file from EGS cloud
        return false;
    }
    bool32 TrySaveUserFile(const char *filename, void *buffer, uint32 size, int32 (*callback)(int32), bool32 compress)
    {
        // save file to EGS cloud
        return false;
    }
    bool32 TryDeleteUserFile(const char *filename, int32 (*callback)(int32))
    {
        // delete file from EGS cloud
        return false;
    }
    void ClearPrerollErrors() {}
};

#endif