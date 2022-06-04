#include "RSDK/Core/RetroEngine.hpp"

// Start Dummy Achievements
std::string RSDK::achievementText = "Achievement!";
std::vector<RSDK::AchievementInfo> RSDK::achievementList;
std::vector<int> RSDK::achievementStack;
// End Dummy Achievements

#if RETRO_USE_MOD_LOADER

#include <filesystem>
#include <sstream>
#include <stdexcept>
#include <functional>

#if RETRO_PLATFORM == RETRO_ANDROID
namespace fs = std::__fs::filesystem;
#else
namespace fs = std::filesystem;
#endif

#if RETRO_PLATFORM == RETRO_WIN
#include "Windows.h"
#undef GetObject // fuck you
#endif

#if RETRO_PLATFORM == RETRO_OSX || RETRO_PLATFORM == RETRO_LINUX || RETRO_PLATFORM == RETRO_ANDROID
#include <dlfcn.h>
#if RETRO_PLATFORM == RETRO_ANDROID
#define dlopen(file, flags) SDL_LoadObject(file)
#endif
#endif

#include "iniparser/iniparser.h"

using namespace RSDK;

int32 RSDK::currentObjectID = 0;

// this helps later on just trust me
#define MODAPI_ENDS_WITH(str) buf.length() > strlen(str) && !buf.compare(buf.length() - strlen(str), strlen(str), std::string(str))

std::vector<RSDK::ModInfo> RSDK::modList;
int32 RSDK::activeMod = -1;
std::vector<RSDK::ModCallbackSTD> RSDK::modCallbackList[RSDK::MODCB_MAX];
RSDK::ModInfo *RSDK::currentMod;

std::vector<RSDK::ModPublicFunctionInfo> gamePublicFuncs;

void *RSDK::modFunctionTable[RSDK::ModTable_Max];

#define ADD_MOD_FUNCTION(id, func) modFunctionTable[id] = (void *)func;

// https://www.techiedelight.com/trim-string-cpp-remove-leading-trailing-spaces/
std::string trim(const std::string &s)
{
    auto start = s.begin();
    while (start != s.end() && std::isspace(*start)) {
        start++;
    }

    auto end = s.end();
    do {
        end--;
    } while (std::distance(start, end) > 0 && std::isspace(*end));

    return std::string(start, end + 1);
}

void RSDK::InitModAPI()
{
    memset(modFunctionTable, 0, sizeof(modFunctionTable));

    // ============================
    // Mod Function Table
    // ============================

    // Registration & Core
    ADD_MOD_FUNCTION(ModTable_GetGlobals, GetGlobals);
    ADD_MOD_FUNCTION(ModTable_RegisterGlobals, ModRegisterGlobalVariables);
    ADD_MOD_FUNCTION(ModTable_RegisterObject, ModRegisterObject);
    ADD_MOD_FUNCTION(ModTable_RegisterObjectSTD, ModRegisterObject_STD);
    ADD_MOD_FUNCTION(ModTable_Super, Super);

    // Mod Info
    ADD_MOD_FUNCTION(ModTable_LoadModInfo, LoadModInfo);
    ADD_MOD_FUNCTION(ModTable_GetModPath, GetModPath);
    ADD_MOD_FUNCTION(ModTable_GetModCount, GetModCount);
    ADD_MOD_FUNCTION(ModTable_GetModIDByIndex, GetModIDByIndex);
    ADD_MOD_FUNCTION(ModTable_ForeachModID, ForeachModID);

    // Mod Callbacks & Public Functions
    ADD_MOD_FUNCTION(ModTable_AddModCallback, AddModCallback);
    ADD_MOD_FUNCTION(ModTable_AddModCallbackSTD, AddModCallback_STD);
    ADD_MOD_FUNCTION(ModTable_AddPublicFunction, AddPublicFunction);
    ADD_MOD_FUNCTION(ModTable_GetPublicFunction, GetPublicFunction);

    // Mod Settings
    ADD_MOD_FUNCTION(ModTable_GetSettingsBool, GetSettingsBool);
    ADD_MOD_FUNCTION(ModTable_GetSettingsInt, GetSettingsInteger);
    ADD_MOD_FUNCTION(ModTable_GetSettingsString, GetSettingsString);
    ADD_MOD_FUNCTION(ModTable_SetSettingsBool, SetSettingsBool);
    ADD_MOD_FUNCTION(ModTable_SetSettingsInt, SetSettingsInteger);
    ADD_MOD_FUNCTION(ModTable_SetSettingsString, SetSettingsString);
    ADD_MOD_FUNCTION(ModTable_SaveSettings, SaveSettings);

    // Config
    ADD_MOD_FUNCTION(ModTable_GetConfigBool, GetConfigBool);
    ADD_MOD_FUNCTION(ModTable_GetConfigInt, GetConfigInteger);
    ADD_MOD_FUNCTION(ModTable_GetConfigString, GetConfigString);
    ADD_MOD_FUNCTION(ModTable_ForeachConfig, ForeachConfig);
    ADD_MOD_FUNCTION(ModTable_ForeachConfigCategory, ForeachConfigCategory);

    ADD_MOD_FUNCTION(ModTable_GetObject, GetObject);

    // Achievements
    ADD_MOD_FUNCTION(ModTable_RegisterAchievement, RegisterAchievement);
    ADD_MOD_FUNCTION(ModTable_GetAchievementInfo, GetAchievementInfo);
    ADD_MOD_FUNCTION(ModTable_GetAchievementIndexByID, GetAchievementIndexByID);
    ADD_MOD_FUNCTION(ModTable_GetAchievementCount, GetAchievementCount);

    // Shaders
    ADD_MOD_FUNCTION(ModTable_LoadShader, RenderDevice::LoadShader);

    LoadMods();
}

