// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: APICallback Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

#if !MANIA_USE_PLUS
ObjectAPICallback *APICallback;

void APICallback_Update(void)
{
    RSDK_THIS(APICallback);
    StateMachine_Run(self->state);
}

void APICallback_LateUpdate(void) {}

void APICallback_StaticUpdate(void) {}

void APICallback_Draw(void) {}

void APICallback_Create(void *data)
{
    RSDK_THIS(APICallback);

    self->active  = ACTIVE_ALWAYS;
    self->visible = false;
    self->state   = (Type_StateMachine)data;
}

void APICallback_StageLoad(void)
{
    APICallback->achievementsDisabled = false;
    APICallback->active               = ACTIVE_ALWAYS;

    APICallback->LaunchManual                 = RSDK.GetAPIFunction("LaunchManual");
    APICallback->ExitGame                     = RSDK.GetAPIFunction("ExitGame");
    APICallback->ClearAchievements            = RSDK.GetAPIFunction("ClearAchievements");
    APICallback->UnlockAchievement            = RSDK.GetAPIFunction("UnlockAchievement");
    APICallback->SetRichPresence              = RSDK.GetAPIFunction("SetRichPresence");
    APICallback->FetchLeaderboard             = RSDK.GetAPIFunction("FetchLeaderboard");
    APICallback->LeaderboardStatus            = RSDK.GetAPIFunction("LeaderboardStatus");
    APICallback->LeaderboardEntryCount        = RSDK.GetAPIFunction("LeaderboardEntryCount");
    APICallback->LeaderboardReadEntry         = RSDK.GetAPIFunction("LeaderboardReadEntry");
    APICallback->TrackActClear                = RSDK.GetAPIFunction("TrackActClear");
    APICallback->TrackTAClear                 = RSDK.GetAPIFunction("TrackTAClear");
    APICallback->TrackEnemyDefeat             = RSDK.GetAPIFunction("TrackEnemyDefeat");
    APICallback->TrackGameProgress            = RSDK.GetAPIFunction("TrackGameProgress");
    APICallback->ClearPrerollErrors           = RSDK.GetAPIFunction("ClearPrerollErrors");
    APICallback->TryAuth                      = RSDK.GetAPIFunction("TryAuth");
    APICallback->GetUserAuthStatus            = RSDK.GetAPIFunction("GetUserAuthStatus");
    APICallback->GetUsername                  = RSDK.GetAPIFunction("GetUsername");
    APICallback->TryInitStorage               = RSDK.GetAPIFunction("TryInitStorage");
    APICallback->GetStorageStatus             = RSDK.GetAPIFunction("GetStorageStatus");
    APICallback->LoadUserFile                 = RSDK.GetAPIFunction("LoadUserFile");
    APICallback->SaveUserFile                 = RSDK.GetAPIFunction("SaveUserFile");
    APICallback->SaveSettingsINI              = RSDK.GetAPIFunction("SaveSettingsINI");
    APICallback->GetUserLanguage              = RSDK.GetAPIFunction("GetUserLanguage");
    APICallback->GetConfirmButtonFlip         = RSDK.GetAPIFunction("GetConfirmButtonFlip");
    APICallback->ControllerIDForInputID       = RSDK.GetAPIFunction("ControllerIDForInputID");
    APICallback->MostRecentActiveControllerID = RSDK.GetAPIFunction("MostRecentActiveControllerID");
    APICallback->AssignControllerID           = RSDK.GetAPIFunction("AssignControllerID");
    APICallback->ResetControllerAssignments   = RSDK.GetAPIFunction("ResetControllerAssignments");
    APICallback->InputIDIsDisconnected        = RSDK.GetAPIFunction("InputIDIsDisconnected");
    APICallback->GetInputType                 = RSDK.GetAPIFunction("GetInputType");
    APICallback->GetControllerType            = RSDK.GetAPIFunction("GetControllerType");
    APICallback->ShowSteamControllerOverlay   = RSDK.GetAPIFunction("ShowSteamControllerOverlay");

    APICallback->inputSlots[0]   = CONT_P1;
    APICallback->inputSlots[1]   = CONT_P2;
    APICallback->inputSlots[2]   = CONT_P3;
    APICallback->inputSlots[3]   = CONT_P4;
    APICallback->controllerCount = 4;
}

