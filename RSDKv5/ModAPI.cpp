#include "RetroEngine.hpp"
#if RETRO_USE_MOD_LOADER

std::vector<ModInfo> modList;
std::vector<ModCallback> modCallbackList[MODCB_MAX];

void *modFunctionTable[ModTable_Max];

#define addToModFunctionTable(id, func) modFunctionTable[id] = (void *)func;

void initModAPI() { 
    memset(modFunctionTable, 0, sizeof(modFunctionTable));
    
    addToModFunctionTable(ModTable_LoadModInfo, LoadModInfo);
    addToModFunctionTable(ModTable_AddModCallback, AddModCallback);
    addToModFunctionTable(ModTable_AddPublicFunction, AddPublicFunction);
    addToModFunctionTable(ModTable_GetPublicFunction, GetPublicFunction);
    addToModFunctionTable(ModTable_GetSettingsBool, NULL);
    addToModFunctionTable(ModTable_GetSettingsInt, NULL);
    addToModFunctionTable(ModTable_GetSettingsString, NULL);
    addToModFunctionTable(ModTable_SetSettingsBool, NULL);
    addToModFunctionTable(ModTable_SetSettingsInt, NULL);
    addToModFunctionTable(ModTable_SetSettingsString, NULL);

    loadMods(); 
}

void loadMods()
{
    modList.clear();
    for (int c = 0; c < MODCB_MAX; ++c) modCallbackList[c].clear();
}

bool32 loadMod() { 
    // + 1
    return false; 
}

void saveMods()
{
    for (int m = 0; m < modList.size(); ++m) {
        //
    }
}

void RunModCallbacks(int callbackID, void *data)
{
    if (callbackID < 0 || callbackID >= MODCB_MAX)
        return;

    for (int c = 0; c < modCallbackList[callbackID].size(); ++c) {
        if (modCallbackList[callbackID][c])
            modCallbackList[callbackID][c](data);
    }
}

//Mod API
bool32 LoadModInfo(const char* folder, TextInfo *name, TextInfo* description, TextInfo* version, bool32* active) {
    for (int m = 0; m < modList.size(); ++m) {
        if (modList[m].folder == folder) {
            if (description)
                SetText(description, (char *)modList[m].desc.c_str(), false);
            if (description)
                SetText(description, (char*)modList[m].desc.c_str(), false);
            if (version)
                SetText(description, (char*)modList[m].version.c_str(), false);
            if (active)
                *active = modList[m].active;

            return true;
        }
    }
    return false;
}

void AddModCallback(int callbackID, ModCallback callback)
{
    if (callbackID < 0 || callbackID >= MODCB_MAX)
        return;

    modCallbackList[callbackID].push_back(callback);
}

void *AddPublicFunction(const char *folder, const char *functionName, void *functionPtr)
{
    for (int m = 0; m < modList.size(); ++m) {
        if (modList[m].folder == folder) {
            ModPublicFunctionInfo info;
            info.name = functionName;
            info.ptr  = functionPtr;
            modList[m].functionList.push_back(info);
        }
    }
    return NULL;
}

void* GetPublicFunction(const char *folder, const char* functionName)
{
    for (int m = 0; m < modList.size(); ++m) {
        if (modList[m].folder == folder) {
            for (int f = 0; f < modList[m].functionList.size(); ++f) {
                if (modList[m].functionList[f].name == functionName)
                    return modList[m].functionList[f].ptr;
            }
        }
    }
    return NULL;
}

#endif