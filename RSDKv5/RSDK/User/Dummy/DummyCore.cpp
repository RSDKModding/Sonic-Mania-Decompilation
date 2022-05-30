using namespace RSDK;

#if RETRO_REV02
SKU::DummyCore *RSDK::SKU::dummyCore = NULL;
#endif

uint32 RSDK::SKU::GetAPIValueID(const char *identifier, int32 charIndex)
{
    if (identifier[charIndex])
        return (33 * GetAPIValueID(identifier, charIndex + 1)) ^ identifier[charIndex];
    else
        return 5381;
}

int32 RSDK::SKU::GetAPIValue(uint32 id)
{
    switch (id) {
        default: break;

        case 0x3D6BD740: // SYSTEM_PLATFORM

            // Platform Overrides
#if RETRO_PLATFORM == RETRO_PS4
            return PLATFORM_PS4;
#elif RETRO_PLATFORM == RETRO_XB1
            return PLATFORM_XB1;
#elif RETRO_PLATFORM == RETRO_SWITCH || RETRO_PLATFORM == RETRO_ANDROID
            return PLATFORM_SWITCH;
#endif

            // default to PC (or dev if dev stuff is enabled)
            return engine.devMenu ? PLATFORM_DEV : PLATFORM_PC;

        case 0xD9F55367: return REGION_US; // SYSTEM_REGION

        case 0x0CC0762D: return LANGUAGE_EN; // SYSTEM_LANGUAGE

        case 0xA2ACEF21: return engine.confirmFlip; // SYSTEM_CONFIRM_FLIP

        case 0x4205582D: return 120; // SYSTEM_LEADERBOARD_LOAD_TIME

        case 0xDEF3F8B5: return STATUS_OK; // SYSTEM_LEADERBOARD_STATUS

        case 0x5AD68EAB: return STATUS_OK; // SYSTEM_USERSTORAGE_AUTH_STATUS

        case 0x884E705A: return STATUS_OK; // SYSTEM_USERSTORAGE_STORAGE_STATUS

        case 0xBDF4E94A: return 30; // SYSTEM_USERSTORAGE_AUTH_TIME

        case 0xD77E98BE: return 30; // SYSTEM_USERSTORAGE_STORAGE_INIT_TIME

        case 0x5AF715C2: return 30; // SYSTEM_USERSTORAGE_STORAGE_LOAD_TIME

        case 0x54378EC5: return 30; // SYSTEM_USERSTORAGE_STORAGE_SAVE_TIME

        case 0xCD44607D: return 30; // SYSTEM_USERSTORAGE_STORAGE_DELETE_TIME
    }

    return 0;
}

#if RETRO_REV02

SKU::DummyCore *RSDK::SKU::InitDummyCore()
{
    // Initalize API subsystems
    DummyCore *core = new DummyCore;

    if (achievements)
        delete achievements;
    achievements = new DummyAchievements;

    if (leaderboards)
        delete leaderboards;
    leaderboards = new DummyLeaderboards;

    if (richPresence)
        delete richPresence;
    richPresence = new DummyRichPresence;

    if (stats)
        delete stats;
    stats = new DummyStats;

    if (userStorage)
        delete userStorage;
    userStorage = new DummyUserStorage;

    // Setup default values

    core->values[0]  = &engine.hasPlus;
    core->valueCount = 1;

    leaderboards->userRank = 0;
    leaderboards->isUser   = false;

    achievements->enabled      = true;
    leaderboards->status       = GetAPIValue(GetAPIValueID("SYSTEM_LEADERBOARD_STATUS", 0));
    stats->enabled             = true;
    userStorage->authStatus    = STATUS_NONE;
    userStorage->storageStatus = STATUS_NONE;
    userStorage->saveStatus    = STATUS_NONE;
    userStorage->noSaveActive  = false;

    return core;
}