void APICallback_SetRichPresence(int32 id, String *msg)
{
    if (globals->presenceID != id) {
        char buffer[0x40];
        RSDK.GetCString(buffer, msg);

        if (APICallback->SetRichPresence) {
            LogHelpers_Print("API SetRichPresence(%d)", id);
            LogHelpers_Print("%s", buffer);

            APICallback->SetRichPresence(id, msg);
        }
        else {
            LogHelpers_Print("EMPTY SetRichPresence(%d)", id);
            LogHelpers_Print("%s", buffer);
        }
    }
}

int32 APICallback_GetUserLanguage(void)
{
    if (APICallback->GetUserLanguage) {
        return APICallback->GetUserLanguage();
    }
    else {
        LogHelpers_Print("EMPTY GetUserLanguage() -> GameInfo->language (%d)", sku_language);
        return sku_language;
    }
}

bool32 APICallback_GetConfirmButtonFlip(void)
{
    if (APICallback->GetConfirmButtonFlip)
        return APICallback->GetConfirmButtonFlip();

    // LogHelpers_Print("DUMMY GetConfirmButtonFlip()");
    return false;
}

void APICallback_SetNoSaveEnabled(void)
{
    APICallback->saveStatus = STATUS_ERROR;
    globals->noSave         = true;
}

void APICallback_SetNoSaveDisabled(void)
{
    APICallback->saveStatus = STATUS_FORBIDDEN;
    globals->noSave         = false;
}

void APICallback_SaveUserFile(const char *name, void *buffer, int32 size, void (*callback)(int32))
{
    if (API_GetNoSave()) {
        LogHelpers_Print("SaveUserFile(%s, %X, %d, %X) failing due to noSave", name, buffer, size, callback);
        callback(false);
    }
    else if (APICallback->SaveUserFile) {
        LogHelpers_Print("API SaveUserFile(%s, %X, %d, %X)", name, buffer, size, callback);
        APICallback->SaveUserFile(name, buffer, size, callback);
    }
    else {
        LogHelpers_Print("DUMMY SaveUserFile(%s, %X, %d)", name, buffer, size);
        EntityAPICallback *entity = CREATE_ENTITY(APICallback, APICallback_HandleCallback, 0, 0);
        entity->callback          = APICallback_SaveCB;
        entity->timer             = 30;
        entity->fileSize          = size;
        entity->fileName          = name;
        entity->fileBuffer        = buffer;
        entity->fileCallback      = callback;
    }
}

void APICallback_SaveCB(void)
{
    RSDK_THIS(APICallback);

    int32 saveResult = RSDK.SaveUserFile(self->fileName, self->fileBuffer, self->fileSize);
    LogHelpers_Print("DUMMY DummySaveCB(%s, %x, %d) -> %d", self->fileName, self->fileBuffer, self->fileSize, saveResult);

    if (self->fileCallback)
        self->fileCallback(true);
}

void APICallback_SaveSettingsINI(void)
{
    if (APICallback->SetRichPresence) {
        LogHelpers_Print("API SaveSettingsINI(%d)");
        APICallback->SaveSettingsINI();
    }
    else {
        LogHelpers_Print("EMPTY SaveSettingsINI()");
    }
}