void RSDK::SortMods()
{
    std::sort(modList.begin(), modList.end(), [](ModInfo a, ModInfo b) {
        if (!(a.active && b.active))
            return a.active;
        // keep it unsorted i guess
        return false;
    });
}

void RSDK::UnloadMods()
{
    for (ModInfo &mod : modList) {
        if (mod.unloadMod)
            mod.unloadMod();

        for (modLogicHandle &handle : mod.modLogicHandles) {
#if RETRO_PLATFORM == RETRO_WIN
            if (handle) {
                FreeLibrary(handle);
                handle = NULL;
            }
#endif

#if RETRO_PLATFORM == RETRO_OSX || RETRO_PLATFORM == RETRO_LINUX
            if (handle)
                dlclose(handle);
#endif

#if RETRO_PLATFORM == RETRO_SWITCH
            //if (handle)
            //    dlclose(handle);
#endif
        }
        mod.modLogicHandles.clear();
    }
    modList.clear();
    for (int32 c = 0; c < MODCB_MAX; ++c) modCallbackList[c].clear();

    // Clear stage storage
    ClearUnusedStorage(DATASET_STG);
    ClearUnusedStorage(DATASET_SFX);
    dataStorage[DATASET_TMP].usedStorage = 0;
}

void RSDK::LoadMods()
{
    UnloadMods();

#if RETRO_USE_MOD_LOADER
    if (AudioDevice::initializedAudioChannels) {
        // we're about to reload these, so clear anything we already have
        ClearGlobalSfx();
    }
#endif

    using namespace std;
    char modBuf[0x100];
    sprintf_s(modBuf, (int32)sizeof(modBuf), "%smods/", SKU::userFileDir);
    fs::path modPath(modBuf);

    if (fs::exists(modPath) && fs::is_directory(modPath)) {
        string mod_config  = modPath.string() + "/modconfig.ini";
        FileIO *configFile = fOpen(mod_config.c_str(), "r");
        if (configFile) {
            fClose(configFile);
            auto ini = iniparser_load(mod_config.c_str());

            int32 c           = iniparser_getsecnkeys(ini, "Mods");
            const char **keys = new const char *[c];
            iniparser_getseckeys(ini, "Mods", keys);

            for (int32 m = 0; m < c; ++m) {
                ModInfo info;
                bool32 active = iniparser_getboolean(ini, keys[m], false);
                bool32 loaded = LoadMod(&info, modPath.string(), string(keys[m] + 5), active);
                if (!loaded)
                    info.active = false;
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

                    if (std::find_if(modList.begin(), modList.end(), [&folder](ModInfo m) { return m.id == folder; }) == modList.end()) {

                        const std::string modDir = modPath.string() + "/" + modDirPath.filename().string();

                        FileIO *f = fOpen((modDir + "/mod.ini").c_str(), "r");
                        if (f) {
                            fClose(f);
                            LoadMod(&info, modPath.string(), modDirPath.filename().string(), false);
                            modList.push_back(info);
                        }
                    }
                }
            }
        } catch (fs::filesystem_error fe) {
            PrintLog(PRINT_ERROR, "Mods folder scanning error: %s", fe.what());
        }
    }
    SortMods();
}

void loadCfg(ModInfo *info, std::string path)
{
    FileInfo *cfg     = new FileInfo();
    cfg->externalFile = true;
    // CFG FILE READ
    if (LoadFile(cfg, path.c_str(), FMODE_RB)) {
        int32 catCount = ReadInt8(cfg);
        for (int32 c = 0; c < catCount; ++c) {
            char catBuf[0x100];
            ReadString(cfg, catBuf);
            int32 keyCount = ReadInt8(cfg);
            for (int32 k = 0; k < keyCount; ++k) {
                // ReadString except w packing the type bit
                uint8 size   = ReadInt8(cfg);
                char *keyBuf = new char[size & 0x7F];
                ReadBytes(cfg, keyBuf, size & 0x7F);
                keyBuf[size & 0x7F] = 0;
                uint8 type          = size & 0x80;
                if (!type) {
                    char buf[0xFFFF];
                    ReadString(cfg, buf);
                    info->config[catBuf][keyBuf] = buf;
                }
                else
                    info->config[catBuf][keyBuf] = std::to_string(ReadInt32(cfg, false));
            }
        }
    }
    CloseFile(cfg);
}

