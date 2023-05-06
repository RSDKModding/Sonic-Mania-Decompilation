#ifndef OBJ_BSS_MESSAGE_H
#define OBJ_BSS_MESSAGE_H

#include "Game.h"

typedef enum {
    BSS_MESSAGE_GETSPHERES,
    BSS_MESSAGE_PERFECT,
    BSS_MESSAGE_FINISHED,
} BSS_MessageTypes;

// Object Class
struct ObjectBSS_Message {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityBSS_Message {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    int32 messageFinishTimer;
    bool32 fadeEnabled;
    int32 color;
    bool32 saveInProgress;
    Animator leftAnimator;
    Animator rightAnimator;
};

// Object Struct
extern ObjectBSS_Message *BSS_Message;

// Standard Entity Events
void BSS_Message_Update(void);
void BSS_Message_LateUpdate(void);
void BSS_Message_StaticUpdate(void);
void BSS_Message_Draw(void);
void BSS_Message_Create(void *data);
void BSS_Message_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void BSS_Message_EditorDraw(void);
void BSS_Message_EditorLoad(void);
#endif
void BSS_Message_Serialize(void);

// Extra Entity Functions
void BSS_Message_State_GetBS(void);
void BSS_Message_State_GetBSWait(void);
void BSS_Message_State_Finished(void);
void BSS_Message_State_Perfect(void);
void BSS_Message_State_WaitPerfect(void);
void BSS_Message_State_MsgFinished(void);

#if MANIA_USE_PLUS
void BSS_Message_TrackProgressCB(bool32 success);
#else
void BSS_Message_TrackProgressCB(void);
#endif

void BSS_Message_State_SaveGameProgress(void);
void BSS_Message_State_LoadPrevScene(void);

#endif //! OBJ_BSS_MESSAGE_H