LeaderboardEntry *APICallback_ReadLeaderboardEntry(int32 rankID)
{
    if (APICallback->LeaderboardReadEntry)
        return APICallback->LeaderboardReadEntry(rankID);

    if (!sku_platform) {
        LogHelpers_Print("EMPTY LeaderboardReadEntry()");
        return NULL;
    }

    LogHelpers_Print("DUMMY LeaderboardReadEntry()");

    if (APICallback->leaderboardsStatus != STATUS_OK || rankID < 0 || rankID >= APICallback->leaderboardEntryCount) {
        LogHelpers_Print("ERROR index out of bounds for RankEntry");
        return NULL;
    }

    const char *dummyNames[] = { "ORCIHILLARY124",      "AUCTORJOLIE521",       "SENECTUSFLORENCE789", "MAGNAAVRAM503",       "SITVERNON320",
                                 "DUICHRISTEN429",      "NULLAKERMIT649",       "INTEGERGEORGE708",    "HENDRERITDREW443",    "UTULYSSES507",
                                 "ACCUMSANBRUCE276",    "BANANAARON804",        "MAURISSILAS372",      "ETCALVIN641",         "UTGALENA780",
                                 "FEUGIATSHAY665",      "ORCIJULIET388",        "ETJENNA318",          "LIBEROIDOLA420",      "ATVIELKA576",
                                 "PHASELLUSJENETTE627", "CONSECTETUERCALEB550", "SITVICTOR359",        "PURUSDARRYL50",       "DONECAVRAM56",
                                 "PENATIBUSSYBILL30",   "MAURISBARCLAY563",     "CONGUECOLLEEN811",    "MORBIDESTINY655",     "SEMPERIONA579",
                                 "RISUSKEANE807",       "QUISQUEMARYAM927",     "COMMODOSTEPHANIE54",  "MALESUADABURTON350",  "VIVAMUSROBIN330",
                                 "QUISQUEWHILEMINA413", "VESTIBULUMRYDER890",   "LACUSINDIRA112",      "NEQUEJESCIE446",      "EGESTASMORGAN390",
                                 "ETODYSSEUS95",        "EGETHERMIONE127",      "ENIMPASCALE599",      "ODIOWHOOPI328",       "DOLORANGELICA718",
                                 "ODIOSLOANE426",       "DUIZELDA319",          "METUSPORTER906",      "NONUMMYSHANNON166",   "IPSUMANTHONY906",
                                 "ACCUMSANRAPHAEL296",  "ORNAREHU604",          "MOLESTIERYDER304",    "FACILISILAITH900",    "MIYVONNE287",
                                 "CRASHEDY110",         "TEMPUSRAE486",         "ORCICOLBY173",        "ULTRICESJADEN480",    "LECTUSBEAU552",
                                 "LIGULAJESSAMINE824",  "ORCISOLOMON19",        "VARIUSDARRYL437",     "EUISMODKATELL668",    "LACUSSTEVEN302",
                                 "ALIQUETRICHARD807",   "PEDEKAI692",           "VIVAMUSCHARLOTTE167", "MAURISLEWIS101",      "NULLANELLE11",
                                 "MAECENASKAMEKO602",   "LOREMHALEY545",        "PHASELLUSPAUL954",    "ERATGLORIA527",       "DIAMBURKE290",
                                 "SEMYOKO792",          "ANTEMICHELLE769",      "SEMPERDOMINIC873",    "CONDIMENTUMNYSSA826", "PEDEDYLAN675",
                                 "ETCRUZ380",           "VELJAQUELYN95",        "NONDACEY190",         "UTCAIN386",           "NULLANOMLANGA941",
                                 "QUISQUESIERRA659",    "MATTISABDUL346",       "PHASELLUSKNOX183",    "NEQUEXYLA92",         "ORNAREGAY372",
                                 "FACILISISIVY303",     "TELLUSHARLAN339",      "METUSSOPOLINE75",     "NUNCKYLYNN927",       "PROINHERMIONE775",
                                 "TEMPORFITZGERALD656", "VELVELMA504",          "FAUCIBUSTAMEKAH272",  "PORTTITORWHOOPI881",  "EUPETER41" };

    APICallback->leaderboardEntry.globalRank = rankID + (APICallback->rankScore + 1);
    APICallback->leaderboardEntry.score      = (4 * APICallback->leaderboardEntry.globalRank + 2400) % 59999;
    APICallback->leaderboardEntry.isUser     = APICallback->prevIsUser && rankID == APICallback->prevRank;
    if (APICallback->leaderboardEntry.isUser) {
        RSDK.InitString(&APICallback->leaderboardEntry.username, "YOU", 0);
        LogHelpers_Print("RSDKRankEntry { globalRank: %d, score: %d, entryName: %s, isUser: %d }", APICallback->leaderboardEntry.globalRank,
                         APICallback->leaderboardEntry.score, "YOU", APICallback->leaderboardEntry.isUser);
    }
    else {
        RSDK.InitString(&APICallback->leaderboardEntry.username, "", 0);
        RSDK.SetString(&APICallback->leaderboardEntry.username, dummyNames[rankID]);
        LogHelpers_Print("RSDKRankEntry { globalRank: %d, score: %d, entryName: %s, isUser: %d }", APICallback->leaderboardEntry.globalRank,
                         APICallback->leaderboardEntry.score, dummyNames[rankID], APICallback->leaderboardEntry.isUser);
    }
    return &APICallback->leaderboardEntry;
}

void APICallback_NotifyAutoSave_OK(void)
{
    APICallback->isAutoSaving = false;
    globals->notifiedAutosave = true;
    UIWaitSpinner_FinishWait();
}

