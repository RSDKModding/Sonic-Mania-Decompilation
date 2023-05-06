#ifndef OBJ_APICALLBACK_H
#define OBJ_APICALLBACK_H

#include "Game.h"

#if MANIA_USE_PLUS
#define sku_platform SKU->platform
#define sku_language SKU->language
#define sku_region   SKU->region

#define API_GetConfirmButtonFlip            API.GetConfirmButtonFlip
#define API_UnlockAchievement(info)         API.TryUnlockAchievement(info)
#define API_SetAchievementsEnabled(enabled) API.SetAchievementsEnabled(enabled)
#define API_GetAchievementsEnabled()        API.GetAchievementsEnabled()
#define API_SetRichPresence                 API.SetRichPresence
#define API_LaunchManual                    API.LaunchManual
#define API_LoadUserFile                    API.LoadUserFile
#define API_SaveUserFile                    API.SaveUserFile
#define API_GetNoSave()                     API.GetNoSave()
#define API_SetNoSave(status)               API.SetNoSave(status)
#define API_GetUserAuthStatus               API.GetUserAuthStatus
#define API_ClearPrerollErrors              API.ClearPrerollErrors
#define API_GetInputDeviceID                RSDK.GetInputDeviceID
#define API_AssignInputSlotToDevice         RSDK.AssignInputSlotToDevice
#define API_IsInputDeviceAssigned(deviceID) RSDK.IsInputDeviceAssigned(deviceID)
#define API_IsInputSlotAssigned(inputSlot)  RSDK.IsInputSlotAssigned(inputSlot)
#define API_GetFilteredInputDeviceID        RSDK.GetFilteredInputDeviceID
#define API_ResetInputSlotAssignments       RSDK.ResetInputSlotAssignments
#define API_GetInputDeviceType              RSDK.GetInputDeviceType
#define API_GetUsername                     API.GetUsername
#define API_ReadLeaderboardEntry            API.ReadLeaderboardEntry
#else
#define sku_platform GameInfo->platform
#define sku_language GameInfo->language
#define sku_region   GameInfo->region

#define API_GetConfirmButtonFlip            APICallback_GetConfirmButtonFlip
#define API_UnlockAchievement(info)         APICallback_UnlockAchievement((info)->id)
#define API_SetAchievementsEnabled(enabled) APICallback->achievementsDisabled = !(enabled)
#define API_GetAchievementsEnabled()        !APICallback->achievementsDisabled
#define API_SetRichPresence                 APICallback_SetRichPresence
#define API_LaunchManual                    APICallback_LaunchManual
#define API_LoadUserFile                    APICallback_LoadUserFile
#define API_SaveUserFile                    APICallback_SaveUserFile
#define API_GetNoSave()                     globals->noSave
#define API_SetNoSave(status)               globals->noSave = status
#define API_GetUserAuthStatus               APICallback_GetUserAuthStatus
#define API_ClearPrerollErrors              APICallback_ClearPrerollErrors
#define API_GetInputDeviceID                APICallback_ControllerIDForInputID
#define API_AssignInputSlotToDevice         APICallback_AssignControllerID
#define API_IsInputDeviceAssigned(deviceID) ; // doesn't exist in preplus afaik
#define API_IsInputSlotAssigned(inputSlot)  !APICallback_InputIDIsDisconnected(inputSlot)
#define API_GetFilteredInputDeviceID        APICallback_MostRecentActiveControllerID
#define API_ResetInputSlotAssignments       APICallback_ResetControllerAssignments
#define API_GetInputDeviceType              APICallback_GetControllerType
#define API_GetUsername                     APICallback_GetUsername
#define API_ReadLeaderboardEntry            APICallback_ReadLeaderboardEntry
#endif

// 90% sure this is "DialogRunner" in plus/1.05
#if !MANIA_USE_PLUS
// Object Class
struct ObjectAPICallback {
    RSDK_OBJECT
    int32 (*LaunchManual)(void);
    int32 (*ExitGame)(void);
    void (*ClearAchievements)(void);
    int32 (*UnlockAchievement)(AchievementID *);
    int32 (*SetRichPresence)(int32, String *);
    int32 (*LoadUserFile)(const char *, void *, size_t, void (*)(int32));
    int32 (*SaveUserFile)(const char *, void *, size_t, void (*)(int32));
    void (*SaveSettingsINI)(void);
    int32 (*GetUserLanguage)(void);
    int32 (*GetConfirmButtonFlip)(void);
    int32 (*FetchLeaderboard)(uint8, uint8, int32, int32, int32, int32);
    int32 (*LeaderboardStatus)(void);
    int32 (*LeaderboardEntryCount)(void);
    LeaderboardEntry *(*LeaderboardReadEntry)(int32);
    void (*TrackActClear)(uint8, uint8, uint8, int32, int32, int32);
    void (*TrackTAClear)(int32, int32, int32, int32);
    void (*TrackEnemyDefeat)(uint8 zoneID, uint8 actID, uint8 playerID, int32 entityX, int32 entityY);
    void (*TrackGameProgress)(int32);
    int32 (*ClearPrerollErrors)(void);
    int32 (*TryAuth)(void);
    int32 (*GetUserAuthStatus)(void);
    int32 (*GetUsername)(String *userName);
    int32 (*TryInitStorage)(void);
    int32 (*GetStorageStatus)(int32);
    int32 (*ControllerIDForInputID)(uint8 inputSlot);
    int32 (*MostRecentActiveControllerID)(uint8 inputSlot);
    void (*AssignControllerID)(uint8 inputSlot, uint32 deviceID);
    void (*ResetControllerAssignments)(void);
    int32 (*InputIDIsDisconnected)(uint8 inputSlot);
    void *GetInputType;
    int32 (*GetControllerType)(int32 deviceID);
    int32 (*ShowSteamControllerOverlay)(int32 overlay);
    int32 saveStatus;
    bool32 authForbidden;
    bool32 signedOut;
    int32 prevLeaderboardEntry;
    int32 unused;
    int32 isAutoSaving;
    int32 statusTimer;
    int32 leaderboardsStatus;
    int32 leaderboardEntryCount;
    int32 rankScore;
    int32 prevIsUser;
    int32 prevRank;
    LeaderboardEntry leaderboardEntry;
    int32 inputSlots[PLAYER_COUNT];
    int32 controllerCount;
    Entity *activeEntity;
    int32 authStatus;
    int32 storageStatus;
    bool32 achievementsDisabled;
};

