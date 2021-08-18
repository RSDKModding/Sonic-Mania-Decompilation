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
    int(*LaunchManual)(void);
    int (*ExitGame)(void);
    void (*ClearAchievements)(void);
    int(*UnlockAchievement)(const char *);
    int(*SetRichPresence)(int, TextInfo *);
    int (*LoadUserFile)(const char *, void *, size_t, void(*)(int));
    int (*SaveUserFile)(const char *, void *, size_t, void(*)(int));
    void (*SaveSettingsINI)(void);
    int (*GetUserLanguage)(void);
    int (*GetConfirmButtonFlip)(void);
    int(*FetchLeaderboard)(byte, byte, int, int, int, int);
    int (*LeaderboardStatus)(void);
    int (*LeaderboardEntryCount)(void);
    LeaderboardEntry *(*LeaderboardReadEntry)(int);
    void(*TrackActClear)(byte, byte, byte, int, int, int);
    void(*TrackTAClear)(int, int, int, int);
    void (*TrackEnemyDefeat)(byte zoneID, byte actID, byte playerID, int entityX, int entityY);
    void(*TrackGameProgress)(int);
    int (*ClearPrerollErrors)(void);
    int (*TryAuth)(void);
    int (*GetUserAuthStatus)(void);
    int(*GetUsername)(TextInfo *info);
    int (*TryInitStorage)(void);
    int(*GetStorageStatus)(int);
    int(*ControllerIDForInputID)(int);
    int(*MostRecentActiveControllerID)(int);
    void(*AssignControllerID)(int, int);
    void (*ResetControllerAssignments)(void);
    int(*InputIDIsDisconnected)(int);
    void *GetInputType;
    int (*GetControllerType)(int id, int);
    int(*ShowSteamControllerOverlay)(int);
    int saveStatus;
    bool32 authFlag;
    bool32 signoutFlag;
    int prevLeaderboardEntry;
    int playerFlipFlags;
    int notifyAutoSaveFlag;
    int unknown;
    int leaderboardsStatus;
    int leaderboardEntryCount;
    int rankScore;
    int prevIsUser;
    int prevRank;
    LeaderboardEntry leaderboardEntry;
    int controllerIDs[4];
    int controllerCount;
    Entity *activeEntity;
    int authStatus;
    int storageStatus;
    bool32 achievementsDisabled;
} ObjectAPICallback;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int timer;
    void (*callback)(void);
    const char *fileName;
    void *fileBuffer;
    uint fileSize;
    void(*fileCallback)(int);
    int field_74;
    int field_78;
    int field_7C;
    int field_80;
    int status;
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
void APICallback_EditorDraw(void);
void APICallback_EditorLoad(void);
void APICallback_Serialize(void);

// Extra Entity Functions
void APICallback_SetRichPresence(int id, TextInfo *msg);
int APICallback_GetUserLanguage(void);
bool32 APICallback_GetConfirmButtonFlip(void);
void APICallback_SetNoSaveEnabled(void);
void APICallback_SetNoSaveDisabled(void);
void APICallback_SaveUserFile(const char *name, void *buffer, int size, void (*callback)(int));
void APICallback_SaveCB(void);
void APICallback_SaveSettingsINI();
LeaderboardEntry *APICallback_ReadLeaderboardEntry(int rankID);
void APICallback_NotifyAutoSave_OK(void);
void APICallback_NotifyAutoSave_CB(void);
void APICallback_PromptSavePreference_CB(void);
void APICallback_PromptSavePreference(int status);
void APICallback_LoadUserFile(const char *name, void *buffer, int size, void (*callback)(int));
int APICallback_LoadCB(void);
int APICallback_LeaderboardStatus(void);
int APICallback_LeaderboardEntryCount(void);
void APICallback_LaunchManual(void);
void APICallback_HandleCallback(void);
int APICallback_GetUserAuthStatus(void);
int APICallback_GetStorageStatus(void);
int APICallback_GetSaveStatus(void);
int APICallback_GetControllerType(int id);
int APICallback_FetchLeaderboardData(byte a1, byte a2, int a3, int a4, int a5, bool32 isUser);
void APICallback_ExitGame(void);
void APICallback_ClearPrerollErrors(void);
bool32 APICallback_CheckInputDisconnected(void);
bool32 APICallback_InputIDIsDisconnected(int id);
int APICallback_ControllerIDForInputID(int id);
int APICallback_MostRecentActiveControllerID(int id);
void APICallback_AssignControllerID(int controllerID, int id);
void APICallback_ResetControllerAssignments(void);
void APICallback_TrackActClear(byte zoneID, byte actID, byte playerID, int score, int rings, int time);
void APICallback_TrackTAClear(byte zoneID, byte actID, byte playerID, int time);
void APICallback_TrackEnemyDefeat(byte zoneID, byte actID, byte playerID, int entityX, int entityY);
void APICallback_TryAuth_No(void);
void APICallback_TryAuth_Yes(void);
void APICallback_TryAuth_CB(void);
int APICallback_TryAuth(void);
void APICallback_TryInitStorage(void);
bool32 APICallback_GetUsername(TextInfo *info);
void APICallback_ClearAchievements(void);
void APICallback_UnlockAchievement(const char *name);
void APICallback_CheckUserAuth_OK(void);
void APICallback_CheckUserAuth_CB(void);
void APICallback_Wait(int success);
void APICallback_GetNextNotif(void);
void APICallback_ManageNotifs(void);
bool32 APICallback_CheckUnreadNotifs(void);
bool32 APICallback_NotifyAutosave(void);
#endif

#endif //!OBJ_APICALLBACK_H