void APICallback_NotifyAutoSave_CB(void)
{
    RSDK_THIS(APICallback);

    if (APICallback->isAutoSaving) {
        if (!UIDialog->activeDialog) {
            String string;
            Localization_GetString(&string, STR_AUTOSAVENOTIF);
            EntityUIDialog *dialog = UIDialog_CreateDialogOk(&string, APICallback_NotifyAutoSave_OK, true);
            dialog->useAltColor    = true;
        }
    }
    else {
        APICallback->activeEntity = NULL;
        destroyEntity(self);
    }
}

void APICallback_PromptSavePreference_CB(void)
{
    RSDK_THIS(APICallback);

    if (APICallback->saveStatus == STATUS_CONTINUE) {
        if (!UIDialog->activeDialog) {
            String string;

            int32 stringID = STR_SAVELOADFAIL;
            switch (self->status) {
                case STATUS_ERROR:
                    stringID = STR_NOXBOXPROFILE;
                    if (sku_platform != PLATFORM_XB1)
                        stringID = STR_SAVELOADFAIL;
                    break;
                case STATUS_CORRUPT: stringID = STR_CORRUPTSAVE; break;
                case STATUS_NOSPACE: stringID = (sku_platform == PLATFORM_XB1) + STR_NOSAVESPACE; break;
            }
            Localization_GetString(&string, stringID);

            EntityUIDialog *dialog = UIDialog_CreateDialogYesNo(&string, APICallback_SetNoSaveEnabled, APICallback_SetNoSaveDisabled, true, true);
            dialog->useAltColor    = true;
        }
    }
    else {
        APICallback->activeEntity = NULL;
        destroyEntity(self);
    }
}

void APICallback_PromptSavePreference(int32 status)
{
    if (API_GetNoSave()) {
        LogHelpers_Print("PromptSavePreference() returning due to noSave");
        return;
    }

    LogHelpers_Print("PromptSavePreference()");

    if (APICallback->saveStatus == STATUS_CONTINUE)
        LogHelpers_Print("WARNING PromptSavePreference() when prompt already in progress.");

    APICallback->saveStatus = STATUS_CONTINUE;

    EntityAPICallback *entity = CREATE_ENTITY(APICallback, APICallback_PromptSavePreference_CB, 0, 0);
    entity->status            = status;
    APICallback->activeEntity = (Entity *)entity;
}

void APICallback_LoadUserFile(const char *name, void *buffer, int32 size, void (*callback)(int32))
{
    if (API_GetNoSave()) {
        LogHelpers_Print("LoadUserFile(%s, %X, %d, %X) loading 0's due to noSave", name, buffer, size, callback);
        memset(buffer, 0, size);
        callback(false);
    }
    else if (APICallback->LoadUserFile) {
        LogHelpers_Print("API LoadUserFile(%s, %X, %d, %X)", name, buffer, size, callback);
        APICallback->LoadUserFile(name, buffer, size, callback);
    }
    else {
        LogHelpers_Print("DUMMY LoadUserFile(%s, %X, %d)", name, buffer, size);
        EntityAPICallback *entity = CREATE_ENTITY(APICallback, APICallback_HandleCallback, 0, 0);
        entity->callback          = APICallback_LoadCB;
        entity->timer             = 60;
        entity->fileSize          = size;
        entity->fileName          = name;
        entity->fileBuffer        = buffer;
        entity->fileCallback      = callback;
    }
}

void APICallback_LoadCB(void)
{
    RSDK_THIS(APICallback);

    int32 loadResult = RSDK.LoadUserFile(self->fileName, self->fileBuffer, self->fileSize);
    LogHelpers_Print("DUMMY DummyLoadCB(%s, %x, %d) -> %d", self->fileName, self->fileBuffer, self->fileSize, loadResult);

    if (self->fileCallback)
        self->fileCallback(true);
}

int32 APICallback_LeaderboardStatus(void)
{
    if (APICallback->LeaderboardStatus)
        return APICallback->LeaderboardStatus();

    int32 status = 0;
    if (sku_platform < PLATFORM_PS4) {
        LogHelpers_Print("EMPTY LeaderboardStatus()");
        status = 0;
    }
    else {
        LogHelpers_Print("DUMMY LeaderboardStatus()");

        if (APICallback->leaderboardsStatus == STATUS_CONTINUE) {
            if (APICallback->statusTimer < 60) {
                APICallback->statusTimer++;
                return APICallback->leaderboardsStatus;
            }

            if ((RSDK.Rand(0, 100) % 10) > 10)
                APICallback->leaderboardsStatus = STATUS_ERROR;
            else
                APICallback->leaderboardsStatus = STATUS_OK;
        }
        status = APICallback->leaderboardsStatus;
    }

    return status;
}

