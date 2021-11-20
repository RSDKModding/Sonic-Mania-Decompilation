#ifndef OBJ_DIALOGRUNNER_H
#define OBJ_DIALOGRUNNER_H

#include "SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
    RSDK_OBJECT
    int32 authForbiddenFlag;
    bool32 signoutFlag;
    int32 unused1;
    int32 unused2;
    bool32 isAutoSaving;
    void *entityPtr;
    int32 unused3;
    int32 unused4;
} ObjectDialogRunner;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    StateMachine(callback);
    int32 field_64;
    int32 field_68;
    int32 field_6C;
    int32 field_70;
    int32 field_74;
    int32 field_78;
    int32 field_7C;
    int32 field_80;
    int32 status;
    bool32 useGenericText;
} EntityDialogRunner;

// Object Struct
extern ObjectDialogRunner *DialogRunner;

// Standard Entity Events
void DialogRunner_Update(void);
void DialogRunner_LateUpdate(void);
void DialogRunner_StaticUpdate(void);
void DialogRunner_Draw(void);
void DialogRunner_Create(void *data);
void DialogRunner_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
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
void DialogRunner_ManageNotifs(int32 success);
void DialogRunner_Wait(int32 success);
void DialogRunner_GetNextNotif(void);
bool32 DialogRunner_CheckUnreadNotifs(void);
bool32 DialogRunner_NotifyAutosave(void);
void DialogRunner_GetUserAuthStatus(void);
void DialogRunner_PromptSavePreference(int32 id);
void DialogRunner_SignedOutCB(void);
#endif

#endif //! OBJ_DIALOGRUNNER_H
