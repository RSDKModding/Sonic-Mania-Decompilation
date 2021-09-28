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
    SUPER_UPDATE,
    SUPER_LATEUPDATE,
    SUPER_STATICUPDATE,
    SUPER_DRAW,
    SUPER_CREATE,
    SUPER_STAGELOAD,
    SUPER_EDITORDRAW,
    SUPER_EDITORLOAD,
    SUPER_SERIALIZE
} ModSuper;

typedef enum {
    ModTable_RegisterGlobals,
    ModTable_RegisterObject,
    ModTable_GetGlobals,
    ModTable_Super,
    ModTable_LoadModInfo,
    ModTable_AddModCallback,
    ModTable_AddPublicFunction,
    ModTable_GetPublicFunction,
    ModTable_GetModPath,
    ModTable_GetSettingsBool,
    ModTable_GetSettingsInt,
    ModTable_GetSettingsString,
    ModTable_SetSettingsBool,
    ModTable_SetSettingsInt,
    ModTable_SetSettingsString,
    ModTable_SaveSettings,
    ModTable_GetConfigBool,
    ModTable_GetConfigInt,
    ModTable_GetConfigString,
    ModTable_ForeachConfig,
    ModTable_ForeachConfigCategory,
    ModTable_GetObject,
    ModTable_Max
} ModFunctionTable;

typedef void (*ModCallback)(void *);

struct ModPublicFunctionInfo {
    std::string name;
    void *ptr;
};

typedef bool (*modLink)(GameInfo *, const char *);
typedef const char* (*langSetup)(GameInfo *);

struct ModInfo {
    std::string name;
    std::string desc;
    std::string author;
    std::string version;
    std::map<std::string, std::string> fileMap;
    std::vector<ModPublicFunctionInfo> functionList;
    std::string folder;
    bool active;
    std::vector<modLink> linkModLogic;
    std::map<std::string, std::map<std::string, std::string>> settings;
    std::map<std::string, std::map<std::string, std::string>> config;
    const char* language = NULL;
};

extern std::map<std::string, ModInfo> langMap;

extern std::vector<ModInfo> modList;
extern std::vector<ModCallback> modCallbackList[MODCB_MAX];

extern void *modFunctionTable[ModTable_Max];
extern int currentObjectID;

void initModAPI();
void loadMods();
bool32 loadMod(ModInfo *info, std::string modsPath, std::string folder, bool32 active);
void saveMods();

inline void sortMods();

void RunModCallbacks(int callbackID, void *data);

// Mod API
void ModRegisterGlobalVariables(const char* globalsPath, void **globals, uint size);
void ModRegisterObject(Object **structPtr, const char *name, uint entitySize, uint objectSize, void (*update)(void), void (*lateUpdate)(void),
                       void (*staticUpdate)(void), void (*draw)(void), void (*create)(void *), void (*stageLoad)(void), void (*editorDraw)(void),
                       void (*editorLoad)(void), void (*serialize)(void), const char *inherited);
void* GetGlobals();

bool32 LoadModInfo(const char *folder, TextInfo *name, TextInfo *description, TextInfo *version, bool32 *active);
void AddModCallback(int callbackID, ModCallback callback);
void *AddPublicFunction(const char *folder, const char *functionName, void *functionPtr);
void *GetPublicFunction(const char *folder, const char *functionName);
void GetModPath(const char *id, TextInfo *result);

bool32 GetSettingsBool(const char *id, const char *key, bool32 fallback);
int GetSettingsInteger(const char *id, const char *key, int fallback);
void GetSettingsString(const char *id, const char *key, TextInfo *result, const char *fallback);

void SetSettingsBool(const char *id, const char *key, bool32 val);
void SetSettingsInteger(const char *id, const char *key, int val);
void SetSettingsString(const char *id, const char *key, TextInfo *val);

void SaveSettings(const char *id);

bool32 GetConfigBool(const char *id, const char *key, bool32 fallback);
int GetConfigInteger(const char *id, const char *key, int fallback);
void GetConfigString(const char *id, const char *key, TextInfo *result, const char *fallback);
bool32 ForeachConfig(const char* id, TextInfo* textInfo);
bool32 ForeachConfigCategory(const char *id, TextInfo *textInfo);


void Super(int objectID, ModSuper callback, void *data);

Object* GetObject(const char* name);

#endif

#endif // !MOD_API_H