void RSDK::SKU::HandleUserStatuses()
{
    if (userStorage) {
        if (userStorage->authStatus == STATUS_CONTINUE) {
            if (API_TypeOf(userStorage, DummyUserStorage)->authTime)
                API_TypeOf(userStorage, DummyUserStorage)->authTime--;
            else
                userStorage->authStatus = STATUS_OK;
        }

        if (userStorage->storageStatus == STATUS_CONTINUE) {
            if (API_TypeOf(userStorage, DummyUserStorage)->storageInitTime)
                API_TypeOf(userStorage, DummyUserStorage)->storageInitTime--;
            else
                userStorage->storageStatus = STATUS_OK;
        }
    }

    if (leaderboards) {
        if (leaderboards->status == STATUS_CONTINUE) {
            if (API_TypeOf(leaderboards, DummyLeaderboards)->loadTime) {
                API_TypeOf(leaderboards, DummyLeaderboards)->loadTime--;
            }
            else {
                leaderboards->status = STATUS_OK;

                if (!leaderboards->isUser) {
                    leaderboards->entryInfo.entryStart.start  = 1;
                    leaderboards->entryInfo.entryStart.length = 20;
                    leaderboards->entryInfo.globalRankOffset  = 1;
                    leaderboards->entryInfo.entryCount.start  = 20;
                }
                else {
                    leaderboards->entryInfo.entryStart.start  = leaderboards->userRank - 10;
                    leaderboards->entryInfo.entryStart.length = 20;
                    leaderboards->entryInfo.globalRankOffset  = leaderboards->userRank - 10;
                    leaderboards->entryInfo.entryCount.start  = 20;
                }

                FillDummyLeaderboardEntries();
            }
        }

        if (API_TypeOf(leaderboards, DummyLeaderboards)->trackTime != 0) {
            API_TypeOf(leaderboards, DummyLeaderboards)->trackTime--;
        }
        else {
            leaderboards->status = STATUS_OK;

            if (API_TypeOf(leaderboards, DummyLeaderboards)->trackCB) {
                API_TypeOf(leaderboards, DummyLeaderboards)->trackCB(true, API_TypeOf(leaderboards, DummyLeaderboards)->trackRank);
            }

            API_TypeOf(leaderboards, DummyLeaderboards)->trackTime = -1;
            API_TypeOf(leaderboards, DummyLeaderboards)->trackCB   = NULL;
        }
    }
}
#endif

#if RETRO_REV02
const char *userValueNames[8] = { "Ext <PLUS>" };
void RSDK::SKU::DummyCore::StageLoad()
{
    UserCore::StageLoad();

    for (int32 v = 0; v < valueCount; ++v) AddViewableVariable(userValueNames[v], values[v], VIEWVAR_BOOL, false, true);
}

bool32 RSDK::SKU::DummyCore::CheckFocusLost() { return focusState != 0; }

int32 RSDK::SKU::DummyCore::GetUserLanguage() { return GetAPIValue(GetAPIValueID("SYSTEM_LANGUAGE", 0)); }
int32 RSDK::SKU::DummyCore::GetUserRegion() { return GetAPIValue(GetAPIValueID("SYSTEM_REGION", 0)); }
int32 RSDK::SKU::DummyCore::GetUserPlatform() { return GetAPIValue(GetAPIValueID("SYSTEM_PLATFORM", 0)); }
bool32 RSDK::SKU::DummyCore::GetConfirmButtonFlip() { return GetAPIValue(GetAPIValueID("SYSTEM_CONFIRM_FLIP", 0)); }

void RSDK::SKU::DummyCore::LaunchManual()
{
    // LaunchManual() just opens the mania manual URL, thats it
#if RETRO_RENDERDEVICE_SDL2
    // SDL_OpenURL("http://www.sonicthehedgehog.com/mania/manual");
    PrintLog(PRINT_NORMAL, "DUMMY LaunchManual()");
#else
    PrintLog(PRINT_NORMAL, "EMPTY LaunchManual()");
#endif
}
void RSDK::SKU::DummyCore::ExitGame() { RenderDevice::isRunning = false; }

int32 RSDK::SKU::DummyCore::GetDefaultGamepadType()
{
#if RETRO_REV02
    int32 platform = curSKU.platform;
#else
    int32 platform = gameVerInfo.platform;
#endif

    switch (platform) {
        case PLATFORM_SWITCH:
#if RETRO_INPUTDEVICE_NX
            return currentNXControllerType;
#else
            return (DEVICE_API_NONE << 16) | (DEVICE_TYPE_CONTROLLER << 8) | (DEVICE_SWITCH_HANDHELD << 0);
#endif

        default:
        case PLATFORM_PS4:
        case PLATFORM_XB1:
        case PLATFORM_PC:
        case PLATFORM_DEV: return (DEVICE_API_NONE << 16) | (DEVICE_TYPE_CONTROLLER << 8) | (0 << 0); break;
    }
}

int32 RSDK::SKU::DummyCore::ShowExtensionOverlay(uint8 overlay)
{
    switch (overlay) {
        default: PrintLog(PRINT_POPUP, "Show Extension Overlay: %d", overlay); break;
        case 0: PrintLog(PRINT_POPUP, "Show Extension Overlay: %d (Plus Upsell Screen)", overlay); break;
    }
    return 1;
}
#endif

#if RETRO_VER_EGS
bool32 RSDK::SKU::DummyCore::ShowCheckoutPage(int32 a1)
{
    PrintLog(PRINT_POPUP, "ShowCheckoutPage(%d)");
    return true;
}
int32 RSDK::SKU::DummyCore::ShowEncorePage(int32 a1)
{
    PrintLog(PRINT_POPUP, "Show EncorePage Overlay: %d", a1);
    return 1;
}
void RSDK::SKU::DummyCore::EpicUnknown4(int32 a1) { PrintLog(PRINT_POPUP, "EpicUnknown4(%d)", a1); }
#endif