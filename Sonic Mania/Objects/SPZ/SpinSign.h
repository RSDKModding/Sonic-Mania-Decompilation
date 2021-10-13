#ifndef OBJ_SPINSIGN_H
#define OBJ_SPINSIGN_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    Hitbox hitboxes[4];
    uint16 sfxSignPost;
} ObjectSpinSign;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(stateDraw);
    StateMachine(state);
    uint8 type;
    int timer;
    Animator animator1;
    Animator animator2;
    Animator animator3;
} EntitySpinSign;

// Object Struct
extern ObjectSpinSign *SpinSign;

// Standard Entity Events
void SpinSign_Update(void);
void SpinSign_LateUpdate(void);
void SpinSign_StaticUpdate(void);
void SpinSign_Draw(void);
void SpinSign_Create(void* data);
void SpinSign_StageLoad(void);
void SpinSign_EditorDraw(void);
void SpinSign_EditorLoad(void);
void SpinSign_Serialize(void);

// Extra Entity Functions
void SpinSign_DebugSpawn(void);
void SpinSign_DebugDraw(void);

void SpinSign_Unknown1(void);
void SpinSign_Unknown2(void);

void SpinSign_Unknown3(void);
void SpinSign_Unknown4(void);
void SpinSign_Unknown5(void);
void SpinSign_Unknown6(void);

#endif //!OBJ_SPINSIGN_H
