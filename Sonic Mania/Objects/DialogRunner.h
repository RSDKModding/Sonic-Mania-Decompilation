#ifndef OBJ_DIALOGRUNNER_H
#define OBJ_DIALOGRUNNER_H

#include "../SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
    RSDK_OBJECT
    int field_4;
    int field_8;
    int field_C;
    int field_10;
    int field_14;
    void *entityPtr;
    int field_1C;
    int field_20;
} ObjectDialogRunner;

// Entity Class
typedef struct {
    RSDK_ENTITY
    void (*state)(void);
    int timer;
    void (*callback)(void);
    int field_64;
    int field_68;
    int field_6C;
    int field_70;
    int field_74;
    int field_78;
    int field_7C;
    int field_80;
    int unknownID;
    int field_88;
} EntityDialogRunner;

// Object Struct
extern ObjectDialogRunner *DialogRunner;

// Standard Entity Events
void DialogRunner_Update(void);
void DialogRunner_LateUpdate(void);
void DialogRunner_StaticUpdate(void);
void DialogRunner_Draw(void);
void DialogRunner_Create(void* data);
void DialogRunner_StageLoad(void);
void DialogRunner_EditorDraw(void);
void DialogRunner_EditorLoad(void);
void DialogRunner_Serialize(void);

// Extra Entity Functions
void DialogRunner_HandleCallback(void);
void DialogRunner_NotifyAutoSave_CB(void);
void DialogRunner_NotifyAutoSave(void);
void DialogRunner_Unknown4(void);
void DialogRunner_Unknown5(void);
void DialogRunner_Unknown6(void);
void DialogRunner_Unknown7(int a1, int a2);
void DialogRunner_ManageNotifs(int a1);
void DialogRunner_Wait(int a1);
void DialogRunner_GetNextNotif(void);
bool32 DialogRunner_CheckUnreadNotifs(void);
bool32 DialogRunner_NotifyAutosave(void);
void DialogRunner_GetUserAuthStatus(void);
void DialogRunner_PromptSavePreference(int id);
void DialogRunner_Unknown14(void);
#endif

#endif //!OBJ_DIALOGRUNNER_H