bool32 RSDK::LoadMod(ModInfo *info, std::string modsPath, std::string folder, bool32 active)
{
    if (!info)
        return false;

    ModInfo *cur = currentMod;

    PrintLog(PRINT_NORMAL, "[MOD] Trying to load mod %s...", folder.c_str());

    info->fileMap.clear();
    info->name    = "";
    info->desc    = "";
    info->author  = "";
    info->version = "";
    info->id      = "";
    info->active  = false;

    const std::string modDir = modsPath + "/" + folder;

    FileIO *f = fOpen((modDir + "/mod.ini").c_str(), "r");
    if (f) {
        fClose(f);
        auto ini = iniparser_load((modDir + "/mod.ini").c_str());

        info->id     = folder;
        info->active = active;

        info->name    = iniparser_getstring(ini, ":Name", "Unnamed Mod");
        info->desc    = iniparser_getstring(ini, ":Description", "");
        info->author  = iniparser_getstring(ini, ":Author", "Unknown Author");
        info->version = iniparser_getstring(ini, ":Version", "1.0.0");

        // DATA
        fs::path dataPath(modDir + "/Data");

        if (active && fs::exists(dataPath) && fs::is_directory(dataPath)) {
            try {
                auto data_rdi = fs::recursive_directory_iterator(dataPath, fs::directory_options::follow_directory_symlink);
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
                        int32 tokenPos = -1;
                        for (int32 i = 0; i < 4; ++i) {
                            tokenPos = (int32)std::string(modBuf).find(folderTest[i], 0);
                            if (tokenPos >= 0)
                                break;
                        }

                        if (tokenPos >= 0) {
                            char buffer[0x80];
                            for (int32 i = (int32)strlen(modBuf); i >= tokenPos; --i) {
                                buffer[i - tokenPos] = modBuf[i] == '\\' ? '/' : modBuf[i];
                            }

                            // PrintLog(modBuf);
                            std::string path(buffer);
                            std::string modPath(modBuf);
                            char pathLower[0x100];
                            memset(pathLower, 0, sizeof(char) * 0x100);
                            for (int32 c = 0; c < path.size(); ++c) {
                                pathLower[c] = tolower(path.c_str()[c]);
                            }

                            info->fileMap.insert(std::pair<std::string, std::string>(pathLower, modBuf));
                        }
                    }
                }
            } catch (fs::filesystem_error fe) {
                PrintLog(PRINT_ERROR, "Data Folder Scanning Error: %s", fe.what());
            }
        }
        // LOGIC
        std::string logic(iniparser_getstring(ini, ":LogicFile", ""));
        if (logic.length() && info->active) {
            std::istringstream stream(logic);
            std::string buf;
            while (std::getline(stream, buf, ',')) {
                bool32 exists = false;
                buf           = trim(buf);
#if RETRO_PLATFORM == RETRO_WIN
                if (MODAPI_ENDS_WITH(".dll"))
#elif RETRO_PLATFORM == RETRO_OSX
                if (MODAPI_ENDS_WITH(".dylib"))
#elif RETRO_PLATFORM == RETRO_LINUX || RETRO_PLATFORM == RETRO_ANDROID
                if (MODAPI_ENDS_WITH(".so"))
#endif
                    exists = true;
                fs::path file(modDir + "/" + buf);

                if (!exists) {
                    // autodec
                    std::string autodec = "";
#if RETRO_PLATFORM == RETRO_WIN
                    autodec = ".dll";
#elif RETRO_PLATFORM == RETRO_OSX
                    autodec = ".dylib";
#elif RETRO_PLATFORM == RETRO_LINUX || RETRO_PLATFORM == RETRO_ANDROID
                    autodec = ".so";
#elif RETRO_PLATFORM == RETRO_SWITCH
                    autodec = ".nro";
#endif
                    file += autodec;
                    if (fs::exists(file)) {
                        buf += autodec;
                        exists = true;
                    }
                }

                if (!exists) {
                    // can be a lang not seen yet, set the language flag
                    iniparser_freedict(ini);
                    return false;
                }

                bool linked = false;

                modLogicHandle link_handle;
#if RETRO_PLATFORM == RETRO_WIN
                link_handle = LoadLibraryA(file.string().c_str());
#define getAddress GetProcAddress
#elif RETRO_PLATFORM == RETRO_OSX || RETRO_PLATFORM == RETRO_LINUX || RETRO_PLATFORM == RETRO_ANDROID
                std::string fl = file.string().c_str();
#if RETRO_PLATFORM == RETRO_ANDROID
                // only load ones that are compiled. this is to still allow lang mods to work
                fl                = "lib" + buf;
#endif
                link_handle = (void *)dlopen(fl.c_str(), RTLD_LOCAL | RTLD_LAZY);
#define getAddress dlsym
#elif RETRO_PLATFORM == RETRO_SWITCH
                // TODO
                link_handle = NULL;
#define getAddress(x, y) NULL
#endif

                if (link_handle) {
                    modLink linkGameLogic = (modLink)getAddress(link_handle, "LinkModLogic");
                    if (linkGameLogic) {
                        info->linkModLogic.push_back(linkGameLogic);
                        linked = true;
                    }
                    info->unloadMod = (void (*)())getAddress(link_handle, "UnloadMod");
                    info->modLogicHandles.push_back(link_handle);
                }

                if (!linked) {
                    iniparser_freedict(ini);
                    currentMod = cur;
                    return false;
                }
            }
        }
        // SETTINGS
        FileIO *set = fOpen((modDir + "/modSettings.ini").c_str(), "r");
        if (set) {
            fClose(set);
            using namespace std;
            auto ini  = iniparser_load((modDir + "/modSettings.ini").c_str());
            int32 sec = iniparser_getnsec(ini);
            if (sec) {
                for (int32 i = 0; i < sec; ++i) {
                    const char *secn  = iniparser_getsecname(ini, i);
                    int32 len         = iniparser_getsecnkeys(ini, secn);
                    const char **keys = new const char *[len];
                    iniparser_getseckeys(ini, secn, keys);
                    map<string, string> secset;
                    for (int32 j = 0; j < len; ++j)
                        secset.insert(pair<string, string>(keys[j] + strlen(secn) + 1, iniparser_getstring(ini, keys[j], "")));
                    info->settings.insert(pair<string, map<string, string>>(secn, secset));
                }
            }
            else {
                // either you use categories or you don't, i don't make the rules
                map<string, string> secset;
                for (int32 j = 0; j < ini->size; ++j) secset.insert(pair<string, string>(ini->key[j] + 1, ini->val[j]));
                info->settings.insert(pair<string, map<string, string>>("", secset));
            }
            iniparser_freedict(ini);
        }
        // CONFIG
        loadCfg(info, modDir + "/modConfig.cfg");

        std::string cfg(iniparser_getstring(ini, ":ConfigFile", ""));
        bool saveCfg = false;
        if (cfg.length() && info->active) {
            std::istringstream stream(cfg);
            std::string buf;
            while (std::getline(stream, buf, ',')) {
                buf        = trim(buf);
                int32 mode = 0;
                if (MODAPI_ENDS_WITH(".ini"))
                    mode = 1;
                else if (MODAPI_ENDS_WITH(".cfg"))
                    mode = 2;
                fs::path file;

                if (!mode) {
                    file = fs::path(modDir + "/" + buf + ".ini");
                    if (fs::exists(file))
                        mode = 1;
                }
                if (!mode) {
                    file = fs::path(modDir + "/" + buf + ".cfg");
                    if (fs::exists(file))
                        mode = 2;
                }

                // if fail just free do nothing
                if (!mode)
                    continue;
                saveCfg = true;

                if (mode == 1) {
                    FileIO *set = fOpen(file.string().c_str(), "r");
                    if (set) {
                        fClose(set);
                        using namespace std;
                        auto ini  = iniparser_load(file.string().c_str());
                        int32 sec = iniparser_getnsec(ini);
                        for (int32 i = 0; i < sec; ++i) {
                            const char *secn  = iniparser_getsecname(ini, i);
                            int32 len         = iniparser_getsecnkeys(ini, secn);
                            const char **keys = new const char *[len];
                            iniparser_getseckeys(ini, secn, keys);
                            for (int32 j = 0; j < len; ++j) info->config[secn][keys[j] + strlen(secn) + 1] = iniparser_getstring(ini, keys[j], "");
                        }
                        iniparser_freedict(ini);
                    }
                }
                else if (mode == 2)
                    loadCfg(info, file.string());
            }
        }

        if (saveCfg && info->config.size()) {
            FileIO *cfg = fOpen((modDir + "/modConfig.cfg").c_str(), "wb");
            uint8 ct    = info->config.size();
            fWrite(&ct, 1, 1, cfg);
            for (auto kv : info->config) {
                if (!kv.first.length())
                    continue; // don't save no-categories
                uint8 len = kv.first.length();
                fWrite(&len, 1, 1, cfg);
                WriteText(cfg, kv.first.c_str());
                uint8 kt = kv.second.size();
                fWrite(&kt, 1, 1, cfg);
                for (auto kkv : kv.second) {
                    uint8 len    = (uint8)(kkv.first.length()) & 0x7F;
                    bool32 isint = false;
                    int32 r      = 0;
                    try {
                        r     = std::stoi(kkv.second, nullptr, 0);
                        isint = true;
                        len |= 0x80;
                    } catch (...) {
                    }
                    fWrite(&len, 1, 1, cfg);
                    WriteText(cfg, kkv.first.c_str());
                    if (isint)
                        fWrite(&r, sizeof(int32), 1, cfg);
                    else {
                        uint8 len = kkv.second.length();
                        fWrite(&len, 1, 1, cfg);
                        WriteText(cfg, kkv.second.c_str());
                    }
                }
            }
            fClose(cfg);
        }

        PrintLog(PRINT_NORMAL, "[MOD] Loaded mod %s! Active: %s", folder.c_str(), active ? "Y" : "N");

        iniparser_freedict(ini);
        currentMod = cur;
        return true;
    }
    return false;
}

