#ifndef OBJ_APICALLBACK_H
#define OBJ_APICALLBACK_H

#include "../SonicMania.h"

//90% sure this is "DialogRunner" in plus/1.05
#if !RETRO_USE_PLUS
// Object Class
typedef struct {
    RSDK_OBJECT
    int(*LaunchManual)(void *);
    int (*ExitGame)(void);
    void *ClearAchievements;
    int(*UnlockAchievement)(const char *);
    int(*SetRichPresence)(int, char *);
    int (*LoadUserFile)(const char *, void *, size_t, int(*)(int));
    int (*SaveUserFile)(const char *, void *, size_t, int(*)(int));
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
    int statusUnknown2;
    int authFlag;
    int flag;
    int globeSpeedInc;
    int playerFlipFlags;
    int notifyAutoSaveFlag;
    int unknown;
    int leaderboardsStatus;
    int leaderboardEntryCount;
    int rankScore;
    int prevIsUser;
    int prevRank;
    TextInfo entryName;
    int globalRank;
    int score;
    int isUser;
    int controllerIDs[4];
    int controllerCount;
    Entity *entityPtr;
    int userStorageStatus;
    int storageStatusCode;
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
    int(*fileCallback)(int);
    int field_74;
    int field_78;
    int animIDs;
    int field_80;
    int field_84;
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
bool32 APICallback_GetConfirmButtonFlip();
void APICallback_SetNoSave();
void APICallback_SaveUserFile(void *buffer, const char *name, int size, int (*callback)(int));
int APICallback_SaveCB(int status);
int APICallback_ReadLeaderboardEntry(int rankID);
void APICallback_PrompSavePreference(void *this);
void APICallback_LoadUserFile(void *buffer, const char *name, int size, int (*callback)(int));
int APICallback_LoadCB(int status);
int APICallback_LeaderboardStatus();
int APICallback_LeaderboardEntryCount();
void APICallback_LaunchManual(void *this);
void APICallback_HandleCallback();
void APICallback_GetUserAuthStatus();
int APICallback_GetStorageStatus();
void APICallback_GetStatusUnkown3();
int APICallback_GetStatusUnkown2();
int APICallback_GetControllerType(void *this);
int APICallback_FetchLeaderboardData(byte a1, byte a2, int a3, int a4, int a5, int isUser);
void APICallback_ExitGame();
void APICallback_ClearPrerollErrors();
void APICallback_CheckNoSave();
bool32 APICallback_CheckInputDisconnected();
void APICallback_TrackGameProgress(int (*callback)(int), float a2);
int APICallback_TryAuth();
void APICallback_TryInitStorage();
void APICallback_UnlockAchievement(const char *name);

#endif

#endif //!OBJ_APICALLBACK_H
