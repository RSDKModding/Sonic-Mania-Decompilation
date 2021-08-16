#include "SonicMania.h"

#if !RETRO_USE_PLUS
ObjectAPICallback *APICallback;

void APICallback_Update(void)
{
    RSDK_THIS(APICallback);
    StateMachine_Run(entity->state);
}

void APICallback_LateUpdate(void) {}

void APICallback_StaticUpdate(void) {}

void APICallback_Draw(void) {}

void APICallback_Create(void *data)
{
    RSDK_THIS(APICallback);

    entity->active  = ACTIVE_ALWAYS;
    entity->visible = false;
    entity->state   = (Type_StateMachine)data;
}

void APICallback_StageLoad(void)
{
    APICallback->achievementsDisabled         = false;
    APICallback->active                       = ACTIVE_ALWAYS;
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
    APICallback->controllerIDs[0]             = CONT_P1;
    APICallback->controllerIDs[1]             = CONT_P2;
    APICallback->controllerIDs[2]             = CONT_P3;
    APICallback->controllerIDs[3]             = CONT_P4;
    APICallback->controllerCount              = 4;
}

void APICallback_SetRichPresence(int id, TextInfo *msg)
{
    if (globals->presenceID != id) {
        // RSDK.PrependText(&message, id + '.');
        if (APICallback->SetRichPresence) {
            LogHelpers_Print("API SetRichPresence(%d)", id);
            LogHelpers_PrintString(msg);
            APICallback->SetRichPresence(id, msg);
        }
        else {
            LogHelpers_Print("EMPTY SetRichPresence(%d)", id);
            LogHelpers_PrintString(msg);
        }
    }
}

int APICallback_GetUserLanguage(void)
{
    LogHelpers_Print("EMPTY GetUserLanguage() -> GameInfo->language (%d)", RSDK_info->language);
    return RSDK_info->language;
}