void RSDK::SaveMods()
{
    ModInfo *cur = currentMod;
    char modBuf[0x100];
    sprintf_s(modBuf, (int32)sizeof(modBuf), "%smods/", SKU::userFileDir);
    fs::path modPath(modBuf);

    SortMods();

    PrintLog(PRINT_NORMAL, "[MOD] Saving mods...");

    if (fs::exists(modPath) && fs::is_directory(modPath)) {
        std::string mod_config = modPath.string() + "/modconfig.ini";
        FileIO *file           = fOpen(mod_config.c_str(), "w");

        WriteText(file, "[Mods]\n");

        for (int32 m = 0; m < modList.size(); ++m) {
            currentMod = &modList[m];
            SaveSettings();
            WriteText(file, "%s=%c\n", currentMod->id.c_str(), currentMod->active ? 'y' : 'n');
        }
        fClose(file);
    }
    currentMod = cur;
}

void RSDK::RunModCallbacks(int32 callbackID, void *data)
{
    if (callbackID < 0 || callbackID >= MODCB_MAX)
        return;

    for (auto &c : modCallbackList[callbackID]) {
        if (c)
            c(data);
    }
}

// Mod API
bool32 RSDK::LoadModInfo(const char *id, String *name, String *description, String *version, bool32 *active)
{
    for (int32 m = 0; m < modList.size(); ++m) {
        if (modList[m].id == id) {
            if (description)
                InitString(description, (char *)modList[m].desc.c_str(), false);
            if (description)
                InitString(description, (char *)modList[m].desc.c_str(), false);
            if (version)
                InitString(description, (char *)modList[m].version.c_str(), false);
            if (active)
                *active = modList[m].active;

            return true;
        }
    }
    return false;
}

