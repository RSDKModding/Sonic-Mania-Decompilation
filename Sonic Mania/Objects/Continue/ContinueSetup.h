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
    int32 timer2;
    int32 countTimer;
    int32 rotationX;
    int32 field_6C;
    int32 colour;
    int32 dword74;
    Matrix matrix1;
    Matrix matrix2;
    Matrix matrix3;
    Matrix matrix4;
    Matrix matrix5;
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
void ContinueSetup_Unknown1(void);
void ContinueSetup_Unknown2(void);
void ContinueSetup_Unknown3(void);
void ContinueSetup_Unknown4(void);

#endif //!OBJ_CONTINUESETUP_H
