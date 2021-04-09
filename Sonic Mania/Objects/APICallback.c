#include "../SonicMania.h"

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
    entity->state   = (void (*)())data;
}

void APICallback_StageLoad(void)
{
    APICallback->achievementsDisabled         = false;
    APICallback->active                       = ACTIVE_ALWAYS;
    APICallback->LaunchManual                 = (int (*)(void *))RSDK.GetFuncPtr("LaunchManual");
    APICallback->ExitGame                     = (int (*)(void))RSDK.GetFuncPtr("ExitGame");
    APICallback->ClearAchievements            = RSDK.GetFuncPtr("ClearAchievements");
    APICallback->UnlockAchievement            = (int (*)(const char *))RSDK.GetFuncPtr("UnlockAchievement");
    APICallback->SetRichPresence              = (int (*)(int, char *))RSDK.GetFuncPtr("SetRichPresence");
    APICallback->FetchLeaderboard             = (int (*)(byte, byte, int, int, int, int))RSDK.GetFuncPtr("FetchLeaderboard");
    APICallback->LeaderboardStatus            = (int (*)(void))RSDK.GetFuncPtr("LeaderboardStatus");
    APICallback->LeaderboardEntryCount        = RSDK.GetFuncPtr("LeaderboardEntryCount");
    APICallback->LeaderboardReadEntry         = (int (*)(int))RSDK.GetFuncPtr("LeaderboardReadEntry");
    APICallback->TrackActClear                = (void (*)(byte, byte, byte, int, int, int))RSDK.GetFuncPtr("TrackActClear");
    APICallback->TrackTAClear                 = (void (*)(int, int, int, int))RSDK.GetFuncPtr("TrackTAClear");
    APICallback->TrackEnemyDefeat             = RSDK.GetFuncPtr("TrackEnemyDefeat");
    APICallback->TrackGameProgress            = (void (*)(int))RSDK.GetFuncPtr("TrackGameProgress");
    APICallback->ClearPrerollErrors           = (int (*)(void))RSDK.GetFuncPtr("ClearPrerollErrors");
    APICallback->TryAuth                      = (int (*)(void))RSDK.GetFuncPtr("TryAuth");
    APICallback->GetUserAuthStatus            = (int (*)(void))RSDK.GetFuncPtr("GetUserAuthStatus");
    APICallback->GetUsername                  = (int (*)(int))RSDK.GetFuncPtr("GetUsername");
    APICallback->TryInitStorage               = (int (*)(void))RSDK.GetFuncPtr("TryInitStorage");
    APICallback->GetStorageStatus             = (int (*)(int))RSDK.GetFuncPtr("GetStorageStatus");
    APICallback->LoadUserFile                 = (int (*)(const char *, void *, size_t, int(__cdecl *)(int)))RSDK.GetFuncPtr("LoadUserFile");
    APICallback->SaveUserFile                 = (int (*)(const char *, void *, size_t, int(__cdecl *)(int)))RSDK.GetFuncPtr("SaveUserFile");
    APICallback->GetUserLanguage              = RSDK.GetFuncPtr("GetUserLanguage");
    APICallback->GetConfirmButtonFlip         = (int (*)(void))RSDK.GetFuncPtr("GetConfirmButtonFlip");
    APICallback->ControllerIDForInputID       = (int (*)(int))RSDK.GetFuncPtr("ControllerIDForInputID");
    APICallback->MostRecentActiveControllerID = (int (*)(int))RSDK.GetFuncPtr("MostRecentActiveControllerID");
    APICallback->AssignControllerID           = (void (*)(int, int))RSDK.GetFuncPtr("AssignControllerID");
    APICallback->ResetControllerAssignments   = (void (*)(void))RSDK.GetFuncPtr("ResetControllerAssignments");
    APICallback->InputIDIsDisconnected        = (int (*)(int))RSDK.GetFuncPtr("InputIDIsDisconnected");
    APICallback->GetInputType                 = RSDK.GetFuncPtr("GetInputType");
    APICallback->GetControllerType            = (int (*)(EngineInfo *, int))RSDK.GetFuncPtr("GetControllerType");
    APICallback->ShowSteamControllerOverlay   = (int (*)(int))RSDK.GetFuncPtr("ShowSteamControllerOverlay");
    APICallback->controllerIDs[1]             = 1;
    APICallback->controllerIDs[2]             = 2;
    APICallback->controllerIDs[3]             = 3;
    APICallback->controllerCount              = 4;
}