int32 RSDK::GetModCount(bool32 active)
{
    int32 c = 0;
    for (auto &m : modList) {
        if (++c && active && !m.active)
            return c - 1;
    }
    return c;
}

const char *RSDK::GetModIDByIndex(uint32 index)
{
    if (index >= modList.size())
        return NULL;
    return modList[index].id.c_str();
}

bool32 RSDK::ForeachModID(String *id)
{
    if (!id)
        return false;

    using namespace std;

    if (id->chars)
        ++foreachStackPtr->id;
    else {
        ++foreachStackPtr;
        foreachStackPtr->id = 0;
    }

    if (foreachStackPtr->id >= modList.size()) {
        foreachStackPtr--;
        return false;
    }
    string set = modList[foreachStackPtr->id].id;
    InitString(id, (char *)set.c_str(), (int)set.length());
    return true;
}

void RSDK::AddModCallback(int32 callbackID, ModCallback callback) { return AddModCallback_STD(callbackID, callback); }

void RSDK::AddModCallback_STD(int32 callbackID, ModCallbackSTD callback)
{
    if (callbackID < 0 || callbackID >= MODCB_MAX)
        return;

    modCallbackList[callbackID].push_back(callback);
}

void RSDK::AddPublicFunction(const char *functionName, void *functionPtr)
{
    if (!currentMod)
        return gamePublicFuncs.push_back({ functionName, functionPtr });
    if (!currentMod->active)
        return;
    currentMod->functionList.push_back({ functionName, functionPtr });
}

void *RSDK::GetPublicFunction(const char *id, const char *functionName)
{
    if (!id) {
        for (auto &f : gamePublicFuncs)
            if (f.name == functionName)
                return f.ptr;
        return NULL;
    }
    if (!strlen(id))
        id = currentMod->id.c_str();
    for (ModInfo &m : modList) {
        if (m.active && m.id == id) {
            for (auto &f : m.functionList)
                if (f.name == functionName)
                    return f.ptr;
            return NULL;
        }
    }
    return NULL;
}

void RSDK::GetModPath(const char *id, String *result)
{
    int32 m;
    for (m = 0; m < modList.size(); ++m)
        if (modList[m].active && modList[m].id == id)
            break;
    if (m == modList.size())
        return;

    char buf[0x200];
    sprintf_s(buf, (int32)sizeof(buf), "%smods/%s", SKU::userFileDir, id);
    InitString(result, buf, (int)strlen(buf));
}

std::string GetModPath_i(const char *id)
{
    int32 m;
    for (m = 0; m < modList.size(); ++m)
        if (modList[m].active && modList[m].id == id)
            break;
    if (m == modList.size())
        return std::string();

    return std::string(SKU::userFileDir) + "mods/" + id;
}

std::string GetModSettingsValue(const char *id, const char *key)
{
    std::string skey(key);
    if (!strchr(key, ':'))
        skey = std::string(":") + key;

    std::string cat  = skey.substr(0, skey.find(":"));
    std::string rkey = skey.substr(skey.find(":") + 1);

    for (ModInfo &m : modList) {
        if (m.active && m.id == id) {
            try {
                return m.settings.at(cat).at(rkey);
            } catch (std::out_of_range) {
                return std::string();
            }
        }
    }
    return std::string();
}

bool32 RSDK::GetSettingsBool(const char *id, const char *key, bool32 fallback)
{
    if (!id) {
        if (!currentMod)
            return fallback;
        id = currentMod->id.c_str();
    }
    std::string v = GetModSettingsValue(id, key);
    if (!v.length()) {
        if (currentMod->id == id)
            SetSettingsBool(key, fallback);
        return fallback;
    }
    char first = v.at(0);
    if (first == 'y' || first == 'Y' || first == 't' || first == 'T' || (first = GetSettingsInteger(id, key, 0)))
        return true;
    if (first == 'n' || first == 'N' || first == 'f' || first == 'F' || !first)
        return false;
    if (currentMod->id == id)
        SetSettingsBool(key, fallback);
    return fallback;
}

int32 RSDK::GetSettingsInteger(const char *id, const char *key, int32 fallback)
{
    if (!id) {
        if (!currentMod)
            return fallback;
        id = currentMod->id.c_str();
    }
    std::string v = GetModSettingsValue(id, key);
    if (!v.length()) {
        if (currentMod->id == id)
            SetSettingsInteger(key, fallback);
        return fallback;
    }
    try {
        return std::stoi(v, nullptr, 0);
    } catch (...) {
        if (currentMod->id == id)
            SetSettingsInteger(key, fallback);
        return fallback;
    }
}

