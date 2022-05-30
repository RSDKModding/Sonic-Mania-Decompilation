
#if RETRO_REV02

// This is the "dummy" struct, it serves as the base in the event a suitable API isn't loaded (such as in this decomp)
// This struct should never be removed, other structs such as "SteamUserStorage" would be added and "userStorage" would be set to that instead
struct DummyUserStorage : UserStorage {
    int32 TryAuth();
    int32 TryInitStorage();
    bool32 GetUsername(String *name);
    bool32 TryLoadUserFile(const char *filename, void *buffer, uint32 size, int32 (*callback)(int32));
    bool32 TrySaveUserFile(const char *filename, void *buffer, uint32 size, int32 (*callback)(int32), bool32 compress);
    bool32 TryDeleteUserFile(const char *filename, int32 (*callback)(int32));
    void ClearPrerollErrors();

    int32 authTime        = 0;
    int32 storageInitTime = 0;
    int32 unknown[2];
};

#endif