void APICallback_SetRichPresence(int id, TextInfo *msg)
{
    if (globals->presenceID != id) {
        // RSDK.PrependString(&message, id + '.');
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

bool32 APICallback_GetConfirmButtonFlip()
{
    if (APICallback->GetConfirmButtonFlip)
        return APICallback->GetConfirmButtonFlip();

    LogHelpers_Print("DUMMY GetConfirmButtonFlip()");
    return false;
}

void APICallback_SetNoSave()
{
    APICallback->statusUnknown2 = 500;
    globals->noSave             = true;
}

void APICallback_SaveUserFile(void *buffer, const char *name, int size, int (*callback)(int))
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
        // EntityAPICallback *entityentity = (EntityAPICallback *)RSDK.SpawnEntity(APICallback->id, APICallback_HandleCallback, 0, 0);
        // entity->callback     = APICallback_SaveCB;
        // entity->timer        = 30;
        // entity->fileSize     = size;
        // entity->fileName     = name;
        // entity->fileBuffer   = buffer;
        // entity->fileCallback = callback;
    }
}

int APICallback_SaveCB(int status)
{
    RSDK_THIS(APICallback);
    int saveResult = RSDK.SaveUserFile(entity->fileName, entity->fileBuffer, entity->fileSize);
    LogHelpers_Print("DUMMY DummySaveCB(%s, %x, %d) -> %d", entity->fileName, entity->fileBuffer, entity->fileSize, saveResult);

    if (entity->fileCallback)
        entity->fileCallback(1);
    return 1;
}