// Entity Class
struct EntityAPICallback {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    void (*callback)(void);
    const char *fileName;
    void *fileBuffer;
    uint32 fileSize;
    void (*fileCallback)(int32);
    int32 unused;
    int32 minVelocity;
    int32 deviceID;
    int32 unused3;
    int32 status;
};

// Object Struct
extern ObjectAPICallback *APICallback;

// Standard Entity Events
void APICallback_Update(void);
void APICallback_LateUpdate(void);
void APICallback_StaticUpdate(void);
void APICallback_Draw(void);
void APICallback_Create(void *data);
void APICallback_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void APICallback_EditorDraw(void);
void APICallback_EditorLoad(void);
#endif
void APICallback_Serialize(void);

// Extra Entity Functions
void APICallback_SetRichPresence(int32 id, String *msg);
int32 APICallback_GetUserLanguage(void);
bool32 APICallback_GetConfirmButtonFlip(void);
void APICallback_SetNoSaveEnabled(void);
void APICallback_SetNoSaveDisabled(void);
void APICallback_SaveUserFile(const char *name, void *buffer, int32 size, void (*callback)(int32));
void APICallback_SaveCB(void);
void APICallback_SaveSettingsINI(void);
LeaderboardEntry *APICallback_ReadLeaderboardEntry(int32 rankID);
void APICallback_NotifyAutoSave_OK(void);
void APICallback_NotifyAutoSave_CB(void);
void APICallback_PromptSavePreference_CB(void);
void APICallback_PromptSavePreference(int32 status);
void APICallback_LoadUserFile(const char *name, void *buffer, int32 size, void (*callback)(int32));
void APICallback_LoadCB(void);
int32 APICallback_LeaderboardStatus(void);
int32 APICallback_LeaderboardEntryCount(void);
void APICallback_LaunchManual(void);
void APICallback_HandleCallback(void);
int32 APICallback_GetUserAuthStatus(void);
int32 APICallback_GetStorageStatus(void);
int32 APICallback_GetSaveStatus(void);
int32 APICallback_GetControllerType(int32 id);
int32 APICallback_FetchLeaderboardData(uint8 zoneID, uint8 actID, int32 playerID, int32 start, int32 end, bool32 isUser);
void APICallback_ExitGame(void);
void APICallback_ClearPrerollErrors(void);
bool32 APICallback_CheckInputDisconnected(void);
bool32 APICallback_InputIDIsDisconnected(uint8 inputSlot);
int32 APICallback_ControllerIDForInputID(uint8 inputSlot);
int32 APICallback_MostRecentActiveControllerID(uint8 inputSlot);
void APICallback_AssignControllerID(uint8 inputSlot, int32 deviceID);
void APICallback_ResetControllerAssignments(void);
void APICallback_TrackActClear(uint8 zoneID, uint8 actID, uint8 playerID, int32 time, int32 rings, int32 score);
void APICallback_TrackTAClear(uint8 zoneID, uint8 actID, uint8 playerID, int32 time);
void APICallback_TrackEnemyDefeat(uint8 zoneID, uint8 actID, uint8 playerID, int32 x, int32 y);
void APICallback_TrackGameProgress(float percent);
void APICallback_TryAuth_No(void);
void APICallback_TryAuth_Yes(void);
void APICallback_TryAuth_CB(void);
int32 APICallback_TryAuth(void);
void APICallback_TryInitStorage(void);
bool32 APICallback_GetUsername(String *userName);
void APICallback_ClearAchievements(void);
void APICallback_UnlockAchievement(const char *name);
void APICallback_CheckUserAuth_OK(void);
void APICallback_CheckUserAuth_CB(void);
#if MANIA_USE_PLUS
void APICallback_TrackGameProgressCB(bool32 success);
#else
void APICallback_TrackGameProgressCB(void);
#endif
void APICallback_GetNextNotif(void);
void APICallback_ManageNotifs(void);
bool32 APICallback_CheckUnreadNotifs(void);
bool32 APICallback_NotifyAutosave(void);
#endif

#endif //! OBJ_APICALLBACK_H
