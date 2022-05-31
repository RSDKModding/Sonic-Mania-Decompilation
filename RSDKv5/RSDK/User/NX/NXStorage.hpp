#if RETRO_REV02
struct NXUserStorage : UserStorage {
    int32 TryAuth()
    {
        authStatus = STATUS_OK;
        return authStatus;
    }
    int32 TryInitStorage() { 
        // init switch save dir 
        return 0; 
    }
    bool32 GetUsername(String *userName) { 
        // get switch nickname
        return false; 
    }
    bool32 TryLoadUserFile(const char *filename, void *buffer, uint32 size, void (*callback)(int32 status)) { 
        // load from switch save dir
        return false; 
    }
    bool32 TrySaveUserFile(const char *filename, void *buffer, uint32 size, void (*callback)(int32 status), bool32 compressed) { 
        // save to switch save dir
        return false; 
    }
    bool32 TryDeleteUserFile(const char *filename, void (*callback)(int32 status)) {
        // delete from switch save dir
        return false; 
    }
    void ClearPrerollErrors()
    {
        if (storageStatus != STATUS_OK)
            storageStatus = STATUS_NONE;
    }
};

#endif