int APICallback_ReadLeaderboardEntry(int rankID)
{
    if (APICallback->LeaderboardReadEntry)
        return APICallback->LeaderboardReadEntry(rankID);
    if (!RSDK_info->platformID) {
        LogHelpers_Print("EMPTY LeaderboardReadEntry()");
        return 0;
    }
    LogHelpers_Print("DUMMY LeaderboardReadEntry()");
    if (APICallback->leaderboardsStatus != 200 || rankID < 0 || rankID >= APICallback->leaderboardEntryCount) {
        LogHelpers_Print("ERROR index out of bounds for RankEntry");
        return 0;
    }

    const char *dummyNames[100];
    dummyNames[0]  = "ORCIHILLARY124";
    dummyNames[1]  = "AUCTORJOLIE521";
    dummyNames[2]  = "SENECTUSFLORENCE789";
    dummyNames[3]  = "MAGNAAVRAM503";
    dummyNames[4]  = "SITVERNON320";
    dummyNames[5]  = "DUICHRISTEN429";
    dummyNames[6]  = "NULLAKERMIT649";
    dummyNames[7]  = "INTEGERGEORGE708";
    dummyNames[8]  = "HENDRERITDREW443";
    dummyNames[9]  = "UTULYSSES507";
    dummyNames[10] = "ACCUMSANBRUCE276";
    dummyNames[11] = "BANANAARON804";
    dummyNames[12] = "MAURISSILAS372";
    dummyNames[13] = "ETCALVIN641";
    dummyNames[14] = "UTGALENA780";
    dummyNames[15] = "FEUGIATSHAY665";
    dummyNames[16] = "ORCIJULIET388";
    dummyNames[17] = "ETJENNA318";
    dummyNames[18] = "LIBEROIDOLA420";
    dummyNames[19] = "ATVIELKA576";
    dummyNames[20] = "PHASELLUSJENETTE627";
    dummyNames[21] = "CONSECTETUERCALEB550";
    dummyNames[22] = "SITVICTOR359";
    dummyNames[23] = "PURUSDARRYL50";
    dummyNames[24] = "DONECAVRAM56";
    dummyNames[25] = "PENATIBUSSYBILL30";
    dummyNames[26] = "MAURISBARCLAY563";
    dummyNames[27] = "CONGUECOLLEEN811";
    dummyNames[28] = "MORBIDESTINY655";
    dummyNames[29] = "SEMPERIONA579";
    dummyNames[30] = "RISUSKEANE807";
    dummyNames[31] = "QUISQUEMARYAM927";
    dummyNames[32] = "COMMODOSTEPHANIE54";
    dummyNames[33] = "MALESUADABURTON350";
    dummyNames[34] = "VIVAMUSROBIN330";
    dummyNames[35] = "QUISQUEWHILEMINA413";
    dummyNames[36] = "VESTIBULUMRYDER890";
    dummyNames[37] = "LACUSINDIRA112";
    dummyNames[38] = "NEQUEJESCIE446";
    dummyNames[39] = "EGESTASMORGAN390";
    dummyNames[40] = "ETODYSSEUS95";
    dummyNames[41] = "EGETHERMIONE127";
    dummyNames[42] = "ENIMPASCALE599";
    dummyNames[43] = "ODIOWHOOPI328";
    dummyNames[44] = "DOLORANGELICA718";
    dummyNames[45] = "ODIOSLOANE426";
    dummyNames[46] = "DUIZELDA319";
    dummyNames[47] = "METUSPORTER906";
    dummyNames[48] = "NONUMMYSHANNON166";
    dummyNames[49] = "IPSUMANTHONY906";
    dummyNames[50] = "ACCUMSANRAPHAEL296";
    dummyNames[51] = "ORNAREHU604";
    dummyNames[52] = "MOLESTIERYDER304";
    dummyNames[53] = "FACILISILAITH900";
    dummyNames[54] = "MIYVONNE287";
    dummyNames[55] = "CRASHEDY110";
    dummyNames[56] = "TEMPUSRAE486";
    dummyNames[57] = "ORCICOLBY173";
    dummyNames[58] = "ULTRICESJADEN480";
    dummyNames[59] = "LECTUSBEAU552";
    dummyNames[60] = "LIGULAJESSAMINE824";
    dummyNames[61] = "ORCISOLOMON19";
    dummyNames[62] = "VARIUSDARRYL437";
    dummyNames[63] = "EUISMODKATELL668";
    dummyNames[64] = "LACUSSTEVEN302";
    dummyNames[65] = "ALIQUETRICHARD807";
    dummyNames[66] = "PEDEKAI692";
    dummyNames[67] = "VIVAMUSCHARLOTTE167";
    dummyNames[68] = "MAURISLEWIS101";
    dummyNames[69] = "NULLANELLE11";
    dummyNames[70] = "MAECENASKAMEKO602";
    dummyNames[71] = "LOREMHALEY545";
    dummyNames[72] = "PHASELLUSPAUL954";
    dummyNames[73] = "ERATGLORIA527";
    dummyNames[74] = "DIAMBURKE290";
    dummyNames[75] = "SEMYOKO792";
    dummyNames[76] = "ANTEMICHELLE769";
    dummyNames[77] = "SEMPERDOMINIC873";
    dummyNames[78] = "CONDIMENTUMNYSSA826";
    dummyNames[79] = "PEDEDYLAN675";
    dummyNames[80] = "ETCRUZ380";
    dummyNames[81] = "VELJAQUELYN95";
    dummyNames[82] = "NONDACEY190";
    dummyNames[83] = "UTCAIN386";
    dummyNames[84] = "NULLANOMLANGA941";
    dummyNames[85] = "QUISQUESIERRA659";
    dummyNames[86] = "MATTISABDUL346";
    dummyNames[87] = "PHASELLUSKNOX183";
    dummyNames[88] = "NEQUEXYLA92";
    dummyNames[89] = "ORNAREGAY372";
    dummyNames[90] = "FACILISISIVY303";
    dummyNames[91] = "TELLUSHARLAN339";
    dummyNames[92] = "METUSSOPOLINE75";
    dummyNames[93] = "NUNCKYLYNN927";
    dummyNames[94] = "PROINHERMIONE775";
    dummyNames[95] = "TEMPORFITZGERALD656";
    dummyNames[96] = "VELVELMA504";
    dummyNames[97] = "FAUCIBUSTAMEKAH272";
    dummyNames[98] = "PORTTITORWHOOPI881";
    dummyNames[99] = "EUPETER41";

    APICallback->globalRank = rankID + (APICallback->rankScore + 1);
    APICallback->score      = (4 * APICallback->globalRank + 2400) % 59999;
    APICallback->isUser     = APICallback->prevIsUser && rankID == APICallback->prevRank;
    if (APICallback->isUser) {
        // RSDK.SetText(&APICallback->entryName, off_61A44C, 0);
    }
    else {
        // RSDK.SetText(&APICallback->entryName, &textBuffer, 0);
        // RSDK.FuncUnknown3((int)&APICallback->entryName, (int)dummyNames[rankID]);
    }
    LogHelpers_Print("RSDKRankEntry { globalRank: %d, score: %d, entryName: %s, isUser: %d }", APICallback->globalRank, APICallback->score,
               APICallback->entryName.text, APICallback->isUser);
    return 1;
}

