#ifndef OBJ_CONTINUESETUP_H
#define OBJ_CONTINUESETUP_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Animator animator;
    ushort sfxAccept;
    ushort countIndex[10];
    ushort sceneIndex;
} ObjectContinueSetup;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int timer;
    int timer2;
    int countTimer;
    int rotationX;
    int field_6C;
    int colour;
    int dword74;
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
void ContinueSetup_EditorDraw(void);
void ContinueSetup_EditorLoad(void);
void ContinueSetup_Serialize(void);

// Extra Entity Functions
void ContinueSetup_Unknown1(void);
void ContinueSetup_Unknown2(void);
void ContinueSetup_Unknown3(void);
void ContinueSetup_Unknown4(void);

#endif //!OBJ_CONTINUESETUP_H
