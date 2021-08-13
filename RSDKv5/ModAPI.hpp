#ifndef MOD_API_H
#define MOD_API_H

#if RETRO_USE_MOD_LOADER
#include <vector>
#include <string>
#include <map>

typedef enum {
    MODCB_GAME_STARTUP,
    MODCB_STAGELOAD,
    MODCB_ONUPDATE,
    MODCB_ONLATEUPDATE,
    MODCB_ONSTATICUPDATE,
    MODCB_ONDRAW,
    MODCB_STAGEUNLOAD,
    MODCB_MAX,
} ModCallbackEvents;

typedef enum {
    ModTable_LoadModInfo,
    ModTable_AddModCallback,
    ModTable_AddPublicFunction,
    ModTable_GetPublicFunction,
    ModTable_GetSettingsBool,
    ModTable_GetSettingsInt,
    ModTable_GetSettingsString,
    ModTable_SetSettingsBool,
    ModTable_SetSettingsInt,
    ModTable_SetSettingsString,
    ModTable_Max
} ModFunctionTable;

typedef void (*ModCallback)(void *);

struct ModPublicFunctionInfo {
    std::string name;
    void *ptr;
};

struct ModInfo {
    std::string name;
    std::string desc;
    std::string author;
    std::string version;
    std::map<std::string, std::string> fileMap;
    std::vector<ModPublicFunctionInfo> functionList;
    std::string folder;
    bool active;
};


extern std::vector<ModInfo> modList;
extern std::vector<ModCallback> modCallbackList[MODCB_MAX];

extern void *modFunctionTable[ModTable_Max];

void initModAPI();
void loadMods();
bool32 loadMod(ModInfo *info, std::string modsPath, std::string folder, bool32 active);
void saveMods();

void RunModCallbacks(int callbackID, void *data);

//Mod API
bool32 LoadModInfo(const char *folder, TextInfo *name, TextInfo *description, TextInfo *version, bool32 *active);
void AddModCallback(int callbackID, ModCallback callback);
void *AddPublicFunction(const char *folder, const char *functionName, void *functionPtr);
void *GetPublicFunction(const char *folder, const char *functionName);

#endif

#endif // !MOD_API_H