void APICallback_PrompSavePreference(void *this)
{
    if (globals->noSave) {
        LogHelpers_Print("PromptSavePreference() returning due to noSave");
        return;
    }
    LogHelpers_Print("PromptSavePreference()");

    if (APICallback->statusUnknown2 == 100) {
        LogHelpers_Print("WARNING PromptSavePreference() when prompt already in progress.");
    }
    APICallback->statusUnknown2 = 100;

    // EntityAPICallback *entity   = (EntityAPICallback *)RSDK.SpawnEntity(APICallback->id, APICallback_Unknown3, 0, 0);
    // entity->field_84            = (int)this;
    // APICallback->entityPtr      = (Entity*)entity;
}

void APICallback_LoadUserFile(void *buffer, const char *name, int size, int (*callback)(int))
{
    if (globals->noSave) {
        LogHelpers_Print("LoadUserFile(%s, %X, %d, %X) loading 0's due to noSave", name, buffer, size, callback);
        memset(buffer, 0, size);
        callback(0);
    }
    else if (APICallback->LoadUserFile) {
        LogHelpers_Print("API LoadUserFile(%s, %X, %d, %X)", name, buffer, size, callback);
        APICallback->LoadUserFile(name, buffer, size, callback);
    }
    else {
        LogHelpers_Print("DUMMY LoadUserFile(%s, %X, %d)", name, buffer, size);
        EntityAPICallback *entity = (EntityAPICallback *)RSDK.CreateEntity(APICallback->objectID, APICallback_HandleCallback, 0, 0);
        entity->callback          = (void (*)(void))APICallback_LoadCB;
        entity->timer             = 60;
        entity->fileSize          = size;
        entity->fileName          = name;
        entity->fileBuffer        = buffer;
        entity->fileCallback      = callback;
    }
}

int APICallback_LoadCB(int status)
{
    RSDK_THIS(APICallback);
    int loadResult = RSDK.LoadUserFile(entity->fileName, entity->fileBuffer, entity->fileSize);
    LogHelpers_Print("DUMMY DummyLoadCB(%s, %x, %d) -> %d", entity->fileName, entity->fileBuffer, entity->fileSize, loadResult);

    if (entity->fileCallback)
        entity->fileCallback(1);
    return 1;
}

int APICallback_LeaderboardStatus()
{
    if (APICallback->LeaderboardStatus)
        return APICallback->LeaderboardStatus();

    int status = 0;
    if (RSDK_info->platformID < PLATFORM_PS4) {
        LogHelpers_Print("EMPTY LeaderboardStatus()");
        status = 0;
    }
    else {
        LogHelpers_Print("DUMMY LeaderboardStatus()");
        if (APICallback->leaderboardsStatus == 100) {
            if (APICallback->unknown < 60) {
                APICallback->unknown++;
                return APICallback->leaderboardsStatus;
            }

            status = 200;
            if ((RSDK.Rand(0, 100) % 10) > 10)
                status = 500;
            APICallback->leaderboardsStatus = status;
        }
        status = APICallback->leaderboardsStatus;
    }
    return status;
}

int APICallback_LeaderboardEntryCount()
{
    if (APICallback->LeaderboardEntryCount)
        return APICallback->LeaderboardEntryCount();
    if (RSDK_info->platformID < PLATFORM_PS4) {
        LogHelpers_Print("EMPTY LeaderboardEntryCount()");
    }
    else {
        LogHelpers_Print("DUMMY LeaderboardEntryCount()");
        if (APICallback->leaderboardsStatus == 200)
            return APICallback->leaderboardEntryCount;
    }
    return -1;
}