void RSDK::GetSettingsString(const char *id, const char *key, String *result, const char *fallback)
{
    if (!id) {
        if (!currentMod) {
            InitString(result, (char *)fallback, (int)strlen(fallback));
            return;
        }
        id = currentMod->id.c_str();
    }

    std::string v = GetModSettingsValue(id, key);
    if (!v.length()) {
        if (currentMod->id == id)
            SetSettingsString(key, result);
        InitString(result, (char *)fallback, (int)strlen(fallback));
        return;
    }
    InitString(result, (char *)v.c_str(), (int)v.length());
}

std::string GetNidConfigValue(const char *key)
{
    if (!currentMod || !currentMod->active)
        return std::string();
    std::string skey(key);
    if (!strchr(key, ':'))
        skey = std::string(":") + key;

    std::string cat  = skey.substr(0, skey.find(":"));
    std::string rkey = skey.substr(skey.find(":") + 1);

    try {
        return currentMod->config.at(cat).at(rkey);
    } catch (std::out_of_range) {
        return std::string();
    }
    return std::string();
}

bool32 RSDK::GetConfigBool(const char *key, bool32 fallback)
{
    std::string v = GetNidConfigValue(key);
    if (!v.length())
        return fallback;
    char first = v.at(0);
    if (first == 'y' || first == 'Y' || first == 't' || first == 'T' || (first = GetConfigInteger(key, 0)))
        return true;
    if (first == 'n' || first == 'N' || first == 'f' || first == 'F' || !first)
        return false;
    return fallback;
}

int32 RSDK::GetConfigInteger(const char *key, int32 fallback)
{
    std::string v = GetNidConfigValue(key);
    if (!v.length())
        return fallback;
    try {
        return std::stoi(v, nullptr, 0);
    } catch (...) {
        return fallback;
    }
}

void RSDK::GetConfigString(const char *key, String *result, const char *fallback)
{
    std::string v = GetNidConfigValue(key);
    if (!v.length()) {
        InitString(result, (char *)fallback, (int)strlen(fallback));
        return;
    }
    InitString(result, (char *)v.c_str(), (int)v.length());
}

bool32 RSDK::ForeachConfigCategory(String *category)
{
    if (!category || !currentMod)
        return false;

    using namespace std;
    if (!currentMod->config.size())
        return false;

    if (category->chars)
        ++foreachStackPtr->id;
    else {
        ++foreachStackPtr;
        foreachStackPtr->id = 0;
    }
    int32 sid = 0;
    string cat;
    bool32 set = false;
    if (currentMod->config[""].size() && foreachStackPtr->id == sid++) {
        set = true;
        cat = "";
    }
    if (!set) {
        for (pair<string, map<string, string>> kv : currentMod->config) {
            if (!kv.first.length())
                continue;
            if (kv.second.size() && foreachStackPtr->id == sid++) {
                set = true;
                cat = kv.first;
                break;
            }
        }
    }
    if (!set) {
        foreachStackPtr--;
        return false;
    }
    InitString(category, (char *)cat.c_str(), (int)cat.length());
    return true;
}

bool32 RSDK::ForeachConfig(String *config)
{
    if (!config || !currentMod)
        return false;
    using namespace std;
    if (!currentMod->config.size())
        return false;

    if (config->chars)
        ++foreachStackPtr->id;
    else {
        ++foreachStackPtr;
        foreachStackPtr->id = 0;
    }
    int32 sid = 0;
    string key, cat;
    if (currentMod->config[""].size()) {
        for (pair<string, string> pair : currentMod->config[""]) {
            if (foreachStackPtr->id == sid++) {
                cat = "";
                key = pair.first;
                break;
            }
        }
    }
    if (!key.length()) {
        for (pair<string, map<string, string>> kv : currentMod->config) {
            if (!kv.first.length())
                continue;
            for (pair<string, string> pair : kv.second) {
                if (foreachStackPtr->id == sid++) {
                    cat = kv.first;
                    key = pair.first;
                    break;
                }
            }
        }
    }
    if (!key.length()) {
        foreachStackPtr--;
        return false;
    }
    string r = cat + ":" + key;
    InitString(config, (char *)r.c_str(), (int)r.length());
    return true;
}

void SetModSettingsValue(const char *key, std::string val)
{
    if (!currentMod)
        return;
    std::string skey(key);
    if (!strchr(key, ':'))
        skey = std::string(":") + key;

    std::string cat  = skey.substr(0, skey.find(":"));
    std::string rkey = skey.substr(skey.find(":") + 1);

    currentMod->settings[cat][rkey] = val;
}

void RSDK::SetSettingsBool(const char *key, bool32 val) { SetModSettingsValue(key, val ? "Y" : "N"); }
void RSDK::SetSettingsInteger(const char *key, int32 val) { SetModSettingsValue(key, std::to_string(val)); }
void RSDK::SetSettingsString(const char *key, String *val)
{
    char *buf = new char[val->length];
    GetCString(buf, val);
    SetModSettingsValue(key, buf);
}