int32 APICallback_LeaderboardEntryCount(void)
{
    if (APICallback->LeaderboardEntryCount)
        return APICallback->LeaderboardEntryCount();

    if (sku_platform < PLATFORM_PS4) {
        LogHelpers_Print("EMPTY LeaderboardEntryCount()");
    }
    else {
        LogHelpers_Print("DUMMY LeaderboardEntryCount()");
        if (APICallback->leaderboardsStatus == STATUS_OK)
            return APICallback->leaderboardEntryCount;
    }

    return -1;
}

void APICallback_LaunchManual()
{
    if (APICallback->LaunchManual)
        APICallback->LaunchManual();
    else if (sku_platform == PLATFORM_DEV)
        LogHelpers_Print("DUMMY LaunchManual()");
    else
        LogHelpers_Print("EMPTY LaunchManual()");
}

void APICallback_HandleCallback(void)
{
    RSDK_THIS(APICallback);

    if (self->timer <= 0) {
        LogHelpers_Print("Callback: %x", self->callback);
        StateMachine_Run(self->callback);
        destroyEntity(self);
    }
    else {
        self->timer--;
    }
}

int32 APICallback_GetUserAuthStatus(void)
{
    if (APICallback->GetUserAuthStatus)
        APICallback->authStatus = APICallback->GetUserAuthStatus();
    else
        APICallback->authStatus = STATUS_OK;

    if (APICallback->saveStatus || APICallback->authStatus != STATUS_ERROR) {
        if (APICallback->authStatus == STATUS_FORBIDDEN && !APICallback->authForbidden) {
            EntityAPICallback *entity  = CREATE_ENTITY(APICallback, APICallback_CheckUserAuth_CB, 0, 0);
            entity->active             = ACTIVE_ALWAYS;
            APICallback->activeEntity  = (Entity *)entity;
            APICallback->authForbidden = true;
        }
    }
    else {
        APICallback->saveStatus = STATUS_ERROR;
    }

    return APICallback->authStatus;
}

int32 APICallback_GetStorageStatus(void)
{
    if (APICallback->saveStatus == STATUS_ERROR)
        return STATUS_ERROR;

    int32 status = 0;
    if (APICallback->GetStorageStatus) {
        status = APICallback->GetStorageStatus(0);
    }
    else { // normally this is "if sku_platform != PLATFORM_PC" but we don't have a get storage status so lets pretend that check doesn't exist
        if (APICallback->activeEntity) {
            status = STATUS_CONTINUE;
        }
        else {
            if (APICallback->storageStatus == STATUS_CONTINUE) {
                if (APICallback->statusTimer >= 0) {
                    if (RSDK.Rand(0, 100) % 10 > 10)
                        APICallback->storageStatus = STATUS_ERROR;
                    else
                        APICallback->storageStatus = STATUS_OK;
                }
                else {
                    APICallback->statusTimer++;
                }
            }
            status = APICallback->storageStatus;
        }
    }

    if (!APICallback->saveStatus) {
        if (status == STATUS_ERROR) {
            APICallback->saveStatus = STATUS_ERROR;
            return STATUS_ERROR;
        }
        if (status == STATUS_OK)
            APICallback->saveStatus = STATUS_OK;
    }

    return status;
}

int32 APICallback_GetSaveStatus(void)
{
    if (API_GetNoSave())
        return STATUS_OK;

    if (APICallback->activeEntity)
        return STATUS_CONTINUE;
    else
        return APICallback->saveStatus;
}

int32 APICallback_GetControllerType(int32 deviceID)
{
    if (APICallback->GetControllerType)
        return APICallback->GetControllerType(deviceID);
    else
        return sku_platform == PLATFORM_DEV;
}

