#ifndef OBJ_APICALLBACK_H
#define OBJ_APICALLBACK_H

#include "SonicMania.h"

#if RETRO_USE_PLUS
#define sku_platform RSDK_sku->platform
#define sku_language RSDK_sku->language
#define sku_region   RSDK_sku->region

#define API_GetConfirmButtonFlip         API.GetConfirmButtonFlip
#define API_UnlockAchievement            API.UnlockAchievement
#define API_SetRichPresence              API.SetRichPresence
#define API_LoadUserFile                 API.LoadUserFile
#define API_SaveUserFile                 API.SaveUserFile
#define API_GetUserAuthStatus            API.GetUserAuthStatus
#define API_ControllerIDForInputID       RSDK.ControllerIDForInputID
#define API_AssignControllerID           RSDK.AssignControllerID
#define API_InputIDIsDisconnected        RSDK.InputIDIsDisconnected
#define API_MostRecentActiveControllerID RSDK.MostRecentActiveControllerID
#define API_ResetControllerAssignments   RSDK.ResetControllerAssignments
#define API_GetControllerType            RSDK.GetControllerType
#define API_GetUsername                  API.GetUsername
#define API_ReadLeaderboardEntry         API.ReadLeaderboardEntry
#else
#define sku_platform RSDK_info->platform
#define sku_language RSDK_info->language
#define sku_region   RSDK_info->region

#define API_GetConfirmButtonFlip         APICallback_GetConfirmButtonFlip
#define API_UnlockAchievement            APICallback_UnlockAchievement
#define API_SetRichPresence              APICallback_SetRichPresence
#define API_LoadUserFile                 APICallback_LoadUserFile
#define API_SaveUserFile                 APICallback_SaveUserFile
#define API_GetUserAuthStatus            APICallback_GetUserAuthStatus
#define API_ControllerIDForInputID       APICallback_ControllerIDForInputID
#define API_AssignControllerID           APICallback_AssignControllerID
#define API_InputIDIsDisconnected        APICallback_InputIDIsDisconnected
#define API_MostRecentActiveControllerID APICallback_MostRecentActiveControllerID
#define API_ResetControllerAssignments   APICallback_ResetControllerAssignments
#define API_GetControllerType            APICallback_GetControllerType
#define API_GetUsername                  APICallback_GetUsername
#define API_ReadLeaderboardEntry         APICallback_ReadLeaderboardEntry
#endif

//90% sure this is "DialogRunner" in plus/1.05
#if !RETRO_USE_PLUS
// Object Class
typedef struct {
    RSDK_OBJECT
    int32(*LaunchManual)(void);
    int32 (*ExitGame)(void);
    void (*ClearAchievements)(void);
    int32(*UnlockAchievement)(const char *);
    int32(*SetRichPresence)(int32, TextInfo *);
    int32 (*LoadUserFile)(const char *, void *, size_t, void(*)(int32));
    int32 (*SaveUserFile)(const char *, void *, size_t, void(*)(int32));
    void (*SaveSettingsINI)(void);
    int32 (*GetUserLanguage)(void);
    int32 (*GetConfirmButtonFlip)(void);
    int32(*FetchLeaderboard)(uint8, uint8, int32, int32, int32, int32);
    int32 (*LeaderboardStatus)(void);
    int32 (*LeaderboardEntryCount)(void);
    LeaderboardEntry *(*LeaderboardReadEntry)(int32);
    void(*TrackActClear)(uint8, uint8, uint8, int32, int32, int32);
    void(*TrackTAClear)(int32, int32, int32, int32);
    void (*TrackEnemyDefeat)(uint8 zoneID, uint8 actID, uint8 playerID, int32 entityX, int32 entityY);
    void(*TrackGameProgress)(int32);
    int32 (*ClearPrerollErrors)(void);
    int32 (*TryAuth)(void);
    int32 (*GetUserAuthStatus)(void);
    int32(*GetUsername)(TextInfo *info);
    int32 (*TryInitStorage)(void);
    int32(*GetStorageStatus)(int32);
    int32(*ControllerIDForInputID)(int32);
    int32(*MostRecentActiveControllerID)(int32);
    void(*AssignControllerID)(int32, int32);
    void (*ResetControllerAssignments)(void);
    int32(*InputIDIsDisconnected)(int32);
    void *GetInputType;
    int32 (*GetControllerType)(int32 id, int32);
    int32(*ShowSteamControllerOverlay)(int32);
    int32 saveStatus;
    bool32 authFlag;
    bool32 signoutFlag;
    int32 prevLeaderboardEntry;
    int32 playerFlipFlags;
    int32 notifyAutoSaveFlag;
    int32 unknown;
    int32 leaderboardsStatus;
    int32 leaderboardEntryCount;
    int32 rankScore;
    int32 prevIsUser;
    int32 prevRank;
    LeaderboardEntry leaderboardEntry;
    int32 controllerIDs[4];
    int32 controllerCount;
    Entity *activeEntity;
    int32 authStatus;
    int32 storageStatus;
    bool32 achievementsDisabled;
} ObjectAPICallback;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    void (*callback)(void);
    const char *fileName;
    void *fileBuffer;
    uint32 fileSize;
    void(*fileCallback)(int32);
    int32 field_74;
    int32 field_78;
    int32 field_7C;
    int32 field_80;
    int32 status;
} EntityAPICallback;

