#ifndef OBJ_APICALLBACK_H
#define OBJ_APICALLBACK_H

#include "SonicMania.h"

//90% sure this is "DialogRunner" in plus/1.05
#if !RETRO_USE_PLUS
// Object Class
typedef struct {
    RSDK_OBJECT
    int(*LaunchManual)(void *);
    int (*ExitGame)(void);
    void (*ClearAchievements)(void);
    int(*UnlockAchievement)(const char *);
    int(*SetRichPresence)(int, TextInfo *);
    int (*LoadUserFile)(const char *, void *, size_t, void(*)(int));
    int (*SaveUserFile)(const char *, void *, size_t, void(*)(int));
    void *GetUserLanguage;
    int (*GetConfirmButtonFlip)(void);
    int(*FetchLeaderboard)(byte, byte, int, int, int, int);
    int (*LeaderboardStatus)(void);
    int (*LeaderboardEntryCount)(void);
    int(*LeaderboardReadEntry)(int);
    void(*TrackActClear)(byte, byte, byte, int, int, int);
    void(*TrackTAClear)(int, int, int, int);
    void *TrackEnemyDefeat;
    void(*TrackGameProgress)(int);
    int (*ClearPrerollErrors)(void);
    int (*TryAuth)(void);
    int (*GetUserAuthStatus)(void);
    int(*GetUsername)(int);
    int (*TryInitStorage)(void);
    int(*GetStorageStatus)(int);
    int(*ControllerIDForInputID)(int);
    int(*MostRecentActiveControllerID)(int);
    void(*AssignControllerID)(int, int);
    void (*ResetControllerAssignments)(void);
    int(*InputIDIsDisconnected)(int);
    void *GetInputType;
    int(*GetControllerType)(EngineInfo *, int);
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
    Entity *curCallback;
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
void APICallback_SaveUserFile(void *buffer, const char *name, int size, void (*callback)(int));
void APICallback_SaveCB(int status);
int APICallback_ReadLeaderboardEntry(int rankID);
void APICallback_NotifyAutoSave_OK(void);
void APICallback_NotifyAutoSave_CB(void);
void APICallback_PromptSavePreference_CB(void);
void APICallback_PromptSavePreference(int status);
void APICallback_LoadUserFile(void *buffer, const char *name, int size, void (*callback)(int));
int APICallback_LoadCB(int status);
int APICallback_LeaderboardStatus(void);
int APICallback_LeaderboardEntryCount(void);
void APICallback_LaunchManual(void *this);
void APICallback_HandleCallback(void);
void APICallback_GetUserAuthStatus(void);
int APICallback_GetStorageStatus(void);
int APICallback_GetSaveStatus(void);
int APICallback_GetControllerType(void *this);
int APICallback_FetchLeaderboardData(byte a1, byte a2, int a3, int a4, int a5, bool32 isUser);
void APICallback_ExitGame(void);
void APICallback_ClearPrerollErrors(void);
bool32 APICallback_CheckInputDisconnected(void);
void APICallback_TrackActClear(byte zoneID, byte actID, byte playerID, int score, int rings, int time);
void APICallback_TrackTAClear(byte zoneID, byte actID, byte playerID, int time);
void APICallback_TrackEnemyDefeat(byte zoneID, byte actID, byte playerID, int entityX, int entityY);
void APICallback_TrackGameProgress(void (*callback)(int), float percent);
void APICallback_TryAuth_No(void);
void APICallback_TryAuth_Yes(void);
void APICallback_TryAuth_CB(void);
int APICallback_TryAuth(void);
void APICallback_TryInitStorage(void);
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