void RSDK::SaveSettings()
{
    using namespace std;
    if (!currentMod || !currentMod->settings.size())
        return;

    FileIO *file = fOpen((GetModPath_i(currentMod->id.c_str()) + "/modSettings.ini").c_str(), "w");

    if (currentMod->settings[""].size()) {
        for (pair<string, string> pair : currentMod->settings[""]) WriteText(file, "%s = %s\n", pair.first.c_str(), pair.second.c_str());
    }
    for (pair<string, map<string, string>> kv : currentMod->settings) {
        if (!kv.first.length())
            continue;
        WriteText(file, "\n[%s]\n", kv.first.c_str());
        for (pair<string, string> pair : kv.second) WriteText(file, "%s = %s\n", pair.first.c_str(), pair.second.c_str());
    }
    fClose(file);
    PrintLog(PRINT_NORMAL, "[MOD] Saved mod settings for mod %s", currentMod->id.c_str());
    return;
}

// i'm going to hell for this
// nvm im actually so proud of this func yall have no idea i'm insane
int32 superLevels = 1;
void SuperInternal(RSDK::ObjectClass *super, RSDK::ModSuper callback, void *data)
{
    using namespace RSDK;

    Object *before  = NULL;
    ModInfo *curMod = currentMod;
    if (HASH_MATCH_MD5(super->hash, super->inherited->hash)) {
        for (int32 i = 0; i < superLevels; i++) {
            before = *super->staticVars;
            if (!super->inherited) {
                superLevels = i;
                break;
            }
            // if overriding, force it all to be that object and don't set it back
            *super->inherited->staticVars = *super->staticVars;
            super                         = super->inherited;
        }
        ++superLevels;
    }
    else if (super->inherited) {
        // if we're just inheriting, set it back properly afterward
        before                        = *super->inherited->staticVars;
        *super->inherited->staticVars = *super->staticVars;
        super                         = super->inherited;
        superLevels                   = 1;
    }

    switch (callback) {
        case SUPER_CREATE:
            if (super->create)
                super->create(data);
            break;

        case SUPER_DRAW:
            if (super->draw)
                super->draw();
            break;

        case SUPER_UPDATE:
            if (super->update)
                super->update();
            break;

        case SUPER_STAGELOAD:
            if (super->stageLoad)
                super->stageLoad();
            break;

        case SUPER_LATEUPDATE:
            if (super->lateUpdate)
                super->lateUpdate();
            break;

        case SUPER_STATICUPDATE:
            if (super->staticUpdate)
                super->staticUpdate();
            break;

        case SUPER_SERIALIZE:
            if (super->serialize)
                super->serialize();
            break;

        case SUPER_EDITORLOAD:
            if (super->editorLoad)
                super->editorLoad();
            break;

        case SUPER_EDITORDRAW:
            if (super->editorDraw)
                super->editorDraw();
            break;
    }

    *super->staticVars = before;
    superLevels        = 1;
    currentMod         = curMod;
}

void RSDK::Super(int32 objectID, ModSuper callback, void *data) { return SuperInternal(&objectClassList[stageObjectIDs[objectID]], callback, data); }

void *RSDK::GetGlobals() { return (void *)globalVarsPtr; }

void RSDK::ModRegisterGlobalVariables(const char *globalsPath, void **globals, uint32 size)
{
    AllocateStorage(size, globals, DATASET_STG, true);
    FileInfo info;
    InitFileInfo(&info);

    int32 *varPtr = *(int32 **)globals;
    if (LoadFile(&info, globalsPath, FMODE_RB)) {
        uint8 varCount = ReadInt8(&info);
        for (int32 i = 0; i < varCount && globalVarsPtr; ++i) {
            int32 offset = ReadInt32(&info, false);
            int32 count  = ReadInt32(&info, false);
            for (int32 v = 0; v < count; ++v) {
                varPtr[offset + v] = ReadInt32(&info, false);
            }
        }

        CloseFile(&info);
    }
}

void RSDK::ModRegisterObject(Object **structPtr, const char *name, uint32 entitySize, uint32 objectSize, void (*update)(), void (*lateUpdate)(),
                             void (*staticUpdate)(), void (*draw)(), void (*create)(void *), void (*stageLoad)(), void (*editorDraw)(),
                             void (*editorLoad)(), void (*serialize)(), const char *inherited)
{
    return ModRegisterObject_STD(structPtr, name, entitySize, objectSize, update, lateUpdate, staticUpdate, draw, create, stageLoad, editorDraw,
                                 editorLoad, serialize, inherited);
}