void APICallback_LaunchManual(void *this)
{
    // RSDK.PlaySFX(*(_WORD *)(*(_DWORD *)&OBJ_UIWidgets + 112), 0, 0xFFu);

    if (APICallback->LaunchManual) {
        APICallback->LaunchManual(this);
    }
    else if (RSDK_info->platformID == PLATFORM_DEV) {
        LogHelpers_Print("DUMMY LaunchManual()");
    }
    else {
        LogHelpers_Print("EMPTY LaunchManual()");
    }
}

void APICallback_HandleCallback()
{
    RSDK_THIS(APICallback);

    if (entity->timer <= 0) {
        LogHelpers_Print("Callback: %x", entity->callback);
        if (entity->callback)
            entity->callback();
        RSDK.ResetEntityPtr(entity, TYPE_BLANK, 0);
    }
    else {
        entity->timer--;
    }
}

void APICallback_GetUserAuthStatus()
{
    int status = 0;
    if (APICallback->GetUserAuthStatus) {
        status = APICallback->GetUserAuthStatus();
    }
    else {
        status = 200;
    }

    if (APICallback->statusUnknown2 || status != 500) {
        if (status == 403 && !APICallback->authFlag) {
            // EntityAPICallback *entity  = (EntityAPICallback *)RSDK.CreateEntity(result->id, APICallback_Unknown4, 0, 0);
            // entity->active             = ACTIVE_ALWAYS;
            // APICallback->entityPtr     = entity;
            APICallback->authFlag = true;
        }
    }
    else {
        APICallback->statusUnknown2 = 500;
    }
}

signed int APICallback_GetStorageStatus()
{
    if (APICallback->statusUnknown2 == 500)
        return 500;

    int status = 0;
    if (APICallback->GetStorageStatus) {
        status = APICallback->GetStorageStatus(0);
    }
    else if (RSDK_info->platformID >= PLATFORM_PS4) {
        if (APICallback->entityPtr) {
            status = 100;
        }
        else {
            if (APICallback->storageStatusCode == 100) {
                if (APICallback->unknown >= 0) {
                    status = 200;
                    if (RSDK.Rand(0, 100) % 10 > 10)
                        status = 500;
                    APICallback->storageStatusCode = status;
                }
                else {
                    APICallback->unknown++;
                }
            }
            status = APICallback->storageStatusCode;
        }
    }
    if (!APICallback->statusUnknown2) {
        if (status == 500) {
            APICallback->statusUnknown2 = 500;
            return 500;
        }
        if (status == 200)
            APICallback->statusUnknown2 = 200;
    }
    return status;
}

void APICallback_GetStatusUnkown3()
{
    APICallback->userStorageStatus = 500;
    APICallback->statusUnknown2    = 500;
}

int APICallback_GetStatusUnkown2()
{
    if (globals->noSave)
        return 200;

    if (APICallback->entityPtr)
        return 100;
    else
        return APICallback->statusUnknown2;
}

int APICallback_GetControllerType(void *this)
{
    if (APICallback->GetControllerType)
        return APICallback->GetControllerType(this, 0);
    else
        return RSDK_info->platformID == PLATFORM_DEV;
}

int APICallback_FetchLeaderboardData(byte a1, byte a2, int a3, int a4, int a5, int isUser)
{
    if (APICallback->FetchLeaderboard)
        return APICallback->FetchLeaderboard(a2, a1, a3, 0, 100, isUser);
    if (RSDK_info->platformID < PLATFORM_PS4) {
        LogHelpers_Print("EMPTY FetchLeaderboardData(%d, %d, %d, %d, %d, %d)", a2, a1, (byte)a3, 0, 100, isUser);
        return 0;
    }

    LogHelpers_Print("DUMMY FetchLeaderboardData(%d, %d, %d, %d, %d, %d)", a2, a1, (byte)a3, 0, 100, isUser);
    if (APICallback->leaderboardsStatus == 100) {
        LogHelpers_Print("WARNING Called FetchLeaderboard() while loading");
        return 0;
    }

    APICallback->rankScore             = 0;
    APICallback->leaderboardEntryCount = RSDK.Rand(0, 100);
    APICallback->prevIsUser            = isUser;
    if (isUser) {
        APICallback->rankScore = RSDK.Rand(1000, 9999);
        APICallback->prevRank  = RSDK.Rand(0, APICallback->leaderboardEntryCount - 1);
    }
    APICallback->leaderboardsStatus = 100;
    APICallback->unknown            = 0;
    return 0;
}