int32 APICallback_FetchLeaderboardData(uint8 zoneID, uint8 actID, int32 playerID, int32 start, int32 end, bool32 isUser)
{
    if (APICallback->FetchLeaderboard)
        return APICallback->FetchLeaderboard(zoneID, actID, playerID, start, end, isUser);

    if (sku_platform < PLATFORM_PS4) {
        LogHelpers_Print("EMPTY FetchLeaderboardData(%d, %d, %d, %d, %d, %d)", zoneID, actID, playerID, start, end, isUser);
        return 0;
    }

    LogHelpers_Print("DUMMY FetchLeaderboardData(%d, %d, %d, %d, %d, %d)", zoneID, actID, playerID, start, end, isUser);
    if (APICallback->leaderboardsStatus == STATUS_CONTINUE) {
        LogHelpers_Print("WARNING Called FetchLeaderboard() while loading");
        return 0;
    }

    APICallback->rankScore             = 0;
    APICallback->leaderboardEntryCount = RSDK.Rand(start, end);
    APICallback->prevIsUser            = isUser;
    if (isUser) {
        APICallback->rankScore = RSDK.Rand(1000, 9999);
        APICallback->prevRank  = RSDK.Rand(0, APICallback->leaderboardEntryCount - 1);
    }
    APICallback->leaderboardsStatus = STATUS_CONTINUE;
    APICallback->statusTimer        = 0;
    return 0;
}

void APICallback_ExitGame(void)
{
    if (APICallback->ExitGame) {
        APICallback->ExitGame();
    }
    else {
        if (sku_platform == PLATFORM_DEV)
            exit(0);

        LogHelpers_Print("EMPTY ExitGame()");
    }
}

void APICallback_ClearPrerollErrors(void)
{
    if (APICallback->ClearPrerollErrors) {
        LogHelpers_Print("API ClearPrerollErrors()");
        APICallback->ClearPrerollErrors();
    }
    else {
        LogHelpers_Print("DUMMY ClearPrerollErrors()");

        if (APICallback->authStatus != STATUS_OK) {
            APICallback->authStatus = STATUS_NONE;
        }
        if (APICallback->storageStatus != STATUS_OK)
            APICallback->storageStatus = STATUS_NONE;
    }
}

bool32 APICallback_CheckInputDisconnected(void)
{
    RSDK_THIS(APICallback);

    return APICallback_InputIDIsDisconnected(self->deviceID) || PauseMenu->forcedDisconnect;
}

bool32 APICallback_InputIDIsDisconnected(uint8 inputSlot)
{
    if (APICallback->InputIDIsDisconnected) {
        return APICallback->InputIDIsDisconnected(inputSlot);
    }
    else {
        LogHelpers_Print("DUMMY InputIDIsDisconnected()");
        return APICallback->inputSlots[inputSlot];
    }
}

int32 APICallback_ControllerIDForInputID(uint8 inputSlot)
{
    if (APICallback->ControllerIDForInputID) {
        return APICallback->ControllerIDForInputID(inputSlot);
    }
    else {
        LogHelpers_Print("DUMMY ControllerIDForInputID()");
        return 1;
    }
}
int32 APICallback_MostRecentActiveControllerID(uint8 inputSlot)
{
    if (APICallback->MostRecentActiveControllerID) {
        return APICallback->MostRecentActiveControllerID(inputSlot);
    }
    else {
        LogHelpers_Print("DUMMY MostRecentActiveControllerID()");
        return 1;
    }
}
void APICallback_AssignControllerID(uint8 inputSlot, int32 deviceID)
{
    if (APICallback->AssignControllerID) {
        APICallback->AssignControllerID(inputSlot, deviceID);
    }
    else {
        LogHelpers_Print("DUMMY AssignControllerID()");
        APICallback->inputSlots[inputSlot] = deviceID;
    }
}
void APICallback_ResetControllerAssignments(void)
{
    if (APICallback->ResetControllerAssignments) {
        APICallback->ResetControllerAssignments();
    }
    else {
        LogHelpers_Print("DUMMY ResetControllerAssignments()");
        APICallback->inputSlots[0] = 0;
        APICallback->inputSlots[1] = 0;
        APICallback->inputSlots[2] = 0;
        APICallback->inputSlots[3] = 0;
    }
}

void APICallback_TrackActClear(uint8 zoneID, uint8 actID, uint8 playerID, int32 time, int32 rings, int32 score)
{
    if (APICallback->TrackActClear)
        APICallback->TrackActClear(zoneID, actID, playerID, score, rings, time);
    else
        LogHelpers_Print("EMPTY TrackActClear(%d, %d, %d, %d, %d, %d)", zoneID, actID, playerID, score, rings, time);
}
void APICallback_TrackTAClear(uint8 zoneID, uint8 actID, uint8 playerID, int32 time)
{
    if (APICallback->TrackTAClear)
        APICallback->TrackTAClear(zoneID, actID, playerID, time);
    else
        LogHelpers_Print("EMPTY TrackTAClear(%d, %d, %d, %d)", zoneID, actID, playerID, time);
}
void APICallback_TrackEnemyDefeat(uint8 zoneID, uint8 actID, uint8 playerID, int32 x, int32 y)
{
    if (APICallback->TrackEnemyDefeat)
        APICallback->TrackEnemyDefeat(zoneID, actID, playerID, x, y);
    else
        LogHelpers_Print("EMPTY TrackEnemyDefeat(%d, %d, %d, %d, %d)", zoneID, actID, playerID, x, y);
}

