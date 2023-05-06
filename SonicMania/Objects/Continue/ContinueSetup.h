#ifndef OBJ_CONTINUESETUP_H
#define OBJ_CONTINUESETUP_H

#include "Game.h"

// Object Class
struct ObjectContinueSetup {
    RSDK_OBJECT
    Animator animator;
    uint16 sfxAccept;
    uint16 countIndex[10];
    uint16 sceneIndex;
};

// Entity Class
struct EntityContinueSetup {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    int32 secondTimer;
    int32 countTimer;
    int32 rotationX;
    int32 unused1;
    int32 numberColor;
    bool32 showContinues;
    Matrix matTemp;
    Matrix matTranslate;
    Matrix matRotateX;
    Matrix matRotateY;
    Matrix matFinal;
};

// Object Struct
extern ObjectContinueSetup *ContinueSetup;

// Standard Entity Events
void ContinueSetup_Update(void);
void ContinueSetup_LateUpdate(void);
void ContinueSetup_StaticUpdate(void);
void ContinueSetup_Draw(void);
void ContinueSetup_Create(void *data);
void ContinueSetup_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void ContinueSetup_EditorDraw(void);
void ContinueSetup_EditorLoad(void);
#endif
void ContinueSetup_Serialize(void);

// Extra Entity Functions
void ContinueSetup_State_FadeIn(void);
void ContinueSetup_State_HandleCountdown(void);
void ContinueSetup_State_ContinueGame(void);
void ContinueSetup_State_ReturnToMenu(void);

#endif //! OBJ_CONTINUESETUP_H