void APICallback_ExitGame()
{
    if (APICallback->ExitGame) {
        APICallback->ExitGame();
    }
    else {
        if (RSDK_info->platformID == PLATFORM_DEV)
            exit(0);
        LogHelpers_Print("EMPTY ExitGame()");
    }
}

void APICallback_ClearPrerollErrors()
{
    if (APICallback->TryAuth) {
        LogHelpers_Print("API ClearPrerollErrors()");
        APICallback->ClearPrerollErrors();
    }
    else {
        LogHelpers_Print("DUMMY ClearPrerollErrors()");
        if (APICallback->userStorageStatus != 200) {
            APICallback->userStorageStatus = 0;
        }
        if (APICallback->storageStatusCode != 200)
            APICallback->storageStatusCode = 0;
    }
}

void APICallback_CheckNoSave()
{
    APICallback->statusUnknown2 = 403;
    globals->noSave             = false;
}

bool32 APICallback_CheckInputDisconnected()
{
    RSDK_THIS(APICallback);
    if (APICallback->InputIDIsDisconnected) {
        return APICallback->InputIDIsDisconnected(entity->field_7C) /*|| *(_DWORD *)(PauseMenu + 16)*/;
    }
    return false;
}

void APICallback_TrackGameProgress(int (*callback)(int), float a2)
{
    if (globals->noSave || globals->saveLoaded != 200) {
        LogHelpers_Print("WARNING GameProgress Attempted to save before loading SaveGame file");
    }
    else if (!globals->saveRAM[0x900]) {
        // sub_420250();

        if (APICallback->TrackGameProgress)
            APICallback->TrackGameProgress(a2);
        else
            LogHelpers_Print("EMPTY TrackGameProgress(%f)", a2);
       //SaveGame_SaveGameProgress(callback);
    }
    if (callback)
        callback(0);
}

int APICallback_TryAuth()
{
    if (APICallback->TryAuth) {
        LogHelpers_Print("API TryAuth()");
        return APICallback->TryAuth();
    }
    else {
        LogHelpers_Print("DUMMY TryAuth()");
        if (APICallback->userStorageStatus == 100) {
            LogHelpers_Print("WARNING TryAuth() when auth already in progress.");
        }
        APICallback->unknown           = 0;
        APICallback->userStorageStatus = 100;
        // RSDK.CreateEntity(APICallback->objectID, APICallback_Unknown1, 0, 0);
        return 100;
    }
}

void APICallback_TryInitStorage()
{
    if (APICallback->TryInitStorage) {
        LogHelpers_Print("API TryInitStorage()");
        APICallback->TryInitStorage();
    }
    else {
        LogHelpers_Print("DUMMY TryInitStorage()");
        if (APICallback->storageStatusCode == 100) {
            LogHelpers_Print("WARNING TryInitStorage() when init already in progress.");
        }
        APICallback->unknown           = 0;
        APICallback->storageStatusCode = 100;
    }
}

void APICallback_UnlockAchievement(const char *name)
{
    if (APICallback->achievementsDisabled) {
        LogHelpers_Print("SKIP UnlockAchievement(%s)", name);
        return;
    }

    if (APICallback->UnlockAchievement) {
        APICallback->UnlockAchievement(name);
        return;
    }
    if (RSDK_info->platformID == PLATFORM_DEV) {
        LogHelpers_Print("DUMMY UnlockAchievement(%s)", name);
        return;
    }

    LogHelpers_Print("EMPTY UnlockAchievement(%s)", name);
}

void APICallback_EditorDraw(void) {}

void APICallback_EditorLoad(void) {}

void APICallback_Serialize(void) {}

#endif
