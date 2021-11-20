#ifndef OBJ_CONTINUESETUP_H
#define OBJ_CONTINUESETUP_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Animator animator;
    uint16 sfxAccept;
    uint16 countIndex[10];
    uint16 sceneIndex;
} ObjectContinueSetup;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    int32 secondTimer;
    int32 countTimer;
    int32 rotationX;
    int32 unused1;
    int32 numberColour;
    bool32 showContinues;
    Matrix matTranslate;
    Matrix matTranslateFinal;
    Matrix matRotateX;
    Matrix matRotateY;
    Matrix matFinal;
} EntityContinueSetup;

// Object Struct
extern ObjectContinueSetup *ContinueSetup;

// Standard Entity Events
void ContinueSetup_Update(void);
void ContinueSetup_LateUpdate(void);
void ContinueSetup_StaticUpdate(void);
void ContinueSetup_Draw(void);
void ContinueSetup_Create(void* data);
void ContinueSetup_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void ContinueSetup_EditorDraw(void);
void ContinueSetup_EditorLoad(void);
#endif
void ContinueSetup_Serialize(void);

// Extra Entity Functions
void ContinueSetup_State_FadeIn(void);
void ContinueSetup_State_HandleCountdown(void);
void ContinueSetup_State_ContinueGame(void);
void ContinueSetup_State_ReturnToMenu(void);

#endif //!OBJ_CONTINUESETUP_H