void APICallback_TrackGameProgress(float percent)
{
    if (APICallback->TrackGameProgress)
        APICallback->TrackGameProgress(percent);
    else
        LogHelpers_Print("EMPTY TrackGameProgress(%f)", percent);
}

void APICallback_TryAuth_No(void)
{
    APICallback->authStatus = STATUS_ERROR;
    APICallback->saveStatus = STATUS_ERROR;
}

void APICallback_TryAuth_Yes(void) { APICallback->authStatus = STATUS_OK; }

void APICallback_TryAuth_CB(void)
{
    RSDK_THIS(APICallback);

    if (APICallback->authStatus == STATUS_CONTINUE) {
        if (!UIDialog->activeDialog) {
            String string;
            switch (sku_platform) {
                case PLATFORM_PC:
#if MANIA_USE_EGS
                    Localization_GetString(&string, STR_LOADEGS);
#else
                    Localization_GetString(&string, STR_LOADSTEAM);
#endif
                    break;
                case PLATFORM_PS4: Localization_GetString(&string, STR_LOADPSN); break;
                case PLATFORM_XB1: Localization_GetString(&string, STR_LOADXBOX); break;
                case PLATFORM_SWITCH: Localization_GetString(&string, STR_LOADNINTENDO); break;
            }

            EntityUIDialog *dialog = UIDialog_CreateDialogYesNo(&string, APICallback_TryAuth_Yes, APICallback_TryAuth_No, true, true);
            dialog->useAltColor    = true;
        }
    }
    else {
        APICallback->activeEntity = NULL;
        destroyEntity(self);
    }
}

int32 APICallback_TryAuth(void)
{
    if (APICallback->TryAuth) {
        LogHelpers_Print("API TryAuth()");
        return APICallback->TryAuth();
    }
    else {
        LogHelpers_Print("DUMMY TryAuth()");
        if (APICallback->authStatus == STATUS_CONTINUE) {
            LogHelpers_Print("WARNING TryAuth() when auth already in progress.");
        }
        APICallback->statusTimer = 0;
        APICallback->authStatus  = STATUS_CONTINUE;
        CREATE_ENTITY(APICallback, APICallback_TryAuth_CB, 0, 0);

        return STATUS_CONTINUE;
    }
}

void APICallback_TryInitStorage(void)
{
    if (APICallback->TryInitStorage) {
        LogHelpers_Print("API TryInitStorage()");
        APICallback->TryInitStorage();
    }
    else {
        LogHelpers_Print("DUMMY TryInitStorage()");
        if (APICallback->storageStatus == STATUS_CONTINUE) {
            LogHelpers_Print("WARNING TryInitStorage() when init already in progress.");
        }
        APICallback->statusTimer   = 0;
        APICallback->storageStatus = STATUS_CONTINUE;
    }
}

bool32 APICallback_GetUsername(String *userName)
{
    if (APICallback->GetUsername) {
        LogHelpers_Print("API GetUsername()");
        APICallback->GetUsername(userName);
        return true;
    }
    else {
        if (sku_platform == PLATFORM_DEV) {
            LogHelpers_Print("DUMMY GetUsername()");
            RSDK.InitString(userName, "IntegerGeorge802", 0);
            return true;
        }
        else {
            LogHelpers_Print("EMPTY GetUsername()");
        }
    }

    return false;
}

void APICallback_ClearAchievements(void)
{
    if (APICallback->ClearAchievements)
        APICallback->ClearAchievements();
    else if (sku_platform == PLATFORM_DEV)
        LogHelpers_Print("DUMMY ClearAchievements()");
    else
        LogHelpers_Print("EMPTY ClearAchievements()");
}