void RSDK::ModRegisterObject_STD(Object **structPtr, const char *name, uint32 entitySize, uint32 objectSize, std::function<void()> update,
                                 std::function<void()> lateUpdate, std::function<void()> staticUpdate, std::function<void()> draw,
                                 std::function<void(void *)> create, std::function<void()> stageLoad, std::function<void()> editorDraw,
                                 std::function<void()> editorLoad, std::function<void()> serialize, const char *inherited)
{
    // TODO: i think i introduced a memleak somewhere here??

    ModInfo *curMod = currentMod;
    int32 preCount  = objectClassCount + 1;
    RETRO_HASH_MD5(hash);
    GEN_HASH_MD5(name, hash);

    int32 superSlot      = preCount;
    ObjectClass *inherit = NULL;
    for (int32 i = 0; i < objectClassCount; ++i) {
        if (HASH_MATCH_MD5(objectClassList[i].hash, hash)) {
            objectClassCount = i;
            superSlot        = i;
            inherit          = new ObjectClass(objectClassList[i]);
            --preCount;
            if (!inherited)
                inherited = name;
            break;
        }
    }

    if (inherited) {
        RETRO_HASH_MD5(hash);
        GEN_HASH_MD5(inherited, hash);
        if (!inherit)
            for (int32 i = 0; i < preCount; ++i) {
                if (HASH_MATCH_MD5(objectClassList[i].hash, hash)) {
                    inherit = new ObjectClass(objectClassList[i]);
                    break;
                }
            }

        if (!inherit)
            inherited = NULL;
    }

    RegisterObject_STD(structPtr, name, entitySize, objectSize, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);

    ObjectClass *info = &objectClassList[objectClassCount - 1];
    if (inherited) {
        info->inherited = inherit;

        if (HASH_MATCH_MD5(info->hash, inherit->hash)) {
            // we override an obj and lets check for structPtr
            if (!info->staticVars) {
                info->staticVars      = inherit->staticVars;
                info->staticClassSize = inherit->staticClassSize;
            }
        }

        ObjectClass *copy = new ObjectClass(*info);

        // clang-format off
        if (!update)       info->update       = [curMod, copy]()           { currentMod = curMod; SuperInternal(copy, SUPER_UPDATE, NULL);       currentMod = NULL; };
        if (!lateUpdate)   info->lateUpdate   = [curMod, copy]()           { currentMod = curMod; SuperInternal(copy, SUPER_LATEUPDATE, NULL);   currentMod = NULL; };
        if (!staticUpdate) info->staticUpdate = [curMod, copy]()           { currentMod = curMod; SuperInternal(copy, SUPER_STATICUPDATE, NULL); currentMod = NULL; };
        if (!draw)         info->draw         = [curMod, copy]()           { currentMod = curMod; SuperInternal(copy, SUPER_DRAW, NULL);         currentMod = NULL; };
        if (!stageLoad)    info->stageLoad    = [curMod, copy]()           { currentMod = curMod; SuperInternal(copy, SUPER_STAGELOAD, NULL);    currentMod = NULL; };
        if (!serialize)    info->serialize    = [curMod, copy]()           { currentMod = curMod; SuperInternal(copy, SUPER_SERIALIZE, NULL);    currentMod = NULL; };
        if (!editorDraw)   info->editorDraw   = [curMod, copy]()           { currentMod = curMod; SuperInternal(copy, SUPER_EDITORDRAW, NULL);   currentMod = NULL; };
        if (!editorLoad)   info->editorLoad   = [curMod, copy]()           { currentMod = curMod; SuperInternal(copy, SUPER_EDITORLOAD, NULL);   currentMod = NULL; };
        if (!create)       info->create       = [curMod, copy](void* data) { currentMod = curMod; SuperInternal(copy, SUPER_CREATE, data);       currentMod = NULL; };
        // clang-format on
    }

    // clang-format off
    if (update)       info->update       = [curMod, update]()           { currentMod = curMod; update();       currentMod = NULL; };
    if (lateUpdate)   info->lateUpdate   = [curMod, lateUpdate]()       { currentMod = curMod; lateUpdate();   currentMod = NULL; };
    if (staticUpdate) info->staticUpdate = [curMod, staticUpdate]()     { currentMod = curMod; staticUpdate(); currentMod = NULL; };
    if (draw)         info->draw         = [curMod, draw]()             { currentMod = curMod; draw();         currentMod = NULL; };
    if (stageLoad)    info->stageLoad    = [curMod, stageLoad]()        { currentMod = curMod; stageLoad();    currentMod = NULL; };
    if (serialize)    info->serialize    = [curMod, serialize]()        { currentMod = curMod; serialize();    currentMod = NULL; };
    if (editorDraw)   info->editorDraw   = [curMod, editorDraw]()       { currentMod = curMod; editorDraw();   currentMod = NULL; };
    if (editorLoad)   info->editorLoad   = [curMod, editorLoad]()       { currentMod = curMod; editorLoad();   currentMod = NULL; };
    if (create)       info->create       = [curMod, create](void* data) { currentMod = curMod; create(data);   currentMod = NULL; };
    // clang-format on

    objectClassCount = preCount;
}

Object *RSDK::GetObject(const char *name)
{
    if (int32 o = FindObject(name))
        return *objectClassList[stageObjectIDs[o]].staticVars;

    return NULL;
}

void RSDK::GetAchievementInfo(uint32 id, String *name, String *description, String *identifer, bool32 *achieved)
{
    if (id >= achievementList.size())
        return;

    if (name)
        InitString(name, (char *)achievementList[id].name.c_str(), 0);

    if (description)
        InitString(description, (char *)achievementList[id].description.c_str(), 0);

    if (identifer)
        InitString(identifer, (char *)achievementList[id].identifier.c_str(), 0);

    if (achieved)
        *achieved = achievementList[id].achieved;
}

int32 RSDK::GetAchievementIndexByID(const char *id)
{
    for (int32 i = 0; i < achievementList.size(); ++i) {
        if (achievementList[i].identifier == std::string(id))
            return i;
    }

    return -1;
}
int32 RSDK::GetAchievementCount() { return (int)achievementList.size(); }
#endif

// Start custom achievement code
// this is added because we don't have access to any store APIs that would otherwise use this featur
void RSDK::RegisterAchievement(const char *identifier, const char *name, const char *desc)
{
    AchievementInfo info;
    info.identifier  = identifier;
    info.name        = name;
    info.description = desc;
    info.achieved    = false;
    achievementList.push_back(info);
}

// End custom achievement code