bool32 APICallback_GetConfirmButtonFlip(void)
{
    if (APICallback->GetConfirmButtonFlip)
        return APICallback->GetConfirmButtonFlip();

    LogHelpers_Print("DUMMY GetConfirmButtonFlip()");
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

void APICallback_SaveUserFile(const char *name, void *buffer, int size, void (*callback)(int))
{
    if (globals->noSave) {
        LogHelpers_Print("SaveUserFile(%s, %X, %d, %X) failing due to noSave", name, buffer, size, callback);
        callback(0);
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
    int saveResult = RSDK.SaveUserFile(entity->fileName, entity->fileBuffer, entity->fileSize);
    LogHelpers_Print("DUMMY DummySaveCB(%s, %x, %d) -> %d", entity->fileName, entity->fileBuffer, entity->fileSize, saveResult);

    if (entity->fileCallback)
        entity->fileCallback(true);
}

LeaderboardEntry *APICallback_ReadLeaderboardEntry(int rankID)
{
    if (APICallback->LeaderboardReadEntry)
        return APICallback->LeaderboardReadEntry(rankID);
    if (!RSDK_info->platform) {
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
        RSDK.SetText(&APICallback->leaderboardEntry.username, "YOU", 0);
    }
    else {
        RSDK.SetText(&APICallback->leaderboardEntry.username, "", 0);
        RSDK.PrependText(&APICallback->leaderboardEntry.username, dummyNames[rankID]);
    }
    LogHelpers_Print("RSDKRankEntry { globalRank: %d, score: %d, entryName: %s, isUser: %d }", APICallback->leaderboardEntry.globalRank,
                     APICallback->leaderboardEntry.score, APICallback->leaderboardEntry.username.text, APICallback->leaderboardEntry.isUser);
    return &APICallback->leaderboardEntry;
}

void APICallback_NotifyAutoSave_OK(void)
{
    APICallback->notifyAutoSaveFlag = false;
    globals->notifiedAutosave       = true;
    UIWaitSpinner_Wait2();
}

void APICallback_NotifyAutoSave_CB(void)
{
    RSDK_THIS(APICallback);
    if (APICallback->notifyAutoSaveFlag) {
        if (!UIDialog->activeDialog) {
            TextInfo info;
            Localization_GetString(&info, STR_SIGNOUTDETECTED);
            EntityUIDialog *dialog = UIDialog_CreateActiveDialog(&info);
            dialog->field_B8       = true;
            UIDialog_AddButton(DIALOG_OK, dialog, APICallback_NotifyAutoSave_OK, true);
            UIDialog_Setup(dialog);
        }
    }
    else {
        APICallback->activeEntity = NULL;
        destroyEntity(entity);
    }
}

void APICallback_PromptSavePreference_CB(void)
{
    RSDK_THIS(APICallback);
    if (APICallback->saveStatus == STATUS_CONTINUE) {
        if (!UIDialog->activeDialog) {
            TextInfo info;

            int stringID = STR_SAVELOADFAIL;
            switch (entity->status) {
                case STATUS_ERROR:
                    stringID = STR_NOXBOXPROFILE;
                    if (RSDK_info->platform != PLATFORM_XB1)
                        stringID = STR_SAVELOADFAIL;
                    break;
                case STATUS_CORRUPT: stringID = STR_CORRUPTSAVE; break;
                case STATUS_NOSPACE: stringID = (RSDK_info->platform == PLATFORM_XB1) + STR_NOSAVESPACE; break;
            }
            Localization_GetString(&info, stringID);

            EntityUIDialog *dialog = (EntityUIDialog *)UIDialog_CreateActiveDialog(&info);
            dialog->field_B8       = true;
            UIDialog_AddButton(DIALOG_NO, dialog, APICallback_SetNoSaveDisabled, true);
            UIDialog_AddButton(DIALOG_YES, dialog, APICallback_SetNoSaveEnabled, true);
            UIDialog_Setup(dialog);
        }
    }
    else {
        APICallback->activeEntity = NULL;
        destroyEntity(entity);
    }
}

void APICallback_PromptSavePreference(int status)
{
    if (globals->noSave) {
        LogHelpers_Print("PromptSavePreference() returning due to noSave");
        return;
    }
    LogHelpers_Print("PromptSavePreference()");

    if (APICallback->saveStatus == STATUS_CONTINUE) {
        LogHelpers_Print("WARNING PromptSavePreference() when prompt already in progress.");
    }
    APICallback->saveStatus = STATUS_CONTINUE;

    EntityAPICallback *entity = CREATE_ENTITY(APICallback, APICallback_PromptSavePreference_CB, 0, 0);
    entity->status            = status;
    APICallback->activeEntity = (Entity *)entity;
}

void APICallback_LoadUserFile(const char *name, void *buffer, int size, void (*callback)(int))
{
    if (globals->noSave) {
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

int APICallback_LoadCB(void)
{
    RSDK_THIS(APICallback);
    int loadResult = RSDK.LoadUserFile(entity->fileName, entity->fileBuffer, entity->fileSize);
    LogHelpers_Print("DUMMY DummyLoadCB(%s, %x, %d) -> %d", entity->fileName, entity->fileBuffer, entity->fileSize, loadResult);

    if (entity->fileCallback)
        entity->fileCallback(1);
    return 1;
}

int APICallback_LeaderboardStatus(void)
{
    if (APICallback->LeaderboardStatus)
        return APICallback->LeaderboardStatus();

    int status = 0;
    if (RSDK_info->platform < PLATFORM_PS4) {
        LogHelpers_Print("EMPTY LeaderboardStatus()");
        status = 0;
    }
    else {
        LogHelpers_Print("DUMMY LeaderboardStatus()");
        if (APICallback->leaderboardsStatus == STATUS_OK) {
            if (APICallback->unknown < 60) {
                APICallback->unknown++;
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

int APICallback_LeaderboardEntryCount(void)
{
    if (APICallback->LeaderboardEntryCount)
        return APICallback->LeaderboardEntryCount();
    if (RSDK_info->platform < PLATFORM_PS4) {
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
    if (APICallback->LaunchManual) {
        APICallback->LaunchManual();
    }
    else if (RSDK_info->platform == PLATFORM_DEV) {
        LogHelpers_Print("DUMMY LaunchManual()");
    }
    else {
        LogHelpers_Print("EMPTY LaunchManual()");
    }
}

void APICallback_HandleCallback(void)
{
    RSDK_THIS(APICallback);

    if (entity->timer <= 0) {
        LogHelpers_Print("Callback: %x", entity->callback);
        StateMachine_Run(entity->callback);
        destroyEntity(entity);
    }
    else {
        entity->timer--;
    }
}

int APICallback_GetUserAuthStatus(void)
{
    if (APICallback->GetUserAuthStatus) {
        APICallback->authStatus = APICallback->GetUserAuthStatus();
    }
    else {
        APICallback->authStatus = STATUS_OK;
    }

    if (APICallback->saveStatus || APICallback->authStatus != STATUS_ERROR) {
        if (APICallback->authStatus == STATUS_FORBIDDEN && !APICallback->authFlag) {
            EntityAPICallback *entity = CREATE_ENTITY(APICallback, APICallback_CheckUserAuth_CB, 0, 0);
            entity->active            = ACTIVE_ALWAYS;
            APICallback->activeEntity = (Entity *)entity;
            APICallback->authFlag     = true;
        }
    }
    else {
        APICallback->saveStatus = STATUS_ERROR;
    }
    return APICallback->authStatus;
}

signed int APICallback_GetStorageStatus(void)
{
    if (APICallback->saveStatus == STATUS_ERROR)
        return STATUS_ERROR;

    int status = 0;
    if (APICallback->GetStorageStatus) {
        status = APICallback->GetStorageStatus(0);
    }
    else if (RSDK_info->platform >= PLATFORM_PS4) {
        if (APICallback->activeEntity) {
            status = STATUS_CONTINUE;
        }
        else {
            if (APICallback->storageStatus == STATUS_CONTINUE) {
                if (APICallback->unknown >= 0) {
                    if (RSDK.Rand(0, 100) % 10 > 10)
                        APICallback->storageStatus = STATUS_ERROR;
                    else
                        APICallback->storageStatus = STATUS_OK;
                }
                else {
                    APICallback->unknown++;
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

int APICallback_GetSaveStatus(void)
{
    if (globals->noSave)
        return STATUS_OK;

    if (APICallback->activeEntity)
        return STATUS_CONTINUE;
    else
        return APICallback->saveStatus;
}

int APICallback_GetControllerType(int id)
{
    if (APICallback->GetControllerType)
        return APICallback->GetControllerType(id, 0);
    else
        return RSDK_info->platform == PLATFORM_DEV;
}

int APICallback_FetchLeaderboardData(byte zoneID, byte actID, int playerID, int start, int end, bool32 isUser)
{
    if (APICallback->FetchLeaderboard)
        return APICallback->FetchLeaderboard(zoneID, actID, playerID, start, end, isUser);
    if (RSDK_info->platform < PLATFORM_PS4) {
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
    APICallback->unknown            = 0;
    return 0;
}

void APICallback_ExitGame(void)
{
    if (APICallback->ExitGame) {
        APICallback->ExitGame();
    }
    else {
        if (RSDK_info->platform == PLATFORM_DEV)
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
    return APICallback_InputIDIsDisconnected(entity->field_7C) || PauseMenu->dword10;
}

bool32 APICallback_InputIDIsDisconnected(int id)
{
    RSDK_THIS(APICallback);
    if (APICallback->InputIDIsDisconnected) {
        return APICallback->InputIDIsDisconnected(id);
    }
    else {
        LogHelpers_Print("DUMMY InputIDIsDisconnected()");
        return APICallback->controllerIDs[id];
    }
}

int APICallback_ControllerIDForInputID(int id)
{
    if (APICallback->ControllerIDForInputID) {
        return APICallback->ControllerIDForInputID(id);
    }
    else {
        LogHelpers_Print("DUMMY ControllerIDForInputID()");
        return 1;
    }
}
int APICallback_MostRecentActiveControllerID(int id)
{
    if (APICallback->MostRecentActiveControllerID) {
        return APICallback->MostRecentActiveControllerID(id);
    }
    else {
        LogHelpers_Print("DUMMY MostRecentActiveControllerID()");
        return 1;
    }
}
void APICallback_AssignControllerID(int controllerID, int id)
{
    if (APICallback->AssignControllerID) {
        APICallback->AssignControllerID(controllerID, id);
    }
    else {
        LogHelpers_Print("DUMMY AssignControllerID()");
        APICallback->controllerIDs[controllerID] = id;
    }
}
void APICallback_ResetControllerAssignments(void)
{
    if (APICallback->ResetControllerAssignments) {
        APICallback->ResetControllerAssignments();
    }
    else {
        LogHelpers_Print("DUMMY ResetControllerAssignments()");
        APICallback->controllerIDs[0] = 0;
        APICallback->controllerIDs[1] = 0;
        APICallback->controllerIDs[2] = 0;
        APICallback->controllerIDs[3] = 0;
    }
}

void APICallback_TrackActClear(byte zoneID, byte actID, byte playerID, int score, int rings, int time)
{
    if (APICallback->TrackActClear)
        APICallback->TrackActClear(zoneID, actID, playerID, score, rings, time);
    else
        LogHelpers_Print("EMPTY TrackActClear(%d, %d, %d, %d, %d, %d)", zoneID, actID, playerID, score, rings, time);
}
void APICallback_TrackTAClear(byte zoneID, byte actID, byte playerID, int time)
{
    if (APICallback->TrackTAClear)
        APICallback->TrackTAClear(zoneID, actID, playerID, time);
    else
        LogHelpers_Print("EMPTY TrackTAClear(%d, %d, %d, %d)", zoneID, actID, playerID, time);
}
void APICallback_TrackEnemyDefeat(byte zoneID, byte actID, byte playerID, int entityX, int entityY)
{
    if (APICallback->TrackEnemyDefeat)
        APICallback->TrackEnemyDefeat(zoneID, actID, playerID, entityX, entityY);
    else
        LogHelpers_Print("EMPTY TrackEnemyDefeat(%d, %d, %d, %d, %d)", zoneID, actID, playerID, entityX, entityY);
}

void APICallback_TrackGameProgress(void (*callback)(int))
{
    if (globals->noSave || globals->saveLoaded != STATUS_OK) {
        LogHelpers_Print("WARNING GameProgress Attempted to save before loading SaveGame file");
    }
    else if (!globals->saveRAM[0x900]) {
        float percent = SaveGame_GetCompletionPercent(&globals->saveRAM[0x900]);
        if (APICallback->TrackGameProgress)
            APICallback->TrackGameProgress(percent);
        else
            LogHelpers_Print("EMPTY TrackGameProgress(%f)", percent);
        SaveGame_SaveFile(callback);
    }
    if (callback)
        callback(0);
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
    if (APICallback->authStatus == 100) {
        if (!UIDialog->activeDialog) {
            TextInfo info;
            Localization_GetString(&info, STR_LOADNINTENDO);
            EntityUIDialog *dialog = UIDialog_CreateActiveDialog(&info);
            dialog->field_B8       = true;
            UIDialog_AddButton(DIALOG_NO, dialog, APICallback_TryAuth_No, true);
            UIDialog_AddButton(DIALOG_YES, dialog, APICallback_TryAuth_Yes, true);
            UIDialog_Setup(dialog);
        }
    }
    else {
        APICallback->activeEntity = NULL;
        destroyEntity(entity);
    }
}

int APICallback_TryAuth(void)
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
        APICallback->unknown    = 0;
        APICallback->authStatus = STATUS_CONTINUE;
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
        APICallback->unknown       = 0;
        APICallback->storageStatus = STATUS_CONTINUE;
    }
}

bool32 APICallback_GetUsername(TextInfo *info)
{
    if (APICallback->GetUsername) {
        LogHelpers_Print("API GetUsername()");
        APICallback->GetUsername(info);
        return true;
    }
    else {
        if (RSDK_info->platform == PLATFORM_DEV) {
            LogHelpers_Print("DUMMY GetUsername()");
            RSDK.SetText(info, "IntegerGeorge802", false);
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
    if (APICallback->ClearAchievements) {
        APICallback->ClearAchievements();
    }
    else if (RSDK_info->platform == PLATFORM_DEV) {
        LogHelpers_Print("DUMMY ClearAchievements()");
    }
    else {
        LogHelpers_Print("EMPTY ClearAchievements()");
    }
}

void APICallback_UnlockAchievement(const char *name)
{
    if (APICallback->achievementsDisabled) {
        LogHelpers_Print("SKIP UnlockAchievement(%s)", name);
    }
    else if (APICallback->UnlockAchievement) {
        APICallback->UnlockAchievement(name);
    }
    else if (RSDK_info->platform == PLATFORM_DEV) {
        LogHelpers_Print("DUMMY UnlockAchievement(%s)", name);
    }
    else {
        LogHelpers_Print("EMPTY UnlockAchievement(%s)", name);
    }
}

void APICallback_CheckUserAuth_OK(void) { APICallback->signoutFlag = true; }

void APICallback_CheckUserAuth_CB(void)
{
    RSDK_THIS(APICallback);
    EntityUIDialog *dialog = UIDialog->activeDialog;
    if (entity->timer) {
        if (APICallback->signoutFlag) {
            if (!dialog) {
                if (Zone) {
                    RSDK.SetScene("Presentation", "Title Screen");
                    Zone_StartFadeOut(10, 0x000000);
                }
                else if (MenuSetup) {
                    MenuSetup_StartReturnToTitle();
                }
                APICallback->activeEntity = NULL;
                destroyEntity(entity);
            }
        }
        else if (!dialog) {
            TextInfo info;
            Localization_GetString(&info, STR_SIGNOUTDETECTED);
            dialog           = UIDialog_CreateActiveDialog(&info);
            dialog->field_B8 = true;
            UIDialog_AddButton(DIALOG_OK, dialog, APICallback_CheckUserAuth_OK, true);
            UIDialog_Setup(dialog);
        }
    }
    else {
        if (dialog) {
            if (dialog->state != UIDialog_Unknown13) {
                dialog->parent->selectionDisabled = true;
                dialog->field_5C                  = 0;
                dialog->state                     = UIDialog_Unknown13;
                dialog->curCallback              = NULL;
            }
        }
        else {
            if (UIControl) {
                if (UIControl_GetUIControl())
                    UIControl_Unknown6(UIControl_GetUIControl());
            }
            RSDK.SetGameMode(ENGINESTATE_FROZEN);
            RSDK.StopChannel(Music->channelID);
            entity->timer = 1;
        }
    }
}

void APICallback_Wait(int success) { UIWaitSpinner_Wait2(); }

void APICallback_GetNextNotif(void)
{
    int *saveRAM = NULL;
    if (RSDK_sceneInfo->inEditor || globals->noSave || globals->saveLoaded != STATUS_OK) {
        LogHelpers_Print("WARNING GameProgress Attempted to save before loading SaveGame file");
        return;
    }
    else {
        saveRAM            = &globals->saveRAM[0x900];
        int id             = SaveGame_GetNextNotif();
        saveRAM[id + 0x35] = true;
    }
}

void APICallback_ManageNotifs(void)
{
    RSDK_THIS(APICallback);
    if (SaveGame_CountUnreadNotifs()) {
        TextInfo info;
        INIT_TEXTINFO(info);
        if (!UIDialog->activeDialog) {
            int str = SaveGame_GetNotifStringID(SaveGame_GetNextNotif());
            Localization_GetString(&info, str);
            EntityUIDialog *dialog = UIDialog_CreateActiveDialog(&info);
            dialog->field_B4       = true;
            dialog->field_B8       = true;
            UIDialog_AddButton(DIALOG_OK, dialog, APICallback_GetNextNotif, true);
            UIDialog_Setup(dialog);
        }
    }
    else {
        APICallback->activeEntity = NULL;
        UIWaitSpinner_Wait();
        SaveGame_TrackGameProgress(APICallback_Wait);
        destroyEntity(entity);
    }
}

bool32 APICallback_CheckUnreadNotifs(void)
{
    if (!SaveGame_CountUnreadNotifs())
        return false;
    if (!APICallback->activeEntity)
        APICallback->activeEntity = (Entity *)CREATE_ENTITY(APICallback, APICallback_ManageNotifs, 0, 0);
    return true;
}

bool32 APICallback_NotifyAutosave(void)
{
    if (!APICallback->notifyAutoSaveFlag && !APICallback->activeEntity) {
        return false;
    }

    if (!APICallback->activeEntity || (!globals->notifiedAutosave && !APICallback->notifyAutoSaveFlag)) {
        UIWaitSpinner_Wait();
        APICallback->notifyAutoSaveFlag = true;
        globals->notifiedAutosave       = false;
        LogHelpers_Print("DUMMY NotifyAutosave()");
        EntityAPICallback *dialogRunner = CREATE_ENTITY(APICallback, APICallback_NotifyAutoSave_CB, 0, 0);
        dialogRunner->active            = ACTIVE_ALWAYS;
        APICallback->activeEntity       = (Entity *)dialogRunner;
    }
    return true;
}

void APICallback_EditorDraw(void) {}

void APICallback_EditorLoad(void) {}

void APICallback_Serialize(void) {}

#endif
