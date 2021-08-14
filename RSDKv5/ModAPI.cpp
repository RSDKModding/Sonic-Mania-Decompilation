#include "RetroEngine.hpp"
#if RETRO_USE_MOD_LOADER

#include <filesystem>
#include <sstream>

#if RETRO_PLATFORM == RETRO_ANDROID
namespace fs = std::__fs::filesystem;
#else
namespace fs = std::filesystem;
#endif
#if RETRO_PLATFORM == RETRO_WIN
#include "Windows.h"
#endif

#if RETRO_PLATFORM == RETRO_OSX || RETRO_PLATFORM == RETRO_LINUX || RETRO_PLATFORM == RETRO_ANDROID
#include <dlfcn.h>
#endif

std::vector<ModInfo> modList;
std::vector<ModCallback> modCallbackList[MODCB_MAX];

void *modFunctionTable[ModTable_Max];

#define addToModFunctionTable(id, func) modFunctionTable[id] = (void *)func;

void initModAPI()
{
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

inline void sortMods()
{
    std::sort(modList.begin(), modList.end(), [](ModInfo a, ModInfo b) {
        if (!(a.active && b.active))
            return a.active;
        // keep it unsorted i guess
        return false;
    });
}

void loadMods()
{
    modList.clear();
    for (int c = 0; c < MODCB_MAX; ++c) modCallbackList[c].clear();
    char modBuf[0x100];
    sprintf(modBuf, "%smods/", userFileDir);
    fs::path modPath(modBuf);

    if (fs::exists(modPath) && fs::is_directory(modPath)) {
        std::string mod_config = modPath.string() + "/modconfig.ini";
        FileIO *configFile     = fOpen(mod_config.c_str(), "r");
        if (configFile) {
            fClose(configFile);
            char buffer[0x100];
            auto ini = iniparser_load(mod_config.c_str());

            int c             = iniparser_getsecnkeys(ini, "Mods");
            const char **keys = new const char *[c];
            iniparser_getseckeys(ini, "Mods", keys);

            for (int m = 0; m < c; ++m) {
                bool active = false;
                ModInfo info;
                if (loadMod(&info, modPath.string(), std::string(keys[m] + 5), iniparser_getboolean(ini, keys[m], false)))
                    modList.push_back(info);
            }
        }

        try {
            auto rdi = fs::directory_iterator(modPath);
            for (auto de : rdi) {
                if (de.is_directory()) {
                    fs::path modDirPath = de.path();

                    ModInfo info;

                    std::string modDir            = modDirPath.string().c_str();
                    const std::string mod_inifile = modDir + "/mod.ini";
                    std::string folder            = modDirPath.filename().string();

                    bool flag = true;
                    for (int m = 0; m < modList.size(); ++m) {
                        if (modList[m].folder == folder) {
                            flag = false;
                            break;
                        }
                    }

                    if (flag && loadMod(&info, modPath.string(), modDirPath.filename().string(), false))
                        modList.push_back(info);
                }
            }
        } catch (fs::filesystem_error fe) {
            printLog(PRINT_ERROR, "Mods Folder Scanning Error: %s", fe.what());
        }
    }
    sortMods();
}

bool32 loadMod(ModInfo *info, std::string modsPath, std::string folder, bool32 active)
{
    if (!info)
        return false;

    info->fileMap.clear();
    info->name    = "";
    info->desc    = "";
    info->author  = "";
    info->version = "";
    info->folder  = "";
    info->active  = false;

    const std::string modDir = modsPath + "/" + folder;

    FileIO *f = fOpen((modDir + "/mod.ini").c_str(), "r");
    if (f) {
        fClose(f);
        auto ini = iniparser_load((modDir + "/mod.ini").c_str());

        info->folder = folder;
        info->active = active;

        char infoBuf[0x100];
        info->name    = iniparser_getstring(ini, ":Name", "Unnamed Mod");
        info->desc    = iniparser_getstring(ini, ":Description", "");
        info->author  = iniparser_getstring(ini, ":Author", "Unknown Author");
        info->version = iniparser_getstring(ini, ":Version", "1.0.0");

        // Check for Data/ replacements
        fs::path dataPath(modDir + "/Data");

        if (fs::exists(dataPath) && fs::is_directory(dataPath)) {
            try {
                auto data_rdi = fs::recursive_directory_iterator(dataPath);
                for (auto data_de : data_rdi) {
                    if (data_de.is_regular_file()) {
                        char modBuf[0x100];
                        strcpy(modBuf, data_de.path().string().c_str());
                        char folderTest[4][0x10] = {
                            "Data/",
                            "Data\\",
                            "data/",
                            "data\\",
                        };
                        int tokenPos = -1;
                        for (int i = 0; i < 4; ++i) {
                            tokenPos = std::string(modBuf).find(folderTest[i], 0);
                            if (tokenPos >= 0)
                                break;
                        }

                        if (tokenPos >= 0) {
                            char buffer[0x80];
                            for (int i = strlen(modBuf); i >= tokenPos; --i) {
                                buffer[i - tokenPos] = modBuf[i] == '\\' ? '/' : modBuf[i];
                            }

                            // printLog(modBuf);
                            std::string path(buffer);
                            std::string modPath(modBuf);
                            char pathLower[0x100];
                            memset(pathLower, 0, sizeof(char) * 0x100);
                            for (int c = 0; c < path.size(); ++c) {
                                pathLower[c] = tolower(path.c_str()[c]);
                            }

                            info->fileMap.insert(std::pair<std::string, std::string>(pathLower, modBuf));
                        }
                    }
                }
            } catch (fs::filesystem_error fe) {
                printLog(PRINT_ERROR, "Data Folder Scanning Error: %s", fe.what());
            }
        }
        std::string logic(iniparser_getstring(ini, ":LogicFile", ""));
        if (logic.length() && info->active) {
            std::istringstream stream(logic);
            std::string buf;
            while (std::getline(stream, buf, ',')) {
                int mode = 0;
#define ENDS_WITH(str) buf.length() > strlen(str) && !buf.compare(buf.length() - strlen(str), strlen(str), std::string(str))
#if RETRO_PLATFORM == RETRO_WIN
                if (ENDS_WITH(".dll"))
                    mode = 1;
#elif RETRO_PLATFORM == RETRO_OSX
                if (ENDS_WITH(".dylib"))
                    mode = 1;
#elif RETRO_PLATFORM == RETRO_LINUX || RETRO_PLATFORM == RETRO_ANDROID
                if (ENDS_WITH(".so"))
                    mode = 1;
#else
                if (false)
                    ;
#endif
#if RETRO_USE_PYTHON
                else if (ENDS_WITH(".py"))
                    mode = 2;
#endif
#undef ENDS_WITH
                fs::path file;

                if (!mode) {
                    // autodec
                    std::string autodec = "";
#if RETRO_PLATFORM == RETRO_WIN
                    autodec = ".dll";
#elif RETRO_PLATFORM == RETRO_OSX
                    autodec = ".dylib";
#elif RETRO_PLATFORM == RETRO_LINUX || RETRO_PLATFORM == RETRO_ANDROID
                    autodec = ".so";
#endif
                    file = fs::path(modDir + "/" + buf + autodec);
                    if (fs::exists(file))
                        mode = 1;
                }
#if RETRO_USE_PYTHON
                if (!mode) {
                    file = fs::path(modDir + "/" + buf + ".py");
                    if (fs::exists(file))
                        mode = 2;
                }
#endif
                if (!mode) {
                    iniparser_freedict(ini);
                    return false;
                }

                bool linked = false;
                if (mode == 1) {
#if RETRO_PLATFORM == RETRO_WIN
                    HMODULE hLibModule = LoadLibraryA(file.string().c_str());

                    if (hLibModule) {
                        modLink linkGameLogic = (modLink)GetProcAddress(hLibModule, "LinkModLogic");
                        if (linkGameLogic) {
                            info->linkModLogic.push_back(linkGameLogic);
                            linked = true;
                        }
                    }
#elif RETRO_PLATFORM == RETRO_OSX || RETRO_PLATFORM == RETRO_LINUX || RETRO_PLATFORM == RETRO_ANDROID
                    void *link_handle = (void *)dlopen(file.string().c_str(), RTLD_LOCAL | RTLD_LAZY);

                    if (link_handle) {
                        modLink linkGameLogic = (modLink)dlsym(link_handle, "LinkModLogic");
                        if (linkGameLogic) {
                            info->linkModLogic.push_back(linkGameLogic);
                            linked = true;
                        }
                    }
#endif
                }
                // TODO: mode 2 (python)
                if (!linked) {
                    iniparser_freedict(ini);
                    return false;
                }
            }
        }

        iniparser_freedict(ini);
        return true;
    }
    return false;
}

void saveMods()
{
    char modBuf[0x100];
    sprintf(modBuf, "%smods/", userFileDir);
    fs::path modPath(modBuf);

    sortMods();

    if (fs::exists(modPath) && fs::is_directory(modPath)) {
        std::string mod_config = modPath.string() + "/modconfig.ini";
        FileIO *file           = fOpen(mod_config.c_str(), "w");

        writeText(file, "[Mods]\n");

        for (int m = 0; m < modList.size(); ++m) {
            ModInfo *info = &modList[m];

            writeText(file, "%s=%c\n", info->folder.c_str(), info->active ? 'y' : 'n');
        }
        fClose(file);
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

// Mod API
bool32 LoadModInfo(const char *folder, TextInfo *name, TextInfo *description, TextInfo *version, bool32 *active)
{
    for (int m = 0; m < modList.size(); ++m) {
        if (modList[m].folder == folder) {
            if (description)
                SetText(description, (char *)modList[m].desc.c_str(), false);
            if (description)
                SetText(description, (char *)modList[m].desc.c_str(), false);
            if (version)
                SetText(description, (char *)modList[m].version.c_str(), false);
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

void *GetPublicFunction(const char *folder, const char *functionName)
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