void APICallback_UnlockAchievement(const char *name)
{
    // Silly Workaround
    AchievementID id;
    id.id = name;

    if (APICallback->achievementsDisabled)
        LogHelpers_Print("SKIP UnlockAchievement(%s)", name);
    else if (APICallback->UnlockAchievement)
        APICallback->UnlockAchievement(&id);
    else if (sku_platform == PLATFORM_DEV)
        LogHelpers_Print("DUMMY UnlockAchievement(%s)", name);
    else
        LogHelpers_Print("EMPTY UnlockAchievement(%s)", name);
}

void APICallback_CheckUserAuth_OK(void) { APICallback->signedOut = true; }

void APICallback_CheckUserAuth_CB(void)
{
    RSDK_THIS(APICallback);

    EntityUIDialog *dialog = UIDialog->activeDialog;
    if (self->timer) {
        if (APICallback->signedOut) {
            if (!dialog) {
                if (Zone) {
                    RSDK.SetScene("Presentation", "Title Screen");
                    Zone_StartFadeOut(10, 0x000000);
                }
                else if (MenuSetup) {
                    MenuSetup_StartReturnToTitle();
                }

                APICallback->activeEntity = NULL;
                destroyEntity(self);
            }
        }
        else if (!dialog) {
            String string;
            Localization_GetString(&string, STR_SIGNOUTDETECTED);
            dialog              = UIDialog_CreateDialogOk(&string, APICallback_CheckUserAuth_OK, true);
            dialog->useAltColor = true;
        }
    }
    else {
        if (dialog) {
            UIDialog_CloseOnSel_HandleSelection(dialog, StateMachine_None);
        }
        else {
            if (UIControl) {
                if (UIControl_GetUIControl())
                    UIControl_SetInactiveMenu(UIControl_GetUIControl());
            }

            RSDK.SetEngineState(ENGINESTATE_FROZEN);
            Music_Stop();
            self->timer = 1;
        }
    }
}

#if MANIA_USE_PLUS
void APICallback_TrackGameProgressCB(bool32 success) { UIWaitSpinner_FinishWait(); }
#else
void APICallback_TrackGameProgressCB(void) { UIWaitSpinner_FinishWait(); }
#endif

void APICallback_GetNextNotif(void)
{
    if (SceneInfo->inEditor || API_GetNoSave() || globals->saveLoaded != STATUS_OK) {
        return;
    }
    else {
        ProgressRAM *progress                               = GameProgress_GetProgressRAM();
        progress->unreadNotifs[GameProgress_GetNextNotif()] = true;
    }
}

void APICallback_ManageNotifs(void)
{
    RSDK_THIS(APICallback);

    if (GameProgress_CountUnreadNotifs()) {
        String string;
        INIT_STRING(string);
        if (!UIDialog->activeDialog) {
            int32 str = GameProgress_GetNotifStringID(GameProgress_GetNextNotif());
            Localization_GetString(&string, str);
            EntityUIDialog *dialog = UIDialog_CreateDialogOk(&string, APICallback_GetNextNotif, true);
            dialog->playEventSfx   = true;
            dialog->useAltColor    = true;
        }
    }
    else {
        APICallback->activeEntity = NULL;
        UIWaitSpinner_StartWait();
        GameProgress_TrackGameProgress(APICallback_TrackGameProgressCB);
        destroyEntity(self);
    }
}

bool32 APICallback_CheckUnreadNotifs(void)
{
    if (!GameProgress_CountUnreadNotifs())
        return false;

    if (!APICallback->activeEntity)
        APICallback->activeEntity = (Entity *)CREATE_ENTITY(APICallback, APICallback_ManageNotifs, 0, 0);

    return true;
}

bool32 APICallback_NotifyAutosave(void)
{
    if (globals->notifiedAutosave) {
        if (!APICallback->isAutoSaving && !APICallback->activeEntity)
            return false;
    }
    else if (!APICallback->isAutoSaving || !APICallback->activeEntity) {
        UIWaitSpinner_StartWait();
        APICallback->isAutoSaving = true;
        globals->notifiedAutosave = false;
        LogHelpers_Print("DUMMY NotifyAutosave()");
        EntityAPICallback *dialogRunner = CREATE_ENTITY(APICallback, APICallback_NotifyAutoSave_CB, 0, 0);
        dialogRunner->active            = ACTIVE_ALWAYS;
        APICallback->activeEntity       = (Entity *)dialogRunner;
    }

    return true;
}

#if RETRO_INCLUDE_EDITOR
void APICallback_EditorDraw(void) {}

void APICallback_EditorLoad(void) {}
#endif

void APICallback_Serialize(void) {}

#endif