// Object Struct
extern ObjectAPICallback *APICallback;

// Standard Entity Events
void APICallback_Update(void);
void APICallback_LateUpdate(void);
void APICallback_StaticUpdate(void);
void APICallback_Draw(void);
void APICallback_Create(void* data);
void APICallback_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void APICallback_EditorDraw(void);
void APICallback_EditorLoad(void);
#endif
void APICallback_Serialize(void);

// Extra Entity Functions
void APICallback_SetRichPresence(int32 id, TextInfo *msg);
int32 APICallback_GetUserLanguage(void);
bool32 APICallback_GetConfirmButtonFlip(void);
void APICallback_SetNoSaveEnabled(void);
void APICallback_SetNoSaveDisabled(void);
void APICallback_SaveUserFile(const char *name, void *buffer, int32 size, void (*callback)(int32));
void APICallback_SaveCB(void);
void APICallback_SaveSettingsINI();
LeaderboardEntry *APICallback_ReadLeaderboardEntry(int32 rankID);
void APICallback_NotifyAutoSave_OK(void);
void APICallback_NotifyAutoSave_CB(void);
void APICallback_PromptSavePreference_CB(void);
void APICallback_PromptSavePreference(int32 status);
void APICallback_LoadUserFile(const char *name, void *buffer, int32 size, void (*callback)(int32));
int32 APICallback_LoadCB(void);
int32 APICallback_LeaderboardStatus(void);
int32 APICallback_LeaderboardEntryCount(void);
void APICallback_LaunchManual(void);
void APICallback_HandleCallback(void);
int32 APICallback_GetUserAuthStatus(void);
int32 APICallback_GetStorageStatus(void);
int32 APICallback_GetSaveStatus(void);
int32 APICallback_GetControllerType(int32 id);
int32 APICallback_FetchLeaderboardData(uint8 a1, uint8 a2, int32 a3, int32 a4, int32 a5, bool32 isUser);
void APICallback_ExitGame(void);
void APICallback_ClearPrerollErrors(void);
bool32 APICallback_CheckInputDisconnected(void);
bool32 APICallback_InputIDIsDisconnected(int32 id);
int32 APICallback_ControllerIDForInputID(int32 id);
int32 APICallback_MostRecentActiveControllerID(int32 id);
void APICallback_AssignControllerID(int32 controllerID, int32 id);
void APICallback_ResetControllerAssignments(void);
void APICallback_TrackActClear(uint8 zoneID, uint8 actID, uint8 playerID, int32 score, int32 rings, int32 time);
void APICallback_TrackTAClear(uint8 zoneID, uint8 actID, uint8 playerID, int32 time);
void APICallback_TrackEnemyDefeat(uint8 zoneID, uint8 actID, uint8 playerID, int32 entityX, int32 entityY);
void APICallback_TryAuth_No(void);
void APICallback_TryAuth_Yes(void);
void APICallback_TryAuth_CB(void);
int32 APICallback_TryAuth(void);
void APICallback_TryInitStorage(void);
bool32 APICallback_GetUsername(TextInfo *info);
void APICallback_ClearAchievements(void);
void APICallback_UnlockAchievement(const char *name);
void APICallback_CheckUserAuth_OK(void);
void APICallback_CheckUserAuth_CB(void);
void APICallback_Wait(int32 success);
void APICallback_GetNextNotif(void);
void APICallback_ManageNotifs(void);
bool32 APICallback_CheckUnreadNotifs(void);
bool32 APICallback_NotifyAutosave(void);
#endif

#endif //!OBJ_APICALLBACK_H
