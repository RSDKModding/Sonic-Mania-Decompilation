#ifndef OBJ_DIALOGRUNNER_H
#define OBJ_DIALOGRUNNER_H

#include "Game.h"

#if MANIA_USE_PLUS
// Object Class
struct ObjectDialogRunner {
    RSDK_OBJECT
    int32 authForbidden;
    bool32 signedOut;
    int32 unused1;
    int32 unused2;
    bool32 isAutoSaving;
    void *activeCallback;
    int32 unused3;
    int32 unused4;
};

// Entity Class
struct EntityDialogRunner {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    StateMachine(callback);
    int32 unused1;
    int32 unused2;
    int32 unused3;
    int32 unused4;
    int32 unused5;
    int32 unused6;
    int32 unused7;
    int32 unused8;
    int32 status;
    bool32 useGenericText;
};

// Object Struct
extern ObjectDialogRunner *DialogRunner;

// Standard Entity Events
void DialogRunner_Update(void);
void DialogRunner_LateUpdate(void);
void DialogRunner_StaticUpdate(void);
void DialogRunner_Draw(void);
void DialogRunner_Create(void *data);
void DialogRunner_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void DialogRunner_EditorDraw(void);
void DialogRunner_EditorLoad(void);
#endif
void DialogRunner_Serialize(void);

// Extra Entity Functions
void DialogRunner_HandleCallback(void);
void DialogRunner_NotifyAutoSave_CB(void);
void DialogRunner_NotifyAutoSave(void);
void DialogRunner_SetNoSaveDisabled(void);
void DialogRunner_SetNoSaveEnabled(void);
void DialogRunner_PromptSavePreference_CB(void);
void DialogRunner_CheckUserAuth_CB(void);
void DialogRunner_ManageNotifs(void);
void DialogRunner_TrackGameProgressCB(bool32 success);
void DialogRunner_GetNextNotif(void);
bool32 DialogRunner_CheckUnreadNotifs(void);
bool32 DialogRunner_NotifyAutosave(void);
void DialogRunner_GetUserAuthStatus(void);
void DialogRunner_PromptSavePreference(int32 id);
void DialogRunner_CheckUserAuth_OK(void);
#endif

#endif //! OBJ_DIALOGRUNNER_H
